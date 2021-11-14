# [folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/master/folly)/[io](https://github.com/facebook/folly/tree/master/folly/io)/[async](https://github.com/facebook/folly/tree/master/folly/io/async)/**[README.md](https://github.com/facebook/folly/blob/master/folly/io/async/README.md)**

[libevent](https://github.com/libevent/libevent) is an excellent cross-platform eventing library. Folly's async provides C++ object wrappers for fd callbacks and `event_base`, as well as providing implementations for many common types of fd uses.

> NOTE: 
>
> **fd callback**的含义是什么？监控一个fd，一旦有event发生，则触发相应的callback。

## [`EventBase`](https://github.com/facebook/folly/blob/main/folly/io/async/EventBase.h)

The main libevent / epoll loop. 

> NOTE: 
>
> 一、`EventBase` 表示的是 "main libevent / epoll loop"，即main event loop
>
> 二、一个 `EventBase` 管理多个 `AsyncSocket`

Generally there is a single `EventBase` per thread, and once started, nothing else happens on the thread except `fd` callbacks. 

> NOTE: "nothing else happens on the thread except `fd` callbacks"的意思是: "在thread中，仅仅执行 "`fd` callback""，下面的例子就展示了对应的写法

For example:

```C++
EventBase base;
auto thread = std::thread([&](){
  base.loopForever();
});

```

> NOTE: 
>
> 每个thread执行一个event loop，这是否是类似于 "Memcache-concurrent server-event driven-IO multiplexing-multiple reactor-multiple thread"
>
> 



### Message passing

`EventBase` has built-in support for message passing between threads. To send a function to be run in the `EventBase` thread, use [`runInEventBaseThread()`](https://github.com/facebook/folly/blob/main/folly/io/async/EventBase.h#L540).

> NOTE: 它的实现是基于 `NotificationQueue` 的

`EventBase` always calls all callbacks inline - that is, there is no explicit or implicit queuing.

> NOTE: 这段话的意思是: `EventBase` 总是立即调用所有的callback。

## [EventHandler](https://github.com/facebook/folly/blob/main/folly/io/async/EventHandler.h)

> NOTE: 在 [EventHandler](https://github.com/facebook/folly/blob/main/folly/io/async/EventHandler.h) 中的注释是: 
>
> >  The EventHandler class is used to asynchronously wait for events on a **file descriptor**.
>
> 

`EventHandler` is the object wrapper for fd's. Any class you wish to receive callbacks on will inherit from `EventHandler`. `registerHandler(EventType)` will register to receive events of a specific type.

## Implementations of EventHandler

### [AsyncSocket](https://github.com/facebook/folly/blob/main/folly/io/async/AsyncSocket.h)

A nonblocking socket implementation. 

#### Write

Writes are queued and written asynchronously, even before connect() is successful.

#### Read 

The `read` api consists of two methods: `getReadBuffer()` and `readDataAvailable()`. When the **READ event** is signaled, `libevent` has no way of knowing how much data is available to read. In some systems (linux), we *could* make another syscall to get the data size in the kernel read buffer, but syscalls are slow. Instead, most users will just want to provide a fixed size buffer in `getReadBuffer()`, probably using the IOBufQueue in folly/io. readDataAvailable() will then describe exactly how much data was read.

### [AsyncServerSocket](https://github.com/facebook/folly/blob/main/folly/io/async/AsyncServerSocket.h)

A listen()ing socket that accept()s fds, and passes them to other event bases.

> NOTE: 
>
> "Memcache-concurrent server-event driven-IO multiplexing-multiple reactor-multiple thread"
>
> 每个event base管理多个fd

The general pattern is:

```C++
EventBase base;
auto socket = AsyncServerSocket::newSocket(&base);
socket->bind(port); // 0 to choose any free port
socket->addAcceptCallback(object, &base); // where object is the object that implements the accept callback, and base is the object's eventbase.  base::runInEventBaseThread() will be called to send it a message.
socket->listen(backlog);
socket->startAccepting();
```

Generally there is a single accept() thread, and multiple `AcceptCallback` objects. The Acceptee objects then will manage the individual AsyncSockets. While AsyncSockets *can* be moved between event bases, most users just tie them to a single event base to get better cache locallity, and to avoid locking.

> NOTE: 
>
> 线程定位

Multiple ServerSockets can be made, but currently the linux kernel has a lock on accept()ing from a port, preventing more than ~20k accepts / sec. There are various workarounds (`SO_REUSEPORT`), but generally clients should be using connection pooling instead when possible.

> NOTE: 
>
> 这段话要如何理解？



### `NotificationQueue` (EventFD or pipe notifications)

> NOTE: 
>
> notify-via-fd-NotificationQueue(ITC EventFD or IPC pipe)

`NotificationQueue` is used to send messages between threads in the *same process*. It is what backs `EventBase::runInEventBaseThread()`, so it is unlikely you'd want to use it directly instead of using `runInEventBaseThread()`.





### AsyncTimeout



## Helper Classes

### RequestContext (in Request.h)

Since messages are frequently passed between threads with `runInEventBaseThread()`, ThreadLocals don't work for messages. Instead, `RequestContext` can be used, which is saved/restored between threads. 

### DelayedDestruction

Since `EventBase` callbacks already have the `EventHandler` and `EventBase` on the stack, calling `delete` on either of these objects would most likely result in a segfault. Instead, these objects inherit from `DelayedDestruction`, which provides reference counting in the callbacks. Instead of delete, `destroy()` is called, which notifies that is ready to be destroyed. In each of the callbacks there is a `DestructorGuard`, which prevents destruction until all the Guards are gone from the stack, when the actual delete method is called.

`DelayedDestruction` can be a painful to use, since `shared_ptrs` and `unique_ptrs` need to have a special `DelayedDestruction` destructor type. It's also pretty easy to forget to add a `DestructorGuard` in code that calls callbacks. But it is well worth it to avoid queuing callbacks, and the improved P99 times as a result.

### DestructorCheck



## Generic Multithreading Advice

Facebook has a lot of experience running services. For background reading, see [The C10k problem](http://www.kegel.com/c10k.html) and [Fast UNIX servers](http://nick-black.com/dankwiki/index.php/Fast_UNIX_Servers)

Some best practices we've found:

1、It's much easier to maintain latency expectations when each EventBase thread is used for only a single purpose: AsyncServerSocket, or inbound AsyncSocket, or in proxies, outbound AsyncSocket calls. In a perfect world, one EventBase per thread per core would be enough, but the implementor needs to be extremely diligent to make sure all CPU work is moved off of the IO threads to prevent slow read/write/closes of fds.

2、**ANY** work that is CPU intensive should be offloaded to a pool of CPU-bound threads, instead of being done in the EventBase threads. runInEventBaseThread() is fast: It can be called millions of times per second before the spinlock becomes an issue - so passing the request off to a different thread is probably fine perf wise.

3、In contrast to the first two recommendations, if there are more total threads than cores, context switching overhead can become an issue. In particular we have seen this be an issue when a CPU-intensive thread blocks the scheduling of an IO thread, using the linux `perf sched` tool.

4、For async programming, in contrast to synchronous systems, managing load is extremely hard - it is better to use out-of-band methods to notify of overload, such as timeouts, or CPU usage. For sync systems, you are almost always limited by the number of threads. For more details see [No Time for Asynchrony](https://www.usenix.org/legacy/event/hotos09/tech/full_papers/aguilera/aguilera.pdf)
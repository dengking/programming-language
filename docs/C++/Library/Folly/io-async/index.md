# [folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/master/folly)/[io](https://github.com/facebook/folly/tree/master/folly/io)/[async](https://github.com/facebook/folly/tree/master/folly/io/async)/**[README.md](https://github.com/facebook/folly/blob/master/folly/io/async/README.md)**

[libevent](https://github.com/libevent/libevent) is an excellent cross-platform eventing library. Folly's async provides C++ object wrappers for fd callbacks and event_base, as well as providing implementations for many common types of fd uses.

## `EventBase`

The main libevent / epoll loop. 

> NOTE: 
>
> 一、`EventBase` 表示的是 "main libevent / epoll loop"，即main event loop
>
> 二、一个 `EventBase` 管理多个 `AsyncSocket`

Generally there is a single `EventBase` per thread, and once started, nothing else happens on the thread except fd callbacks. For example:

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





## EventHandler

`EventHandler` is the object wrapper for fd's. Any class you wish to receive callbacks on will inherit from EventHandler. `registerHandler(EventType)` will register to receive events of a specific type.

## Implementations of EventHandler

### AsyncSocket

A listen()ing socket that accept()s fds, and passes them to other event bases.



### AsyncServerSocket

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

## Generic Multithreading Advice

Facebook has a lot of experience running services. For background reading, see [The C10k problem](http://www.kegel.com/c10k.html) and [Fast UNIX servers](http://nick-black.com/dankwiki/index.php/Fast_UNIX_Servers)

Some best practices we've found:

1、It's much easier to maintain latency expectations when each EventBase thread is used for only a single purpose: AsyncServerSocket, or inbound AsyncSocket, or in proxies, outbound AsyncSocket calls. In a perfect world, one EventBase per thread per core would be enough, but the implementor needs to be extremely diligent to make sure all CPU work is moved off of the IO threads to prevent slow read/write/closes of fds.

2、**ANY** work that is CPU intensive should be offloaded to a pool of CPU-bound threads, instead of being done in the EventBase threads. runInEventBaseThread() is fast: It can be called millions of times per second before the spinlock becomes an issue - so passing the request off to a different thread is probably fine perf wise.

3、In contrast to the first two recommendations, if there are more total threads than cores, context switching overhead can become an issue. In particular we have seen this be an issue when a CPU-intensive thread blocks the scheduling of an IO thread, using the linux `perf sched` tool.

4、For async programming, in contrast to synchronous systems, managing load is extremely hard - it is better to use out-of-band methods to notify of overload, such as timeouts, or CPU usage. For sync systems, you are almost always limited by the number of threads. For more details see [No Time for Asynchrony](https://www.usenix.org/legacy/event/hotos09/tech/full_papers/aguilera/aguilera.pdf)
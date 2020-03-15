# [Single Threaded Event Loop vs Multi Threaded Non Blocking Worker in Node.JS](https://stackoverflow.com/questions/21485920/single-threaded-event-loop-vs-multi-threaded-non-blocking-worker-in-node-js)

Node.JS biggest advantage is it's non blocking nature. It's single threaded, so it doesn't need to spawn a new thread for each new incoming connection.

Behind the **event-loop** (which is in fact **single threaded**), there is a "Non blocking Worker". This thing is not single threaded anymore, so (as far as I understood) it can spawn a new thread for each task.

Maybe I misunderstood something, but where exactly is the advantage. If there are to many tasks to handle, wouldn't be the Non Blocking Working turn into a Blocking Worker?

Thanks Christian

## [A](https://stackoverflow.com/a/21488067)

[You need to read about libuv](http://nikhilm.github.io/uvbook/), the "magic" behind node's non-blocking I/O.

The important thing to take away from the libuv book is that libuv uses the host OS's [asynchronous](http://en.wikipedia.org/wiki/Epoll)[I/O](http://en.wikipedia.org/wiki/Kqueue) [facilities](http://en.wikipedia.org/wiki/IOCP); **it does not simply create a new thread** for every connection.

libuv tells the OS that it would like to know about any changes (connection state, data received, etc) that happen on **a particular set of sockets**. It is then up to the OS to deal with managing the connections. The OS itself may create one or more threads to accomplish that, but that's not our concern. (And it certainly won't create a thread for every connection.)

For other types of operations like calls to C libraries that may be computationally expensive (ie crypto), libuv provides a *thread pool* on which those operations may run. Since it is a thread pool, again you don't have to worry about thread count growing without bound. When the pool is fully busy, operations are queued.

So yes, JavaScript runs on a **single thread**. Yes, node (via libuv) spawns many threads in the background to do work so that it need not block the JavaScript thread. However, the **thread count** is always controlled, and I/O generally doesn't even get its own node-allocated thread because that's handled by the OS.



## [A](https://stackoverflow.com/a/21486005)

Alright, let's break this down a bit. Single threaded applications have advantages: you can never get deadlocks or race conditions. These issues stem from simultaneous memory access in multi-threaded systems. If two threads access the same piece of information weird things can happen.

So why does JavaScript have Workers? If you need do some heavy processing you're going to block the event loop, you could try to split up the workload by generating timer events but that's tedious. A Worker allows you to spawn a thread under one condition: **no shared memory access**. This solves the issue of heavy processing in a single threaded environment while avoiding the pitfalls of multi-threaded environments (deadlocks, race-conditions).

And as @dandavis said, if you have a multi-core CPU (which everyone does these days) the Worker threads can be offloaded to the other cores.

You have to appreciate that, although JavaScript is single threaded, the environment around it is still very much multi-threaded. Reading a file is non-blocking in Node.JS but there is likely a thread to support it in the OS.

------

As a minor addendum I would say that Node.JS's biggest advantage is that it allows you to write JavaScript on the server, which allows you to **share code** between the client and the server. The fact that it's non-blocking is nice but threads already solve that. The non-blocking IO stems from the single threaded-ness. It's very inconvenient to have a single thread with blocking IO.



# [Threads vs Event Loop, Again](http://troglobit.com/2017/09/19/threads-vs-event-loop-again-.../) 

I still get asked this, from time to time. Maybe it’s because I only use **event loops**, maybe it’s because I’ve written [libuEv](https://github.com/troglobit/libuev), or maybe people still don’t understand:

> Why an event loop, why not use threads?

So here’s my response, once more.

With the advent of light-weight processes (threads) programmers these days have a [golden hammer](http://c2.com/cgi/wiki?GoldenHammer) they often swing without consideration. Event loops and non-blocking I/O is often a far easier approach, as well as less error prone.

The purpose of many applications is, with a little logic sprinkled on top, to act on network packets entering an interface, timeouts expiring, mouse clicks, or other types of events. Such applications are often very well suited to use an **event loop**.

***SUMMARY*** : 适合event loop的情形

Applications that need to churn massively **parallel algorithms** are more suitable for running multiple (independent) threads on several CPU cores. However, threaded applications must deal with the side effects of concurrency, like race conditions, deadlocks, live locks, etc. Writing error free threaded applications is hard, debugging them can be even harder.

***SUMMARY*** : 适合multiple threads的情形

Sometimes the combination of multiple threads *and* an event loop per thread can be the best approach, but each application of course needs to be broken down individually to find the most optimal approach. Do keep in mind, however, that not all systems your application will run on have multiple CPU cores – some small embedded systems still use a single CPU core, even though they run Linux, with multiple threads a program may actually run slower! Always profile your program, and if possible, test it on different architectures.

***SUMMARY*** : combine event loop and multiple threads


# 20190710

现在想想，其实这个问题更应该是reactor和multithread之间的异与同
# [libevent – an event notification library](http://libevent.org/)

The `libevent` API provides a mechanism to execute a callback function when a specific **event** occurs on a **file descriptor** or after a timeout has been reached. Furthermore, `libevent` also support **callbacks** due to **signals** or regular **timeouts**.

`libevent` is meant to replace the **event loop** found in **event driven network servers**. An application just needs to call *event_dispatch()* and then add or remove events dynamically without having to change the **event loop**.

***SUMMARY***:For **event driven network servers**, please refer to [this article](https://en.wikipedia.org/wiki/Event-driven_programming) .



Currently, `libevent` supports `/dev/poll`, `kqueue(2)`, `event ports`, [POSIX *select(2)*](http://manpages.debian.net/cgi-bin/man.cgi?query=select), [Windows *select()*](http://msdn.microsoft.com/en-us/library/ms740141(v=vs.85).aspx), [*poll(2)*](http://manpages.debian.net/cgi-bin/man.cgi?query=poll), and `epoll(4)`. The internal event mechanism is completely independent of the exposed event API, and a simple update of libevent can provide new functionality without having to redesign the applications. As a result, `libevent` allows for portable application development and provides the most scalable event notification mechanism available on an operating system. 

***SUMMARY***:obviously,the implementation of `libevent` is very good,which conforms to the idea of interface-oriented programming.



`Libevent` can also be used for multi-threaded applications, either by isolating each `event_base` so that only a single thread accesses it, or by locked access to a single shared event_base. `libevent` should compile on Linux, *BSD, Mac OS X, Solaris, Windows, and more.



`Libevent` additionally provides a sophisticated framework for buffered network IO, with support for sockets, filters, rate-limiting, `SSL`, zero-copy file transmission, and `IOCP`. `Libevent` includes support for several useful protocols, including `DNS`, HTTP, and a minimal RPC framework.



More information about event notification mechanisms for network servers can be found on Dan Kegel's "[The C10K problem](http://www.kegel.com/c10k.html)" web page.




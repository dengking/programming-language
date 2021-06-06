# [folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/master/folly)/[io](https://github.com/facebook/folly/tree/master/folly/io)/[async](https://github.com/facebook/folly/tree/master/folly/io/async)/**[README.md](https://github.com/facebook/folly/blob/master/folly/io/async/README.md)**

[libevent](https://github.com/libevent/libevent) is an excellent cross-platform eventing library. Folly's async provides C++ object wrappers for fd callbacks and event_base, as well as providing implementations for many common types of fd uses.



## Generic Multithreading Advice

Facebook has a lot of experience running services. For background reading, see [The C10k problem](http://www.kegel.com/c10k.html) and [Fast UNIX servers](http://nick-black.com/dankwiki/index.php/Fast_UNIX_Servers)

Some best practices we've found:

1、It's much easier to maintain latency expectations when each EventBase thread is used for only a single purpose: AsyncServerSocket, or inbound AsyncSocket, or in proxies, outbound AsyncSocket calls. In a perfect world, one EventBase per thread per core would be enough, but the implementor needs to be extremely diligent to make sure all CPU work is moved off of the IO threads to prevent slow read/write/closes of fds.

2、**ANY** work that is CPU intensive should be offloaded to a pool of CPU-bound threads, instead of being done in the EventBase threads. runInEventBaseThread() is fast: It can be called millions of times per second before the spinlock becomes an issue - so passing the request off to a different thread is probably fine perf wise.

3、In contrast to the first two recommendations, if there are more total threads than cores, context switching overhead can become an issue. In particular we have seen this be an issue when a CPU-intensive thread blocks the scheduling of an IO thread, using the linux `perf sched` tool.

4、For async programming, in contrast to synchronous systems, managing load is extremely hard - it is better to use out-of-band methods to notify of overload, such as timeouts, or CPU usage. For sync systems, you are almost always limited by the number of threads. For more details see [No Time for Asynchrony](https://www.usenix.org/legacy/event/hotos09/tech/full_papers/aguilera/aguilera.pdf)
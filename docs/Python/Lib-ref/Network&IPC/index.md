# [Networking and Interprocess Communication](https://docs.python.org/3/library/ipc.html)

> NOTE: 
>
> network也是一种IPC

Some modules only work for two processes that are on the same machine, e.g. [`signal`](https://docs.python.org/3/library/signal.html#module-signal) and [`mmap`](https://docs.python.org/3/library/mmap.html#module-mmap). Other modules support networking protocols that two or more processes can use to communicate across machines.
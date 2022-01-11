# Executor

一、运行task，典型的 "closure-task-first-class-function-based-concurrent"。

二、无论是future、eventbase，都需要描述它们如何运行，而executor就是对如何运行的抽象。

每个eventbase都是要和thread进行bind的。

## [folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[docs](https://github.com/facebook/folly/tree/main/folly/docs)/**[Executors.md](https://github.com/facebook/folly/blob/main/folly/docs/Executors.md)**

> NOTE: 
>
> 用于运行task



### Why do we need several different types of thread pools?

> NOTE: 
>
> 
>
> |                         | notification | queue                                                        |
> | ----------------------- | ------------ | ------------------------------------------------------------ |
> | `IOThreadPoolExecutor`  | `event_fd`   | one queue (`NotificationQueue` specifically) per thread/epoll. |
> | `CPUThreadPoolExecutor` | semaphore    | single queue                                                 |
>
> 



### [`IOThreadPoolExecutor`](https://github.com/facebook/folly/blob/main/folly/executors/IOThreadPoolExecutor.h) 



### [`CPUThreadPoolExecutor`](https://github.com/facebook/folly/blob/main/folly/executors/CPUThreadPoolExecutor.h) 





## Source code



[folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[executors](https://github.com/facebook/folly/tree/main/folly/executors)/


# Executor

运行task。

## [folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[docs](https://github.com/facebook/folly/tree/main/folly/docs)/**[Executors.md](https://github.com/facebook/folly/blob/main/folly/docs/Executors.md)**

> NOTE: 
>
> 用于运行task



### Why do we need several different types of thread pools?

> NOTE: 
>
> 
>
> |                         | notification |                                                              |
> | ----------------------- | ------------ | ------------------------------------------------------------ |
> | `IOThreadPoolExecutor`  | `event_fd`   | one queue (`NotificationQueue` specifically) per thread/epoll. |
> | `CPUThreadPoolExecutor` | semaphore    | single queue                                                 |
>
> 



### [`IOThreadPoolExecutor`](https://github.com/facebook/folly/blob/main/folly/executors/IOThreadPoolExecutor.h) 



### [`CPUThreadPoolExecutor`](https://github.com/facebook/folly/blob/main/folly/executors/CPUThreadPoolExecutor.h) 



## Source code

[folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[Executor.h](https://github.com/facebook/folly/blob/main/folly/Executor.h)

[folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[executors](https://github.com/facebook/folly/tree/main/folly/executors)/


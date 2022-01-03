# Task queue

[folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[**MPMCQueue.h**](https://github.com/facebook/folly/blob/main/folly/MPMCQueue.h) 是各种task queue的底层结构。



## interface

[folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[executors](https://github.com/facebook/folly/tree/main/folly/executors)/[task_queue](https://github.com/facebook/folly/tree/main/folly/executors/task_queue)/[**BlockingQueue.h**](https://github.com/facebook/folly/blob/main/folly/executors/task_queue/BlockingQueue.h)



## Implementation

1、[folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[executors](https://github.com/facebook/folly/tree/main/folly/executors)/[task_queue](https://github.com/facebook/folly/tree/main/folly/executors/task_queue)/[**LifoSemMPMCQueue.h**](https://github.com/facebook/folly/blob/main/folly/executors/task_queue/LifoSemMPMCQueue.h)

2、[folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[executors](https://github.com/facebook/folly/tree/main/folly/executors)/[task_queue](https://github.com/facebook/folly/tree/main/folly/executors/task_queue)/[**PriorityLifoSemMPMCQueue.h**](https://github.com/facebook/folly/blob/main/folly/executors/task_queue/PriorityLifoSemMPMCQueue.h)

3、[folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[executors](https://github.com/facebook/folly/tree/main/folly/executors)/[task_queue](https://github.com/facebook/folly/tree/main/folly/executors/task_queue)/[**UnboundedBlockingQueue.h**](https://github.com/facebook/folly/blob/main/folly/executors/task_queue/UnboundedBlockingQueue.h)

4、[folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[executors](https://github.com/facebook/folly/tree/main/folly/executors)/[task_queue](https://github.com/facebook/folly/tree/main/folly/executors/task_queue)/[**PriorityUnboundedBlockingQueue.h**](https://github.com/facebook/folly/blob/main/folly/executors/task_queue/PriorityUnboundedBlockingQueue.h)



## draft

```
queue D:\GitHub-src\folly
```



```C++
D:\GitHub-src\folly\folly\executors\task_queue
D:\GitHub-src\folly\folly\io\async\AtomicNotificationQueue-inl.h
D:\GitHub-src\folly\folly\io\async\AtomicNotificationQueue.h
D:\GitHub-src\folly\folly\io\async\test\AtomicNotificationQueueTest.cpp
D:\GitHub-src\folly\folly\experimental\channels\detail\AtomicQueue.h
D:\GitHub-src\folly\folly\experimental\channels\detail\test\AtomicQueueTest.cpp
D:\GitHub-src\folly\folly\executors\task_queue\BlockingQueue.h
D:\GitHub-src\folly\folly\concurrency\DynamicBoundedQueue.h
D:\GitHub-src\folly\folly\concurrency\test\DynamicBoundedQueueTest.cpp
D:\GitHub-src\folly\folly\io\async\EventBaseAtomicNotificationQueue-inl.h
D:\GitHub-src\folly\folly\io\async\EventBaseAtomicNotificationQueue.h
D:\GitHub-src\folly\folly\experimental\FlatCombiningPriorityQueue.h
D:\GitHub-src\folly\folly\experimental\test\FlatCombiningPriorityQueueTest.cpp
D:\GitHub-src\folly\folly\io\IOBufQueue.cpp
D:\GitHub-src\folly\folly\io\IOBufQueue.h
D:\GitHub-src\folly\folly\io\test\IOBufQueueTest.cpp
D:\GitHub-src\folly\folly\executors\task_queue\LifoSemMPMCQueue.h
D:\GitHub-src\folly\folly\MPMCQueue.h
D:\GitHub-src\folly\folly\test\MPMCQueueTest.cpp
D:\GitHub-src\folly\folly\io\async\NotificationQueue.h
D:\GitHub-src\folly\folly\io\async\test\NotificationQueueBenchmark.cpp
D:\GitHub-src\folly\folly\io\async\test\NotificationQueueTest.cpp
D:\GitHub-src\folly\folly\executors\task_queue\PriorityLifoSemMPMCQueue.h
D:\GitHub-src\folly\folly\executors\task_queue\test\PriorityLifoSemMPMCQueueTest.cpp
D:\GitHub-src\folly\folly\executors\task_queue\PriorityUnboundedBlockingQueue.h
D:\GitHub-src\folly\folly\executors\task_queue\test\PriorityUnboundedBlockingQueueTest.cpp
D:\GitHub-src\folly\folly\concurrency\PriorityUnboundedQueueSet.h
D:\GitHub-src\folly\folly\concurrency\test\PriorityUnboundedQueueSetTest.cpp
D:\GitHub-src\folly\folly\ProducerConsumerQueue.h
D:\GitHub-src\folly\folly\docs\ProducerConsumerQueue.md
D:\GitHub-src\folly\folly\test\ProducerConsumerQueueBenchmark.cpp
D:\GitHub-src\folly\folly\test\ProducerConsumerQueueTest.cpp
D:\GitHub-src\folly\folly\io\test\QueueAppenderBenchmark.cpp
D:\GitHub-src\folly\folly\executors\QueuedImmediateExecutor.cpp
D:\GitHub-src\folly\folly\executors\QueuedImmediateExecutor.h
D:\GitHub-src\folly\folly\executors\QueueObserver.cpp
D:\GitHub-src\folly\folly\executors\QueueObserver.h
D:\GitHub-src\folly\folly\experimental\RelaxedConcurrentPriorityQueue.h
D:\GitHub-src\folly\folly\experimental\test\RelaxedConcurrentPriorityQueueTest.cpp
D:\GitHub-src\folly\folly\experimental\coro\SmallUnboundedQueue.h
D:\GitHub-src\folly\folly\experimental\coro\test\SmallUnboundedQueueTest.cpp
D:\GitHub-src\folly\folly\TimeoutQueue.cpp
D:\GitHub-src\folly\folly\TimeoutQueue.h
D:\GitHub-src\folly\folly\test\TimeoutQueueTest.cpp
D:\GitHub-src\folly\folly\executors\task_queue\UnboundedBlockingQueue.h
D:\GitHub-src\folly\folly\executors\task_queue\test\UnboundedBlockingQueueBench.cpp
D:\GitHub-src\folly\folly\executors\task_queue\test\UnboundedBlockingQueueTest.cpp
D:\GitHub-src\folly\folly\concurrency\UnboundedQueue.h
D:\GitHub-src\folly\folly\experimental\coro\UnboundedQueue.h
D:\GitHub-src\folly\folly\concurrency\test\UnboundedQueueTest.cpp
D:\GitHub-src\folly\folly\experimental\coro\test\UnboundedQueueTest.cpp
```


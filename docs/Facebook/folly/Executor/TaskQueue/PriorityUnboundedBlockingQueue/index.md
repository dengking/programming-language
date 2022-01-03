# [folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[executors](https://github.com/facebook/folly/tree/main/folly/executors)/[task_queue](https://github.com/facebook/folly/tree/main/folly/executors/task_queue)/[**PriorityUnboundedBlockingQueue.h**](https://github.com/facebook/folly/blob/main/folly/executors/task_queue/PriorityUnboundedBlockingQueue.h)



https://gitee.com/mirrors/folly/blob/main/folly/executors/task_queue/PriorityUnboundedBlockingQueue.h

```C++
#include <folly/ConstexprMath.h>
#include <folly/Executor.h>
#include <folly/concurrency/PriorityUnboundedQueueSet.h>
#include <folly/executors/task_queue/BlockingQueue.h>
#include <folly/lang/Exception.h>
#include <folly/synchronization/LifoSem.h>


template <class T>
class PriorityUnboundedBlockingQueue : public BlockingQueue<T> 
{
private:
  LifoSem sem_;
  PriorityUMPMCQueueSet<T, /* MayBlock = */ true> queue_;
};

```
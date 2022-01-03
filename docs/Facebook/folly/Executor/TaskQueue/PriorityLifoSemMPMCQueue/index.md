# [folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[executors](https://github.com/facebook/folly/tree/main/folly/executors)/[task_queue](https://github.com/facebook/folly/tree/main/folly/executors/task_queue)/[**PriorityLifoSemMPMCQueue.h**](https://github.com/facebook/folly/blob/main/folly/executors/task_queue/PriorityLifoSemMPMCQueue.h)

>https://gitee.com/mirrors/folly/blob/main/folly/executors/task_queue/PriorityLifoSemMPMCQueue.h

```C++
#include <folly/Executor.h>
#include <folly/MPMCQueue.h>
#include <folly/Range.h>
#include <folly/executors/task_queue/BlockingQueue.h>
#include <folly/synchronization/LifoSem.h>

template <class T, QueueBehaviorIfFull kBehavior = QueueBehaviorIfFull::THROW>
class PriorityLifoSemMPMCQueue : public BlockingQueue<T> 
{


private:
  folly::LifoSem sem_;
  std::vector<folly::MPMCQueue<T>> queues_;
};


```
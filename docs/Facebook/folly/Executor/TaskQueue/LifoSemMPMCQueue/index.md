# [folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[executors](https://github.com/facebook/folly/tree/main/folly/executors)/[task_queue](https://github.com/facebook/folly/tree/main/folly/executors/task_queue)/[**LifoSemMPMCQueue.h**](https://github.com/facebook/folly/blob/main/folly/executors/task_queue/LifoSemMPMCQueue.h)

> https://gitee.com/mirrors/folly/blob/main/folly/executors/task_queue/LifoSemMPMCQueue.h




```C++
#include <folly/MPMCQueue.h>
#include <folly/executors/task_queue/BlockingQueue.h>
#include <folly/synchronization/LifoSem.h>


template <class T, QueueBehaviorIfFull kBehavior = QueueBehaviorIfFull::THROW>
class LifoSemMPMCQueue : public BlockingQueue<T>
{
    
    
private:
  folly::LifoSem sem_;
  folly::MPMCQueue<T> queue_;    
};

```
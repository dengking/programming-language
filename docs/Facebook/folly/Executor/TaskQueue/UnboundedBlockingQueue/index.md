# [folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[executors](https://github.com/facebook/folly/tree/main/folly/executors)/[task_queue](https://github.com/facebook/folly/tree/main/folly/executors/task_queue)/[**UnboundedBlockingQueue.h**](https://github.com/facebook/folly/blob/main/folly/executors/task_queue/UnboundedBlockingQueue.h)

https://gitee.com/mirrors/folly/blob/main/folly/executors/task_queue/UnboundedBlockingQueue.h

```C++
#include <folly/concurrency/UnboundedQueue.h>
#include <folly/executors/task_queue/BlockingQueue.h>
#include <folly/synchronization/LifoSem.h>

template <class T>
class UnboundedBlockingQueue : public BlockingQueue<T> 
{
    
    
private:
  LifoSem sem_;
  UMPMCQueue<T, false, 6> queue_;    
}



```


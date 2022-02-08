# Threading and Tasks in Chrome

## chromium [Design Documents](https://www.chromium.org/developers/design-documents)

## github [Threading and Tasks in Chrome](https://github.com/chromium/chromium/blob/master/docs/threading_and_tasks.md)

Chrome has a [multi-process architecture](https://www.chromium.org/developers/design-documents/multi-process-architecture) and each process is heavily **multi-threaded**. In this document we will go over the basic **threading system** shared by each process. The main goal is to keep the main thread (a.k.a. "UI" thread in the browser process) and IO thread (each process's thread for receiving [IPC](https://en.wikipedia.org/wiki/Inter-process_communication)) **responsive**. This means **offloading** any blocking I/O or other expensive operations to other threads. Our approach is to use message passing as the way of communicating between threads. We discourage locking and thread-safe objects. Instead, objects live on only one (often **virtual** -- we'll get to that later!) thread and we pass messages between those threads for communication. Absent external requirements about latency or workload, Chrome attempts to be a [highly concurrent, but not necessarily parallel](https://stackoverflow.com/questions/1050222/what-is-the-difference-between-concurrency-and-parallelism#:~:text=Concurrency is when two or,e.g.%2C on a multicore processor.), system.

> NOTE: 
>
> 一、**responsive** 要如何理解？
>
> "responsive"的含义是 "可以响应的"，显然，这是浏览器的一个非常重要的要求
>
> 二、**offloading** 如何理解？
>
> "offload"的字面意思是"卸载"，那如何实现的呢？
>
> > Our approach is to use message passing as the way of communicating between threads
>
> 三、 "We discourage locking and thread-safe objects.  Instead, objects live on only one (often virtual -- we'll get to that later!) thread and we pass messages between those threads for communication. "
>
> 不推荐使用"locking "和 "thread-safe object"，
>
> "objects live on only one (often **virtual** -- we'll get to that later!) thread "的含义是什么呢？
>
> 在 "Threading Lexicon" 章节中，有这样的解释:
>
> > Typically thread-unsafe types require that all tasks accessing their state be posted to the same `base::SequencedTaskRunner` and they verify this in debug builds with a `SEQUENCE_CHECKER` member. Locks are also an option to synchronize access but in Chrome we strongly [prefer sequences to locks](https://github.com/chromium/chromium/blob/master/docs/threading_and_tasks.md#Using-Sequences-Instead-of-Locks).
>
> 



### Core Concepts

**Sequence** or **Virtual thread**: A chrome-managed thread of execution. Like a physical thread, only one task can run on a given sequence / virtual thread at any given moment and each task sees the side-effects of the preceding tasks. Tasks are executed sequentially but may hop physical threads between each one.

> NOTE: 
>
> "**Sequence** or **Virtual thread**"的执行单位是 **task**，类似的 physical thread的执行单位 **指令**
>
> "Tasks are executed sequentially but may hop physical threads between each one"
>
> "**Sequence** or **Virtual thread**"的task是顺序执行的，但是在具体的实现上，tasks是可能由多个thread执行的



| Concept                       | implementation                 |
| ----------------------------- | ------------------------------ |
| **Task runner**               | `base::TaskRunner`             |
| **Sequenced task runner**     | `base::SequencedTaskRunner`    |
| **Single-thread task runner** | `base::SingleThreadTaskRunner` |


### Threading Lexicon

**Thread-unsafe**: The vast majority of types in Chrome are thread-unsafe (by design). Access to such types/methods must be externally synchronized. Typically thread-unsafe types require that all tasks accessing their state be posted to the same `base::SequencedTaskRunner` and they verify this in debug builds with a `SEQUENCE_CHECKER` member. Locks are also an option to synchronize access but in Chrome we strongly [prefer sequences to locks](https://github.com/chromium/chromium/blob/master/docs/threading_and_tasks.md#Using-Sequences-Instead-of-Locks).

> NOTE: 
>
> 一、提供了一种新的思路
>
> 二、关于 "debug builds"，参见 [chromium](https://github.com/chromium/chromium)/[build](https://github.com/chromium/chromium/tree/master/build)/**[build_config.h](https://github.com/chromium/chromium/blob/master/build/build_config.h)**

**Thread-affine**: Such types/methods need to be always accessed from the same physical thread (i.e. from the same `base::SingleThreadTaskRunner`) and typically have a `THREAD_CHECKER` member to verify that they are. Short of using a third-party API or having a leaf dependency which is thread-affine: there's pretty much no reason for a type to be thread-affine in Chrome. Note that `base::SingleThreadTaskRunner` is-a `base::SequencedTaskRunner` so thread-affine is a subset of thread-unsafe. Thread-affine is also sometimes referred to as **thread-hostile**.

> NOTE: 
>
> "affine" 即 "仿射性"，它的含义可以从CPU affinity进行类推。显然"thread-affine"意味着同一个thread执行
>
> "there's pretty much no reason for a type to be thread-affine in Chrome"的意思是:
>
> > 在Chrome中，几乎没有理由让一个类型是线程仿射的
>
> 

### Tasks

A group of tasks can be executed in one of the following ways:

1、[Parallel](https://github.com/chromium/chromium/blob/master/docs/threading_and_tasks.md#Posting-a-Parallel-Task): No task execution ordering, possibly all at once on any thread

2、[Sequenced](https://github.com/chromium/chromium/blob/master/docs/threading_and_tasks.md#Posting-a-Sequenced-Task): Tasks executed in posting order, one at a time on any thread.

3、Single Threaded: Tasks executed in posting order, one at a time on a single thread.

- [COM Single Threaded](https://github.com/chromium/chromium/blob/master/docs/threading_and_tasks.md#Posting-Tasks-to-a-COM-Single_Thread-Apartment-STA_Thread-Windows): A variant of single threaded with COM initialized.

### Cancelling a Task

#### Using `base::WeakPtr`

[`base::WeakPtr`](https://cs.chromium.org/chromium/src/base/memory/weak_ptr.h) can be used to ensure that any callback bound to an object is canceled when that object is destroyed.

### [Keeping the Browser Responsive](https://github.com/chromium/chromium/blob/master/docs/threading_and_tasks.md#keeping-the-browser-responsive)

> NOTE: 
>
> 在 csdn [Chormium线程模型及应用指南](https://blog.csdn.net/horkychen/article/details/50486062) 中，有非常好的解释:
>
> 设计上遵循以下原则：
>
> 1 不要在UI线程做任何阻塞式的I/O操作，以及其它耗时的操作，通过消息传递把各种操作传给相应用途的线程去做。
>
> 2 不鼓励线程加锁机制和线程安全对象。对象只存在一个线程，通过消息来实现线程之间的通信，线程之间不互相阻塞。通过callback对象实现跨线程请求。
>

### Internals

#### SequenceManager

[SequenceManager](https://cs.chromium.org/chromium/src/base/task/sequence_manager/sequence_manager.h) manages TaskQueues which have different properties (e.g. priority, common task type) multiplexing all posted tasks into a single backing sequence.

#### [MessagePump](https://github.com/chromium/chromium/blob/master/docs/threading_and_tasks.md#messagepump)

> NOTE: 
>
> "pump"的含义是"泵"

`MessagePumps` take care to mixing **delegate callbacks** with **native message processing** so neither type of event starves the other of cycles.

> NOTE: 
>
> MessagePumps注意混合委托回调和本机消息处理，这样两种类型的事件都不会消耗另一种周期。

## github [Threading and Tasks in Chrome - FAQ](https://github.com/chromium/chromium/blob/master/docs/threading_and_tasks_faq.md)





## cnblogs [Chromium中多线程及并发技术要点(C/C++)](https://www.cnblogs.com/tlnshuju/p/7246578.html)

### `SequencedWorkerPool`

它是一个线程池，用于运行须要串行运行的任务请求，这些请求根据不同的Token分组，仅仅在同样组内保证运行顺序。

这样多个组能够并行运行，而单个组则是串行运行。比方：不同实例所相应的分组不同。或者不同的功能相应不同的分组。

它同一时候同意指定当退出时未运行的任务怎样处理，包含:继续运行，忽略，阻止退出。

參考:[The FILE thread is dead, long live the blocking pool](https://groups.google.com/a/chromium.org/forum/#!topic/chromium-dev/OaA-9rnjOqI).





## csdn [Chormium线程模型及应用指南](https://blog.csdn.net/horkychen/article/details/50486062)



## Read the code

一、以自顶向下的方式来进行阅读

二、

### `base/task`

#### 顶层interface

这个模块的顶层interface是`base\task\post_task.h`、`base\task\thread_pool.h`，正如在 `base\task\post_task.h` 中所言:

```
// Note: A migration is in-progress away from this API and in favor of explicit
// API-as-a-destination. thread_pool.h replaced base::ThreadPool() as a task
// trait and browser_thread.h replaced BrowserThread::IO/IO as a task trait
// (ios::WebThread is still lagging and must use this API for now).
//
// To post a simple one-off task with default traits:
//     PostTask(FROM_HERE, BindOnce(...));
// modern equivalent:
//     ThreadPool::PostTask(FROM_HERE, BindOnce(...));
//
// To post a high priority one-off task to respond to a user interaction:
//     PostTask(
//         FROM_HERE,
//         {ThreadPool(), TaskPriority::USER_BLOCKING},
//         BindOnce(...));
// modern equivalent:
//     ThreadPool::PostTask(
//         FROM_HERE,
//         {TaskPriority::USER_BLOCKING},
//         BindOnce(...));
```

可以看到，在两个模块中提供了相同的interface。

`base\task\thread_pool.h` 中的介绍如下: 

> This is the interface to post tasks to base's thread pool.

`base\task\post_task.h` 中的介绍如下:

> This is the interface to post tasks.

显然两种都是post task，提供查看`base\task\post_task.h`的source code可知，它是调用 `base\task\thread_pool.h` 中的method来实现的。



### `ThreadPool`

典型的遵循C style interface、implementation file的模式

```c++
class BASE_EXPORT ThreadPool
```

这是一个顶层class，它提供了多种static interface，它是学习task、thread的入口。

在  [chromium](https://github.com/chromium/chromium)/[docs](https://github.com/chromium/chromium/tree/master/docs)/**[threading_and_tasks.md](https://github.com/chromium/chromium/blob/master/docs/threading_and_tasks.md)** # Using ThreadPool in a New Process 中，有对它的介绍。

#### interface

`base\task\thread_pool.h`

`base\task\thread_pool.cc`



##### thread_pool_instance



`base\task\thread_pool\thread_pool_instance.h`

`base\task\thread_pool\thread_pool_instance.cc`

一、有一个全局的thread pool `g_thread_pool`，它的实现是比较类似于singlton的，但是这个类并没有基础singlton，提供如下方法来创建 全局的thread pool `g_thread_pool`

```C++
#if !defined(OS_NACL)
  // Creates and starts a thread pool using default params. |name| is used to
  // label histograms, it must not be empty. It should identify the component
  // that calls this. Start() is called by this method; it is invalid to call it
  // again afterwards. CHECKs on failure. For tests, prefer
  // base::test::TaskEnvironment (ensures isolation).
  static void CreateAndStartWithDefaultParams(StringPiece name);

  // Same as CreateAndStartWithDefaultParams() but allows callers to split the
  // Create() and StartWithDefaultParams() calls.
  void StartWithDefaultParams();
#endif  // !defined(OS_NACL)

  // Creates a ready to start thread pool. |name| is used to label histograms,
  // it must not be empty. It should identify the component that creates the
  // ThreadPoolInstance. The thread pool doesn't create threads until Start() is
  // called. Tasks can be posted at any time but will not run until after
  // Start() is called. For tests, prefer base::test::TaskEnvironment
  // (ensures isolation).
  static void Create(StringPiece name);

  // Registers |thread_pool| to handle tasks posted through the post_task.h
  // API for this process. For tests, prefer base::test::TaskEnvironment
  // (ensures isolation).
  static void Set(std::unique_ptr<ThreadPoolInstance> thread_pool);

  // Retrieve the ThreadPoolInstance set via SetInstance() or Create(). This
  // should be used very rarely; most users of the thread pool should use the
  // post_task.h API. In particular, refrain from doing
  //   if (!ThreadPoolInstance::Get()) {
  //     ThreadPoolInstance::Set(...);
  //     base::PostTask(...);
  //   }
  // instead make sure to SetInstance() early in one determinstic place in the
  // process' initialization phase.
  // In doubt, consult with //base/task/thread_pool/OWNERS.
  static ThreadPoolInstance* Get();
```



二、`InitParams`

初始化参数

三、

> stackoverflow [Could you explain STA and MTA?](https://stackoverflow.com/questions/127188/could-you-explain-sta-and-mta)



四、thread pool instance相当于interface



```C++
void ThreadPoolInstance::Create(StringPiece name) {
  Set(std::make_unique<internal::ThreadPoolImpl>(name));
}
```



五、`ScopedExecutionFence`、`ScopedBestEffortExecutionFence`

典型的RAII、典型的non-copyable



#### implementation

这是我需要学习的一种技术

`base\task\thread_pool\thread_pool_impl.h`

`base\task\thread_pool\thread_pool_impl.cc`

```C++
// Default ThreadPoolInstance implementation. This class is thread-safe.
class BASE_EXPORT ThreadPoolImpl : public ThreadPoolInstance,
                                   public TaskExecutor,
                                   public ThreadGroup::Delegate,
                                   public PooledTaskRunnerDelegate
```

需要注意的是: 它实现了如下interface:

1、`TaskExecutor`

2、`TaskExecutor`

3、

#### thread pool的组成



### `base/task/sequence_manager`



`base\task\sequence_manager\task_queue.h`

#### Task queue

```C++
task_queue.h
task_queue.cc
task_queue_impl.h
task_queue_impl.cc
    
```



#### `SequenceManager`



#### `enqueue_order_generator`

```C++
enqueue_order.h
enqueue_order_generator.h
enqueue_order_generator.cc
```



### task executor

```C++
task_executor.h
task_executor.cc
```

需要提供创建`TaskRunner`的接口。

#### `class BASE_EXPORT TaskExecutor`

它是一个interface。



#### `GetTaskExecutorForCurrentThread`、`SetTaskExecutorForCurrentThread`

`ThreadLocalPointer<TaskExecutor>* GetTLSForCurrentTaskExecutor()` 会将thread的task executor此放到thread local storage中

#### 全局的task executor map

```C++
// Maps TaskTraits extension IDs to registered TaskExecutors. Index |n|
// corresponds to id |n - 1|.
using TaskExecutorMap =
    std::array<TaskExecutor*, TaskTraitsExtensionStorage::kMaxExtensionId>;
TaskExecutorMap* GetTaskExecutorMap() {
  static_assert(std::is_trivially_destructible<TaskExecutorMap>::value,
                "TaskExecutorMap not trivially destructible");
  static TaskExecutorMap executors{};
  return &executors;
}

```

#### `RegisterTaskExecutor`、`UnregisterTaskExecutorForTesting`

在下面的模块中，调用了 `RegisterTaskExecutor`

```
content\browser\scheduler\browser_task_executor.cc
ios\web\web_thread_impl.cc
```



#### `TaskExecutor` implementation

##### `SimpleTaskExecutor`

```
base\task\simple_task_executor.h
```

### `TaskRunner`

```
task_runner.h
task_runner.cc
```

按照在 [chromium](https://github.com/chromium/chromium)/[docs](https://github.com/chromium/chromium/tree/master/docs)/**[threading_and_tasks.md](https://github.com/chromium/chromium/blob/master/docs/threading_and_tasks.md)** 中的介绍，task runner的概念是:

> - **Task runner**: An interface through which tasks can be posted. In Chrome this is `base::TaskRunner`.



### `SequenceManager` 



### `class BASE_EXPORT RunLoop` 

```
run_loop.h
run_loop.cc
```

在官方文档中，有如下介绍:

> ### RunLoop
>
> `RunLoop` is a helper class to run the `RunLoop::Delegate` associated with the current thread (usually a `SequenceManager`). Create a `RunLoop` on the stack and call Run/Quit to run a nested `RunLoop` but please avoid nested loops in production code!



### `base/threading`

> A simple thread abstraction that establishes a MessageLoop on a new thread.

jianshu [Chromium中Thread的创建](https://www.jianshu.com/p/fe1799a5b342)

### delegate

要搞清楚所delegate的是什么、将什么的implementation(实现)给delegate出去了；本质上来说delegate所定义的是interface，它需要由其他class来进行实现，然后在host class中，使用delegate的implementation，比如`Thread`的`Delegate`所delegate的是它的task queue的implementation。

`base\task\thread_pool\pooled_task_runner_delegate.h`

```C++
// Delegate interface for PooledParallelTaskRunner and
// PooledSequencedTaskRunner.
class BASE_EXPORT PooledTaskRunnerDelegate
```

`base\task\thread_pool\thread_group.h`

```C++
// Interface and base implementation for a thread group. A thread group is a
// subset of the threads in the thread pool (see GetThreadGroupForTraits() for
// thread group selection logic when posting tasks and creating task runners).
class BASE_EXPORT ThreadGroup {
 public:
  // Delegate interface for ThreadGroup.
  class BASE_EXPORT Delegate {
   public:
    virtual ~Delegate() = default;

    // Invoked when a TaskSource with |traits| is non-empty after the
    // ThreadGroup has run a task from it. The implementation must return the
    // thread group in which the TaskSource should be reenqueued.
    virtual ThreadGroup* GetThreadGroupForTraits(const TaskTraits& traits) = 0;
  };
```



在下面的文章中，进行了解释: 

1、csdn [Chromium源码浅析---委托模式(Delegate Pattern)在Chromium中的应用](https://blog.csdn.net/yl02520/article/details/17058767)

2、titanwolf [An Analysis of Chromium Source Code --- Application of Delegate Pattern in Chromium](https://titanwolf.org/Network/Articles/Article?AID=4d215441-0b51-4f75-8d9f-34e922741f54)





`base\run_loop.h`

其中的 `RunLoop::Delegate` 是一种virtual class idiom。

### proxy pattern

在其中也使用了proxy pattern。

#### groups.google [Client vs Delegate pattern in chromium](https://groups.google.com/a/chromium.org/g/chromium-dev/c/z44HalI8B3E)



### unit testing、单元测试

chromium的unit testing是做的非常好的，它的经验是值得借鉴学习的。



### checker

1、[chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/master/base)/**[sequence_checker.h](https://github.com/chromium/chromium/blob/master/base/sequence_checker.h)**

2、[chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/master/base)/**[check.h](https://github.com/chromium/chromium/blob/master/base/check.h)**

类似断言，不过会记录到日志中

3、[chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/master/base)/**[check_op.h](https://github.com/chromium/chromium/blob/master/base/check_op.h)**

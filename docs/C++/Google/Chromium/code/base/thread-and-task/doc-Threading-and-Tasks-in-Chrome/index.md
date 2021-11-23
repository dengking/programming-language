# doc [Threading and Tasks in Chrome](https://github.com/chromium/chromium/blob/master/docs/threading_and_tasks.md)

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



## Core Concepts

**Sequence** or **Virtual thread**: A chrome-managed thread of execution. Like a physical thread, only one task can run on a given sequence / virtual thread at any given moment and each task sees the side-effects of the preceding tasks. Tasks are executed sequentially but may hop physical threads between each one.

> NOTE: 
>
> "**Sequence** or **Virtual thread**"的执行单位是 **task**，类似的 physical thread的执行单位 **指令**
>
> "Tasks are executed sequentially but may hop physical threads between each one"
>
> "**Sequence** or **Virtual thread**"的task是顺序执行的，但是在具体的实现上，tasks是可能由多个thread执行的



header 1 | header 2
---|---
row 1 col 1 | row 1 col 2
row 2 col 1 | row 2 col 2


## Threading Lexicon

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

## Tasks

A group of tasks can be executed in one of the following ways:

1、[Parallel](https://github.com/chromium/chromium/blob/master/docs/threading_and_tasks.md#Posting-a-Parallel-Task): No task execution ordering, possibly all at once on any thread

2、[Sequenced](https://github.com/chromium/chromium/blob/master/docs/threading_and_tasks.md#Posting-a-Sequenced-Task): Tasks executed in posting order, one at a time on any thread.

3、Single Threaded: Tasks executed in posting order, one at a time on a single thread.

- [COM Single Threaded](https://github.com/chromium/chromium/blob/master/docs/threading_and_tasks.md#Posting-Tasks-to-a-COM-Single_Thread-Apartment-STA_Thread-Windows): A variant of single threaded with COM initialized.

## Cancelling a Task

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


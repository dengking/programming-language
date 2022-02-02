# chromium/base library

使用C++14开发；

## doc [Important Abstractions and Data Structures](https://www.chromium.org/developers/coding-style/important-abstractions-and-data-structures)

> NOTE: 
>
> 一、使用的是经典的task-executor-model
>
> task
>
> task runner
>
> worker pool
>
> message loop
>
> 二、通过callback进行asynchronous notification

### [TaskRunner](https://code.google.com/p/chromium/codesearch#chromium/src/base/task_runner.h&q=TaskRunner&sq=package:chromium&type=cs) & [SequencedTaskRunner](https://code.google.com/p/chromium/codesearch#chromium/src/base/sequenced_task_runner.h&q=SequencedTaskRunner&sq=package:chromium&type=cs) & [SingleThreadTaskRunner](https://code.google.com/p/chromium/codesearch#chromium/src/base/single_thread_task_runner.h&q=SingleThreadTaskRunner&sq=package:chromium&type=cs)

> NOTE: 
>
> 一、需要注意的是，它们都是interface，这一点在原文第一段进行了说明；各种interface有着各自的特性；

Interfaces for posting `base::Callback`s "tasks" to be run by the `TaskRunner`.

1、[TaskRunner](https://code.google.com/p/chromium/codesearch#chromium/src/base/task_runner.h&q=TaskRunner&sq=package:chromium&type=cs) 

2、[SequencedTaskRunner](https://code.google.com/p/chromium/codesearch#chromium/src/base/sequenced_task_runner.h&q=SequencedTaskRunner&sq=package:chromium&type=cs) 

3、[SingleThreadTaskRunner](https://code.google.com/p/chromium/codesearch#chromium/src/base/single_thread_task_runner.h&q=SingleThreadTaskRunner&sq=package:chromium&type=cs)

`MessageLoopProxy` is the canonical example of a `SingleThreadTaskRunner`.

> NOTE: 
>
> 这段话的含义是: `MessageLoopProxy` 是对  `SingleThreadTaskRunner` interface的implementation



These interfaces are also useful for testing via **dependency injection**. 

> NOTE: 
>
> 这段话要如何理解？

### [MessageLoop](https://code.google.com/p/chromium/codesearch#chromium/src/base/message_loop/message_loop.h&q=MessageLoop&sq=package:chromium&type=cs&l=46) & [MessageLoopProxy](https://code.google.com/p/chromium/codesearch#chromium/src/base/message_loop/message_loop_proxy.h&q=MessageLoopProxy&sq=package:chromium&type=cs&l=17) & [BrowserThread](https://code.google.com/p/chromium/codesearch#chromium/src/content/public/browser/browser_thread.h&q=BrowserThread&sq=package:chromium&type=cs) & [RunLoop](https://code.google.com/p/chromium/codesearch#chromium/src/base/run_loop.h&q=run_loop.h&sq=package:chromium&type=cs&l=31)

These are various APIs for posting a task. `MessageLoop` is a concrete object used by `MessageLoopProxy` (the most widely used task runner in Chromium code).

> NOTE: 
>
> `MessageLoopProxy` use `MessageLoop` 

PS: There's some debate about when to use `SequencedTaskRunner` vs `MessageLoopProxy` vs `BrowserThread`. 

> NOTE: 
>
> 通过上面的描述可知 `SequencedTaskRunner` vs `MessageLoopProxy` vs `BrowserThread` 三者类似

### [base::Callback](https://code.google.com/p/chromium/codesearch#chromium/src/base/callback.h) and [base::Bind()](https://code.google.com/p/chromium/codesearch#chromium/src/base/bind.h&q=Base::bind&sq=package:chromium&type=cs)

> NOTE:
>
> 比较好理解

### [scoped_refptr & base::RefCounted & base::RefCountedThreadSafe](https://code.google.com/p/chromium/codesearch#chromium/src/base/memory/ref_counted.h)

> NOTE: 
>
> 一、比较好理解，一个问题是为什么chromium不适用STL的smart pointer？应该是它有非常多的custom需求

### [base::WeakPtr](https://code.google.com/p/chromium/codesearch#chromium/src/base/memory/weak_ptr.h&q=base::WeakPtr&sq=package:chromium&type=cs) & [base::WeakPtrFactory](https://code.google.com/p/chromium/codesearch#chromium/src/base/memory/weak_ptr.h&q=base::WeakPtrFactory&sq=package:chromium&type=cs&l=246)

> NOTE: 
>
> 一、它和前面一样，都是smart pointer，因此把它放到这里

Mostly thread-unsafe weak pointer that returns NULL if the referent has been destroyed. It's safe to pass across threads (and to destroy on other threads), but it should only be used on the original thread it was created on. `base::WeakPtrFactory` is useful for automatically canceling `base::Callbacks` when the referent of the `base::WeakPtr` gets destroyed.

## github [chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/main/base)/**[README.md](https://github.com/chromium/chromium/blob/main/base/README.md)**



## Source code

1、github [chromium](https://github.com/chromium/chromium)/**[base](https://github.com/chromium/chromium/tree/master/base)**/

其中包含非常多的基础功能，值得借鉴


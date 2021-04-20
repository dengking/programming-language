# `std::atomic_bool` VS `bool`

一、`std::atomic_bool`能够解决lock-free的两个挑战:

1、atomic

2、memory ordering

a、compile memory ordering

b、run time memory ordering、inter-cache inconsistency

二、intentional programming

## stackoverflow [When do I really need to use atomic instead of bool? [duplicate]](https://stackoverflow.com/questions/16320838/when-do-i-really-need-to-use-atomicbool-instead-of-bool)

> NOTE: 
>
> 1、下面的三个回答，分别从三个不同的方面进行了论述

### [A](https://stackoverflow.com/a/16321311)

Remember about [memory barriers](https://en.wikipedia.org/wiki/Memory_barrier). Although it may be impossible to change `bool` partially, it is possible that multiprocessor system has this variable in multiple copies and one thread can see old value even after another thread has changed it to new. Atomic introduces memory barrier, so it becomes impossible.

> NOTE: 
>
> 一、上述描述的是典型的memory ordering的问题: 
>
> 1、Prevent seeing stale old data
>
> 2、order of write to shared data may be different among different threads



### [A](https://stackoverflow.com/a/16321065)

*No* type in C++ is "atomic by nature" unless it is an `std::atomic*`-something. That's because the standard says so.

In practice, the actual hardware instructions that are emitted to manipulate an `std::atomic<bool>` may (or may not) be the same as those for an ordinary `bool`, but being atomic is a larger concept with wider ramifications (e.g. restrictions on compiler re-ordering). Furthermore, some operations (like negation) are overloaded on the atomic operation to create a distinctly different instruction on the hardware than the native, non-atomic read-modify-write sequence of a non-atomic variable.

> NOTE: 
>
> 1、从语言的层面来进行论述



### [A](https://stackoverflow.com/a/16322033)

> NOTE: 
>
> 1、非常系统的论述

C++'s atomic types deal with **three** potential problems. 

First, a read or write can be torn by a task switch if the operation requires more than one bus operation (and that **can** happen to a `bool`, depending on how it's implemented). 

> NOTE: 
>
> 1、atomic问题

Second, a read or write may affect only the cache associated with the processor that's doing the operation, and other processors may have a different value in their cache. 

> NOTE: 
>
> 1、run-time memory ordering、inter-cache inconsistency

Third, the compiler can rearrange the order of operations if they don't affect the result (the constraints are a bit more complicated, but that's sufficient for now).

> NOTE: 
>
> 1、compiler memory ordering

You can deal with each of these three problems on your own by making assumptions about how the types you are using are implemented, by explicitly flushing caches, and by using compiler-specific options to prevent reordering (and, no, `volatile` doesn't do this unless your compiler documentation says it does).

But why go through all that? `atomic` takes care of it for you, and probably does a better job than you can do on your own.
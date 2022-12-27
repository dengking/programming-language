# C++ multiple level memory model

本节标题的含义是: 多层次的memory model，它能够帮助我们建立对C++ memory model高屋建瓴的认识。

关于各种memory model的描述；

本章概述C++ memory model，主要参考:

1、sciencedirect Victor Alessandrini, in [Shared Memory Application Programming](https://www.sciencedirect.com/book/9780128037614/shared-memory-application-programming), 2016

2、zhihu [如何理解 C++11 的六种 memory order？](https://www.zhihu.com/question/24301047)

3、modernescpp [C++ Memory Model](https://www.modernescpp.com/index.php/c-memory-model)

## 三种还是四种还是六种?

可以看到:

一、sciencedirect Victor Alessandrini, in [Shared Memory Application Programming](https://www.sciencedirect.com/book/9780128037614/shared-memory-application-programming), 2016

有如下三种memory model:

1 Sequential consistency

2 Acquire-release

3 Relaxed

> NOTE: 由强到弱

二、zhihu [如何理解 C++11 的六种 memory order？](https://www.zhihu.com/question/24301047)  # [A](https://www.zhihu.com/question/24301047/answer/85844428) 

有如下四种memory model:

\1. Relaxed ordering

\2. Release -- acquire

\3. Release -- consume

\4. **Sequential consistency**

> NOTE: 有弱到强

三、modernescpp [C++ Memory Model](https://www.modernescpp.com/index.php/c-memory-model)

有如下三种:

1 Sequential consistency

2 Acquire-release

3 Relaxed

> NOTE: 由强到弱

四、cppreference [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order)

在 `Explanation` 段中给出了四种:

1、[Relaxed ordering](https://en.cppreference.com/w/cpp/atomic/memory_order#Relaxed_ordering) 

2、[Release-Consume ordering](https://en.cppreference.com/w/cpp/atomic/memory_order#Release-Consume_ordering) 

3、[Release-Acquire ordering](https://en.cppreference.com/w/cpp/atomic/memory_order#Release-Acquire_ordering) 

4、[Sequentially-consistent ordering](https://en.cppreference.com/w/cpp/atomic/memory_order#Sequentially-consistent_ordering) 

在 `Constants`中给出了六种:

1、`memory_order_relaxed`

2、`memory_order_consume`

3、`memory_order_acquire`

4、`memory_order_release`

5、`memory_order_acq_rel`

6、`memory_order_seq_cst`









## Memory order设计总结

C++还提供了对memory order的控制。

关于C++ memory order的设计，我主要是参考的如下两篇文章:

1、zhihu [如何理解 C++11 的六种 memory order？](https://www.zhihu.com/question/24301047) # [A](https://www.zhihu.com/question/24301047/answer/1193956492)

2、cppreference [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order) # Formal description

### shared data是同步点

本节的内容主要源自 zhihu [如何理解 C++11 的六种 memory order？](https://www.zhihu.com/question/24301047) # [A](https://www.zhihu.com/question/24301047/answer/1193956492) 。

1、在使用C++ atomic library进行lockless programming的时候，一般使用 atomic variable 来作为shared data。

### `std::memory_order` "控制的是同一个线程内指令的执行顺序"

本节的内容主要源自 zhihu [如何理解 C++11 的六种 memory order？](https://www.zhihu.com/question/24301047) # [A](https://www.zhihu.com/question/24301047/answer/1193956492) ，标题中的"控制的是同一个线程内指令的执行顺序"也是源自其中。

1、`std::memory_order` 是控制single thread的memory order

2、zhihu [如何理解 C++11 的六种 memory order？](https://www.zhihu.com/question/24301047) # [A](https://www.zhihu.com/question/24301047/answer/1193956492) : "限制编译器以及CPU对单线程当中的指令执行顺序进行重排的程度"



### 可以使用memory order进行控制的对象

1、`std::atomic`

2、`std::atomic_thread_fence`

为什么要这样设计呢？它们是同步点。

### Modification order

本节的内容主要源自 cppreference [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order) # Formal description

1、Modification order是从全局角度基于"happens-before"关系对所有thread对同一个**atomic variable**的read、write进行order而形成的sequence，基于它我们能够对C++ multi thread program的行为进行formal description、formal analysis。

2、上述"atomic variable"就是**同步点**

3、"happens-before"关系需要由programmer来进行建立，C++为programmer提供了实现"happens-before"关系的方法(后面会进行介绍)，可以肯定的是，它的实现是会使用到 `std::memory_order` 的。



## 思维误区

是否会存在不同thread对shared data的修改没有同步过来而导致看到了旧数据？这是一个容易陷入的误区，事实是: 不同thread对shared data的修改，其他的thread是能够看到的，不同thread可能看到的不同的是: memory order的不同，正是由于order的不同，而导致了在lock free情况下会出现问题。因此需要使用memory order进行控制。

> NOTE: tag-order of write to shared data may be different among different threads


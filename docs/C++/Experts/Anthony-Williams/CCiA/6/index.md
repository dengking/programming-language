# Designing lock-based concurrent data structures



## 6.1 What does it mean to design for concurrency?

> NOTE: 这一节作者告诉了我们设计一个concurrent data structure要考虑的两个方面：
>
> - ensuring that the accesses are *safe* 
> - *enabling* genuine concurrent access
>
> 关于第一点，在chapter 3中已经讨论了，在本节作者再次描述了这些guideline。设计一个concurrent data structure需要考虑的一个非常重要的问题是：多个线程调用这个data structure的不同method时（使用OOP思想），如何保证thread-safe？
>
> 接着作者讨论了第二点。诚如作者所言：如何达到这一点无法给出形式化地描述，而是在实现concurrent data structure时，我们应该时刻如下核心问题：
>
> how can you minimize the amount of serialization that must occur and enable the greatest amount of true concurrency?
>
> 本书第六章介绍Lock-based concurrent data structures
>
> 本书第七章介绍Lock-free concurrent data structures

At the basic level, designing a data structure for concurrency means that multiple threads can access the data structure concurrently, either performing the same or distinct operations, and each thread will see a self-consistent view of the data structure. No data will be lost or corrupted, all invariants will be upheld, and there’ll be no problematic **race conditions**. Such a data structure is said to be **thread-safe**. In general, a data structure will be safe only for particular types of concurrent access. It may be possible to have multiple threads performing one type of operation on the data structure concurrently, whereas another operation requires exclusive access by a single thread. Alternatively, it may be safe for multiple threads to access a data structure concurrently if they’re performing different actions, whereas multiple threads performing the same action would be problematic.

> NOTE:
> 一、每个thread像是一个独立的个体，能够see自己的view，作者后面关于thread的论述都是采用的这样的论述

### True concurrency VS serialization

By its very nature, a mutex provides **mutual exclusion**: only one thread can acquire a lock on the mutex at a time. A mutex protects a data structure by explicitly preventing true concurrent access to the data it protects. This is called **serialization**: threads take turns accessing the data protected by the mutex; they must access it serially rather than concurrently.

Consequently, you must put careful thought into the design of the data structure to enable true concurrent access. 

Some data structures have more scope for **true concurrency** than others, but in all cases the idea is the same: the smaller the protected region, the fewer operations are serialized, and the greater the potential for concurrency.

> NOTE: 这一节中描述的概念非常重要，阅读完后，有如下启发：
>
> - 使用*invariant*（在Theory章节有对invariant的描述）来描述*thread-safe*
> - race condition
> - mutex-》*mutual exclusion*-》preventing true concurrent access to the data it protects-》*serialization*
>
> 本章所要讨论内容是如何实现true concurrent；我们原来使用的mutex机制能够保证thread-safe，但是并不够高效。

### 6.1.1 Guidelines for designing data structures for concurrency

I covered the basics of how to make the data structure thread-safe back in chapter 3:

1、Ensure that no thread can see a state where the invariants of the data structure have been broken by the actions of another thread.
> NOTE: 
> 一、上述论述也是使用的"thread see"的方式

2、Take care to avoid race conditions inherent in the interface to the data structure by providing functions for complete operations rather than for operation steps.

> NOTE:
> 一、上述讨论的是如何设计interface，作者的意思简而言之就是: 将多步合并为一步

3、Pay attention to how the data structure behaves in the presence of exceptions to ensure that the invariants are not broken.
> NOTE:
> 一、上述讨论的是exception safety

4、Minimize the opportunities for deadlock when using the data structure by restricting the scope of locks and avoiding nested locks where possible.

> NOTE:
> 一、如何避免deadlock


Before you think about any of these details, it’s also important to think about what constraints you wish to put on the users of the data structure; if one thread is accessing the data structure through a particular function, which functions are safe to call from other threads?

This is actually quite a crucial question to consider. Generally **constructors** and **destructors** require exclusive access to the data structure, but it’s up to the user to ensure that they’re not accessed before **construction** is complete or after **destruction** has started. If the data structure supports **assignment**, `swap()`, or **copy construction**, then as the designer of the data structure, you need to decide whether these operations are safe to call concurrently with other operations or whether they require the user to ensure exclusive access even though the majority of functions for manipulating the data structure may be called from multiple threads concurrently without problem.

> NOTE:
> 一、设计一个concurrent data structure需要考虑的一个非常重要的问题是：多个线程调用这个data structure的不同method时（使用OOP思想），如何保证thread-safe？

The second aspect to consider is that of enabling genuine concurrent access. 

All these questions are guided by a single idea: how can you minimize the amount of **serialization** that must occur and enable the greatest amount of true concurrency? It’s not uncommon for data structures to allow concurrent access from multiple threads that merely read the data structure, whereas a thread that can modify the data structure must have exclusive access. This is supported by using constructs like `boost:: shared_mutex`. Likewise, as you’ll see shortly, it’s quite common for a data structure to support concurrent access from threads performing different operations while serializing threads that try to perform the same operation.


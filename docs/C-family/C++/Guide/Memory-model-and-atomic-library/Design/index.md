# Design of C++ memory model and atomic library

C++ memory model and atomic library是比较复杂的，本章对它的设计进行讨论，以期为建立其高屋建瓴的视角、从整体上对它进行掌握。



## C++ memory model feature

总的来说:

能够容纳当今主流的各种CPU memory model(hardware)，容纳它们的feature/差异，让programmer以portable、standard方式，充分发挥hardware power。抽象地描述/model了当前主流hardware。

> NOTE: 
>
> abstraction and design to an abstraction

下面是一些具体的点: 

1、multi-threaded by design(multicore)

2、has a well-defined *memory model*(memory reordering)

3、multiple level，非常的灵活



参考: 

1、stackoverflow [C++11 introduced a standardized memory model. What does it mean? And how is it going to affect C++ programming?](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g)

2、modernescpp [C++ Memory Model](https://www.modernescpp.com/index.php/c-memory-model)



### CPU memory model and programming language memory model

C++ memory model能够容纳当今大多数CPU memory model，让我想到了有必要强调一下"CPU memory model and programming language memory model"，关于 "programming language memory model"，参见 `Programming-language-Memory-model` 章节。

> NOTE: 
>
> abstraction and design to an abstraction



## Multiple-level-memory-model

`Multiple-level-memory-model` 对多层次的C++ memory model进行详细的讨论。



## How C++ ensure thread safety?

在工程 Parallel-computing 的 `Concurrent-computing\Multithread\Thread-safety\What-cause-unsafety` 章节中总结了导致非线程安全的两个原因，那C++ 是如何解决这两个问题从而保证thread safety的呢？C++ atomic library提供了对这两个cause的控制:

1、`std::atomic`: 避免了 Race condition

2、`std::memory_ordering`: 对memory ordering进行控制

> 多个thread同时对原子变量进行操作，原子变量能够保证原子性，即没有中间状态，使用原子变量进行lockless programming还需要克服的一个问题是：memory reordering，因此，还需要对memory order进行控制。



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

### 总结

1、C++ memory model 和 atomic library 基于了programmer实现让所有的thread 按照设计的order去access shared atomic variable，而不是去contention，从而可以实现lockfree programming

2、对一个shared data的read、write order进行控制，从而避免data race，达到目的cooperative的目的，从而实现lockless programming。

> NOTE: 上面这段话中的"cooperative"，我记得是preshing提出的，但是现在去看它的文章，却找不到了

## 思维误区

是否会存在不同thread对shared data的修改没有同步过来而导致看到了旧数据？这是一个容易陷入的误区，事实是: 不同thread对shared data的修改，其他的thread是能够看到的，不同thread可能看到的不同的是: memory order的不同，正是由于order的不同，而导致了在lock free情况下会出现问题。因此需要使用memory order进行控制。

> NOTE: tag-order of write to shared data may be different among different threads



## TODO 

### `memory_ordering` and visibility

stackoverflow [Does seq_cst ordering guarantee immediate visibility?](https://stackoverflow.com/questions/14846494/does-seq-cst-ordering-guarantee-immediate-visibility)



### Memory model and consistency model and memory ordering

需要注意的是: consistency model其实所讨论的就是memory的问题，因此一个programming language的memory model是需要定义它的consistency model的，它需要明确定义它的consistency。


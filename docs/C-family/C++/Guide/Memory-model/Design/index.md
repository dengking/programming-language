# Design of C++ memory model and atomic library

C++ memory model and atomic library是比较复杂的，本章对这个它进行讨论。

1、首先讨论"Why-need-memory-model"

2、"Multiple-level-memory-model" 对C++ memory model进行详细的讨论

## Implementation: C++ atomic library

前面总结了导致非线程安全的两个原因，这让我想到了C++ atomic library，显然C++ atomic library提供了对这两个cause的控制:

1、`std::atomic`: 避免了 Race condition

2、`std::memory_ordering`: 对memory ordering进行控制

> 多个thread同时对原子变量进行操作，原子变量能够保证原子性，即没有中间状态，使用原子变量进行lockless programming还需要克服的一个问题是：memory reordering，因此，还需要对memory order进行控制。

更多详细的信息，参见工程programming language的 `C++\Guide\Memory-model\Atomic-operations-library\Design-and-implementation` 章节。



## Memory ordering总结

### Modification order

1、Modification order是一个全局的order

2、Modification order是基于 "happens-before" 而建立的



### `std::memory_order`

1、它是控制single thread的memory order

2、memory ordering 所有的thread 按照系统的order去access shared atomic variable，而不是去contention

a、对一个shared data的read、write order进行控制，从而避免data race，达到目的

b、这应该是preshing cooperative的含义吧





### C++ memory ordering

参见:

1、zhihu [如何理解 C++11 的六种 memory order？](https://www.zhihu.com/question/24301047) # [A](https://www.zhihu.com/question/24301047/answer/1193956492)

这篇文章对C++ memory ordering的分析是非常深刻的。

2、stackoverflow [What do each memory_order mean?](https://stackoverflow.com/questions/12346487/what-do-each-memory-order-mean)



## TODO `memory_ordering` and visibility

stackoverflow [Does seq_cst ordering guarantee immediate visibility?](https://stackoverflow.com/questions/14846494/does-seq-cst-ordering-guarantee-immediate-visibility)





## C++11 memory model feature

抽象地描述/model了当前主流hardware:

1、multi-threaded by design(multicore)

2、has a well-defined *memory model*(memory reordering)

能够容纳大多数hardware的feature/差异，让programmer以portable、standard方式，充分发挥hardware power。

参考: 

1、stackoverflow [C++11 introduced a standardized memory model. What does it mean? And how is it going to affect C++ programming?](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g)



## Memory model and consistency model and memory ordering

需要注意的是: consistency model其实所讨论的就是memory的问题，因此一个programming language的memory model是需要定义它的consistency model的，它需要明确定义它的consistency。


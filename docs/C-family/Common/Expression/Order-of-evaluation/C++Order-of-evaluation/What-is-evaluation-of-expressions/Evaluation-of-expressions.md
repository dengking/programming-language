# What is Evaluation of Expressions？

最近越多的多篇文章中，都谈及了side effect，那它到底什么是呢？C++官方文档在cppreference [Order of evaluation](https://en.cppreference.com/w/cpp/language/eval_order) # [Evaluation of Expressions](https://en.cppreference.com/w/cpp/language/eval_order#Evaluation_of_Expressions) 中给出了definition，但是它并不容易理解，本文就是对Evaluation of Expressions进行讨论，其中就会涉及side effect。



## cppreference [Order of evaluation](https://en.cppreference.com/w/cpp/language/eval_order) # [Evaluation of Expressions](https://en.cppreference.com/w/cpp/language/eval_order#Evaluation_of_Expressions) 

Evaluation of each expression includes:

1、*value computations*: calculation of the value that is returned by the expression. This may involve determination of the identity of the object (glvalue evaluation, e.g. if the expression returns a reference to some object) or reading the value previously assigned to an object (prvalue evaluation, e.g. if the expression returns a number, or some other value)



2、Initiation of *side effects*: access (read or write) to an object designated by a volatile glvalue, modification (writing) to an object, calling a library I/O function, or calling a function that does any of those operations.



## 问题: 为什么分为两个部分?

可以看到，上述对 [Evaluation of Expressions](https://en.cppreference.com/w/cpp/language/eval_order#Evaluation_of_Expressions) 的定义分为了两个部分，那为什么要这样就行划分呢？



### 从CPU的运行机制来进行分析

下面是直观理解:

1、*value computations* 显然和 computation 相关，显然它对应的是ALU

2、*side effects* 显然和 state 相关，它对应的是memory (需要注意的是: 不是cache)

现状:

1、parallel computing: multicore

2、bottleneck: CPU的运行速度 远高于 access memory的速度，因此现代CPU中，每个core都有cache，分为L1 cache、L2 cache

下面是完整的分析过程: 

1、expression对应的是运算、因此最终对应的是一堆instruction

2、CPU执行的过程: memory、register、ALU

3、ALU一般从register中取得operand，它的**运行结果**也会先放到register中，这样后续的instruction就可以直接使用前一个instruction的**运行结果**

4、(猜测的)CPU的执行过程中，在完成了instruction的执行后，应该会继续执行下一条指令，而无需等待这条指令的运行结果写入到memory中(这应该就是死的 effect)，也就是说CPU应该是不会等待instruction的side effect的完成



在下面文章中，也对这个topic进行了讨论。

## josephmansfield [C++ sequenced-before graphs](https://josephmansfield.uk/articles/c++-sequenced-before-graphs.html)



## Google: c++ side effects

stackoverflow [What exactly is a 'side-effect' in C++?](https://stackoverflow.com/questions/9563600/what-exactly-is-a-side-effect-in-c)



## Google: operators with side effects c++
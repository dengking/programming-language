# What is Evaluation of Expressions？

1、C++官方文档在cppreference [Order of evaluation](https://en.cppreference.com/w/cpp/language/eval_order) # [Evaluation of Expressions](https://en.cppreference.com/w/cpp/language/eval_order#Evaluation_of_Expressions) 中给出了definition，但是它并不容易理解，本文就是对Evaluation of Expressions进行讨论，其中就会涉及side effect。

2、最近越多的多篇文章中，都谈及了side effect，那它到底什么是呢？在cppreference [Order of evaluation](https://en.cppreference.com/w/cpp/language/eval_order) # [Evaluation of Expressions](https://en.cppreference.com/w/cpp/language/eval_order#Evaluation_of_Expressions) 中，对它进行了讨论，因此，将它收录在本章；

## cppreference [Order of evaluation](https://en.cppreference.com/w/cpp/language/eval_order) # [Evaluation of Expressions](https://en.cppreference.com/w/cpp/language/eval_order#Evaluation_of_Expressions) 

> NOTE: 
>
> 1、"Evaluation of Expressions"是language designer规定的，language implementation需要遵循它来翻译我们的code
>
> 2、*value computations* 和 *side effects*是两个正交概念

Evaluation of each expression includes:

1、*value computations*: calculation of the value that is returned by the expression. This may involve determination of the identity of the object (glvalue evaluation, e.g. if the expression returns a reference to some object) or reading the value previously assigned to an object (prvalue evaluation, e.g. if the expression returns a number, or some other value)

> NOTE: "calculation of the value that is returned by the expression"是容易理解的

2、Initiation of *side effects*: access (read or write) to an object designated by a volatile glvalue, modification (writing) to an object, calling a library I/O function, or calling a function that does any of those operations.



## 问题: side effect 和 value computation 分别是指什么？

结合具体的例子来说明是容易理解的，最最典型的例子就是 `++` and `--` 。

### 以postfix `++` and `--` 和 prefix `++` and `--` 为例来进行说明

对于 "postfix `++` and `--` "，我们知道，它是先返回值，然后执行increment、decrement，那用cppreference evaluation of expression中的专业说法如何来进行解释呢？在 josephmansfield [C++ sequenced-before graphs](https://josephmansfield.uk/articles/c++-sequenced-before-graphs.html) 中，对此有着非常好的总结:

> 4、The value computation of postfix `++` and `--` is sequenced before their side effects.
>
> > NOTE: 也就是它们会先返回return value，然后将修改后的value写入到memory中
>
> 5、The side effects of prefix `++` and `--` are sequenced before their value computation.
>
> > NOTE: 显然，它是先将value写入到memory中，然后返回return value的



## 问题: 为什么分为两个部分?

可以看到，上述对 [Evaluation of Expressions](https://en.cppreference.com/w/cpp/language/eval_order#Evaluation_of_Expressions) 的定义分为了两个部分，那为什么要这样就行划分呢？



### 从CPU的运行机制来进行分析

下面是直观理解:

1、*value computations* 显然和 computation 相关，显然它对应的是ALU

2、*side effects* 显然和 state 相关，它对应的是memory

现状:

1、parallel computing: multicore

2、bottleneck: CPU的运行速度 远高于 access memory的速度，因此现代CPU中，每个core都有cache，分为L1 cache、L2 cache

下面是完整的分析过程: 

1、expression对应的是运算、因此最终对应的是一堆instruction

2、CPU执行的过程: memory、register、ALU

3、ALU一般从register中取得operand，它的**运行结果**也会先放到register中，这样后续的instruction就可以直接使用前一个instruction的**运行结果**

4、(猜测的)CPU的执行过程中，在完成了instruction的执行后，应该会继续执行下一条指令，而无需等待这条指令的运行结果写入到memory中，也就是说CPU应该是不会等待instruction的side effect的完成

### Value computation和side effect是正交的

有的operator只有value computation，而没有side effect，由此我们可以推断出:Value computation和side effect是正交的 ；只有部分的operator才有side effect，参见下面的"Operator with side effect"。

### 为了实现更好的performance

给予compiler更多的optimization空间，从而实现更好的performance，显然是遵循optimization principle。



## Side effect



1、Google: c++ side effects

2、stackoverflow [What exactly is a 'side-effect' in C++?](https://stackoverflow.com/questions/9563600/what-exactly-is-a-side-effect-in-c)



### Operator with side effect

1、Google: operators with side effects c++

2、修改state的operator，就是"operator with side effect"

在下面文章中，提及了"Operator with side effect"

1、josephmansfield [C++ sequenced-before graphs](https://josephmansfield.uk/articles/c++-sequenced-before-graphs.html)

> Both the assignment and the increment have side effects (i.e. they modify the value of an object).



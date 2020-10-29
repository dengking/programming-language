# Programming model

在文章`Language.md`的"如何来进行描述？"章节中，我们知道: **在不同领域，有着适合于该领域问题的问题的描述方式，这就是所谓的programming model**，下面是我总结的在一些领域的programming model，熟悉这些programming model，能够让我们以最佳实践来编写程序: 

1) parallel computing: 工程Parallel-computing的`Programming-model`章节。

2) deep learning: 工程machine-learning的`Programming\Programming-paradigm`章节。

## wikipedia [Programming model](https://en.wikipedia.org/wiki/Programming_model)

*This article is about the definition of the term 'programming model'. For classification of programming languages*, see [Programming paradigm](https://en.wikipedia.org/wiki/Programming_paradigm).

A **Programming model** refers to the style of programming where execution is invoked by making what appear to be [library](https://en.wikipedia.org/wiki/Programming_library)  calls （编程模型指的是编程风格，其中通过使看起来像是库调用来调用执行）. Examples include the [POSIX Threads](https://en.wikipedia.org/wiki/POSIX_Threads) library and Hadoop's [MapReduce](https://en.wikipedia.org/wiki/MapReduce).[[1\]](https://en.wikipedia.org/wiki/Programming_model#cite_note-1) In both cases, the [execution model](https://en.wikipedia.org/wiki/Execution_model) is different from that of the **base language** in which the code is written. For example, the [C programming language](https://en.wikipedia.org/wiki/C_programming_language) has no **execution model** for input/output or **thread behavior**. But such behavior can be invoked from C syntax, by making, what appears to be, a call to a normal C library.

What distinguishes a **programming model** from a normal library is that the behavior of the call cannot be understood in terms of the language the program is written in. For example, the behavior of calls to the POSIX thread library cannot be understood in terms of the C language. The reason is that the call invokes an **execution model** that is different from the execution model of the language. This invocation of an outside execution model is the defining characteristic of a programming *model*, in contrast to a programming *language*.

In [parallel computing](https://en.wikipedia.org/wiki/Parallel_computing), the **execution model** often must expose features of the hardware in order to achieve high performance. The large amount of variation in **parallel hardware**（并行硬件） causes a concurrent need for a similarly large number of parallel execution models（ 并行硬件的大量变化导致同时需要类似的大量并行执行模型）. It is impractical to make a new language for each execution model, hence it is a common practice to invoke the behaviors of the parallel execution model via an API（ 为每个执行模型创建一种新语言是不切实际的，因此通过API调用并行执行模型的行为是一种常见的做法）. So, most of the programming effort is done via **parallel programming models** rather than **parallel languages**. Unfortunately, the terminology around such programming models tends to focus on the details of the hardware that inspired the execution model, and in that insular world the mistaken belief is formed that a programming model is only for the case when an execution model is closely matched to hardware features.





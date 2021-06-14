# Programming paradigm



## Introduction to programming paradigm

Programming paradigm所表达的是编程的理念、思想，它指导着programming language的设计，进而影响programming language的方方面面，在programming language中，有相应的construct来实现它。在学习各种programming paradigm的时候，我觉得应该抓住各种paradigm的思想，然后再学习具体的programming language对具体programming paradigm的实现，关于这个观点，在wikipedia [Generic programming#Stepanov–Musser and other generic programming paradigms](https://en.wikipedia.org/wiki/Generic_programming#Stepanov%E2%80%93Musser_and_other_generic_programming_paradigms)中有这样的描述：

> In this article we distinguish the high-level [programming paradigms](https://en.wikipedia.org/wiki/Programming_paradigm) of *generic programming*, above, from the lower-level programming language *genericity mechanisms* used to implement them (see [Programming language support for genericity](https://en.wikipedia.org/wiki/Generic_programming#Programming_language_support_for_genericity)). 

上面这段话中的“high-level [programming paradigms](https://en.wikipedia.org/wiki/Programming_paradigm) of *generic programming*”就是generic programming这种paradigm的核心思想，而“lower-level programming language *genericity mechanisms* used to implement them ”即为programming language对它的实现。

不同programming language对programming paradigm的实现是不同的。

## wikipedia [Programming paradigm](https://en.wikipedia.org/wiki/Programming_paradigm) 

**Programming paradigms** are a way to classify [programming languages](https://en.wikipedia.org/wiki/Programming_language) based on their features. Languages can be classified into multiple paradigms.

> NOTE：它是一种对programming language进行分类的方法

Some paradigms are concerned mainly with implications for the [execution model](https://en.wikipedia.org/wiki/Execution_model) of the language, such as allowing [side effects](https://en.wikipedia.org/wiki/Side_effect_(computer_science)), or whether the sequence of operations is defined by the execution model. Other paradigms are concerned mainly with the way that code is organized, such as grouping a code into units along with the state that is modified by the code. Yet others are concerned mainly with the style of syntax and grammar.

> NOTE：不同的paradigm所关注的点是不同的

Common programming paradigms include: 

1) imperative（命令式）

in which the programmer instructs（指示） the machine how to change its state,

- [procedural](https://en.wikipedia.org/wiki/Procedural_programming) which groups instructions into procedures,

  > NOTE: procedural语言的典型代表是C

- [object-oriented](https://en.wikipedia.org/wiki/Object-oriented_programming) which groups instructions together with the part of the state they operate on,

2) declarative（陈述式）

in which the programmer merely declares properties of the desired result, but not how to compute it

- [functional](https://en.wikipedia.org/wiki/Functional_programming) in which the desired result is declared as the value of a series of function applications,
- [logic](https://en.wikipedia.org/wiki/Logic_programming) in which the desired result is declared as the answer to a question about a system of facts and rules,
- [mathematical](https://en.wikipedia.org/wiki/Mathematical_programming) in which the desired result is declared as the solution of an optimization problem

> NOTE: 目前的主流programming language基本上属于上述两大类。



[Symbolic](https://en.wikipedia.org/wiki/Symbolic_programming) techniques such as [reflection](https://en.wikipedia.org/wiki/Reflection_(computer_programming)), which allow the program to refer to itself, might also be considered as a **programming paradigm**. However, this is compatible with the major paradigms and thus is not a real paradigm in its own right.

> NOTE: 关于reflection，参见`Theory\Reflection`

For example, languages that fall into the **imperative paradigm** have two main features: they state the order in which operations occur, with constructs that explicitly control that order, and they allow **side effects**, in which **state** can be modified at one point in time, within one unit of code, and then later read at a different point in time inside a different unit of code. The communication between the **units of code** is not explicit. Meanwhile, in **object-oriented** programming, code is organized into [objects](https://en.wikipedia.org/wiki/Object_(programming)) that contain state that is only modified by the code that is part of the object. Most object-oriented languages are also imperative languages.

> NOTE：当到了现在的这个层级，就会阅读越来越多的编程的理论，如果我没有记错的话，**state**这个术语在多篇文章中出现过，在阅读[Overview](#Overview)的时候，其中有这样的一段话“ In [functional programming](https://en.wikipedia.org/wiki/Functional_programming), programs are treated as a sequence of **stateless** function evaluations.”，这突然让我想起来这是和面向对象相反的，面向对象是state的，是有side effect的；state的概念在编程语言理论是是非常重要的，下面的分析说明了state的本质: 
>
> 其实state是基于memory的，即通过memory来记录state；functional programming是stateless，这是源于他不需要memory。而OOP是有state的，所以它需要memory。

In contrast, languages that fit the **declarative paradigm** do not state the order in which to execute operations. Instead, they supply a number of operations that are available in the system, along with the conditions under which each is allowed to execute. The implementation of the language's execution model tracks which operations are free to execute and chooses the order on its own. More at [Comparison of multi-paradigm programming languages](https://en.wikipedia.org/wiki/Comparison_of_multi-paradigm_programming_languages).

### Overview

Just as [software engineering](https://en.wikipedia.org/wiki/Software_engineering) (as a process) is defined by differing *methodologies*, so the [programming languages](https://en.wikipedia.org/wiki/Programming_language) (as models of computation) are defined by differing *paradigms*. Some languages are designed to support one paradigm ([Smalltalk](https://en.wikipedia.org/wiki/Smalltalk)supports object-oriented programming, [Haskell](https://en.wikipedia.org/wiki/Haskell_(programming_language)) supports functional programming), while other programming languages support multiple paradigms (such as [Object Pascal](https://en.wikipedia.org/wiki/Object_Pascal), [C++](https://en.wikipedia.org/wiki/C%2B%2B), [Java](https://en.wikipedia.org/wiki/Java_(programming_language)), [C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language)), [Scala](https://en.wikipedia.org/wiki/Scala_(programming_language)), [Visual Basic](https://en.wikipedia.org/wiki/Visual_Basic), [Common Lisp](https://en.wikipedia.org/wiki/Common_Lisp), [Scheme](https://en.wikipedia.org/wiki/Scheme_(programming_language)), [Perl](https://en.wikipedia.org/wiki/Perl), [PHP](https://en.wikipedia.org/wiki/PHP), [Python](https://en.wikipedia.org/wiki/Python_(programming_language)), [Ruby](https://en.wikipedia.org/wiki/Ruby_(programming_language)), [Oz](https://en.wikipedia.org/wiki/Oz_(programming_language)), and [F#](https://en.wikipedia.org/wiki/F_Sharp_(programming_language))). For example, programs written in C++, Object Pascal or PHP can be purely [procedural](https://en.wikipedia.org/wiki/Procedural_programming), purely [object-oriented](https://en.wikipedia.org/wiki/Object-oriented_programming), or can contain elements of both or other paradigms. Software designers and programmers decide how to use those paradigm elements.

In object-oriented programming, programs are treated as a set of interacting objects. In [functional programming](https://en.wikipedia.org/wiki/Functional_programming), programs are treated as a sequence of **stateless** function evaluations. When programming computers or systems with many processors, in [process-oriented programming](https://en.wikipedia.org/wiki/Process-oriented_programming), programs are treated as sets of concurrent processes acting on logically shared [data structures](https://en.wikipedia.org/wiki/Data_structure).

> NOTE：上面这段中关于functional programming 的描述中的stateless与object-oriented language的state是非常具对比价值的；

**Programming paradigms** can also be compared with *[programming models](https://en.wikipedia.org/wiki/Programming_model)* which allow invoking an [execution model](https://en.wikipedia.org/wiki/Execution_model) by using only an API. **Programming models** can also be classified into paradigms, based on features of the execution model.

For [parallel computing](https://en.wikipedia.org/wiki/Parallel_computing), using a **programming model** instead of a language is common. The reason is that details of the parallel hardware leak into(泄露) the abstractions used to program the hardware. This causes the programmer to have to map patterns in the algorithm onto patterns in the execution model (which have been inserted due to leakage of hardware into the abstraction). As a consequence, no one **parallel programming language** maps well to all computation problems. It is thus more convenient to use a base sequential language and insert API calls to **parallel execution models**, via a programming model. Such parallel programming models can be classified according to abstractions that reflect the hardware, such as shared memory, distributed memory with message passing, notions of *place* visible in the code, and so forth. These can be considered flavors of programming paradigm that apply to only parallel languages and programming models.

> NOTE：上面这段中的关于在parallel computing中使用programming model而非language的解释可以结合在*[programming models](https://en.wikipedia.org/wiki/Programming_model)*解释相结合。由于the execution model of **parallel hardware** often must expose features of the hardware in order to achieve high performance， The large amount of variation in parallel hardware causes a concurrent need for a similarly large number of parallel execution models.这就导致了It is impractical to make a new language for each execution model, hence it is a common practice to invoke the behaviors of the parallel execution model via an API. 





## TODO

1) ybrikman [Six programming paradigms that will change how you think about coding](https://www.ybrikman.com/writing/2014/04/09/six-programming-paradigms-that-will/)

这篇文章对几种programming paradigm有着较好的描述。
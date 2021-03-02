# Guide

本章对C++ STL的使用理念进行总结，主要包括如下guideline: 

## Prefer algorithm over hand-write loop

1、drdobbs [STL Algorithms vs. Hand-Written Loops](https://www.drdobbs.com/stl-algorithms-vs-hand-written-loops/184401446)

主要提出了如下原因:

> There are three reasons:
>
> - **Efficiency:** Algorithms are often more efficient than the loops programmers produce.
> - **Correctness:** Writing loops is more subject to errors than calling algorithms.
> - **Maintainability:** Algorithm calls often yield code that is clearer and more straightforward than the corresponding explicit loops.

2、arne-mertz [algorithms and the KISS principle](https://arne-mertz.de/2019/05/algorithms-and-the-kiss-principle/)



3、Intentional programming

相比于`for` loop，algorithm(比如`std::for_each`、`std::transform`等)的含义是更加精准、明确，符合[intentional programming](https://en.wikipedia.org/wiki/Intentional_programming)，能够让code更加得readable。

关于这个观点，参见 stackoverflow [Should I use std::for_each?](https://stackoverflow.com/questions/6260600/should-i-use-stdfor-each) 。



## Range-based algorithm

参见 `Range-based-algorithms` 章节



## Functional programming: Higher-order function

STL algorithm是典型的使用 FP的Higher-order function。
# Design and implementation of C++

涉及的内容非常多，本文将涉及的内容进行梳理，以期作为了解C++ design 和 implementation的guideline。

## How to design programming language?

programming language norm/standard是一种contact，在进行design的时候：

1、如何来描述contact、目标的执行效果: 其实完整的描述是比较困难的，只能怪以expression、statement为例来进行说明

它需要综合考虑多种因素

1、需要提供充足的optimization空间

2、

## How to implement programming language?
compiler AS-IF。那它的基准是什么？基准是：value computation、side effect，显然基准是 programming language standard中进行描述的，那标准是如何来进行描述的呢？

C++ programming language是如何描述基准的，它留下了充足的optimization空间。

compiler能够在多大程度上了解我们的program: 它不知道multithread。

compiler会进行哪些optimization

expression、statement

compiler的局限

compiler是无法知道function是否会被multithread执行的

compiler是无法知道一个variabl是否是shared variable，即会被multithread来执行read、write


topic为: C++的设计与实现
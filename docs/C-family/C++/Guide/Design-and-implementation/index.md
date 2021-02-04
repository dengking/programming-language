# Design and implementation of C++

Design and implementation of C++涉及的内容非常多，本文将涉及的内容进行梳理，以期作为了解C++ design 和 implementation的guideline。

## How to design programming language?

Programming language norm/standard是一种contract，在进行design的时候：

一、如何来描述目标的**执行效果**: 其实完整的描述是比较困难的，只能够以expression、statement为例来进行说明。

C++ expression将目标执行效果分为:

a、value computation

b、side effect

可以看到，C++仅仅定义了目标执行效果，对于执行过程并没有进行限制，因此这就给予了implementation充分的optimization空间。

二、语言的设计需要综合考虑多种因素

1、需要提供充足的optimization空间，这样可以让compiler、hardware充分进行optimization

2、

## How to implement programming language?

对于C++而言，它的implementation主要是由compiler来完成。

### Compiler的局限

> NOTE: 首先介绍compiler的局限性

1、compiler是无法知道function是否会被multithread执行的

2、compiler是无法知道一个variabl是否是shared variable，即会被multithread来执行read、write

3、compiler能够在多大程度上了解我们的program: 它不知道multithread。

C++11引入了memory model，基于了programmer解决上述问题的手段。

### Compiler optimization 

#### Principle

compiler AS-IF。那它的基准是什么？基准是：value computation、side effect，显然基准是 programming language standard中进行描述的，那标准是如何来进行描述的呢？

C++ programming language是如何描述基准的，它留下了充足的optimization空间。

### 有哪些optimization？

compiler会进行哪些optimization

expression、statement

### Topics



#### Compiler编译的流程

构建起一个完整的编译流程，对于理解C++非常重要。

draft:

需要补充C++ compile流程，下面是素材:
https://stackoverflow.com/questions/55205874/deletion-of-copy-ctor-copy-assignment-public-private-or-protected:

Name lookup and overload resolution happen before access checking. 



#### Name lookup

参见 `Language-reference\Basic-concept\Organization\Name-lookup` 章节。



#### Implementation of polymorphism

参见 `C++\Guide\Polymorphism` 章节。
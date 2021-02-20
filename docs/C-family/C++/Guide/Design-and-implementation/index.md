# Design and implementation of C++

Design and implementation of C++涉及的内容非常多，本文将涉及的内容进行梳理，以期作为了解C++ design 和 implementation的guideline。

## How to design programming language?

Programming language norm/standard是一种contract，在进行design的时候：

一、如何来描述目标的**执行效果**: 其实完整的描述是比较困难的，只能够以expression、statement为例来进行说明。

C++ expression将目标执行效果分为:

a、value computation

b、side effect

可以看到，C++仅仅定义了目标执行效果，对于执行过程并没有进行限制，因此这就给予了implementation充分的optimization空间。

> NOTE: 这在 `Order-of-evaluation` 章节进行了描述

二、语言的设计需要综合考虑多种因素

1、需要提供充足的optimization空间，这样可以让compiler、hardware充分进行optimization

2、......


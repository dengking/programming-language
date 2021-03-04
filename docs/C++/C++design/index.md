# Design and implementation of C++

Design and implementation of C++涉及的内容非常多，本文将涉及的内容进行梳理，以期作为了解C++ design 和 implementation的guideline。

## How to design C++ programming language?

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



## C++语言的设计者的设计原则

### C++是一门安全、自由、灵活的语言

它给予programmer极大的自由，此处所谓的**自由**是指programmer有着很大的**选择**空间，或者说它并没有给予programmer过多的限制。

[正如在如何评价 C++11 的右值引用（Rvalue reference）特性？ - zihuatanejo的回答 - 知乎]( https://www.zhihu.com/question/22111546/answer/31929118) 中作者所援引的：

正如Bjarne Stroustrup所说，

> "C++的许多设计决策根源于我对强迫人按某种特定方式行事的极度厌恶。"

> "当我试图去取缔一个我不喜欢的语言特征时，我总抑制住这样的欲望，因为我认为我无权把个人观点强加给别人。"

选择是自主权的表现，向往自由的人一定不会认为有选择是件坏事。

在http://thbecker.net/articles/rvalue_references/section_04.html中有这样的总结：

> As we all know, the First Amendment to the C++ Standard states: "The committee shall make no rule that prevents C++ programmers from shooting themselves in the foot." Speaking less facetiously, when it comes to choosing between giving programmers more control and saving them from their own carelessness, C++ tends to err on the side of giving more control. Being true to that spirit, C++11 allows you to use move semantics not just on rvalues, but, at your discretion, on lvalues as well. A good example is the `std` library function `swap`. As before, let `X` be a class for which we have overloaded the copy constructor and copy assignment operator to achieve move semantics on rvalues.

验证上面这段话所表达的思想的另外一个例子是：[`const_cast`](https://en.cppreference.com/w/cpp/language/const_cast) 

C++的安全性体现在compiler进行了严格的类型检查，对于不符合规范的program，compiler会给出报错提示。同时C++基于了programmer执行危险操作的权利，不过C++采用的“explicit”的策略，即需要由programmer“explicit”地来执行，而不是由compiler“implicit”地执行。

C++语言就是在这样的发展中，逐渐变大庞杂。

### C++是兼容并包的

c++提供了极大的选择空间，这就意味着它需要提供各种各样的program technique。下面是典型的例子：

1、C++语言的概念比其他的programming language要多得多：比如在python、java，它们仅仅有reference semantic，而c++囊括了：value semantic、reference semantic。



### Prefer introducing new features via the standard library

c++语言是在不断地演进的，那c++标准委员会的专家们是以什么样的原则来不断地更新这么语言呢？这个问题在isocpp What specific design goals guided the committee? [¶](https://isocpp.org/wiki/faq/cpp11#cpp11-specific-goals) [Δ](https://isocpp.org/wiki/faq/cpp11#)中给出了答案，在维基百科[C++11](https://en.wikipedia.org/wiki/C%2B%2B11)的[Design goals](https://en.wikipedia.org/wiki/C%2B%2B11#Design_goals)章节也引用了这个答案，我觉得其中非常重要的一点是：

> Prefer introducing new features via the standard library, rather than extending the core language

上述standard library，更加具体来说就是[Language support library](https://en.cppreference.com/w/cpp/utility#Language_support)。

例子包括:

1 `std::swap`

2 `std::declval`

3 `std::invoke`

> NOTE: tag: prefer standard library over extending the core language





## Feature of C++



在c和c++中，需要由programmer来管理[object](https://en.cppreference.com/w/cpp/language/object ) 的lifetime、storage duration。object的lifetime有：

- global

- stack

- heap

c++的smart pointer library就是用来帮助programmer管理位于heap的object的。

在带有GC的programming language中，programmer是无需关注这些内容的。

c++囊括了：

### Value semantic and reference semantic

在`Language-reference\Basic-concept\Reference-and-Value-Semantics`中对此进行了说明。

### Compile-time and run-time

在`C-and-C++\Compile-time-and-run-time`中对此进行描述

### Static and dynamic 

#### static type and dynamic type

对于polymorphism object，它有static type和dynamic type，关于此，在如下章节中对此进行了说明：

- `C++\Language-reference\Basic-concept\Data-model\Object\Object.md#`[Polymorphic objects](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects)
- `C++\Language-reference\Basic-concept\Type-system\Type-system\Type-system.md#Static type and dynamic type`

对于polymorphic object，如何获得它的static type？如何获得它的static type？这些C++都是支持的。

### Programming paradigm

1、OOP

2、generic programming

3、template meta programming

4、......

#### Explicit and implicit

`explicit`供programmer对conversion进行控制；



### Automatic

为了提供C++的易用性，C++ compiler automatic地执行了很多操作，同时它也提供了供programmer将automatic操作取消掉的权利。如下是一些例子：

- C++允许programmer控制是否进行template argument deduction；


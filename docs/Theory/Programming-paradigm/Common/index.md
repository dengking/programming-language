# 关于本章

本章讨论各种programming paradigm都可能会涉及到的topic。



## 循序渐进地实现

Programming language的设计者在设计实现某种paradigm的时候，需要考虑的问题是非常之多的，工作量也是非常之大的，所以它可能会将对某种paradigm的实现分为多个版本来进行实现: 在某个版本中提供初级的实现，然后在后续版本中提供更多的、高级的实现。一个典型的例子就是C++对generic programming的实现，generic programming的核心思想中明确地提出了concept，但是c++知道C++20版本才提供显式定义concept的实现，在此之前的版本，用户只能够通过`SFINAE`、`enable_if`等来间接地描述concept。关于这一点是我在阅读[Generic Programming in ConceptC++](http://www.generic-programming.org/languages/conceptcpp.html)时，受到的启发

> ConceptC++ is an extension to the C++ Programming Language that introduces first-class support for Generic Programming. It is the first attempt to introduce **complete** support for Generic Programming into a mainstream programming language. 

## 各种programming paradigm都有着自己的优势和劣势

参见 `Theory\Programming-paradigm\Comparison` 章节

## 博采众长，充分发挥各种programming paradigm的优势

1、前面我们知道每种paradigm都有着各自的优势，能够解决一些问题，所以现代programming language往往会采用多种paradigm，即“博采众长”，显然，这样的做法，让programming language更加地强大，让programmer通过灵活运用、结合多种paradigm从而充分发挥programming language的powered。

2、现代programming language往往是multi-paradigm的，这样能够极大的增强语言的power，比如C++、Python同时支持OOP、generic programming；

下面是阐述了这个思想的素材:

1、stackoverflow [Aspect Oriented Programming vs. Object-Oriented Programming](https://stackoverflow.com/questions/232884/aspect-oriented-programming-vs-object-oriented-programming) # [A](https://stackoverflow.com/a/232918)

> Why "vs"? It is not "vs". You can use Aspect Oriented programming in combination with functional programming, but also in combination with Object Oriented one.

当然，也不可避免的会带来programming language的复杂度的提高，一个典型的例子就是C++，它的两个典型的paradigm是OOP和generic programming。

2、visitor pattern是FP、OOP的结合的典型；

3、[CppCoreGuidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines) [T.5: Combine generic and OO techniques to amplify their strengths, not their costs](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rt-generic-oo)

4、artima [On the Tension Between Object-Oriented and Generic Programming in C++](https://www.artima.com/cppsource/type_erasure.html)

## 理解各种programming paradigm的核心思想

每一种programming paradigm都告诉了我们一种思考、抽象问题的方法，比如:

1、AOP：对于流程，找到横切关注点

2、generic programming: 抽象出类型，使用concept进行描述

3、OOP: object、class



## 各种programming paradigm中都需要考虑的问题

各种programming paradigm都会涉及如下问题: 如何抽象、如何实现code reuse，如何实现polymorphism: 

1、abstraction 

2、polymorphism

Polymorphism在各种programming paradigm都有体现: generic programming中体现了polymorphism、OOP中也体现了polymorphism；

3、code reuse

4、information hiding

所有的programming paradigm都需要考虑information hiding，OOP中采用的encapsulate

5、extendibility

如何进行可扩展


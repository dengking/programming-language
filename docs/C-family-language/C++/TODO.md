# TODO

## trailing return type

越来越多的programming language支持trailing return type，比如:

1) C++

2) python

3) Rust



## C++ RVO 和 prvalue

这是我一直以来的疑惑，上周又碰到了，原因是在阅读[Lifetime](https://en.cppreference.com/w/cpp/language/lifetime)的[Temporary object lifetime](https://en.cppreference.com/w/cpp/language/lifetime)章节的时候，其中有这样的描述:

> returning a prvalue from a function



于是我Google: "returning a prvalue from a function C++"，得到的回答中，大多数是在讨论C++ RVO 和 prvalue的，下面是我觉得讨论的比较好的:

- stackoverflow [C++ return an 'prvalue', from a function with a formal param 'rvalue' reference with assgined 'prvalue'?](https://stackoverflow.com/questions/27115814/c-return-an-prvalue-from-a-function-with-a-formal-param-rvalue-reference)

- cppreference [Copy elision](https://en.cppreference.com/w/cpp/language/copy_elision)

对RVO进行的讨论

- stackoverflow [c++11 Return value optimization or move? [duplicate]](https://stackoverflow.com/questions/17473753/c11-return-value-optimization-or-move)

- stackoverflow [C++11 rvalues and move semantics confusion (return statement)](https://stackoverflow.com/questions/4986673/c11-rvalues-and-move-semantics-confusion-return-statement)

这篇文章其实是已经收录的

### value semantic

显而易见的是: 两者都和value semantic有关，显然它们都是C++对value semantic的支持。

## Lifetime of a temporary

需要将[Reference initialization](https://en.cppreference.com/w/cpp/language/reference_initialization)中"Lifetime of a temporary"章节的内容移到lifetime章节。



## Lifetime of OOP object

在OOP object的lifetime中，它的initialization是涉及到非常多的问题的，下面是我需要进一步进行汇总整理的:

### destruction

Destruction sequence:

参见[Destructors#Destruction sequence](https://en.cppreference.com/w/cpp/language/destructor)

### construction

Initialization order:

[Constructors and member initializer lists#Initialization order](https://en.cppreference.com/w/cpp/language/constructor#Initialization_order)



### Calling virtual during construction and destruction

[During construction and destruction](https://en.cppreference.com/w/cpp/language/virtual#During_construction_and_destruction)
# C++ paradigm

C++是一种multi-paradigm programming language，下面罗列了C++支持的programming paradigm以及每种paradigm的实现。在`Idiom`章节总结了各种paradigm的idiom。

## OOP: Object-oriented programming

C++直接支持OOP，C++OOP是class-based OOP，参见 `OOP` 章节



## Generic programming and template-meta programming

C++直接支持generic programming 和 template-meta programming，它们都是通过template来实现的，在Wikipedia [Generic programming](https://en.wikipedia.org/wiki/Generic_programming) # [Templates in C++](https://en.wikipedia.org/wiki/Generic_programming#Templates_in_C++) 中有这样的描述: 

> C++ uses templates to enable **generic programming** techniques. The C++ Standard Library includes the [Standard Template Library](https://en.wikipedia.org/wiki/Standard_Template_Library) or STL that provides a framework of templates for common data structures and algorithms. Templates in C++ may also be used for [template metaprogramming](https://en.wikipedia.org/wiki/Template_metaprogramming), which is a way of pre-evaluating some of the code at compile-time rather than [run-time](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase)). Using **template specialization**, C++ Templates are considered [Turing complete](https://en.wikipedia.org/wiki/Turing_complete).

C++通过template来实现:

1) generic programming

2) template metaprogramming

关于template，参见`C++\Language-reference\Template`章节。

### Generic programming

参见如下章节:

1、`C++\Programming-paradigm\Generic-programming`

2、`C++\Language-reference\Functions\Generic-programming`



### TMP: Template-meta programming

参见如下章节:

1、`C++\Programming-paradigm\GP-and-TMP\TMP`



## AOP

C++并不直接支持AOP，但是C++可以使用AOP的思想来编写更加抽象的code，参见 `AOP` 章节。



## Idiom

在`C++\Idiom`中总结了各种paradigm的idiom。
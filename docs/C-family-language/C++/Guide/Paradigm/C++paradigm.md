# C++ paradigm

C++是一种multi-paradigm programming language，下面罗列了c++支持的paradigm以及每种paradigm的实现。在`Idiom`章节总结了各种paradigm的idiom。

## Object-oriented programming

C++直接支持OOP，C++OOP是class-based OOP。



## Generic programming and template-meta programming

C++直接支持generic programming 和 template-meta programming，它们都是通过template来实现的，参见`C++\Language-reference\Template`章节。

### Generic programming

在`Theory\Programming-paradigm\Generic-programming`中讨论了generic programming，其中所描述的是思想，其中已经涉及了C++中的generic programming。

在C++ Core Guidelines: [Templates and generic programming](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-templates)中，对C++中Generic programming的介绍如下:

> Generic programming is programming using types and algorithms parameterized by types, values, and algorithms. In C++, generic programming is supported by the `template` language mechanisms.

简而言之，C++中的generic programming是通过template来实现的。

在 boost [Generic Programming Techniques](https://www.boost.org/community/generic_programming.html) 介绍了一些GP technique。

我觉得关于C++ GP介绍的最好的文章是：boost [Generic Programming Techniques](https://www.boost.org/community/generic_programming.html)。

在gotw [Two-Phase or Not Two-Phase: The Story of Dependent Names in Templates](http://www.gotw.ca/gotw/087.htm)中有这样的描述:

> *In the land of C++, there are two towns: The village of traditional nontemplate C++ code, and the hamlet of templates.* 

### Template-meta programming

参见`C++\Language-reference\Template`章节。



## AOP

C++并不直接支持AOP，但是C++可以使用AOP的思想来编写更加抽象的code。

## 发展情况

感觉C++ 在 OOP上，并没有太多的发展，而在generic programming、template-meta programming上，依然不断地引入新的特性。

## Idiom

在`C++\Idiom`中总结了各种paradigm的idiom。
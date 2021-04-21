# 提供 uniform、consistent的interface、API、syntax

C++的一个非常重要的方向是提供 uniform、consistent的interface、API、syntax。

下面是一些分析: 

1、uniform、consistent促进了对各种差异的abstraction，充分发挥polymorphism，遵循"Program to an abstraction and polymorphism"，促进了generic programming。

2、促进创建good abstraction

3、uniform、consistent简化了C++

4、统一的描述方式(syntax)，其实和一致性接口类似，最最典型的例子就是: uniform initialization。

主要包括如下方面:

## Syntax

对很多implementation-specific进行uniform，提供uniform/consistent syntax，这包括:

1) C++ 11 attribute: `C++\Language-reference\Attribute`

C++11 attribute就是对attribute的统一

2) C++11 uniform initialization: `C++\Language-reference\Initialization\Uniform-initialization`

3) C++11 universal reference、perfect forwarding

## Interface、API

对复杂的、常用的运算进行uniform，提供uniform/consistent interface，主要包含如下方面:

### Generic programming

1、generic programming中通过统一的operator，比如`decltype`，`std::declval`，`trait`

2、"统一"意味着"抽象"，因为着需要polymorphism。

下面是一些例子:

**Callable**

1) C++17 `std::invoke`

http://www.cplusplus2017.info/c17-stdinvoke-stdapply/ 

> `std::invoke` it’s universal interface for calling anything it’s callable.

2) `std::decay`



### Language support interface、API

`std::swap` 是C++ programming language支持的

`std::move` 支持move semantic



## Good article

本节总结一些涉及这个topic的一些非常好的文章: 

### More C++ Idioms [Non-throwing swap](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-throwing_swap)

> To provide uniform interface to it to facilitate generic programming



> Although an efficient and exception-safe `swap` function can be implemented (as shown above) as a member function, non-throwing swap idiom goes further than that for **simplicity**, **consistency**, and to facilitate **generic programming**. 


# Execution model

在工程[Linux-OS](https://dengking.github.io/Linux-OS/)中总结了 [Process execution model](https://dengking.github.io/Linux-OS/Kernel/Guide/Linux-OS's-multitasking/Process-model/Process-model/)，它是通用的execution model，对使用C++编写的program同样适用；但是，对于C++ program，它也有一些个性化的特性，这些特性是需要C++ programmer掌握的，本文对C++中于此相关的内容进行梳理，建立起一个更加完整的C++ execution model，下面是于此相关的内容:

## 使用scope来进行描述

Scope是一个静态/compile-time概念，C++中很多dynamic/runtime概念，其实也是基于它而创建的，因此，后面的内容中，很多都和scope相关。

## Object lifetime



### Scope and object lifetime

关于scope和object lifetime，参见:

1) [Declarations](https://en.cppreference.com/w/cpp/language/declarations)的Notes段中有描述

2) 参见 `C++\Language-reference\Basic-concept\Object\Lifetime-and-storage-duration` 章节

### Scope and resource management

c++中的RAII就是基于scope的，所以c++的scope和resource management之间有着强烈的关联。



## Function execution

> NOTE: 在阅读本章内容之前，需要有如下认知: 
>
> 1、cppreference [Statements](https://en.cppreference.com/w/cpp/language/statements#Compound_statements)
>
> > Compound statements or *blocks* are brace-enclosed sequences of statements.
>
> C++ block是compound statement的同义词
>
> 1、cppreference [Scope # Block scope](https://en.cppreference.com/w/cpp/language/scope#Block_scope)
>
> 3、cppreference [Function declaration](https://en.cppreference.com/w/cpp/language/function)

1、Function execution有**stack frame**，当function termination的时候，会执行stack unwinding；

2、Function block/scope中的每个block都有自己是一个stack frame，它的stack frame是nested in enclosing block的stack frame的。当block的scope结束时，会执行stack unwinding；

> 因此可以说，C++中每个block(scope)都有一个自己的stack。



基于scope的

基于stack unwinding的



每个function scope都 有自己的stack

在function scope中的block scope

在scope exit的时候，会执行stack unwinding，C++ 的很多feature都是建立在stack unwinding上；

1、C++ execution model，最最典型的是scope exit

2、C++ object lifetime



对于与function Invokation相关的，大多数 都是可以使用scope来进行刻画
# Execution model

在工程[Linux-OS](https://dengking.github.io/Linux-OS/)中总结了 [Process execution model](https://dengking.github.io/Linux-OS/Kernel/Guide/Linux-OS's-multitasking/Process-model/Process-model/)，它是通用的execution model，对使用C++编写的program同样适用；但是，对于C++ program，它也有一些个性化的特性，这些特性是需要C++ programmer掌握的，本文对C++中于此相关的内容进行梳理，建立起一个更加完整的C++ execution model，下面是于此相关的内容:

## 使用scope来进行描述

Scope是一个静态/compile-time概念，但是C++中很多**dynamic/runtime概念**，其实也是基于它而创建的，从后面的内容可以看到，很多**dynamic/runtime概念**都和scope相关，或者说是**一致**的，这种static(program static structure)和dynamic(object lifetime、execution)的**一致性**，能够帮助programmer理解program的execution。

## Function execution

> NOTE: 在阅读本章内容之前，需要有如下认知: 
>
> 1、cppreference [Statements](https://en.cppreference.com/w/cpp/language/statements#Compound_statements)
>
> > Compound statements or *blocks* are brace-enclosed sequences of statements.
>
> C++ block是compound statement的同义词
>
> 2、cppreference [Scope # Block scope](https://en.cppreference.com/w/cpp/language/scope#Block_scope)
>
> 每个block有一个独立的scope
>
> 3、cppreference [Function declaration](https://en.cppreference.com/w/cpp/language/function)
>
> 每个function有一个独立的scope

1、Function execution有**stack frame**，当function termination的时候，会进行**stack unwinding**；

**一致性: function termination->scope exit**

2、可以认为: function中的每个**block**都有自己独立的**stack frame**，它的stack frame是nested in enclosing block的stack frame的。当block结束时，会执行stack unwinding；

**一致性: block termination->scope exit**

> thought: C++中每个block(scope)都有一个自己的stack frame。

因此: 在function execution中: 

1、**scope enter**时，入栈，allocation，construct object，constructor被执行

2、**scope exit**时，出栈，执行stack unwinding，deallocation，destruct object，destructor被执行；



由于C++中，scope是**nesting**的，因此上述过程也是**nesting**的；

> thought: 对于与function Invokation相关的，大多数 都是可以使用scope来进行刻画

在scope exit的时候，会执行stack unwinding，C++ 的很多feature都是建立在stack unwinding上；





## Scope and object lifetime

关于scope和object lifetime，参见:

1) [Declarations](https://en.cppreference.com/w/cpp/language/declarations)的Notes段中有描述

2) 参见 `C++\Language-reference\Basic-concept\Object\Lifetime-and-storage-duration` 章节

3、cppreference [Storage class specifiers](https://en.cppreference.com/w/cpp/language/storage_duration)

> Together with the [scope](https://en.cppreference.com/w/cpp/language/scope) of the name, they control two independent properties of the name: its *storage duration* and its *linkage*.



## Scope based idiom

C++对stack unwind有着非常充分的运用，很多idiom都是建立在stack unwind之上的。

一、RAII

C++中的RAII和scope密切相关，所以C++的scope和resource management之间有着强烈的关联。

基于RAII的:

1、scope guard



### `scoped_***`

boost [`scoped_ptr`](http://www.boost.org/doc/libs/1_44_0/libs/smart_ptr/scoped_ptr.htm)

cppreference [std::scoped_lock](https://en.cppreference.com/w/cpp/thread/scoped_lock)

boost [Boost ScopeExit](http://www.boost.org/doc/libs/release/libs/scope_exit/doc/html/index.html)

[boost::mutex::scoped_lock](http://www.google.com/codesearch/p?hl=en#so90T49b54s/przyklady/cpprec.zip|g3JV5CE7tsM/12-02.cpp&q=mutex::scoped_lock)




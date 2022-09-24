# Resource ownership

## Rule: 明确Resource ownership

C++ resource management中非常重要的一点是: 明确Resource ownership，这一点无论是在C++ language level、CppCoreGuidelines [R: Resource management](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#S-resource) 都有体现: 

**C++ language level**

1、move semantic

**C++ STL**

1、smart pointer

CppCoreGuidelines [R: Resource management](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#S-resource) :

1、提出了"owner"的概念

2、 [R.1: Manage resources automatically using resource handles and RAII (Resource Acquisition Is Initialization)](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-raii) 其中有"takes over ownership of the pointer"的描述

3、[R.3: A raw pointer (a `T*`) is non-owning](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#r3-a-raw-pointer-a-t-is-non-owning)



### Owning and non-owning  

> NOTE:
>
> 一、TODO: C++three-ownership-model
>
> 二、C++non-owning-type

这是在 CppCoreGuidelines [R.2: In interfaces, use raw pointers to denote individual objects (only)](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-use-ptr) 的下面这段话: 

> Arrays are best represented by a container type (e.g., `vector` (owning)) or a `span` (non-owning). Such containers and views hold sufficient information to do range checking.

中提出的，它提示我们: 需要区分Owning and non-owning  。

### Shared 和 unique

shared 和 unique，其实表达了对resource的ownership 语义；

reader lock是shared，write lock是unique的；

在C++标准库中，有如下使用: 

|                   | unique                                                       | shared                                                       |
| ----------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| future            | [std::future](https://en.cppreference.com/w/cpp/thread/future) | [std::shared_future](https://en.cppreference.com/w/cpp/thread/shared_future) |
| pointer to object | [std::unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr) | [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr) |
| lock              | [std::unique_lock](https://en.cppreference.com/w/cpp/thread/unique_lock) | [std::shared_lock](https://en.cppreference.com/w/cpp/thread/shared_lock) (C++14) |

在下面这篇非常好的文章中，对这个topic进行了非常好的总结: 

1、akrzemi1 [Unique and shared ownership](https://akrzemi1.wordpress.com/2011/06/27/unique-ownership-shared-ownership/)



## 对resource ownership的操作

总的来说，对object的operation主要两种:

1、copy

2、move

我们可以从resource ownership的角度来分析copy、move。

### move

一、move对应的是transfer ownership，它需要将resource的ownership transfer走，因此在source object中，需要表明它已经不再own resource了，source object就不再release resource。

> CppCoreGuidelines-C.64 A move operation should move and leave its source in a valid state



二、transfer/take over ownership

> NOTE: 
>
> 1、"take over ownership"是我在阅读 CppCoreGuidelines [R.1: Manage resources automatically using resource handles and RAII (Resource Acquisition Is Initialization)](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-raii) 时，其中提出的。



### copy

deep copy: 显然是重新acquire resource了

shallow copy: 

显然表达的是shared ownership的，需要注意的是，raw pointer是无法表达shared ownership，`std::shared_ptr`是可以的。


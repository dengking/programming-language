# Resource ownership

## Rule: 明确Resource ownership

C++ resource management中非常重要的一点是: 明确Resource ownership，这一点无论是在C++ language level、CppCoreGuidelines [R: Resource management](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#S-resource) 都有体现: 

C++ language level:

1、move semantic

C++ STL:

1、smart pointer

CppCoreGuidelines [R: Resource management](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#S-resource) :

1、提出了"owner"的概念

### Owning and non-owning  

这是在 CppCoreGuidelines [R.2: In interfaces, use raw pointers to denote individual objects (only)](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-use-ptr) 的下面这段话: 

> Arrays are best represented by a container type (e.g., `vector` (owning)) or a `span` (non-owning). Such containers and views hold sufficient information to do range checking.

中提出的，它提示我们: 需要区分Owning and non-owning  。

### shared 和 unique

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

1、share ownership

2、transfer/take over ownership

### Move semantic and ownership

transfer ownership后，原来的object就不再拥有了，则就不应该再使用它们了。
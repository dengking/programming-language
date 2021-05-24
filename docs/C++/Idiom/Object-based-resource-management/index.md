# Object-based resource management

一、本节标题的含义是: 基于object的resource management。

二、本章讨论C++中实现object-based resource management的各种idiom。

通过C++中的各种established idiom，来实现strong resource safety: strong exception safety: 不会resource leak

## Object-based resource management 



### C++ value semantic是Object-based resource management的前提

Object是C++的data model、C++允许programmer对object lifetime进行control、参见 zhihu zhihu [如何评价 C++11 的右值引用（Rvalue reference）特性？ - zihuatanejo的回答 - 知乎](https://www.zhihu.com/question/22111546/answer/31929118)，简而言之就是: C++支持value semantic。这是使用"Object-based resource management"的前提条件。



### Object-based resource management: 将object 和 resource进行bind

1、将object 和 resource进行bind，然后就可以基于object来对resource进行management，关于此的非常好的论述是: 

CppCoreGuidelines [R.1: Manage resources automatically using resource handles and RAII (Resource Acquisition Is Initialization)](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-raii)

2、C++中很多resource相关的idiom都是基于"Object-based resource management"思想的

3、C++中，一般不会直接使用一个resource，而是会将它bind到一个object



### Example

1、Smart pointer

`std::share_ptr`、`std::smart_ptr`都是基于这个思路的: 

`std::share_ptr`: reference count 等于 live object 个数

`std::smart_ptr`: resource 和 object 直接绑定

2、resource wrapper



## 明确resource ownership

对于resource management，需要resource ownership，这在 `Copyable-and-movable` 中进行了深入讨论。





## Resource wrapper

使用"Single responsibility+rule of three/five/zero+RAII"来实现Resource wrapper。



## Copyable and movable

参见 `Copyable-and-movable` 章节。

## Idioms

### RAII

一、避免leakage

参见 `RAII` 章节。

### Rule of three/five/zero

参见 `Rule-of-zero-three-five` 章节。



### Copy-and-swap-idiom

参见 `Copy-and-swap-idiom` 章节。

### Scope-guard

参见 `Scope-guard` 章节。

### Resource-return

参见 `Resource-return` 章节。


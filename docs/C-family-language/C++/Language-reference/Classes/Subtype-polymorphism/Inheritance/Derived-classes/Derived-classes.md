# Derived classes

本文讨论C++中的这些问题：

- 多继承

## cppreference [Derived classes](https://en.cppreference.com/w/cpp/language/derived_class)

Any class type (whether declared with *class-key* `class` or `struct`) may be declared as *derived* from one or more *base classes* which, in turn, may be derived from their own base classes, forming an **inheritance hierarchy**.

> NOTE: C++ 支持multiple inheritance，这就使得它的inheritance hierarchy不是tree shape



### Virtual base classes
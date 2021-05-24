# Derived classes



## cppreference [Derived classes](https://en.cppreference.com/w/cpp/language/derived_class)

Any class type (whether declared with *class-key* `class` or `struct`) may be declared as *derived* from one or more *base classes* which, in turn, may be derived from their own base classes, forming an **inheritance hierarchy**.

> NOTE: C++ 支持multiple inheritance，这就使得它的inheritance hierarchy不是tree shape，而可能出现[diamond problem](https://en.wikipedia.org/wiki/Diamond_problem) 

Each direct and indirect base class is present, as *base class subobject*, within the object representation of the derived class at implementation-defined offset. Empty base classes usually do not increase the size of the derived object due to [empty base optimization](https://en.cppreference.com/w/cpp/language/ebo). The constructors of base class subobjects are called by the constructor of the derived class: arguments may be provided to those constructors in the [member initializer list](https://en.cppreference.com/w/cpp/language/initializer_list).

> NOTE: 上面这段话是对object layout的讨论，关于object layout，在下面章节中进行了讨论：
>
> - `C++\Language-reference\Basic-concept\Data-model\Object`

### Virtual base classes

> NOTE: 这部分内容放到了`Multiple-inheritance.md`中。

### Public inheritance

### Protected inheritance

### Private inheritance
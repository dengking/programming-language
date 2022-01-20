# Dynamic memory management

C++的核心概念是[Object](https://en.cppreference.com/w/cpp/language/object)，object有[storage duration](https://en.cppreference.com/w/cpp/language/storage_duration)、[lifetime](https://en.cppreference.com/w/cpp/language/lifetime)属性，dynamic object就是其中一种，C++ Dynamic memory management library就是帮助programmer来管理dynamic object的。



## cppreference [Dynamic memory management](https://en.cppreference.com/w/cpp/memory)

> NOTE: 
>
> dynamic memory management主要可以分为如下几个模块:
>
> 一、Smart pointers
>
> 二、Allocators
>
> 三、Memory resources
>
> 四、Uninitialized storage
>
> 五、Garbage collector support

### Smart pointers

Smart pointers enable automatic, exception-safe, object lifetime management.

### Allocators

Allocators are class templates encapsulating memory allocation strategy. This allows generic containers to decouple memory management from the data itself.

### Memory resources

Memory resources implement memory allocation strategies that can be used by [std::pmr::polymorphic_allocator](https://en.cppreference.com/w/cpp/memory/polymorphic_allocator)

> NOTE: 
>
> 用于支持C++17的polymorphic allocator

### Uninitialized storage

Several utilities are provided to create and access raw storage.

> NOTE: 
>
> 它们可以用于实现memory pool

#### Uninitialized memory algorithms



#### Constrained uninitialized memory algorithms (C++20)

C++20 provides [constrained](https://en.cppreference.com/w/cpp/language/constraints) uninitialized memory algorithms that accept range arguments or iterator-sentinel pairs.

> NOTE: 
>
> 它本质上其实就是基于range的
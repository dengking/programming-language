# Class memory layout

本章探讨:

1、C++如何编排class memory layout

2、如何dump class memory layout

## C++如何编排class memory layout

需要考虑的问题:

一、class hierarchy 

1、需要考虑各种可能的inheritance

二、member order



三、alignment



四、implementation of polymorphism



五、implementation of RTTI



下面是涉及这个topic的素材: 

### stackoverflow [memory layout C++ objects [closed]](https://stackoverflow.com/questions/1632600/memory-layout-c-objects)

[A](https://stackoverflow.com/a/27682344)

Memory layout is mostly left to the implementation. The key exception is that member variables for a given access specifier will be in order of their declaration.

Other than member variables, a class or struct needs to provide space for member variables, subobjects of base classes, virtual function management (e.g. a virtual table), and padding and alignment of these data. This is up to the implementation but the **Itanium ABI** specification is a popular choice. gcc and clang adhere to it (at least to a degree).

> NOTE: 
>
> 1、上述总结非常好

### wikipedia [Virtual inheritance](https://en..org/wiki/Virtual_inheritance)



### cppreference [Object # Subobjects](https://en.cppreference.com/w/cpp/language/object#Subobjects)
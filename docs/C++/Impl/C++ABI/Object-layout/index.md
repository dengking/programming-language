# Class memory layout

本章探讨:

一、What is object layout

二、C++标准中，关于object layout的一些概念

三、C++如何编排object memory layout

四、如何dump class memory layout

## What is object layout

在`C++\Language-reference\Basic-concept\Data-model\Object\Object.md`中说明了object，我们知道: 

> object本质上是a region of storage

现在我们思考这个问题：C++ compiler如何来编排object的"region of storage"？这就是**object layout**？其实这个问题涉及到了C++ ABI，下面是object layout需要考虑的：

| 考虑内容                                              | 说明                                                         |
| ----------------------------------------------------- | ------------------------------------------------------------ |
| [endianess](https://en.wikipedia.org/wiki/Endianness) | 这在工程hardware的`CPU\Endianess`章节对此进行了说明          |
| alignment                                             | `C++\Language-reference\Basic-concept\Data-model\Object\Object.md`的"Alignment"章节 |
| C++提供的很多高级特性的实现                           | 比如:<br>- polymorphic type，polymorphic type有[virtual functions](https://en.cppreference.com/w/cpp/language/virtual)，<br>需要RTTI、virtual method table<br>- [virtual base classes](https://en.cppreference.com/w/cpp/language/derived_class#Virtual_base_classes) |
| compiler optimization                                 |                                                              |
| subobjects                                            | `C++\Language-reference\Basic-concept\Data-model\Object\Object.md`的的“subobjects”有对它的描述；<br>在cppreference [Derived classes](https://en.cppreference.com/w/cpp/language/derived_class)中，有对它的讨论 |
| platform                                              | 需要考虑平台相关的信息                                       |
| ......                                                |                                                              |

> TODO: 
>
> 需要考虑的问题:
>
> 一、class hierarchy 
>
> 1、需要考虑各种可能的inheritance
>
> 二、member order
>
> 三、alignment
>
> 四、implementation of polymorphism
>
> 五、implementation of RTTI

需要考虑的问题非常多，出于各种考虑，C++标准并没有对object layout的方方面面都进行统一规定，而是将一些留给了C++ implementation去自由地选择。关于这一点，在文章microsoft [Trivial, standard-layout, POD, and literal types](https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019)中进行了说明：

> The term *layout* refers to how the members of an object of class, struct or union type are arranged in memory. In some cases, the layout is well-defined by the language specification. But when a class or struct contains certain C++ language features such as [virtual base classes](https://en.cppreference.com/w/cpp/language/derived_class#Virtual_base_classes), [virtual functions](https://en.cppreference.com/w/cpp/language/virtual), members with different access control, then the compiler is free to choose a **layout**. That **layout** may vary depending on what optimizations are being performed and in many cases the object might not even occupy a contiguous area of memory. 



关于polymorphic type，参见：

- `C++\Language-reference\Basic-concept\Type-system\Type-system\Type-system#Polymorphic type`



## C++如何编排class memory layout

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
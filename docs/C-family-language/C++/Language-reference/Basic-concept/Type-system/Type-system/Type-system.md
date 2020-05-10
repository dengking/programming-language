# Type system



## cppreference [Type](https://en.cppreference.com/w/cpp/language/type)

### [Type classification](https://en.cppreference.com/w/cpp/language/type#Type_classification)

> NOTE: 对type的classification是有多种方法的，原文的这一段就提供了多种划分方法，下面是原文主要描述的一种划分方法

[fundamental types](https://en.cppreference.com/w/cpp/language/types) (see also [std::is_fundamental](https://en.cppreference.com/w/cpp/types/is_fundamental))

compound types (see also [std::is_compound](https://en.cppreference.com/w/cpp/types/is_compound))



> NOTE: 通过原文可以看到，从c++11开始，每种类型都通过了对应的trait



### [Type naming](https://en.cppreference.com/w/cpp/language/type#Type_naming)





### [Elaborated type specifier](https://en.cppreference.com/w/cpp/language/type#Elaborated_type_specifier)



### [Static type](https://en.cppreference.com/w/cpp/language/type#Static_type)

​	

### [Dynamic type](https://en.cppreference.com/w/cpp/language/type#Dynamic_type)

If some [glvalue expression](https://en.cppreference.com/w/cpp/language/value_category) refers to a [polymorphic object](https://en.cppreference.com/w/cpp/language/object), the type of its **most derived object** is known as the dynamic type.

> NOTE: 关于上面这段话中的**most derived object**，参见cppreference [Object#Subobjects](https://en.cppreference.com/w/cpp/language/object#Subobjects)



> NOTE: static type和dynamic type的概念对于c++ 非常重要，关于两者的一个典型例子就是：c++中如何实现multiple dispatch，参见[A polyglot's guide to multiple dispatch](https://eli.thegreenplace.net/2016/a-polyglots-guide-to-multiple-dispatch/)，在`Theory\Programming-paradigm\Object-oriented-programming\Polymorphism\Subtyping-polymorphism\Multiple-dispatch.md`中收录了这篇文章。

### [Incomplete type](https://en.cppreference.com/w/cpp/language/type#Incomplete_type)



## cppreference [Type support library](https://en.cppreference.com/w/cpp/types)

在`C++\Library\Standard-library\Utility-library\Language-support\Type_support`中，对此进行了分析。



## cppreference [typeid](https://en.cppreference.com/w/cpp/language/typeid)

参见本章的`typeid`

## cppreference [decltype](https://en.cppreference.com/w/cpp/language/typeid) 

参见本章的`decltype`

## type cast and conversion

参见本章的`Type-conversion`


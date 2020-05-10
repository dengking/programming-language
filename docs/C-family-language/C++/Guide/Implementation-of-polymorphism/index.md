# 关于本章

本章讨论c++中如何实现polymorphism，这是一个较大的话题。c++中提供了多种polymorphism，下面根据static/dynamic进行分类：

static

- overload
- template

dynamic

- virtual method

当对它们进行实现的时候，无论是哪种实现方式，都需要考虑one-to-many的问题，即需要考虑使用set  of candidates中的哪一个来作为实现，那到底选择哪一个呢？显然这有一个比较的过程，通俗来讲，选择最“合适” 的那一个，那到底哪个最合适呢？不同的polymorphism有不同的标准。

## Implementation of overload 

需要根据function name、type of argument来从set  of candidates中找到对应的实现。

### 实现方式

[Name mangling](https://en.wikipedia.org/wiki/Name_mangling)

匹配过程在下面的文章中有提及：

文章：[SFINAE and enable_if](https://eli.thegreenplace.net/2014/sfinae-and-enable_if/)

这篇文章中给出的例子非常好，其实的“matches perfectly”。

维基百科文章：[Function overloading](https://en.wikipedia.org/wiki/Function_overloading)

这篇文章中，将匹配过程描述为resolution。

与此相关的问题有：

- [Argument-dependent lookup](https://en.cppreference.com/w/cpp/language/adl)

## Implementation of template

最最简单的情况是，仅仅存在primary template，此时compiler仅仅根据primary template进行instantiation of template即可。比较复杂的情况是：当存在多个partial specialization of template的时候，compiler如何选择set  of candidates中的哪个partial specialization of template进行instantiation？

与此相关的有：

- trait
- sfinae
- enable_if

## Implementation of virtual method

需要考虑，使用哪个实现方式，因为可能的实现方式是多个的。



### 实现方式

[Virtual method table](https://en.wikipedia.org/wiki/Virtual_method_table)



most derived class

在https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019中提及了

> For example, if a class has virtual functions, all the instances of that class might share a single virtual function table



## Template Specialization VS Function Overloading

https://stackoverflow.com/questions/7108033/template-specialization-vs-function-overloading

https://www.fluentcpp.com/2017/08/15/function-templates-partial-specialization-cpp/
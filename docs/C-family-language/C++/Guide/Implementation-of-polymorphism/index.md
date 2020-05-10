# 关于本章

本章讨论c++中如何实现polymorphism，这是一个较大的话题。c++中提供了多种polymorphism，下面根据static/dynamic进行分类：

static

- overload
- template

dynamic

- virtual method

当对它们进行实现的时候，无论是哪种实现方式，都需要考虑one-to-many的问题，即需要考虑使用多个candidate中的哪一个来作为实现。

## Implementation of overload 

[Name mangling](https://en.wikipedia.org/wiki/Name_mangling)

需要根据function name、type of argument来从candidate中找到对应的实现。

## Implementation of template

最最简单的情况是，仅仅存在primary template，此时compiler仅仅根据primary template进行instantiation of template即可。比较复杂的情况是：当存在多个partial specialization of template的时候，compiler如何选择哪个partial specialization of template进行instantiation。

## Implementation of virtual method

需要考虑，使用哪个实现方式，因为可能的实现方式是多个的。

[Virtual method table](https://en.wikipedia.org/wiki/Virtual_method_table)

在https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019中提及了

> For example, if a class has virtual functions, all the instances of that class might share a single virtual function table
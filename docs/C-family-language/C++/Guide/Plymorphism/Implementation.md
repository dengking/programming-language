# Implementation of polymorphism

在`Theory\Programming-paradigm\Object-oriented-programming\Polymorphism`中讨论了各种polymorphism，本章讨论c++中如何实现polymorphism，这是一个较大的话题。c++中提供了多种polymorphism，下面根据static/dynamic进行分类：

| static                    | dynamic                     |
| ------------------------- | --------------------------- |
| - overload <br>- template | - sub-type / virtual method |



正如在`Theory\Programming-paradigm\Object-oriented-programming\Polymorphism`中所总结的:

> Polymorphism is a mapping/dispatch

因此，无论是哪种实现方式，都需要考虑one-to-many的问题，即需要考虑使用set  of candidates中的哪一个来作为实现，那到底选择哪一个呢？显然这有一个**比较**/**排序**的过程，通俗来讲，选择最“**合适**” 的那一个，那到底哪个最**合适**呢？不同的polymorphism有不同的标准。

Overload、specialization、subclass都是对某种“特殊情况的说明”，compiler在进行选择/resolve的时候，应该选择**最最特殊情况**: 

| Type of polymorphism  | 最最特殊的                      | 样例                                                         |
| --------------------- | ------------------------------- | ------------------------------------------------------------ |
| sub-type polymorphism | most derived object             | cppreference [Object#Subobjects](https://en.cppreference.com/w/cpp/language/object#Subobjects) |
| template              | most specialized specialization | cppreference [Partial template specialization#Partial ordering](https://en.cppreference.com/w/cpp/language/partial_specialization#Partial_ordering) |
| overload              | most matched                    | cppreference [Overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution) |

> draft: c++ overload 、template specialization、virtual method和Python attribute find的过程类似，都是实现polymorphsim的基础。

## Static polymorphism

### Static polymorphism and template metaprogramming

c++的static polymorphism是由compiler来实现，C++的实现允许programmer通过template metaprogramming来对static polymorphism的过程进行控制，这是C++的核心特性，主要的实现方式：

- trait
- sfinae
- `enable_if`

上诉三者在`C-family-language\C++\Idiom\Template-metaprogramming\SFINAE-trait-enable-if`中总结了。

### Implementation of template

在`C++\Language-reference\Template\Implementation`章节对此进行了深入分析。



### Implementation of overload 

在`C++\Language-reference\Functions\Overload-resolution`章节对此进行深入分析。

### Overload + template

在`C++\Language-reference\Template\Function-template`章节对此进行深入分析。

## Implementation of dynamic polymorphism



在`C++\Language-reference\Classes\Subtype-polymorphism\Implementation`章节对此进行深入分析。



## Draft

### static polymorphism VS dynamic polymorphism 

需要考虑dynamic polymorphism的增加的耗费

https://eli.thegreenplace.net/2013/12/05/the-cost-of-dynamic-virtual-calls-vs-static-crtp-dispatch-in-c/
https://eli.thegreenplace.net/2011/05/17/the-curiously-recurring-template-pattern-in-c/
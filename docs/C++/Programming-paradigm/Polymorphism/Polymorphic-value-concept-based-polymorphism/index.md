# Polymorphic value and concept based polymorphism

一、它是由Sean Parent提出，并大力推广的，参见 `Sean-Parent\Value-semantic-and-Concept-based-Polymorphism` 章节

二、consistent interface

## Sean parent Value-Semantics-and-Concept-based-Polymorphism、Polymorphic-Value-Types-idiom

1、type erasure是实现Concept-based-Polymorphism、Polymorphic-Value-Types-idiom的基础，参见 `Value-Semantics-and-Concept-based-Polymorphism` 章节。





## 标准化

open-std [A polymorphic value-type for `C++`](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0201r5.html)

## Implementation

1、[jbcoe](https://github.com/jbcoe)/**[polymorphic_value](https://github.com/jbcoe/polymorphic_value)**

2、type earsure

参见 `Type-Erasure-and-polymorphism` 章节

3、`Polymorphic-Value-Types-idiom`



4、groups.google [Safe 'type erasure' without runtime memory allocation](https://groups.google.com/a/isocpp.org/g/std-proposals/c/eWl57Y96_iI)

> Generally, the 'type erasure' pattern involves the use of:
>
> \1. boost/std any - which is inefficient because of the need to query the object or it forces use of the visitor pattern which scatters the logic all over the place.
>
> \2. A custom abstract interface - which leads to runtime memory allocation.
>
> 

上述总结是非常好的。


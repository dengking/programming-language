# 关于本章

本章讨论C++的subtyping polymorphism，这是一种runtime polymorphism，是C++的核心特性之一。



## Reference semantic and runtime polymorphism

在C++中，runtime polymorphism与value semantic、reference semantic密切相关。触发我对它们产生思考的是：

1、[如何评价 C++11 的右值引用（Rvalue reference）特性？ - zihuatanejo的回答 - 知乎](https://www.zhihu.com/question/22111546/answer/31929118)中提及的：[值语义和运行时多态是矛盾的。](https://link.zhihu.com/?target=http%3A//akrzemi1.wordpress.com/2012/02/03/value-semantics/%23comment-270)

2、multiple dispatch

C++的value semantic是不支持runtime polymorphsim的，reference semantic是支持runtime polymorphsim的，所以需要通过reference semantic才能够实现runtime polymorphysim的，classic example包括:

1、stackoverflow [What is object slicing?](https://stackoverflow.com/questions/274626/what-is-object-slicing) # [A](https://stackoverflow.com/a/14461532)

这篇文章收录在`Object-slicing`章节中，它非常好的展示了value semantic、reference semantic、runtime polymorphism之间的关联

## Implementation

需要思考：reference semantic是如何实现polymorphism的，这在`C++\Language-reference\Classes\Subtype-polymorphism\Implementation`章节进行了描述。

## Virtual in C++

### Virtual method and virtual data

在C++，dynamic polymorphism是behavior（function），只有virtual method（virtual table），而不是data/value的。

> NOTE: 这是符合Uniform function model的，参见`C++\Language-reference\Basic-concept\index.md`的“Uniform function model”章节，它所对应的是**Member method is a kind of function**。

那C++中能否实现virtual data呢？这个问题在What is “`virtual` data,” and how-can / why-would I use it in C++? [¶](https://isocpp.org/wiki/faq/value-vs-ref-semantics#virt-data) [Δ](https://isocpp.org/wiki/faq/value-vs-ref-semantics#)中讨论了，这个讨论收录在了`C++\Language-reference\Basic-concept\Reference-and-Value-Semantics\Value-and-reference-semantics`中。



### Virtual method



|                             | 章节                                                         | 说明 |
| --------------------------- | ------------------------------------------------------------ | ---- |
| Virtual Constructor         | `C++\Idiom\OOP\Virtual-Constructor`                          |      |
| Virtual destructor          | `C++\Language-reference\Classes\Special-member-functions\Destructor\Virtual-destructor` |      |
| Virtual Friend-Function     | `C++\Idiom\OOP\Virtual-Friend-Function`                      |      |
| Virtual function            | `C++\Language-reference\Classes\Subtype-polymorphism\Inheritance\Virtual-function` |      |
| Virtual assignment operator | `Object-slicing`                                             |      |



### Virtual data

在What is “`virtual` data,” and how-can / why-would I use it in C++? [¶](https://isocpp.org/wiki/faq/value-vs-ref-semantics#virt-data) [Δ](https://isocpp.org/wiki/faq/value-vs-ref-semantics#)中讨论了，这个讨论收录在了`C++\Language-reference\Basic-concept\Reference-and-Value-Semantics\Value-and-reference-semantics`中。

### Virtual return type 

Covariant，参见 `Return-Covariant-Types` 章节。

### Virtual inheritance

这在`C++\Language-reference\Classes\Subtype-polymorphism\Inheritance\Derived-classes\Virtual-inheritance`中进行了讨论。


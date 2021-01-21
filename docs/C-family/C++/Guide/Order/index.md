# Order

C++中的很多问题，如果我们使用"order"即"次序"来进行分析，能够以清晰的、鸟瞰的视角对问题形成全面的理解。本文对C++ language中，涉及order的内容进行梳理。

## Ordered and unordered

本节的标题含义: 有序与无序; 

对于标准中定义的有序的，我们可以依赖的；对于标准中定义的无序的，我们是不可依赖的，否则可能导致；



## Ordered 



### Object lifetime

object是C family language的核心概念，C family language对object lifetime进行了非常详细的定义，参见`C++\Language-reference\Basic-concept\Data-model\Object\Object-lifetime-and-storage-duration`。



### Order of evaluation

C++语言对expression的evaluation的规定，参见`C-family-language\C-and-C++\Order-of-evaluation`。

## Unordered

无序: cppreference [Initialization#Dynamic initialization](https://en.cppreference.com/w/cpp/language/initialization#Dynamic_initialization)

### Initialization Order Fiasco

这种错误是非常严重的一种错误。

## Typical order

下面描述在C++语言中，常用的一些order。

### Stack order

在C++中，很多order相关的内容，都可以使用stack模型来进行描述。

基于stack模型的initialization and deinitialization:

1) OOP object

2) Temporary: https://en.cppreference.com/w/cpp/language/lifetime#Temporary_object_lifetime

> All temporary objects are destroyed as the last step in evaluating the full-expression that (lexically) contains the point where they were created, and if multiple temporary objects were created, they are destroyed in the order opposite to the order of creation. This is true even if that evaluation ends in throwing an exception.




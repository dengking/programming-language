# Order

C++中的很多问题，如果我们使用"order"即"次序"来进行分析，能够以清晰的、鸟瞰的视角对问题形成全面的理解。本文对C++ language中，涉及order的内容进行梳理。

## Object lifetime

object是C family language的核心概念，C family language对object lifetime进行了非常详细的定义，参见`C++\Language-reference\Basic-concept\Data-model\Object\Object-lifetime-and-storage-duration`。





## Order of evaluation

C++语言对expression的evaluation的规定，参见`C-family-language\C-and-C++\Order-of-evaluation`。



## Initialization Order Fiasco

这种错误是非常严重的一种错误。



## Stack and order

在C++中，很多order相关的内容，都可以使用stack模型来进行描述。

基于stack模型的initialization and deinitialization:

1) OOP object



2) temporary: https://en.cppreference.com/w/cpp/language/lifetime#Temporary_object_lifetime

> All temporary objects are destroyed as the last step in evaluating the full-expression that (lexically) contains the point where they were created, and if multiple temporary objects were created, they are destroyed in the order opposite to the order of creation. This is true even if that evaluation ends in throwing an exception.



C++的object是复杂的，它有subobject。

它们都是lifetime的内容。应该将它们放到lifetime章节。

全部都是事关lifetime的。
# Implementation of polymorphic type

在cppreference [Object](https://en.cppreference.com/w/cpp/language/object)`#`[Polymorphic objects](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects)中给出了这个问题的一个大致解答：

> Within each polymorphic object, the implementation stores additional information (in every existing implementation, it is one pointer unless optimized out), which is used by [virtual function](https://en.cppreference.com/w/cpp/language/virtual) calls and by the RTTI features ([dynamic_cast](https://en.cppreference.com/w/cpp/language/dynamic_cast) and [typeid](https://en.cppreference.com/w/cpp/language/typeid)) to determine, at run time, the type with which the object was created, regardless of the expression it is used in.

对于polymorphic object的，在使用它的时候，一个非常重要的topic是: 得到dynamic type entire objec，因为它们一般都是通过static type subobject来进行使用的。


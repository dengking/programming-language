# boost [Boost.TypeErasure](https://www.boost.org/doc/libs/1_71_0/doc/html/boost_typeerasure.html)

## [Introduction](https://www.boost.org/doc/libs/1_71_0/doc/html/boost_typeerasure.html#boost_typeerasure.introduction)

> NOTE: 原文这一段关于C++ static polymorphism 和 dynamic polymorphism的对比是非常好的

The Boost.TypeErasure library combines the superior abstraction capabilities of templates, with the runtime flexibility of virtual functions.

> NOTE: 
>
> 1、它是如何做到的？
>
> 2、上面这段话，让我想到了concept-based polymorphism，看了它后面的一些介绍，初步判断它也是使用的concept-based polymorphism
# `std::function` 类型的功能的实现

在下面的文章中，提供了类似的实现:

1、sean-parent [Polymorphic Task Template in Ten](https://sean-parent.stlab.cc/presentations/2017-11-11-lightning-talk/lightning-talk.pdf)

2、groups.google [Safe 'type erasure' without runtime memory allocation](https://groups.google.com/a/isocpp.org/g/std-proposals/c/eWl57Y96_iI)

3、sodocumentation [C++Type Erasure](https://sodocumentation.net/cplusplus/topic/2872/type-erasure)

4、open-std [P1144R2 Object relocation in terms of move plus destroy](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html) 

## Template function signature

sodocumentation [C++Type Erasure](https://sodocumentation.net/cplusplus/topic/2872/type-erasure)

```C++
template<class Sig>
struct task;
template<class R, class...Args>
struct task<R(Args...)> {
};
```



## `std::function` is based on type erasure

`std::function` 是基于 type erasure 实现的，这在 artima [On the Tension Between Object-Oriented and Generic Programming in C++](https://www.artima.com/cppsource/type_erasure.html) 中进行了说明:

> The most widely known and used examples of type erasure are `boost::any`[[5](https://www.artima.com/cppsource/type_erasure2.html#notes)] and `boost::function`[[6](https://www.artima.com/cppsource/type_erasure2.html#notes)]. I'll discuss `boost::any` in detail in the second part of this article. `boost::function` is a class template that takes one template argument, **a function type**. Choosing a function type amounts to(等价于) choosing a return type and a list of argument types for a function. Suppose we instantiate `boost::function` as follows:
>
> ```
> boost::function<int (int)> foo;
> ```
>
> The variable `foo` can now hold anything that's callable with an `int` as its only argument, and whose return type is convertible to `int`. This could be a **function pointer**, a **user-defined functor**, the result of a `boost::bind`, or what have you. Clearly, this matches the above definition of type erasure.





## TODO wizmann [C++类型擦除与`std::function`性能探索](https://wizmann.tk/cpp-type-erasure-and-std-function.html)


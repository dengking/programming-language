# [Type modifications](https://en.cppreference.com/w/cpp/types#Type_modifications)

Type modification templates create new type definitions by applying modifications on a template parameter. The resulting type can then be accessed through `type` member typedef.

> NOTE: 各种type modification的实现也是使用的trait技术；
>
> 在文章 http://bajamircea.github.io/coding/cpp/2016/04/07/move-forward.html 中提及了`std::remove_reference` ，意思是通过`std::remove_reference` ，我们可以获得template argument的underlying type；我们需要举一反三，[Type modifications](https://en.cppreference.com/w/cpp/types#Type_modifications)系列函数的作用和`std::remove_reference`类似：获得template argument的underlying type



## References

### cppreference [std::remove_reference](https://en.cppreference.com/w/cpp/types/remove_reference)

> NOTE: 使用[std::remove_reference](https://en.cppreference.com/w/cpp/types/remove_reference)的典型例子就是`std::move`，在`C++\Language-reference\Reference\Move-semantic\std-move.md`中对它进行了详细分析。

## Const-volatility specifiers

### cppreference [std::remove_cv, std::remove_const, std::remove_volatile](https://en.cppreference.com/w/cpp/types/remove_cv)


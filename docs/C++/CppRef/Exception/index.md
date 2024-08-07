# Exception

正如在文章boost [**Lessons Learned from Specifying Exception-Safety for the C++ Standard Library**](https://www.boost.org/community/exception_safety.html) 中所述：

> “exception”已经“built-in to the core C++ language”。

在阅读cppreference中关于standard library的描述中，我们可以看到其中往往都有Exceptions说明，比如[`std::swap`](https://en.cppreference.com/w/cpp/algorithm/swap)。

在阅读[*More C++ Idioms*](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms)的[Copy-and-swap](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Copy-and-swap)章节时，其中对Exception safety进行了详细的介绍。

所以对于c++ programming language，了解exception非常有必要。

关于exception，有多个方面的内容需要了解。



## cppreference [Exceptions](https://en.cppreference.com/w/cpp/language/exceptions)

When declaring a function, [exception specifications](https://en.cppreference.com/w/cpp/language/except_spec) and [noexcept specifiers](https://en.cppreference.com/w/cpp/language/noexcept_spec) may be provided to limit the types of the exceptions a function may throw.

> NOTE:  
>
> 一、[exception specifications](https://en.cppreference.com/w/cpp/language/except_spec) and [noexcept specifiers](https://en.cppreference.com/w/cpp/language/noexcept_spec) 用来说明一个function可能抛出的exception，其实  [noexcept specifiers](https://en.cppreference.com/w/cpp/language/noexcept_spec) 也可以归入到exception specification到范畴

Errors that arise during exception handling are handled by [std::terminate](https://en.cppreference.com/w/cpp/error/terminate) and [std::unexpected](https://en.cppreference.com/w/cpp/error/unexpected) (until C++17).



## throw exception

cppreference [`throw` expression](https://en.cppreference.com/w/cpp/language/throw)



## Function-try-block

cppreference [Function-try-block](https://en.cppreference.com/w/cpp/language/function-try-block)



## try-block

cppreference [try-block](https://en.cppreference.com/w/cpp/language/try_catch)



## Exception specification

参见 `Exception-specification` 章节。






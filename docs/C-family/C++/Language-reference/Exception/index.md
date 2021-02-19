# Exception

正如在文章boost [**Lessons Learned from Specifying Exception-Safety for the C++ Standard Library**](https://www.boost.org/community/exception_safety.html) 中所述：

> “exception”已经“built-in to the core C++ language”。

在阅读cppreference中关于standard library的描述中，我们可以看到其中往往都有Exceptions说明，比如[`std::swap`](https://en.cppreference.com/w/cpp/algorithm/swap)。

在阅读[*More C++ Idioms*](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms)的[Copy-and-swap](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Copy-and-swap)章节时，其中对Exception safety进行了详细的介绍。

所以对于c++ programming language，了解exception非常有必要。

关于exception，有多个方面的内容需要了解。

## 内容概述

Exception safety: C++是支持exception的，实现strong exception safety具有非常重要的意义，因此如何实现strong exception safety是C++中非常重要的一个topic。

## cppreference [Exceptions](https://en.cppreference.com/w/cpp/language/exceptions)

When declaring a function, [exception specifications](https://en.cppreference.com/w/cpp/language/except_spec) and [noexcept specifiers](https://en.cppreference.com/w/cpp/language/noexcept_spec) may be provided to limit the types of the exceptions a function may throw.

> NOTE:  [exception specifications](https://en.cppreference.com/w/cpp/language/except_spec) and [noexcept specifiers](https://en.cppreference.com/w/cpp/language/noexcept_spec) 用来说明一个function可能抛出的exception。

Errors that arise during exception handling are handled by [std::terminate](https://en.cppreference.com/w/cpp/error/terminate) and [std::unexpected](https://en.cppreference.com/w/cpp/error/unexpected) (until C++17).



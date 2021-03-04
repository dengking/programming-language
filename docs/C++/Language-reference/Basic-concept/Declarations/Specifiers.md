# Specifiers

本文参考cppreference [Declarations](https://en.cppreference.com/w/cpp/language/declarations)的Specifiers章节。

## [`typedef`](https://en.cppreference.com/w/cpp/language/typedef) specifier



## Function specifiers

function specifiers ([`inline`](https://en.cppreference.com/w/cpp/language/inline), [`virtual`](https://en.cppreference.com/w/cpp/language/virtual), [`explicit`](https://en.cppreference.com/w/cpp/language/explicit)), only allowed in [function declarations](https://en.cppreference.com/w/cpp/language/function)



## [`friend`](https://en.cppreference.com/w/cpp/language/friend) specifier

the [`friend`](https://en.cppreference.com/w/cpp/language/friend) specifier, allowed in class and function declarations.



## [`constexpr`](https://en.cppreference.com/w/cpp/language/constexpr) [`consteval`](https://en.cppreference.com/w/cpp/language/consteval) [`constinit`](https://en.cppreference.com/w/cpp/language/constinit) specifier

上述三个specifier都是以`const`打头。

## [storage class specifier](https://en.cppreference.com/w/cpp/language/storage_duration)

[storage class specifier](https://en.cppreference.com/w/cpp/language/storage_duration) ([`register`](https://en.cppreference.com/w/cpp/keyword/register), [`static`](https://en.cppreference.com/w/cpp/keyword/static), [`thread_local`](https://en.cppreference.com/w/cpp/keyword/thread_local) (since C++11), [`extern`](https://en.cppreference.com/w/cpp/keyword/extern), [`mutable`](https://en.cppreference.com/w/cpp/keyword/mutable)). Only one storage class specifier is allowed, except that `thread_local` may appear together with `extern` or `static`.



## Type specifiers (`type-specifier-seq`)

此处略去。



## [Access specifier](https://en.cppreference.com/w/cpp/language/access)
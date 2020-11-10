# Dynamic exception specification

这种grammar是有一定价值的，在`Theory\Error-handling`章节中进行了介绍，但是这种用法: deprecated in C++11、removed in C++17。

## cppreference [Dynamic exception specification](https://en.cppreference.com/w/cpp/language/except_spec)	



## Why dynamic exception specification is removed ?

### stackoverflow [Throw keyword in function's signature](https://stackoverflow.com/questions/1055387/throw-keyword-in-functions-signature)



[A](https://stackoverflow.com/a/1055407)

No, it is not considered good practice. On the contrary, it is generally considered a bad idea.

http://www.gotw.ca/publications/mill22.htm goes into a lot more detail about why, but the problem is partly that the compiler is unable to enforce this, so it has to be checked at runtime, which is usually undesirable. And it is not well supported in any case. (MSVC ignores exception specifications, except throw(), which it interprets as a guarantee that no exception will be thrown.



### devblogs.microsoft [The sad history of the C++ `throw(…)` exception specifier](https://devblogs.microsoft.com/oldnewthing/20180928-00/?p=99855)

This has made a lot of people very angry and [has been widely regarded as a bad move](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0003r0.html#2.0).



### gotw [A Pragmatic Look at Exception Specifications](http://www.gotw.ca/publications/mill22.htm)



### open-std [Removing Deprecated Exception Specifications from C++17](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0003r0.html#4.0)
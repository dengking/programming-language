[TOC]



# [std::result_of, std::invoke_result](https://en.cppreference.com/w/cpp/types/result_of)

```c++
template< class >
class result_of; // not defined

template< class F, class... ArgTypes >
class result_of<F(ArgTypes...)>;
```



Deduces the return type of an INVOKE expression at compile time.


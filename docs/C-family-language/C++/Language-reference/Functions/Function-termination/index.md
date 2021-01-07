# Function termination



## Function termination方式

在 cppreference [Functions](https://en.cppreference.com/w/cpp/language/functions) 中，说明了function termination: 

> A function can terminate by [returning](https://en.cppreference.com/w/cpp/language/return) or by [throwing](https://en.cppreference.com/w/cpp/language/throw) an [exception](https://en.cppreference.com/w/cpp/language/exceptions).

关于exception，参见 `C++\Language-reference\Exception` 章节。

由于function也是一个scope，因此function termination，其实是scope exit，因此当function termination的时候，在 `Exception\Stack-unwinding` 章节中，我们知道，scope exit的时候，一个非常重要的过程是: stack unwinding，因此当function termination的时候，一个非常重要的过程就是stack unwinding。

## Function return value



### Function return multiple value

发展概述:

C++11:

1) return multiple value: `std::tuple`、`std::tie`

C++17: 

1) return multiple value: structured binding



关于function return multiple value的发展历程，在codeproject [C++ 17 New Features and Trick](https://www.codeproject.com/Articles/5262072/Cplusplus-17-New-Features-and-Trick) # [Structure Binding](https://www.codeproject.com/Articles/5262072/Cplusplus-17-New-Features-and-Trick#Structure%20binding) 中进行了非常好的总结，在`C++\What-is-new-in-C++\C++17\Structured-binding`中收录了这篇文章。
# Function termination



## Function termination方式

在 cppreference [Functions](https://en.cppreference.com/w/cpp/language/functions) 中，说明了function termination: 

> A function can terminate by [returning](https://en.cppreference.com/w/cpp/language/return) or by [throwing](https://en.cppreference.com/w/cpp/language/throw) an [exception](https://en.cppreference.com/w/cpp/language/exceptions).

关于exception，参见 `C++\Language-reference\Exception` 章节。

按照在 `Execution-model` 章节讨论的内容: 

当function termination的时候，一个非常重要的过程是: stack unwinding；

关于stack unwinding，参见:

1、 `C++\Language-reference\Exception\Stack-unwinding` 

2、`C-and-C++\From-source-code-to-exec\ABI\Call-convention\Call-stack`

## Function return value



### Function return multiple value

发展概述:

C++11:

1) return multiple value: `std::tuple`、`std::tie`

C++17: 

1) return multiple value: structured binding



关于function return multiple value的发展历程，在codeproject [C++ 17 New Features and Trick](https://www.codeproject.com/Articles/5262072/Cplusplus-17-New-Features-and-Trick) # [Structure Binding](https://www.codeproject.com/Articles/5262072/Cplusplus-17-New-Features-and-Trick#Structure%20binding) 中进行了非常好的总结，在`C++\What-is-new-in-C++\C++17\Structured-binding`中收录了这篇文章。
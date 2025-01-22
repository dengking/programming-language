# 关于本章

本章描述C++的 [Type support (basic types, RTTI, type traits)](https://en.cppreference.com/w/cpp/types)，查看原文可知，C++ type support library提供了丰富的API，我们首先需要建立高屋建瓴的视角来全面了解这些API，然后逐个深入。

## cppreference [Type support (basic types, RTTI, type traits)](https://en.cppreference.com/w/cpp/types) 

> NOTE: 概述:
>
> | 章节                        | 说明    |
> | --------------------------- | ------- |
> | Basic types                 |         |
> | Runtime type identification | dynamic |
> | Type traits                 | static  |
>
> 



### Basic types

#### [Fundamental types defined by the language](https://en.cppreference.com/w/cpp/language/types)

> NOTE: C++语言内置的type，可以直接使用，不需要使用`std::`。

#### Additional basic types and macros

> NOTE: C++语言定义的类型，这些类型非常重要，往往是以standard library的方式提供，需要使用`std::`



#### [Fixed width integer types](https://en.cppreference.com/w/cpp/types/integer) (since C++11)

> NOTE: 这是C++11新增的，对fixed width integer type进行了统一。

### Numeric limits

|                                                              | 简介                |
| ------------------------------------------------------------ | ------------------- |
| [numeric_limits](https://en.cppreference.com/w/cpp/types/numeric_limits) | C++ trait 风格的API |
| [C numeric limits interface](https://en.cppreference.com/w/cpp/types/climits) | 传统C风格的API      |



### Runtime type identification

> NOTE: RTTI特性

### Type traits (since C++11)

> NOTE: 
>
> 参见 `C++11-Type-traits` 章节



#### Helper classes

### Constant evaluation context (since C++20)

Detects whether the call occurs within a constant-evaluated context;
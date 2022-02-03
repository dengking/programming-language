# C++ Core Guidelines # [GSL: Guidelines support library](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#S-gsl)

> 在阅读 [hsutter](https://github.com/hsutter)/**[gcpp](https://github.com/hsutter/gcpp)** 的source code的时候，发现了其中使用了gsl。
>
> 



Summary of GSL components:

1、[GSL.view: Views](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#SS-views)

2、[GSL.owner](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#SS-ownership)

3、[GSL.assert: Assertions](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#SS-assertions)

4、[GSL.util: Utilities](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#SS-utilities)

5、[GSL.concept: Concepts](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#SS-gsl-concepts)

## GSL.view: Views

These types allow the user to distinguish between **owning and non-owning pointers** and between **pointers to a single object and pointers to the first element of a sequence**.

> NOTE: 
>
> 一、"**owning and non-owning pointers**"告诉我们需要阐明清楚ownership，它的实现是通过`gsl::owner`来实现的。
>
> 二、"**pointers to a single object and pointers to the first element of a sequence**"告诉我们需要区分清楚range，它的实现主要是通过`gsl::span`
>
> 在 [microsoft](https://github.com/microsoft)/**[GSL](https://github.com/microsoft/GSL)** 中，"GSL.view: Views"包含的内容进行了梳理: 
>
> | [**1. Views**](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#gslview-views) |      |                                                              |
> | ------------------------------------------------------------ | ---- | ------------------------------------------------------------ |
> | owner                                                        | ☑    | an alias for a raw pointer                                   |
> | not_null                                                     | ☑    | restricts a pointer / smart pointer to hold non-null values  |
> | span                                                         | ☑    | a view over a contiguous sequence of memory. Based on the standardized verison of `std::span`, however `gsl::span` enforces **bounds checking**. See the [wiki](https://github.com/microsoft/GSL/wiki/gsl::span-and-std::span) for additional information. |
> | span_p                                                       | ☐    | spans a range starting from a pointer to the first place for which the predicate is true |
> | basic_zstring                                                | ☑    | A pointer to a C-string (zero-terminated array) with a templated char type |
> | zstring                                                      | ☑    | An alias to `basic_zstring` with a char type of `char`       |
> | czstring                                                     | ☑    | An alias to `basic_zstring` with a char type of `const char` |
> | wzstring                                                     | ☑    | An alias to `basic_zstring` with a char type of `wchar_t`    |
> | cwzstring                                                    | ☑    | An alias to `basic_zstring` with a char type of `const wchar_t` |
> | u16zstring                                                   | ☑    | An alias to `basic_zstring` with a char type of `char16_t`   |
> | cu16zstring                                                  | ☑    | An alias to `basic_zstring` with a char type of `const char16_t` |
> | u32zstring                                                   | ☑    | An alias to `basic_zstring` with a char type of `char32_t`   |
> | cu32zstring                                                  | ☑    | An alias to `basic_zstring` with a char type of `const char32_t` |





## TODO

stackoverflow [What “are” the C++ GSL guidelines?](https://stackoverflow.com/questions/36980975/what-are-the-c-gsl-guidelines)



modernescpp [C++ Core Guideline: The Guideline Support Library](https://www.modernescpp.com/index.php/c-core-guideline-the-guidelines-support-library)



## gsl library implementation

### [gsl-lite](https://github.com/gsl-lite/gsl-lite)

特点:

1、C++98、C++11





### stackoverflow [How can I use GSL with C++11 and the new STL?](https://stackoverflow.com/questions/15678275/how-can-i-use-gsl-with-c11-and-the-new-stl)
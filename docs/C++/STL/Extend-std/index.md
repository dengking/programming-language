# Extending std



## cppreference [Extending the namespace `std`](https://en.cppreference.com/w/cpp/language/extending_std)

> NOTE: 原文的描述采用的枚举法；

### Adding declarations to `std`

It is **undefined behavior** to add declarations or definitions to `namespace std` or to any namespace nested within `std`, with a few exceptions noted below

> NOTE: 这说明我们不能直接向`std`中添加declaration。

### Adding template specializations

> NOTE: 关于template specializaiton，参见`C++\Language-reference\Template\Specialization`。

#### Class templates

It is **allowed** to add **template specializations** for any standard library class template to the namespace `std` only if the declaration depends on at least one program-defined type and the specialization satisfies all requirements for the original template, except where such specializations are prohibited.

#### Function templates and member functions of templates

|                                                              | version       |
| ------------------------------------------------------------ | ------------- |
| It is allowed to add template specializations for any standard library function template to the namespace `std` only if the declaration depends on at least one program-defined type and the specialization satisfies all requirements for the original template, except where such specializations are prohibited. | (until C++20) |
| It is undefined behavior to declare a full specialization of any standard library function template. | (since C++20) |

> NOTE:
>
> 一、注意: 从C++20开始，是不在允许通过full specialization function template的方式来extend std了

#### Variable templates

> NOTE: C++14引入的

## 补充

### [More C++ Idioms/Non-throwing_swap](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-throwing_swap)

> Using non-throwing swap idiom for template classes (e.g., `Matrix<T>`) can be a subtle issue. As per the C++98 standard, only the **full specialization** of `std::swap` is allowed to be defined inside `std` namespace for the **user-defined types**. **Partial specializations** or **function overloading** is not allowed by the language. Trying to achieve the similar effect for template classes (e.g., `Matrix<T>`) results into overloading of `std::swap` in `std` namepspace, which is technically **undefined behavior**. 
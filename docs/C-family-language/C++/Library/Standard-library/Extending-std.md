# Extending std



## cppreference [Extending the namespace `std`](https://en.cppreference.com/w/cpp/language/extending_std)

> NOTE: 原文的描述采用的枚举法；

### Adding declarations to `std`

It is **undefined behavior** to add declarations or definitions to `namespace std` or to any namespace nested within `std`, with a few exceptions noted below

> NOTE: 这说明我们不能直接向`std`中添加declaration。

### Adding template specializations

#### Class templates



#### Function templates and member functions of templates



#### Variable templates



## 补充

### [More C++ Idioms/Non-throwing_swap](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-throwing_swap)

> Using non-throwing swap idiom for template classes (e.g., `Matrix<T>`) can be a subtle issue. As per the C++98 standard, only the **full specialization** of `std::swap` is allowed to be defined inside `std` namespace for the **user-defined types**. **Partial specializations** or **function overloading** is not allowed by the language. Trying to achieve the similar effect for template classes (e.g., `Matrix<T>`) results into overloading of `std::swap` in `std` namepspace, which is technically **undefined behavior**. 
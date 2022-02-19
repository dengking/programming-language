
# Operator function object

一、根据uniform function model，operator(比如加减乘除)可以看做是function，所以存在operator function object其实是非常自然而然的事情。

二、policy-based-design

STL通过operator function object，遵循了policy-based-design，programmer通过将这些operator function object作为实参(可能是)实现了灵活的代码复用，典型例子:

1、STL algorithm library中的很多algorithm都支持传入operator function object

2、STL container library中的很多algorithm的入参都是一个function object

三、STL推崇使用operator function object而不是function pointer，这样做是为了实现更好static type safety

## cppreference [Function objects#Operator function object](https://en.cppreference.com/w/cpp/utility/functional#Operator_function_objects)



### Arithmetic operations

`std::plus`

> NOTE:
>
> 在[`std::transform`](https://en.cppreference.com/w/cpp/algorithm/transform) 的 example中，使用了`std::plus`



### Comparisons

|                                                              |                                                      |
| ------------------------------------------------------------ | ---------------------------------------------------- |
| [equal_to](https://en.cppreference.com/w/cpp/utility/functional/equal_to) | function object implementing x == y (class template) |
| [not_equal_to](https://en.cppreference.com/w/cpp/utility/functional/not_equal_to) | function object implementing x != y (class template) |
| [greater](https://en.cppreference.com/w/cpp/utility/functional/greater) | function object implementing x > y (class template)  |
| [less](https://en.cppreference.com/w/cpp/utility/functional/less) | function object implementing x < y (class template)  |
| [greater_equal](https://en.cppreference.com/w/cpp/utility/functional/greater_equal) | function object implementing x >= y (class template) |
| [less_equal](https://en.cppreference.com/w/cpp/utility/functional/less_equal) | function object implementing x <= y (class template) |
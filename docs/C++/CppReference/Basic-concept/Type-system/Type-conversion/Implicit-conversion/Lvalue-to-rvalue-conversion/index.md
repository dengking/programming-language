# Lvalue to rvalue conversion



## stackoverflow [lvalue to rvalue implicit conversion](https://stackoverflow.com/questions/20850536/lvalue-to-rvalue-implicit-conversion)

> NOTE: 没有读懂



## josephmansfield [C++ sequenced-before graphs](https://josephmansfield.uk/articles/c++-sequenced-before-graphs.html)

For those who want to read more, look up [value categories](http://en.cppreference.com/w/cpp/language/value_category). The left operand of `=` is an lvalue, which means that we don't care about its value. **Lvalue-to-rvalue conversion** can be thought of as reading the value from an object.



## cppreference [Implicit conversions # Value transformations # Lvalue to rvalue conversion](https://en.cppreference.com/w/cpp/language/implicit_conversion#Lvalue_to_rvalue_conversion)

A [glvalue](https://en.cppreference.com/w/cpp/language/value_category#glvalue) of any non-function, non-array type `T` can be implicitly converted to a [prvalue](https://en.cppreference.com/w/cpp/language/value_category#prvalue) of the same type. If `T` is a non-class type, this conversion also removes cv-qualifiers.

> NOTE: 思考:
>
> 1、这种conversion在什么情况下会发生? 有没有example
>
> 

The object denoted by the glvalue is not accessed if:

> NOTE: "is not accessed"是什么含义？

1、the conversion occurs in an [unevaluated context](https://en.cppreference.com/w/cpp/language/expressions#Unevaluated_expressions) (outside an [immediate invocation](https://en.cppreference.com/w/cpp/language/consteval)) (since C++20), such as an operand of `sizeof`, `noexcept`, `decltype`, (since C++11) or the static form of `typeid`

2、the glvalue has the type [std::nullptr_t](https://en.cppreference.com/w/cpp/types/nullptr_t): in this case the resulting prvalue is the null pointer constant nullptr.  (since C++11)

3、the value stored in the object is a compile-time constant and certain other conditions are satisfied (see [ODR-use](https://en.cppreference.com/w/cpp/language/definition#ODR-use))



This conversion models the act of reading a value from a **memory location** into a **CPU register**.

> NOTE: 这段是理解的关键，它提醒了我:
>
> 1、value semantic


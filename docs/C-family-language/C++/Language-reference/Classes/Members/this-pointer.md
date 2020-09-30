# `this` pointer



## cppreference [`this` pointer](https://en.cppreference.com/w/cpp/language/this)



## Value category of `this` pointer

在cppreference [`this` pointer](https://en.cppreference.com/w/cpp/language/this) 中:

> The keyword `this` is a [prvalue](https://en.cppreference.com/w/cpp/language/value_category) [expression](https://en.cppreference.com/w/cpp/language/expressions) whose value is the address of the [implicit object parameter](https://en.cppreference.com/w/cpp/language/overload_resolution) (object on which the non-static member function is being called).

在

在cppreference [Non-static member functions](https://en.cppreference.com/w/cpp/language/member_functions) 中:

> unlike cv-qualification, ref-qualification does not change the properties of the [this](https://en.cppreference.com/w/cpp/language/this) pointer: within a rvalue ref-qualified function, `*this` remains an **lvalue expression**.


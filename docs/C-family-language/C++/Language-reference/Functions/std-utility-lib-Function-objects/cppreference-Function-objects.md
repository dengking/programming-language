[TOC]

# [Function objects](https://en.cppreference.com/w/cpp/utility/functional)

A *function object* is any object for which the **function call operator** is defined. `C++` provides many built-in function objects as well as support for **creation** and **manipulation** of new function objects.

***SUMMARY*** : 上面这段话中的object，并不是指object-oriented programming中的object，而是指的[Object](https://en.cppreference.com/w/cpp/language/object)。

### Function wrappers

[std::function](https://en.cppreference.com/w/cpp/utility/functional/function) provides support for storing arbitrary function objects.

|                                                              |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [function](https://en.cppreference.com/w/cpp/utility/functional/function)(C++11) | wraps callable object of any type with specified function call signature  (class template) |
| [bad_function_call](https://en.cppreference.com/w/cpp/utility/functional/bad_function_call)(C++11) | the exception thrown when invoking an empty [std::function](https://en.cppreference.com/w/cpp/utility/functional/function)  (class) |
| [mem_fn](https://en.cppreference.com/w/cpp/utility/functional/mem_fn)(C++11) | creates a function object out of a pointer to a member  (function template) |

***THINKING*** : 能存储function rvalue吗？



### Function invocation

[std::invoke](https://en.cppreference.com/w/cpp/utility/functional/invoke) can invoke any [*Callable*](https://en.cppreference.com/w/cpp/named_req/Callable) object with given arguments.


|                                                              |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [invoke](https://en.cppreference.com/w/cpp/utility/functional/invoke)(C++17) | invokes any [*Callable*](https://en.cppreference.com/w/cpp/named_req/Callable) object with given arguments  (function template) |

***THINKING*** : [Function object](https://en.cppreference.com/w/cpp/named_req/FunctionObject)和[Callable object](https://en.cppreference.com/w/cpp/named_req/Callable)之间有什么异同？？

### Identity



### Partial function application

std::bind_front and [std::bind](https://en.cppreference.com/w/cpp/utility/functional/bind) provide support for [partial function application](https://en.wikipedia.org/wiki/Partial_application), i.e. binding arguments to functions to produce new functions.

***NOTE*** : Wikipedia的partial function application已经收录到youdao notebook了。

|                                                              |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [bind_front](https://en.cppreference.com/w/cpp/utility/functional/bind_front)(C++20) | binds a variable number of arguments, in order, to a function object  (function template) |
| [bind](https://en.cppreference.com/w/cpp/utility/functional/bind)(C++11) | binds one or more arguments to a function object  (function template) |
| [is_bind_expression](https://en.cppreference.com/w/cpp/utility/functional/is_bind_expression)(C++11) | indicates that an object is `std::bind` expression or can be used as one  (class template) |
| [is_placeholder](https://en.cppreference.com/w/cpp/utility/functional/is_placeholder)(C++11) | indicates that an object is a standard placeholder or can be used as one  (class template) |
| Defined in namespace `std::placeholders`                     |                                                              |
| [_1, _2, _3, _4, ...](https://en.cppreference.com/w/cpp/utility/functional/placeholders)(C++11) | placeholders for the unbound arguments in a `std::bind` expression  (constant) |

### Negators

[std::not_fn](https://en.cppreference.com/w/cpp/utility/functional/not_fn) creates a function object that negates the result of the callable object passed to it.

| [not_fn](https://en.cppreference.com/w/cpp/utility/functional/not_fn)(C++17) | Creates a function object that returns the complement of the result of the function object it holds  (function template) |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
|                                                              |                                                              |

### Reference wrappers

Reference wrappers allow **reference** arguments to be stored in copyable function objects:

| [reference_wrapper](https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper)(C++11) | [*CopyConstructible*](https://en.cppreference.com/w/cpp/named_req/CopyConstructible) and [*CopyAssignable*](https://en.cppreference.com/w/cpp/named_req/CopyAssignable) reference wrapper  (class template) |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [refcref](https://en.cppreference.com/w/cpp/utility/functional/ref)(C++11)(C++11) | creates a [std::reference_wrapper](https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper) with a type deduced from its argument  (function template) |
| [unwrap_referenceunwrap_ref_decay](https://en.cppreference.com/w/cpp/utility/functional/unwrap_reference)(C++20)(C++20) | get the reference type wrapped in [std::reference_wrapper](https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper)  (class template) |

### Operator function objects

C++ defines several function objects that represent common arithmetic and logical operations:

| Arithmetic operations                                        |                                                      |
| ------------------------------------------------------------ | ---------------------------------------------------- |
| [plus](https://en.cppreference.com/w/cpp/utility/functional/plus) | function object implementing x + y  (class template) |
| [minus](https://en.cppreference.com/w/cpp/utility/functional/minus) | function object implementing x - y  (class template) |
| [multiplies](https://en.cppreference.com/w/cpp/utility/functional/multiplies) | function object implementing x * y  (class template) |
| [divides](https://en.cppreference.com/w/cpp/utility/functional/divides) | function object implementing x / y  (class template) |
| [modulus](https://en.cppreference.com/w/cpp/utility/functional/modulus) | function object implementing x % y  (class template) |
| [negate](https://en.cppreference.com/w/cpp/utility/functional/negate) | function object implementing -x  (class template)    |



| Comparisons                                                  |                                                       |
| ------------------------------------------------------------ | ----------------------------------------------------- |
| [equal_to](https://en.cppreference.com/w/cpp/utility/functional/equal_to) | function object implementing x == y  (class template) |
| [not_equal_to](https://en.cppreference.com/w/cpp/utility/functional/not_equal_to) | function object implementing x != y  (class template) |
| [greater](https://en.cppreference.com/w/cpp/utility/functional/greater) | function object implementing x > y  (class template)  |
| [less](https://en.cppreference.com/w/cpp/utility/functional/less) | function object implementing x < y  (class template)  |
| [greater_equal](https://en.cppreference.com/w/cpp/utility/functional/greater_equal) | function object implementing x >= y  (class template) |
| [less_equal](https://en.cppreference.com/w/cpp/utility/functional/less_equal) | function object implementing x <= y  (class template) |



| Logical operations                                           |                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------- |
| [logical_and](https://en.cppreference.com/w/cpp/utility/functional/logical_and) | function object implementing x && y  (class template)   |
| [logical_or](https://en.cppreference.com/w/cpp/utility/functional/logical_or) | function object implementing x \|\| y  (class template) |
| [logical_not](https://en.cppreference.com/w/cpp/utility/functional/logical_not) | function object implementing !x  (class template)       |



| Bitwise operations                                           |                                                       |
| ------------------------------------------------------------ | ----------------------------------------------------- |
| [bit_and](https://en.cppreference.com/w/cpp/utility/functional/bit_and) | function object implementing x & y  (class template)  |
| [bit_or](https://en.cppreference.com/w/cpp/utility/functional/bit_or) | function object implementing x \| y  (class template) |
| [bit_xor](https://en.cppreference.com/w/cpp/utility/functional/bit_xor) | function object implementing x ^ y  (class template)  |
| [bit_not](https://en.cppreference.com/w/cpp/utility/functional/bit_not)(C++14) | function object implementing ~x  (class template)     |


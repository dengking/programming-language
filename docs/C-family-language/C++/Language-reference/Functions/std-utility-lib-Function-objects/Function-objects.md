# Function objects

Function as first class citizen.

## cppreference [Utility library#Function objects](https://en.cppreference.com/w/cpp/utility/functional)

A *function object* is any object for which the **function call operator** is defined. `C++` provides many built-in function objects as well as support for **creation** and **manipulation** of new function objects.

> NOTE: 上面这段话中的object，并不是指object-oriented programming中的object，而是指的[Object](https://en.cppreference.com/w/cpp/language/object)。

### Function wrappers

[std::function](https://en.cppreference.com/w/cpp/utility/functional/function) provides support for storing arbitrary function objects.

> NOTE: `std::function`是一个abstraction



### Function invocation

[std::invoke](https://en.cppreference.com/w/cpp/utility/functional/invoke) can invoke any [*Callable*](https://en.cppreference.com/w/cpp/named_req/Callable) object with given arguments.


|                                                              |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [invoke](https://en.cppreference.com/w/cpp/utility/functional/invoke)(C++17) | invokes any [*Callable*](https://en.cppreference.com/w/cpp/named_req/Callable) object with given arguments  (function template) |

> NOTE : [Function object](https://en.cppreference.com/w/cpp/named_req/FunctionObject)和[Callable object](https://en.cppreference.com/w/cpp/named_req/Callable)之间有什么异同？？

### Identity



### Partial function application

`std::bind_front` and [std::bind](https://en.cppreference.com/w/cpp/utility/functional/bind) provide support for [partial function application](https://en.wikipedia.org/wiki/Partial_application), i.e. binding arguments to functions to produce new functions.

> NOTE : Wikipedia的partial function application已经收录到youdao notebook了。

### Negators

[std::not_fn](https://en.cppreference.com/w/cpp/utility/functional/not_fn) creates a function object that negates the result of the callable object passed to it.

### Reference wrappers

Reference wrappers allow **reference** arguments to be stored in copyable function objects:

### Operator function objects

C++ defines several function objects that represent common arithmetic and logical operations:



#### Arithmetic operations

`std::plus`

在[`std::transform`](https://en.cppreference.com/w/cpp/algorithm/transform) 的 example中，使用了`std::plus`
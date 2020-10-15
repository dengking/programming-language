# Function objects

Function as first class citizen.

## cppreference [Utility library#Function objects](https://en.cppreference.com/w/cpp/utility/functional)

A *function object* is any object for which the **function call operator** is defined. `C++` provides many built-in function objects as well as support for **creation** and **manipulation** of new function objects.

> NOTE: 上面这段话中的object，并不是指object-oriented programming中的object，而是指的[Object](https://en.cppreference.com/w/cpp/language/object)。

### Function wrappers



|                                                              |                                                              |                             |
| ------------------------------------------------------------ | ------------------------------------------------------------ | --------------------------- |
| [std::function](https://en.cppreference.com/w/cpp/utility/functional/function)(C++11) | wraps callable object of any type with specified function call signature<br/>(class template) | 需要指定function的signature |
| [mem_fn](https://en.cppreference.com/w/cpp/utility/functional/mem_fn) (C++11) | creates a function object out of a pointer to a member<br/>(function template) |                             |

> NOTE: 
>
> wrapper and target：
>
> - `std::function`
> - `std::mem_fn`
> - `std::thread`
>
> wrapper是一种abstraction。
>
> `std::function`、`std::mem_fu`都是class template，所以它们的object是first class citizen。
>
> 如何描述它们的类型？

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
# Function objects

## What is Function objects?

cppreference中的如下章节介绍了function object: 

1) cppreference [Function #Function objects](https://en.cppreference.com/w/cpp/language/functions#Function_objects)

2) cppreference [C++ named requirements: *FunctionObject*](https://en.cppreference.com/w/cpp/named_req/FunctionObject)

function object的概念可以顾名思义地来进行理解，我们把它拆解开来，逐个理解:

function: 说明它需要支持the function call expression 

object: 说明它需要是object，那么这就排除了function lvalue、reference to function

### cppreference [Function #Function objects](https://en.cppreference.com/w/cpp/language/functions#Function_objects)



[*FunctionObject*s](https://en.cppreference.com/w/cpp/named_req/FunctionObject):

1) pointers to functions

2) any value of class type that overloads the **function-call operator** or is convertible to **function pointer** (including [lambda-expressions](https://en.cppreference.com/w/cpp/language/lambda)) (since C++11).

> NOTE: lambda expression本质上是一个unnamed function，因此它convertible to function pointer。

The standard library also provides a number of pre-defined [function object templates](https://en.cppreference.com/w/cpp/utility/functional) as well as the methods to compose new ones (including [std::less](https://en.cppreference.com/w/cpp/utility/functional/less), [std::mem_fn](https://en.cppreference.com/w/cpp/utility/functional/mem_fn), [std::bind](https://en.cppreference.com/w/cpp/utility/functional/bind), and [std::function](https://en.cppreference.com/w/cpp/utility/functional/function) (since C++11)).



### cppreference [C++ named requirements: *FunctionObject*](https://en.cppreference.com/w/cpp/named_req/FunctionObject)

#### Notes

Functions and references to functions are not function object types, but can be used where function object types are expected due to function-to-pointer [implicit conversion](https://en.cppreference.com/w/cpp/language/implicit_cast).

> NOTE: 章节传函数名即可。

## Application

Function object是我们首先function as first class的重要方式。



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
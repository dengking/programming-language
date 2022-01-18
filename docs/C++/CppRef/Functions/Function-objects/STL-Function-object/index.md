# cppreference [Utility library#Function objects](https://en.cppreference.com/w/cpp/utility/functional)

## Function wrappers

> NOTE: 关于wrapper，参见`C-and-C++\Wrapper`

|                                                              | paradigm | explanation                                                  | 章节             |
| ------------------------------------------------------------ | -------- | ------------------------------------------------------------ | ---------------- |
| [std::function](https://en.cppreference.com/w/cpp/utility/functional/function)(C++11) |          | wraps callable object of any type with specified function call signature<br/>(class template) | `./std-function` |
| [mem_fn](https://en.cppreference.com/w/cpp/utility/functional/mem_fn) (C++11) | OOP      | creates a function object out of a pointer to a member<br/>(function template) |                  |



## Partial function application

[std::bind_front](https://en.cppreference.com/w/cpp/utility/functional/bind_front) and [std::bind](https://en.cppreference.com/w/cpp/utility/functional/bind) provide support for [partial function application](https://en.wikipedia.org/wiki/Partial_application), i.e. binding arguments to functions to produce new functions.



## Operator function objects

> NOTE: 参见`STL-Functor`



## TODO



oopscenities [C++11: std::function and std::bind](https://oopscenities.net/2012/02/24/c11-stdfunction-and-stdbind/)
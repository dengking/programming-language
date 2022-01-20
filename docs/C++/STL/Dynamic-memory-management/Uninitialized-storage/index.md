# Uninitialized storage

和它相关的topic:

1、object lifetime management

2、memory pool

## cppreference [Dynamic memory management # Uninitialized storage](https://en.cppreference.com/w/cpp/memory)



## cppreference [Dynamic memory management # Uninitialized memory algorithms](https://en.cppreference.com/w/cpp/memory#Uninitialized_memory_algorithms)



### object lifetime management

|                                                              |                                                             |
| ------------------------------------------------------------ | ----------------------------------------------------------- |
| [destroy_at](https://en.cppreference.com/w/cpp/memory/destroy_at)(C++17) | destroys an object at a given address (function template)   |
| [destroy](https://en.cppreference.com/w/cpp/memory/destroy)(C++17) | destroys a range of objects (function template)             |
| [destroy_n](https://en.cppreference.com/w/cpp/memory/destroy_n)(C++17) | destroys a number of objects in a range (function template) |
| [construct_at](https://en.cppreference.com/w/cpp/memory/construct_at)(C++20) | creates an object at a given address (function template)    |

## cppreference [Dynamic memory management # Constrained uninitialized memory algorithms (C++20)](https://en.cppreference.com/w/cpp/memory#Constrained_uninitialized_memory_algorithms_(C++20))

> NOTE: 
>
> 其实是提供了range版本的

### object lifetime management

|                                                              |                                         |
| ------------------------------------------------------------ | --------------------------------------- |
| [ranges::destroy_at](https://en.cppreference.com/w/cpp/memory/ranges/destroy_at)(C++20) | destroys an object at a given address   |
| [ranges::destroy](https://en.cppreference.com/w/cpp/memory/ranges/destroy)(C++20) | destroys a range of objects             |
| [ranges::destroy_n](https://en.cppreference.com/w/cpp/memory/ranges/destroy_n)(C++20) | destroys a number of objects in a range |
| [ranges::construct_at](https://en.cppreference.com/w/cpp/memory/ranges/construct_at)(C++20) | creates an object at a given address    |


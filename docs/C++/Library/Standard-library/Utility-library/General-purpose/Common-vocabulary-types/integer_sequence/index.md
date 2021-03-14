# `std::integer_sequence`

1、是在学习 [Mazrog](https://github.com/Mazrog)/**[srlzio](https://github.com/Mazrog/srlzio)** 的implementation时，发现的`std::integer_sequence`，从中可以看出，`std::integer_sequence` + `std::tuple` 的威力。

2、[std::integer_sequence](https://en.cppreference.com/w/cpp/utility/integer_sequence) 所运用的是它的template parameter `Ints` ，而不是class template本身。

## cppreference [std::integer_sequence](https://en.cppreference.com/w/cpp/utility/integer_sequence)

```C++
template< class T, T... Ints >
class integer_sequence;
```

The class template `std::integer_sequence` represents a compile-time sequence of integers.

> NOTE: 
>
> 1、所指为它的template parameter `Ints`

### Helper templates

```C++
template<std::size_t... Ints>
using index_sequence = std::integer_sequence<std::size_t, Ints...>;
```



#### `make_index_sequence`

```C++
template<std::size_t N>
using make_index_sequence = std::make_integer_sequence<std::size_t, N>;
```

> NOTE: 
>
> 1、这是经常需要的一个功能，它非常类似于Python的 `range`

#### `index_sequence_for`

```C++
template<class... T>
using index_sequence_for = std::make_index_sequence<sizeof...(T)>;
```

> NOTE: 
>
> 1、这是一个非常常见的需求

## Before C++14 custom implementation

参见: 

1、`CSDN-C++14-make_index_sequence原理`

算法复杂度: O(N)

2、`stackoverflow-Implementation-C++14-make_integer_sequence`

算法复杂度: O(logN)



## TODO

fluentcpp [std::index_sequence and its Improvement in C++20](https://www.fluentcpp.com/2021/03/05/stdindex_sequence-and-its-improvement-in-c20/)
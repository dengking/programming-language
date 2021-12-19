# `std::integer_sequence`

1、是在学习 [Mazrog](https://github.com/Mazrog)/**[srlzio](https://github.com/Mazrog/srlzio)** 的implementation时，发现的`std::integer_sequence`，从中可以看出，`std::integer_sequence` + `std::tuple` 的威力。

2、[std::integer_sequence](https://en.cppreference.com/w/cpp/utility/integer_sequence) 所运用的是它的template parameter `Ints` ，而不是class template本身。

3、在使用 `std::integer_sequence` 的时候，需要注意的是: `std::integer_sequence` 是一个类型，传递的是它的对象

4、包含 `std::integer_sequence` 类型的参数，是需要有模板参数的

## cppreference [std::integer_sequence](https://en.cppreference.com/w/cpp/utility/integer_sequence)

```C++
template< class T, T... Ints >
class integer_sequence;
```

The class template `std::integer_sequence` represents a compile-time sequence of integers.

> NOTE: 
>
> 1、`T`是sequence中integer的具体类型，`Ints`所指为具体的sequence

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



### Example1

输出sequence，可以看到，函数模板`print_sequence`的模板参数就是`std::integer_sequence`的模板参数

```C++
#include <iostream>
#include <utility>
// debugging aid
template <typename T, T... ints>
void print_sequence(std::integer_sequence<T, ints...> int_seq)
{
    std::cout << "The sequence of size " << int_seq.size() << ": ";
    ((std::cout << ints << ' '), ...);
    std::cout << '\n';
}
int main()
{
    print_sequence(std::integer_sequence<unsigned, 9, 2, 5, 1, 9, 1, 6>{});
    print_sequence(std::make_integer_sequence<int, 20>{});
    print_sequence(std::make_index_sequence<10>{});
    print_sequence(std::index_sequence_for<float, std::iostream, char>{});
}
// g++ test.cpp --std=c++17 -pedantic -Wall -Wextra

```

输出如下:

```
The sequence of size 7: 9 2 5 1 9 1 6
The sequence of size 20: 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19
The sequence of size 10: 0 1 2 3 4 5 6 7 8 9
The sequence of size 3: 0 1 2
```



## Before C++14 custom implementation

参见: 

1、`CSDN-C++14-make_index_sequence原理`

算法复杂度: O(N)

2、`stackoverflow-Implementation-C++14-make_integer_sequence`

算法复杂度: O(logN)



## TODO

fluentcpp [std::index_sequence and its Improvement in C++20](https://www.fluentcpp.com/2021/03/05/stdindex_sequence-and-its-improvement-in-c20/)
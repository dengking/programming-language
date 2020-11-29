# `void_t`



## cppreference [`std::void_t`](https://en.cppreference.com/w/cpp/types/void_t) (since C++17)



```c++
template< class... >
using void_t = void;
```

> NOTE:  需要注意的是，上面使用的是**variable template**

Utility metafunction that maps a sequence of any types to the type `void`.



### Example

```C++
#include <iostream>
#include <type_traits>
#include <vector>
#include <map>
template<typename ...Ts> struct make_void
{
	using type = void;
};
template<typename ...Ts> using void_t = typename make_void<Ts...>::type;

class A
{
};

template<typename T, typename = void>
struct is_iterable: std::false_type
{
};
template<typename T>
struct is_iterable<T, void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end())>> : std::true_type
{
};

// An iterator trait which value_type is always the value_type of the
// iterated container, even with back_insert_iterator which value_type is void
template<typename T, typename = void>
struct iterator_trait: std::iterator_traits<T>
{
};

template<typename T>
struct iterator_trait<T, void_t<typename T::container_type>> : std::iterator_traits<typename T::container_type::iterator>
{
};

int main()
{
	std::cout << std::boolalpha;
	std::cout << is_iterable<std::vector<double>>::value << '\n';
	std::cout << is_iterable<std::map<int, double>>::value << '\n';
	std::cout << is_iterable<double>::value << '\n';
	std::cout << is_iterable<A>::value << '\n';

	std::vector<int> v;

	std::cout << std::is_same<iterator_trait<decltype(std::back_inserter(v))>::value_type, iterator_trait<decltype(v.cbegin())>::value_type>::value << '\n';
}
// g++ --std=c++11 test.cpp

```

> NOTE: 输出如下:
>
> ```C++
> true
> true
> false
> false
> true
> 
> ```
>
> 关于上述程序中`decltype`、`std::declval`的分析，参见`C++\Language-reference\Basic-concept\Type-system\Type-inference\decltype`章节。

## Application

> NOTE: 本段内容基于cppreference [`std::void_t`](https://en.cppreference.com/w/cpp/types/void_t) (since C++17)，并进行了一定的扩充。

### Member detection idiom

This metafunction is used in template metaprogramming to detect ill-formed types in SFINAE context:

```C++
// primary template handles types that have no nested ::type member:
template< class, class = void >
struct has_type_member : std::false_type { };
 
// specialization recognizes types that do have a nested ::type member:
template< class T >
struct has_type_member<T, std::void_t<typename T::type>> : std::true_type { };
```

> NOTE: member detection idiom，参见`C++\Language-reference\Template\Programming-paradigm\Idioms\Detection`章节。



### Detect validity of an expression

It can also be used to detect validity of an expression:

```C++
// primary template handles types that do not support pre-increment:
template< class, class = void >
struct has_pre_increment_member : std::false_type { };
// specialization recognizes types that do support pre-increment:
template< class T >
struct has_pre_increment_member<T,
           std::void_t<decltype( ++std::declval<T&>() )>
       > : std::true_type { };
```



在 riptutorial [C++ `void_t`](https://riptutorial.com/cplusplus/example/3778/void-t) 中也给出了一些详细的例子: 

> The primary application of `void_t` is writing type traits that check **validity** of a statement. 

For example, let's check if a type has a member function `foo()` that takes no arguments:

```c++
#include <utility> // std::declval
#include <type_trait> // std::false_type

template<class T, class = void>
struct has_foo: std::false_type
{
};

template<class T>
struct has_foo<T, void_t<decltype(std::declval<T&>().foo())>> : std::true_type
{
};
```



## Implementation

> NOTE: 本段内容基于cppreference [`std::void_t`](https://en.cppreference.com/w/cpp/types/void_t) (since C++17)，并进行了一定的扩充。

Until [CWG 1558](https://wg21.cmeerw.net/cwg/issue1558) (a C++11 defect), unused parameters in [alias templates](https://en.cppreference.com/w/cpp/language/type_alias) were not guaranteed to ensure SFINAE and could be ignored, so earlier compilers require a more complex definition of `void_t`, such as

> NOTE: 由于这个原因，所以在C++11中，使用下面的"C++14 variable template"段中的实现方式是无法生效的。

```C++
template<typename... Ts> struct make_void { typedef void type;};
template<typename... Ts> using void_t = typename make_void<Ts...>::type;
```

关于C++17 `std::void_t`的实现，参见：

1) stackoverflow [How does `void_t` work](https://stackoverflow.com/questions/27687389/how-does-void-t-work)，在`C++\Language-reference\Template\Implementation\index.md`中收录了这篇文章

2) riptutorial [C++ `void_t`](https://riptutorial.com/cplusplus/example/3778/void-t) ，在`C++\Language-reference\Template\Implementation\index.md`中收录了这篇文章



### C++11 implementation

nlohmann [`void_t.hpp`](https://github.com/nlohmann/json/blob/develop/include/nlohmann/detail/meta/void_t.hpp) 

```C++
#pragma once

namespace nlohmann
{
namespace detail
{
template <typename ...Ts> struct make_void
{
    using type = void;
};
template <typename ...Ts> using void_t = typename make_void<Ts...>::type;
} // namespace detail
}  // namespace nlohmann

```





### C++14 variable template

```C++
template< class... >
using void_t = void;
```

> NOTE: 需要注意的是，这种实现方式在C++11中是无法生效的，具体原因参见本节第一段。



## TO READ

提及`void_t`的文章：

- https://foonathan.net/2015/11/overload-resolution-4/
- https://www.bfilipek.com/2016/02/notes-on-c-sfinae.html#voidt


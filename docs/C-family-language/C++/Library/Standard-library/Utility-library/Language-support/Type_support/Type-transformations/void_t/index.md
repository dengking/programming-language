# `void_t`



## cppreference [`std::void_t`](https://en.cppreference.com/w/cpp/types/void_t) (since C++17)



```c++
template< class... >
using void_t = void;
```

> NOTE:  需要注意的是，上面使用的是**variable template**

Utility metafunction that maps a sequence of any types to the type `void`.

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

> NOTE: member detection idiom



#### wikipedia [Substitution failure is not an error](https://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error)

```c++
#include <iostream>
#include <type_traits>

template <typename... Ts>
using void_t = void;

template <typename T, typename = void>
struct has_typedef_foobar : std::false_type {};

template <typename T>
struct has_typedef_foobar<T, void_t<typename T::foobar>> : std::true_type {};

struct foo {
  using foobar = float;
};

int main() {
  std::cout << std::boolalpha;
  std::cout << has_typedef_foobar<int>::value << std::endl;
  std::cout << has_typedef_foobar<foo>::value << std::endl;
}
// g++ --std=c++11 test.cpp

```



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

> The primary application of `void_t` is writing type traits that check validity of a statement. 

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



## TO READ

提及`void_t`的文章：

- https://foonathan.net/2015/11/overload-resolution-4/
- https://www.bfilipek.com/2016/02/notes-on-c-sfinae.html#voidt


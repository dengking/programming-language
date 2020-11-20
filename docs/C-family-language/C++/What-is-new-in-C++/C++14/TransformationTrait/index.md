# TransformationTraits

阅读cppreference [Type support (basic types, RTTI, type traits)](https://en.cppreference.com/w/cpp/types)中的一些trait，就会发现，它们添加了很多`_t` type alias，比如[std::remove_reference](https://en.cppreference.com/w/cpp/types/remove_reference)

```C++
template< class T >
using remove_reference_t = typename remove_reference<T>::type;
```

这是C++14引入的新特性，这就是本章要介绍的内容。

## isocpp faq: Type transformation `_t` aliases [¶](https://isocpp.org/wiki/faq/cpp14-library#type-transformation-aliases) [Δ](https://isocpp.org/wiki/faq/cpp14-library#)

As part of the movement of C++ away from “traits” types, C++14 added type aliases to avoid having to spell out `typename` and `::type`, and actually make the traits more technically usable because they now work in deduced contexts.

```cpp
// C++11
... typename remove_reference<T>::type ...
... typename make_unsigned<T>::type ...
// new in C++14 
... remove_reference_t<T> ...
... make_unsigned_t<T> ...
```

> NOTE: 简化了变成，强化了TMP。



See also:

- [N3655] Walter E. Brown: [TransformationTraits Redux, v2](http://isocpp.org/files/papers/N3655.pdf). Note part 4 was not adopted.



## abseil [The Meta Library#C++14 `_t` type aliases](https://abseil.io/docs/cpp/guides/meta#c14-_t-type-aliases)

The Abseil `meta` library provides C++11 versions of `<type_traits>` aliases added to C++14. These aliases allow you to more easily (and intuitively) get the type of a `type_traits` class template.

For example:

```C++
// decay_t is the type of std::decay<T>
template <typename T>
using decay_t = typename std::decay<T>::type;
```

The Abseil `meta` library provides aliases for the following type traits that yield a type:

- `absl::remove_cv_t`
- `absl::remove_const_t`
- `absl::remove_volatile_t`
- `absl::add_cv_t`
- `absl::add_const_t`
- `absl::add_volatile_t`
- `absl::remove_reference_t`
- `absl::add_lvalue_reference_t`
- `absl::add_rvalue_reference_t`



## Before C++14

在C++14之前，可以自定义的方式来实现:

### [nlohmann](https://github.com/nlohmann)/[json](https://github.com/nlohmann/json)/[`cpp_future.hpp`](https://github.com/nlohmann/json/blob/develop/include/nlohmann/detail/meta/cpp_future.hpp) 

```C++
#pragma once

#include <cstddef> // size_t
#include <type_traits> // conditional, enable_if, false_type, integral_constant, is_constructible, is_integral, is_same, remove_cv, remove_reference, true_type

namespace nlohmann
{
namespace detail
{
// alias templates to reduce boilerplate
template<bool B, typename T = void>
using enable_if_t = typename std::enable_if<B, T>::type;

template<typename T>
using uncvref_t = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
}
}

```


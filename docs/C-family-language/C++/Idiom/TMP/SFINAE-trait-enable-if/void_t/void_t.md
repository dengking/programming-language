# `void_t`



## cppreference `std::void_t`

> 本地路径: file:///D:/github/dengking/programming-language-theory/programming-language/docs/C-family-language/C-and-C++/cppreference/reference/en/cpp/types/void_t.html

```c++
template< class... >
using void_t = void;
```

> NOTE:  需要注意的是，上面使用的是**variable template**

## Implementation

关于C++17 `std::void_t`的实现，参见：

- stackoverflow [How does `void_t` work](https://stackoverflow.com/questions/27687389/how-does-void-t-work)，在`C++\Language-reference\Template\Implementation\index.md`中收录了这篇文章
- riptutorial [C++ `void_t`](https://riptutorial.com/cplusplus/example/3778/void-t) ，在`C++\Language-reference\Template\Implementation\index.md`中收录了这篇文章



### C++11 implementation

nlohmann `void_t.hpp`

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



## Application

### Check validity of a statement

在 riptutorial [C++ `void_t`](https://riptutorial.com/cplusplus/example/3778/void-t) 中對這個問題進行了詳細的説明

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



## TO READ



提及`void_t`的文章：

- https://foonathan.net/2015/11/overload-resolution-4/
- https://www.bfilipek.com/2016/02/notes-on-c-sfinae.html#voidt

## Example

### 维基百科[Substitution failure is not an error](https://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error)

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


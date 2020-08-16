# `void_t`



## cppreference `std::void_t`

本地路径: file:///D:/github/dengking/programming-language-theory/programming-language/docs/C-family-language/C-and-C++/cppreference/reference/en/cpp/types/void_t.html

```c++
template< class... >
using void_t = void;
```



## TO READ

https://stackoverflow.com/questions/27687389/how-does-void-t-work

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


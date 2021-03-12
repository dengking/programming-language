# Detection idiom

## 涉及Detection idiom的章节

1、`SFINAE`

2、`SFINAE\Expression-SFINAE`

## Example

### wikipedia [Substitution failure is not an error](https://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error)



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



正确版本如下:

```C++
#include <iostream>
#include <type_traits>

template<typename ...Ts> struct make_void
{
	using type = void;
};
template<typename ...Ts> using void_t = typename make_void<Ts...>::type;

template<typename T, typename = void>
struct has_typedef_foobar: std::false_type
{
};

template<typename T>
struct has_typedef_foobar<T, void_t<typename T::foobar>> : std::true_type
{
};

struct foo
{
	using foobar = float;
};

int main()
{
	std::cout << std::boolalpha;
	std::cout << has_typedef_foobar<int>::value << std::endl;
	std::cout << has_typedef_foobar<foo>::value << std::endl;
}
// g++ --std=c++11 test.cpp

```





### `std::experimental::is_detected`

参见 `C++\Library\Standard-library\Extensions\Version-2\The-C++detection-idiom` 章节。



### stackoverflow [What is “Expression SFINAE”?](https://stackoverflow.com/questions/12654067/what-is-expression-sfinae)

收录在了`SFINAE\Expression-SFINAE`章节。
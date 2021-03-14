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



### cppreference [std::is_member_function_pointer](https://en.cppreference.com/w/cpp/types/is_member_function_pointer)



### gist [sfinae_tostring_ex.cpp](https://gist.github.com/fenbf/d2cd670704b82e2ce7fd#file-sfinae_tostring_ex-cpp)



https://gist.github.com/fenbf/d2cd670704b82e2ce7fd

### gist [has_member.hpp](https://gist.github.com/maddouri/0da889b331d910f35e05ba3b7b9d869b#file-has_member-hpp)

https://gist.github.com/fenbf/d2cd670704b82e2ce7fd







### Detect member function

- stackoverflow [Templated check for the existence of a class member function?](https://stackoverflow.com/questions/257288/templated-check-for-the-existence-of-a-class-member-function)

- stackoverflow [Check if a class has a member function of a given signature](https://stackoverflow.com/questions/87372/check-if-a-class-has-a-member-function-of-a-given-signature)

#### [A](https://stackoverflow.com/a/257382): SFINA+`decltype`

```c++
#include <iostream>

struct Hello
{
	int helloworld()
	{
		return 0;
	}
};

struct Generic
{
};

// SFINAE test
template<typename T>
class has_helloworld
{
	typedef char one;
	struct two
	{
		char x[2];
	};

	template<typename C> static one test(typeof(&C::helloworld));
	template<typename C> static two test(...);

public:
	enum
	{
		value = sizeof(test<T>(0)) == sizeof(char)
	};
};

int main(int argc, char *argv[])
{
	std::cout << has_helloworld<Hello>::value << std::endl;
	std::cout << has_helloworld<Generic>::value << std::endl;
	return 0;
}
// g++ test.cppm

```



#### [A](https://stackoverflow.com/a/9154394): expression SFINAE 

This question is old, but with `C++11` we got a new way to check for a functions existence (or existence of any non-type member, really), relying on SFINAE again:

```cpp
template<class T>
auto serialize_imp(std::ostream& os, T const& obj, int)
    -> decltype(os << obj, void())
{
  os << obj;
}

template<class T>
auto serialize_imp(std::ostream& os, T const& obj, long)
    -> decltype(obj.stream(os), void())
{
  obj.stream(os);
}

template<class T>
auto serialize(std::ostream& os, T const& obj)
    -> decltype(serialize_imp(os, obj, 0), void())
{
  serialize_imp(os, obj, 0);
}
```

Now onto some explanations. First thing, I use [expression SFINAE](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2634.html) to exclude the `serialize(_imp)` functions from **overload resolution**, if the first expression inside `decltype` isn't valid (aka, the function doesn't exist).

The `void()` is used to make the return type of all those functions `void`.

The `0` argument is used to prefer the `os << obj` overload if both are available (literal `0` is of type `int` and as such the first overload is a better match).





## TODO

https://www.codeplay.com/portal/09-15-17-detection-idiom-a-stopgap-for-concepts

https://stackoverflow.com/questions/45249985/how-to-require-an-exact-function-signature-in-the-detection-idiom

https://humanreadablemag.com/issues/1/articles/cpp-evolution-via-detection-idiom-lens

https://blog.tartanllama.xyz/detection-idiom/

https://stackoverflow.com/questions/257288/templated-check-for-the-existence-of-a-class-member-function/9154394#9154394

https://stackoverflow.com/questions/87372/check-if-a-class-has-a-member-function-of-a-given-signature

https://stackoverflow.com/questions/257288/templated-check-for-the-existence-of-a-class-member-function

https://stackoverflow.com/questions/18570285/using-sfinae-to-detect-a-member-function
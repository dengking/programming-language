# Function template SFINAE



## cpppatterns [Function template SFINAE](https://cpppatterns.com/patterns/function-template-sfinae.html)

```c++
#include <type_traits>
#include <limits>
#include <cmath>
template <typename T>
typename std::enable_if<std::is_integral<T>::value, bool>::type
  equal(T lhs, T rhs)
{
  return lhs == rhs;
}
template <typename T>
typename std::enable_if<std::is_floating_point<T>::value, bool>::type
  equal(T lhs, T rhs)
{
  return std::abs(lhs - rhs) < 0.0001;
}
```



### Requires

[c++11](https://cpppatterns.com/#/search/c++11) or newer.

### INTENT

Conditionally instantiate a function template depending on the template arguments.

### DESCRIPTION

We provide two implementations of the `equal` function template:

1. The template on [lines 5–10](https://cpppatterns.com/patterns/function-template-sfinae.html#line5) will only be instantiated when `T` is an integral type.
2. The template on [lines 12–17](https://cpppatterns.com/patterns/function-template-sfinae.html#line12) will only be instantiated when `T` is a floating point type.

We have used [`std::enable_if`](http://en.cppreference.com/w/cpp/types/enable_if) on [line 6](https://cpppatterns.com/patterns/function-template-sfinae.html#line6) and [line 13](https://cpppatterns.com/patterns/function-template-sfinae.html#line13) to force instantiation to succeed only for the appropriate template arguments. This relies on [Substitution Failure Is Not An Error](https://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error) (SFINAE), which states that failing to instantiate a template with some particular template arguments does not result in an error and simply discards that instantiation.

The second template argument of `std::enable_if` — in this case, `bool` — is what the full `std::enable_if<...>::type` evaluates to when the first template argument is `true`. This means that the return type of `equal` will be `bool`.

If you want to simply prevent a template from being instantiated for certain template arguments, consider using [`static_assert`](http://en.cppreference.com/w/cpp/language/static_assert) instead.

> NOTE: 上述解释和cpppatterns [Class template SFINAE](https://cpppatterns.com/patterns/class-template-sfinae.html)如出一辙。



## Function template中SFINAE的作用对象

在class template中，SFINAE只能够作用于argument of type template parameter，在function template中，SFINAE可以作用于如下对象：

- SFINAE working in return type，cpppatterns [Function template SFINAE](https://cpppatterns.com/patterns/function-template-sfinae.html)就是这种写法
- SFINAE working in dummy template parameter
- SFINAE working in default argument of type template parameter

下面结合具体例子来对上述几种情况进行介绍：

### SFINAE working in return type

cpppatterns [Function template SFINAE](https://cpppatterns.com/patterns/function-template-sfinae.html)就是这种写法，使用c++11的[trailing return type](https://en.cppreference.com/w/cpp/language/function)的写法类似如下：

```c++
template<typename T>
auto foo(T)
    -> typename std::enable_if<std::is_integral<T>::value>::type
{
    std::cout << "I'm an integrer!\n";
}

template<typename T>
auto foo(T)
    -> typename std::enable_if<std::is_floating_point<T>::value>::type
{
    std::cout << "I'm a floating point number!\n";
}
```

上述代码取自：[SFINAE working in return type but not as template parameter](https://stackoverflow.com/questions/15427667/sfinae-working-in-return-type-but-not-as-template-parameter)





#### SFINAE and Function template and *trailing-return-type*

在阅读[redis-plus-plus](https://github.com/sewenew/redis-plus-plus)的[redis.h](https://github.com/sewenew/redis-plus-plus/blob/master/src/sw/redis%2B%2B/redis.h)时，发现了类似于这样的语法

```c++
    template <typename ...Args>
    auto command(const StringView &cmd_name, Args &&...args)
        -> typename std::enable_if<IsIter<typename LastType<Args...>::type>::value, void>::type;
```



### SFINAE working in dummy template parameter

To fix the error move the `enable_if` expression from default argument to a **dummy template parameter**

```c++
template <class T,
          typename std::enable_if<std::is_floating_point<T>::value, int>::type* = nullptr>
     bool less(T a, T b) {
  // ....
}

template <class T,
          typename std::enable_if<std::is_integral<T>::value, int>::type* = nullptr>
     bool less(T a, T b) {
  // ....
}
```

上述代码取自：https://stackoverflow.com/a/29502338

需要注意的是，上述第二个模板参数是一个non-type template parameter。



### SFINAE working in default argument of type template parameter

这种用法是比较不好的，在下面文章中对这种写法进行了剖析：

- [SFINAE working in return type but not as template parameter](https://stackoverflow.com/questions/15427667/sfinae-working-in-return-type-but-not-as-template-parameter)
- [Template specialization and enable_if problems [duplicate]](https://stackoverflow.com/questions/29502052/template-specialization-and-enable-if-problems)

样例程序1取自[SFINAE working in return type but not as template parameter](https://stackoverflow.com/questions/15427667/sfinae-working-in-return-type-but-not-as-template-parameter)，如下：

```c++
template<typename T,
         typename = typename std::enable_if<std::is_integral<T>::value>::type>
auto foo(T)
    -> void
{
    std::cout << "I'm an integer!\n";
}

template<typename T,
         typename = typename std::enable_if<std::is_floating_point<T>::value>::type>
auto foo(T)
    -> void
{
    std::cout << "I'm a floating point number!\n";
}
```



样例程序2取自[Template specialization and enable_if problems [duplicate]](https://stackoverflow.com/questions/29502052/template-specialization-and-enable-if-problems)，如下：

```c++
#include <type_traits>
#include <iostream>

template <class T,
          class = typename std::enable_if<std::is_floating_point<T>::value>::type>
     bool less(T a, T b) {
  // ....
}

template <class T,
          class = typename std::enable_if<std::is_integral<T>::value>::type>
     bool less(T a, T b) {
  // ....
}

int main() {
    float a;
    float b;
    less(a,b);
    return 0;
}
```

在[回答](https://stackoverflow.com/a/15428104)中，给出的解释是比较清晰的：

> You should take a look at `14.5.6.1 Function template overloading` (C++11 standard) where function templates equivalency is defined. In short, default template arguments are not considered, so in the 1st case you have the same function template defined twice. In the 2nd case you have expression referring template parameters used in the return type (again see 14.5.6.1/4). Since this expression is part of signature you get two different function template declarations and thus SFINAE get a chance to work.

在[Template specialization and enable_if problems [duplicate]](https://stackoverflow.com/questions/29502052/template-specialization-and-enable-if-problems)中，也提供了回答。



## Examples

### Generic container printer

[Using SFINAE to specialize template methods, depending on the type of container std::map or std::vector, passed as an argument](https://evileg.com/en/post/468/)

```c++
#include <iterator>
#include <type_traits>

// templates for determining whether the std::pair container element is
template<typename T>
struct is_pair: std::false_type
{
};

template<typename T, typename U>
struct is_pair<std::pair<T, U>> : std::true_type
{
};

template<typename T>
constexpr bool is_pair_v = is_pair<T>::value;

// templates for determining if the container is std::map
template<typename, typename = void>
struct is_mapping: std::false_type
{
};

template<typename Container>
struct is_mapping<Container, std::enable_if_t<is_pair_v<typename std::iterator_traits<typename Container::iterator>::value_type>>> : std::true_type
{
};

template<typename T>
constexpr bool is_mapping_v = is_mapping<T>::value;

#include <map>
#include <vector>
#include <iostream>

class ClassWithSpecializedMethods
{
public:
	ClassWithSpecializedMethods()
	{
	}

	// A specialized method for handling std::map
	template<class ContainerType>
	static typename std::enable_if<is_mapping_v<ContainerType>, void>::type printContainer(ContainerType &container);

	// A specialized method for handling std::vector
	template<class ContainerType>
	static typename std::enable_if<!is_mapping_v<ContainerType>, void>::type printContainer(ContainerType &container);
};

// Method implementations
template<class ContainerType>
inline typename std::enable_if<is_mapping_v<ContainerType>, void>::type ClassWithSpecializedMethods::printContainer(ContainerType &container)
{
	std::cout << "Map:" << std::endl;
	for (const auto& [key, value] : container)
	{
		std::cout << "Key: " << key << " Value: " << value << std::endl;
	}
	std::cout << std::endl;
}

template<class ContainerType>
inline typename std::enable_if<!is_mapping_v<ContainerType>, void>::type ClassWithSpecializedMethods::printContainer(ContainerType &container)
{
	std::cout << "Vector:" << std::endl;
	for (const auto &value : container)
	{
		std::cout << "Value: " << value << std::endl;
	}
	std::cout << std::endl;
}

// Testing functions
int main()
{
	std::cout << "is_pair:" << std::endl;
	std::cout << "Map:    " << is_pair_v<std::iterator_traits<std::map<int, int>::iterator>::value_type> << std::endl;
	std::cout << "Vector: " << is_pair_v<std::iterator_traits<std::vector<int>::iterator>::value_type> << std::endl;
	std::cout << std::endl;
	std::cout << "is_mapping:" << std::endl;
	std::cout << "Map:    " << is_mapping_v<std::map<int, int>> << std::endl;
	std::cout << "Vector: " << is_mapping_v<std::vector<int>> << std::endl;
	std::cout << std::endl;

	std::map<int, int> map_container = { { 1, 1 }, { 2, 2 }, { 3, 3 } };
	std::vector<int> vector_container = { 1, 2, 3 };

	ClassWithSpecializedMethods::printContainer(map_container);
	ClassWithSpecializedMethods::printContainer(vector_container);
}

// g++   --std=c++17 -Wall -pedantic -pthread main.cpp && ./a.out

```

输出如下:

```C++
is_pair:
Map:    1
Vector: 0

is_mapping:
Map:    1
Vector: 0

Map:
Key: 1 Value: 1
Key: 2 Value: 2
Key: 3 Value: 3

Vector:
Value: 1
Value: 2
Value: 3
```



## TODO

[Enable_if for template function specialization](https://stackoverflow.com/questions/48097889/enable-if-for-template-function-specialization)



[std::enable_if to conditionally compile a member function](https://stackoverflow.com/questions/6972368/stdenable-if-to-conditionally-compile-a-member-function)


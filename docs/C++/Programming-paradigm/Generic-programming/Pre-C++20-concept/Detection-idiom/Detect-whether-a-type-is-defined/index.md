# Detecting in C++ whether a type is defined

1、仅仅declare但是没有definition的type，就是incomplete type，参见 `Incomplete-type` 章节

## Detecting in C++ whether a type is defined



### stackoverflow [Check if type is defined](https://stackoverflow.com/questions/39816779/check-if-type-is-defined)

Consider this example:

```cpp
#include <iostream>
#include <type_traits>

template <class, class = void>
struct is_defined : std::false_type
{ };

template <class T>
struct is_defined<T,
    std::enable_if_t<std::is_object<T>::value &&
                    !std::is_pointer<T>::value
        >
    > : std::true_type
{
private:
    static const T test; //try to create incomplete type member
};

struct defined { };

struct forward_declared;

int main()
{
    std::cout << std::boolalpha
              << is_defined<defined>::value << std::endl
              << is_defined<forward_declared>::value << std::endl;
}
// g++ --std=C++14 test.cpp
```



> NOTE: 
>
> 1、下面是C++11版本
>
> ```c++
> #include <iostream>
> #include <type_traits>
> 
> template<class, class = void>
> struct is_defined: std::false_type
> {
> };
> 
> template<class T>
> struct is_defined<T, typename std::enable_if<std::is_object<T>::value && !std::is_pointer<T>::value>::type> : std::true_type
> {
> private:
> 	static const T test; //try to create incomplete type member
> };
> 
> struct defined
> {
> };
> 
> struct forward_declared;
> 
> int main()
> {
> 	std::cout << std::boolalpha << is_defined<defined>::value << std::endl << is_defined<forward_declared>::value << std::endl;
> }
> // g++ --std=c++11 test.cpp
> ```
>
> 
>
> 

Output is `true` for both. I thought if I try to make `struct` member of incomplete type, then this template specialization would be discarded from overload set. But it isn't. Removal of `static const` causes incomplete-type compile-time error. What is wrong with this approach, and if it's possible, how could this be implemented?

#### [A](https://stackoverflow.com/a/39816909)

```C++
struct is_defined<T,
    std::enable_if_t<std::is_object<T>::value &&
                    !std::is_pointer<T>::value &&
                    (sizeof(T) > 0)
        >
    > : std::true_type
{
};
```

> NOTE: 
>
> 1、完整测试程序如下:
>
> ```C++
> #include <iostream>
> #include <type_traits>
> 
> template<class, class = void>
> struct is_defined: std::false_type
> {
> };
> 
> template<class T>
> struct is_defined<T, typename std::enable_if<std::is_object<T>::value && !std::is_pointer<T>::value && (sizeof(T) > 0)>::type> : std::true_type
> {
> private:
> 	static const T test; //try to create incomplete type member
> };
> 
> struct defined
> {
> };
> 
> struct forward_declared;
> 
> int main()
> {
> 	std::cout << std::boolalpha << is_defined<defined>::value << std::endl << is_defined<forward_declared>::value << std::endl;
> }
> // g++ --std=c++11 test.cpp
> 
> ```
>
> 

#### [A](https://stackoverflow.com/a/39818497)

In general, in this case you can use for your sfinae expression some of those operators that don't accept incomplete types.
As an example you can use `typeid`:

```cpp
#include <iostream>
#include <type_traits>
#include <utility>

template<typename T, typename = void>
constexpr bool is_defined = false;

template<typename T>
constexpr bool is_defined<T, decltype(typeid(T), void())> = true;

struct defined
{
};
struct forward_declared;

int main()
{
	std::cout << std::boolalpha << is_defined<defined> << std::endl << is_defined<forward_declared> << std::endl;
}
// g++ --std=c++14 test.cpp
```

> NOTE: 
>
> 1、上述使用了C++14 template variable 特性

As mentioned by others, another *valid* operator is `sizeof`.



## Detecting in C++ whether a type is defined in a class type 

思路:`is_defined`  + SFINAE，初步测试程序如下:

```C++
#include <iostream>
#include <type_traits>
template<class, class = void>
struct is_defined: std::false_type
{
};

template<class T>
struct is_defined<T, typename std::enable_if<std::is_object<T>::value && !std::is_pointer<T>::value && (sizeof(T) > 0)>::type> : std::true_type
{
private:
	static const T test; //try to create incomplete type member
};

struct test
{
};
struct defined
{
	using t = test;
};

template<typename T>
struct is_has_member_type_t
{
	template<typename U>
	static constexpr auto get_v(int) -> typename std::enable_if< is_defined<typename U::t>::value, bool>::type
	{

		return true;
	}
	template<typename U>
	static constexpr auto get_v(...)->bool
	{
		return false;
	}
	constexpr static bool value = get_v<T>(0);
};

template<typename T>
struct is_has_member_type_t2
{
	template<typename U>
	static constexpr auto get_v(int) -> typename std::enable_if< is_defined<typename U::t2>::value, bool>::type
	{
		return true;
	}
	template<typename U>
	static constexpr auto get_v(...)->bool
	{
		return false;
	}
	constexpr static bool value = get_v<T>(0);
};

int main()
{

	std::cout << std::boolalpha << is_has_member_type_t<defined>::value << std::endl;
	std::cout << std::boolalpha << is_has_member_type_t2<defined>::value << std::endl;
}
// g++ --std=c++11 test.cpp

```

输出如下:

```C++
true
false
```

显然，符合预期。

### 封装为macro

#### 参考程序

```C++
#include <iostream>
#include <type_traits>
// A compile-time method for checking the existence of a class member
// @see https://general-purpose.io/2017/03/10/checking-the-existence-of-a-cpp-class-member-at-compile-time/

// This code uses "decltype" which, according to http://en.cppreference.com/w/cpp/compiler_support
// should be supported by Clang 2.9+, GCC 4.3+ and MSVC 2010+ (if you have an older compiler, please upgrade :)
// As of "constexpr", if not supported by your compiler, you could try "const"
// or use the value as an inner enum value e.g. enum { value = ... }

// check "test_has_member.cpp" for a usage example

/// Defines a "has_member_member_name" class template
///
/// This template can be used to check if its "T" argument
/// has a data or function member called "member_name"
#define define_has_member(member_name)                                         \
    template <typename T>                                                      \
    class has_member_##member_name                                             \
    {                                                                          \
        typedef char yes_type;                                                 \
        typedef long no_type;                                                  \
        template <typename U> static yes_type test(decltype(&U::member_name)); \
        template <typename U> static no_type  test(...);                       \
    public:                                                                    \
        static constexpr bool value = sizeof(test<T>(0)) == sizeof(yes_type);  \
    }

/// Shorthand for testing if "class_" has a member called "member_name"
///
/// @note "define_has_member(member_name)" must be used
///       before calling "has_member(class_, member_name)"
#define has_member(class_, member_name)  has_member_##member_name<class_>::value

/**
 * @brief 
 * 
 * @tparam T
 */
define_has_member(a);
define_has_member(b);
define_has_member(c);

struct Test
{
	int a;
	int b;
};
int main()
{

	std::cout << std::boolalpha << has_member(Test, a) << std::endl;
	std::cout << std::boolalpha << has_member(Test, b) << std::endl;
	std::cout << std::boolalpha << has_member(Test, c) << std::endl;
}
// g++ --std=c++11 test.cpp


```

输出如下:

```C++
true
true
false
```

#### 完整程序

```C++
#include <iostream>
#include <type_traits>
template<class, class = void>
struct is_defined: std::false_type
{
};

template<class T>
struct is_defined<T, typename std::enable_if<std::is_object<T>::value && !std::is_pointer<T>::value && (sizeof(T) > 0)>::type> : std::true_type
{
private:
	static const T test; //try to create incomplete type member
};

#define define_has_member_type(member_type_name)                                            \
template<typename T>																		\
struct has_member_type_##member_type_name													\
{																							\
	template<typename U>																	\
	static constexpr auto test(int) -> typename std::enable_if< is_defined<typename U::member_type_name>::value, bool>::type \
	{                                                                                                                        \
		return true;                                                                                                         \
	}                                                                                                                        \
	template<typename U>                                                                                                     \
	static constexpr auto test(...)->bool \
	{                                     \
		return false;                     \
	}                                     \
	constexpr static bool value = test<T>(0); \
}

/// Shorthand for testing if "class_" has a member called "member_name"
///
/// @note "define_has_member_type(member_name)" must be used
///       before calling "has_member_type(class_, member_name)"
#define has_member_type(class_, member_type_name)  has_member_type_##member_type_name<class_>::value

define_has_member_type(t);define_has_member_type(t2);

struct test
{
};
struct defined
{
	using t = test;
};

int main()
{

	std::cout << std::boolalpha << has_member_type(defined, t) << std::endl;
	std::cout << std::boolalpha << has_member_type(defined, t2) << std::endl;
}
// g++ --std=c++11 test.cpp

```

输出如下:

```
true
false
```


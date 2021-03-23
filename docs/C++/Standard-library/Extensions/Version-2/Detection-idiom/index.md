# The C++ detection idiom

在阅读wikipedia [Substitution failure is not an error](https://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error) 时，其中有 `std::is_detected` 的描述: 

> With the standardisation of the detection idiom in the [Library fundamental v2 (n4562)](http://en.cppreference.com/w/cpp/experimental/lib_extensions_2) proposal, the above code could be re-written as follows:
>
> ```C++
> #include <iostream>
> #include <type_traits>
> 
> template<typename T>
> using has_typedef_foobar_t = typename T::foobar;
> 
> struct foo
> {
> 	using foobar = float;
> };
> 
> int main()
> {
> 	std::cout << std::boolalpha;
> 	std::cout << std::is_detected<has_typedef_foobar_t, int>::value << std::endl;
> 	std::cout << std::is_detected<has_typedef_foobar_t, foo>::value << std::endl;
> }
> 
> ```
>
> 

显然，C++ standard library试图提供detection idiom的标准版。

## cppreference [The C++ detection idiom](https://en.cppreference.com/w/cpp/experimental/lib_extensions_2#The_C++_detection_idiom)



```C++
#include <experimental/type_traits>
#include <cstddef>
#include <utility>

template<class T>
using copy_assign_t = decltype(std::declval<T&>() = std::declval<const T&>());

struct Meow
{
};
struct Purr
{
	void operator=(const Purr&) = delete;
};

static_assert(std::experimental::is_detected<copy_assign_t, Meow>::value,
				"Meow should be copy assignable!");
static_assert(!std::experimental::is_detected_v<copy_assign_t, Purr>,
				"Purr should not be copy assignable!");
static_assert(std::experimental::is_detected_exact_v<Meow&, copy_assign_t, Meow>,
				"Copy assignment of Meow should return Meow&!");

template<class T>
using diff_t = typename T::difference_type;

template<class Ptr>
using difference_type = std::experimental::detected_or_t<std::ptrdiff_t, diff_t, Ptr>;

struct Woof
{
	using difference_type = int;
};
struct Bark
{
};

static_assert(std::is_same<difference_type<Woof>, int>::value,
				"Woof's difference_type should be int!");
static_assert(std::is_same<difference_type<Bark>, std::ptrdiff_t>::value,
				"Bark's difference_type should be ptrdiff_t!");

int main()
{
}

```

> NOTE: 
>
> 1、上述code中，没有给出`nonesuch`的定义，在  riptutorial [C++ is_detected](https://riptutorial.com/cplusplus/example/18585/is-detected) 中，给出了完整的定义

### Example

```C++
#include <experimental/type_traits>
#include <cstddef>
 
template<class T>
using copy_assign_t = decltype(std::declval<T&>() = std::declval<const T&>());
 
struct Meow { };
struct Purr { void operator=(const Purr&) = delete; };
 
static_assert(std::experimental::is_detected<copy_assign_t, Meow>::value,
              "Meow should be copy assignable!");
static_assert(!std::experimental::is_detected_v<copy_assign_t, Purr>,
              "Purr should not be copy assignable!");
static_assert(std::experimental::is_detected_exact_v<Meow&, copy_assign_t, Meow>,
              "Copy assignment of Meow should return Meow&!");
 
template<class T>
using diff_t = typename T::difference_type;
 
template <class Ptr>
using difference_type = std::experimental::detected_or_t<std::ptrdiff_t, diff_t, Ptr>;
 
struct Woof { using difference_type = int; };
struct Bark {};
 
static_assert(std::is_same<difference_type<Woof>, int>::value,
              "Woof's difference_type should be int!");
static_assert(std::is_same<difference_type<Bark>, std::ptrdiff_t>::value,
              "Bark's difference_type should be ptrdiff_t!");
 
int main() {}
```

## 实现分析

在下面文章中给出了对其实现源代码的分析，可以借鉴。

### riptutorial [C++ is_detected](https://riptutorial.com/cplusplus/example/18585/is-detected)

To generalize type_trait creation: based on SFINAE there are experimental traits `detected_or`, `detected_t`, `is_detected`.

With template parameters `typename Default`, `template <typename...> Op` and `typename ... Args`:

1、`is_detected`: alias of `std::true_type` or `std::false_type` depending of the validity of `Op<Args...>`

2、`detected_t`: alias of `Op<Args...>` or `nonesuch` depending of validity of `Op<Args...>`.

3、`detected_or`: alias of a struct with `value_t` which is `is_detected`, and `type` which is `Op<Args...>` or `Default` depending of validity of `Op<Args...>`

which can be implemented using `std::void_t` for SFINAE as following:

C++17

```C++
#include <type_traits>
namespace detail
{
template<class Default, class AlwaysVoid, template<class ...> class Op, class... Args>
struct detector
{
	using value_t = std::false_type;
	using type = Default;
};

template<class Default, template<class ...> class Op, class... Args>
struct detector<Default, std::void_t<Op<Args...>>, Op, Args...>
{
	using value_t = std::true_type;
	using type = Op<Args...>;
};

}
 // namespace detail

// special type to indicate detection failure
struct nonesuch
{
	nonesuch() = delete;
	~nonesuch() = delete;
	nonesuch(nonesuch const&) = delete;
	void operator=(nonesuch const&) = delete;
};

template<template<class ...> class Op, class... Args>
using is_detected =
typename detail::detector<nonesuch, void, Op, Args...>::value_t;

template<template<class ...> class Op, class... Args>
using detected_t = typename detail::detector<nonesuch, void, Op, Args...>::type;

template<class Default, template<class ...> class Op, class... Args>
using detected_or = detail::detector<Default, void, Op, Args...>;

```

> NOTE: 
>
> 1、上述仅仅用到了C++ 17 `is_void` 

Traits to detect presence of method can then be simply implemented:

```C++
typename <typename T, typename ...Ts>
using foo_type = decltype(std::declval<T>().foo(std::declval<Ts>()...));

struct C1 {};

struct C2 {
    int foo(char) const;
};

template <typename T>
using has_foo_char = is_detected<foo_type, T, char>;

static_assert(!has_foo_char<C1>::value, "Unexpected");
static_assert(has_foo_char<C2>::value, "Unexpected");

static_assert(std::is_same<int, detected_t<foo_type, C2, char>>::value,
              "Unexpected");

static_assert(std::is_same<void, // Default
                           detected_or<void, foo_type, C1, char>>::value,
              "Unexpected");
static_assert(std::is_same<int, detected_or<void, foo_type, C2, char>>::value,
              "Unexpected");
```

> NOTE: 
>
> 1、上述code，存在一个非常明确明显的错误:
>
> ```C++
> typename <typename T, typename ...Ts>
> using foo_type = decltype(std::declval<T>().foo(std::declval<Ts>()...));
> ```
>
> 应该是:
>
> ```C++
> template <typename T, typename ...Ts>
> using foo_type = decltype(std::declval<T>().foo(std::declval<Ts>()...));
> ```
>
> 2、完整测试程序如下:
>
> C++17版本:
>
> ```c++
> #include <type_traits>
> #include <utility> // std::declval
> 
> #pragma once
> 
> namespace detail
> {
> template<class Default, class AlwaysVoid, template<class ...> class Op, class... Args>
> struct detector
> {
> 	using value_t = std::false_type;
> 	using type = Default;
> };
> 
> template<class Default, template<class ...> class Op, class... Args>
> struct detector<Default, std::void_t<Op<Args...>>, Op, Args...>
> {
> 	using value_t = std::true_type;
> 	using type = Op<Args...>;
> };
> 
> }
>  // namespace detail
> 
> // special type to indicate detection failure
> struct nonesuch
> {
> 	nonesuch() = delete;
> 	~nonesuch() = delete;
> 	nonesuch(nonesuch const&) = delete;
> 	void operator=(nonesuch const&) = delete;
> };
> 
> template<template<class ...> class Op, class... Args>
> using is_detected =
> typename detail::detector<nonesuch, void, Op, Args...>::value_t;
> 
> template<template<class ...> class Op, class... Args>
> using detected_t = typename detail::detector<nonesuch, void, Op, Args...>::type;
> 
> template<class Default, template<class ...> class Op, class... Args>
> using detected_or = detail::detector<Default, void, Op, Args...>;
> 
> template<typename T, typename ...Ts>
> using foo_type = decltype(std::declval<T>().foo(std::declval<Ts>()...));
> 
> struct C1
> {
> };
> 
> struct C2
> {
> 	int foo(char) const;
> };
> 
> template<typename T>
> using has_foo_char = is_detected<foo_type, T, char>;
> 
> int main()
> {
> 	static_assert(!has_foo_char<C1>::value, "Unexpected");
> 	static_assert(has_foo_char<C2>::value, "Unexpected");
> 
> 	static_assert(std::is_same<int, detected_t<foo_type, C2, char>>::value,
> 					"Unexpected");
> 
> 	static_assert(std::is_same<void, // Default
> 					detected_or<void, foo_type, C1, char>>::value,
> 					"Unexpected");
> 	static_assert(std::is_same<int, detected_or<void, foo_type, C2, char>>::value,
> 					"Unexpected");
> }
> // g++ --std=c++17 test.cpp
> ```
>
> 
>
> C++11版本:
>
> ```C++
> #include <type_traits>
> #include <utility> // std::declval
> 
> #pragma once
> 
> namespace std_ext
> {
> template <typename ...Ts> struct make_void
> {
>     using type = void;
> };
> template <typename ...Ts> using void_t = typename make_void<Ts...>::type;
> } // namespace detail
> 
> namespace detail
> {
> template<class Default, class AlwaysVoid, template<class ...> class Op, class... Args>
> struct detector
> {
> 	using value_t = std::false_type;
> 	using type = Default;
> };
> 
> template<class Default, template<class ...> class Op, class... Args>
> struct detector<Default, std_ext::void_t<Op<Args...>>, Op, Args...>
> {
> 	using value_t = std::true_type;
> 	using type = Op<Args...>;
> };
> 
> }
>  // namespace detail
> 
> // special type to indicate detection failure
> struct nonesuch
> {
>     nonesuch() = delete;
>     ~nonesuch() = delete;
>     nonesuch(nonesuch const&) = delete;
>     void operator=(nonesuch const&) = delete;
> };
> 
> template<template<class ...> class Op, class... Args>
> using is_detected =
> typename detail::detector<nonesuch, void, Op, Args...>::value_t;
> 
> template<template<class ...> class Op, class... Args>
> using detected_t = typename detail::detector<nonesuch, void, Op, Args...>::type;
> 
> template<class Default, template<class ...> class Op, class... Args>
> using detected_or = detail::detector<Default, void, Op, Args...>;
> 
> template<typename T, typename ...Ts>
> using foo_type = decltype(std::declval<T>().foo(std::declval<Ts>()...));
> 
> struct C1
> {
> };
> 
> struct C2
> {
> 	int foo(char) const;
> };
> 
> template<typename T>
> using has_foo_char = is_detected<foo_type, T, char>;
> 
> int main()
> {
> 	static_assert(!has_foo_char<C1>::value, "Unexpected");
> 	static_assert(has_foo_char<C2>::value, "Unexpected");
> 
> 	static_assert(std::is_same<int, detected_t<foo_type, C2, char>>::value,
> 					"Unexpected");
> 
> 	static_assert(std::is_same<void, // Default
> 					detected_or<void, foo_type, C1, char>>::value,
> 					"Unexpected");
> 	static_assert(std::is_same<int, detected_or<void, foo_type, C2, char>>::value,
> 					"Unexpected");
> }
> // g++ --std=c++11 test.cpp
> ```
>
> 



### tartanllama [Detection Idiom - A Stopgap for Concepts](https://blog.tartanllama.xyz/detection-idiom/)
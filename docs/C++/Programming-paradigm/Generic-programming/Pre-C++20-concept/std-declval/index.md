# `std::declval`

1、`std::declval`是经常和`decltype`一起使用的。

2、当有object的时候，在expression SFINAE中，可以直接书写valid expression；当没有object，只有type的时候，在expression SFINAE中，需要使用`std::declval`

## boost [Declval](https://www.boost.org/doc/libs/1_54_0/libs/utility/doc/html/declval.html)

> NOTE: 这篇文章说明了C++11引入`std::decltype`的意图: 
>
> C++11引入了`declval`，从而允许**expression SFINAE**；为了便于书写expression，需要`std::declval`；
>
> 它们的组合，能够让我们在C++11中实现C++20 concept的部分功能。

### [Overview](https://www.boost.org/doc/libs/1_54_0/libs/utility/doc/html/declval.html#declval.overview)

The motivation for `declval` was introduced in [N2958: Moving Swap Forward](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2009/n2958.html#Value). Here follows a rewording of this chapter.

With the provision(规定) of `decltype`, **late-specified return types**, and **default template-arguments** for **function templates** a new generation of **SFINAE patterns** will emerge to at least partially compensate(补偿) the lack of **concepts** on the C++0x timescale. Using this technique, it is sometimes necessary to obtain an object of a known type in a non-using context, e.g. given the declaration

> NOTE: 上面这段话的总结是非常好的，它让明确说明了C++ 11 引入 `decltype`、`std::declval` 的意图，掌握**意图**是我们准确使用的前提:
>
> 1) 显然，`decltype`、`std::declval` 在一定程度上补偿了 C++0x 中 lack of concepts 的局限，显然在C++11中， 我们可以使用 `decltype`、`std::declval` 来实现 C++20 concept 的功能，从我目前的认知来看，detection idiom( 基于`decltype`、`std::declval`、`std::enalble_if`、`std::void_` ) + SFINAE能够实现C++20 concept一部分的功能。
>
> 2) "obtain an object of a known type in a non-using context, e.g. given the declaration" 意味着:  [unevaluated contexts](https://en.cppreference.com/w/cpp/language/expressions#Unevaluated_expressions) 。
>
> 3) "new generation of SFINAE pattern"指的是expression template，在expression template中，我们常常是需要"obtain an object of a known type in a non-using context"才能够完整的表达一个expression。

```C++
template<class T>
T&& declval(); // not used
```

as part of the function template declaration

```C++
template<class To, class From>
decltype(static_cast<To>(declval<From>())) convert(From&&);
```

or as part of a class template definition

```C++
template<class> class result_of;

template<class Fn, class... ArgTypes>
struct result_of<Fn(ArgTypes...)> 
{
  typedef decltype(declval<Fn>()(declval<ArgTypes>()...)) type;
};
```

The role of the function template `declval()` is a transformation of a type `T` into a **value** without **using** or **evaluating** this function. The name is supposed to direct the reader's attention to the fact that the expression `declval<T>()` is an **lvalue** if and only if `T` is an **lvalue-reference**, otherwise an **rvalue**. To extend the domain of this function we can do a bit better by changing its declaration to

```C++
template<class T>
typename std::add_rvalue_reference<T>::type declval(); // not used
```

which ensures that we can also use cv void as template parameter. The careful reader might have noticed that `declval()` already exists under the name `create()` as part of the definition of the semantics of the type trait `is_convertible` in the C++0x standard.

The provision(提供) of a new library component that allows the production of values in **unevaluated expressions** is considered important to realize constrained templates in C++0x where concepts are not available. This extremely light-weight function is expected to be part of the daily tool-box of the C++0x programmer.

> NOTE: 这段话总结的非常好

## cppreference [std::declval](https://en.cppreference.com/w/cpp/utility/declval)

```C++
template<class T>
typename std::add_rvalue_reference<T>::type declval() noexcept;
```



Converts any type `T` to a **reference type**, making it possible to use **member functions** in `decltype` expressions without the need to go through constructors.

> NOTE: 这段已经将 `std::declval` 的用途 总结得非常好了，结合下面的例子来看: `NonDefault` 是 没有 constructor的，通过 
>
> `decltype(std::declval<NonDefault>().foo()) n2 = n1;`
>
> 可以实现: "use **member functions** in `decltype` expressions without the need to go through constructors"。
>
> `std::declval` 是一个抽象层: 它屏蔽了 `T` 是否有 constructor 的细节，使programmer以 **generic** 的方式来 "use **member functions** in `decltype` expressions"，显然它是对generic programming的enhance。
>
> 

`declval` is commonly used in templates where acceptable template parameters may have no constructor in common, but have the same member function whose return type is needed.

Note that `declval` can only be used in [unevaluated contexts](https://en.cppreference.com/w/cpp/language/expressions#Unevaluated_expressions) and is not required to be defined; it is an error to evaluate an expression that contains this function. Formally, the program is ill-formed if this function is [odr-used](https://en.cppreference.com/w/cpp/language/definition#ODR-use).

> NOTE: `decltype`是  [unevaluated contexts](https://en.cppreference.com/w/cpp/language/expressions#Unevaluated_expressions) ，显然 [unevaluated contexts](https://en.cppreference.com/w/cpp/language/expressions#Unevaluated_expressions) 意味中 并不会有 object 会被创建。

```C++
#include <utility>
#include <iostream>

struct Default
{
	int foo() const
	{
		return 1;
	}
};

struct NonDefault
{
	NonDefault() = delete;
	int foo() const
	{
		return 1;
	}
};

int main()
{
	decltype(Default().foo()) n1 = 1;                   // type of n1 is int
//  decltype(NonDefault().foo()) n2 = n1;               // error: no default constructor
	decltype(std::declval<NonDefault>().foo()) n2 = n1; // type of n2 is int
	std::cout << "n1 = " << n1 << '\n' << "n2 = " << n2 << '\n';
}
// g++ --std=c++11 test.cpp

```



## Implementation

在 "**Ben's Blog** [C++ Detection Idiom Through the Years](https://people.eecs.berkeley.edu/~brock/blog/detection_idiom.php) # Pre-C++11 Detection Idiom:" 章节中，给出了一个样例实现:

```C++
template<typename T>
T declval();

template<typename T>
struct has_get
{
	typedef char yes[1];
	typedef char no[2];

	template<typename U>
	static yes& test_get(int (*)[sizeof(declval<U>().get(), 1)]);

	template<typename U>
	static no& test_get(...);

	static const bool value = sizeof(test_get<T>(NULL)) == sizeof(yes);
};

```


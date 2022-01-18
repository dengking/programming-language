# Ellipsis/catch-all operator

1、Ellipsis/catch-all operator是指：

```
...
```

在C++中，一般将它称为"ellipsis"。

2、本文总结它在C++中的用法

## cppreference [Other operators](https://en.cppreference.com/w/cpp/language/operator_other)

> NOTE:
>
> 1、其实谈及了`...`

## 用法1: Catch-all exception

stackoverflow [C++ catching all exceptions](https://stackoverflow.com/questions/315948/c-catching-all-exceptions)

[A](https://stackoverflow.com/a/315967)

```cpp
try{
    // ...
} catch (...) {
    // ...
}
```

will catch all C++ exceptions, but it should be considered bad design. You can use c++11's new [std::current_exception](https://en.cppreference.com/w/cpp/error/current_exception) mechanism, but if you don't have the ability to use c++11 (legacy code systems requiring a rewrite), then you have no named exception pointer to use to get a message or name. You may want to add separate catch clauses for the various exceptions you can catch, and only catch everything at the bottom to record an unexpected exception. E.g.:

```cpp
try{
    // ...
} catch (const std::exception& ex) {
    // ...
} catch (const std::string& ex) {
    // ...
} catch (...) {
    // ...
}
```

[A](https://stackoverflow.com/a/24142104)

```cpp
#include <iostream> // std::clog

#include <typeinfo>
#include <iostream>
#include <exception>      // std::exception_ptr, std::current_exception, std::rethrow_exception
#include <stdexcept>      // std::logic_error'

int main()
{
	try
	{
		throw std::logic_error("some logic_error exception"); // throw something
	}
	catch (...)
	{
		std::exception_ptr p = std::current_exception();
		std::clog << (p ? p.__cxa_exception_type()->name() : "null") << std::endl;
	}
	return 1;
}
// g++ --std=c++11 test.cpp
```

> NOTE:上述程序的输出：
>
> ```
> St11logic_error
> ```



## 用法2: Catch-all argument/ellipsis parameter/used as the catch-all fallbacks in [SFINAE](https://en.cppreference.com/w/cpp/language/sfinae)

这主要对应的是这种情况:

```c++
a(...)
```

是通过学习detection idiom发现了这种用法的。

### 标准

按照C++标准的说法，它叫做Variadic arguments，在cppreference的下列章节中对它进行了描述：

1、cppreference [Variadic arguments](https://en.cppreference.com/w/cpp/language/variadic_arguments)中对它进行了专门描述

Because variadic parameters have the lowest rank for the purpose of [overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution), they are commonly used as the catch-all fallbacks in [SFINAE](https://en.cppreference.com/w/cpp/language/sfinae).

2、cppreference Overload resolution`#`Viable functions中将它称为 **ellipsis parameter**

### Application: detection idiom



#### Example: wikipedia [Substitution failure is not an error](https://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error) 

> NOTE: 在这篇文章中对它有着比较好的说明:

For example, SFINAE can be used to determine if a type contains a certain **typedef**:

```C++
#include <iostream>

template<typename T>
struct has_typedef_foobar
{
	// Types "yes" and "no" are guaranteed to have different sizes,
	// specifically sizeof(yes) == 1 and sizeof(no) == 2.
	typedef char yes[1];
	typedef char no[2];

	template<typename C>
	static yes& test(typename C::foobar*);

	template<typename >
	static no& test(...);

	// If the "sizeof" of the result of calling test<T>(nullptr) is equal to
	// sizeof(yes), the first overload worked and T has a nested type named
	// foobar.
	static const bool value = sizeof(test<T>(nullptr)) == sizeof(yes);
};

struct foo
{
	typedef float foobar;
};

int main()
{
	std::cout << std::boolalpha;
	std::cout << has_typedef_foobar<int>::value << std::endl;  // Prints false
	std::cout << has_typedef_foobar<foo>::value << std::endl;  // Prints true
}
// g++ --std=c++11 test.cpp

```

When `T` has the nested type `foobar` defined, the instantiation of the first `test` works and the null pointer constant is successfully passed. (And the resulting type of the expression is `yes`.) If it does not work, the only available function is the second `test`, and the resulting type of the expression is `no`. **An ellipsis is used not only because it will accept any argument, but also because its conversion rank is lowest, so a call to the first function will be preferred if it is possible; this removes ambiguity.**

> NOTE: 这句话解释了在detection idiom中必须要使用 `test(...)`的原因。

#### Example: stackoverflow [What is “Expression SFINAE”?](https://stackoverflow.com/questions/12654067/what-is-expression-sfinae) `#` [A](https://stackoverflow.com/a/12654277)



```cpp
#include <iostream>

// catch-all case
void test(...)
{
	std::cout << "Couldn't call\n";
}

// catch when C is a reference-to-class type and F is a member function pointer
template<class C, class F>
auto test(C c, F f) -> decltype((c.*f)(), void()) // 'C' is reference type
{
	std::cout << "Could call on reference\n";
}

// catch when C is a pointer-to-class type and F is a member function pointer
template<class C, class F>
auto test(C c, F f) -> decltype((c->*f)(), void()) // 'C' is pointer type
{
	std::cout << "Could call on pointer\n";
}

struct X
{
	void f()
	{
	}
};

int main()
{
	X x;
	test(x, &X::f);
	test(&x, &X::f);
	test(42, 1337);
}
// g++ --std=c++11 test.cpp
```

> NOTE: 上述程序的输出如下:
>
> ```
> Could call on reference
> Could call on pointer
> Couldn't call
> ```

#### Example: stackoverflow [Templated check for the existence of a class member function?](https://stackoverflow.com/questions/257288/templated-check-for-the-existence-of-a-class-member-function) `#` [A](https://stackoverflow.com/a/257382)



## 用法3: Parameter pack

参见cppreference [Parameter pack(since C++11)](https://en.cppreference.com/w/cpp/language/parameter_pack)。
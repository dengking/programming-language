# `static_assert`



## cppreference [static_assert declaration](https://en.cppreference.com/w/cpp/language/static_assert)

Performs compile-time assertion checking



```C++
#include <type_traits>

template<class T>
void swap(T &a, T &b)
{
	static_assert(std::is_copy_constructible<T>::value,
					"Swap requires copying");
	static_assert(std::is_nothrow_copy_constructible<T>::value
					&& std::is_nothrow_copy_assignable<T>::value,
					"Swap requires nothrow copy/assign");
	auto c = b;
	b = a;
	a = c;
}

template<class T>
struct data_structure
{
	static_assert(std::is_default_constructible<T>::value,
					"Data structure requires default-constructible elements");
};

struct no_copy
{
	no_copy(const no_copy&) = delete;
	no_copy() = default;
};

struct no_default
{
	no_default() = delete;
};

int main()
{
	/**
	 * 可以校验通过
	 */
	int a, b;
	swap(a, b);
	/**
	 * 无法校验通过
	 */
	no_copy nc_a, nc_b;
	swap(nc_a, nc_b); // 1

	data_structure<int> ds_ok;
	data_structure<no_default> ds_error; // 2
}
// g++ --std=c++11 test.cpp

```

> NOTE: 上述程序，编译报错如下:
>
> ```C++
> test.cpp: In instantiation of ‘struct data_structure<no_default>’:
> test.cpp:43:29:   required from here
> test.cpp:19:2: 错误：static assertion failed: Data structure requires default-constructible elements
> static_assert(std::is_default_constructible<T>::value,
> ^
> test.cpp: In instantiation of ‘void swap(T&, T&) [with T = no_copy]’:
> test.cpp:40:17:   required from here
> test.cpp:6:2: 错误：static assertion failed: Swap requires copying
> static_assert(std::is_copy_constructible<T>::value,
> ^
> test.cpp:8:2: 错误：static assertion failed: Swap requires nothrow copy/assign
> static_assert(std::is_nothrow_copy_constructible<T>::value
> ```
>
> 



## CppCoreGuidelines [T.150: Check that a class matches a concept using `static_assert`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#t150-check-that-a-class-matches-a-concept-using-static_assert)

### Reason

If you intend for a class to match a concept, verifying that early saves users pain.



## Examples

1、[microsoft](https://github.com/microsoft)/**[GSL](https://github.com/microsoft/GSL)**/[pointers](https://github.com/microsoft/GSL/blob/main/include/gsl/pointers) 

```C++
template <class T>
class not_null
{
public:
    static_assert(details::is_comparable_to_nullptr<T>::value, "T cannot be compared to nullptr.");
};    
```



## stackoverflow [C++ templates that accept only certain types](https://stackoverflow.com/questions/874298/c-templates-that-accept-only-certain-types)

In Java you can define generic class that accept only types that extends class of your choice, eg:

```cpp
public class ObservableList<T extends List> {
  ...
}
```

This is done using "extends" keyword.

Is there some simple equivalent to this keyword in C++?

### [A](https://stackoverflow.com/a/874337)

> NOTE: 
>
> Boost's [static assert](http://www.boost.org/doc/libs/1_39_0/doc/html/boost_staticassert.html)，在C++11中已经包含[`static_assert`](https://en.cppreference.com/w/cpp/language/static_assert) ;
>
> Boost's [`is_base_of`](http://www.boost.org/doc/libs/1_39_0/libs/type_traits/doc/html/boost_typetraits/reference/is_base_of.html), 在C++11中已经包含;

I suggest using Boost's [static assert](http://www.boost.org/doc/libs/1_39_0/doc/html/boost_staticassert.html) feature in concert with [`is_base_of`](http://www.boost.org/doc/libs/1_39_0/libs/type_traits/doc/html/boost_typetraits/reference/is_base_of.html) from the Boost Type Traits library:

```cpp
template<typename T>
class ObservableList {
    BOOST_STATIC_ASSERT((is_base_of<List, T>::value)); //Yes, the double parentheses are needed, otherwise the comma will be seen as macro argument separator
    ...
};
```

In some other, simpler cases, you can simply forward-declare a global template, but only define (explicitly or partially specialise) it for the valid types:

```cpp
template<typename T> class my_template;
// Declare, but don't define

// int is a valid type
template<> class my_template<int>
{
	//...
};

// All pointer types are valid
template<typename T> class my_template<T*>
{
	//...
};

// All other types are invalid, and will cause linker error messages.
int main()
{
	my_template<int> i;
	my_template<int*> i2;
	my_template<char> i3;
}

```

> NOTE: 上述程序，编译报错如下:
>
> ```C++
> test.cpp:21:20: 错误：聚合‘my_template<char> i3’类型不完全，无法被定义
>   my_template<char> i3;
> 
> ```
>
> 

**[Minor EDIT 6/12/2013: Using a declared-but-not-defined template will result in \*linker\*, not compiler, error messages.]**





### [A](https://stackoverflow.com/a/25216349)

This typically is unwarranted(无根据的；无保证的) in C++, as other answers here have noted. In C++ we tend to define generic types based on other constraints other than "inherits from this class". If you really wanted to do that, it's quite easy to do in C++11 and `type_traits`:

> NOTE: generic type，基于inheritance和基于template

```cpp
#include <type_traits>

template<typename T>
class observable_list {
    static_assert(std::is_base_of<list, T>::value, "T must inherit from list");
    // code here..
};
```

This breaks a lot of the concepts that people expect in C++ though. It's better to use tricks like defining your own **traits**. For example, maybe `observable_list` wants to accept any type of container that has the typedefs `const_iterator` and a `begin` and `end` member function that returns `const_iterator`. If you restrict this to classes that inherit from `list` then a user who has their own type that doesn't inherit from `list` but provides these member functions and typedefs would be unable to use your `observable_list`.

> NOTE: 这段话对比了inheritance-based generic programming（Java generics）和behavior-based generic programming(C++ template、Python duck type)，显然相比于inheritance-based generic programming，基于behavior-based generic programming具备更多的优势，更加能够发挥出generic programming的优势。

There are two solutions to this issue, one of them is to not constrain anything and rely on duck typing. A big con(缺陷、缺点) to this solution is that it involves a massive amount of errors that can be hard for users to grok(凭直觉深刻了解、理解). Another solution is to define **traits** to constrain the type provided to meet the **interface requirements**. The big con(缺陷、缺点) for this solution is that involves extra writing which can be seen as annoying. However, the positive side is that you will be able to write your own error messages a la `static_assert`.

For completeness, the solution to the example above is given:

```cpp
#include <type_traits>

#include <list>
#include <vector>

template<typename ...>
struct void_
{
	using type = void;
};

template<typename ... Args>
using Void = typename void_<Args...>::type;

template<typename T, typename = void>
struct has_const_iterator: std::false_type
{
};

template<typename T>
struct has_const_iterator<T, Void<typename T::const_iterator>> : std::true_type
{
};

struct has_begin_end_impl
{
	template<typename T, typename Begin = decltype(std::declval<const T&>().begin()), typename End = decltype(std::declval<const T&>().end())>
	static std::true_type test(int);
	template<typename ...>
	static std::false_type test(...);
};

template<typename T>
struct has_begin_end: decltype(has_begin_end_impl::test<T>(0))
{
};

template<typename T>
class observable_list
{
	static_assert(has_const_iterator<T>::value, "Must have a const_iterator typedef");
	static_assert(has_begin_end<T>::value, "Must have begin and end member functions");
	// code here...
};
template<typename T>
class my_container
{

};
// All other types are invalid, and will cause linker error messages.
int main()
{
	observable_list<std::list<int>> o;
	observable_list<std::vector<int>> o2;
	observable_list<my_container<int>> o3;
}
// g++ --std=c++11 test.cpp

```

> NOTE: 上述程序编译报错如下:
>
> ```C++
> test.cpp: In instantiation of ‘class observable_list<my_container<int> >’:
> test.cpp:55:37:   required from here
> test.cpp:41:2: 错误：static assertion failed: Must have a const_iterator typedef
>   static_assert(has_const_iterator<T>::value, "Must have a const_iterator typedef");
>   ^
> test.cpp:42:2: 错误：static assertion failed: Must have begin and end member functions
>   static_assert(has_begin_end<T>::value, "Must have begin and end member functions");
> ```
>
> 

There are a lot of concepts shown in the example above that showcase C++11's features. Some search terms for the curious are **variadic templates**, SFINAE, **expression SFINAE**, and type traits.









## stackoverflow [How do I restrict a template class to certain built-in types?](https://stackoverflow.com/questions/16976720/how-do-i-restrict-a-template-class-to-certain-built-in-types)



### [A](https://stackoverflow.com/a/16977241)

> NOTE: `static_assert`比本回答中描述的方案是更好的，因为使用`static_assert`是支持打印出提示信息的



## stackoverflow [restrict a template function, to only allow certain types](https://stackoverflow.com/questions/32267324/restrict-a-template-function-to-only-allow-certain-types)
# std::forward

## cppreference [`std::forward`](https://en.cppreference.com/w/cpp/utility/forward) 

1) Forwards lvalues as either lvalues or as rvalues, depending on T

> NOTE: `std::forward`的入参是lvalue，它的返回值的类型是lvalue还是rvalue，取决于`T`，这就是perfect forwarding

```c++
#include <iostream>
#include <string>
#include <utility>

void foo(std::string && s)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	std::cout << s << std::endl;
}

void foo(std::string & s)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	std::cout << s << std::endl;
}

void foo(const std::string & s)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	std::cout << s << std::endl;
}

template<class T>
void wrapper(T&& arg)
{
	// arg is always lvalue
	foo(std::forward<T>(arg)); // Forward as lvalue or as rvalue, depending on T
}

int main()
{
	std::cout << "lvalue:" << std::endl;
	std::string s1 = "hello world";
	wrapper(s1);
	std::cout << "const lvalue:" << std::endl;
	const std::string s2 = "hello world";
	wrapper(s2);
	std::cout << "rvalue:" << std::endl;
	wrapper("hello world");

}
// g++ --std=c++11 test.cpp
```

> NOTE: 输出如下:
>
> ```c++
> lvalue:
> void foo(std::string&)
> hello world
> const lvalue:
> void foo(const string&)
> hello world
> rvalue:
> void foo(std::string&&)
> hello world
> ```
>
> 



2) Forwards rvalues as rvalues and prohibits forwarding of rvalues as lvalues

This overload makes it possible to forward a **result** of an expression (such as function call), which may be rvalue or lvalue, as the original value category of a forwarding reference argument.

> NOTE: 需要结合下面的例子来理解上面这段话：函数`foo`的入参的value category是由`T`决定的，而不是由**result** of an function call决定的；

```c++
#include <iostream>
#include <utility>
void foo(int& i)
{
	std::cout << __PRETTY_FUNCTION__ << " " << i << std::endl;
}

void foo(int&& i)
{
	std::cout << __PRETTY_FUNCTION__ << " " << i << std::endl;
}
// transforming wrapper
template<class T>
void wrapper(T&& arg)
{
	foo(std::forward<decltype(std::forward<T>(arg).get())>(std::forward<T>(arg).get()));
}

struct Arg
{
	int i = 1;
	int& get() &&
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return i;
	} // call to this overload is rvalue
	int& get() &
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return i;
	} // call to this overload is lvalue
};

int main()
{
	std::cout << "lvalue reference:" << std::endl;
	Arg a;
	wrapper(a);
	std::cout << "rvalue reference:" << std::endl;
	wrapper(Arg());
}

// g++ --std=c++11 test.cpp
```

> NOTE: 输出如下
>
> ```c++
> lvalue reference:
> int& Arg::get() &
> void foo(int&) 1
> rvalue reference:
> int& Arg::get() &&
> void foo(int&) 1
> ```



### Example

```c++
#include <iostream>
#include <memory>
#include <utility>

struct A
{
	A(int&& n)
	{
		std::cout << __PRETTY_FUNCTION__ << ", n=" << n << "\n";
	}
	A(int& n)
	{
		std::cout << __PRETTY_FUNCTION__ << ", n=" << n << "\n";
	}
};

class B
{
public:
	template<class T1, class T2, class T3>
	B(T1&& t1, T2&& t2, T3&& t3)
			:
					a1_ { std::forward<T1>(t1) },
					a2_ { std::forward<T2>(t2) },
					a3_ { std::forward<T3>(t3) }
	{
	}

private:
	A a1_, a2_, a3_;
};

template<class T, class U>
std::unique_ptr<T> make_unique1(U&& u)
{
	return std::unique_ptr<T>(new T(std::forward<U>(u)));
}

template<class T, class ... U>
std::unique_ptr<T> make_unique2(U&&... u)
{
	return std::unique_ptr<T>(new T(std::forward<U>(u)...));
}

int main()
{
	auto p1 = make_unique1<A>(2); // rvalue
	int i = 1;
	auto p2 = make_unique1<A>(i); // lvalue

	std::cout << "B\n";
	auto t = make_unique2<B>(2, i, 3);
}

// g++ --std=c++11 test.cpp
```

> NOTE: 输入如下:
>
> ```c++
> A::A(int&&), n=2
> A::A(int&), n=1
> B
> A::A(int&&), n=2
> A::A(int&), n=1
> A::A(int&&), n=3
> ```
>
> 

## Implementation

### libstdc++

[gcc](https://github.com/gcc-mirror/gcc)/[libstdc++-v3](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3)/[include](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include)/[bits](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include/bits)/[move.h](https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/move.h)

```c++
/**
 *  @brief  Forward an lvalue.
 *  @return The parameter cast to the specified type.
 *
 *  This function is used to implement "perfect forwarding".
 */
template<typename _Tp>
constexpr _Tp&&
forward(typename std::remove_reference<_Tp>::type& __t) noexcept
{
	return static_cast<_Tp&&>(__t);
}

/**
 *  @brief  Forward an rvalue.
 *  @return The parameter cast to the specified type.
 *
 *  This function is used to implement "perfect forwarding".
 */
template<typename _Tp>
constexpr _Tp&&
forward(typename std::remove_reference<_Tp>::type&& __t) noexcept
{
	static_assert(!std::is_lvalue_reference<_Tp>::value, "template argument"
			" substituting _Tp is an lvalue reference type");
	return static_cast<_Tp&&>(__t);
}

```

#### non-deducing context 

bajamircea [C++ std::move and std::forward](http://bajamircea.github.io/coding/cpp/2016/04/07/move-forward.html)中此有描述：

> The type `T` is not deduced, therefore we had to specify it when **using** `std::forward`.

thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c)中此有描述：

> Another thing I want to mention is the use of `std::remove_reference<T>`. In fact, if you think about it, `forward` could do without it. **Reference collapsing** does the job already, so `std::remove_reference<T>` is superfluous（多余的）. It's there to turn the `T& t` into a **non-deducing context** (according to the C++ standard, section 14.8.2.5), thus forcing us to explicitly specify the **template parameter** when calling `std::forward`.



关于 **non-deducing context** ，参见cppreference [Template argument deduction#Non-deduced contexts](https://en.cppreference.com/w/cpp/language/template_argument_deduction#Non-deduced_contexts)，显然`std::forward`符合第一条：

> everything to the left of the scope resolution operator `::`

#### Stop at compile time attempts to convert from an `rvalue` to an `lvalue` 

The `static_assert` is there to stop at compile time attempts to convert from an `rvalue` to an `lvalue` (that would have the dangling reference problem: a reference pointing to a temporary long gone). This is explained in more details in [N2835](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2009/n2835.html), but the gist is:

```c++
forward<const Y&>(Y()); // does not compile
// static assert in forward triggers compilation failure for line above
// with "invalid rvalue to lvalue conversion"
```



Some non-obvious properties of `std::forward` are that the return value can be more cv-qualified (i.e. can add a `const`). Also it allows for the case where the argument and return are different e.g. to forward expressions from **derived type** to it’s **base type** (even some scenarios where the base is derived from as `private`).



## `std::forward` does not **forward**

在bajamircea [C++ std::move and std::forward](http://bajamircea.github.io/coding/cpp/2016/04/07/move-forward.html)中，有这样的描述：

> C++ `std::move` does not **move** and `std::forward` does not **forward**.
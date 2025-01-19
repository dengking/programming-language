# C++11 forwarding reference

forwarding reference是function generic programming的基础:

1、`auto&&`解决的是函数的返回值

2、`template &&`解决的函数的参数

后面会对它们进行专门的介绍。

## cppreference [Reference declaration](https://en.cppreference.com/w/cpp/language/reference) # Forwarding references (since C++11)



**Forwarding references** are a special kind of references that preserve the value category of a function argument, making it possible to *forward* it by means of [std::forward](https://en.cppreference.com/w/cpp/utility/forward). Forwarding references are either:

### 1)

1) function parameter of a function template declared as **rvalue reference** to **cv-unqualified** [type template parameter](https://en.cppreference.com/w/cpp/language/template_parameters) of that same function template:

```c++
#include <utility> // std::forward
#include <iostream>

template<class T>
int g(const T&& x) // x is not a forwarding reference: const T is not cv-unqualified
{
	std::cout << __PRETTY_FUNCTION__ << " " << " " << x << std::endl;
}

template<class T>
int g(const T& x)
{
	std::cout << __PRETTY_FUNCTION__ << " " << " " << x << std::endl;
}

template<class T>
int f(T&& x)
{                    // x is a forwarding reference
	return g(std::forward<T>(x)); // and so can be forwarded
}

int main()
{
	int i = 100;
	f(i); // argument is lvalue, calls f<int&>(int&), std::forward<int&>(x) is lvalue
	f(0); // argument is rvalue, calls f<int>(int&&), std::forward<int>(x) is rvalue
}



// g++ --std=c++11 test.cpp
```

> NOTE: 输出如下:
>
> ```c++
> int g(const T&) [with T = int]  100
> int g(const T&&) [with T = int]  0
> ```



```c++
template<class T> struct A {
    template<class U>
    A(T&& x, U&& y, int* p); // x is not a forwarding reference: T is not a
                             // type template parameter of the constructor,
                             // but y is a forwarding reference
};
```

### 2)

2) `auto&&` except when deduced from a brace-enclosed initializer list:

```c++
#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <type_traits>
#include <vector> // std::vector

std::vector<int> foo()
{
	return
	{	1,2,3,4};
}
std::vector<int> f()
{
	return
	{	1,2,3,4};
}
template<class T>
int g(T&& x)
{                    // x is a forwarding reference
	return 0;
}

int main()
{
	auto&& vec = foo();       // foo() may be lvalue or rvalue, vec is a forwarding reference
	auto i = std::begin(vec); // works either way
	(*i)++;                   // works either way
	g(std::forward<decltype(vec)>(vec)); // forwards, preserving value category

	for (auto&& x : f())
	{
		// x is a forwarding reference; this is the safest way to use range for loops
	}

	auto&& z = { 1, 2, 3 }; // *not* a forwarding reference (special case for initializer lists)
}

// g++ --std=c++11 test.cpp
```

See also [template argument deduction](https://en.cppreference.com/w/cpp/language/template_argument_deduction#Deduction_from_a_function_call) and [std::forward](https://en.cppreference.com/w/cpp/utility/forward).


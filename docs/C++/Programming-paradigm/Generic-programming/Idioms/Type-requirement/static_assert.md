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
>   static_assert(std::is_default_constructible<T>::value,
>   ^
> test.cpp: In instantiation of ‘void swap(T&, T&) [with T = no_copy]’:
> test.cpp:40:17:   required from here
> test.cpp:6:2: 错误：static assertion failed: Swap requires copying
>   static_assert(std::is_copy_constructible<T>::value,
>   ^
> test.cpp:8:2: 错误：static assertion failed: Swap requires nothrow copy/assign
>   static_assert(std::is_nothrow_copy_constructible<T>::value
> ```
>
> 
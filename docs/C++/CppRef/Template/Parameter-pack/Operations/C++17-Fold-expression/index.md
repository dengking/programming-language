# Fold expression



## cppreference [fold expression](https://en.cppreference.com/w/cpp/language/fold)(since C++17)

> NOTE: 这是C++17中增强parameter pack的新特性。
>
> 它使用的是functional programming中的reduce: 对一个sequence执行一个函数；需要注意的是，这是在compile time执行的，即它是compile time function execution，显然它是C++ 对 compile time computation的增强，参见`C-and-C++\Compile-time-and-run-time\Compile-time-function-execution`章节。

Reduces ([folds](https://en.wikipedia.org/wiki/Fold_(higher-order_function))) a [parameter pack](parameter_pack.html) over a binary operator.



### Example: `all`

```c++
template<typename ... Args>
bool all(Args ... args)
{
	return (... && args);
}
int main()
{
	 // within all(), the unary left fold expands as
	 //  return ((true && true) && true) && false;
	 // b is false
	bool b = all(true, true, true, false);

}
// g++ --std=c++17
```



### Example: endianness swap

```C++
#include <cstddef> // std::size_t
#include <type_traits>
#include <utility>

// compile-time endianness swap based on http://stackoverflow.com/a/36937049
template<class T, std::size_t ... N>
constexpr T bswap_impl(T i, std::index_sequence<N...>)
{
	return (((i >> N*CHAR_BIT & std::uint8_t(-1)) << (sizeof(T)-1-N)*CHAR_BIT) | ...);
}
template<class T, class U = std::make_unsigned_t<T>>
constexpr U bswap(T i)
{
	return bswap_impl<U>(i, std::make_index_sequence<sizeof(T)> { });
}

int main()
{
    static_assert(bswap<std::uint16_t>(0x1234u)==0x3412u);
    static_assert(bswap<std::uint64_t>(0x0123456789abcdefULL)==0xefcdab8967452301ULL);
}
// g++ --std=c++17 test.cpp

```



### Example: `push_back`

```c++
#include <iostream>
#include <vector> // std::vector
#include <type_traits> // std::is_constructible_v
#include <utility> // std::forward

template<typename T, typename ... Args>
void push_back_vec(std::vector<T>& v, Args&&... args)
{
	static_assert((std::is_constructible_v<T, Args&&> && ...));
	(v.push_back(std::forward<Args>(args)), ...);
}

int main()
{
	std::vector<int> v;
	push_back_vec(v, 6, 2, 45, 12);
	push_back_vec(v, 1, 2, 9);
	for (int i : v)
		std::cout << i << ' ';
}
// g++ --std=c++17 test.cpp

```




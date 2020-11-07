# Parameter pack



## cppreference [Parameter pack(since C++11)](https://en.cppreference.com/w/cpp/language/parameter_pack)

A template parameter pack is a **template parameter** that accepts zero or more **template arguments** (non-types, types, or templates).

> NOTE: 需要注意的是:
>
> 1) 它的名称是**parameter** pack，它是parameter，这种语法是告诉compiler，这个parameter可以接受多个argument的，它是一个**pack**，这是template parameter pack的内涵。
>
> 2) 既然是parameter，它可以是:
>
> - [alias template](https://en.cppreference.com/w/cpp/language/type_alias)
> - [class template](https://en.cppreference.com/w/cpp/language/class_template)
> - [function template](https://en.cppreference.com/w/cpp/language/function_template) parameter lists
> - [variable template](https://en.cppreference.com/w/cpp/language/variable_template) 
>
> 
>
> 3) 既然有pack，那么肯定有对应的unpack过程，在原文中，将unpack过程称为“Parameter pack expansion”。
>
> 4) "parameter pack是linear structure"，之所以强调此的原因是: 我们应该将parameter pack看做是一个linear structure，这就让我们可以对它进行operation。
>
> - `len` 查询它的长度，参见[sizeof... operator](https://en.cppreference.com/w/cpp/language/sizeof...)
>
> - index 索引其中的某个元素
> - fold，参见[fold expression(since C++17)](https://en.cppreference.com/w/cpp/language/fold)
> - apply
>
> 可以按照functional programming来对它进行处理。
>
> 

A template with at least one parameter pack is called a *variadic template*.



### Syntax

Template parameter pack (appears in [alias template](https://en.cppreference.com/w/cpp/language/type_alias), [class template](https://en.cppreference.com/w/cpp/language/class_template), [variable template](https://en.cppreference.com/w/cpp/language/variable_template) and [function template](https://en.cppreference.com/w/cpp/language/function_template) parameter lists)

> NOTE: 在下面可以看到，上述三种类型的argument(non-types, types, or templates)的对应的template parameter pack的声明方式是不同的。

| Syntax                                                       | Explanation                                                  |      |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ---- |
| `type ... Args(optional)`                                    | A non-type template parameter pack with an optional name     |      |
| `typename|class ... Args(optional)`                          | A type template parameter pack with an optional name         |      |
| `template < parameter-list > typename | class ... Args(optional)` | A template template parameter pack with an optional name     |      |
| `Args ... args(optional)`                                    | A function parameter pack with an optional name              |      |
| `pattern ...`                                                | Parameter pack expansion:<br>expands to comma-separated list of zero or more `pattern`s. |      |

### Explanation

#### Variadic class template

```C++
template<class ... Types>
struct Tuple
{
};
int main()
{
	Tuple<> t0;           // Types contains no arguments
	Tuple<int> t1;        // Types contains one argument: int
	Tuple<int, float> t2; // Types contains two arguments: int and float
	// Tuple<0> error;       // error: 0 is not a type

}
```

#### Variadic function template 



```C++


template<class ... Types>
void f(Types ... args)
{

}
int main()
{
	f();       // OK: args contains no arguments
	f(1);       // OK: args contains one argument: int
	f(2, 1.0);       // OK: args contains two arguments: int and double
}

```



### Pack expansion

**Syntax**

```
Args ... args(optional)	(4)	
```



## cppreference `sizeof...` operator

Queries the number of elements in a [parameter pack](parameter_pack.html).



### Example

```c++
#include <iostream>
#include <array>
#include <type_traits>

template<typename ... Ts>
constexpr auto make_array(Ts&&... ts) -> std::array<std::common_type_t<Ts...>,sizeof...(ts)>
{
	return {	std::forward<Ts>(ts)...};
}

int main()
{
	auto b = make_array(1, 2, 3);
	std::cout << b.size() << '\n';
	for (auto i : b)
		std::cout << i << ' ';
}
// g++ --std=c++11 test.cpp
```



## cppreference [fold expression](https://en.cppreference.com/w/cpp/language/fold)(since C++17)

> NOTE: 这是C++17中增强parameter pack的新特性。
>
> 它使用的是functional programming中的apply: 对一个sequence执行一个函数；需要注意的是，这是在compile time执行的，显然它是C++ 对 compile time computation的增强。

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




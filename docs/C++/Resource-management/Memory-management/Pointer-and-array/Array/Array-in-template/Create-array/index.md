# create array



## Create static array at compile time

### stackoverflow [Create static array with variadic templates](https://stackoverflow.com/questions/6060103/create-static-array-with-variadic-templates)

```cpp
template <class T, T... args> 
struct array_
{
    static const T data[sizeof...(args)];
};

template <class T, T... args> 
const T array_<T, args...>::data[sizeof...(args)] = { args... };
```



[A](https://stackoverflow.com/a/6413430)

```C++
#include <iostream>

template<typename T>
class my_class
{
public:
	my_class(T)
	{
		//construct
	}

	void print_something()
	{
		std::cout << "something\n";
	}
};

template<class C, class T, T ... args>
struct array_
{
	static C data[sizeof...(args)];
};

template<class C, class T, T ... args>
C array_<C, T, args...>::data[sizeof...(args)] = { C(args)... };

int main()
{
	array_<my_class<int>, int, 1, 200, 0, 42>::data[2].print_something();
}
// g++ test.cpp --std=c++11 -pedantic -Wall -Wextra

```

> NOTE: 
>
> 1、`data`的类型: 元素类型为`C`的数组
>
> 2、 `C(args)...` 是parameter pack expansion



[A](https://stackoverflow.com/a/21690056)

In C++11 (and especially in C++14), the best way to initialize objects at compile-time is with `constexpr` constructors, not by playing metagames with the typesystem.

```cpp
struct MyObject {
    int x_, y_;
    constexpr MyObject(int x, int y) : x_(x), y_(y) { }
};

const MyObject array[] = { MyObject(1,2), MyObject(3,4) };
```

You can apply your "generator function" idea here, too, if you really want to:

```cpp
#include <stdio.h>

#if __cplusplus < 201400
template<size_t ... II> struct integer_sequence
{
	typedef integer_sequence type;
};
template<size_t N, size_t ... II> struct make_index_sequence;
template<size_t ... II> struct make_index_sequence<0, II...> : integer_sequence<II...>
{
};
template<size_t N, size_t ... II> struct make_index_sequence: make_index_sequence<N - 1, N - 1, II...>
{
};
#define HACK(x) typename x::type
#else
#include <utility>  // the C++14 way of doing things
using std::integer_sequence;
using std::make_index_sequence;
#define HACK(x) x
#endif

struct MyObject
{
	int x_, y_;
	constexpr MyObject(int x, int y) :
					x_(x), y_(y)
	{
	}
};

template<typename T, int N, T (*Func)(int), typename Indices>
struct GeneratedArrayHelper;

template<typename T, int N, T (*Func)(int), size_t ... i>
struct GeneratedArrayHelper<T, N, Func, integer_sequence<i...>>
{
	static const T data[N];  // element i is initialized with Func(i)
};

template<typename T, int N, T (*Func)(int), size_t ... i>
const T GeneratedArrayHelper<T, N, Func, integer_sequence<i...>>::data[N] = { Func(i)... };

template<typename T, int N, T (*Func)(int)>
struct GeneratedArray: GeneratedArrayHelper<T, N, Func, HACK(make_index_sequence<N>)>
{
};

constexpr MyObject newObject(int i)
{
	return MyObject(2 * i, 2 * i + 1);
}

int main()
{
	for (const MyObject &m : GeneratedArray<MyObject, 5, newObject>::data)
	{
		printf("%d %d\n", m.x_, m.y_);
	}

	// Output:
	//   0 1
	//   2 3
	//   4 5
	//   6 7
	//   8 9
}
// g++ --std=c++11 test.cpp -Wall -Wextra -pedantic

```

I don't know why Clang 3.5 and GCC 4.8 insist that I put the `HACK()` macro in there, but they refuse to compile the code without it. Probably I made some dumb mistake and someone can point it out. Also, I'm not confident that all the `const`s and `constexpr`s are in the best places.



## TODO

stackoverflow [static compile time table with floating point values](https://stackoverflow.com/questions/20387714/static-compile-time-table-with-floating-point-values)

stackoverflow [Programmatically create static arrays at compile time in C++](https://stackoverflow.com/questions/2978259/programmatically-create-static-arrays-at-compile-time-in-c)
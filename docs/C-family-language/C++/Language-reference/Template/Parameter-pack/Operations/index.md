# Operations

## Iteration

### C++11: recursion

**riptutorial [Iterating over a parameter pack](https://riptutorial.com/cplusplus/example/3208/iterating-over-a-parameter-pack)**

Often, we need to perform an operation over every element in a **variadic template parameter pack**. There are many ways to do this, and the solutions get easier to read and write with `C++17`. Suppose we simply want to print every element in a pack. The simplest solution is to recurse:

```C++
#include <iostream>

void print_all()
{
	// base case
	std::cout << std::endl;
}

template<class T, class ... Ts>
void print_all(T const &first, Ts const &... rest)
{
	std::cout << first << " ";

	print_all(rest...);
}

template<class ...Args>
void g(Args ...args)
{
	print_all(args...);
}
template<class ...Args>
void f(Args ... args)
{
//	auto lm = [&, args ...] {	return g(args...);};
//	lm();
	print_all(args...);
}
int main()
{
	f("hello", "world");
}
// g++ --std=c++11 test.cpp

```

> NOTE: 输出如下:
>
> ```C++
> hello world
> ```



### C++17: fold expression



## Examples

### `std::declval`

[C++ named requirements: *Callable*](https://en.cppreference.com/w/cpp/named_req/Callable): 

`INVOKE<R>(f, std::declval<ArgTypes>()...)`


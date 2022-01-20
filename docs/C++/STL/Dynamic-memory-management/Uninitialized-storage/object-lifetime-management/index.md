# object lifetime management



## destruction

### cppreference [std::destroy_at](https://en.cppreference.com/w/cpp/memory/destroy_at)

If `T` is not an array type, calls the destructor of the object pointed to by `p`, as if by `p->~T()`.

If `T` is an array type, 

1、(until C++20)the program is ill-formed 

2、(since C++20)recursively destroys elements of `*p` in order, as if by calling [std::destroy](http://en.cppreference.com/w/cpp/memory/destroy)([std::begin](http://en.cppreference.com/w/cpp/iterator/begin)(`*p`), [std::end](http://en.cppreference.com/w/cpp/iterator/end)(`*p`)) .

#### Example

```C++
#include <memory>
#include <new>
#include <iostream>

struct Tracer {
	int value;
	~Tracer() { std::cout << value << " destructed\n"; }
};

int main()
{
	alignas( Tracer ) unsigned char buffer[sizeof(Tracer) * 8];

	for (int i = 0; i < 8; ++i)
		new( buffer + sizeof(Tracer) * i ) Tracer{ i }; //manually construct objects

	auto ptr = std::launder(reinterpret_cast<Tracer*>( buffer ));

	for (int i = 0; i < 8; ++i)
		std::destroy_at(ptr + i);
}
// g++ test.cpp --std=c++17 -pedantic -Wall -Wextra -Werror

```

> NOTE: 
>
> 上面的例子中使用了 `std::launder`，关于它的解释可以参见:
>
> stackoverflow [What is the purpose of std::launder?](https://stackoverflow.com/questions/39382501/what-is-the-purpose-of-stdlaunder)

## construction

### cppreference [std::construct_at](https://en.cppreference.com/w/cpp/memory/construct_at)


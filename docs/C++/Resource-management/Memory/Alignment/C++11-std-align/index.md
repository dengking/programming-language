# `std::align`

一、其实就是实现了align to功能

## cppreference [std::align](https://en.cppreference.com/w/cpp/memory/align)

### Example



```C++
#include <iostream>
#include <memory>

template<std::size_t N>
struct MyAllocator
{
	char data[N]; // memory pool
	void *p;
	std::size_t sz; // memory pool的剩余空间的大小
	MyAllocator() :
					p(data), sz(N)
	{
	}
	template<typename T>
	T* aligned_alloc(std::size_t a = alignof(T))
	{
		if (std::align(a, sizeof(T), p, sz))
		{
			T *result = reinterpret_cast<T*>(p); // p指向分配的内存起始地址
			p = (char*) p + sizeof(T); // 更新p
			sz -= sizeof(T); // 更新sz
			return result;
		}
		return nullptr;
	}
};

int main()
{
	MyAllocator<64> a;

	// allocate a char
	char *p1 = a.aligned_alloc<char>();
	if (p1)
		*p1 = 'a';
	std::cout << "allocated a char at " << (void*) p1 << '\n';

	// allocate an int
	int *p2 = a.aligned_alloc<int>();
	if (p2)
		*p2 = 1;
	std::cout << "allocated an int at " << (void*) p2 << '\n';

	// allocate an int, aligned at 32-byte boundary
	int *p3 = a.aligned_alloc<int>(32);
	if (p3)
		*p3 = 2;
	std::cout << "allocated an int at " << (void*) p3 << " (32 byte alignment)\n";
}
// g++ test.cpp --std=c++11 -pedantic -Wall -Wextra


```

在我的环境中的运行结果如下:

```C++
allocated a char at 0x7fffd9d98800
allocated an int at 0x7fffd9d98804
allocated an int at 0x7fffd9d98820 (32 byte alignment)
```


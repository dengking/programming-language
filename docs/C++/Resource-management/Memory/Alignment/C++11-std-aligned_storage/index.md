# `std::aligned_storage`



## cppreference [std::aligned_storage](https://en.cppreference.com/w/cpp/types/aligned_storage)



### Possible implementation

Except for default argument, aligned_storage is expressible in terms of alignas:

```C++
template<std::size_t Len, std::size_t Align /* default alignment not implemented */>
struct aligned_storage
{
	struct type
	{
		alignas(Align) unsigned char data[Len];
	};
};

```

> NOTE: 
>
> 可以看到，本质上是一个static array

### Example

A primitive static vector class, demonstrating creation, access, and destruction of objects in aligned storage



```C++
#include <iostream>
#include <type_traits>
#include <string>

template<class T, std::size_t N>
class static_vector
{
	// properly aligned uninitialized storage for N T's
	typename std::aligned_storage<sizeof(T), alignof(T)>::type data[N];
	std::size_t m_size = 0;

public:
	// Create an object in aligned storage
	template<typename ...Args>
	void emplace_back(Args &&... args)
	{
		if (m_size >= N) // possible error handling
			throw std::bad_alloc { };

		// construct value in memory of aligned storage
		// using inplace operator new
		new (&data[m_size]) T(std::forward<Args>(args)...); // placement new
		++m_size;
	}

	// Access an object in aligned storage
	const T& operator[](std::size_t pos) const
	{
		// note: needs std::launder as of C++17
		return *reinterpret_cast<const T*>(&data[pos]);
	}

	// Delete objects from aligned storage
	~static_vector()
	{
		for (std::size_t pos = 0; pos < m_size; ++pos)
		{
			// note: needs std::launder as of C++17
			reinterpret_cast<T*>(&data[pos])->~T(); // 手工调用destructor
		}
	}
};

int main()
{
	static_vector<std::string, 10> v1;
	v1.emplace_back(5, '*');
	v1.emplace_back(10, '*');
	std::cout << v1[0] << '\n' << v1[1] << '\n';
}
// g++ test.cpp --std=c++11 -pedantic -Wall -Wextra

```

非常好的例子。

## aligned storage placement new

stackoverflow [Placement new in std::aligned_storage?](https://stackoverflow.com/questions/28187732/placement-new-in-stdaligned-storage)

## 素材

stackoverflow [Can std::byte replace std::aligned_storage?](https://stackoverflow.com/questions/58288225/can-stdbyte-replace-stdaligned-storage)


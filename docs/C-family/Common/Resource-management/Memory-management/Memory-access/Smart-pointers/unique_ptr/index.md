# `unique_ptr`



## cppreference [`std::unique_ptr`](https://en.cppreference.com/w/cpp/memory/unique_ptr) 



### [std::unique_ptr<T,Deleter>::release](https://en.cppreference.com/w/cpp/memory/unique_ptr/release) and [std::unique_ptr<T,Deleter>::reset](https://en.cppreference.com/w/cpp/memory/unique_ptr/reset)

> NOTE: 
>
> release: returns a pointer to the managed object and **releases the ownership**
>
> reset: Replaces the managed object.

> NOTE:  `unique_ptr` release and delete the owned object：
>
> ```c++
> u_ptr.reset(nullptr);
> ```
>
> 



## Implementation

> NOTE: 很多问题，当阅读它的实现的时候，就非常任意理解了。

### [gcc](https://github.com/gcc-mirror/gcc)/[libstdc++-v3](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3)/[include](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include)/[bits](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include/bits)/[unique_ptr.h](https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/unique_ptr.h)





## `std::unique_ptr<void>`

[Should std::unique_ptr be permitted](https://stackoverflow.com/questions/19840937/should-stdunique-ptrvoid-be-permitted)

https://stackoverflow.com/questions/39288891/why-is-shared-ptrvoid-legal-while-unique-ptrvoid-is-ill-formed





## std::unique_ptr return from function and test for null

[C++ std::unique_ptr return from function and test for null](https://stackoverflow.com/questions/30293338/c-stdunique-ptr-return-from-function-and-test-for-null)

[`std::unique_ptr<T,Deleter>::operator bool`](https://en.cppreference.com/w/cpp/memory/unique_ptr/operator_bool) 



## Exmaple

```c++
#include <iostream>
#include <memory> // for std::shared_ptr

class Resource
{
public:
	Resource()
	{
		std::cout << "Resource acquired\n";
	}
	~Resource()
	{
		std::cout << "Resource destroyed\n";
	}
};

class Test
{
public:
	Test()
	{
		InitR();
	}
	void InitR()
	{
		Resource* R = new Resource();
		m_R = std::unique_ptr<Resource>(R);
	}
private:
	std::unique_ptr<Resource> m_R;
};

int main()
{
	Test t;
}
```
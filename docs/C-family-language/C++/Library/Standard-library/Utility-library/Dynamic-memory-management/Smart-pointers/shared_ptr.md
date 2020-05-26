# `shared_ptr`

destroy of `shard_ptr`对象、destroy of resource。

## learncpp [std::shared_ptr](https://www.learncpp.com/cpp-tutorial/15-6-stdshared_ptr/)



## cppreference [`std::shared_ptr`](https://en.cppreference.com/w/cpp/memory/shared_ptr) 

A `shared_ptr` can share ownership of an object while storing a pointer to another object. This feature can be used to point to member objects while owning the object they belong to. The **stored pointer** is the one accessed by [get()](https://en.cppreference.com/w/cpp/memory/shared_ptr/get), the dereference and the comparison operators. The **managed pointer** is the one passed to the deleter when use count reaches zero.

> NOTE: stored pointer、managed pointer



## Implementation of `shard_ptr`

http://www.josuttis.com/libbook/cont/countptr.hpp.html



## Example

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


# Requiring or Prohibiting Heap-based Objects

"Requiring"的含义是"要求"，"Prohibiting"的含义是"禁止"。

## More C++ Idioms [Requiring or Prohibiting Heap-based Objects](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Requiring_or_Prohibiting_Heap-based_Objects)



### Solution and Sample Code

#### Requiring heap-based objects

> NOTE: 
>
> 1、"Requiring heap-based objects"意味着只能怪在heap上创建object，而不能够在stack上创建object

```c++
class HeapOnly
{
public:
	HeapOnly()
	{
	}
	void destroy() const
	{
		delete this;
	}
protected:
	~HeapOnly()
	{
	}
};

HeapOnly h1;     // Destructor is protected so h1 can't be created globally
HeapOnly func()  // Compiler error because destructor of temporary is protected
{
	HeapOnly *hoptr = new HeapOnly; // This is ok. No destructor is invoked automatically for heap-based objects
	return *hoptr;
}
int main(void)
{
	HeapOnly h2; // Destructor is protected so h2 can't be created on stack
}
// g++ test.cpp -pedantic -Wall -Wextra


```

**Protected destructor** also prevents access to delete `HeapOnly` because it internally invokes the destructor. To prevent memory leak, `destroy` member function is provided, which calls `delete` on itself. Derived classes have access to the protected destructor so `HeapOnly` class can still be used as a base class. However, the derived class no longer has the same restrictions.

> NOTE: 
>
> 1、通过上述描述来看，它是通过"**protected destructor**"的方式实现的

#### Prohibiting heap-based objects

Dynamic allocation of objects can be prevented by disallowing access to all forms of classspecific `new` operators. The `new` operator for scalar objects and for an array of objects are two possible variations. Both should be declared protected (or private) to prevent heapbased objects.

```c++
#include <cstddef>

class NoHeap
{
protected:
	static void* operator new(std::size_t);      // #1: To prevent allocation of scalar objects
	static void* operator new [](std::size_t);  // #2: To prevent allocation of array of objects
};
class NoHeapTwo: public NoHeap
{
};
int main(void)
{
	new NoHeap;        // Not allowed because of #1
	new NoHeap[1];     // Not allowed because of #2
	new NoHeapTwo[10];  // Not allowed because of inherited protected new operator (#2).
}
// g++ test.cpp -pedantic -Wall -Wextra


```





## csdn [C++ 如何限制一个类对象只在栈（堆）上创建](https://blog.csdn.net/u011857683/article/details/81837879)



### 只能在堆上分配类对象(heap only)

```C++
class A
{

protected:
	A()
	{
	}
	~A()
	{
	}

public:
	static A* create()
	{
		return new A();
	}

	void destory()
	{
		delete this;
	}
};
// g++ test.cpp -pedantic -Wall -Wextra


```


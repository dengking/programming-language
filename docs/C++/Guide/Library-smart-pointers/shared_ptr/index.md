# `shared_ptr`

destroy of `shard_ptr`对象、destroy of resource。

## 术语

### Managed Object

由shared pointer 管理的object，这个名称源自:

1、nextptr [shared_ptr - basics and internals with examples](https://www.nextptr.com/tutorial/ta1358374985/shared_ptr-basics-and-internals-with-examples)。

### Managed pointer

指向managed object的pointer，这个名称源自:

1、cppreference [`std::shared_ptr`](https://en.cppreference.com/w/cpp/memory/shared_ptr) 

### Stored pointer

`shared_ptr` object的pointer；

## cppreference [`std::shared_ptr`](https://en.cppreference.com/w/cpp/memory/shared_ptr) 

`std::shared_ptr` is a smart pointer that retains(保持) shared ownership of an object through a pointer. Several `shared_ptr` objects may own the same object. The object is destroyed and its memory deallocated when either of the following happens:

1、the last remaining `shared_ptr` owning the object is destroyed;

2、the last remaining `shared_ptr` owning the object is assigned another pointer via [operator=](https://en.cppreference.com/w/cpp/memory/shared_ptr/operator%3D) or [reset()](https://en.cppreference.com/w/cpp/memory/shared_ptr/reset).

### Share ownership of an object while storing a pointer to another object

A `shared_ptr` can share ownership of an object while storing a pointer to another object. This feature can be used to point to member objects while owning the object they belong to. The **stored pointer** is the one accessed by [get()](https://en.cppreference.com/w/cpp/memory/shared_ptr/get), the dereference and the comparison operators. The **managed pointer** is the one passed to the deleter when use count reaches zero.

> NOTE: 
>
> 1、在 nextptr [shared_ptr - basics and internals with examples](https://www.nextptr.com/tutorial/ta1358374985/shared_ptr-basics-and-internals-with-examples) 中，列举了这样的例子；
>
> 2、这其实就是 **aliasing constructor**

### Empty status

A `shared_ptr` may also own no objects, in which case it is called *empty* (an empty `shared_ptr` may have a non-null stored pointer if the **aliasing constructor** was used to create it).

> NOTE: 没有搞懂

### Data race

All member functions (including copy constructor and copy assignment) can be called by multiple threads on different instances of `shared_ptr` without additional synchronization even if these instances are copies and share ownership of the same object. 

> NOTE: 对于具备shared ownership的所有`shared_ptr`，它们是共享同一个control block；通过上面这段话可知: `shared_ptr`的实现能够保证: **不同**的thread同时使用**不同**的`shared_ptr` object，control block是thread safe的；



If multiple threads of execution access the same `shared_ptr` without synchronization and any of those accesses uses a non-const member function of `shared_ptr` then a data race will occur; the [`shared_ptr` overloads of atomic functions](https://en.cppreference.com/w/cpp/memory/shared_ptr/atomic) can be used to prevent the data race.

> NOTE: 上面这段话所描述的是: 多个thread同时使用同一个`shared_ptr` object;
>
> 我觉得 需要考虑两层thread safe:
>
> 1、`shared_ptr` object的thread safe
>
> 2、target object的thread safe
>
> 没有理解上面这段话的含义；

### Example

#### Subtyping polymorphism

下面这个例子展示了`std::shared_ptr` + subtyping polymorphism:

```C++
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>

struct Base
{
	Base()
	{
		std::cout << "  Base::Base()\n";
	}
	// Note: non-virtual destructor is OK here
	~Base()
	{
		std::cout << "  Base::~Base()\n";
	}
};

struct Derived: public Base
{
	Derived()
	{
		std::cout << "  Derived::Derived()\n";
	}
	~Derived()
	{
		std::cout << "  Derived::~Derived()\n";
	}
};

void thr(std::shared_ptr<Base> p)
{
	std::this_thread::sleep_for(std::chrono::seconds(3));
	std::shared_ptr<Base> lp = p; // thread-safe, even though the
								  // shared use_count is incremented
	{
		static std::mutex io_mutex;
		std::lock_guard<std::mutex> lk(io_mutex);
		std::cout << "local pointer in a thread:\n" << "  lp.get() = " << lp.get() << ", lp.use_count() = " << lp.use_count() << '\n';
	}
}

int main()
{
	std::shared_ptr<Base> p = std::make_shared<Derived>();

	std::cout << "Created a shared Derived (as a pointer to Base)\n" << "  p.get() = " << p.get() << ", p.use_count() = " << p.use_count() << '\n';
	std::thread t1(thr, p), t2(thr, p), t3(thr, p);
	p.reset(); // release ownership from main
	std::cout << "Shared ownership between 3 threads and released\n" << "ownership from main:\n" << "  p.get() = " << p.get() << ", p.use_count() = " << p.use_count() << '\n';
	t1.join();
	t2.join();
	t3.join();
	std::cout << "All threads completed, the last one deleted Derived\n";
}
// g++ --std=c++11 test.cpp

```



## 如何维护shared ownership?

对于shared ownership，可以acquire、release；

### Acquire shared ownership

本节标题的含义是: 获取对某个object/resource的shared ownership，在下面的文章中，涉及了这个topic: 

#### cppreference [std::shared_ptr # Notes](https://en.cppreference.com/w/cpp/memory/shared_ptr)

The ownership of an object can only be shared with another `shared_ptr` by 

1、copy constructing or 

2、copy assigning its value to another `shared_ptr`. 

Constructing a new `shared_ptr` using the raw underlying pointer owned by another `shared_ptr` leads to undefined behavior.

> NOTE: 关于这段话中描述的undefined behavior的场景，参见:
>
> 1、learncpp [std::shared_ptr](https://www.learncpp.com/cpp-tutorial/15-6-stdshared_ptr/)，在 `Guide\learncpp-std-shared_ptr` 中收录了这篇文章
>
> 

####  `enable_shared_from_this` 和 `shared_from_this`

这也是实现"Acquire shared ownership"的一种方式，参见 `./shared_ptr-and-this`。

### Release shared ownership

本节标题的含义是: 释放对某个object/resource的shared ownership。

#### cppreference [shared_ptr::reset](https://en.cppreference.com/w/cpp/memory/shared_ptr/reset)

如何release shared ownership？在 "cppreference [std::shared_ptr # Notes](https://en.cppreference.com/w/cpp/memory/shared_ptr) # Example "中展示的例子，演示了用法；
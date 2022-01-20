# `std::shared_ptr` and `this`



## stackoverflow [std::shared_ptr of this](https://stackoverflow.com/questions/11711034/stdshared-ptr-of-this)

```C++
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>
#include <list>

class A;
class B;

class A
{
public:
	void addChild(std::shared_ptr<B> child)
	{
		children.push_back(child);

		// How to do pass the pointer correctly?
		// child->setParent(this);  // wrong
		//                  ^^^^
	}

private:
	std::list<std::shared_ptr<B>> children;
};

class B
{
public:
	void setParent(std::shared_ptr<A> parent)
	{
		this->parent = parent;
	}

private:
	std::shared_ptr<A> parent;
};

int main()
{
}
// g++ --std=c++11 test.cpp

```

The question is how can an object of class A pass a `std::shared_ptr` of itself (`this`) to its child?

There are solutions for Boost shared pointers ([Getting a `boost::shared_ptr` for `this`](https://stackoverflow.com/questions/142391/getting-a-boostshared-ptr-for-this)), but how to handle this using the `std::` smart pointers?

### [A](https://stackoverflow.com/a/11711094)

There is [`std::enable_shared_from_this`](http://en.cppreference.com/w/cpp/memory/enable_shared_from_this) just for this purpose. You inherit from it and you can call [`.shared_from_this()`](http://en.cppreference.com/w/cpp/memory/enable_shared_from_this/shared_from_this) from inside the class. Also, you are creating circular dependencies here that can lead to resource leaks. That can be resolved with the use of [`std::weak_ptr`](http://en.cppreference.com/w/cpp/memory/weak_ptr). So your code might look like this (assuming children rely on existence of parent and not the other way around):

```cpp
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>
#include <list>

class A;
class B;

class A: public std::enable_shared_from_this<A>
{
public:
	void addChild(std::shared_ptr<B> child)
	{
		children.push_back(child);

		// like this
		child->setParent(shared_from_this());  // ok
		//               ^^^^^^^^^^^^^^^^^^
	}

private:
	// note weak_ptr
	std::list<std::weak_ptr<B>> children;
	//             ^^^^^^^^
};

class B
{
public:
	void setParent(std::shared_ptr<A> parent)
	{
		this->parent = parent;
	}

private:
	std::shared_ptr<A> parent;
};

int main()
{
}
// g++ --std=c++11 test.cpp

```

Note however, that calling `.shared_from_this()` requires that `this` is owned by `std::shared_ptr` at the point of call. This means that you cannot create such object on stack anymore, and *generally* cannot call `.shared_from_this()` from within a constructor or destructor.

> NOTE: 参见: Requiring-or-Prohibiting-Heap-based-Objects idiom

## stackoverflow [Getting a boost::shared_ptr for this](https://stackoverflow.com/questions/142391/getting-a-boostshared-ptr-for-this)


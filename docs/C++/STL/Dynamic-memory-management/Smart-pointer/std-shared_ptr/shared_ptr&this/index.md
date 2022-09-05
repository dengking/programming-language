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

> NOTE:
>
> 一、需要注意的是: 上述代码是无法编译通过的，因为 `A`  和 `B` 之间存在compilation dependency，借鉴 cppreference [PImpl](https://en.cppreference.com/w/cpp/language/pimpl) 中的修改方式，可以编译通过的代码如下:
>
> `test.h` :
>
> ```C++
> #include <iostream>
> #include <memory>
> #include <thread>
> #include <chrono>
> #include <mutex>
> #include <list>
> 
> class A;
> class B;
> 
> class A : public std::enable_shared_from_this<A>
> {
> public:
>     A() = default;
>     ~A()
>     {
>         std::cout << "~A()" << std::endl;
>     }
>     void addChild(std::shared_ptr<B> child);
> 
> private:
>     // note weak_ptr
>     std::list<std::weak_ptr<B>> children;
>     //             ^^^^^^^^
> };
> 
> class B
> {
> public:
>     void setParent(std::shared_ptr<A> parent);
>     ~B()
>     {
>         std::cout << "~B()" << std::endl;
>     }
> 
> private:
>     std::shared_ptr<A> parent;
> };
> 
> ```
>
> 
>
> `test.cpp`
>
> ```c++
> #include "test2.h"
> #include <memory>
> #include <thread>
> #include <chrono>
> #include <mutex>
> #include <list>
> 
> void A::addChild(std::shared_ptr<B> child)
> {
>     children.push_back(child);
> 
>     // like this
>     child->setParent(shared_from_this()); // ok
>     //               ^^^^^^^^^^^^^^^^^^
> }
> 
> void B::setParent(std::shared_ptr<A> parent)
> {
>     this->parent = parent;
> }
> 
> int main()
> {
>     auto parent = std::make_shared<A>();
>     auto child = std::make_shared<B>();
>     parent->addChild(child);
> }
> 
> // g++ --std=c++11 test.cpp
> 
> ```
>
> 

Note however, that calling `.shared_from_this()` requires that `this` is owned by `std::shared_ptr` at the point of call. This means that you cannot create such object on stack anymore, and *generally* cannot call `.shared_from_this()` from within a constructor or destructor.

> NOTE: 
>
> 一、参见: Requiring-or-Prohibiting-Heap-based-Objects idiom

## stackoverflow [Getting a boost::shared_ptr for this](https://stackoverflow.com/questions/142391/getting-a-boostshared-ptr-for-this)



### [A](https://stackoverflow.com/a/142401/10173843)

You can derive from [enable_shared_from_this](http://www.boost.org/doc/libs/1_36_0/libs/smart_ptr/enable_shared_from_this.html) and then you can use "shared_from_this()" instead of "this" to spawn a shared pointer to your own self object.

Example in the link:

```cpp
#include <boost/enable_shared_from_this.hpp>

class Y: public boost::enable_shared_from_this<Y>
{
public:

    shared_ptr<Y> f()
    {
        return shared_from_this();
    }
}

int main()
{
    shared_ptr<Y> p(new Y);
    shared_ptr<Y> q = p->f();
    assert(p == q);
    assert(!(p < q || q < p)); // p and q must share ownership
}
```

It's a good idea when spawning threads from a member function to boost::bind to a shared_from_this() instead of this. It will ensure that the object is not released.





## `std::enable_shared_from_this`

在阅读 [gabime](https://github.com/gabime)/**[spdlog](https://github.com/gabime/spdlog)** 的 [async_logger.h](https://github.com/gabime/spdlog/blob/v1.x/include/spdlog/async_logger.h) 时，其中使用了 `std::enable_shared_from_this`:

```C++
class SPDLOG_API async_logger final : public std::enable_shared_from_this<async_logger>, public logger
{

};

```

### CRTP mixin from above

`std::enable_shared_from_this`是典型的使用CRTP的，它是mixin from above。



### stackoverflow [What is the usefulness of `enable_shared_from_this`?](https://stackoverflow.com/questions/712279/what-is-the-usefulness-of-enable-shared-from-this)

> NOTE: 
>
> 一、在读完了这篇文章后，我理解了
>
> 1、`enable_shared_from_this`的作用: `shared_from_this`，其实`shared_from_this`是一个factory method，它允许不使用copy constructor也能够正确的构建一个shared pointer，且这个shared pointer是有效的；
>
> 2、使用shared pointer的关键是所有的需要从同一个copy，这样它们才能够有相同的reference count；

I ran across `enable_shared_from_this` while reading the Boost.Asio examples and after reading the documentation I am still lost for how this should correctly be used. Can someone please give me an example and explanation of when using this class makes sense.

#### [A](https://stackoverflow.com/a/712307)

It enables you to get a valid `shared_ptr` instance to `this`, when all you have is `this`. Without it, you would have no way of getting a `shared_ptr` to `this`, unless you already had one as a member. This example from the [boost documentation for enable_shared_from_this](http://www.boost.org/doc/libs/1_38_0/libs/smart_ptr/enable_shared_from_this.html):

```c++
class Y: public enable_shared_from_this<Y>
{
public:

    shared_ptr<Y> f()
    {
        return shared_from_this();
    }
}

int main()
{
    shared_ptr<Y> p(new Y);
    shared_ptr<Y> q = p->f();
    assert(p == q);
    assert(!(p < q || q < p)); // p and q must share ownership
}
```

The method `f()` returns a valid `shared_ptr`, even though it had no member instance. Note that you cannot simply do this:

```c++
class Y: public enable_shared_from_this<Y>
{
public:

    shared_ptr<Y> f()
    {
        return shared_ptr<Y>(this);
    }
}
```

The shared pointer that this returned will have a different reference count from the "proper" one, and one of them will end up losing and holding a dangling reference when the object is deleted.

`enable_shared_from_this` has become part of C++ 11 standard. You can also get it from there as well as from boost.

#### [A](https://stackoverflow.com/a/5548314)

from Dr Dobbs article on weak pointers, I think this example is easier to understand (source: http://drdobbs.com/cpp/184402026):

...code like this won't work correctly:

```c++
int *ip = new int;
shared_ptr<int> sp1(ip);
shared_ptr<int> sp2(ip);
```

Neither of the two `shared_ptr` objects knows about the other, so both will try to release the resource when they are destroyed. That usually leads to problems.

Similarly, if a member function needs a `shared_ptr` object that owns the object that it's being called on, it can't just create an object on the fly:

```c++
struct S
{
  shared_ptr<S> dangerous()
  {
     return shared_ptr<S>(this);   // don't do this!
  }
};

int main()
{
   shared_ptr<S> sp1(new S);
   shared_ptr<S> sp2 = sp1->dangerous();
   return 0;
}
```

This code has the same problem as the earlier example, although in a more subtle form. When it is constructed, the `shared_pt`r object `sp1` owns the newly allocated resource. The code inside the member function `S::dangerous` doesn't know about that `shared_ptr` object, so the `shared_ptr` object that it returns is distinct from `sp1`. Copying the new `shared_ptr` object to `sp2` doesn't help; when `sp2` goes out of scope, it will release the resource, and when `sp1` goes out of scope, it will release the resource again.

The way to avoid this problem is to use the class template `enable_shared_from_this`. The template takes one template type argument, which is the name of the class that defines the managed resource. That class must, in turn, be derived publicly from the template; like this:

```c++
struct S : enable_shared_from_this<S>
{
  shared_ptr<S> not_dangerous()
  {
    return shared_from_this();
  }
};

int main()
{
   shared_ptr<S> sp1(new S);
   shared_ptr<S> sp2 = sp1->not_dangerous();
   return 0;
}
```

When you do this, keep in mind that the object on which you call `shared_from_this` must be owned by a `shared_ptr` object. This won't work:

```c++
int main()
{
   S *p = new S;
   shared_ptr<S> sp2 = p->not_dangerous();     // don't do this
}
```



### cppreference [std::enable_shared_from_this](https://en.cppreference.com/w/cpp/memory/enable_shared_from_this)



#### Example

```c++
#include <memory>
#include <iostream>

struct Good: std::enable_shared_from_this<Good> // note: public inheritance
{
    std::shared_ptr<Good> getptr() {
        return shared_from_this();
    }
};

```


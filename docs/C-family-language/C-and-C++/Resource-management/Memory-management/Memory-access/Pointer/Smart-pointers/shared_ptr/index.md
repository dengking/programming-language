# `shared_ptr`

destroy of `shard_ptr`对象、destroy of resource。

## learncpp [std::shared_ptr](https://www.learncpp.com/cpp-tutorial/15-6-stdshared_ptr/)

> NOTE: 下面是一个正确使用的例子

```c++
#include <iostream>
#include <memory> // for std::shared_ptr
 
class Resource
{
public:
	Resource() { std::cout << "Resource acquired\n"; }
	~Resource() { std::cout << "Resource destroyed\n"; }
};
 
int main()
{
	// allocate a Resource object and have it owned by std::shared_ptr
	Resource *res = new Resource;
	std::shared_ptr<Resource> ptr1(res);
	{
		std::shared_ptr<Resource> ptr2(ptr1); // use copy initialization to make another std::shared_ptr pointing to the same thing
 
		std::cout << "Killing one shared pointer\n";
	} // ptr2 goes out of scope here, but nothing happens
 
	std::cout << "Killing another shared pointer\n";
 
	return 0;
} // ptr1 goes out of scope here, and the allocated Resource is destroyed


// 编译: g++ --std=c++11 test.cpp 
```

> NOTE: 下面是一个错误使用的例子

```c++
#include <iostream>
#include <memory> // for std::shared_ptr
 
class Resource
{
public:
	Resource() { std::cout << "Resource acquired\n"; }
	~Resource() { std::cout << "Resource destroyed\n"; }
};
 
int main()
{
	Resource *res = new Resource;
	std::shared_ptr<Resource> ptr1(res);
	{
		std::shared_ptr<Resource> ptr2(res); // create ptr2 directly from res (instead of ptr1)
 
		std::cout << "Killing one shared pointer\n";
	} // ptr2 goes out of scope here, and the allocated Resource is destroyed
 
	std::cout << "Killing another shared pointer\n";
 
	return 0;
} // ptr1 goes out of scope here, and the allocated Resource is destroyed again
```

and then crashes (at least on the author’s machine).

The difference here is that we created two std::shared_ptr independently from each other. As a consequence, even though they’re both pointing to the same Resource, they aren’t aware of each other. When ptr2 goes out of scope, it thinks it’s the only owner of the Resource, and deallocates it. When ptr1 later goes out of the scope, it thinks the same thing, and tries to delete the Resource again. Then bad things happen.





## cppreference [`std::shared_ptr`](https://en.cppreference.com/w/cpp/memory/shared_ptr) 

A `shared_ptr` can share ownership of an object while storing a pointer to another object. This feature can be used to point to member objects while owning the object they belong to. The **stored pointer** is the one accessed by [get()](https://en.cppreference.com/w/cpp/memory/shared_ptr/get), the dereference and the comparison operators. The **managed pointer** is the one passed to the deleter when use count reaches zero.

> NOTE: stored pointer、managed pointer



## Implementation of `shard_ptr`

### 理论

参见维基百科[Garbage collection](https://en.wikipedia.org/wiki/Garbage_collection_(computer_science)) 

- [Reference counting](https://en.wikipedia.org/wiki/Reference_counting)

learncpp [std::shared_ptr](https://www.learncpp.com/cpp-tutorial/15-6-stdshared_ptr/)：

> Unlike std::unique_ptr, which uses a single pointer internally, std::shared_ptr uses two pointers internally. One pointer points at the resource being managed. The other points at a “control block”, which is a dynamically allocated object that tracks of a bunch of stuff, including how many std::shared_ptr are pointing at the resource. When a std::shared_ptr is created via a std::shared_ptr constructor, the memory for the managed object (which is usually passed in) and control block (which the constructor creates) are allocated separately. However, when using std::make_shared(), this can be optimized into a single memory allocation, which leads to better performance.
>
> This also explains why independently creating two std::shared_ptr pointed to the same resource gets us into trouble. Each std::shared_ptr will have one pointer pointing at the resource. However, each std::shared_ptr will independently allocate its own control block, which will indicate that it is the only pointer owning that resource. Thus, when that std::shared_ptr goes out of scope, it will deallocate the resource, not realizing there are other std::shared_ptr also trying to manage that resource.
>
> However, when a std::shared_ptr is cloned using copy assignment, the data in the control block can be appropriately updated to indicate that there are now additional std::shared_ptr co-managing the resource.



### Implementation 1

http://www.josuttis.com/libbook/cont/countptr.hpp.html

```c++
#ifndef COUNTED_PTR_HPP
#define COUNTED_PTR_HPP

/* class for counted reference semantics
 * - deletes the object to which it refers when the last CountedPtr
 *   that refers to it is destroyed
 */
template <class T>
class CountedPtr {
  private:
    T* ptr;        // pointer to the value
    long* count;   // shared number of owners

  public:
    // initialize pointer with existing pointer
    // - requires that the pointer p is a return value of new
    explicit CountedPtr (T* p=0)
     : ptr(p), count(new long(1)) {
    }

    // copy pointer (one more owner)
    CountedPtr (const CountedPtr<T>& p) throw()
     : ptr(p.ptr), count(p.count) {
        ++*count;
    }

    // destructor (delete value if this was the last owner)
    ~CountedPtr () throw() {
        dispose();
    }

    // assignment (unshare old and share new value)
    CountedPtr<T>& operator= (const CountedPtr<T>& p) throw() {
        if (this != &p) {
            dispose();
            ptr = p.ptr;
            count = p.count;
            ++*count;
        }
        return *this;
    }

    // access the value to which the pointer refers
    T& operator*() const throw() {
        return *ptr;
    }
    T* operator->() const throw() {
        return ptr;
    }

  private:
    void dispose() {
        if (--*count == 0) {
             delete count;
             delete ptr;
        }
    }
    
};

#endif /*COUNTED_PTR_HPP*/
```

### Implementation 2

[Boost.SmartPtr: The Smart Pointer Library](https://www.boost.org/doc/libs/1_72_0/libs/smart_ptr/doc/html/smart_ptr.html)

## [std::make_shared](https://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared)

`make_shard`使用的是Object-Generator idiom，参见`C++\Idiom\Template-metaprogramming\Object-Generator`。

这个例子源自[learncpp 15.6 — std::shared_ptr](https://www.learncpp.com/cpp-tutorial/15-6-stdshared_ptr/)：

```c++
#include <iostream>
#include <memory> // for std::shared_ptr
 
class Resource
{
public:
	Resource() { std::cout << "Resource acquired\n"; }
	~Resource() { std::cout << "Resource destroyed\n"; }
};
 
int main()
{
	// allocate a Resource object and have it owned by std::shared_ptr
	auto ptr1 = std::make_shared<Resource>();
	{
		auto ptr2 = ptr1; // create ptr2 using copy initialization of ptr1
 
		std::cout << "Killing one shared pointer\n";
	} // ptr2 goes out of scope here, but nothing happens
 
	std::cout << "Killing another shared pointer\n";
 
	return 0;
} // ptr1 goes out of scope here, and the allocated Resource is destroyed
```



## `shard_ptr` and multiple-thread

### Multiple-thread share a pointer and [Dangling pointer](https://en.wikipedia.org/wiki/Dangling_pointer)

当一个pointer由多个thread访问，那么对于这个pointer的管理就变得棘手了：

- 谁来负责pointer的释放

如果某个thread释放了pointer所指向的object，那么其他thread如果继续使用该pointer的话，则就会导致[Dangling pointer](https://en.wikipedia.org/wiki/Dangling_pointer)；

显然，从[Inversion of control principle](https://en.wikipedia.org/wiki/Inversion_of_control)的角度来思考的话：当没有thread需要使用这个pointer的时候，由 [framework](https://en.wikipedia.org/wiki/Software_framework)来将它释放，但是C++没有GC。C++ 11`shard_ptr`就正好适用于这种场景，它使用reference counting的技术，来保证pointer的自动释放，从而避免了上诉问题；



下面是多个thread同时使用一个pointer的例子，`periodic_worker`的thread和main thread同时使用`m_Serialization`：

```c++

#include <set>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <iostream>

class periodic_worker
{
public:
	periodic_worker(const std::function<void()> &callback_fun, std::chrono::seconds interval)
	{
		active_ = (interval > std::chrono::seconds::zero());
		if (!active_)
		{
			return;
		}

		worker_thread_ = std::thread([this, callback_fun, interval]()
		{
			for (;;)
			{
				std::unique_lock<std::mutex> lock(this->mutex_);
				if (this->cv_.wait_for(lock, interval, [this]
								{	return !this->active_;}))
				{
					return; // active_ == false, so exit this thread
			}
			callback_fun();
		}
	});
	}
	periodic_worker(const periodic_worker &) = delete;
	periodic_worker &operator=(const periodic_worker &) = delete;
	// stop the worker thread and join it
	~periodic_worker()
	{
		if (worker_thread_.joinable())
		{
			{
				std::lock_guard<std::mutex> lock(mutex_);
				active_ = false;
			}
			cv_.notify_one();
			worker_thread_.join();
		}
	}
private:
	bool active_;
	std::thread worker_thread_;
	std::mutex mutex_;
	std::condition_variable cv_;
};

template<typename SerializationType>
class CQueryMgr
{
public:
	static CQueryMgr& Instance()
	{
		static CQueryMgr Singlton;
		return Singlton;
	}
	void Add(SerializationType* S)
	{
		std::unique_lock<std::mutex> Lock(m_Mutex);
		m_S.insert(S);
	}
	void Del(SerializationType* S)
	{
		if (S)
		{
			std::unique_lock<std::mutex> Lock(m_Mutex);
			m_S.erase(S);
		}
	}
private:
	void PeriodicJob()
	{
		for (auto& S : m_S)
		{
			S->Query();
		}
	}
	CQueryMgr()
	{
		auto Func = [this]()
		{	this->PeriodicJob();};
		m_PeriodicWorker = std::unique_ptr<periodic_worker>( new periodic_worker(Func, std::chrono::seconds(3)));
	}
private:
	std::mutex m_Mutex;
	std::set<SerializationType*> m_S;
	std::unique_ptr<periodic_worker> m_PeriodicWorker;
};

class CSerialization
{
public:
	CSerialization()
	{
		CQueryMgr<CSerialization>::Instance().Add(this);
	}
	~CSerialization()
	{
		CQueryMgr<CSerialization>::Instance().Del(this);
	}
	void Query()
	{
		std::cout << "Query" << std::endl;
	}
};

class CApi
{
public:
	CApi()
	{
		m_Serialization = new CSerialization();
	}
	~CApi()
	{
		if (m_Serialization)
		{
			delete m_Serialization;
			m_Serialization = nullptr;
		}
	}
private:
	CSerialization* m_Serialization;
};

int main()
{
	CApi* Api0 = new CApi();
	std::this_thread::sleep_for(std::chrono::seconds(9));
	delete Api0;
}

// 编译: g++  --std=c++11 test.cpp -lpthread
```



在`CApi`的析构函数中，直接将`m_Serializer`析构掉，`CSerialization`使用了RAII，如果在它的destructor中，没有`Del(this)`，则导致了[Dangling pointer](https://en.wikipedia.org/wiki/Dangling_pointer)。

### `std::shared_ptr` thread_safety

综合stackoverflow [std::shared_ptr thread safety explained](https://stackoverflow.com/questions/9127816/stdshared-ptr-thread-safety-explained)、cppreference [`std::shared_ptr`](https://en.cppreference.com/w/cpp/memory/shared_ptr) 、[Boost.SmartPtr: The Smart Pointer Library](https://www.boost.org/doc/libs/1_72_0/libs/smart_ptr/doc/html/smart_ptr.html)中的描述可知：

- Standard guarantees that reference counting is handled thread safe and it's platform independent
- standard guarantees that only one thread (holding last reference) will call delete on shared object
- `shared_ptr` does not guarantee any thread safety for object stored in it

#### cppreference [`std::shared_ptr`](https://en.cppreference.com/w/cpp/memory/shared_ptr) 

> All member functions (including copy constructor and copy assignment) can be called by multiple threads on different instances of `shared_ptr` without additional synchronization even if these instances are copies and share ownership of the same object. If multiple threads of execution access the same `shared_ptr` without synchronization and any of those accesses uses a non-const member function of `shared_ptr` then a data race will occur; the [`shared_ptr` overloads of atomic functions](https://en.cppreference.com/w/cpp/memory/shared_ptr/atomic) can be used to prevent the data race.

上面这段话其实描述了两种情况：

- 多个thread同时使用同一个`shared_ptr` instance，此时是需要synchronization 
- 多个thread，每个thread有一个自己的`shared_ptr` instance，这些instance执行同一个object

#### stackoverflow [std::shared_ptr thread safety explained](https://stackoverflow.com/questions/9127816/stdshared-ptr-thread-safety-explained)

I'm reading http://gcc.gnu.org/onlinedocs/libstdc++/manual/shared_ptr.html and some thread safety issues are still not clear for me:

1. Standard guarantees that reference counting is handled thread safe and it's platform independent, right?
2. Similar issue - standard guarantees that only one thread (holding last reference) will call delete on shared object, right?
3. `shared_ptr` does not guarantee any thread safety for object stored in it?

Pseudo code:

```cpp
// Thread I
shared_ptr<A> a (new A (1));

// Thread II
shared_ptr<A> b (a);

// Thread III
shared_ptr<A> c (a);

// Thread IV
shared_ptr<A> d (a);

d.reset (new A (10));
```

[A](https://stackoverflow.com/a/13639645)

As others have pointed out, you've got it figured out correctly regarding your original 3 questions.

But the ending part of your edit

> Calling reset() in thread IV will delete previous instance of A class created in first thread and replace it with new instance? Moreover after calling reset() in IV thread other threads will see only newly created object?

is incorrect. Only `d` will point to the new `A(10)`, and `a`, `b`, and `c` will continue to point to the original `A(1)`. This can be seen clearly in the following short example.

[A](https://stackoverflow.com/a/9133225)

Correct, `shared_ptr`s use atomic increments/decrements of a reference count value.



## Example

### `make_shard` with no argument

这个例子是源自cppreference [std::make_shared, std::make_shared_for_overwrite](https://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared)

```c++
#include <iostream>
#include <memory>
#include <type_traits>
 
struct C
{
  
  int i;
};
 
int main()
{
    auto sp = std::make_shared<C>();
 
    //static_assert(std::is_same_v<decltype(sp), std::shared_ptr<C>>);
 
    std::cout << sp->i << '\n';
}
```


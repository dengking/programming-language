# `compare_exchange`



## cppreference [std::atomic_compare_exchange_`***`](https://en.cppreference.com/w/cpp/atomic/atomic_compare_exchange)

```C++
template< class T >
bool atomic_compare_exchange_weak( std::atomic<T>* obj,
                                   typename std::atomic<T>::value_type* expected,
                                   typename std::atomic<T>::value_type desired ) noexcept;
```



Atomically compares the [object representation](https://en.cppreference.com/w/cpp/language/object) (until C++20)[value representation](https://en.cppreference.com/w/cpp/language/object) (since C++20) of the object pointed to by `obj` with that of the object pointed to by `expected`, and if those are bitwise-equal, replaces the former with `desired` (performs read-modify-write operation). Otherwise, loads the actual value pointed to by `obj` into `*expected` (performs load operation). Copying is performed as if by [std::memcpy](https://en.cppreference.com/w/cpp/string/byte/memcpy).

> NOTE: 
>
> 一、需要注意的是，C++的 [std::atomic_compare_exchange_`***`](https://en.cppreference.com/w/cpp/atomic/atomic_compare_exchange) 执行了如下两个operation:
>
> a、[CAS](https://en.wikipedia.org/wiki/Compare-and-swap)
>
> 比较 `obj` 和 `expected`，如果相等，则说明状态没有变化、没有其它的transaction发生，则commit；否则rollback。
>
> b、"loads"(其实是"Copying"): "loads the actual value pointed to by `obj` into `*expected` (performs load operation)"
>
> 这样的设计在一定程度上简化了使用；
>
> 因此，它需要分别为这两个operation指定memory ordering option。

The memory models for the read-modify-write and load operations are `succ` and `fail` respectively. The (1-2) versions use [std::memory_order_seq_cst](https://en.cppreference.com/w/cpp/atomic/memory_order) by default.

### Example

compare and exchange operations are often used as basic building blocks of lockfree data structures

```C++
#include <atomic>

template<class T>
struct node
{
	T data;
	node *next;
	node(const T &data) :
					data(data), next(nullptr)
	{
	}
};

template<class T>
class stack
{
	std::atomic<node<T>*> head;
public:
	void push(const T &data)
	{
		node<T> *new_node = new node<T>(data);

		// put the current value of head into new_node->next
		new_node->next = head.load(std::memory_order_relaxed);

		// now make new_node the new head, but if the head
		// is no longer what's stored in new_node->next
		// (some other thread must have inserted a node just now)
		// then put that new head into new_node->next and try again
		while (!std::atomic_compare_exchange_weak_explicit(&head, &new_node->next, new_node, std::memory_order_release, std::memory_order_relaxed))
			; // the body of the loop is empty
// note: the above loop is not thread-safe in at least
// GCC prior to 4.8.3 (bug 60272), clang prior to 2014-05-05 (bug 18899)
// MSVC prior to 2014-03-17 (bug 819819). See member function version for workaround
	}
};

int main()
{
	stack<int> s;
	s.push(1);
	s.push(2);
	s.push(3);
}
// g++   --std=c++11 -Wall -pedantic -pthread main.cpp && ./a.out

```

## cppreference [`std::atomic<T>::compare_exchange_***`](https://en.cppreference.com/w/cpp/atomic/atomic/compare_exchange)

### Example



```c++
#include <atomic>
template<typename T>
struct node
{
	T data;
	node *next;
	node(const T &data) :
					data(data), next(nullptr)
	{
	}
};

template<typename T>
class stack
{
	std::atomic<node<T>*> head;
public:
	void push(const T &data)
	{
		node<T> *new_node = new node<T>(data);

		// put the current value of head into new_node->next
		new_node->next = head.load(std::memory_order_relaxed);

		// now make new_node the new head, but if the head
		// is no longer what's stored in new_node->next
		// (some other thread must have inserted a node just now)
		// then put that new head into new_node->next and try again
		while (!head.compare_exchange_weak(new_node->next, new_node, std::memory_order_release, std::memory_order_relaxed))
			; // the body of the loop is empty

// Note: the above use is not thread-safe in at least
// GCC prior to 4.8.3 (bug 60272), clang prior to 2014-05-05 (bug 18899)
// MSVC prior to 2014-03-17 (bug 819819). The following is a workaround:
//      node<T>* old_head = head.load(std::memory_order_relaxed);
//      do {
//          new_node->next = old_head;
//       } while(!head.compare_exchange_weak(old_head, new_node,
//                                           std::memory_order_release,
//                                           std::memory_order_relaxed));
	}
};
int main()
{
	stack<int> s;
	s.push(1);
	s.push(2);
	s.push(3);
}

// g++   --std=c++11 -Wall -pedantic -pthread main.cpp && ./a.out

```





```C++
#include <atomic>
template<typename T>
struct node
{
	T data;
	node *next;
	node(const T &data) :
					data(data), next(nullptr)
	{
	}
};

template<typename T>
class stack
{
	std::atomic<node<T>*> head;
public:
	void push(const T &data)
	{
		node<T> *new_node = new node<T>(data);

		// put the current value of head into new_node->next
		new_node->next = head.load(std::memory_order_relaxed);

		// now make new_node the new head, but if the head
		// is no longer what's stored in new_node->next
		// (some other thread must have inserted a node just now)
		// then put that new head into new_node->next and try again
//		while (!head.compare_exchange_weak(new_node->next, new_node, std::memory_order_release, std::memory_order_relaxed))
//			; // the body of the loop is empty

// Note: the above use is not thread-safe in at least
// GCC prior to 4.8.3 (bug 60272), clang prior to 2014-05-05 (bug 18899)
// MSVC prior to 2014-03-17 (bug 819819). The following is a workaround:
		node<T> *old_head = head.load(std::memory_order_relaxed);
		do
		{
			new_node->next = old_head;
		} while (!head.compare_exchange_weak(old_head, new_node, std::memory_order_release, std::memory_order_relaxed));
	}
};
int main()
{
	stack<int> s;
	s.push(1);
	s.push(2);
	s.push(3);
}
// g++   --std=c++11 -Wall -pedantic -pthread main.cpp && ./a.out

```


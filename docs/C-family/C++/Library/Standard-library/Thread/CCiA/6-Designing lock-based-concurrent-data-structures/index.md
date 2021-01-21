# Designing lock-based concurrent data structures



## 6.1 What does it mean to design for concurrency?

> NOTE: 这一节中描述的概念非常重要，阅读完后，有如下启发：
>
> - 使用*invariant*（在Theory章节有对invariant的描述）来描述*thread-safe*
> - race condition
> - mutex-》*mutual exclusion*-》preventing true concurrent access to the data it protects-》*serialization*
>
> 本章所要讨论内容是如何实现true concurrent；我们原来使用的mutex机制能够保证thread-safe，但是并不够高效。

### 6.1.1 Guidelines for designing data structures for concurrency

> NOTE: 这一节作者告诉了我们设计一个concurrent data structure要考虑的两个方面：
>
> - ensuring that the accesses are *safe* 
> - *enabling* genuine concurrent access
>
> 关于第一点，在chapter 3中已经讨论了，在本节作者再次描述了这些guideline。设计一个concurrent data structure需要考虑的一个非常重要的问题是：多个线程调用这个data structure的不同method时（使用OOP思想），如何保证thread-safe？
>
> 接着作者讨论了第二点。诚如作者所言：如何达到这一点无法给出形式化地描述，而是在实现concurrent data structure时，我们应该时刻如下核心问题：
>
> how can you minimize the amount of serialization that must occur and enable the greatest amount of true concurrency?
>
> 本书第六章介绍Lock-based concurrent data structures
>
> 本书第七章介绍Lock-free concurrent data structures

## 6.2 Lock-based concurrent data structures

> NOTE: one mutex VS multiple mutex
>
> 当使用multiple mutex的时候，情况会更加复杂，更加可能出现deadlock。

### 6.2.1 A thread-safe stack using locks

Listing 6.1 A class definition for a thread-safe stack

```c++
#include <exception>
#include <algorithm>
#include <deque>
#include <memory>
#include <mutex>
#include <stack>

struct empty_stack: std::exception
{
	const char* what() const throw ();
};
template<typename T>
class threadsafe_stack
{
private:
	std::stack<T> data;
	mutable std::mutex m;
public:
	threadsafe_stack()
	{
	}
	threadsafe_stack(const threadsafe_stack& other)
	{
		std::lock_guard < std::mutex > lock(other.m);
		data = other.data;
	}
	threadsafe_stack& operator=(const threadsafe_stack&) = delete;
	void push(T new_value)
	{
		std::lock_guard<std::mutex> lock(m);
		data.push(std::move(new_value));
	}
	std::shared_ptr<T> pop()
	{
		std::lock_guard<std::mutex> lock(m);
		if (data.empty())
			throw empty_stack();
		std::shared_ptr<T> const res(
				std::make_shared<T>(std::move(data.top())));
		data.pop();
		return res;
	}
	void pop(T& value)
	{
		std::lock_guard<std::mutex> lock(m);
		if (data.empty())
			throw empty_stack();
		value = std::move(data.top());
		data.pop();
	}
	bool empty() const
	{
		std::lock_guard<std::mutex> lock(m);
		return data.empty();
	}
};

```

> NOTE: 上述代码使用了一个mutex。它使用的是函数级别的锁。

Second, there’s a potential for a race condition between  `empty()` and either of the `pop()` functions, but because the code explicitly checks for the contained stack being empty while holding the lock in  `pop()` , this **race condition** isn’t problematic. By returning the popped data item directly as part of the call to  `pop()` , you avoid a potential race condition that would be present with separate  `top()` and  `pop()` member functions such as those in  `std::stack<>` .

> NOTE: 原文中的这段话是在介绍race condition，它有两种可能的race condition：
>
> - `empty()`和`pop()`，这种race condition，我并不了解，感觉应该非常类似于time-of-check-to-time-of-use
> - `top()`和`pop()`，这个race condition是比较好理解的，虽然使用mutex能够保证`top()`和`pop()`都是thread safe的，但是当一个用户先执行`top()`然后执行`pop()`，这种情况下是not thread safe的，因为在这两个操作之间，stack可能已经变更了，比如有线程插入了新的数据，这种race condition，比较类似于time-of-check-to-time-of-use。所以上述代码为例规避这个问题，在一个`pop()`函数来实现原来两个函数的功能，其实这样看下来，这个`threadsafe_stack`的功能是不齐全的，它无法实现`top()`。

Although it’s safe for multiple threads to call the member functions concurrently, because of the use of locks, only one thread is ever actually doing any work in the stack data structure at a time. This *serialization* of threads can potentially limit the performance of an application where there’s significant contention on the  stack : while a thread is waiting for the lock, it isn’t doing any useful work. Also, the stack doesn’t provide any means for waiting for an item to be added, so if a thread needs to wait, it must periodically call  `empty()` or just call  `pop()` and catch the  `empty_stack exception`s. This makes this stack implementation a poor choice if such a scenario is required, because a **waiting thread** must either consume precious resources checking for data or the user must write external wait and notification code (for example, using condition variables), which might render the internal locking unnecessary and therefore wasteful. The queue from chapter 4 shows a way of incorporating such waiting into the data structure itself using a condition variable inside the data structure, so let’s look at that next.

> NOTE:  上面这段话中所讨论的waiting for an item to be added，其实意图是实现produce-consumer模型，即当一个thread往stack中push后，通知另外一个thread去pop，其实这种场景使用queue会更加好，这在下一节进行了介绍。

### 6.2.2 A thread-safe queue using locks and condition variables

Listing 6.2 The full class definition for a thread-safe queue using condition variables

```c++
#include <algorithm>
#include <deque>
#include <memory>
#include <mutex>
#include <queue>
#include <condition_variable>

template<typename T>
class threadsafe_queue
{
private:
	mutable std::mutex mut;
	std::queue<T> data_queue;
	std::condition_variable data_cond;
public:
	threadsafe_queue()
	{
	}
	void push(T new_value)
	{
		std::lock_guard<std::mutex> lk(mut);
		data_queue.push(std::move(data));
		data_cond.notify_one();
	}
	void wait_and_pop(T& value)
	{
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk, [this]
		{	return !data_queue.empty();});
		value = std::move(data_queue.front());
		data_queue.pop();
	}
	std::shared_ptr<T> wait_and_pop()
	{
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk, [this]
		{	return !data_queue.empty();});
		std::shared_ptr<T> res(
				std::make_shared<T>(std::move(data_queue.front())));
		data_queue.pop();
		return res;
	}
	bool try_pop(T& value)
	{
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty())
			return false;
		value = std::move(data_queue.front());
		data_queue.pop();
		return true;
	}
	std::shared_ptr<T> try_pop()
	{
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty())
			return std::shared_ptr<T>();
		std::shared_ptr<T> res(
				std::make_shared<T>(std::move(data_queue.front())));
		data_queue.pop();
		return res;
	}
	bool empty() const
	{
		std::lock_guard<std::mutex> lk(mut);
		return data_queue.empty();
	}
};
```

> NOTE: 为什么`empty()`要加锁？可能存在race condition，如

The new  `wait_and_pop()` functions are a solution to the problem of waiting for a queue entry that you saw with the stack; rather than continuously calling  `empty()` , the waiting thread can just call  `wait_and_pop()` and the data structure will handle the waiting with a condition variable. The call to  `data_cond.wait()` won’t return until the underlying queue has at least one element, so you don’t have to worry about the possibility of an empty queue at this point in the code, and the data is still protected with
the lock on the mutex. These functions don’t therefore add any new race conditions or possibilities for deadlock, and the invariants will be upheld.

There’s a slight twist with regard to exception safety in that if more than one thread is waiting when an entry is pushed onto the queue, only one thread will be woken by the call to  `data_cond.notify_one()`...

> NOTE: 原文的这一段所讨论的是`wait_and_pop`函数[exception safety](https://en.wikipedia.org/wiki/Exception_safety)的问题，它是**No exception safety**的，它的exception来源是当构造`std::shared_ptr<>`时，可能抛出exception。



Listing 6.3 A thread-safe queue holding  `std::shared_ptr<>` instances

```c++
#include <memory>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <exception>
template<typename T>
class threadsafe_queue
{
private:
	mutable std::mutex mut;
	std::queue<std::shared_ptr<T> > data_queue;
	std::condition_variable data_cond;
	public:
	threadsafe_queue()
	{
	}
	void wait_and_pop(T& value)
	{
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk, [this]
		{	return !data_queue.empty();});
		value = std::move(*data_queue.front());
		data_queue.pop();
	}
	bool try_pop(T& value)
	{
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty())
			return false;
		value = std::move(*data_queue.front());
		data_queue.pop();
		return true;
	}
	std::shared_ptr<T> wait_and_pop()
	{
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk, [this]
		{	return !data_queue.empty();});
		std::shared_ptr<T> res = data_queue.front();
		data_queue.pop();
		return res;
	}
	std::shared_ptr<T> try_pop()
	{
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty())
			return std::shared_ptr<T>();
		std::shared_ptr<T> res = data_queue.front();
		data_queue.pop();
		return res;
	}
	void push(T new_value)
	{
		std::shared_ptr<T> data(
				std::make_shared<T>(std::move(new_value)));
		std::lock_guard<std::mutex> lk(mut);
		data_queue.push(data);
		data_cond.notify_one();
	}
	bool empty() const
	{
		std::lock_guard<std::mutex> lk(mut);
		return data_queue.empty();
	}
};
```


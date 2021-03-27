# Use reference counting to control object lifetime to avoid dangling pointer



## Multiple-thread share a pointer and [Dangling pointer](https://en.wikipedia.org/wiki/Dangling_pointer)

当一个pointer由多个thread访问，那么对于这个pointer的管理就变得棘手了：

1、谁来负责pointer的释放

2、如果某个thread释放了pointer所指向的object，那么其他thread如果继续使用该pointer的话，则就会导致[Dangling pointer](https://en.wikipedia.org/wiki/Dangling_pointer)；

显然，从[Inversion of control principle](https://en.wikipedia.org/wiki/Inversion_of_control)的角度来思考的话：当没有thread需要使用这个pointer的时候，由 [framework](https://en.wikipedia.org/wiki/Software_framework)来将它释放，但是C++没有GC。C++ 11`shard_ptr`就正好适用于这种场景，它使用reference counting的技术，来保证pointee的自动释放，从而避免了上诉问题；

### Example

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



## nextptr [Using weak_ptr for circular references](https://www.nextptr.com/tutorial/ta1382183122/using-weak_ptr-for-circular-references) 

One of the biggest concerns dealing with the raw pointers is that sometimes it is hard to ensure that a raw pointer is not dangling or valid. **Automatic memory management** by *shared_ptr* leads to a safer and easier to maintain code. As long as a component or function holds a *shared_ptr*, the object managed by the *shared_ptr* stays in memory.



这是一个非常重要的主题，在control theory中已经涉及这个topic了，只是没有提供具体的实现案例，在下面的文章中，给出了非常好的例子:

1、《shared_ptr-draft》

2、spdlog `async_logger`

使用reference counting所实现的control，是一种隐式的control、自动的control，这种control方式是更加高级的，在很多情况下，我们都是 需要这种更加高级的control方式的，因为它使得用户不用关注、调用很多的接口。





# dangling pointer的情况

1、multithread: 在multithread中，是非常容易出现dangling pointer的，比如:

event-driven/network library中，传入一个object来作为callback，下面会对这种情况进行详细分析；这种情况的最最典型的例子是: amust api。

# access outside of object lifetime

将一个object作为callback传入到另外一个event driven library中的做法是非常不好的，这种情况下，一般涉及multithread，这个object同时被多个thread access，这样的做法有如下劣势:

## 1

1、如果在这个object的constructor中，使用`this`传入到另外一个library中，则这就导致了access outside of object lifetime；

在这个object的constructor的执行过程中，可能library中的thread就开始access它了，那么这就发生了access outside of object lifetime，这个问题，我之前碰到过，就是在是使用研发中心的仲裁插件的时候。

相关问题: Google: pass this in constructor c++: 

stackoverflow [Passing “this” to a function from within a constructor?](https://stackoverflow.com/questions/445127/passing-this-to-a-function-from-within-a-constructor)

https://isocpp.org/wiki/faq/ctors#using-this-in-ctors




## TODO

将spdlog中`std::shared_ptr`的使用，作为一个例子来说明这个问题；


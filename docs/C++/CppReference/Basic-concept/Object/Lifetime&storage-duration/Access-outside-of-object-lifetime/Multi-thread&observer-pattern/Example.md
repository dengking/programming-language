# Example

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



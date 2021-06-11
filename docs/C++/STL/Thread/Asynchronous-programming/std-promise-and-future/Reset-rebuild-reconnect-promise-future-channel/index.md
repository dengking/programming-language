# Rebuild、reconnect promise-future channel

1、我的应用中，需要重复地登录、接收登录响应，它是Asynchronous IO + callback，为了处理方便我使用promise-future channel来实现"async to sync-blocking-等待"，简化处理；后来基于它开发了一个交互式的application，在这个application可能需要重复进行登录，此时会抛出:

[std::future_errc](https://en.cppreference.com/w/cpp/thread/future_errc) `promise_already_satisfied`

2、后来经过查阅方才知道: 

对于一个promise对象，[std::promise<R>::set_value](https://en.cppreference.com/w/cpp/thread/promise/set_value)  只能够被调用一次，第二次调用，可能会抛出上述exception；也就是 promise-future channel 只能够使用一次。

3、那如何实现我的重复使用的目的呢？方法是: "rebuild、reconnect promise-future channel"

经过实践，下面是我的最终程序:

```C++
#include <future>
#include <chrono>
#include <thread>

#include <sstream>
#include <iostream>

#define LOG(...) LogWrapper(__FILE__, __LINE__, __VA_ARGS__)

// Terminator
void Log_Recursive(const char *file, int line, std::ostringstream &msg)
{
	std::cout << file << "(" << line << "): " << msg.str() << std::endl;
}

// "Recursive" variadic function
template<typename T, typename ... Args>
void Log_Recursive(const char *file, int line, std::ostringstream &msg, T value, const Args &... args)
{
	msg << value;
	Log_Recursive(file, line, msg, args...);
}

// Log_Recursive wrapper that creates the ostringstream
template<typename ... Args>
void LogWrapper(const char *file, int line, const Args &... args)
{
	std::ostringstream msg;
	Log_Recursive(file, line, msg, args...);
}

class Test
{
	std::future<bool> m_ConnectResultFuture;
	std::promise<bool> m_ConnectResultPromise;
	bool m_ConnectedFlag { false };
	std::future<void> m_SendRspTask;
public:
	bool Init()
	{
		m_ConnectResultPromise = std::promise<bool>();
		m_ConnectResultFuture = m_ConnectResultPromise.get_future(); // 建立future-promise channel

		m_SendRspTask = std::async(std::launch::async, [this]
		{
			std::this_thread::sleep_for(std::chrono::seconds(2));
			this->m_ConnectResultPromise.set_value(true);
		}
		);
		std::future_status status = m_ConnectResultFuture.wait_for(std::chrono::seconds(3));
		if (status == std::future_status::deferred)
		{
			LOG("未知状态，将等待3S(可能会连接超时)");
			std::this_thread::sleep_for(std::chrono::seconds(3));
			m_ConnectedFlag = true;
		}
		else if (status == std::future_status::timeout)
		{
			LOG("连接超时了");
			m_ConnectedFlag = true;
		}
		else if (status == std::future_status::ready)
		{
			m_ConnectedFlag = m_ConnectResultFuture.get();
			if (m_ConnectedFlag)
			{
				LOG("连接成功了!");
			}
			else
			{
				LOG("连接失败了!");
			}

		}
		return m_ConnectedFlag;
	}
};

int main()
{
	Test t;
	t.Init();
	t.Init();
	return 0;
}
// g++ --std=c++11 test.cpp -Wall -Wextra -pedantic -lpthread


```



## cppreference [std::promise<R>::set_value](https://en.cppreference.com/w/cpp/thread/promise/set_value)

> NOTE: 
>
> 1、这里面已经讨论了thread safety问题



## stackoverflow [How do I “clear” std::promise?](https://stackoverflow.com/questions/35173835/how-do-i-clear-stdpromise)



### [A](https://stackoverflow.com/a/43103035)

To reuse promises, simply reassign them.

```cpp
std::promise<int> my_promise;
//use the promise
my_promise = std::promise<int>(); //now you have a new promise
```

> NOTE: 
>
> 1、然后需要重新建立promise-future channel





## stackoverflow [std::promise set_value and thread safety](https://stackoverflow.com/questions/45626919/stdpromise-set-value-and-thread-safety)

> NOTE: 
>
> 1、非常好的topic

### [A](https://stackoverflow.com/a/45653110)

> NOTE: 
>
> 1、非常好的回答



## github [std::future_error Promise already satisfied #55](https://github.com/alexa/avs-device-sdk/issues/55)
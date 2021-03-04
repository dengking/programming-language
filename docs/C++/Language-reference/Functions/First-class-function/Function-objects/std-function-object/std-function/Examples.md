



# Examples

## Examples from cppreference [std::function](https://en.cppreference.com/w/cpp/utility/functional/function)

下面的这些例子是根据cppreference [std::function](https://en.cppreference.com/w/cpp/utility/functional/function)拆分、改编的。

### Store a free function

```c++
#include <functional>
#include <iostream>

void print_num(int i)
{
	std::cout << i << '\n';
}

int main()
{
	// store a free function
	std::function<void(int)> f_display = print_num;
	f_display(-9);

}
// g++ --std=c++11 test.cpp
```



### Store a lambda

```c++
#include <functional>
#include <iostream>

void print_num(int i)
{
	std::cout << i << '\n';
}

int main()
{

	// store a lambda
	std::function<void()> f_display_42 = [](){	print_num(42);};
	f_display_42();

}
// g++ --std=c++11 test.cpp
```



### Store the result of a call to `std::bind`

```c++
#include <functional>
#include <iostream>

int print_num(int i)
{
	std::cout << i << '\n';
	return i;
}

int main()
{

	// store the result of a call to std::bind
	std::function<int()> f_display_31337 = std::bind(print_num, 31337);
	int i = f_display_31337();
    std::cout<<i<<std::endl;

}
// g++ --std=c++11 test.cpp
```

输出如下:

```
31337
31337
```

### OOP

```c++
#include <functional>
#include <iostream>

struct Foo
{
	Foo(int num)
			: num_(num)
	{
	}
	void print_add(int i) const
    {
		std::cout << num_ + i << '\n';
	}
	int num_;
};

int main()
{

	// store a call to a member function
	std::function<void(const Foo&, int)> f_add_display = &Foo::print_add;
	const Foo foo(314159);
	f_add_display(foo, 1);
	f_add_display(314159, 1); // // 首先调用Foo::Foo(int)构造对象，然后调用成员函数；

	// store a call to a data member accessor
	std::function<int(Foo const&)> f_num = &Foo::num_;
	std::cout << "num_: " << f_num(foo) << '\n';

	// store a call to a member function and object
	using std::placeholders::_1;
	std::function<void(int)> f_add_display2 = std::bind(&Foo::print_add, foo, _1);
	f_add_display2(2);

	// store a call to a member function and object ptr
	std::function<void(int)> f_add_display3 = std::bind(&Foo::print_add, &foo, _1);
	f_add_display3(3);

}

```



### Store a call to a function object

```c++
#include <functional>
#include <iostream>

struct PrintNum
{
	void operator()(int i) const
	{
		std::cout << i << '\n';
	}
};

int main()
{

	// store a call to a function object
	std::function<void(int)> f_display_obj = PrintNum();
	f_display_obj(18);
}

```



## Passing Lambda Expressions with `std::function`

### drdobbs [Lambdas in C++11](https://www.drdobbs.com/cpp/lambdas-in-c11/240168241?pgno=2)

```c++
#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
using namespace std;

void run_within_for_each(std::function<void(int)> func)
{
	vector<int> numbers { 1, 2, 3, 4, 5, 10, 15, 20, 25, 35, 45, 50 };

	for_each(numbers.begin(), numbers.end(), func);
}

int main()
{
	auto func1 = [](int y)
	{
		cout << y << endl;
	};

	auto func2 = [](int z)
	{
		cout << z * 2 << endl;
	};

	run_within_for_each(func1);
	run_within_for_each(func2);
}
// g++ --std=c++11 test.cpp 
```





```c++
std::function<bool(int)> create_function()
{
    return [](int x)
    {
        return (x < 100);
    };
}
```



> The combination of lambda expressions with the `std::function` can boost your productivity and reduce a huge amount of boilerplate code when you work with code that requires the use of functors.



### spdlog [`periodic_worker`](https://github.com/gabime/spdlog/blob/v1.x/include/spdlog/details/periodic_worker-inl.h) 



```c++
#include <chrono>
#include <functional>
#include <mutex> // std::mutex
#include <thread> // std::thread
#include <condition_variable> // std::condition_variable
#include <iostream>
#include <memory> // std::unique_ptr

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
				if (this->cv_.wait_for(lock, interval, [this] {	return !this->active_;}))
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

class Test
{
public:
	Test()
	{
		auto Job = [this](){this->PeriodicJob();};
		m_Worker = std::unique_ptr<periodic_worker> { new periodic_worker { Job, std::chrono::seconds(3) } };
	}
private:
	void PeriodicJob()
	{
		std::cout << "Hello world..." << std::endl;
	}

private:
	std::unique_ptr<periodic_worker> m_Worker;
};

int main()
{
	Test t;
	for(;;)
	{
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
}
// g++ -g --std=c++11 test.cpp -lpthread
```

## spdlog examples

1、[spdlog](https://github.com/gabime/spdlog)/[include](https://github.com/gabime/spdlog/tree/v1.x/include)/[spdlog](https://github.com/gabime/spdlog/tree/v1.x/include/spdlog)/[details](https://github.com/gabime/spdlog/tree/v1.x/include/spdlog/details)/[**thread_pool.h**](https://github.com/gabime/spdlog/blob/v1.x/include/spdlog/details/thread_pool.h)

2、[spdlog](https://github.com/gabime/spdlog)/[include](https://github.com/gabime/spdlog/tree/v1.x/include)/[spdlog](https://github.com/gabime/spdlog/tree/v1.x/include/spdlog)/[details](https://github.com/gabime/spdlog/tree/v1.x/include/spdlog/details)/[**periodic_worker.h**](https://github.com/gabime/spdlog/blob/v1.x/include/spdlog/details/periodic_worker.h)


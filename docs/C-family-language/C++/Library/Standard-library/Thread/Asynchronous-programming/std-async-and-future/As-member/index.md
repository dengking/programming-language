# 将`std::future`作为member



下面是示例代码，改编自: [Stopping long-sleep threads](https://stackoverflow.com/questions/29775153/stopping-long-sleep-threads) # [A](https://stackoverflow.com/a/29775639):

```C++
#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <future>
#include <chrono>
#include <iostream>
#include <vector>

struct timer_killer
{
	// returns false if killed:
	template<class R, class P>
	bool wait_for(std::chrono::duration<R, P> const &time) const
	{
		std::unique_lock<std::mutex> lock(m);
		return !cv.wait_for(lock, time, [&]
		{	return terminate;});
	}
	void kill()
	{
		std::unique_lock<std::mutex> lock(m);
		terminate=true; // should be modified inside mutex lock
		cv.notify_all();// it is safe, and *sometimes* optimal, to do this outside the lock
	}
	// I like to explicitly delete/default special member functions:
	timer_killer() = default;
	timer_killer(timer_killer&&)=delete;
	timer_killer(timer_killer const&)=delete;
	timer_killer& operator=(timer_killer&&)=delete;
	timer_killer& operator=(timer_killer const&)=delete;
private:
	mutable std::condition_variable cv;
	mutable std::mutex m;
	bool terminate = false;
};
timer_killer bob;

class Test
{
public:
	Test()
	{
		m_f = std::async(std::launch::async, [this]
		{
			while(bob.wait_for(std::chrono::milliseconds(50)))
			{
				std::cout << "thread 1 says hi\n";
			}
			std::cout << "thread 1 dead\n";
		});
		std::cout << "构造完成\n";
	}
	~Test()
	{
		m_f.wait();
	}
private:
	std::future<void> m_f;
};
int main()
{
	Test t;
	bob.wait_for(std::chrono::milliseconds(1000));
	std::cout << "killing threads\n";
	bob.kill();
	std::cout << "done\n";
	// your code goes here
	return 0;
}
// g++ --std=c++11 test.cpp -lpthread
```

上述 `class Test` 就是将`std::future`作为member
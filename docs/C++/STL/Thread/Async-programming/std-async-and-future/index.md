# `std::async` and `std::future`

如何启动？如何获取结果？

## thispointer [C++11 Multithreading – Part 9: std::async Tutorial & Example](https://thispointer.com/c11-multithreading-part-9-stdasync-tutorial-example/)

In this article we will discuss how to execute tasks asynchronously with `std::async` in C++11.

`std::async` is introduced in c++11.

### what is std::async()

**`std::async()`** is a function template that accepts a **callback**(i.e. function or function object) as an argument and potentially executes them asynchronously.

```C++
template<class Fn, class ... Args>
future<typename result_of<Fn(Args...)>::type> async(launch policy, Fn &&fn, Args &&... args);

```

**`std::async`** returns a **`std::future<T>`** that stores the value returned by function object executed by **`std::async()`**. Arguments expected by function can be passed to `std::async()` as arguments after the function pointer argument.

### Launch policy

First argument in `std::async` is launch policy, it control the **asynchronous behaviour** of `std::async`. We can create `std::async` with 3 different launch policies i.e.

#### `std::launch::async`

It guarantees the **asynchronous behaviour** i.e. passed function will be executed in seperate thread.

#### `std::launch::deferred`

Non **asynchronous behaviour** i.e. Function will be called when other thread will call `get()` on future to access the shared state.

#### `std::launch::async | std::launch::deferred`

Its the default behaviour. With this **launch policy** it can run asynchronously or not depending on the load on system. But we have no control over it.

If we do not specify an launch policy. Its behaviour will be similar to **`std::launch::async | std::launch::deferred`**.



We are going to use `std::launch::async` launch policy in this article.

We can pass any callback in `std::async` i.e.

- Function Pointer
- Function Object
- Lambda Function

Let’s understand the need of `std::async` by an example,

### Need of `std::async()`

Suppose we have to fetch some data (string) from DB and some from files in file-system. Then I need to merge both the strings and print.

```C++
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
using namespace std::chrono;
std::string fetchDataFromDB(std::string recvdData)
{
	// Make sure that function takes 5 seconds to complete
	std::this_thread::sleep_for(seconds(5));
	//Do stuff like creating DB Connection and fetching Data
	return "DB_" + recvdData;
}
std::string fetchDataFromFile(std::string recvdData)
{
	// Make sure that function takes 5 seconds to complete
	std::this_thread::sleep_for(seconds(5));
	//Do stuff like fetching Data File
	return "File_" + recvdData;
}
int main()
{
	// Get Start Time
	system_clock::time_point start = system_clock::now();
	//Fetch Data from DB
	std::string dbData = fetchDataFromDB("Data");
	//Fetch Data from File
	std::string fileData = fetchDataFromFile("Data");
	// Get End Time
	auto end = system_clock::now();
	auto diff = duration_cast<std::chrono::seconds>(end - start).count();
	std::cout << "Total Time Taken = " << diff << " Seconds" << std::endl;
	//Combine The Data
	std::string data = dbData + " :: " + fileData;
	//Printing the combined Data
	std::cout << "Data = " << data << std::endl;
	return 0;
}
// g++ --std=c++11 test.cpp

```

> NOTE: 输出如下:
>
> ```C++
> Total Time Taken = 10 Seconds
> Data = DB_Data :: File_Data
> 
> ```

As both the functions **fetchDataFromDB()** & **fetchDataFromFile()** takes 5 seconds each and are running in a single thread so, total time consumed will be 10 seconds.

Now as fetching data from DB and file are independent of each other and also time consuming. So, we can run them in parallel.

One way to do is create a new thread pass a **promise** as an argument to thread function and fetch data from associated `std::future` object in calling thread.

> NOTE: 使用 `std::promise` + `std::future` 的模式，参见 `C++\Library\Standard-library\Thread\Asynchronous-programming\std-async-and-future` 章节。

The other easy way is using `std::async`.

### Calling `std::async` with function pointer as callback

Now let’s modify the above code and call `fetchDataFromDB()` asynchronously using `std::async()` i.e.

```C++
#include <thread>
#include <future>
#include <chrono>
std::future<std::string> resultFromDB = std::async(std::launch::async, fetchDataFromDB, "Data");
// Do Some Stuff
//Fetch Data from DB
// Will block till data is available in future<std::string> object.
std::string dbData = resultFromDB.get();

```



**`std::async()` does following things,**

- It automatically creates a thread (Or picks from internal thread pool) and a **promise object** for us.
- Then passes the `std::promise` object to thread function and returns the associated `std::future` object.
- When our passed argument `function` exits then its value will be set in this promise object, so eventually return value will be available in `std::future` object.

Now change the above example and use `std::async` to read data from DB asyncronously i.e.

Checkout the compete example as follows,

```C++
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <future>
using namespace std::chrono;
std::string fetchDataFromDB(std::string recvdData)
{
	// Make sure that function takes 5 seconds to complete
	std::this_thread::sleep_for(seconds(5));
	//Do stuff like creating DB Connection and fetching Data
	return "DB_" + recvdData;
}
std::string fetchDataFromFile(std::string recvdData)
{
	// Make sure that function takes 5 seconds to complete
	std::this_thread::sleep_for(seconds(5));
	//Do stuff like fetching Data File
	return "File_" + recvdData;
}
int main()
{
	// Get Start Time
	system_clock::time_point start = system_clock::now();
	std::future<std::string> resultFromDB = std::async(std::launch::async, fetchDataFromDB, "Data");
	//Fetch Data from File
	std::string fileData = fetchDataFromFile("Data");
	//Fetch Data from DB
	// Will block till data is available in future<std::string> object.
	std::string dbData = resultFromDB.get();
	// Get End Time
	auto end = system_clock::now();
	auto diff = duration_cast<std::chrono::seconds>(end - start).count();
	std::cout << "Total Time Taken = " << diff << " Seconds" << std::endl;
	//Combine The Data
	std::string data = dbData + " :: " + fileData;
	//Printing the combined Data
	std::cout << "Data = " << data << std::endl;
	return 0;
}
// g++ --std=c++11 test.cpp -lpthread

```

> NOTE: 上述程序的输出如下:
>
> ```
> Total Time Taken = 5 Seconds
> Data = DB_Data :: File_Data
> ```

Now it will take 5 seconds only.

### Calling `std::async` with Function Object as callback

```C++
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <future>
using namespace std::chrono;
/*
 * Function Object
 */
struct DataFetcher
{
	std::string operator()(std::string recvdData)
	{
		// Make sure that function takes 5 seconds to complete
		std::this_thread::sleep_for(seconds(5));
		//Do stuff like fetching Data File
		return "File_" + recvdData;
	}
};

int main()
{
	//Calling std::async with function object
	std::future<std::string> fileResult = std::async(DataFetcher(), "Data");
	std::cout << fileResult.get() << std::endl;
	return 0;
}
// g++ --std=c++11 test.cpp -lpthread

```



### Calling `std::async` with Lambda function as callback

```C++
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <future>
using namespace std::chrono;

int main()
{
	//Calling std::async with lambda function
	std::future<std::string> resultFromDB = std::async([](std::string recvdData)
	{
		std::this_thread::sleep_for (seconds(5));
		//Do stuff like creating DB Connection and fetching Data
					return "DB_" + recvdData;
	}, "Data");
	std::cout << resultFromDB.get() << std::endl;
	return 0;
}
// g++ --std=c++11 test.cpp -lpthread

```



## cpppatterns [Execute a task asynchronously](https://cpppatterns.com/patterns/execute-task-asynchronously.html)

### DESCRIPTION

[`std::async`](http://en.cppreference.com/w/cpp/thread/async) provides a high-level way to accomplish **asynchronous execution of tasks**, abstracting over [`std::thread`](http://en.cppreference.com/w/cpp/thread/thread) and [`std::promise`](http://en.cppreference.com/w/cpp/thread/promise).

On [line 14](https://cpppatterns.com/patterns/execute-task-asynchronously.html#line14), we call `std::async`, passing `func` as the function to execute asynchronously. Arguments to `func` can be passed as additional arguments to `std::async`. The return value is a [`std::future`](http://en.cppreference.com/w/cpp/thread/future), representing an `int` value that will be returned from the task at some point in the future.

By default, `std::async` will decide whether 

1 to execute the task concurrently or 

2 to wait until we request the result and then execute the task. 

If we want to specifically request one of these behaviours, we can use a flag as the first argument. 

To request concurrent execution, if possible, we use [`std::launch::async`](http://en.cppreference.com/w/cpp/thread/launch). 

To request that the task is executed only when the result is needed, we use [`std::launch::deferred`](http://en.cppreference.com/w/cpp/thread/launch).

On [line 18](https://cpppatterns.com/patterns/execute-task-asynchronously.html#line18), we use the `std::future`’s `get` member function to get the result of the asynchronous task. This blocks until the result is available.

**Note**: It is important that we capture the future returned by `std::async`. If we do not, then the call will always block.

```C++
#include <future>
int func()
{
	int some_value = 0;
	// Do work...
	return some_value;
}
int main()
{
	std::future<int> result_future = std::async(func);
	// Do something...
	int result = result_future.get();
}
// g++ --std=c++11 test.cpp -lpthread

```



## Why should I use `std::async`?

为什么要使用`std::async`？本节讨论这个问题。

### stackoverflow [C++: Simple return value from std::thread?](https://stackoverflow.com/questions/7686939/c-simple-return-value-from-stdthread)

[A](https://stackoverflow.com/a/7687050)

> NOTE: 这个回答能够回答本节的问题，即:" `std::async` (higher-level wrapper for threads and futures)"，这样我们可以写更少的code 

See [this video tutorial](https://www.youtube.com/watch?v=o0pCft99K74&list=PL1835A90FC78FF8BE&index=4) on C++11 futures.

Explicitly with threads and futures:

```cpp
#include <thread>
#include <future>

void func(std::promise<int> && p) {
    p.set_value(1);
}

std::promise<int> p;
auto f = p.get_future();
std::thread t(&func, std::move(p));
t.join();
int i = f.get();
```

Or with `std::async` (higher-level wrapper for threads and futures):

```cpp
#include <thread>
#include <future>
int func() { return 1; }
std::future<int> ret = std::async(&func);
int i = ret.get();
```

I can't comment whether it works on *all* platforms (it seems to work on Linux, but doesn't build for me on Mac OSX with GCC 4.6.1).

### stackoverflow [Why should I use std::async?](https://stackoverflow.com/questions/17963172/why-should-i-use-stdasync)

[A](https://stackoverflow.com/a/17973892)



```C++
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <future>
using namespace std::chrono;
int doSomethingThatTakesTenSeconds()
{
	std::this_thread::sleep_for(std::chrono::seconds(2));
	return 1;
}
int main()
{
	auto fut = std::async(std::launch::async, doSomethingThatTakesTenSeconds);
	auto result1 = doSomethingThatTakesTenSeconds();
	auto result2 = fut.get();
	std::cout << result1 << std::endl;
	std::cout << result2 << std::endl;
	return 0;
}
// g++ --std=c++11 test.cpp -lpthread

```



## modernescpp [Asynchronous Function Calls](https://www.modernescpp.com/index.php/asynchronous-function-calls)
# `std::function`

## cppreference [std::function](https://en.cppreference.com/w/cpp/utility/functional/function)

Class template `std::function` is a general-purpose polymorphic function wrapper. Instances of `std::function` can store, copy, and invoke any [*Callable*](https://en.cppreference.com/w/cpp/named_req/Callable) *target* -- 

- functions
- [lambda expressions](https://en.cppreference.com/w/cpp/language/lambda)
- [bind expressions](https://en.cppreference.com/w/cpp/utility/functional/bind)
- other function objects
- pointers to member functions 
- pointers to data members

> NOTE: 
>
> `std::function`是一种abstraction、wrapper；正如在文章drdobbs [Lambdas in C++11](https://www.drdobbs.com/cpp/lambdas-in-c11/240168241?pgno=2)中所说的：
>
> > Luckily, C++11 incorporates a **wrapper** for any of the following functions with `std::function` and makes it easy to pass around lambda expressions:
> >
> > - Lambda expression
> > - Function pointer
> > - Functor
>
> 上面这段话中的polymorphic 是非常重要的，它表明`std::function`是支持多态的，`std::function`所存储的应该是pointer to function，所以它是reference semantic，所以它能够支持polymorphism；关于reference semantic，参见`C++\Language-reference\Basic-concept\Reference-and-Value-Semantics`。

The stored callable object is called the *target* of `std::function`. If a `std::function` contains no target, it is called *empty*. Invoking the *target* of an *empty* `std::function` results in [std::bad_function_call](https://en.cppreference.com/w/cpp/utility/functional/bad_function_call) exception being thrown.

> NOTE: the stored callable object和`std::function`之间的关系就好比thread和`std::thread`之间的关系。

`std::function` satisfies the requirements of [*CopyConstructible*](https://en.cppreference.com/w/cpp/named_req/CopyConstructible) and [*CopyAssignable*](https://en.cppreference.com/w/cpp/named_req/CopyAssignable).

> NOTE: `std::function`它满足  [*CopyConstructible*](https://en.cppreference.com/w/cpp/named_req/CopyConstructible) 和[*CopyAssignable*](https://en.cppreference.com/w/cpp/named_req/CopyAssignable) ，这传达出的一个semantic是两个 `std::function` object能够表示相同的target；想想，`std::function`是否是 [*MoveConstructible*](https://en.cppreference.com/w/cpp/named_req/MoveConstructible) 和 [*MoveAssignable*](https://en.cppreference.com/w/cpp/named_req/MoveAssignable) 的呢？



### Notes

Care should be taken when a `std::function` whose **result type** is a **reference** is initialized from a lambda expression without a **trailing-return-type**. Due to the way auto deduction works, such **lambda expression** will always return a **prvalue**. Hence, the **resulting reference** will usually bind to a **temporary** whose lifetime ends when `std::function::operator()` returns.

> NOTE: 上面这段话的第一句的意思是：当使用一个“lambda expression”来构造`std::function`的时候，如果这个lambda expression所表示的function的返回值类型是reference，并且lambda expression没有使用**trailing-return-type**来说明返回值的，那么在这种情况下，compiler的auto deduction将会导致这个**lambda expression**的返回值是**prvalue**；那么这种情况下，就会导致dangling reference。下面这个例子就是最好的说明：

```c++
#include <functional>
#include <iostream>


int main()
{
	std::function<const int&()> F([]{ return 42; });
	int x = F(); // Undefined behavior: the result of F() is a dangling reference
	std::cout<<x<<std::endl;
}
// g++ --std=c++11 test.cpp
```





## Examples

### Examples from cppreference [std::function](https://en.cppreference.com/w/cpp/utility/functional/function)

下面的这些例子是根据cppreference [std::function](https://en.cppreference.com/w/cpp/utility/functional/function)拆分、改编的。

#### Store a free function

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



#### Store a lambda

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



#### Store the result of a call to `std::bind`

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

#### OOP

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



#### Store a call to a function object

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



### Passing Lambda Expressions with `std::function`

#### drdobbs [Lambdas in C++11](https://www.drdobbs.com/cpp/lambdas-in-c11/240168241?pgno=2)

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



#### spdlog [`periodic_worker`](https://github.com/gabime/spdlog/blob/v1.x/include/spdlog/details/periodic_worker-inl.h) 



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



## wizmann [C++类型擦除与`std::function`性能探索](https://wizmann.tk/cpp-type-erasure-and-std-function.html)





## `std::function` is generic and a good abstraction

`std::function` is generic and a good abstraction，所有callable都可以包装为`std::function`:

1、lambda

2、method

3、member function

此处需要添加一些例子来进行说明:

1、spdlog的thread_pool

2、`generic_thread_pool`



## `std::function` and member function

### stackoverflow [Using generic std::function objects with member functions in one class](https://stackoverflow.com/questions/7582546/using-generic-stdfunction-objects-with-member-functions-in-one-class)

> NOTE: 非常好的一篇文章

[A](https://stackoverflow.com/a/7582576)

A non-static member function must be called with an object. That is, it always implicitly passes "this" pointer as its argument.

Because your `std::function` signature specifies that your function doesn't take any arguments (`<void(void)>`), you must *bind* the first (and the only) argument.

```cpp
std::function<void(void)> f = std::bind(&Foo::doSomething, this);
```

If you want to bind a function with parameters, you need to specify placeholders:

```cpp
using namespace std::placeholders;
std::function<void(int,int)> f = std::bind(&Foo::doSomethingArgs, this, std::placeholders::_1, std::placeholders::_2);
```

Or, if your compiler supports C++11 lambdas:

```cpp
std::function<void(int,int)> f = [=](int a, int b) {
    this->doSomethingArgs(a, b);
}
```

(I don't have a C++11 capable compiler at hand *right now*, so I can't check this one.)

[A](https://stackoverflow.com/a/7582574)

Either you need

```cpp
std::function<void(Foo*)> f = &Foo::doSomething;
```

> NOTE: pointer to member function

so that you can call it on any instance, or you need to bind a specific instance, for example `this`

```cpp
std::function<void(void)> f = std::bind(&Foo::doSomething, this);
```



[A](https://stackoverflow.com/a/40394121)

If you need to store a member function **without** the class instance, you can do something like this:

```cpp
class MyClass
{
public:
    void MemberFunc(int value)
    {
      //do something
    }
};

// Store member function binding
auto callable = std::mem_fn(&MyClass::MemberFunc);

// Call with late supplied 'this'
MyClass myInst;
callable(&myInst, 123);
```

What would the storage type look like without *auto*? Something like this:

```cpp
std::_Mem_fn_wrap<void,void (__cdecl TestA::*)(int),TestA,int> callable
```

You can also pass this function storage to a standard function binding

```cpp
std::function<void(int)> binding = std::bind(callable, &testA, std::placeholders::_1);
binding(123); // Call
```

Past and future notes: An older interface *std::mem_func* existed, but has since been deprecated. A proposal exists, post C++17, to make [pointer to member functions callable](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0312r0.pdf). This would be most welcome.
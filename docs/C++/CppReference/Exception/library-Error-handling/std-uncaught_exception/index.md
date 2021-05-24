# `std::uncaught_exception`, `std::uncaught_exceptions`

## 发展概述

在1998 [GOTW issue 47: Uncaught Exceptions](http://www.gotw.ca/gotw/047.htm) 中，指出，`bool uncaught_exception() throw();`的问题； 

在 2014 [paper N4152](https://isocpp.org/files/papers/N4152.pdf) 中提出 `int uncaught_exceptions() noexcept;`，最终在 C++2017被标准化了。

## cppreference [std::uncaught_exception, std::uncaught_exceptions](https://en.cppreference.com/w/cpp/error/uncaught_exception)

> NOTE: 理解本文的一个前提是: 理解stack unwind，本文中的stack unwind是指: 由于exception导致退出current scope，从而进行stack unwind，而不是广义上的stack unwind，本文中的[stack unwinding](https://en.cppreference.com/w/cpp/language/throw#Stack_unwinding) 所链接的是 cppreference [`throw` expression # Stack unwinding](https://en.cppreference.com/w/cpp/language/throw#Stack_unwinding)。由于C++ exception stack unwinding是先完成stack unwinding然后执行`catch`，因此在`try` scope中的object的destructor中是可以通过`uncaught_exception()`来判断是否是由于exception而导致的scope exit。

|      | 原型                                  | 版本                                                         |
| ---- | ------------------------------------- | ------------------------------------------------------------ |
| (1)  | `bool uncaught_exception() throw();`  | (until C++11)                                                |
|      | `bool uncaught_exception() noexcept;` | (since C++11)<br/>(deprecated in C++17)<br/>(removed in C++20) |
| (2)  | `int uncaught_exceptions() noexcept;` | (since C++17)                                                |

1) Detects if the current thread has a live exception object, that is, an exception has been thrown or rethrown and not yet entered a matching catch clause, [std::terminate](https://en.cppreference.com/w/cpp/error/terminate) or [std::unexpected](https://en.cppreference.com/w/cpp/error/unexpected). In other words, `std::uncaught_exception` detects if [stack unwinding](https://en.cppreference.com/w/cpp/language/throw#Stack_unwinding) is currently in progress.

> NOTE: 读懂了前面的NOTE，就能够理解上面这段话。

2) Detects how many exceptions in the current thread have been thrown or rethrown and not yet entered their matching catch clauses.

Sometimes it's safe to throw an exception even while `std::uncaught_exception() == true`. For example, if [stack unwinding](https://en.cppreference.com/w/cpp/language/throw#Stack_unwinding) causes an object to be destructed, the destructor for that object could run code that throws an exception as long as the exception is caught by some catch block before escaping the destructor.

> NOTE: 这段话要如何理解？
>
> 理解这段话的前提是: 
>
> 1) C++ try...catch... 是nesting关系；
>
> 2) 上面这段话中的stack unwinding指的是exception stack unwinding。
>
> `std::uncaught_exception() == true`表示当前有exception没有被`catch`，那么第一段话的意思是: 尽管当前有exception没有被`catch`，依然可以throw an exception；
>
> 第二段话，则结合一个具体的例子来对第一段话中的内容进行解释: 在destructor中，可以throw an exception，只要这个exception在escaping the destructor之前被`catch`。

### Notes

An example where int-returning `uncaught_exceptions` is used is the [boost.log](http://www.boost.org/doc/libs/release/libs/log/doc/html/index.html) library: the expression `BOOST_LOG(logger) << foo();` first creates a guard object and records the number of uncaught exceptions in its constructor. The output is performed by the guard object's destructor unless `foo()` throws (in which case the number of uncaught exceptions in the destructor is greater than what the constructor observed)

> NOTE: 上面所描述的其实是scope guard。在下面的例子中，就演示了这种技巧:

### Example

```C++
#include <iostream>
#include <exception>
#include <stdexcept>

struct Foo
{
	int count = std::uncaught_exceptions();
	~Foo()
	{
		std::cout << (count == std::uncaught_exceptions() ? "~Foo() called normally\n" : "~Foo() called during stack unwinding\n");
	}
};
int main()
{
	Foo f;
	try
	{
		Foo f;
		std::cout << "Exception thrown\n";
		throw std::runtime_error("test exception");
	} catch (const std::exception &e)
	{
		std::cout << "Exception caught: " << e.what() << '\n';
	}
}
// g++ --std=c++17 test.cpp

```


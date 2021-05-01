# 关于本章

使用C++ thread support library进行asynchronous programming。

## cppreference [Thread support library # Futures](https://en.cppreference.com/w/cpp/thread#Futures)

The standard library provides facilities to obtain values that are returned and to catch exceptions that are thrown by **asynchronous tasks** (i.e. functions launched in separate threads). These values are communicated in a *shared state*, in which the **asynchronous task** may write its return value or store an exception, and which may be examined, waited for, and otherwise manipulated by other threads that hold instances of [std::future](https://en.cppreference.com/w/cpp/thread/future) or [std::shared_future](https://en.cppreference.com/w/cpp/thread/shared_future) that reference that shared state.

> NOTE: 
>
> 上面这段话是对C++ asynchronous programming的总结，在 [std::future](https://en.cppreference.com/w/cpp/thread/future) 中，有着类似的、更加详细的说明。
>
> 



## Mode: asynchronous task and `std::future`

从cppreference [Thread support library # Futures](https://en.cppreference.com/w/cpp/thread#Futures) 中的内容可以看出，C++为asynchronous programming提供了非常统一、抽象的使用模式: 

1 从下面的内容可知，C++支持多种方式来实现asynchronous task

2 [std::future](https://en.cppreference.com/w/cpp/thread/future) 是获取这些asynchronous task的return value的统一方式，显然 `std::future` 是C++ asynchronous programming的核心所在

3 `std::promise`、`std::future`都是操作shared state的facility。

### Asynchronous task 

在 cppreference [Thread support library # Futures](https://en.cppreference.com/w/cpp/thread#Futures) 中这样介绍asynchronous task:

> The standard library provides facilities to obtain values that are returned and to catch exceptions that are thrown by **asynchronous tasks** (i.e. functions launched in separate threads).

在 cppreference [std::future](https://en.cppreference.com/w/cpp/thread/future) 中，这样介绍asynchronous task:

> The class template `std::future` provides a mechanism to access the result of asynchronous operations:
>
> - An asynchronous operation (created via [std::async](https://en.cppreference.com/w/cpp/thread/async), [std::packaged_task](https://en.cppreference.com/w/cpp/thread/packaged_task), or [std::promise](https://en.cppreference.com/w/cpp/thread/promise)) can provide a `std::future` object to the creator of that asynchronous operation.

从上面的描述可以看出，C++支持如下方式的Asynchronous task:

1 [std::async](https://en.cppreference.com/w/cpp/thread/async)

2 [std::packaged_task](https://en.cppreference.com/w/cpp/thread/packaged_task)

3 [std::promise](https://en.cppreference.com/w/cpp/thread/promise)

在 cppreference [std::future](https://en.cppreference.com/w/cpp/thread/future) 中，给出了包含上述三种 asynchronous task 的example。

## TODO

ncona [Futures, async, packaged_tasks and promises in C++](https://ncona.com/2018/02/futures-async-packaged_tasks-and-promises-in-c/)
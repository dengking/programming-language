# Wrapper

实现方式：

| 实现方式                  | 章节               |      |
| ------------------------- | ------------------ | ---- |
| function object           | `Function-objects` |      |
| template function wrapper |                    |      |
| function pointer          |                    |      |



## Template function wrapper

### stackoverflow [c++11: Templated wrapper function](https://stackoverflow.com/questions/25495448/c11-templated-wrapper-function)

I try to create a general wrapper function which takes any function as argument and also their parameters. Just something like the `std::thread` constructor.

```C++
#include <iostream>

using namespace std;

template<typename FUNCTION, typename ... ARGS>
void wrapper(FUNCTION &&func, ARGS &&... args)
{
	cout << "WRAPPER: BEFORE" << endl;
	auto res = func(args...);
	cout << "WRAPPER: AFTER" << endl;
	//return res;
}

int dummy(int a, int b)
{
	cout << a << '+' << b << '=' << (a + b) << endl;
	return a + b;
}

int main(void)
{
	dummy(3, 4);
	wrapper(dummy, 3, 4);
}
// g++ --std=c++11 test.cpp
```

> NOTE: 上述代码是无法实现perfect forwarding的，正如评论所言：
>
> You should be passing like this: `func(std::forward<Args>(args)...)` – [CoffeeandCode](https://stackoverflow.com/users/2297448/coffeeandcode)

The wrapper function itself works. It calls the given function object (`std::function`, functor or just a "normal" function) with the given arguments. But i also like to return its return value.

#### [A](https://stackoverflow.com/a/25495463) 

C++11: `std::result_of` + `std::forward`

Use [`std::result_of`](http://en.cppreference.com/w/cpp/types/result_of) :

```c++
#include <iostream>
#include <type_traits> // std::result_of
using namespace std;

template<typename F, typename ...Args>
typename std::result_of<F&& (Args&&...)>::type wrapper(F &&f, Args &&... args)
{
	return std::forward<F>(f)(std::forward<Args>(args)...);
}

int dummy(int a, int b)
{
	cout << a << '+' << b << '=' << (a + b) << endl;
	return a + b;
}

int main(void)
{
	dummy(3, 4);
	wrapper(dummy, 3, 4);
}
// g++ --std=c++11 test.cpp
```

C++14:

In C++14 you can use the `result_of_t` alias:

```cpp
template <typename F, typename ...Args>
std::result_of_t<F &&(Args &&...)> wrapper(F && f, Args &&... args)
{
    return std::forward<F>(f)(std::forward<Args>(args)...);
}
```

Or you can use return type deduction:

```cpp
template <typename F, typename ...Args>
decltype(auto) wrapper(F && f, Args &&... args)
{
    std::cout << "before\n";
    auto && res = std::forward<F>(f)(std::forward<Args>(args)...);
    std::cout << "after\n";
    return res;
}
```

> In C++14 you can also forget about `result_of` and friends completely and use return type deduction :) 

#### [A](https://stackoverflow.com/a/25495501)

You can use `decltype` with the C++11 auto trailing return type :

```cpp
#include <iostream>

using namespace std;

template<typename FUNCTION, typename... ARGS>
auto wrapper(FUNCTION&& func, ARGS&&... args) -> decltype(func(std::forward<ARGS>(args)...))
{
    cout << "WRAPPER: BEFORE" << endl;
    auto res = func(args...);
    cout << "WRAPPER: AFTER" << endl;
    return res;
}

int dummy(int a, int b)
{
    cout << a << '+' << b << '=' << (a + b) << endl;
    return a + b;
}

int main(void)
{
    dummy(3, 4);
    cout << "WRAPPERS RES IS: " << wrapper(dummy, 3, 4) << endl;
}
```

[**Live demo**](http://coliru.stacked-crooked.com/a/de9cc92d8d305070)

------

And in **C++14**, simply do :

```cpp
template<typename FUNCTION, typename... ARGS>
decltype(auto) wrapper(FUNCTION&& func, ARGS&&... args)
```

[**Live demo**](http://coliru.stacked-crooked.com/a/158fd792b0972671)



## stackoverflow [how to write a C++ class member function wrapper?](https://stackoverflow.com/questions/48355723/how-to-write-a-c-class-member-function-wrapper)

按照[回答](https://stackoverflow.com/a/48355758)中给出的思路：

There are two ways to solve this:

1. Use [`std::bind`](http://en.cppreference.com/w/cpp/utility/functional/bind). As in `std::bind(&A::c, a)`
2. Use a [lambda](http://en.cppreference.com/w/cpp/language/lambda). As in `[&a](int i, std::string const& s) { return a.c(i, s); }`

原文中的实现，没有perfect forwarding，下面是结合stackoverflow [c++11: Templated wrapper function](https://stackoverflow.com/questions/25495448/c11-templated-wrapper-function) 中的写法

### `std::result_of` + `std::forward` + lambda

```C++
#include <iostream>
#include <string>
#include <functional>
#include <type_traits> // std::result_of
using namespace std;
class A
{
public:
	A()
	{

	}
	A(int i)
	{

	}
	void connect()
	{
	}
	void close()
	{
	}
	template<typename F, typename ... Args>
	auto wrapper(F &&f, Args &&... args) -> typename std::result_of<F &&(Args&&...)>::type
	{
		using return_type = typename std::result_of<F &&(Args&&...)>::type;
		connect();
		return_type ret = std::forward < F > (f)(std::forward<Args>(args)...);
		close();
		return ret;
	}
	bool c(int a, string b)
	{
		return true;
	}
	string c(string b)
	{
		return b;
	}
	bool r(int a, string b)
	{
		return true;
	}
};
int main()
{
	A a;
	a.connect();
	a.c(1, "abc");
	a.close(); // equal to a.wrapper(a.c, 1, "abc"); but compling error, how to write it correctly?

	auto clbk = [&](int p1, string p2) -> bool {	return a.c(p1, p2);};
	bool ret = a.wrapper(clbk, 1, "abc");
	std::cout << a.wrapper(clbk, 1, "abc");
}
// g++ --std=c++11 test.cpp 

```



### `std::result_of` + `std::forward` + `std::bind`

```c++
#include <iostream>
#include <string>
#include <type_traits> // std::result_of
#include <functional>
using namespace std::placeholders;
// for _1, _2, _3...
using namespace std;
using namespace std;
class A
{
public:
	A()
	{

	}
	A(int i)
	{

	}
	void connect()
	{
	}
	void close()
	{
	}
	template<typename F, typename ... Args>
	auto wrapper(F &&f, Args &&... args) -> typename std::result_of<F &&(Args&&...)>::type
	{
		using return_type = typename std::result_of<F &&(Args&&...)>::type;
		connect();
		return_type ret = std::forward < F > (f)(std::forward<Args>(args)...);
		close();
		return ret;
	}
	bool c(int a, string b)
	{
		return true;
	}
	string c(string b)
	{
		return b;
	}
	bool r(int a, string b)
	{
		return true;
	}
};
int main()
{
	A a;
	a.connect();
	a.c(1, "abc");
	a.close(); // equal to a.wrapper(a.c, 1, "abc"); but compling error, how to write it correctly?

	std::cout << a.wrapper(std::bind(&A::c, a), 1, "abc");
}
// g++ --std=c++11 test.cpp 

```

编译报错如下:

```c++
test.cpp: 在函数‘int main()’中:
test.cpp:55:43: 错误：对‘bind(<未解决的重载函数类型>, A&)’的调用没有匹配的函数
  std::cout << a.wrapper(std::bind(&A::c, a), 1, "abc");
                                           ^
test.cpp:55:43: 附注：备选是：
In file included from test.cpp:4:0:
/usr/include/c++/4.8.2/functional:1655:5: 附注：template<class _Func, class ... _BoundArgs> typename std::_Bind_helper<std::__or_<std::is_integral<typename std::decay<_Tp>::type>, std::is_enum<typename std::decay<_Tp>::type> >::value, _Func, _BoundArgs ...>::type std::bind(_Func&&, _BoundArgs&& ...)
     bind(_Func&& __f, _BoundArgs&&... __args)
     ^
/usr/include/c++/4.8.2/functional:1655:5: 附注：  template argument deduction/substitution failed:
test.cpp:55:43: 附注：  couldn't deduce template parameter ‘_Func’
  std::cout << a.wrapper(std::bind(&A::c, a), 1, "abc");
                                           ^
In file included from test.cpp:4:0:
/usr/include/c++/4.8.2/functional:1682:5: 附注：template<class _Result, class _Func, class ... _BoundArgs> typename std::_Bindres_helper<_Result, _Func, _BoundArgs>::type std::bind(_Func&&, _BoundArgs&& ...)
     bind(_Func&& __f, _BoundArgs&&... __args)
     ^
/usr/include/c++/4.8.2/functional:1682:5: 附注：  template argument deduction/substitution failed:
test.cpp:55:43: 附注：  couldn't deduce template parameter ‘_Result’
  std::cout << a.wrapper(std::bind(&A::c, a), 1, "abc");

```

按照[回答](https://stackoverflow.com/a/48368717)中的写法：

```C++
#include <iostream>
#include <string>
#include <type_traits> // std::result_of
#include <functional>
using namespace std::placeholders;
// for _1, _2, _3...
using namespace std;
using namespace std;
class A
{
public:
	A()
	{

	}
	A(int i)
	{

	}
	void connect()
	{
	}
	void close()
	{
	}
	template<typename F, typename ... Args>
	auto wrapper(F &&f, Args &&... args) -> typename std::result_of<F &&(Args&&...)>::type
	{
		using return_type = typename std::result_of<F &&(Args&&...)>::type;
		connect();
		return_type ret = std::forward < F > (f)(std::forward<Args>(args)...);
		close();
		return ret;
	}
	bool c(int a, string b)
	{
		return true;
	}
	string c(string b)
	{
		return b;
	}
	bool r(int a, string b)
	{
		return true;
	}
};
int main()
{
	A a;
	a.connect();
	a.c(1, "abc");
	a.close(); // equal to a.wrapper(a.c, 1, "abc"); but compling error, how to write it correctly?

	using sig1 = bool(A::*)(int, string);
	cout << "result of a is: " << (a.wrapper(bind(static_cast<sig1>(&A::c), a, _1, _2), 1, string("abc")) ? "successful" : "fail") << endl;
}
// g++ --std=c++11 test.cpp 

```



## Timeit

度量函数的耗时，是各种programming language中的一个任务，本文描述C++中如何来实现这个需求。

参考如下:

- stackoverflow [Measuring execution time of a function in C++](https://stackoverflow.com/questions/22387586/measuring-execution-time-of-a-function-in-c)
- stackoverflow [How to Calculate Execution Time of a Code Snippet in C++](https://stackoverflow.com/questions/1861294/how-to-calculate-execution-time-of-a-code-snippet-in-c) 
- https://stackoverflow.com/questions/48355723/how-to-write-a-c-class-member-function-wrapper

file:///E:/github/dengking/programming-language-theory/programming-language/docs/C-family-language/C-and-C++/cppreference/reference/en/cpp/types/result_of.html

https://en.cppreference.com/w/cpp/utility/functional/bind

https://en.cppreference.com/w/cpp/utility/functional/mem_fn



https://codereview.stackexchange.com/questions/180221/c-member-function-wrapper-for-interfacing-with-c-apis

https://codereview.stackexchange.com/questions/148824/simple-wrapper-for-member-function-pointers-with-known-signature





## Generic forwarding function template

在`C++\Language-reference\Basic-concept\Type-system\Type-inference`章节也对这个问题进行了探讨。

需要总结C++在此之上的发展:

C++11: 

1) perfect forwarding

2) `decltype`

3)  *[trailing-return-type](https://en.wikipedia.org/wiki/Trailing-return-type)* 



C++14:

1) `decltype(auto)`
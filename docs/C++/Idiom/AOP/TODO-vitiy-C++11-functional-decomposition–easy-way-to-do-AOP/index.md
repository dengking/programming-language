

# vitiy [C++11 functional decomposition – easy way to do AOP](http://vitiy.info/c11-functional-decomposition-easy-way-to-do-aop/)

This post is about making **functional decomposition** from perspective of [Aspect Oriented Programming](http://en.wikipedia.org/wiki/Aspect-oriented_programming) using C++11.

[![Old picture about AOP](http://vitiy.info/wp-content/uploads/2015/02/aop_1.jpg)](http://vitiy.info/wp-content/uploads/2015/02/aop_1.jpg)



> NOTE: **functional decomposition** 的含义是 "功能分解"

```C++
#include <iostream>
#include <functional>

#define LOG std::cout
#define NL std::endl
auto plus = [](int a, int b) {	LOG << a + b << NL;};                      // the body of the function ends

template<typename ...Args>
std::function<void(Args...)> wrapLog(std::function<void(Args...)> f)
{
	return [f](Args ... args)
	{
		LOG << "start" << NL;
		f(args...);
		LOG << "finish" << NL;
	};
}

int main()
{
	auto loggedPlus = wrapLog(static_cast<std::function<void(int, int)>>(plus));
	loggedPlus(2, 3);
	return 0;
}
// g++ --std=c++11 test.cpp


```

> NOTE:
>
> 输出为:
>
> ```C++
> start
> 5
> finish
> ```
>
> 



```C++
#include <iostream>
#include <functional>

#define LOG std::cout
#define NL std::endl
auto plus = [](int a, int b) { LOG << a + b << NL;};                      // the body of the function ends

template<typename ...Args>
std::function<void(Args...)> wrapLog(std::function<void(Args...)> f)
{
	return [f](Args ... args)
	{
		LOG << "start" << NL;
		f(args...);
		LOG << "finish" << NL;
	};
}

template<typename Function>
struct function_traits: public function_traits<decltype(&Function::operator())>
{
};

template<typename ClassType, typename ReturnType, typename ... Args>
struct function_traits<ReturnType (ClassType::*)(Args...) const >
{
	typedef ReturnType (*pointer)(Args...);
	typedef std::function<ReturnType(Args...)> function;
};

template<typename Function>
typename function_traits<Function>::function to_function(Function &lambda)
{
	return typename function_traits<Function>::function(lambda);
}

int main()
{
	auto loggedPlus = wrapLog(to_function(plus));
	loggedPlus(2, 3);
	return 0;
}
// g++ --std=c++11 test.cpp


```


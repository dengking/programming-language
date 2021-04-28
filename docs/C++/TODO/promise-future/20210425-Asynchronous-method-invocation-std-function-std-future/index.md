# stackoverflow [Why is GCC producing a strange error and trying to call the wrong method when template arguments are specified explicitly?](https://stackoverflow.com/questions/12900578/why-is-gcc-producing-a-strange-error-and-trying-to-call-the-wrong-method-when-te)



```C++
#include<iostream>
#include<memory>
#include<utility>
#include <functional>
#include <future>

using namespace std;

template<typename Ret, typename ... Args>
future<Ret> submitAsync(const function<Ret(Args...)> &func, Args &&... args);
{
	func(std::forward<Arsg>(args)...);
	return future<Ret>{};
}
int main()
{
	auto F = [&](int a)
	{	std::cout<<a<<std::endl;};
	submitAsync(F, 1);
}
// g++ test.cpp -pedantic -Wall -Wextra --std=c++11

```



```C++
test.cpp:11:1: error: expected unqualified-id before '{' token
 {
 ^
test.cpp: In function 'int main()':
test.cpp:20:18: error: no matching function for call to 'submitAsync(main()::<lambda(int)>&, int)'
  submitAsync(F, 1);
                  ^
test.cpp:10:13: note: candidate: 'template<class Ret, class ... Args> std::future<_Res> submitAsync(const std::function<_Res(_ArgTypes ...)>&, Args&& ...)'
 future<Ret> submitAsync(const function<Ret(Args...)> &func, Args &&... args);
             ^~~~~~~~~~~
test.cpp:10:13: note:   template argument deduction/substitution failed:
test.cpp:20:18: note:   'main()::<lambda(int)>' is not derived from 'const std::function<_Res(_ArgTypes ...)>'
  submitAsync(F, 1);

```


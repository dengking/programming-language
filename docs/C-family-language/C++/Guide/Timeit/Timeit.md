# Timeit

度量函数的耗时，是各种programming language中的一个任务，本文描述C++中如何来实现这个需求。

参考如下:

- stackoverflow [Measuring execution time of a function in C++](https://stackoverflow.com/questions/22387586/measuring-execution-time-of-a-function-in-c)
- stackoverflow [How to Calculate Execution Time of a Code Snippet in C++](https://stackoverflow.com/questions/1861294/how-to-calculate-execution-time-of-a-code-snippet-in-c)





```c++
#include <iostream>
#include <string>
#include <functional>
using namespace std::placeholders;
// for _1, _2, _3...
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
	;
	void close()
	{
	}
	;
	template<typename F, typename ... Args>
	auto wrapper(F &&f, Args &&... args) -> typename std::result_of<F(Args...)>::type
	{
		using return_type = typename std::result_of<F(Args...)>::type;
		connect();
		return_type ret = f(args...);
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
	auto clbk = [&](int p1, string p2)->bool { return a.c(p1, p2); };
	cout << "result of a is: " << a.wrapper(clbk, 1, "abc") ? "successful" : "fail" << endl;
//	using sig1 = bool(A::*)(int, string);
//	cout << "result of another a is: " << a.wrapper(std::bind(static_cast<sig1>(&A::c, a)), "abc") << endl;
	using sig1 = bool(A::*)(int, string);
	cout << "result of a is: " << (a.wrapper(bind(static_cast<sig1>(&A::c), a, _1, _2), 1, string("abc")) ? "successful" : "fail") << endl;
//	cout << "result of r is:" << a.wrapper(std::bind(&A::r, a), 1, "abc") << endl;
}

```

file:///E:/github/dengking/programming-language-theory/programming-language/docs/C-family-language/C-and-C++/cppreference/reference/en/cpp/types/result_of.html

https://en.cppreference.com/w/cpp/utility/functional/bind

https://en.cppreference.com/w/cpp/utility/functional/mem_fn

https://stackoverflow.com/questions/48355723/how-to-write-a-c-class-member-function-wrapper

https://codereview.stackexchange.com/questions/180221/c-member-function-wrapper-for-interfacing-with-c-apis

https://codereview.stackexchange.com/questions/148824/simple-wrapper-for-member-function-pointers-with-known-signature
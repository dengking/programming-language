# Construct On First Use



## [More C++ Idioms/Construct On First Use](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Construct_On_First_Use)

### Motivation

**Static objects** that have **non-trivial constructors** must be initialized before they are used. It is possible to access an uninitialized **non-local static object** before its initialization if proper care is not exercised.

> NOTE: **non-trivial constructors**

```c++
#include <iostream>
using namespace std;
struct Bar
{
	Bar()
	{
		cout << "[" << __FILE__ << "]" << "[" << __LINE__ << "]" << __PRETTY_FUNCTION__ << std::endl;
	}
	void f()
	{
		cout << "[" << __FILE__ << "]" << "[" << __LINE__ << "]" << __PRETTY_FUNCTION__ << std::endl;
	}
};
struct Foo
{
	Foo()
	{
		bar_.f();
	}
	static Bar bar_;
};

Foo f; // 会调用`Foo()`，进而调用 `f()`

Bar Foo::bar_; // definition of static member bar_，此时会调用`Bar()`

int main()
{
	cout << "[" << __FILE__ << "]" << "[" << __LINE__ << "]" <<  __PRETTY_FUNCTION__ << std::endl;
}
// g++ -Wall -pedantic test.cpp
```

> NOTE: 
>
> 1、输出如下：
>
> ```
> [test.cpp][11]void Bar::f()
> 
> [test.cpp][7]Bar::Bar()
> 
> [test.cpp][29]int main()
> ```
>
> 2、这是典型的`static` initialization order ‘fiasco’ (problem)
>
> 显然，在构造函数执行之前，就已经执行了函数`f()`，显然这是错误的。
>
> 静态成员，内存应该已经分配了，直到运行的时候，才调用构造函数对它进行初始化。

### Solution and Sample Code

> NOTE: 
>
> 1、下面的solution，说白了其实就是使用singleton来替代static global

#### Construct on first use using dynamic allocation

```C++
#include <iostream>
using namespace std;
struct Bar
{
	Bar()
	{
		cout << "[" << __FILE__ << "]" << "[" << __LINE__ << "]" <<  __PRETTY_FUNCTION__ << std::endl;
	}
	void f()
	{
		cout << "[" << __FILE__ << "]" << "[" << __LINE__ << "]" <<  __PRETTY_FUNCTION__ << std::endl;
	}
};
struct Foo
{
	Foo()
	{
		bar().f();
	}
	Bar& bar()
	{
        cout << "[" << __FILE__ << "]" << "[" << __LINE__ << "]" <<  __PRETTY_FUNCTION__ << std::endl;
		static Bar *b = new Bar();
		return *b;
	}
};

Foo f; // 会调用`Foo()`，进而调用 `Bar()`

int main()
{
	cout << "[" << __FILE__ << "]" << "[" << __LINE__ << "]" <<  __PRETTY_FUNCTION__ << std::endl;
}
// g++ -Wall -pedantic test.cpp
```

> NOTE: 
>
> 输出如下:
>
> ```C++
> [test.cpp][22]Bar& Foo::bar()
> 
> [test.cpp][7]Bar::Bar()
> 
> [test.cpp][11]void Bar::f()
> 
> [test.cpp][32]int main()
> ```
>
> 

#### Construct on first use using local static

```C++
#include <iostream>
using namespace std;
struct Bar
{
	Bar()
	{
		cout << "[" << __FILE__ << "]" << "[" << __LINE__ << "]" <<  __PRETTY_FUNCTION__ << std::endl;
	}
	void f()
	{
		cout << "[" << __FILE__ << "]" << "[" << __LINE__ << "]" <<  __PRETTY_FUNCTION__ << std::endl;
	}
};
struct Foo
{
	Foo()
	{
		bar().f();
	}
	Bar& bar()
	{
		static Bar b;
		return b;
	}
};

Foo f; // 会调用`Foo()`，进而调用 `Bar()`

int main()
{
	cout << "[" << __FILE__ << "]" << "[" << __LINE__ << "]" <<  __PRETTY_FUNCTION__ << std::endl;
}
// g++ -Wall -pedantic test.cpp

```

> NOTE: 输出如下:
>
> ```C++
> [test.cpp][7]Bar::Bar()
> 
> [test.cpp][11]void Bar::f()
> 
> [test.cpp][31]int main()
> ```
>
> 



## 素材

stackoverflow [Construct on first use + forced initialization to solve static initialization order fiasco?](https://stackoverflow.com/questions/25122575/construct-on-first-use-forced-initialization-to-solve-static-initialization-or) 

parashift [Construct on first use](http://www.parashift.com/c++-faq/construct-on-first-use-v2.html)
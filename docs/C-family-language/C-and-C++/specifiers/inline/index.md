# inline

首先描述inline的含义，然后再来描述各种用法。

`inline`是C family language中非常常见的specifier，本文对它进行总结，下面是参考内容: 

1) cppreference [`inline` specifier](https://en.cppreference.com/w/cpp/language/inline)

2) cppreference [C++ keywords: inline](https://en.cppreference.com/w/cpp/keyword/inline)

3) cppreference [Namespaces#Inline namespaces](https://en.cppreference.com/w/cpp/language/namespace#Inline_namespaces)

下面以programming paradigm来对inline的用法进行总结: 

## Non-OOP

### inline function

这种情况下，`inline`的含义是"内联"，它是一种compiler optimization技术。

参见:

1) cppreference [`inline` specifier](https://en.cppreference.com/w/cpp/language/inline)

2) `C-and-C++\From-source-code-to-exec\Compile\Optimization\inline`章节

### inline variable

参见:

1) cppreference [`inline` specifier](https://en.cppreference.com/w/cpp/language/inline)

## OOP



## inline definition

本节标题是我根据"defined inline"重新定义的，"defined inline"是在阅读 cppreference [Non-static member functions](https://en.cppreference.com/w/cpp/language/member_functions) 时发现的，下面结合 cppreference [Non-static member functions](https://en.cppreference.com/w/cpp/language/member_functions) 给出的例子来理解它的含义: 

```C++
class S
{
	int mf1(); // non-static member function declaration
	int mf4() const // can be defined inline
	{
		return data;
	}
	int data;
};
int S::mf1() // if not defined inline, has to be defined at namespace
{
	return 7;
}

```

通过上述例子我们可以理解 "defined inline" 的含义，显然: 

1) inline defined: 

[`inline` specifier](https://en.cppreference.com/w/cpp/language/inline): 

> A function defined entirely inside a [class/struct/union definition](https://en.cppreference.com/w/cpp/language/classes), whether it's a member function or a non-member friend function, is implicitly an **inline function**.



2) not defined inline: declaration 和 definition 是分隔开来的，一般的做法是: `.h`文件中存放declaration，`.cpp`文件中存放definition。





目前只有static member才能够inline，所以在`C++\Language-reference\Classes\Members\Static-member`对OOP中的`inline`相关的内容进行了总结。

inline member data

inline member function 

## inline namespace

参见: cppreference [Namespaces#Inline namespaces](https://en.cppreference.com/w/cpp/language/namespace#Inline_namespaces) 。











#### inline

关于member function的inline规则，参见`inline` specifier，其中有这样的描述:

> A function defined entirely inside a [class/struct/union definition](https://en.cppreference.com/w/cpp/language/classes), whether it's a **member function** or a **non-member friend function**, is implicitly an inline function.

这样的规则的优势是: 使得C++ header only library成为现实。一个问题是: 这些member function，是否是同一个？

下面是对比的例子: 

`test.h`

```C++
#ifndef TEST_H_
#define TEST_H_

#include <iostream>

struct X
{
	static void f();

};

void X::f()
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}

#endif /* TEST_H_ */

```

非`inline`。

`test2.cpp`

```C++
#include "test.h"

void f()
{
	X::f();  // X::f is a qualified name of static member function
}

```

`test.cpp`

```C++
#include "test.h"

X g()
{
	return X();
} // some function returning X



int main()
{
	g().f();
}

```



编译: `g++ test.cpp test2.cpp` ，报错如下:

```C++
/tmp/ccFetxpp.o：在函数‘X::f()’中：
test2.cpp:(.text+0x0): multiple definition of `X::f()'
/tmp/ccRczaCY.o:test.cpp:(.text+0x0)：第一次在此定义
collect2: 错误：ld 返回 1

```



`test.h`

```C++
#ifndef TEST_H_
#define TEST_H_

#include <iostream>

struct X
{
	static void f()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

};

void X::f()


#endif /* TEST_H_ */

```

`test2.cpp`

```C++
#include "test.h"

void f()
{
	X::f();  // X::f is a qualified name of static member function
}

```

`test.cpp`

```C++
#include "test.h"

X g()
{
	return X();
} // some function returning X



int main()
{
	g().f();
}

```

编译: `g++ test.cpp test2.cpp` ，正确运行。
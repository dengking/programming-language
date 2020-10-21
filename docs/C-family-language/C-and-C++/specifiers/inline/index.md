# inline

首先描述inline的含义，然后再来描述各种用法。

`inline`是C family language中非常常见的specifier，本文对它进行总结，下面是参考内容: 

1) cppreference [`inline` specifier](https://en.cppreference.com/w/cpp/language/inline)

2) cppreference [C++ keywords: inline](https://en.cppreference.com/w/cpp/keyword/inline)

3) cppreference [Namespaces#Inline namespaces](https://en.cppreference.com/w/cpp/language/namespace#Inline_namespaces)



## Semantic of `inline` keyword 

首先理解inline的语义，本节参考: 1) cppreference [`inline` specifier#Explanation](https://en.cppreference.com/w/cpp/language/inline) : 

### "inlining is preferred": inline function

The original intent of the `inline` keyword was to serve as an indicator to the optimizer that [inline substitution of a function](https://en.wikipedia.org/wiki/inline_expansion) is preferred over function call, that is, instead of executing the function call CPU instruction to transfer control to the function body, a copy of the function body is executed without generating the call. This avoids overhead created by the function call (passing the arguments and retrieving the result) but it may result in a larger executable as the code for the function has to be repeated multiple times.

Since **this meaning** of the keyword inline is non-binding, compilers are free to use inline substitution for any function that's not marked inline, and are free to generate function calls to any function marked inline. Those optimization choices do not change the rules regarding **multiple definitions** and shared statics listed above.

> NOTE: 上面这段话的意思是: `inline`只是一种hint，即建议"inlining is preferred"，它最终是由compiler来决定是否进行optimization。

### "multiple definitions are permitted": inline variable (since C++17)

Because the meaning of the keyword `inline` for functions came to mean "multiple definitions are permitted" rather than "inlining is preferred", that meaning was extended to variables.

> 因为函数的关键字“inline”的意思变成了“允许多重定义”，而不是“内联是首选”，这个意思被扩展到了变量。



下面以programming paradigm来对inline的用法进行总结: 

## Non-OOP

### Inline function

#### 1) `inline` function

这种情况下，`inline`的含义是"内联"，它是一种compiler optimization技术。

参见:

- cppreference [`inline` specifier](https://en.cppreference.com/w/cpp/language/inline)

- `C-and-C++\From-source-code-to-exec\Compile\Optimization\inline`章节

下面依据linkage对它进行进一步分类:

inline function with [external linkage](https://en.cppreference.com/w/cpp/language/storage_duration)



inline function with [internal linkage](https://en.cppreference.com/w/cpp/language/storage_duration) (e.g. not declared static)

在cppreference [`inline` specifier](https://en.cppreference.com/w/cpp/language/inline)中，有这样的解释:

> An inline function or variable (since C++17) with [external linkage](https://en.cppreference.com/w/cpp/language/storage_duration) (e.g. not declared static) has the following additional properties:
>
> 1. There may be [more than one definition](https://en.cppreference.com/w/cpp/language/definition#One_Definition_Rule) of an inline function or variable (since C++17) in the program as long as each definition appears in a different translation unit and (for non-static inline functions and variables (since C++17)) all definitions are identical. For example, an inline function or an inline variable (since C++17) may be defined in a header file that is #include'd in multiple source files.
> 2. It must be declared inline in every translation unit.
> 3. It has the **same address** in every translation unit.



#### 2) `constexpr` function

cppreference [`inline` specifier](https://en.cppreference.com/w/cpp/language/inline): 

> A function declared constexpr is implicitly an inline function.

这种情况确实是非常容易理解的，因为`constexpr` function是在compile-time执行的，compiler应该是不会把它编译到object file中。

### Inline variable: namespace-scope variable 

cppreference [`inline` specifier](https://en.cppreference.com/w/cpp/language/inline):

> The inline specifier, when used in a [decl-specifier-seq](https://en.cppreference.com/w/cpp/language/declarations#Specifiers) of a variable with **static storage duration** (**static class member** or **namespace-scope variable**), declares the variable to be an *inline variable*.



## OOP



### Inline definition

本节标题是我根据"defined inline"而起的，"defined inline"是在阅读 cppreference [Non-static member functions](https://en.cppreference.com/w/cpp/language/member_functions) 时发现的，下面结合 cppreference [Non-static member functions](https://en.cppreference.com/w/cpp/language/member_functions) 给出的例子来理解它的含义: 

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

cppreference [`inline` specifier](https://en.cppreference.com/w/cpp/language/inline): 

> A function defined entirely inside a [class/struct/union definition](https://en.cppreference.com/w/cpp/language/classes), whether it's a member function or a non-member friend function, is implicitly an **inline function**.

2) not defined inline: declaration 和 definition 是分隔开来的，一般的做法是: `.h`文件中存放declaration，`.cpp`文件中存放definition。

在`C++\Language-reference\Classes\Members\Static-member`对OOP中的`inline`相关的内容进行了介绍。

#### 1) Friend function and member function

cppreference [`inline` specifier](https://en.cppreference.com/w/cpp/language/inline) : 

> A function defined entirely inside a [class/struct/union definition](https://en.cppreference.com/w/cpp/language/classes), whether it's a member function or a non-member friend function, is implicitly an inline function.

member function包括:

1) non-static member function

2) static member function

#### 3) implicitly-generated member functions and any member function declared as defaulted

cppreference [`inline` specifier](https://en.cppreference.com/w/cpp/language/inline) : 

> The implicitly-generated member functions and any member function declared as defaulted on its first declaration are inline just like any other function defined inside a class definition.



#### 2) Static member variable

cppreference [`inline` specifier](https://en.cppreference.com/w/cpp/language/inline) : 

> The inline specifier, when used in a [decl-specifier-seq](https://en.cppreference.com/w/cpp/language/declarations#Specifiers) of a variable with static storage duration (static class member or namespace-scope variable), declares the variable to be an *inline variable*.A static member variable (but not a namespace-scope variable) declared constexpr is implicitly an inline variable.



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







使用 Unnamed/anonymous namespaces 而不是 static inline function

`test.h`

```C++
#ifndef TEST_H_
#define TEST_H_

#include <iostream>
namespace
{
struct X
{
	static void f();

};

void X::f()
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}
}
#endif /* TEST_H_ */

```

`test.cpp`

```C++
#include "test.h"

void f()
{
	X::f();  // X::f is a qualified name of static member function
}

```

`test2.cpp`

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


# static members



## cppreference [static members](https://en.cppreference.com/w/cpp/language/static)

### Syntax

|      | syntax                   | explanation                        |
| ---- | ------------------------ | ---------------------------------- |
| (1)  | `static data_member`     | Declares a static data member.     |
| (2)  | `static member_function` | Declares a static member function. |

### Explanation



### Declaration and definition

> NOTE: 原文并没有这样的标题，这是我添加上去的，便于理解

The `static` keyword is only used with the declaration of a static member, inside the class definition, but not with the definition of that static member:

```C++
#include <iostream>
class X
{
	static int n; // declaration (uses 'static')
};

int X::n = 1; // definition (does not use 'static')

int main()
{
	std::cout << X::n << std::endl;
}
// g++ test.cpp

```

#### [Incomplete type](https://en.cppreference.com/w/cpp/language/incomplete_type)

```C++
#include <iostream>

struct Foo;
struct S
{
	static int a[]; // declaration, incomplete type
	static Foo x;   // declaration, incomplete type
	static S s;     // declaration, incomplete type (inside its own definition)
};

int S::a[10]; // definition, complete type
struct Foo
{
};

Foo S::x;     // definition, complete type
S S::s;       // definition, complete type

int main()
{
}
// g++ test.cpp

```

### Refer to a static member

```C++
#include <iostream>

struct X
{
	static void f(); // declaration
	static int n;    // declaration
};

int X::n = 7; // definition

void X::f() // definition
{
	n = 1; // X::n is accessible as just n in this scope
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}

X g()
{
	return X();
} // some function returning X

void f()
{
	X::f();  // X::f is a qualified name of static member function
	g().f(); // g().f is member access expression referring to a static member function
}

int main()
{
	f();
}
// g++ test.cpp

```

### Static member functions

Static member functions cannot be virtual, const, or volatile.

The address of a static member function may be stored in a regular [pointer to function](https://en.cppreference.com/w/cpp/language/pointer#Pointers_to_functions), but not in a [pointer to member function](https://en.cppreference.com/w/cpp/language/pointer#Pointers_to_member_functions).

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



### [Constant static members](https://en.cppreference.com/w/cpp/language/static#Constant_static_members)

补充内容：

[C++ Static Const Member Variable](https://cookierobotics.com/032/)

https://accu.org/index.php/journals/442



## inline

## TODO

### c++ static const members

https://cookierobotics.com/032/

https://stackoverflow.com/questions/3531060/how-to-initialize-a-static-const-member-in-c

https://en.cppreference.com/w/cpp/language/static#Constant_static_members

https://stackoverflow.com/questions/29822181/prevent-static-initialization-order-fiasco-c

https://stackoverflow.com/questions/12247912/extern-vs-singleton-class


https://isocpp.org/wiki/faq/ctors#static-init-order


https://isocpp.org/wiki/faq/ctors#construct-on-first-use-v2


https://stackoverflow.com/questions/14495536/how-to-initialize-const-member-variable-in-a-class


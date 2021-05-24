# static members



## cppreference [static members](https://en.cppreference.com/w/cpp/language/static)

### Syntax

|      | syntax                   | explanation                        |
| ---- | ------------------------ | ---------------------------------- |
| (1)  | `static data_member`     | Declares a static data member.     |
| (2)  | `static member_function` | Declares a static member function. |

### Explanation

> NOTE: 下面对原文Explanation中的内容进行组织，添加了标题。

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

> NOTE: 描述如何使用一个

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

Static member functions are not associated with any object. When called, they have no `this` pointer.

Static member functions cannot be `virtual`, `const`, or volatile.

The address of a static member function may be stored in a regular [pointer to function](https://en.cppreference.com/w/cpp/language/pointer#Pointers_to_functions), but not in a [pointer to member function](https://en.cppreference.com/w/cpp/language/pointer#Pointers_to_member_functions).



### Static data members

There is only one instance of the static data member in the entire program with static [storage duration](https://en.cppreference.com/w/cpp/language/storage_duration), unless the keyword [`thread_local`](https://en.cppreference.com/w/cpp/keyword/thread_local) is used, in which case there is one such object per thread with thread storage duration (since C++11).

> NOTE: static data members能够保证唯一性

#### inline static data member (since C++17)

```C++
struct X
{
    inline static int n = 1;
};
```



> NOTE: 这是C++17引入的新特性，它的意图在cppreference [`inline` specifier](https://en.cppreference.com/w/cpp/language/inline)中进行了描述:
>
> "Inline variables eliminate the main obstacle to packaging C++ code as header-only libraries"即"内联变量消除了将c++代码打包为仅头文件库的主要障碍"，简而言之，它是为了促进header-only librariy的。
>
> inline definition of member function早就是C++的标准了，但是inline definition of static data member却没有得到C++标准的**完整**的支持((后面会解释此处为什么说是**完整**的)，所以，再次之前，"packaging C++ code as header-only libraries"其实是存在着一定的挑战的，往往需要programmer采用walkaround策略。
>
> 上面这段话中说: "inline definition of static data member却没有得到C++标准的**完整**的支持"，这是因为在C++17之前，可以通过[Constant static members](https://en.cppreference.com/w/cpp/language/static#Constant_static_members)实现类似的inline definition of static data member。



#### [Constant static members](https://en.cppreference.com/w/cpp/language/static#Constant_static_members)

> NOTE: inline definition

static data member of integral or enumeration type 





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


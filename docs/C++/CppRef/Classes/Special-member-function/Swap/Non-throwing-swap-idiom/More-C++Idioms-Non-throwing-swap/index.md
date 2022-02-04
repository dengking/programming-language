# More C++ Idioms [Non-throwing swap](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-throwing_swap)

> NOTE: 关于non-throwing，参见cppreference [Exceptions](https://en.cppreference.com/w/cpp/language/exceptions)、cppreference [noexcept specifier](https://en.cppreference.com/w/cpp/language/noexcept_spec) 

## Intent

1、To implement an exception safe and efficient swap operation.

2、To provide uniform interface to it to facilitate generic programming.

> NOTE: 
>
> 1、generic programming能够大大提供程序的可维护性，在下面的Solution and Sample Code章节会讨论这个问题。
>
> 2、generic programming是需要uniform、consistent interface的，这是非常重要的，它的`tag-uniform and consistent interface to facilitate generic programming`



## Motivation

> NOTE: 
>
> 1、这一节主要描述问题，即下面的swap的问题

A typical implementation of swap could be given as follows:

```c++
template<class T>
void swap (T &a, T &b)
{
  T temp (a);
  a = b;
  b = temp;
}
```

### Performance

Swapping of two large, complex objects of the same type is inefficient due to acquisition and release of **resources** for the intermediate temporary object.

> NOTE: 上述都是copy construct

### Exception-safety

This generic swap implementation may throw if resources are not available. (Such a behavior does not make sense where in fact no new resources should have been requested in the first place.) Therefore, this implementation cannot be used for the [Copy-and-swap](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Copy-and-swap) idiom.

> NOTE: 
>
> 1、第一段话的解释是：`T temp (a);`是需要acquisition of resources的，所以当系统中resource已经耗尽的情况下，这个语句是会抛出exception的。括号中的意思是：swap是不需要request new resource的；
>
> 2、最后一句话的意思是：[Copy-and-swap](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Copy-and-swap) idiom的要求swap函数是non-throw的

## Solution and Sample Code

### Use [Handle Body](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Handle_Body) idiom

Non-throwing swap idiom uses [Handle Body](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Handle_Body) idiom to achieve the desired effect. The abstraction under consideration is split between two implementation classes. One is **handle** and other one is **body**. The **handle** holds a pointer to a **body** object. The swap is implemented as a simple swap of pointers, which are guaranted to not throw exceptions and it is very efficient as no new resources are acquired or released.

> NOTE: 
>
> 1、显然non-throwing swap相比于普通的swap优势在于无需构造temporary object，而是直接交换pointer，直接交换pointer是not throw exception的。
>
> 2、"Non-throwing swap idiom uses [Handle Body](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Handle_Body) idiom to achieve the desired effect"
>
> 这段话要如何来理解呢? 看了一下[Handle Body](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Handle_Body) idiom，这个idiom的意图不仅仅局限于使用一个pointer来作为member variable，在Non-throwing swap idiom 中，[Handle Body](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Handle_Body) idiom所表示的是使用一个pointer来作为member variable。

```c++
namespace Orange {
class String 
{
    char * str;
  public:
    void swap (String &s) // noexcept
    {
      std::swap (this->str, s.str);
    }
};
}
```



### To facilitate generic programming

Although an efficient and exception-safe `swap` function can be implemented (as shown above) as a member function, non-throwing swap idiom goes further than that for **simplicity**, **consistency**, and to facilitate **generic programming**. An explicit specialization of `std::swap` should be added in the `std` namespace as well as the namespace of the class itself.

```c++
namespace Orange { // namespace of String
  void swap (String & s1, String & s2) // noexcept
  {
    s1.swap (s2);
  }
}
namespace std {
  template <>
  void swap (Orange::String & s1, Orange::String & s2) // noexcept
  {
    s1.swap (s2);
  }
}
```

> NOTE: 
>
> 1、需要注意的是，上面是full specialization

### Common usage styles of `swap`

Adding it in two places takes care of two different common usage styles of swap :

(1) unqualified swap 

(2) fully qualified swap (e.g., `std::swap`). 

When **unqualified `swap`** is used, right swap is looked up using **Koenig lookup** (provided one is already defined). 

> NOTE: `swap(Orange::String, Orange::String)`
>
> **Koenig lookup** 就是 [Argument-dependent name lookup](https://en.wikipedia.org/wiki/Argument-dependent_name_lookup)

If **fully qualified `swap`** is used, Koenig lookup is suppressed and one in the `std` namespace is used instead. It is a very common practice. 

> NOTE: `std::swap(Orange::String, Orange::String)`

### Example

Remaining discussion here uses **fully qualified swap** only. It gives a uniform look and feel because `C++` programmers often use `swap` function in an idiomatic way by fully qualifying it with **std::** as shown below.

```c++
template <class T>
void zoo (T t1, T t2) {
//...
int i1, i2;
std::swap(i1,i2); // note uniformity
std::swap(t1,t2); // Ditto here
}
```

> NOTE: 
>
> 1、"Ditto here"的意思是: 同样在这里

In such a case, the right, efficient implementation of `swap` is chosen when `zoo` is instantiated with `String` class defined earlier. Otherwise, the default `std::swap` function template would be instantiated -- completely defeating the purpose of defining the member `swap` and namespace scope `swap` function. This idiom of defining explicit specialization of swap in `std` namespace is particularly useful in generic programming.

### Cascading use

> NOTE: 
>
> 1、`class UserDefined`的data member `str`的类似是前面的`Orange::String`
>
> 2、下面的例子所展示的其实是连锁反应: `Orange::String`的swap是no throw的，基于它的`UserDefined`的swap也能够实现no throw

Such uniformity in using **non-throwing swap idiom** leads to its cascading （级联） use as given in the example below.

```c++
class UserDefined 
{
    String str;
  public:
    void swap (UserDefined & u) // throw () 
    { 
      std::swap (str, u.str); 
    }
};
namespace std
{
  // Full specializations of the templates in std namespace can be added in std namespace.
  template <>
  void swap (UserDefined & u1, UserDefined & u2) // throw ()
  {
    u1.swap (u2);
  }
}
class Myclass
{
    UserDefined u;
    char * name;
  public:
    void swap (Myclass & m) // throw ()
    {
      std::swap (u, m.u);       // cascading use of the idiom due to uniformity
      std::swap (name, m.name); // Ditto here
    }   
}
namespace std
{
   // Full specializations of the templates in std namespace can be added in std namespace.
   template <> 
   void swap (Myclass & m1, Myclass & m2) // throw ()
   {
     m1.swap (m2);
   }
};
```

>  NOTE: 
>
>  1、上面代码中的`throw ()`其实是`noexcept`含义
>
>  2、需要注意的是，上面是full specialization

Therefore, it is a good idea to define a specialization of `std::swap` for the types that are amenable to an exception safe, efficient swap implementation. The `C++` standard does not currently allow us to add new templates to the `std` namespace, but it does allow us to specialize templates (e.g. `std::swap`) from that namespace and add them back in it.

> NOTE: 
>
> 1、这里所讨论的是extend std

## Caveats

> NOTE:  
>
> 1、本段所讨论的是将user defined type的`swap`函数添加到`std`中的问题，这是extending `std`，这在`C++\Library\Standard-library\Extending-std`中进行了讨论；
>
> 2、对于non-throwing swap idiom，推荐使用[Swap values idiom](https://cpppatterns.com/patterns/swap-values.html)，即下面的solution 1，关于此，参见`C++\Idiom\Templates-and-generic-programming\Swappable`，这种方式的另外一个优势是: 更加简单，可以写更少的代码

Using non-throwing swap idiom for template classes (e.g., `Matrix<T>`) can be a subtle issue. As per the C++98 standard, only the **full specialization** of `std::swap` is allowed to be defined inside `std` namespace for the **user-defined types**. **Partial specializations** or **function overloading** is not allowed by the language. Trying to achieve the similar effect for template classes (e.g., `Matrix<T>`) results into **overloading** of `std::swap` in `std` namepspace, which is technically **undefined behavior**. This is not necessarily the ideal state of affairs as indicated by some people in a spectacularly(非常地) long discussion thread on comp.lang.c++.moderated newsgroup.[[1\]](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-throwing_swap#cite_note-1) There are two possible solutions, both imperfect, to this issue:

> NOTE: 翻译如下:
>
> "对模板类使用非抛出swap习惯用法(例如，' `Matrix<T>` ')可能是一个微妙的问题。
> 根据c++ 98标准，只有' std::swap '的完全特化才允许在用户定义类型的' std '命名空间中定义。
> 语言不允许部分专门化或函数重载。
> 试图在模板类(例如，' Matrix<T> ')中实现类似的效果会导致' std '命名空间中的' std::swap '的重载，这在技术上是未定义的行为。
> 这并不一定是像一些人在“compl .lang.c++”的冗长讨论中指出的那样理想的状态。
> 主持的新闻组。
> 对于这个问题，有两种可能的解决方案，但都不完美:"
>
> 1、上面这段话让我想到了: full specialization、partial specialization、overload之间的关联
>
> 2、"Trying to achieve the similar effect for template classes (e.g., `Matrix<T>`) results into **overloading** of `std::swap` in `std` namepspace" 要如何理解？
>
> `Matrix<T>`是一个template classe，因此，它的写法如下: 
>
> ```C++
> template<typename T>
> void swap(Matrix<T> &l, Matrix<T> &r)
> {
> }
> 
> ```
>
> 
>
> 

1、Standard-compliant solution. Leveraging on **Koenig lookup**(**ADL**), define an overloaded swap function template in the same namespace as that of the class being swapped. Not all compilers may support this correctly, but this solution is compliant to the standard.[[2\]](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-throwing_swap#cite_note-2)

> NOTE: 目前主流的compiler都是支持ADL的

2、Fingers-crossed solution. Partially specialize `std::swap` and ignore the fact that this is technically undefined behavior, hoping that nothing will happen and wait for a fix in the next language standard.

> NOTE: 这种做法相当于什么都没有做







## 完整测试程序



### 使用 [swap value idiom](https://cpppatterns.com/patterns/swap-values.html)

```c++
#include <utility>
#include <cstring>
#include <algorithm>
#include <iostream>

namespace Orange
{
class String
{
	char * str;
	public:
	void swap(String &s) // noexcept
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		std::swap(this->str, s.str);
	}
	String(const char* p)
	{
		size_t size = strlen(p) + 1;
		str = new char[size];
		memcpy(str, p, size);
	}
	friend std::ostream& operator<<(std::ostream& Stream, String& S)
	{
		Stream << S.str;
		return Stream;
	}
};
void swap(String & s1, String & s2) // noexcept
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	s1.swap(s2);
}

}

int main()
{
	using std::swap;
	Orange::String s1("hello");
	Orange::String s2("world");
	std::cout << "Before swap:" << std::endl;
	std::cout << s1 << " " << s2 << std::endl;
	swap(s1, s2);
	std::cout << "After swap:" << std::endl;
	std::cout << s1 << " " << s2 << std::endl;
}
// g++ --std=c++11 test.cpp
```

输出如下:

```c++
Before swap:
hello world
void Orange::swap(Orange::String&, Orange::String&)
void Orange::String::swap(Orange::String&)
After swap:
world hello
```



### 使用`std::swap`

```c++
#include <utility>
#include <cstring>
#include <algorithm>
#include <iostream>

namespace Orange
{
class String
{
	char * str;
public:
	void swap(String &s) // noexcept
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		std::swap(this->str, s.str);
	}
	String(const char* p)
	{
		size_t size = strlen(p) + 1;
		str = new char[size];
		memcpy(str, p, size);
	}
	friend std::ostream& operator<<(std::ostream& Stream, String& S)
	{
		Stream << S.str;
		return Stream;
	}
};
void swap(String & s1, String & s2) // noexcept
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	s1.swap(s2);
}

}

namespace std
{
template<>
void swap(Orange::String & s1, Orange::String & s2) // noexcept
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	s1.swap(s2);
}
}

int main()
{
	// using std::swap;
	Orange::String s1("hello");
	Orange::String s2("world");
	std::cout << "Before swap:" << std::endl;
	std::cout << s1 << " " << s2 << std::endl;
	std::swap(s1, s2);
	std::cout << "After swap:" << std::endl;
	std::cout << s1 << " " << s2 << std::endl;
}
// g++ --std=c++11 test.cpp
```

输出如下:

```c++
Before swap:
hello world
void std::swap(_Tp&, _Tp&) [with _Tp = Orange::String]
void Orange::String::swap(Orange::String&)
After swap:
world hello
```

上面这种写法中，必须要加上如下template full specialization:

```C++
namespace std
{
template<>
void swap(Orange::String & s1, Orange::String & s2) // noexcept
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	s1.swap(s2);
}
}
```

否则是无法编译通过的。
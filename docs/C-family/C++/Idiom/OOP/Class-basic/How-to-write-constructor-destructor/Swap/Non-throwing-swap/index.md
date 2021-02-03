# Non-throwing swap



## More C++ Idioms [Non-throwing swap](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-throwing_swap)

> NOTE: 关于non-throwing，参见cppreference [Exceptions](https://en.cppreference.com/w/cpp/language/exceptions)、cppreference [noexcept specifier](https://en.cppreference.com/w/cpp/language/noexcept_spec) 

### Intent

1、To implement an exception safe and efficient swap operation.

2、To provide uniform interface to it to facilitate generic programming.

> NOTE: generic programming能够大大提供程序的可维护性，在下面的Solution and Sample Code章节会讨论这个问题。



### Motivation

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

**Performance**

Swapping of two large, complex objects of the same type is inefficient due to acquisition and release of **resources** for the intermediate temporary object.

> NOTE: 上述都是copy construct

**Exception-safety**

This generic swap implementation may throw if resources are not available. (Such a behavior does not make sense where in fact no new resources should have been requested in the first place.) Therefore, this implementation cannot be used for the [Copy-and-swap](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Copy-and-swap) idiom.

> NOTE: 第一段话的解释是：`T temp (a);`是需要acquisition of resources的，所以当系统中resource已经耗尽的情况下，这个语句是会抛出exception的。括号中的意思是：swap是不需要request new resource的；
>
> 最后一句话的意思是：[Copy-and-swap](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Copy-and-swap) idiom的要求swap函数是non-throw的

### Solution and Sample Code

Non-throwing swap idiom uses [Handle Body](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Handle_Body) idiom to achieve the desired effect. The abstraction under consideration is split between two implementation classes. One is **handle** and other one is **body**. The **handle** holds a pointer to a **body** object. The swap is implemented as a simple swap of pointers, which are guaranted to not throw exceptions and it is very efficient as no new resources are acquired or released.

> NOTE: 显然non-throwing swap相比于普通的swap优势在于无需构造temporary object，而是直接交换pointer，直接交换pointer是not throw exception的。
>
> 上面这段话让我疑惑的一个问题是：
>
> Non-throwing swap idiom uses [Handle Body](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Handle_Body) idiom to achieve the desired effect.
>
> 看了一下[Handle Body](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Handle_Body) idiom，这个idiom的意图不仅仅局限于使用一个pointer来作为member variable，在Non-throwing swap idiom 中，[Handle Body](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Handle_Body) idiom所表示的是使用一个pointer来作为member variable。

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

Adding it in two places takes care of two different common usage styles of swap :

(1) unqualified swap 

(2) fully qualified swap (e.g., `std::swap`). 

When **unqualified `swap`** is used, right swap is looked up using **Koenig lookup** (provided one is already defined). 

> NOTE: `swap(Orange::String, Orange::String)`
>
> **Koenig lookup** 就是 [Argument-dependent name lookup](https://en.wikipedia.org/wiki/Argument-dependent_name_lookup)

If **fully qualified `swap`** is used, Koenig lookup is suppressed and one in the `std` namespace is used instead. It is a very common practice. 

> NOTE: `std::swap(Orange::String, Orange::String)`



Remaining discussion here uses fully qualified swap only. It gives a uniform look and feel because `C++` programmers often use `swap` function in an idiomatic way by fully qualifying it with **std::** as shown below.

```c++
template <class T>
void zoo (T t1, T t2) {
//...
int i1, i2;
std::swap(i1,i2); // note uniformity
std::swap(t1,t2); // Ditto here
}
```

In such a case, the right, efficient implementation of `swap` is chosen when `zoo` is instantiated with `String` class defined earlier. Otherwise, the default `std::swap` function template would be instantiated -- completely defeating the purpose of defining the member `swap` and namespace scope `swap` function. This idiom of defining explicit specialization of swap in `std` namespace is particularly useful in generic programming.

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

>  NOTE: 上面代码中的`throw ()`其实是`noexcept`含义

Therefore, it is a good idea to define a specialization of `std::swap` for the types that are amenable to an exception safe, efficient swap implementation. The `C++` standard does not currently allow us to add new templates to the `std` namespace, but it does allow us to specialize templates (e.g. `std::swap`) from that namespace and add them back in it.

### Caveats

Using non-throwing swap idiom for template classes (e.g., `Matrix<T>`) can be a subtle issue. As per the C++98 standard, only the **full specialization** of `std::swap` is allowed to be defined inside `std` namespace for the **user-defined types**. **Partial specializations** or **function overloading** is not allowed by the language. Trying to achieve the similar effect for template classes (e.g., `Matrix<T>`) results into overloading of `std::swap` in `std` namepspace, which is technically **undefined behavior**. This is not necessarily the ideal state of affairs as indicated by some people in a spectacularly long discussion thread on comp.lang.c++.moderated newsgroup.[[1\]](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-throwing_swap#cite_note-1) There are two possible solutions, both imperfect, to this issue:

1、Standard-compliant solution. Leveraging on Koenig lookup, define an overloaded swap function template in the same namespace as that of the class being swapped. Not all compilers may support this correctly, but this solution is compliant to the standard.[[2\]](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-throwing_swap#cite_note-2)

2、Fingers-crossed solution. Partially specialize `std::swap` and ignore the fact that this is technically undefined behavior, hoping that nothing will happen and wait for a fix in the next language standard.

> NOTE: 这种做法相当于什么都没有做

> NOTE:  Caveats所讨论的是将user defined type的`swap`函数添加到`std`中的问题，这是extending namespace `std`，这在`C++\Library\Standard-library\Extending-std.md`中进行了讨论；对于non-throwing swap idiom，推荐使用[Swap values idiom](https://cpppatterns.com/patterns/swap-values.html)，即上述solution 1，关于此，参见`C++\Idiom\Templates-and-generic-programming\Swappable`。



### Source Code

#### 使用cpppatterns [Swappable idiom](https://cpppatterns.com/patterns/swap-values.html)

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



#### 使用`std::swap`

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



## Non-throw swap idiom and move semantic

正如More C++ Idioms [Non-throwing swap](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-throwing_swap)的Solution and Sample Code所述：

> Non-throwing swap idiom uses [Handle Body](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Handle_Body) idiom to achieve the desired effect.

这种设计就使得它非常适合于move semantic，move semantic有着non-throw的特性，所以对它的swap是non-throw的，关于此，在下面的[Why implementing swap() as non-throwing](https://stackoverflow.com/questions/44042043/why-implementing-swap-as-non-throwing)中还会有描述。



## stackoverflow [Why implementing swap() as non-throwing](https://stackoverflow.com/questions/44042043/why-implementing-swap-as-non-throwing)

[A](https://stackoverflow.com/a/44042914)

> My question is **why** we should implement our `swap()` function as a **non-throwing** one

1、Because `swap` is completely useless if it might throw.

Consider: you `swap` two instances, and the operation throws. Now, what state are they in?

The strong guarantee is that there are no side-effects if an exception was thrown, meaning both original objects are left in their original state.

If we *can't* meet the strong guarantee, we simply can't use `swap` in many cases, because there's no way to recover usefully from a failure, and there's no point in writing that version of `swap` at all.

2、Because there's no reason for it to throw.

The trivial implementation of `swap` (now) uses **move-assignment** and **move-construction**.

There's generally no reason for a **move-constructor** to throw: it doesn't allocate anything new, it's just re-seating existing data. There's generally no reason for **move-assignment** to throw (as above), and destructors should never throw - and those are the only operations required.

> NOTE: 需要结合`std::move`的实现来看，gcc `std::swap`的源码路径：https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/move.h
>
> ```c++
>   /**
>    *  @brief Swaps two values.
>    *  @param  __a  A thing of arbitrary type.
>    *  @param  __b  Another thing of arbitrary type.
>    *  @return   Nothing.
>   */
>   template<typename _Tp>
>     _GLIBCXX20_CONSTEXPR
>     inline
> #if __cplusplus >= 201103L
>     typename enable_if<__and_<__not_<__is_tuple_like<_Tp>>,
> 			      is_move_constructible<_Tp>,
> 			      is_move_assignable<_Tp>>::value>::type
> #else
>     void
> #endif
>     swap(_Tp& __a, _Tp& __b)
>     _GLIBCXX_NOEXCEPT_IF(__and_<is_nothrow_move_constructible<_Tp>,
> 				is_nothrow_move_assignable<_Tp>>::value)
>     {
> #if __cplusplus < 201103L
>       // concept requirements
>       __glibcxx_function_requires(_SGIAssignableConcept<_Tp>)
> #endif
>       _Tp __tmp = _GLIBCXX_MOVE(__a);
>       __a = _GLIBCXX_MOVE(__b);
>       __b = _GLIBCXX_MOVE(__tmp);
>     }
> 
>   // _GLIBCXX_RESOLVE_LIB_DEFECTS
>   // DR 809. std::swap should be overloaded for array types.
>   /// Swap the contents of two arrays.
>   template<typename _Tp, size_t _Nm>
>     _GLIBCXX20_CONSTEXPR
>     inline
> #if __cplusplus >= 201103L
>     typename enable_if<__is_swappable<_Tp>::value>::type
> #else
>     void
> #endif
>     swap(_Tp (&__a)[_Nm], _Tp (&__b)[_Nm])
>     _GLIBCXX_NOEXCEPT_IF(__is_nothrow_swappable<_Tp>::value)
>     {
>       for (size_t __n = 0; __n < _Nm; ++__n)
> 	swap(__a[__n], __b[__n]);
>     }
> 
>   /// @} group utilities
> _GLIBCXX_END_NAMESPACE_VERSION
> } // namespace
> ```
>
> 可以看到，它充分发挥了move semantic，正如上面的描述所言。



## cpptruths [tale of noexcept swap for user defined](http://cpptruths.blogspot.com/2011/09/tale-of-noexcept-swap-for-user-defined.html)
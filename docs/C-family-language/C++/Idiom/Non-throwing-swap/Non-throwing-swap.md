# Non-throwing swap

## More C++ Idioms [Non-throwing swap](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-throwing_swap)

> NOTE: 关于non-throwing，参见cppreference [Exceptions](https://en.cppreference.com/w/cpp/language/exceptions)、cppreference [noexcept specifier](https://en.cppreference.com/w/cpp/language/noexcept_spec) 

### Intent

- To implement an exception safe and efficient swap operation.

- To provide uniform interface to it to facilitate generic programming.

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

> NOTE: 第一段话的解释是：`T temp (a);`是需要acquisition of resources的，所以当系统中resource已经耗尽的情况下，这个语句是会抛出exception的。
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

Although an efficient and exception-safe swap function can be implemented (as shown above) as a member function, non-throwing swap idiom goes further than that for simplicity, consistency, and to facilitate generic programming. An explicit specialization of `std::swap` should be added in the `std` namespace as well as the namespace of the class itself.

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

Remaining discussion here uses fully qualified swap only. It gives a uniform look and feel because `C++` programmers often use swap function in an idiomatic way by fully qualifying it with **std::** as shown below.

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

Using non-throwing swap idiom for template classes (e.g., `Matrix<T>`) can be a subtle issue. As per the C++98 standard, only the full specialization of `std::swap` is allowed to be defined inside `std` namespace for the user-defined types. **Partial specializations** or **function overloading** is not allowed by the language. Trying to achieve the similar effect for template classes (e.g., `Matrix<T>`) results into overloading of `std::swap` in `std` namepspace, which is technically **undefined behavior**. This is not necessarily the ideal state of affairs as indicated by some people in a spectacularly long discussion thread on comp.lang.c++.moderated newsgroup.[[1\]](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-throwing_swap#cite_note-1) There are two possible solutions, both imperfect, to this issue:

1. Standard-compliant solution. Leveraging on Koenig lookup, define an overloaded swap function template in the same namespace as that of the class being swapped. Not all compilers may support this correctly, but this solution is compliant to the standard.[[2\]](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-throwing_swap#cite_note-2)

2. Fingers-crossed solution. Partially specialize `std::swap` and ignore the fact that this is technically undefined behavior, hoping that nothing will happen and wait for a fix in the next language standard.

   > NOTE: 这种做法相当于什么都没有做

> NOTE: 这一节的内容并没有读懂



## [Why implementing swap() as non-throwing](https://stackoverflow.com/questions/44042043/why-implementing-swap-as-non-throwing)





## [tale of noexcept swap for user defined](http://cpptruths.blogspot.com/2011/09/tale-of-noexcept-swap-for-user-defined.html)
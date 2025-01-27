# ADL

ADL是compiler的一种行为，可以将compiler的name lookup分为两种：

1. ADL

2. ordinary lookup



## ADL简介

stackoverflow [What is “Argument-Dependent Lookup” (aka ADL, or “Koenig Lookup”)?](https://stackoverflow.com/questions/8111677/what-is-argument-dependent-lookup-aka-adl-or-koenig-lookup) # [A](https://stackoverflow.com/a/8111750) 

> In simpler terms Nicolai Josuttis states1:
>
> > You don’t have to qualify the namespace for functions if one or more argument types are defined in the namespace of the function.
>
> A simple code example:
>
> ```cpp
> namespace MyNamespace {
>     class MyClass {
>     };
> 
>     void doSomething(MyClass) {}
> }
> 
> MyNamespace::MyClass obj; // global object
> 
> int main() {
>     doSomething(obj); // Works Fine - MyNamespace::doSomething() is called.
> }
> ```
>
> What is the advantage of ADL?
>
> As the simple code example above demonstrates, ADL provides convenience and ease of usage to the programmer. Without ADL there would be an overhead on the programmer, to repeatedly specify the fully qualified names, or instead, use numerous `using`-declarations.



## cppreference [ADL](https://en.cppreference.com/w/cpp/language/adl) 

**Argument-dependent lookup**, also known as **ADL**, or **Koenig lookup**, is the set of rules for looking up the **unqualified function names** in [function-call expressions](operator_other.html), including implicit function calls to [overloaded operators](operators.html). These **function names** are looked up in the namespaces of their arguments(不止考虑一个argument，而是多个argument) in addition to the scopes and namespaces considered by the usual [unqualified name lookup](lookup.html).

> NOTE: 
>
> 一、使用ADL的场景:
>
> "**unqualified function names** in [function-call expressions](operator_other.html), including implicit function calls to [overloaded operators](operators.html)"
>
> 二、简而言之：在lookup function name的时候，考虑如下namespace：
>
> 1、ADL: namespace of their argument
>
> 2、ordinary lookup: scope and namespace considered by the usual [unqualified name lookup](lookup.html)（可能包含当前scope、global scope）
>
> 三、次序
>
> 1、思考：上述两者，谁先谁后？
>
> 通过`swap`的例子来看，应该是首先进行ADL，然后进行ordinary lookup。在cpppatterns [Range-based algorithms](https://cpppatterns.com/patterns/range-based-algorithms.html)中对此进行了说明：
>
> > On [lines 12–13](https://cpppatterns.com/patterns/range-based-algorithms.html#line12), we call `begin` and `end` on the range to get the respective iterators to the beginning and end of the range. We use using-declarations on [lines 7–8](https://cpppatterns.com/patterns/range-based-algorithms.html#line7) to allow these calls to be found via [argument-dependent lookup](https://en.wikipedia.org/wiki/Argument-dependent_name_lookup) before using the standard [`std::begin`](http://en.cppreference.com/w/cpp/iterator/begin) and [`std::end`](http://en.cppreference.com/w/cpp/iterator/end) functions. With these iterators, we can now implement the algorithm over the elements between them.
>
> 2、ADL优先在parameters的name space中进行lookup，然后是current name space，这和Python的lookup顺序是有些类似的

**Argument-dependent lookup** makes it possible to use operators defined in a different namespace. Example:

### Example

```c++
#include <iostream>
int main()
{
    std::cout << "Test\n"; // There is no operator<< in global namespace, but ADL
                           // examines std namespace because the left argument is in
                           // std and finds std::operator<<(std::ostream&, const char*)
    operator<<(std::cout, "Test\n"); // same, using function call notation
 
    // however,
    std::cout << endl; // Error: 'endl' is not declared in this namespace.
                       // This is not a function call to endl(), so ADL does not apply
 
    endl(std::cout); // OK: this is a function call: ADL examines std namespace
                     // because the argument of endl is in std, and finds std::endl
 
    (endl)(std::cout); // Error: 'endl' is not declared in this namespace.
                       // The sub-expression (endl) is not a function call expression
}
```

> NOTE: 上述例子能够极好的帮助我们理解ADL：
>
> 通过上面的解释可知: `std::cout << "Test\n"`竟然蕴藏着如此之多的玄机：
>
> 1、`std::cout`是一个object，它没有成员函数`operator<<`
>
> 2、在`<iostram>`中，定义了`std::operator<<(std::ostream&, const char*)`
>
> 3、当我们编写`std::cout << "Test\n";`的时候，compiler会使用ADL来进行查找：
>
> > There is no `operator<<` in global namespace, but ADL examines `std` namespace because the left argument is in `std` and finds `std::operator<<(std::ostream&, const char*)` 
>
> 这让我想到了之前在阅读代码的时候，有很多类似的写法：
>
> `std::cout << endl`无法编译通过的原因是：ADL是用于lookup function的，unqualified的`endl`，没有在当前scope中声明，所以它无法被找到。

### Details

> NOTE: 原文的一段，冗长且没有例子，难以阅读；

First, the argument-dependent lookup is not considered if the lookup set produced by usual [unqualified lookup](https://en.cppreference.com/w/cpp/language/lookup) contains any of the following:

1) a declaration of a class member.
2) a declaration of a function at block scope (that's not a [using declaration](https://en.cppreference.com/w/cpp/language/namespace#Using-declarations)).
3) any declaration that is not a function or a function template (e.g. a function object or another variable whose name conflicts with the name of the function that's being looked up).

Otherwise, for every argument in a function call expression its type is examined to determine the *associated set of namespaces and classes* that it will add to the lookup.

> NOTE: 即考虑argument的namespace

### Notes

#### The interface principle

Because of **argument-dependent lookup**, non-member functions and non-member operators defined in the same namespace as a class are considered part of the public interface of that class (if they are found through ADL) [[2\]](https://en.cppreference.com/w/cpp/language/adl#cite_note-2).

> NOTE: 
>
> 1、这非常重要，从interface的角度来理解。
>
> 2、上述观点是和[Herb Sutter](http://en.wikipedia.org/wiki/Herb_Sutter)的文章[What's In a Class? - The Interface Principle](http://www.gotw.ca/publications/mill02.htm)中的观点一致的

#### Swappable idiom

ADL is the reason behind the established idiom for swapping two objects in generic code:

```c++
using std::swap;
swap(obj1, obj2);
```

because calling [std::swap](http://en.cppreference.com/w/cpp/algorithm/swap)(obj1, obj2) directly would not consider the user-defined swap() functions that could be defined in the same namespace as the types of `obj1` or `obj2`, and just calling the unqualified `swap(obj1, obj2)` would call nothing if no user-defined overload was provided. In particular, [std::iter_swap](https://en.cppreference.com/w/cpp/algorithm/iter_swap) and all other standard library algorithms use this approach when dealing with [*Swappable*](https://en.cppreference.com/w/cpp/named_req/Swappable) types. 

#### ADL & dependent name lookup

Name lookup rules make it impractical(不切实际的) to declare operators in global or user-defined namespace that operate on types from the **`std` namespace**, e.g. a custom `operator>>` or `operator+` for [std::vector](https://en.cppreference.com/w/cpp/container/vector) or for [std::pair](https://en.cppreference.com/w/cpp/utility/pair) (unless the element types of the vector/pair are **user-defined types**, which would add their namespace to **ADL**). Such operators would not be looked up from template instantiations, such as the standard library algorithms. See [dependent names](https://en.cppreference.com/w/cpp/language/dependent_name) for further details.



#### ADL&friend function

ADL can find a [friend function](https://en.cppreference.com/w/cpp/language/friend) (typically, an overloaded operator) that is defined entirely within a class or class template, even if it was never declared at namespace level.

```c++
template<typename T>
struct number {
    number(int) {}

    friend number gcd(number x, number y) { return 0; }; // definition within a class template
};

// unless a matching declaration is provided gcd is an invisible (except through ADL)
// member of this namespace
void g() {
    number<double> a(3), b(4);
    // finds gcd because number<double> is an associated class, making gcd visible in its namespace (global scope)
    a = gcd(a, b);
//  b = gcd(3,4); // Error; gcd is not visible
}

int main() {
    g();
}
```

> NOTE: `g++ test.cpp`

(until C++20)

Although a function call can be resolved through **ADL** even if **ordinary lookup** finds nothing, a function call to a [function template](function_template.html) with explicitly-specified template arguments requires that there is a declaration of the **template** found by **ordinary lookup** (otherwise, it is a syntax error to encounter an unknown name followed by a less-than character)

> NOTE: lookup template和lookup function call

```c++
namespace N1 {
    struct S {
    };

    template<int X>
    void f(S) {}
}
namespace N2 {
    template<class T>
    void f(T t) {}
}

void g(N1::S s) {
    f<3>(s);      // Syntax error until C++20 (unqualified lookup finds no f)
    N1::f<3>(s);  // OK, qualified lookup finds the template 'f'
    /**
     * Error: N2::f does not take a non-type parameter
     * N1::f is not looked up because ADL only works
     * with unqualified names
     */
    N2::f<3>(s);
    using N2::f;
    /**
     * OK: Unqualified lookup now finds N2::f
     * then ADL kicks in because this name is unqualified
     * and finds N1::f
     */
    f<3>(s);
}

int main() {
    g(N1::S());
}
```



#### ADL-only lookup

In the following contexts **ADL-only lookup** (that is, lookup in associated namespaces only) takes place:

- the lookup of non-member functions `begin` and `end` performed by the [range-for](range-for.html) loop if member lookup fails
- the [dependent name lookup](dependent_name.html#Lookup_rules) from the point of **template instantiation**.
- the lookup of non-member function `get` performed by [structured binding declaration](structured_binding.html) for tuple-like types (since C++17)

## hackingcpp [Argument Dependent Lookup](https://hackingcpp.com/cpp/lang/adl.html) 



## ADL的重要意义

在`C++\Language-reference\Classes\The-interface-principle`中对ADL and the Interface Principle进行了分析。

总的来说: ADL make C++ generic and extensible: 

一、从Interface Principle的角度来看，C++ ADL是为了更好、更灵活地支持OOP、generic programming。

[gotw-What's In a Class? - The Interface Principle](http://www.gotw.ca/publications/mill02.htm) 

二、扩展性 和 static polymorphism

借助ADL可以扩展STL，实现static polymorphism，正如wikipedia [Argument-dependent name lookup#Interfaces](https://en.wikipedia.org/wiki/Argument-dependent_name_lookup#Interfaces) 所总结的: 

> Functions found by ADL are considered part of a class's interface. In the C++ Standard Library, several algorithms use unqualified calls to `swap` from within the `std` namespace. As a result, the generic `std::swap` function is used if nothing else is found, but if these algorithms are used with a third-party class, `Foo`, found in another namespace that also contains `swap(Foo&, Foo&)`, that overload of `swap` will be used.

典型的例子：

1、[Range-based for loop](https://en.cppreference.com/w/cpp/language/range-for) 

2、[Swap values](https://cpppatterns.com/patterns/swap-values.html)

三、stackoverflow [What is “Argument-Dependent Lookup” (aka ADL, or “Koenig Lookup”)?](https://stackoverflow.com/questions/8111677/what-is-argument-dependent-lookup-aka-adl-or-koenig-lookup) # [A](https://stackoverflow.com/a/8111750) 

> ADL provides convenience and ease of usage to the programmer. Without ADL there would be an overhead on the programmer, to repeatedly specify the fully qualified names, or instead, use numerous `using`-declarations.



## ADL and multiple dispatch

ADL和OOP multiple dispatch是有着相似之处的: 

1) 都将多个argument纳入了考虑



## Use cases&Examples



### [Range-based algorithms](https://cpppatterns.com/patterns/range-based-algorithms.html) / [Range-based for loop](https://en.cppreference.com/w/cpp/language/range-for) 

`std::begin` 和 类成员函数`begin`：

下面的`initializer_list`就是一个例子：

```c++
// initializer_list standard header (core)

// Copyright (c) Microsoft Corporation.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

// CLASS TEMPLATE initializer_list
template <class _Elem>
class initializer_list {
public:

    _NODISCARD constexpr const _Elem* begin() const noexcept {
        return _First;
    }

    _NODISCARD constexpr const _Elem* end() const noexcept {
        return _Last;
    }


};

// FUNCTION TEMPLATE begin
template <class _Elem>
_NODISCARD constexpr const _Elem* begin(initializer_list<_Elem> _Ilist) noexcept {
    return _Ilist.begin();
}

// FUNCTION TEMPLATE end
template <class _Elem>
_NODISCARD constexpr const _Elem* end(initializer_list<_Elem> _Ilist) noexcept {
    return _Ilist.end();
}
```



### [Swappable idiom](https://en.cppreference.com/w/cpp/named_req/Swappable)=ADL+fallback  

在`C++\Idiom\OOP\Non-throwing-swap`中给出的例子，就展示了ADL：

#### 使用swap value idiom

```c++
#include <utility>
#include <cstring>
#include <algorithm>
#include <iostream>

namespace Orange {
    class String {
        char *str;
    public:
        void swap(String &s) // noexcept
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            std::swap(this->str, s.str);
        }

        String(const char *p) {
            size_t size = strlen(p) + 1;
            str = new char[size];
            memcpy(str, p, size);
        }

        friend std::ostream &operator<<(std::ostream &Stream, String &S) {
            Stream << S.str;
            return Stream;
        }
    };

    void swap(String &s1, String &s2) // noexcept
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        s1.swap(s2);
    }
}


int main() {
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

上述例子使用swap value idiom，它是最最简单的，推荐使用上述写法。

#### Full specialization `std::swap`

```c++
#include <utility>
#include <cstring>
#include <algorithm>
#include <iostream>

namespace Orange {
    class String {
        char *str;
    public:
        void swap(String &s) noexcept {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            std::swap(this->str, s.str);
        }

        String(const char *p) {
            size_t size = strlen(p) + 1;
            str = new char[size];
            memcpy(str, p, size);
        }

        friend std::ostream &operator<<(std::ostream &Stream, String &S) {
            Stream << S.str;
            return Stream;
        }
    };

    void swap(String &s1, String &s2) // noexcept
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        s1.swap(s2);
    }

}

namespace std {
    template<>
    void swap(Orange::String &s1, Orange::String &s2) noexcept {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        s1.swap(s2);
    }
}

int main() {
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

上述例子通过full specialization `std::swap`来达到调用user-defined swap的目的。



#### spdlog:`logger-inc.h` 

```c++
SPDLOG_INLINE void swap(logger &a, logger &b)
{
    a.swap(b);
}
```

`std::swap`和类成员函数`swap`

```c++
using std::swap;
swap(a, b);
```



### Function template: `get_rtti`

我已经使用了C++ ADL了，`get_rtti`就是一个例子。下面是简化的代码：

```c++
#include <string>
#include <iostream>

/**
 * 结构体1
 */
struct MyStruct1 {
};

/**
 * 结构体
 */
struct MyStruct2 {
};

/**
 * 结构体类型信息
 */
typedef std::string CStructRtti;

/**
 * 获得结构体MyStruct的类型信息，这是按照对象来获取结构体类型信息
 * @param f
 * @return
 */
CStructRtti GetRtti(MyStruct1 *f) {
    return "MyStruct1";
}

/**
 * 获得结构体MyStruct的类型信息，这是按照对象来获取结构体类型信息
 * @param f
 * @return
 */
CStructRtti GetRtti(MyStruct2 *f) {
    return "MyStruct2";
}

/**
 * 模板函数，根据结构体类型来获取结构体类型信息，它的实现是派发到上述的根据结构体对象来获取结构体类型休息
 * @return
 */
template<typename StructT>
CStructRtti GetRttiByType() {
    return GetRtti(static_cast<StructT *>(nullptr));
}

class Utility {
public:
    template<typename StrucT>
    static CStructRtti GetRtti() {
        return GetRttiByType<StrucT>();
    }
};
/**
 * 该namespace内部和外部的结构基本一致
 * @param f
 * @return
 */
namespace NS {
    /**
     * 结构体1
     */
    struct MyStruct1 {
    };

    /**
     * 结构体
     */
    struct MyStruct2 {
    };

    /**
     * 获得结构体MyStruct的类型信息，这是按照对象来获取结构体类型信息
     * @param f
     * @return
     */
    CStructRtti GetRtti(MyStruct1 *f) {
        return "NS::MyStruct1";
    }

    /**
     * 获得结构体MyStruct的类型信息，这是按照对象来获取结构体类型信息
     * @param f
     * @return
     */
    CStructRtti GetRtti(MyStruct2 *f) {
        return "NS::MyStruct2";
    }


    class Utility {
    public:
        template<typename StrucT>
        static CStructRtti GetRtti() {
            return GetRttiByType<StrucT>();
        }

    };
}

int main() {
    std::cout << Utility::GetRtti<MyStruct1>() << std::endl;
    std::cout << NS::Utility::GetRtti<NS::MyStruct1>() << std::endl;
}
// g++ test.cpp --std=c++11 -pedantic -Wall 

```

输出如下:

```
MyStruct1
NS::MyStruct1
```

如下template就是ADL发生的地方

```c++
template<typename StructT>
CStructRtti GetRttiByType() {
    return GetRtti(static_cast<StructT *>(nullptr));
}
```



### ADL&dependent name lookup

参见 cppreference [Dependent names](https://en.cppreference.com/w/cpp/language/dependent_name) 



## TODO

http://www.gotw.ca/gotw/030.htm

https://quuxplusone.github.io/blog/2019/04/26/what-is-adl/

https://stackoverflow.com/questions/18086292/is-bjarne-wrong-about-this-example-of-adl-or-do-i-have-a-compiler-bug


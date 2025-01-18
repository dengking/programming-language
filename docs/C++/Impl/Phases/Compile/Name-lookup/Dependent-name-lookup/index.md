# Dependent name lookup

对于template，C++ compiler有着特殊的处理，这是本章讨论的问题。

## Guide

Names inside template are divided into two classes:  

- Dependent: names that depend on the template parameters but aren't declared within the template.
  - Dependent name lookup发生于**template instantiation**，由于会将template argument考虑在内，因此它保证能够看到template specialization，因为template specialization是依赖于模板参数的
  - 由于dependent name可以表示3种不同的entity(identifier、type、template)，因此需要disambiguator来指定entity的类型
- Non-dependent: names that don't depend on the template parameters, plus the name of the template itself and names declared within it
  - Non-dependent name发生于**template definition**，由于不依赖于template argument，因此能够在template definition的时候找到的name对应的entity



它们的主要区别在于执行lookup的时机

两种name(dependent、non-dependent)、两种lookup(ADL、non-ADL)、template的两种context(definition、instantiation)，所以本文花了很大的篇幅来讨论dependent name lookup和ADL之间的关系。

## 入门读物

一、thegreenplace [Dependent name lookup for C++ templates](https://eli.thegreenplace.net/2012/02/06/dependent-name-lookup-for-c-templates)

这篇文章非常好，说明清楚了:

a. 为什么需要dependent name lookup

b. 如何使用



## cppreference [Two-phase name lookup](https://en.cppreference.com/w/cpp/language/two-phase_lookup)

> NOTE:
>
> 一、two-phase name lookup: 
>
> 1、non-dependent name lookup(**template's definition**)
>
> 2、dependent name lookup(**template's instantiation**)
>
> 关于此，参见: thegreenplace [Dependent name lookup for C++ templates](https://eli.thegreenplace.net/2012/02/06/dependent-name-lookup-for-c-templates) 

Two-phase name lookup is the process by which a template is instantiated.

- First, at the point of the **template definition**, the template is checked for syntax(**template's definition**).
- Second, at the point of **instantiation**, the **template argument** is substituted in for the **template parameter**, instantiating the template(**template's instantiation**)

Some compilers do not fully support the first phase, and instead delay nearly all the work until encountering the point of **instantiation**.

## cppreference [Dependent names](https://en.cppreference.com/w/cpp/language/dependent_name) 

Inside the definition of a [template](https://en.cppreference.com/w/cpp/language/templates) (both [class template](https://en.cppreference.com/w/cpp/language/class_template) and [function template](https://en.cppreference.com/w/cpp/language/function_template)), the meaning of some constructs may differ from one instantiation to another. In particular, **types and expressions** may depend on types of **type template parameters** and values of **non-type template parameters**.

> NOTE: 
>
> 一、下面是对最后一句话的理解：
>
> | template parameter           |       |
> | ---------------------------- | ----- |
> | type template parameters     | type  |
> | non-type template parameters | value |
>
> 

```C++
template<typename T>
struct X : B<T> // "B<T>" is dependent on T
{
    typename T::A* pa; // "T::A" is dependent on T
                       // (see below for the meaning of this use of "typename")
    void f(B<T>* pb) {
        static int i = B<T>::i; // "B<T>::i" is dependent on T
        pb->j++; // "pb->j" is dependent on T
    }
};
```

Name lookup and binding are different for *dependent names* and non-*dependent names*

### Binding rules

**Non-dependent names** are looked up and bound at the point of **template definition**. This **binding** holds even if at the point of **template instantiation** there is a better match:

```c++
#include <iostream>

void g(double) {
    std::cout << "g(double)\n";
}

template<class T>
struct S {
    void f() const {
        g(1); // "g" is a non-dependent name, bound now
    }
};

void g(int) {
    std::cout << "g(int)\n";
}

int main() {
    g(1); // calls g(int)

    S<int> s;
    s.f(); // calls g(double)
}
// g++ test.cpp

```

> NOTE: 
>
> 一、上述程序的输出如下:
>
> ```c++
> g(int)
> g(double)
> 
> ```
>
> 可以看到`s.f(); // calls g(double)`，即使有更好的overload `void g(int)`。

Binding of *dependent names* is postponed until lookup takes place.

### Lookup rules

As discussed in [lookup](https://en.cppreference.com/w/cpp/language/lookup), the lookup of a **dependent name** used in a template is postponed(推迟) until the **template arguments** are known, at which time

1、non-ADL lookup examines function declarations with external linkage that are visible from the *template definition* context

2、[ADL](https://en.cppreference.com/w/cpp/language/adl) examines function declarations with external linkage that are visible from either the *template definition* context or the *template instantiation* context

(in other words, adding a new function declaration after **template definition** does not make it visible, except via ADL(在template definition后添加的function declaration只有通过ADL才能够让template看到，否则不行，在前面展示了这种例子)).

> NOTE: 
>
> 一、综合上面描述的compiler进行lookup的两个规则，可知：
>
> |                                                              | *template definition* context | *template instantiation* context |
> | ------------------------------------------------------------ | ----------------------------- | -------------------------------- |
> | non-ADL lookup                                               | yes                           | no                               |
> | [ADL](https://en.cppreference.com/w/cpp/language/adl) lookup | yes                           | yes                              |
>
> 从中可以看出:
>
> 在*template instantiation* context是不执行non-ADL lookup，通过后面的详细解释可知这样做的目的是为了避免违法ODR，尤其需要注意的是搞清楚这样做的原因。

(in other words, adding a new function declaration after **template definition** does not make it visible, except via ADL).



#### Dependent name&ADL lookup&std

> NOTE: 
>
> 其实原文这里花了很大的篇幅讨论"dependent name&ADL lookup&std"，在 cppreference [ADL](https://en.cppreference.com/w/cpp/language/adl) 中对此有着如下描述:
>
> > Name lookup rules make it impractical(不切实际的) to declare operators in global or user-defined namespace that operate on types from the **`std` namespace**, e.g. a custom `operator>>` or `operator+` for [std::vector](https://en.cppreference.com/w/cpp/container/vector) or for [std::pair](https://en.cppreference.com/w/cpp/utility/pair) (unless the element types of the vector/pair are **user-defined types**, which would add their namespace to **ADL**). Such operators would not be looked up from template instantiations, such as the standard library algorithms. See [dependent names](https://en.cppreference.com/w/cpp/language/dependent_name) for further details.
>
> 

##### Non-ADL lookup example

The purpose of this rule is to help guard against violations of the [ODR](https://en.cppreference.com/w/cpp/language/definition#One_Definition_Rule) for template instantiations:

```c++
#include <iostream>
#include <vector>

// an external library
namespace E {
    template<typename T>
    void writeObject(const T &t) {
        std::cout << "Value = " << t << '\n';
    }
}

// translation unit 1:
// Programmer 1 wants to allow E::writeObject to work with vector<int>
namespace P1 {
    std::ostream &operator<<(std::ostream &os, const std::vector<int> &v) {
        for (int n: v)
            os << n << ' ';
        return os;
    }

    void doSomething() {
        std::vector<int> v;
        E::writeObject(v); // error: will not find P1::operator<<
    }
}

// translation unit 2:
// Programmer 2 wants to allow E::writeObject to work with vector<int>
namespace P2 {
    std::ostream &operator<<(std::ostream &os, const std::vector<int> &v) {
        for (int n: v)
            os << n << ':';
        return os << "[]";
    }

    void doSomethingElse() {
        std::vector<int> v;
        E::writeObject(v); // error: will not find P2::operator<<
    }
}

int main() {
    P1::doSomething();
    P2::doSomethingElse();
}
// g++ --std=c++11 test.cpp

```

> NOTE: 
>
> 一、上述程序编译报错，报错如下:
>
> ```C++
> test.cpp: In instantiation of ‘void E::writeObject(const T&) [with T = std::vector<int>]’:
> test.cpp:27:18:   required from here
> test.cpp:10:26: 错误：无法将左值‘std::basic_ostream<char>’绑定到‘std::basic_ostream<char>&&’
> std::cout << "Value = " << t << '\n';
> 
> ```
>
> 具体原因在下面进行了解释: 在template instantiation context，是不能够执行non-ADL lookup，这就意味着在 `namespace P2` 中函数 `doSomethingElse` 中会调用 `E::writeObject(v)`，由于不能够执行non-ADL lookup，所以即使在 `doSomethingElse` 的前面定义了 `std::ostream &operator<<(std::ostream &os, const std::vector<int> &v)`，但是compiler是不会使用它的，因为它是non-ADL lookup查找的结果。如果允许执行 non-ADL lookup 的话，那么就会导致 "the instantiation of `E::writeObject<vector<int>>` would have two different definitions: one using `P1::operator<<` and one using `P2::operator<<`. Such ODR violation may not be detected by the linker, leading to one or the other being used in both instances"

In the above example, if non-ADL lookup for `operator<<` were allowed from the instantiation context, the instantiation of `E::writeObject<vector<int>>` would have two different definitions: one using `P1::operator<<` and one using `P2::operator<<`. Such ODR violation may not be detected by the linker, leading to one or the other being used in both instances.

##### ADL example

To make ADL examine a user-defined namespace, either `std::vector` should be replaced by a user-defined class or its element type should be a user-defined class:

```c++
#include <iostream>
#include <vector>

// an external library
namespace E {
    template<typename T>
    void writeObject(const T &t) {
        std::cout << "Value = " << t << '\n';
    }
}

// translation unit 1:
// Programmer 1 wants to allow E::writeObject to work with vector<int>
namespace P1 {
    class C {

    };

    std::ostream &operator<<(std::ostream &os, const C &v) {
        os << "class P1::C";
        return os;
    }

    // if C is a class defined in the P1 namespace
    std::ostream &operator<<(std::ostream &os, const std::vector<C> &v) {
        for (C n: v)
            os << n << ' ';
        return os;
    }

    void doSomething() {
        std::vector<C> v;
        v.emplace_back();
        v.emplace_back();
        v.emplace_back();
        E::writeObject(v); // OK: instantiates writeObject(std::vector<P1::C>) which finds P1::operator<< via ADL
    }
}

// translation unit 2:
// Programmer 2 wants to allow E::writeObject to work with vector<int>
namespace P2 {
    class C {

    };

    std::ostream &operator<<(std::ostream &os, const C &v) {
        os << "class P2::C";
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const std::vector<C> &v) {
        for (C n: v)
            os << n << ' ';
        return os;
    }

    void doSomethingElse() {
        std::vector<C> v;
        v.emplace_back();
        v.emplace_back();
        v.emplace_back();
        E::writeObject(v);
    }
}

int main() {
    P1::doSomething();
    P2::doSomethingElse();
}
// g++ --std=c++11 test.cpp

```



##### Example: operator and argument in different namespace

Note: this rule makes it impractical to overload operators for standard library types

> NOTE: 

```C++
#include <iostream>
#include <iterator>
#include <utility>
#include <vector>

// Bad idea: operator in global namespace, but its arguments are in std::
std::ostream &operator<<(std::ostream &os, std::pair<int, double> p) {
    return os << p.first << ',' << p.second;
}

int main() {
    typedef std::pair<int, double> elem_t;
    std::vector<elem_t> v(10);
    std::cout << v[0] << '\n'; // OK, ordinary lookup finds ::operator<<
    std::copy(v.begin(), v.end(),
              std::ostream_iterator<elem_t>(std::cout, " "));
    // Error: both ordinary lookup from the point of definition of
    // std::ostream_iterator and ADL will only consider the std namespace,
    // and will find many overloads of std::operator<<, so the lookup will be done.
    // Overload resolution will then fail to find operator<< for elem_t
    // in the set found by the lookup.
}

```

> NOTE: 上述程序报错如下:
>
> ```C++
> /usr/include/c++/4.8.2/bits/stl_algobase.h:428:38:   required from ‘_OI std::__copy_move_a2(_II, _II, _OI) [with bool _IsMove = false; _II = __gnu_cxx::__normal_iterator<std::pair<int, double>*, std::vector<std::pair<int, double> > >; _OI = std::ostream_iterator<std::pair<int, double> >]’
> /usr/include/c++/4.8.2/bits/stl_algobase.h:460:17:   required from ‘_OI std::copy(_II, _II, _OI) [with _II = __gnu_cxx::__normal_iterator<std::pair<int, double>*, std::vector<std::pair<int, double> > >; _OI = std::ostream_iterator<std::pair<int, double> >]’
> test.cpp:16:81:   required from here
> /usr/include/c++/4.8.2/bits/stream_iterator.h:198:13: 错误：无法将左值‘std::ostream_iterator<std::pair<int, double> >::ostream_type {aka std::basic_ostream<char>}’绑定到‘std::basic_ostream<char>&&’
>   *_M_stream << __value;
>              ^
> In file included from /usr/include/c++/4.8.2/iostream:39:0,
>                  from test.cpp:1:
> /usr/include/c++/4.8.2/ostream:602:5: 错误：以初始化‘std::basic_ostream<_CharT, _Traits>& std::operator<<(std::basic_ostream<_CharT, _Traits>&&, const _Tp&) [with _CharT = char; _Traits = std::char_traits<char>; _Tp = std::pair<int, double>]’的实参 1
>      operator<<(basic_ostream<_CharT, _Traits>&& __os, const _Tp& __x)
> 
> ```
>
> 错误的原因在于，compiler的name lookup机制无法找到定义在global namespace的`std::ostream& operator<<(std::ostream &os, std::pair<int, double> p)`；正如源程序中所注释的：
>
> Bad idea: operator in global namespace, but its arguments are in `std::`
>
> 

```C++
#include <iostream>
#include <vector>
#include <iterator>
#include <utility>

namespace std {
    std::ostream &operator<<(std::ostream &os, std::pair<int, double> p) {
        return os << p.first << ',' << p.second;
    }
}

int main() {
    typedef std::pair<int, double> elem_t;
    std::vector<elem_t> v(10);
    std::cout << v[0] << '\n'; // OK, ordinary lookup finds ::operator<<
    std::copy(v.begin(), v.end(), std::ostream_iterator<elem_t>(std::cout, " ")); // Error: both ordinary
    // lookup from the point of definition of std::ostream_iterator and ADL will
    // only consider the std namespace, and will find many overloads of
    // std::operator<<, so the lookup will be done. Overload resolution will then
    // fail to find operator<< for elem_t in the set found by the lookup.
}

```



### Dependent types

The following types are dependent types:

| dependent type     | explanation | 说明                         |
| ------------------ | ----------- | ---------------------------- |
| template parameter |             | 这是最最常见的dependent type |
|                    |             |                              |
|                    |             |                              |
|                    |             |                              |
|                    |             |                              |



Note: a `typedef` member of a current instantiation is only dependent when the type it refers to is.

> NOTE: 这句话要如何理解？

### Type-dependent expressions

The following expressions are type-dependent

> NOTE: 下面收录了我目前遇到过的:

1) `this`, if the class is a dependent type.

> NOTE: 参见`C++\Language-reference\Classes\Members\Non-static-member\this-pointer`章节。

### Value-dependent expressions



### Current instantiation

Within a **class template definition** (including its member functions and nested classes) some names may be deduced to refer to the *current instantiation*. This allows certain errors to be detected at the point of definition, rather than **instantiation**, and removes the requirement on the `typename` and `template` disambiguators for dependent names, see below.

```c++
#include <iostream>
#include <vector>

template<class T>
class A {
    A *p1;    // A is the current instantiation
    A<T> *p2; // A<T> is the current instantiation
    ::A<T> *p4; // ::A<T> is the current instantiation
    A<T *> p3; // A<T*> is not the current instantiation
    class B {
        B *p1; // B is the current instantiation
        A<T>::B *p2; // A<T>::B is the current instantiation
        typename A<T *>::B *p3; // A<T*>::B is not the current instantiation
    };
};

template<class T>
class A<T *> {
    A<T *> *p1;  // A<T*> is the current instantiation
    A<T> *p2;   // A<T> is not the current instantiation
};

template<int I>
struct B {
    static const int my_I = I;
    static const int my_I2 = I + 0;
    static const int my_I3 = my_I;
    B<my_I> *b3;  // B<my_I> is the current instantiation
    B<my_I2> *b4; // B<my_I2> is not the current instantiation
    B<my_I3> *b5; // B<my_I3> is the current instantiation
};

int main() {
    B<5> b;

    A<int> a1;
    A<int *> a2;
}
// g++ --std=c++11 test.cpp

```



### The `typename` disambiguator for dependent names

```c++
#include <iostream>
#include <vector>

int p = 1;

template<typename T>
void foo(const std::vector<T> &v) {

    // std::vector<T>::const_iterator is a dependent name,
    typename std::vector<T>::const_iterator it = v.begin();

    // without 'typename', the following is parsed as multiplication
    // of the type-dependent member variable 'const_iterator'
    // and some variable 'p'. Since there is a global 'p' visible
    // at this point, this template definition compiles.
    // std::vector<T>::const_iterator *p;

    typedef typename std::vector<T>::const_iterator iter_t;
    iter_t *p2; // iter_t is a dependent name, but it's known to be a type name
}

template<typename T>
struct S {
    typedef int value_t; // member of current instantiation
    void f() {
        S<T>::value_t n{};  // S<T> is dependent, but 'typename' not needed
        std::cout << n << '\n';
    }
};

int main() {
    std::vector<int> v;
    foo(v); // template instantiation fails: there is no member variable
    // called 'const_iterator' in the type std::vector<int>
    S<int>().f();
}
// g++ --std=c++11 test.cpp

```



### The `template` disambiguator for dependent names

Similarly, in a template definition, a dependent name that is not a member of the *current instantiation* is not considered to be a template name unless the disambiguation keyword `template` is used or unless it was already established as a template name:

```c++
template<typename T>
struct S {
    template<typename U>
    void foo() {
    }
};

template<typename T>
void bar() {
    S<T> s;
    s.foo<T>(); // error: < parsed as less than operator
    s.template foo<T>(); // OK
}

```

#### Example



```C++
#include "stddef.h" // size_t
#include <vector>

struct SFieldInfo {
    /**
     * 字段在结构体中的偏移位置
     */
    size_t m_iOffset;
};

class CStructRtti {
    std::vector<SFieldInfo> m_FieldMetaList;
public:
    template<typename ValueType>
    const ValueType *GetValue(void *Data, int Index) const {
        const SFieldInfo &FieldMeta = m_FieldMetaList[Index];
        size_t iOffset = FieldMeta.m_iOffset;
        char *DataStartAddress = reinterpret_cast<char *>(Data) + iOffset;
        return reinterpret_cast<const ValueType *>(DataStartAddress);
    }
};

class CTableInterface {
protected:
    /**
     * 列信息
     */
    CStructRtti *m_Columns{nullptr};
public:
    virtual double GetDouble(size_t RowID, size_t ColID) = 0;
};

template<typename RowType>
class CTableImpl : public CTableInterface {
    RowType m_Row{};
public:
    double GetDouble(size_t RowID, size_t ColID) override {
        return *m_Columns->template GetValue<double>(&m_Row,
                                                     ColID); // 必须要使用 The `template` disambiguator for dependent names
    }
};

struct SRow {
};

int main() {
    CTableImpl<SRow> t;
    t.GetDouble(1, 1);
}
// g++ --std=c++11 test.cpp


```

##### Derived class template CRTP

```C++
#include <iostream>

template<typename UstTag, typename SpiType>
class CUstApi {
protected:
    template<typename ServiceImpl, typename ServiceTrait>
    struct CServiceRspBase {
    public:
        CServiceRspBase() {

        }

        int Run() {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            // 回调
            return static_cast<ServiceImpl *>(this)->CallBack();
        }
    };
};

template<typename UstTag, typename SpiType>
class CUstApiImpl : public CUstApi<UstTag, SpiType> {
protected:
    template<typename ServiceTrait>
    struct CServiceMultiSpanRsp
            : CUstApi<UstTag, SpiType>::template CServiceRspBase<CServiceMultiSpanRsp<ServiceTrait>, ServiceTrait> {
        friend typename CUstApi<UstTag, SpiType>::template CServiceRspBase<CServiceMultiSpanRsp<ServiceTrait>, ServiceTrait>;
    protected:
        int CallBack() {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            return 0;
        }

    };

public:
    class CTestServiceTrait {
    };

    void Test() {
        CServiceMultiSpanRsp<CTestServiceTrait> s;
        s.Run();
    }
};

struct TestUstTag {
};
struct TestSpiType {
};

int main() {
    CUstApiImpl<TestUstTag, TestSpiType> Api;
    Api.Test();
}
// g++ test.cpp -Wall -pedantic

```

如果将 `struct CServiceMultiSpanRsp: CUstApi<UstTag, SpiType>::CServiceRspBase<CServiceMultiSpanRsp<ServiceTrait>, ServiceTrait>` 中的 `template` 拿掉，则编译报错如下:

```C++
test.cpp:29:57: error: non-template ‘CServiceRspBase’ used as template
  struct CServiceMultiSpanRsp: CUstApi<UstTag, SpiType>::CServiceRspBase<CServiceMultiSpanRsp<ServiceTrait>, ServiceTrait>
                                                         ^~~~~~~~~~~~~~~
test.cpp:29:57: note: use ‘CUstApi<UstTag, SpiType>::template CServiceRspBase’ to indicate that it is a template
test.cpp:39:3: error: expected ‘{’ before ‘;’ token
  };

```



```C++
test.cpp:29:57: 错误：非模板‘CServiceRspBase’用作模板
  struct CServiceMultiSpanRsp: CUstApi<UstTag, SpiType>::CServiceRspBase<CServiceMultiSpanRsp<ServiceTrait>, ServiceTrait>
                                                         ^
test.cpp:29:57: 附注：使用‘CUstApi<UstTag, SpiType>::template CServiceRspBase’来表示这是一个模板
test.cpp:39:3: 错误：expected ‘{’ before ‘;’ token
  };

```

## TO READ

deque [Why template parameters of dependent type names cannot be deduced, and what to do about it](https://deque.blog/2017/10/12/why-template-parameters-of-dependent-type-names-cannot-be-deduced-and-what-to-do-about-it/)

stackoverflow [How do you understand dependent names in C++](https://stackoverflow.com/questions/1527849/how-do-you-understand-dependent-names-in-c)

gcc [14.7.2 Name Lookup, Templates, and Accessing Members of Base Classes](https://gcc.gnu.org/onlinedocs/gcc/Name-lookup.html)

stackoverflow [Where and why do I have to put the “template” and “typename” keywords?](https://stackoverflow.com/questions/610245/where-and-why-do-i-have-to-put-the-template-and-typename-keywords)

stackoverflow [Two phase lookup - explanation needed](https://stackoverflow.com/questions/7767626/two-phase-lookup-explanation-needed)

microsoft [Two-phase name lookup support comes to MSVC](https://devblogs.microsoft.com/cppblog/two-phase-name-lookup-support-comes-to-msvc/) 


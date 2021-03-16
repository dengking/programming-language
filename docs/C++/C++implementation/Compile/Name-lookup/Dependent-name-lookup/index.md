# 关于本章

对于template，C++ compiler有着特殊的处理，这是本章讨论的问题。

## thegreenplace [Dependent name lookup for C++ templates](https://eli.thegreenplace.net/2012/02/06/dependent-name-lookup-for-c-templates)

参见 `thegreenplace-Dependent-name-lookup-for-C++templates` 章节。



## TO READ

- stackoverflow [Two phase lookup - explanation needed](https://stackoverflow.com/questions/7767626/two-phase-lookup-explanation-needed)
- microsoft [Two-phase name lookup support comes to MSVC](https://devblogs.microsoft.com/cppblog/two-phase-name-lookup-support-comes-to-msvc/)

## cppreference [Two-phase name lookup](https://en.cppreference.com/w/cpp/language/two-phase_lookup)



## cppreference [Dependent names](https://en.cppreference.com/w/cpp/language/dependent_name)

Inside the definition of a [template](https://en.cppreference.com/w/cpp/language/templates) (both [class template](https://en.cppreference.com/w/cpp/language/class_template) and [function template](https://en.cppreference.com/w/cpp/language/function_template)), the meaning of some constructs may differ from one instantiation to another. In particular, types and expressions may depend on types of type template parameters and values of non-type template parameters.

> NOTE: 下面是对最后一句话的理解：
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
void g(double)
{
	std::cout << "g(double)\n";
}

template<class T>
struct S
{
	void f() const
	{
		g(1); // "g" is a non-dependent name, bound now
	}
};

void g(int)
{
	std::cout << "g(int)\n";
}

int main()
{
	g(1); // calls g(int)

	S<int> s;
	s.f(); // calls g(double)
}
// g++ test.cpp

```

> NOTE: 上述程序的输出如下:
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

As discussed in [lookup](https://en.cppreference.com/w/cpp/language/lookup), the lookup of a **dependent name** used in a template is postponed until the **template arguments** are known, at which time

- non-ADL lookup examines function declarations with external linkage that are visible from the *template definition* context
- [ADL](https://en.cppreference.com/w/cpp/language/adl) examines function declarations with external linkage that are visible from either the *template definition* context or the *template instantiation* context

(in other words, adding a new function declaration after template definition does not make it visible, except via ADL).

> NOTE: 综合上面描述的compiler进行lookup的两个规则，可知：
>
> |                                                              | *template definition* context | *template instantiation* context |
> | ------------------------------------------------------------ | ----------------------------- | -------------------------------- |
> | non-ADL lookup                                               | yes                           | no                               |
> | [ADL](https://en.cppreference.com/w/cpp/language/adl) lookup | yes                           | yes                              |
>
> 从中可以看出，在*template instantiation* context是不执行non-ADL lookup，后面会对此进行详细解释，尤其需要注意的是搞清楚这样做的原因。

(in other words, adding a new function declaration after template definition does not make it visible, except via ADL).

The purpose of this rule is to help guard against violations of the [ODR](https://en.cppreference.com/w/cpp/language/definition#One_Definition_Rule) for template instantiations:

```c++
#include <iostream>
#include <vector>

// an external library
namespace E
{
template<typename T>
void writeObject(const T &t)
{
	std::cout << "Value = " << t << '\n';
}
}

// translation unit 1:
// Programmer 1 wants to allow E::writeObject to work with vector<int>
namespace P1
{
std::ostream& operator<<(std::ostream &os, const std::vector<int> &v)
{
	for (int n : v)
		os << n << ' ';
	return os;
}
void doSomething()
{
	std::vector<int> v;
	E::writeObject(v); // error: will not find P1::operator<<
}
}

// translation unit 2:
// Programmer 2 wants to allow E::writeObject to work with vector<int>
namespace P2
{
std::ostream& operator<<(std::ostream &os, const std::vector<int> &v)
{
	for (int n : v)
		os << n << ':';
	return os << "[]";
}
void doSomethingElse()
{
	std::vector<int> v;
	E::writeObject(v); // error: will not find P2::operator<<
}
}
int main()
{
	P1::doSomething();
	P2::doSomethingElse();
}
// g++ --std=c++11 test.cpp

```

> NOTE: 上述程序编译报错，报错如下:
>
> ```C++
> test.cpp: In instantiation of ‘void E::writeObject(const T&) [with T = std::vector<int>]’:
> test.cpp:27:18:   required from here
> test.cpp:10:26: 错误：无法将左值‘std::basic_ostream<char>’绑定到‘std::basic_ostream<char>&&’
>   std::cout << "Value = " << t << '\n';
> 
> ```
>
> 这是因为compiler无法找到用户重载的`std::ostream& operator<<(std::ostream &os, const std::vector<int> &v)`，原因如下:
>
> - 通过前面描述的规则可知，non-ADL lookup for `operator<<` were allowed from the **instantiation context**，所以无法找到`namespace P1`中的`std::ostream& operator<<(std::ostream &os, const std::vector<int> &v)`；
> - `std::vector<int>`不是user-defined type，因此不会触发ADL，所以无法找到在`namespace P1`中提供的上述重载。
>
> 下面是另外一个example，它 通过引入user-define type来触发ADL，从而使程序能够编译通过。

In the above example, if non-ADL lookup for `operator<<` were allowed from the instantiation context, the instantiation of `E::writeObject<vector<int>>` would have two different definitions: one using `P1::operator<<` and one using `P2::operator<<`. Such ODR violation may not be detected by the linker, leading to one or the other being used in both instances.

To make ADL examine a user-defined namespace, either `std::vector` should be replaced by a user-defined class or its element type should be a user-defined class:

```c++
#include <iostream>
#include <vector>

// an external library
namespace E
{
template<typename T>
void writeObject(const T &t)
{
	std::cout << "Value = " << t << '\n';
}
}

// translation unit 1:
// Programmer 1 wants to allow E::writeObject to work with vector<int>
namespace P1
{
class C
{

};
std::ostream& operator<<(std::ostream &os, const C &v)
{
	os << "class P1::C";
	return os;
}
std::ostream& operator<<(std::ostream &os, const std::vector<C> &v)
{
	for (C n : v)
		os << n << ' ';
	return os;
}
void doSomething()
{
	std::vector<C> v;
	v.emplace_back();
	v.emplace_back();
	v.emplace_back();
	E::writeObject(v); // error: will not find P1::operator<<
}
}

// translation unit 2:
// Programmer 2 wants to allow E::writeObject to work with vector<int>
namespace P2
{
class C
{

};
std::ostream& operator<<(std::ostream &os, const C &v)
{
	os << "class P2::C";
	return os;
}
std::ostream& operator<<(std::ostream &os, const std::vector<C> &v)
{
	for (C n : v)
		os << n << ' ';
	return os;
}
void doSomethingElse()
{
	std::vector<C> v;
	v.emplace_back();
	v.emplace_back();
	v.emplace_back();
	E::writeObject(v); // error: will not find P2::operator<<
}
}

int main()
{
	P1::doSomething();
	P2::doSomethingElse();
}
// g++ --std=c++11 test.cpp

```



Note: this rule makes it impractical to overload operators for standard library types

> NOTE: 

```C++
#include <iostream>
#include <vector>
#include <iterator>
#include <utility>
// Bad idea: operator in global namespace, but its arguments are in std::
std::ostream& operator<<(std::ostream &os, std::pair<int, double> p)
{
	return os << p.first << ',' << p.second;
}

int main()
{
	typedef std::pair<int, double> elem_t;
	std::vector<elem_t> v(10);
	std::cout << v[0] << '\n'; // OK, ordinary lookup finds ::operator<<
	std::copy(v.begin(), v.end(), std::ostream_iterator < elem_t > (std::cout, " ")); // Error: both ordinary
	// lookup from the point of definition of std::ostream_iterator and ADL will
	// only consider the std namespace, and will find many overloads of
	// std::operator<<, so the lookup will be done. Overload resolution will then
	// fail to find operator<< for elem_t in the set found by the lookup.
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
namespace std
{
// Bad idea: operator in global namespace, but its arguments are in std::
std::ostream& operator<<(std::ostream &os, std::pair<int, double> p)
{
	return os << p.first << ',' << p.second;
}
}

int main()
{
	typedef std::pair<int, double> elem_t;
	std::vector<elem_t> v(10);
	std::cout << v[0] << '\n'; // OK, ordinary lookup finds ::operator<<
	std::copy(v.begin(), v.end(), std::ostream_iterator < elem_t > (std::cout, " ")); // Error: both ordinary
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

template<class T> class A
{
	A *p1;    // A is the current instantiation
	A<T> *p2; // A<T> is the current instantiation
	::A<T> *p4; // ::A<T> is the current instantiation
	A<T*> p3; // A<T*> is not the current instantiation
	class B
	{
		B *p1; // B is the current instantiation
		A<T>::B *p2; // A<T>::B is the current instantiation
		typename A<T*>::B *p3; // A<T*>::B is not the current instantiation
	};
};
template<class T> class A<T*>
{
	A<T*> *p1;  // A<T*> is the current instantiation
	A<T> *p2;   // A<T> is not the current instantiation
};
template<int I> struct B
{
	static const int my_I = I;
	static const int my_I2 = I + 0;
	static const int my_I3 = my_I;
	B<my_I> *b3;  // B<my_I> is the current instantiation
	B<my_I2> *b4; // B<my_I2> is not the current instantiation
	B<my_I3> *b5; // B<my_I3> is the current instantiation
};

int main()
{
	B<5> b;

	A<int> a1;
	A<int*> a2;
}
// g++ --std=c++11 test.cpp

```





### The `typename` disambiguator for dependent names

```c++
#include <iostream>
#include <vector>

int p = 1;
template<typename T>
void foo(const std::vector<T> &v)
{

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
struct S
{
	typedef int value_t; // member of current instantiation
	void f()
	{
		S<T>::value_t n { };  // S<T> is dependent, but 'typename' not needed
		std::cout << n << '\n';
	}
};

int main()
{
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
struct S
{
	template<typename U> void foo()
	{
	}
};

template<typename T>
void bar()
{
	S<T> s;
	s.foo<T>(); // error: < parsed as less than operator
	s.template foo<T>(); // OK
}

```

> NOTE: 下面是一个补充的例子，更加复杂一些

```C++
#include "stddef.h" // size_t
#include <vector>

typedef struct SFieldInfo
{
	/**
	 * 字段在结构体中的偏移位置
	 */
	size_t m_iOffset;
};

class CStructRtti
{

	std::vector<SFieldInfo> m_FieldMetaList;
public:
	template<typename ValueType>
	const ValueType* GetValue(void *Data, int Index) const
	{
		const SFieldInfo &FieldMeta = m_FieldMetaList[Index];
		size_t iOffset = FieldMeta.m_iOffset;
		char *DataStartAddress = reinterpret_cast<char*>(Data) + iOffset;
		return reinterpret_cast<const ValueType*>(DataStartAddress);
	}
};

class CTableInterface
{
protected:
	/**
	 * 列信息
	 */
	CStructRtti *m_Columns { nullptr };
public:
	virtual double GetDouble(size_t RowID, size_t ColID)=0;
};

template<typename RowType>
class CTableImpl: public CTableInterface
{
	RowType m_Row { };
public:
	double GetDouble(size_t RowID, size_t ColID) override
	{
		return *m_Columns->template GetValue<double>(&m_Row, ColID); // 必须要使用 The `template` disambiguator for dependent names
	}
};

struct SRow
{

};

int main()
{
	CTableImpl<SRow> t;
	t.GetDouble(1, 1);
}
// g++ --std=c++11 test.cpp


```



## TO READ

- deque [Why template parameters of dependent type names cannot be deduced, and what to do about it](https://deque.blog/2017/10/12/why-template-parameters-of-dependent-type-names-cannot-be-deduced-and-what-to-do-about-it/)
- stackoverflow [How do you understand dependent names in C++](https://stackoverflow.com/questions/1527849/how-do-you-understand-dependent-names-in-c)
- gcc [14.7.2 Name Lookup, Templates, and Accessing Members of Base Classes](https://gcc.gnu.org/onlinedocs/gcc/Name-lookup.html)
- stackoverflow [Where and why do I have to put the “template” and “typename” keywords?](https://stackoverflow.com/questions/610245/where-and-why-do-i-have-to-put-the-template-and-typename-keywords)



## Examples

### error: expected primary-expression before ‘>’ token

这是一种非常常见的compile错误，在thegreenplace [Dependent name lookup for C++ templates](https://eli.thegreenplace.net/2012/02/06/dependent-name-lookup-for-c-templates) `#` "Disambiguating dependent template names"段中介绍了这种错误，下面再补充一些例子: 

1) stackoverflow [C++ template compilation error: expected primary-expression before ‘>’ token](https://stackoverflow.com/questions/3505713/c-template-compilation-error-expected-primary-expression-before-token)

[A](https://stackoverflow.com/a/3505738)

You need to do:

```c++
std::cout << pt.template get<std::string>("path");
```

Use `template` in the same situation as `typename`, except for template members instead of types.

(That is, since `pt::get` is a template member *dependent* on a template parameter, you need to tell the compiler it's a template.)

### error: there are no arguments to ‘`***`’ that depend on a template parameter, so a declaration of ‘`***`’ must be available

这是一种常见的compiler错误，在thegreenplace [Dependent name lookup for C++ templates](https://eli.thegreenplace.net/2012/02/06/dependent-name-lookup-for-c-templates) `#` "A simple problem and a solution"中介绍了这种错误，下面再补充一些例子:

stackoverflow [Inheritance and templates in C++ - why are inherited members invisible?](https://stackoverflow.com/questions/1567730/inheritance-and-templates-in-c-why-are-inherited-members-invisible)

```C++
template <int a>
class Test {
public:
    Test() {}
    int MyMethod1() { return a; }
};

template <int b>
class Another : public Test<b>
{
public:
    Another() {}
    void MyMethod2() {
        MyMethod1();
    }
};

int main()
{
    Another<5> a;
    a.MyMethod1();
    a.MyMethod2();
}
// g++ test.cpp

```

编译报错如下: 

```c++
test.cpp: In member function ‘void Another<b>::MyMethod2()’:
test.cpp:14:19: error: there are no arguments to ‘MyMethod1’ that depend on a template parameter, so a declaration of ‘MyMethod1’ must be available [-fpermissive]
         MyMethod1();
                   ^
test.cpp:14:19: note: (if you use ‘-fpermissive’, G++ will accept your code, but allowing the use of an undeclared name is deprecated)
```

[A](https://stackoverflow.com/a/1567781)

```C++
template <int a>
class Test {
public:
    Test() {}
    int MyMethod1() { return a; }
};

template <int b>
class Another : public Test<b>
{
public:
    Another() {}
    void MyMethod2() {
        Test<b>::MyMethod1();
    }
};

int main()
{
    Another<5> a;
    a.MyMethod1();
    a.MyMethod2();
}
// g++ test.cpp

```



### ‘`***`’ does not name a type

这是一种常见的compiler错误，在thegreenplace [Dependent name lookup for C++ templates](https://eli.thegreenplace.net/2012/02/06/dependent-name-lookup-for-c-templates) `#` "Disambiguating dependent type names"中介绍了这种错误，下面再补充一些例子:

stackoverflow [Propagating 'typedef' from based to derived class for 'template'](https://stackoverflow.com/questions/1643035/propagating-typedef-from-based-to-derived-class-for-template)



```C++
#include <iostream>
#include <vector>

template<typename T>
class A
{
public:
	typedef std::vector<T> Vec_t;
};

template<typename T>
class B: public A<T>
{
private:
	Vec_t v;  // fails - Vec_t is not recognized
};
int main()
{
	B<int> b;
}
// g++ test.cpp

```

编译报错如下:

```C++
test.cpp:15:2: error: ‘Vec_t’ does not name a type
  Vec_t v;  // fails - Vec_t is not recognized
  ^
test.cpp:15:2: note: (perhaps ‘typename A<T>::Vec_t’ was intended)
```

正确写法:

```C++
#include <iostream>
#include <vector>

template<typename T>
class A
{
public:
	typedef std::vector<T> Vec_t;
};

template<typename T>
class B: public A<T>
{
private:
	typename A<T>::Vec_t v;  // fails - Vec_t is not recognized
};
int main()
{
	B<int> b;
}
// g++ test.cpp

```


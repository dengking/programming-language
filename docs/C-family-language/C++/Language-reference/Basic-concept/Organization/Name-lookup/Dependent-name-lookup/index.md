# 关于本章

对于template，C++ compiler有着特殊的处理，这是本章讨论的问题。



## thegreenplace [Dependent name lookup for C++ templates](https://eli.thegreenplace.net/2012/02/06/dependent-name-lookup-for-c-templates)

### A simple problem and a solution

```c++
#include <iostream>

template<typename T> struct Base
{
	void f()
	{
		std::cerr << "Base<T>::f\n";
	}
};

template<typename T> struct Derived: Base<T>
{
	void g()
	{
		std::cerr << "Derived<T>::g\n  ";
		f();
	}
};
int main()
{
	Derived<int> D;
}
// g++ test.cpp

```

The intention of `Derived<T>::g` is to call `Base<T>::f`, but what the compiler does instead is produce this error:

```c++
: In member function ‘void Derived<T>::g()’:
:18:10: error: there are no arguments to ‘f’ that depend on a template parameter, so a declaration of ‘f’ must be available
:18:10: note: (if you use ‘-fpermissive’, G++ will accept your code, but allowing the use of an undeclared name is deprecated)
```

> NOTE: 中文如下:
>
> ```c++
> test.cpp: 在成员函数‘void Derived<T>::g()’中:
> test.cpp:16:5: 错误：‘f’的实参不依赖模板参数，所以‘f’的声明必须可用 [-fpermissive]
>    f();
>      ^
> test.cpp:16:5: 附注：(如果您使用‘-fpermissive’，G++ 会接受您的代码，但是允许使用未定义的名称是不建议使用的风格)
> 
> ```
>
> 

First, let's see how to fix this. It's easy. All you have to do is to make the compiler understand that the call `f` depends on the template parameter `T`. A couple of ways to do this are replacing `f()` with `Base<T>::f()`, or with `this->f()` (since `this` is implicitly dependent on `T`). For example:

```c++
#include <iostream>

template<typename T> struct Base
{
	void f()
	{
		std::cerr << "Base<T>::f\n";
	}
};

template<typename T> struct Derived: Base<T>
{
	void g()
	{
		std::cerr << "Derived<T>::g\n  ";
		this->f();
	}
};

int main()
{
	Derived<float> df;
	df.g();

	Derived<int> di;
	di.g();
	return 0;
}
// g++ test.cpp

```

`main` instantiates two `Derived` objects, parametrized for different types, for reasons that will soon become apparent. This code compiles without errors and prints:

```C++
Derived<T>::g
  Base<T>::f
Derived<T>::g
  Base<T>::f
```

Problem fixed. Now, let's understand what's going on. Why does the compiler need an explicit specification for which `f` to call? Can't it figure out on its own that we want it to call `Base<T>::f`? It turns out it can't, because this isn't correct in the general case. Suppose that a specialization of the `Base` class is later created for `int`, and it also defines `f`:

```c++
#include <iostream>

template<typename T> struct Base
{
	void f()
	{
		std::cerr << "Base<T>::f\n";
	}
};

template<typename T> struct Derived: Base<T>
{
	void g()
	{
		std::cerr << "Derived<T>::g\n  ";
		this->f();
        // Base<T>::f(); // 这种写法也是允许的
	}
};

template<>
struct Base<int>
{
	void f()
	{
		std::cerr << "Base<int>::f\n";
	}
};

int main()
{
	Derived<float> df;
	df.g();

	Derived<int> di;
	di.g();
	return 0;
}
// g++ test.cpp

```

With this **specialization** in place, the `main` from the sample above would actually print:

```c++
Derived<T>::g
  Base<T>::f
Derived<T>::g
  Base<int>::f
```

This is the correct behavior. The `Base` template has been specialized for `int`, so it should be used for **inheritance** when `Derived<int>` is required. But how does the compiler manage to figure it out? After all, `Base<int>` was defined *after* `Derived`!

### Two-phase name lookup

To make this work, the C++ standard defines a "two-phase name lookup" rule for names in templates. Names inside templates are divided to two types:

| type            | explanation                                                  | example |
| --------------- | ------------------------------------------------------------ | ------- |
| *Dependent*     | names that depend on the template parameters but aren't declared within the template. |         |
| *Non-dependent* | names that don't depend on the template parameters, plus the name of the template itself and names declared within it. |         |

When the compiler tries to resolve some name in the code, it first decides whether the name is **dependent** or not, and the resolution process stems from this distinction（解析过程就是根据这种区别进行的）. While **non-dependent names** are resolved "normally" - when the template is defined, the resolution for **dependent names** happens at the point of the **template's *instantiation***. This is what ensures that a specialization can be noticed correctly in the example above.

> NOTE: 关于template instantiation，参见`C++\Language-reference\Template\Implementation\index.md`

Now, back to our original problem. Why doesn't the compiler look `f` up in the base class? First, notice that in the call to `f()` in the first code snippet, `f` is a **non-dependent name**. So it must be resolved at the point of the **template's definition**. At that point, the compiler still doesn't know what `Base<T>::f` is, because it can be **specialized** later. So it doesn't look names up in the base class, but only in the enclosing scope. Since there's no `f` in the enclosing scope, the compiler complains.

> TODO:"At that point, the compiler still doesn't know what `Base<T>::f` is, because it can be **specialized** later. So it doesn't look names up in the base class, but only in the enclosing scope. " 这段话需要进一步分析。

On the other hand, when we explicitly make the lookup of `f` dependent by calling it through `this->`, the lookup rule changes. Now `f` is resolved at the point of the **template's instantiation**, where the compiler has full understanding of the base class and can resolve the name correctly.

### Disambiguating dependent type names

> NOTE: “disambiguate” 即 “消除歧义”

I've mentioned above that to fix the problem and make the lookup of `f` dependent, we can either say `this->f()` or `Base<T>::f()`. While this works for **identifiers** like **member names**, it doesn't work with types. Consider this code snippet:

> NOTE: 
>
> dependent name可以有如下三种:
>
> | dependent name | example                                                      | how to disambiguate                                          |
> | -------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
> | identifier     | 上面例子中的`f`是identifier                                  | compiler的default behavior将dependent name当做identifier     |
> | type           | 下面例子中的`MyType`是type                                   | 通过keyword `typename`来告诉compiler，这个name表示的是type   |
> | method         | “Disambiguating dependent template names”中的例子中的`foo_method`是 function template | 通过keyword `template`来告诉compiler，这个name表示的是template |
>
> 

```c++
#include <iostream>

template<typename T> struct Base
{
	typedef int MyType;
};

template<typename T> struct Derived: Base<T>
{
	void g()
	{
		// A. error: ‘MyType’ was not declared in this scope
		// MyType k = 2;

		// B. error: need ‘typename’ before ‘Base<T>::MyType’ because
		// ‘Base<T>’ is a dependent scope
		// Base<T>::MyType k = 2;

		// C. works!
		typename Base<T>::MyType k = 2;

		std::cerr << "Derived<T>::g --> " << k << "\n";
	}
};

int main()
{
	Derived<float> df;
	df.g();
	return 0;
}
// g++ test.cpp

```

Three attempts are shown to declare a **local variable** `k` of type `MyType`. The first two are commented out because they result in compile errors. (A) should be obvious by now - since `MyType` is non-dependent, it can't be found in the **base class** - same problem as before.

But why doesn't (B) work? Well, because `Base<T>` can be specialized, so the compiler can't be sure whether `MyType` is a **type** or not. A **specialization** can easily declare a method called `MyType` instead of it being a **type**. And neither can the compiler delay this decision until the **instantiation point**, because whether `MyType` is a type or not affects how the rest of the definition is *parsed*. So we must tell the compiler explicitly, at the point of definition, whether `MyType` is a type or not. It turns out that the default is "not a type", and we must precede the name with `typename` to tell the compiler it *is* a type. This is stated in the `C++` standard, section 14.6:

> A name used in a template declaration or definition and that is dependent on a template-parameter is assumed not to name a type unless the applicable name lookup finds a type name or the name is qualified by the keyword `typename`.



> NOTE: 因为 `MyType` 是否是一个类型会影响compiler对后面代码的解析，因此，compiler也不能将此决定延迟到**instantiation point**，所以我们必须在定义的时候明确地告诉编译器`MyType`是否是一个类型。结果是，默认值是“不是类型”，我们必须在名称前面加上`typename`，以告诉编译器它*是*类型。这是在“c++”标准14.6节中规定的:

### Disambiguating dependent template names

While we're at it, the following is yet another example of explicit disambiguation that is sometimes required to guide the compiler when templates and specializations are involved:

```c++
struct Foo {
    template<typename U>
    static void foo_method()
    {
    }
};

template<typename T> void func(T* p) {
    // A. error: expected primary-expression before ‘>’ token
    // T::foo_method<T>();

    // B. works!
    T::template foo_method<T>();
}
```



The first attempt to call `T::foo_method` fails - the compiler can't parse the code. As explained before, when a **dependent name** is encountered, it is assumed to be some sort of **identifier** (such as a function or variable name). Previously we've seen how to use the `typename` keyword to explicitly tell the compiler that it deals with a **type**.

So in declaration (A) above can't be parsed, because the compiler assumes `foo_method` is just a member function and interprets the `<` and `>` symbols as comparison operators. But `foo_method` is a template, so we have to notify the compiler about it. As declaration (B) demonstrates, this can be done by using the keyword `template`.

> NOTE:关于primary-expression，参见`C++\Language-reference\Expressions\Expressions.md`。
>
> 写法A中，compiler将`T::foo_method<T>()`中的`<`解析为less than operator，将`>`解析为more than operator，而C++规定: 
>
> > The operands of any operator may be other expressions or primary expressions
>
> 按照C++的规定，`T::foo_method<T`不是一个primary expression，所以compiler explain。
>
>  实际上`foo_method`是一个function template。

### Resources

The following resources have been helpful in the preparation of this article and contain additional information if you're interested to dig deeper:

- [Name lookup](http://gcc.gnu.org/onlinedocs/gcc/Name-lookup.html) in the g++ docs
- [C++ templates FAQ](http://womble.decadent.org.uk/c++/template-faq.html)
- [C++ FAQ Lite](http://www.parashift.com/c++-faq-lite/templates.html), section 18
- C++11 standard, working draft N3242, section 14.6



## TO READ

- stackoverflow [Two phase lookup - explanation needed](https://stackoverflow.com/questions/7767626/two-phase-lookup-explanation-needed)
- microsoft [Two-phase name lookup support comes to MSVC](https://devblogs.microsoft.com/cppblog/two-phase-name-lookup-support-comes-to-msvc/)

## cppreference [Two-phase name lookup](https://en.cppreference.com/w/cpp/language/two-phase_lookup)

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



## TO READ

- deque [Why template parameters of dependent type names cannot be deduced, and what to do about it](https://deque.blog/2017/10/12/why-template-parameters-of-dependent-type-names-cannot-be-deduced-and-what-to-do-about-it/)
- stackoverflow [How do you understand dependent names in C++](https://stackoverflow.com/questions/1527849/how-do-you-understand-dependent-names-in-c)
- gcc [14.7.2 Name Lookup, Templates, and Accessing Members of Base Classes](https://gcc.gnu.org/onlinedocs/gcc/Name-lookup.html)
- stackoverflow [Where and why do I have to put the “template” and “typename” keywords?](https://stackoverflow.com/questions/610245/where-and-why-do-i-have-to-put-the-template-and-typename-keywords)

## Examples

### error: expected primary-expression before ‘>’ token

这是一种非常常见的compile错误，在thegreenplace [Dependent name lookup for C++ templates](https://eli.thegreenplace.net/2012/02/06/dependent-name-lookup-for-c-templates) `#` "Disambiguating dependent template names"段中介绍了这种错误，下面再补充一些例子: 

stackoverflow [C++ template compilation error: expected primary-expression before ‘>’ token](https://stackoverflow.com/questions/3505713/c-template-compilation-error-expected-primary-expression-before-token)





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


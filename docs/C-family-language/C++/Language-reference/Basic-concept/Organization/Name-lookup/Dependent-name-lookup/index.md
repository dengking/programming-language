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

When the compiler tries to resolve some name in the code, it first decides whether the name is **dependent** or not, and the resolution process stems from this distinction（解析过程就是根据这种区别进行的）. While **non-dependent names** are resolved "normally" - when the template is defined, the resolution for **dependent names** happens at the point of the template's *instantiation*. This is what ensures that a specialization can be noticed correctly in the example above.

> NOTE: 关于template instantiation，参见`C++\Language-reference\Template\Implementation\index.md`

Now, back to our original problem. Why doesn't the compiler look `f` up in the base class? First, notice that in the call to `f()` in the first code snippet, `f` is a **non-dependent name**. So it must be resolved at the point of the **template's definition**. At that point, the compiler still doesn't know what `Base<T>::f` is, because it can be **specialized** later. So it doesn't look names up in the base class, but only in the enclosing scope. Since there's no `f` in the enclosing scope, the compiler complains.

On the other hand, when we explicitly make the lookup of `f` dependent by calling it through `this->`, the lookup rule changes. Now `f` is resolved at the point of the template's instantiation, where the compiler has full understanding of the base class and can resolve the name correctly.

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

> NOTE: 实际上`foo_method`是一个function template。

### Resources

The following resources have been helpful in the preparation of this article and contain additional information if you're interested to dig deeper:

- [Name lookup](http://gcc.gnu.org/onlinedocs/gcc/Name-lookup.html) in the g++ docs
- [C++ templates FAQ](http://womble.decadent.org.uk/c++/template-faq.html)
- [C++ FAQ Lite](http://www.parashift.com/c++-faq-lite/templates.html), section 18
- C++11 standard, working draft N3242, section 14.6





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
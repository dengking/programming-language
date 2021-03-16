# Expression SFINAE



## 为什么引入Expression SFINAE？

C++11 Expression SFINAE给予了programmer"迂回"地实现concept的权利，需要注意的是，前面这段话中使用的"迂回"这个词是因为:

1、相对于C++20 concept给予programmer直接、显式地支持concept而言，显然，这种方式是非直接的、迂回的，为了哪种方式，都能够达到同样的目的



### open-std [Solving the SFINAE problem for expressions](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2634.html)

> NOTE: 
>
> 1、这篇文章是提出expression SFNIAE的提案

```c++
#include <iostream>

template<int I> struct A
{
};

char xxx(int)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}
char xxx(float)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}

template<class T>
A<sizeof(xxx((T) 0))> f(T)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}

int main()
{
	f(1);
}
// g++ --std=c++11 test.cpp
```

> NOTE: 上述程序的输出：
>
> ```c++
> A<sizeof (xxx((T)(0)))> f(T) [with T = int]
> ```
>
> 

#### The proposed solution

```c++
#include <iostream>

struct X
{
};
struct Y
{
	Y(X)
	{
	}
};

template<class T> auto f(T t1, T t2) -> decltype(t1 + t2) // #1
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}
X f(Y, Y)  // #2
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}
int main()
{
	X x1, x2;
	X x3 = f(x1, x2);  // deduction fails on #1 (cannot add X+X), calls #2
}
// g++ --std=c++11 test.cpp
```

> NOTE： 上述程序的输出:
>
> ```
> X f(Y, Y)
> ```









## 名称由来

Expression SFINAE中的"Expression"对应的是boost [Generic Programming Techniques](https://www.boost.org/community/generic_programming.html) # [concept](https://www.boost.org/community/generic_programming.html#concept) 中的 "**Valid Expressions**"，再加上 C++ SFINAE 机制，就形成了expression SFINAE。



## cppreference [Expression SFINAE (since C++11)](https://en.cppreference.com/w/cpp/language/sfinae#Expression_SFINAE)

The following expression errors are SFINAE errors

1、Ill-formed expression used in a template parameter type

2、Ill-formed expression used in the function type

> NOTE: 
>
> 一、上面这段话说明可以在如下两种情况下使用expression SFINAE
>
> 1、template parameter type
>
> 2、function type
>
> 经常使用的一种idiom是"A common idiom is to use **expression SFINAE** on the **return type**"，参见下面的"Detection idiom"章节
>
> 

```C++
#include <iostream>
struct X
{
};

struct Y
{
	Y(X)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};
// X is convertible to Y

template<class T>
auto f(T t1, T t2) -> decltype(t1 + t2) // overload #1
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}

X f(Y, Y)  // overload #2
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}

int main()
{
	X x1, x2;
	X x3 = f(x1, x2);  // deduction fails on #1 (expression x1+x2 is ill-formed)
					   // only #2 is in the overload set, and is called
	return 0;
}
// g++ --std=c++11 test.cpp -Wall -pedantic

```

> NOTE:
>
> 1、输出如下:
>
> ```c++
> Y::Y(X)
> Y::Y(X)
> X f(Y, Y)
> ```
>
> 2、可以看到，最终选择的是:
>
> ```c++
> X f(Y, Y)  // overload #2
> ```
>
> 

### Detection idiom

A common idiom is to use **expression SFINAE** on the **return type**, where the expression uses the **comma operator**, whose left subexpression is the one that is being examined (cast to `void` to ensure the **user-defined operator comma** on the returned type is not selected), and the right subexpression has the type that the function is supposed to return.

> NOTE: 
>
> 1、“cast to `void` to ensure the **user-defined operator comma** on the returned type is not selected”是利用[Discarded-value expressions](https://en.cppreference.com/w/cpp/language/expressions#Discarded-value_expressions)的性质。

```c++
#include <iostream>

// this overload is always in the set of overloads
// ellipsis parameter has the lowest ranking for overload resolution
void test(...)
{
	std::cout << "Catch-all overload called\n";
}

// this overload is added to the set of overloads if
// C is a reference-to-class type and F is a pointer to member function of C
template<class C, class F>
auto test(C c, F f) -> decltype((void)(c.*f)(), void())
{
	std::cout << "Reference overload called\n";
}

// this overload is added to the set of overloads if
// C is a pointer-to-class type and F is a pointer to member function of C
template<class C, class F>
auto test(C c, F f) -> decltype((void)((c->*f)()), void())
{
	std::cout << "Pointer overload called\n";
}

struct X
{
	void f()
	{
	}
};

int main()
{
	X x;
	test(x, &X::f);
	test(&x, &X::f);
	test(42, 1337);
}
// g++ --std=c++11 test.cpp -Wall -pedantic

```



## stackoverflow [What is “Expression SFINAE”?](https://stackoverflow.com/questions/12654067/what-is-expression-sfinae)

### [A](https://stackoverflow.com/a/12654277)

Expression SFINAE is explained quite well in the paper you linked, I think. It's [SFINAE](http://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error) on expressions. If the expression inside `decltype` isn't valid, well, kick the function from the VIP lounge（休息室） of overloads. You can find the normative wording at the end of this answer.

A note on VC++: They didn't implement it *completely*. On simple expressions, it might work, but on others, it won't. See a discussion in the comments [on this answer](https://stackoverflow.com/a/6194623/500104) for examples that fail. To make it simple, this won't work:

```cpp
#include <iostream>

// catch-all case
void test(...)
{
	std::cout << "Couldn't call\n";
}

// catch when C is a reference-to-class type and F is a member function pointer
template<class C, class F>
auto test(C c, F f) -> decltype((c.*f)(), void()) // 'C' is reference type
{
	std::cout << "Could call on reference\n";
}

// catch when C is a pointer-to-class type and F is a member function pointer
template<class C, class F>
auto test(C c, F f) -> decltype((c->*f)(), void()) // 'C' is pointer type
{
	std::cout << "Could call on pointer\n";
}

struct X
{
	void f()
	{
	}
};

int main()
{
	X x;
	test(x, &X::f);
	test(&x, &X::f);
	test(42, 1337);
}
// g++ --std=c++11 test.cpp
```

> NOTE: 上述程序的输出如下:
>
> ```
> Could call on reference
> Could call on pointer
> Couldn't call
> ```
>
> 

With `Clang`, this outputs the expected:

> Could call with reference
> Could call with pointer
> Couldn't call

With MSVC, I get... well, a compiler error:

```c++
1>src\main.cpp(20): error C2995: ''unknown-type' test(C,F)' : function template has already been defined
1>          src\main.cpp(11) : see declaration of 'test'
```

It also seems that GCC 4.7.1 isn't quite up to the task:

```c++
source.cpp: In substitution of 'template decltype ((c.*f(), void())) test(C, F) [with C = X*; F = void (X::*)()]':
source.cpp:29:17:   required from here
source.cpp:11:6: error: cannot apply member pointer 'f' to 'c', which is of non-class type 'X*'
source.cpp: In substitution of 'template decltype ((c.*f(), void())) test(C, F) [with C = int; F = int]':
source.cpp:30:16:   required from here
source.cpp:11:6: error: 'f' cannot be used as a member pointer, since it is of type 'int'
```

A common use of Expression SFINAE is when defining traits, like a trait to check if a class sports a certain member function:

```cpp
#include <iostream>
#include <type_traits>
#include <vector>

struct has_member_begin_test
{
	template<class U>
	static auto test(U* p) -> decltype(p->begin(), std::true_type());
	template<class >
	static auto test(...) -> std::false_type;
};

template<class T>
struct has_member_begin
: decltype(has_member_begin_test::test<T>(0))
{
};

int main()
{
	std::cout << std::boolalpha;
	std::cout << has_member_begin<int>::value << std::endl;
}
// g++ --std=c++11 test.cpp
```

> NOTE: 上述程序输出如下：
>
> ```
> false
> ```



[Live example.](http://liveworkspace.org/code/45043173e3b62ee33deaeec3e609516a) (Which, surprisingly, works again on GCC 4.7.1.)

See also [this answer of mine](https://stackoverflow.com/a/9154394/500104), which uses the same technique in another environment (aka without traits).




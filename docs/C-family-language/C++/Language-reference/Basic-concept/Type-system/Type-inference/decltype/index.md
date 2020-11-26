# `decltype`

我们首先学习`decltype`解决的问题，然后描述`decltype`的一些具体细节。

## wikipedia [decltype](https://en.wikipedia.org/wiki/Decltype)



In 2002, [Bjarne Stroustrup](https://en.wikipedia.org/wiki/Bjarne_Stroustrup) proposed that a standardized version of the operator be added to the C++ language, and suggested the name "decltype", to reflect that the operator would yield the "declared type" of an expression.

> NOTE: 所谓“declared type”，其实就是[Static type](https://en.cppreference.com/w/cpp/language/type#Static_type)

Like the `sizeof` operator, `decltype`'s operand is not evaluated.

> NOTE: 关于此，参见cppreference [Expressions#Unevaluated_expressions](https://en.cppreference.com/w/cpp/language/expressions#Unevaluated_expressions) 



### [Motivation](https://en.wikipedia.org/wiki/Decltype#Motivation)

> NOTE: 搞清楚motivation非常重要

One of the cited main motivations for the `decltype` proposal was the ability to write **perfect [forwarding function](https://en.wikipedia.org/wiki/Wrapper_function) templates**.[[8\]](https://en.wikipedia.org/wiki/Decltype#cite_note-n1705-8)  It is sometimes desirable to write a generic forwarding function that returns the same type as the wrapped function, regardless of the type it is instantiated with. Without `decltype`, it is not generally possible to accomplish this.[[8\]](https://en.wikipedia.org/wiki/Decltype#cite_note-n1705-8) An example, which also utilizes the *[trailing-return-type](https://en.wikipedia.org/wiki/Trailing-return-type)*:[[8\]](https://en.wikipedia.org/wiki/Decltype#cite_note-n1705-8)

> NOTE: 关于 "**perfect [forwarding function](https://en.wikipedia.org/wiki/Wrapper_function) templates** "在microsoft [decltype (C++)](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019)的[Decltype and Forwarding Functions (C++11)](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019#decltype-and-forwarding-functions-c11)段中有着非常好的描述。

```c++
#include <iostream>
int g_IntValue = 1;
int& foo(int& i)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	return g_IntValue;
}
float foo(float& f)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	return 0.0;
}

template<class T>
auto transparent_forwarder(T& t) -> decltype(foo(t))
{
	return foo(t);
}

int main()
{
	int i = 0;
	float j = 0.0;
	transparent_forwarder(i);
	transparent_forwarder(j);
}
// g++ --std=c++11 test.cpp

```

> NOTE: 上述程序的输出如下:
>
> ```c++
> int& foo(int&)
> float foo(float&)
> ```
>
> 

`decltype` is essential here because it preserves the information about whether the wrapped function returns a reference type.[[9\]](https://en.wikipedia.org/wiki/Decltype#cite_note-msdn_decltype-9)

> NOTE: 这个例子所展示的是用`decltype`来实现“generic forwarding function ”



### [Semantics](https://en.wikipedia.org/wiki/Decltype#Semantics)

> NOTE: 移到了后面的`decltype` semantic章节。



## microsoft [decltype (C++)](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019)

The **`decltype`** type specifier yields the type of a specified expression. The **`decltype`** type specifier, together with the [`auto` keyword](https://docs.microsoft.com/en-us/cpp/cpp/auto-cpp?view=msvc-160), is useful primarily to developers who write **template libraries**. 

1) Use **`auto`** and **`decltype`** to declare a **template function** whose return type depends on the types of its template arguments. Or, 

2) Use **`auto`** and **`decltype`** to declare a **template function** that wraps a call to another function, and then returns the return type of the **wrapped function**.



### [Decltype and Auto](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019#decltype-and-auto)

In C++14, you can use `decltype(auto)` with no **trailing return type** to declare a **template function** whose **return type** depends on the types of its **template arguments**.

> NOTE: 后面会对这个用法进行详细的说明

In C++11, you can use the **`decltype`** type specifier on a trailing return type, together with the **`auto`** keyword, to declare a **template function** whose **return type** depends on the types of its **template arguments**. 

> NOTE: C++在type inference上的优化:
>
> 1) C++ 11 `auto` + `decltype` + trailing return type
>
> 2) C++14 `decltype(auto)`
>
> 从下面可以看出，上述两种方式的保证是向compiler表达  ***UNKNOWN* placeholder**、**late-specified return type**  ；这其实是一种**static polymorphism**(由于它发生于compile time)。看到了late，我想到了dynamic polymorphism的late-binding:
>
> `virtual` 告诉 compiler 使用 **late binding** ，从而实现 **dynamic polymorphism**；
>
> `decltype(auto)` 告诉 compiler 使用 **late-specified return type**，从而实现 **static polymorphism**；
>
> 



For example, consider the following code example in which the **return type** of the **template function** depends on the types of the template arguments. In the code example, the *UNKNOWN* placeholder indicates that the **return type** cannot be specified.

```C++
template<typename T, typename U>
UNKNOWN func(T&& t, U&& u){ return t + u; };
```

The introduction of the **`decltype`** type specifier enables a developer to obtain the type of the expression that the **template function** returns.

Use the *alternative function declaration syntax* that is shown later, the **`auto`** keyword, and the **`decltype`** type specifier to declare a *late-specified* return type. The **late-specified return type** is determined when the declaration is compiled, instead of when it is coded.

The following prototype illustrates the syntax of an alternative function declaration. Note that the **`const`** and **`volatile`** qualifiers, and the **`throw`** [exception specification](https://docs.microsoft.com/en-us/cpp/cpp/exception-specifications-throw-cpp?view=msvc-160) are optional. The *function_body* placeholder represents a compound statement that specifies what the function does. As a best coding practice, the *expression* placeholder in the **`decltype`** statement should match the expression specified by the **`return`** statement, if any, in the *function_body*.

![](./function-prototype.png)

In the following code example, the late-specified return type of the `myFunc` template function is determined by the types of the `t` and `u` template arguments. As a best coding practice, the code example also uses rvalue references and the `forward` function template, which support *perfect forwarding*. For more information, see [Rvalue Reference Declarator: &&](https://docs.microsoft.com/en-us/cpp/cpp/rvalue-reference-declarator-amp-amp?view=msvc-160).

```c++
#include <iostream>
#include <utility>

using namespace std;
//C++11
template<typename T, typename U>
auto myFunc(T&& t, U&& u) -> decltype (forward<T>(t) + forward<U>(u))
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	return forward<T>(t) + forward<U>(u);
}

////C++14
//template<typename T, typename U>
//decltype(auto) myFunc(T&& t, U&& u)
//{
//	return forward<T>(t) + forward<U>(u);
//}

int main()
{
	std::cout << myFunc(1, 2) << std::endl;
	std::cout << myFunc(1.1, 2.2) << std::endl;
}
// g++ --std=c++11 test.cpp
```

> NOTE: 输出如下：
>
> ```
> decltype ((forward<T>(t) + forward<U>(u))) myFunc(T&&, U&&) [with T = int; U = int; decltype ((forward<T>(t) + forward<U>(u))) = int]
> 3
> decltype ((forward<T>(t) + forward<U>(u))) myFunc(T&&, U&&) [with T = double; U = double; decltype ((forward<T>(t) + forward<U>(u))) = double]
> 3.3
> ```
>





### [Decltype and Forwarding Functions (C++11)](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019#decltype-and-forwarding-functions-c11)

**Forwarding functions** wrap calls to other functions. Consider a **function template** that forwards its arguments, or the results of an expression that involves those arguments, to another function. Furthermore, the **forwarding function** returns the result of calling the other function. In this scenario, the **return type** of the **forwarding function** should be the same as the return type of the wrapped function.

> NOTE: what is **Forwarding functions** ？在 wikipedia [decltype](https://en.wikipedia.org/wiki/Decltype) # [Motivation](https://en.wikipedia.org/wiki/Decltype#Motivation) 段中给出了链接: 
>
> > One of the cited main motivations for the `decltype` proposal was the ability to write **perfect [forwarding function](https://en.wikipedia.org/wiki/Wrapper_function) templates**.[[8\]](https://en.wikipedia.org/wiki/Decltype#cite_note-n1705-8) 
>
> 上面这段话总结了书写 一个 forwarding function 所面临的挑战。

In this scenario, you cannot write an appropriate type expression without the **`decltype`** type specifier. The **`decltype`** type specifier enables generic forwarding functions because it does not lose required information about whether a function returns a reference type. For a code example of a forwarding function, see the previous `myFunc` template function example.

#### Examples

The following code example declares the **late-specified return type** of template function `Plus()`. The `Plus` function processes its two operands with the **`operator+`** overload. Consequently, the interpretation of the plus operator (**`+`**) and the return type of the `Plus` function depends on the types of the function arguments.

```c++
// decltype_1.cpp
// compile with: cl /EHsc decltype_1.cpp

#include <iostream>
#include <string>
#include <utility>
#include <iomanip>

using namespace std;

template<typename T1, typename T2>
auto Plus(T1 &&t1, T2 &&t2) ->
decltype(forward<T1>(t1) + forward<T2>(t2))
{
	return forward<T1>(t1) + forward<T2>(t2);
}

class X
{
	friend X operator+(const X &x1, const X &x2)
	{
		return X(x1.m_data + x2.m_data);
	}

public:
	X(int data) :
					m_data(data)
	{
	}
	int Dump() const
	{
		return m_data;
	}
private:
	int m_data;
};

int main()
{
	// Integer
	int i = 4;
	cout << "Plus(i, 9) = " << Plus(i, 9) << endl;

	// Floating point
	float dx = 4.0;
	float dy = 9.5;
	cout << setprecision(3) << "Plus(dx, dy) = " << Plus(dx, dy) << endl;

	// String
	string hello = "Hello, ";
	string world = "world!";
	cout << Plus(hello, world) << endl;

	// Custom type
	X x1(20);
	X x2(22);
	X x3 = Plus(x1, x2);
	cout << "x3.Dump() = " << x3.Dump() << endl;
}

// g++ --std=c++11 test.cpp

```



> NOTE: 输出如下:
>
> ```C++
> Plus(i, 9) = 13
> Plus(dx, dy) = 13.5
> Hello, world!
> x3.Dump() = 42
> ```
>
> 

**Visual Studio 2017 and later:** The compiler parses **`decltype`** arguments when the templates are **declared** rather than **instantiated**. Consequently, if a **non-dependent specialization** is found in the **`decltype`** argument, it will not be deferred(推迟) to instantiation-time and will be processed immediately and any resulting errors will be diagnosed at that time.

> NOTE: 这里描述了VS实现 `decltype` 的细节。

The following example shows such a **compiler error** that is raised at the point of declaration:

```c++
#include <utility>
template<class T, class ReturnT, class ... ArgsT>
class IsCallable
{
public:
	struct BadType
	{
	};
	template<class U>
	static decltype(std::declval<T>()(std::declval<ArgsT>()...)) Test(int); //C2064. Should be declval<U>
	template<class U>
	static BadType Test(...);
	static constexpr bool value = std::is_convertible<decltype(Test<T>(0)), ReturnT>::value;
};

int main()
{
	constexpr bool test1 = IsCallable<int(), int>::value;
	static_assert(test1, "PASS1");
	constexpr bool test2 = !IsCallable<int*, int>::value;
	static_assert(test2, "PASS2");
}

```

> NOTE: 这个例子没有读懂





## cppreference [decltype specifier](https://en.cppreference.com/w/cpp/language/decltype)



### [Explanation](https://en.cppreference.com/w/cpp/language/decltype#Explanation)

> NOTE: 内容比较繁杂



## `decltype` semantic

在上述三篇文章中都描述了`decltype` semantic。

### wikipedia [decltype](https://en.wikipedia.org/wiki/Decltype) # [Semantics](https://en.wikipedia.org/wiki/Decltype#Semantics)

> NOTE: 总结的非常好，它从设计者的角度来进行说明，是比较容易帮助初学者准确把握的。

Similarly to the `sizeof` operator, the operand of `decltype` is **unevaluated**.[[11\]](https://en.wikipedia.org/wiki/Decltype#cite_note-n2343-11) Informally, the type returned by `decltype(e)` is deduced as follows:[[2\]](https://en.wikipedia.org/wiki/Decltype#cite_note-n1478-2)

1) If the expression `e` refers to a variable in local or namespace scope, a static member variable or a function parameter, then the result is that variable's or parameter's *declared type*

2) Otherwise, if `e` is an [lvalue](https://en.wikipedia.org/wiki/Value_(computer_science)), `decltype(e)` is `T&`, where `T` is the type of e; if e is an [xvalue](https://en.wikipedia.org/wiki/Value_(computer_science)), the result is `T&&`; otherwise, e is a [prvalue](https://en.wikipedia.org/wiki/Value_(computer_science)) and the result is `T`.

These semantics were designed to fulfill the needs of **generic library writers**, while at the same time being intuitive for novice programmers, because the return type of `decltype` always matches the type of the object or function exactly as declared in the source code.[[2\]](https://en.wikipedia.org/wiki/Decltype#cite_note-n1478-2) 

> NOTE: 设计者的角度

More formally, Rule 1 applies to **unparenthesized** *id-expression*s and class member access expressions.[[12\]](https://en.wikipedia.org/wiki/Decltype#cite_note-n2914-12)[[13\]](https://en.wikipedia.org/wiki/Decltype#cite_note-defects-13) Example:[[12\]](https://en.wikipedia.org/wiki/Decltype#cite_note-n2914-12) Note for added lines for `bar()`. Below the type deduced for "`bar()`" is plain `int`, not `const int`, because **prvalues of non-class types** always have **cv-unqualified types**, despite the statically declared different type.

> NOTE: " **prvalues of non-class types** always have **cv-unqualified types**"是非常理所当然的，因此`decltype`的设计是充分考虑到了所有的情况

```c++
const int&& foo();
const int bar();
int i;
struct A
{
	double x;
};
const A* a = new A();
decltype(foo()) x1; // type is const int&&
decltype(bar()) x2; // type is int
decltype(i) x3; // type is int
decltype(a->x) x4; // type is double
decltype((a->x)) x5; // type is const double&

```

The reason for the difference between the latter two invocations of `decltype` is that the parenthesized expression `(a->x)` is neither an *id-expression* nor a member access expression, and therefore does not denote a named object.[[14\]](https://en.wikipedia.org/wiki/Decltype#cite_note-closedissues-14) Because the expression is an lvalue, its deduced type is "reference to the type of the expression", or `const double&`.[[11\]](https://en.wikipedia.org/wiki/Decltype#cite_note-n2343-11)



> NOTE: 原文中的上述例子在下面的microsoft [decltype (C++)](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019)中有更好的解释







### microsoft [decltype (C++)](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019) # [Remarks](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=msvc-160#remarks)

The following code example demonstrates some uses of the **`decltype`** type specifier. First, assume that you have coded the following statements.



```cpp
int var;
const int&& fx();
struct A { double x; }
const A* a = new A();
```

Next, examine the types that are returned by the four **`decltype`** statements in the following table.

| Statement           | Type            | Notes                                                        |
| :------------------ | :-------------- | :----------------------------------------------------------- |
| `decltype(fx());`   | `const int&&`   | An [rvalue reference](https://docs.microsoft.com/en-us/cpp/cpp/rvalue-reference-declarator-amp-amp?view=msvc-160) to a **`const int`**. |
| `decltype(var);`    | **`int`**       | The type of variable `var`.                                  |
| `decltype(a->x);`   | **`double`**    | The type of the member access.                               |
| `decltype((a->x));` | `const double&` | The inner parentheses cause the statement to be evaluated as an expression instead of a member access. And because `a` is declared as a **`const`** pointer, the type is a reference to **`const double`**. |

### cppreference [decltype specifier](https://en.cppreference.com/w/cpp/language/decltype)





### Summary

在上述三篇文章中都描述了`decltype` semantic，其中最最简洁易懂的是microsoft [decltype (C++)](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019)的[Remarks](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019#remarks)段描述的，相比之下cppreference [decltype specifier](https://en.cppreference.com/w/cpp/language/decltype)的描述就难懂一些，所以最好将三篇结合起来：

#### Rule 1

> 1) If the argument is an **unparenthesized** [id-expression](https://en.cppreference.com/w/cpp/language/identifiers) or an **unparenthesized** [class member access](https://en.cppreference.com/w/cpp/language/operator_member_access) expression, then `decltype` yields the type of the *entity* named by this expression. If there is no such entity, or if the argument names a set of overloaded functions, the program is ill-formed.

这条规则源自cppreference [decltype specifier](https://en.cppreference.com/w/cpp/language/decltype)，microsoft [decltype (C++)](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019)中有同样的规则，这种是最最简单的情况。

#### Rule 2

> 2) If the *expression* parameter is a call to a function or an overloaded operator function, `decltype(expression)` is the return type of the function. Parentheses around an overloaded operator are ignored.

这条规则来自microsoft [decltype (C++)](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019)，cppreference [decltype specifier](https://en.cppreference.com/w/cpp/language/decltype)中没有这条规则。

#### Rule 3

> 3) If the argument is any other expression of type `T`, and
>
> a) if the [value category](https://en.cppreference.com/w/cpp/language/value_category) of *expression* is *xvalue*, then decltype yields `T&&`;
>
> b) if the value category of *expression* is *lvalue*, then decltype yields `T&`;
>
> c) if the value category of *expression* is *prvalue*, then decltype yields `T`.

这条规则来自cppreference [decltype specifier](https://en.cppreference.com/w/cpp/language/decltype)。



为什么这样设计呢？在维基百科[decltype#Semantics](https://en.wikipedia.org/wiki/Decltype#Semantics)中给出了解答：

> These semantics were designed to fulfill the needs of generic library writers, while at the same time being intuitive for novice programmers, because the return type of `decltype` always matches the type of the object or function exactly as declared in the source code.



> Note that if the name of an object is parenthesized, it is treated as an ordinary lvalue expression, thus `decltype(x)` and `decltype((x))` are often different types.

这是cppreference [decltype specifier](https://en.cppreference.com/w/cpp/language/decltype)中特别描述的一点，在三篇文章中，都结合具体的例子对它进行了解释：

```c++
struct A { double x; };
decltype(a->x) x4; // type is double
decltype((a->x)) x5; // type is const double&
```



## `decltype` is non-deduced context

参见：

1) cppreference `Template argument deduction#Non-deduced contexts`

2) stackoverflow [How does `void_t` work](https://stackoverflow.com/questions/27687389/how-does-void-t-work)



## Operands of `decltype` is unevaluated expressions

参见:

1) cppreference `Expressions#Unevaluated expressions`

2) wikipedia [decltype](https://en.wikipedia.org/wiki/Decltype) # [Semantics](https://en.wikipedia.org/wiki/Decltype#Semantics)

3) arne-mertz [Modern C++ Features – decltype and std::declval](https://arne-mertz.de/2017/01/decltype-declval/) # decltype does not execute anything

Whatever expression we pass to `decltype` does not get executed. That means, that we don’t pay any runtime overhead and don’t see any side effects. For example, `decltype(std::cout << "Hello world!\n")` will result in `std::ostream&`, but not a single character will be printed to our console.

```C++
#include <iostream>

int main()
{
	decltype(std::cout << "Hello world!\n") s = std::cout;
}
// g++ --std=c++11 test.cpp

```



```C++
#include <iostream>
class Foo;//forward declaration

int main()
{
	Foo f(int); //ok. Foo is still incomplete
	using f_result = decltype(f(11));
	//f_result is Foo
}
// g++ --std=c++11 test.cpp

```



## TO READ



https://riptutorial.com/cplusplus/example/18513/decltype

> NOTE: 这篇内容非常一般



https://arne-mertz.de/2017/01/decltype-declval/

> NOTE: 这篇内容非常一般
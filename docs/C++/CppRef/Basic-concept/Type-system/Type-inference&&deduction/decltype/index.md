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





## stackoverflow [What is decltype with two arguments?](https://stackoverflow.com/questions/16044514/what-is-decltype-with-two-arguments)



[A](https://stackoverflow.com/a/16044577)

```c++
#include <type_traits>

int main()
{
	static_assert(std::is_same<decltype(42, 3.14), double>::value, "Will not fire");
}

```

> NOTE: `42, 3.14`中的`,`为comma operator。



## TO READ



https://riptutorial.com/cplusplus/example/18513/decltype

> NOTE: 这篇内容非常一般



https://arne-mertz.de/2017/01/decltype-declval/

> NOTE: 这篇内容非常一般
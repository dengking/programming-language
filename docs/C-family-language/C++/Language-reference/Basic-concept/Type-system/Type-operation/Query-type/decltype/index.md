# decltype



## 维基百科[decltype](https://en.wikipedia.org/wiki/Decltype)

In 2002, [Bjarne Stroustrup](https://en.wikipedia.org/wiki/Bjarne_Stroustrup) proposed that a standardized version of the operator be added to the C++ language, and suggested the name "decltype", to reflect that the operator would yield the "declared type" of an expression.

> NOTE: 所谓“declared type”，其实就是[Static type](https://en.cppreference.com/w/cpp/language/type#Static_type)

Like the `sizeof` operator, `decltype`'s operand is not evaluated.

> NOTE: 关于此，参见cppreference [Expressions#Unevaluated_expressions](https://en.cppreference.com/w/cpp/language/expressions#Unevaluated_expressions) 

### [Motivation](https://en.wikipedia.org/wiki/Decltype#Motivation)

> NOTE: 搞清楚motivation非常重要

One of the cited main motivations for the `decltype` proposal was the ability to write perfect [forwarding function](https://en.wikipedia.org/wiki/Wrapper_function) templates.[[8\]](https://en.wikipedia.org/wiki/Decltype#cite_note-n1705-8) 

> NOTE: 这一点在原文中没有详细说明，参见microsoft [decltype (C++)](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019)的[Decltype and Forwarding Functions (C++11)](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019#decltype-and-forwarding-functions-c11)段

It is sometimes desirable to write a generic forwarding function that returns the same type as the wrapped function, regardless of the type it is instantiated with. Without `decltype`, it is not generally possible to accomplish this.[[8\]](https://en.wikipedia.org/wiki/Decltype#cite_note-n1705-8) An example, which also utilizes the *[trailing-return-type](https://en.wikipedia.org/wiki/Trailing-return-type)*:[[8\]](https://en.wikipedia.org/wiki/Decltype#cite_note-n1705-8)

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



### Semantics

> NOTE: 原文的这一段没有读懂

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

> NOTE: 原文中的上述例子在下面的microsoft [decltype (C++)](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019)中有更好的解释

## microsoft [decltype (C++)](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019)



### [Decltype and Auto](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019#decltype-and-auto)



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
> 



### [Decltype and Forwarding Functions (C++11)](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019#decltype-and-forwarding-functions-c11)



```c++
// decltype_1.cpp
// compile with: cl /EHsc decltype_1.cpp

#include <iostream>
#include <string>
#include <utility>
#include <iomanip>

using namespace std;

template<typename T1, typename T2>
auto Plus(T1&& t1, T2&& t2) ->
   decltype(forward<T1>(t1) + forward<T2>(t2))
{
   return forward<T1>(t1) + forward<T2>(t2);
}

class X
{
   friend X operator+(const X& x1, const X& x2)
   {
      return X(x1.m_data + x2.m_data);
   }

public:
   X(int data) : m_data(data) {}
   int Dump() const { return m_data;}
private:
   int m_data;
};

int main()
{
   // Integer
   int i = 4;
   cout <<
      "Plus(i, 9) = " <<
      Plus(i, 9) << endl;

   // Floating point
   float dx = 4.0;
   float dy = 9.5;
   cout <<
      setprecision(3) <<
      "Plus(dx, dy) = " <<
      Plus(dx, dy) << endl;

   // String
   string hello = "Hello, ";
   string world = "world!";
   cout << Plus(hello, world) << endl;

   // Custom type
   X x1(20);
   X x2(22);
   X x3 = Plus(x1, x2);
   cout <<
      "x3.Dump() = " <<
      x3.Dump() << endl;
}
```



> NOTE: 编译`g++ --std=c++11 test.cpp`



```c++
#include <utility>
template <class T, class ReturnT, class... ArgsT> class IsCallable
{
public:
   struct BadType {};
   template <class U>
   static decltype(std::declval<T>()(std::declval<ArgsT>()...)) Test(int); //C2064. Should be declval<U>
   template <class U>
   static BadType Test(...);
   static constexpr bool value = std::is_convertible<decltype(Test<T>(0)), ReturnT>::value;
};

constexpr bool test1 = IsCallable<int(), int>::value;
static_assert(test1, "PASS1");
constexpr bool test2 = !IsCallable<int*, int>::value;
static_assert(test2, "PASS2");
```

> NOTE: 这个例子没有读懂

## cppreference [decltype specifier](https://en.cppreference.com/w/cpp/language/decltype)

### [Explanation](https://en.cppreference.com/w/cpp/language/decltype#Explanation)



## `decltype` 的规则

在上述三篇文章中，都描述了`decltype` 的规则，其中最最简洁易懂的是microsoft [decltype (C++)](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019)的[Remarks](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019#remarks)段描述的，相比之下cppreference [decltype specifier](https://en.cppreference.com/w/cpp/language/decltype)的描述就难懂一些，所以最好将三篇结合起来：

> 1) If the argument is an unparenthesized [id-expression](https://en.cppreference.com/w/cpp/language/identifiers) or an unparenthesized [class member access](https://en.cppreference.com/w/cpp/language/operator_member_access) expression, then decltype yields the type of the *entity* named by this expression. If there is no such entity, or if the argument names a set of overloaded functions, the program is ill-formed.

这条规则源自cppreference [decltype specifier](https://en.cppreference.com/w/cpp/language/decltype)，microsoft [decltype (C++)](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019)中有同样的规则，这种是最最简单的情况。

> 2) If the *expression* parameter is a call to a function or an overloaded operator function, `decltype(expression)` is the return type of the function. Parentheses around an overloaded operator are ignored.

这条规则来自microsoft [decltype (C++)](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019)，cppreference [decltype specifier](https://en.cppreference.com/w/cpp/language/decltype)中没有这条规则。



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

- cppreference `Template argument deduction#Non-deduced contexts`
- stackoverflow [How does `void_t` work](https://stackoverflow.com/questions/27687389/how-does-void-t-work)

## Operands of `decltype` is unevaluated expressions

参见:

- cppreference `Expressions#Unevaluated expressions`

## `std::declval`

### cppreference [std::declval](https://en.cppreference.com/w/cpp/utility/declval)







## TO READ

https://en.wikipedia.org/wiki/Decltype#Semantics

https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019#remarks

https://riptutorial.com/cplusplus/example/18513/decltype

https://stackoverflow.com/questions/18815221/what-is-decltype-and-how-is-it-used

https://arne-mertz.de/2017/01/decltype-declval/
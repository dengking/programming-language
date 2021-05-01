# microsoft [decltype (C++)](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019)

> NOTE: 
>
> 1、这篇文章，提出了late specified return type概念，这对于理解C++ generic forwarding function template非常重要

The **`decltype`** type specifier yields the type of a specified expression. The **`decltype`** type specifier, together with the [`auto` keyword](https://docs.microsoft.com/en-us/cpp/cpp/auto-cpp?view=msvc-160)is useful primarily to developers who write **template libraries**. 

1) Use **`auto`** and **`decltype`** to declare a **template function** whose return type depends on the types of its template arguments. Or, 

> NOTE: 
>
> 1、Example: ibm [Introduction to the C++11 feature: trailing return types](https://community.ibm.com/community/user/ibmz-and-linuxone/blogs/fang-lu2/2020/03/24/introduction-to-the-c11-feature-trailing-return-types?lang=en) 
>
> ```c++
> #include <iostream>
> 
> template<class T>
> auto mul(T a, T b) -> decltype(a*b)
> {
> 
> 	return a * b;
> 
> }
> 
> int main()
> {
> 	std::cout << mul(1, 3);
> }
> 
> ```
>
> 

2) Use **`auto`** and **`decltype`** to declare a **template function** that wraps a call to another function, and then returns the return type of the **wrapped function**.

> NOTE: 
>
> 1、这篇文章后面会发布介绍如何实现上述两者



## [Decltype and Auto](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019#decltype-and-auto)

> NOTE: 
>
> 1、这一段主要描述的是late-specified return type，late-specified return type是实现后面的generic forwarding function的基础
>
> 

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



### Syntax of an alternative function declaration

Use the *alternative function declaration syntax* that is shown later, the **`auto`** keyword, and the **`decltype`** type specifier to declare a *late-specified* return type. The **late-specified return type** is determined when the declaration is compiled, instead of when it is coded.



The following prototype illustrates the syntax of an alternative function declaration. Note that the **`const`** and **`volatile`** qualifiers, and the **`throw`** [exception specification](https://docs.microsoft.com/en-us/cpp/cpp/exception-specifications-throw-cpp?view=msvc-160) are optional. The *function_body* placeholder represents a compound statement that specifies what the function does. As a best coding practice, the *expression* placeholder in the **`decltype`** statement should match the expression specified by the **`return`** statement, if any, in the *function_body*.

![](./function-prototype.png)

### Example: `myFunc`

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

//C++14
template<typename T, typename U>
decltype(auto) myFunc(T&& t, U&& u)
{
	return forward<T>(t) + forward<U>(u);
}

int main()
{
	std::cout << myFunc(1, 2) << std::endl;
	std::cout << myFunc(1.1, 2.2) << std::endl;
}
// g++ --std=c++14 test.cpp

```

> NOTE: 输出如下：
>
> ```
> decltype ((forward<T>(t) + forward<U>(u))) myFunc(T&&, U&&) [with T = int; U = int; decltype ((forward<T>(t) + forward<U>(u))) = int]
> 3
> decltype ((forward<T>(t) + forward<U>(u))) myFunc(T&&, U&&) [with T = double; U = double; decltype ((forward<T>(t) + forward<U>(u))) = double]
> 3.3
> ```





## [Decltype and Forwarding Functions (C++11)](https://docs.microsoft.com/en-us/cpp/cpp/decltype-cpp?view=vs-2019#decltype-and-forwarding-functions-c11)

> NOTE: 
>
> 1、这一段主要描述 generic forwarding function，在generic forwarding function中，显然也是涉及"late-specified return type"的，除此之外，它还涉及perfect forwarding function parameter。

**Forwarding functions** wrap calls to other functions. Consider a **function template** that forwards its arguments, or the results of an expression that involves those arguments, to another function. Furthermore, the **forwarding function** returns the result of calling the other function. In this scenario, the **return type** of the **forwarding function** should be the same as the return type of the wrapped function.

> NOTE: what is **Forwarding functions** ？在 wikipedia [decltype](https://en.wikipedia.org/wiki/Decltype) # [Motivation](https://en.wikipedia.org/wiki/Decltype#Motivation) 段中给出了链接: 
>
> > One of the cited main motivations for the `decltype` proposal was the ability to write **perfect [forwarding function](https://en.wikipedia.org/wiki/Wrapper_function) templates**.[[8\]](https://en.wikipedia.org/wiki/Decltype#cite_note-n1705-8) 
>
> 上面这段话总结了书写 一个 forwarding function 所面临的挑战。

In this scenario, you cannot write an appropriate type expression without the **`decltype`** type specifier. The **`decltype`** type specifier enables generic forwarding functions because it does not lose required information about whether a function returns a reference type. For a code example of a forwarding function, see the previous `myFunc` template function example.

## Examples

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




# Perfect forwarding function arguments

参考素材:

thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c) 

stackoverflow [Advantages of using forward](https://stackoverflow.com/questions/3582001/advantages-of-using-forward) # [A](https://stackoverflow.com/a/3582313) 

open-std jtc1 [The Forwarding Problem: Arguments](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2002/n1385.htm) 



## 内容梳理

### What is perfect forwarding function arguments

- 在文章thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c)中结合具体的例子对perfect forwarding问题进行了较好的介绍，可以作为入门读物；

- stackoverflow [Advantages of using forward](https://stackoverflow.com/questions/3582001/advantages-of-using-forward) # [A](https://stackoverflow.com/a/3582313): 

  > Why is this useful? Because combined we maintain the ability to keep track of the value category of a type: if it was an **lvalue**, we have an **lvalue-reference parameter**, otherwise we have an **rvalue-reference parameter**.

- open-std jtc1 [The Forwarding Problem: Arguments](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2002/n1385.htm) 



### Pre C++11 perfect forwarding function arguments的困难

Perfect forwarding是一直以来困扰C++ programmer的问题，在如下文章中进行了概括:

- thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c) 的“The perfect forwarding problem”段中对它进行了说明
- stackoverflow [Advantages of using forward](https://stackoverflow.com/questions/3582001/advantages-of-using-forward) # [A](https://stackoverflow.com/a/3582313) 

下面是基于这两篇文章的简短概括:

1 pass-by-value:

```c++
template <typename T1, typename T2>
void wrapper(T1 e1, T2 e2) {
    func(e1, e2);
}
```

存在的问题是: 参数`e1`、`e2` 是pass-by-value，因此函数 `func` 接收到的参数不是原本的object。

2 pass-by-reference:

```c++
template <typename T1, typename T2>
void wrapper(T1& e1, T2& e2) {
    func(e1, e2);
}
```

存在的问题是: 参数`e1`、`e2` 是pass-by-reference，它们无法bind到rvalue，比如下面的程序无法编译通过:

```c++
wrapper(42, 3.14f);                  // error: invalid initialization of
                                     //        non-const reference from
                                     //        an rvalue

wrapper(i, foo_returning_float());   // same error
```



3 brute-force

穷举 `const` 和 non-`const` references的所有可能组合的overload:

```c++
template <typename T1, typename T2>
void wrapper(T1& e1, T2& e2)                { func(e1, e2); }

template <typename T1, typename T2>
void wrapper(const T1& e1, T2& e2)          { func(e1, e2); }

template <typename T1, typename T2>
void wrapper(T1& e1, const T2& e2)          { func(e1, e2); }

template <typename T1, typename T2>
void wrapper(const T1& e1, const T2& e2)    { func(e1, e2); }
```

存在的问题: 指数爆炸导致可能无法穷举。

显然，对于这种困难，需要在语言层面进行解决，这是下部分的内容。



### C++11 perfect forwarding function arguments的实现

C++11在语言层面对perfect forwarding进行实现，它主要依赖于如下两个方面：

- Reference collapsing(折叠) rule
- Special type deduction rules for rvalue references
- forwarding reference

在下面文章中对上述两者进行了解释:

- thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c) 的"Reference collapsing and special type deduction for rvalues"章节



### 总结

- perfect forwarding的实现 = reference collapsing(折叠) rule + forwarding reference + special type deduction rules for forwarding references + `std::forward`；

- 只能够perfect forwarding "forwarding reference"，这段话是"cppreference [Reference declaration](https://en.cppreference.com/w/cpp/language/reference) # Forwarding references (since C++11)"的例子中给出的:

  ```c++
  template<class T>
  int f(T&& x)                      // x is a forwarding reference
  {
      return g(std::forward<T>(x)); // and so can be forwarded
  }
  ```



## cppreference [Reference declaration](https://en.cppreference.com/w/cpp/language/reference) # Forwarding references (since C++11)

**Forwarding references** are a special kind of references that preserve the **value category** of a function argument, making it possible to *forward* it by means of [std::forward](https://en.cppreference.com/w/cpp/utility/forward). 

> NOTE: 通过后面的例子可知: forwarding reference能够按照function argument的value category来准确的生成reference，简而言之: **forwarding reference**既可以bind to lvalue也可以bind to rvalue同时保持CV-qualification

**Forwarding references** are either:

### 1)

function parameter of a function template declared as **rvalue reference** to **cv-unqualified** [type template parameter](https://en.cppreference.com/w/cpp/language/template_parameters) of that same function template:



#### Example

```c++
#include <utility> // std::forward
#include <iostream>

template<class T>
int g(const T &&x) {
    // x is not a forwarding reference: const T is not cv-unqualified
    std::cout << __PRETTY_FUNCTION__ << " " << " " << x << std::endl;
}

template<class T>
int g(const T &x) {
    std::cout << __PRETTY_FUNCTION__ << " " << " " << x << std::endl;
}

template<class T>
int f(T &&x) {                    // x is a forwarding reference
    return g(std::forward<T>(x)); // and so can be forwarded
}

int main() {
    int i = 100;
    f(i); // argument is lvalue, calls f<int&>(int&), std::forward<int&>(x) is lvalue
    f(0); // argument is rvalue, calls f<int>(int&&), std::forward<int>(x) is rvalue
}

// g++ --std=c++11 test.cpp

/**
输出如下:
int g(const T&) [with T = int]  100
int g(const T&&) [with T = int]  0
**/

```

> NOTE: 从上述程序的输出可以看出: 函数 `f` 内部调用函数 `g`，入参数是通过 `std::forward<T>(x)` 实现的，它实现了"preserve value category"



#### Example

```c++
template<class T> struct A {
    template<class U>
    A(T&& x, U&& y, int* p); // x is not a forwarding reference: T is not a
                             // type template parameter of the constructor,
                             // but y is a forwarding reference
};
```



#### Special deduction rule for forwarding reference

补充说明"special deduction rule for forwarding reference"

##### thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c) 

> The other addition to C++11 relevant to this article is **special type deduction rules** for **rvalue references** in some cases [[1\]](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c#id5). Given a function template like:
>
> ```c++
> template <class T>
> void func(T&& t) {
> }
> ```
>
> Don't let `T&&` fool you here - `t` is not an **rvalue reference** [[2\]](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c#id6). When it appears in a **type-deducing context**, `T&&` acquires a special meaning. When `func` is instantiated, `T` depends on whether the argument passed to `func` is an **lvalue** or an **rvalue**. If it's an **lvalue** of type `U`, `T` is deduced to `U&`. If it's an **rvalue**, `T` is deduced to `U`:
>
> ```c++
> func(4);            // 4 is an rvalue: T deduced to int
> 
> double d = 3.14;
> func(d);            // d is an lvalue; T deduced to double&
> 
> float f() {...}
> func(f());          // f() is an rvalue; T deduced to float
> 
> int bar(int i) {
>   func(i);          // i is an lvalue; T deduced to int&
> }
> ```
>



##### cppreference [Template argument deduction](https://en.cppreference.com/w/cpp/language/template_argument_deduction) 

> If **`P`** is an **rvalue reference** to a **cv-unqualified** template parameter (so-called [forwarding references](https://en.cppreference.com/w/cpp/language/reference#Forwarding_references)), and the corresponding function call argument is an **lvalue**, the type lvalue reference to **`A`** is used in place of **`A`** for deduction (Note: this is the basis for the action of [std::forward](https://en.cppreference.com/w/cpp/utility/forward). Note: in [class template argument deduction](https://en.cppreference.com/w/cpp/language/class_template_argument_deduction), template parameter of a class template is never a **forwarding reference**(since C++17))

上面这段话其实只讲了special rule，但是normal rule却没有进行对比介绍而是通过例子来隐晦地进行说明的: 如果the corresponding function call argument is an **rvalue**，那么不会使用上述special rule，而是还使用  **`A`** ，下面是原文的用以说明的例子:

```c++
#include <iostream>

template<class T>
int f(T &&) {
    // P is an rvalue reference to cv-unqualified T (forwarding reference)
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return 0;
}

template<class T>
int g(const T &&) {
    // P is an rvalue reference to cv-qualified T (not special)
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

int main() {
    int i;
    int n1 = f(i); // argument is lvalue: calls f<int&>(int&) (special case)
    int n2 = f(0); // argument is not lvalue: calls f<int>(int&&)

    //  int n3 = g(i); // error: deduces to g<int>(const int&&), which
    // cannot bind an rvalue reference to an lvalue
}

/**
输出如下: 
int f(T &&) [T = int &]
int f(T &&) [T = int]
**/
```



#### Example: CV-qualified的例子

下面是补充说明的另外一个例子:

stackoverflow [Is this a forwarding reference?](https://stackoverflow.com/questions/40819058/is-this-a-forwarding-reference) # [A](https://stackoverflow.com/a/40835219/23877800)

> ```c++
> #include <iostream>
> 
> template <class T>
> void foo(T &&t) {
> std::cout << __PRETTY_FUNCTION__ << std::endl;
> }
> 
> int main() {
> //...
> const int i = 42;
> foo(i);      // the compiler will defines T = const int &
>             // T&&  = const int & && = const int &
>             // => the compiler instantiates void foo<const int &>(const int &);
> foo(6 * 7);  // the compiler will defines T = int
>             // T&&  = int &&
>             // the compiler instantiates  void foo<int>(int &&);
> }
> // g++ --std=c++11  test.cpp
> /**
> 输出如下:
> void foo(T &&) [T = const int &]
> void foo(T &&) [T = int]
> **/
> ```



#### 总结

通过上述例子可以看出 **forwarding reference**既可以bind to lvalue也可以bind to rvalue同时保持CV-qualification，通过上述例子可以看出，**forwarding reference**的实现是有赖于前面提到的:

- reference collapsing rule

- special type deduction rules for rvalue references 



### 2)

这部分内容放到了 `auto&&` 章节。



## Forwarding reference=universal reference

forwarding reference既可以reference **lvalue**又可以reference **rvalue**，所以它是universal，这是它的另外一个名称universal reference中universal的含义，显然这是一种由abstraction带来的uniform。



## std::forward

在 [Reference declaration](https://en.cppreference.com/w/cpp/language/reference#Reference_collapsing) # [Reference collapsing](https://en.cppreference.com/w/cpp/language/reference#Reference_collapsing) 中有这样的介绍:

> (This(指的是 [Reference collapsing](https://en.cppreference.com/w/cpp/language/reference#Reference_collapsing) ), along with special rules for [template argument deduction](https://en.cppreference.com/w/cpp/language/template_argument_deduction) when `T&&` is used in a function template, forms the rules that make [std::forward](https://en.cppreference.com/w/cpp/utility/forward) possible.)



### cppreference [`std::forward`](https://en.cppreference.com/w/cpp/utility/forward) 

C++11

```c++
#include <type_traits>
template <class T>
T&& forward(typename std::remove_reference<T>::type& t) noexcept;

template <class T>
T&& forward(typename std::remove_reference<T>::type&& t) noexcept;
```

C++14

```C++
#include <type_traits>
template <class T>
constexpr T&& forward(std::remove_reference_t<T>& t) noexcept;

template <class T>
constexpr T&& forward(std::remove_reference_t<T>&& t) noexcept;
```



#### 1 forward lvalues

```c++
template <class T>
T&& forward(typename std::remove_reference<T>::type& t) noexcept;

template <class T>
constexpr T&& forward(std::remove_reference_t<T>& t) noexcept;
```

Forwards lvalues as either lvalues or as rvalues, depending on T

When `t` is a [forwarding reference](https://en.cppreference.com/w/cpp/language/reference#Forwarding_references) (a function argument that is declared as an rvalue reference to a cv-unqualified function template parameter), this overload forwards the argument to another function with the [value category](https://en.cppreference.com/w/cpp/language/value_category) it had when passed to the calling function.

For example, if used in a wrapper such as the following, the template behaves as described below:

```c++
#include <utility>
template <class T>
void wrapper(T&& arg) {
  // arg is always lvalue
  foo(std::forward<T>(arg));  // Forward as lvalue or as rvalue, depending on T
}

```

- If a call to `wrapper()` passes an rvalue `std::string`, then `T` is deduced to `std::string` (not `std::string&`, `const std::string&`, or `std::string&&`), and `std::forward` ensures that an rvalue reference is passed to `foo`.

  

> NOTE: `std::forward`的入参是lvalue，它的返回值的类型是lvalue还是rvalue，取决于`T`，这就是perfect forwarding

```c++
#include <iostream>
#include <string>
#include <utility>

void foo(std::string && s)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	std::cout << s << std::endl;
}

void foo(std::string & s)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	std::cout << s << std::endl;
}

void foo(const std::string & s)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	std::cout << s << std::endl;
}

template<class T>
void wrapper(T&& arg)
{
	// arg is always lvalue
	foo(std::forward<T>(arg)); // Forward as lvalue or as rvalue, depending on T
}

int main()
{
	std::cout << "lvalue:" << std::endl;
	std::string s1 = "hello world";
	wrapper(s1);
	std::cout << "const lvalue:" << std::endl;
	const std::string s2 = "hello world";
	wrapper(s2);
	std::cout << "rvalue:" << std::endl;
	wrapper("hello world");

}
// g++ --std=c++11 test.cpp
```

> NOTE: 输出如下:
>
> ```c++
> lvalue:
> void foo(std::string&)
> hello world
> const lvalue:
> void foo(const string&)
> hello world
> rvalue:
> void foo(std::string&&)
> hello world
> ```
>
> 

#### 2 Forward rvalues 

Forwards rvalues as rvalues and prohibits forwarding of rvalues as lvalues

This overload makes it possible to forward a **result** of an expression (such as function call), which may be rvalue or lvalue, as the original value category of a forwarding reference argument.

> NOTE: 需要结合下面的例子来理解上面这段话：函数`foo`的入参的value category是由`T`决定的，而不是由**result** of an function call决定的；

```c++
#include <iostream>
#include <utility>
void foo(int& i)
{
	std::cout << __PRETTY_FUNCTION__ << " " << i << std::endl;
}

void foo(int&& i)
{
	std::cout << __PRETTY_FUNCTION__ << " " << i << std::endl;
}
// transforming wrapper
template<class T>
void wrapper(T&& arg)
{
	foo(std::forward<decltype(std::forward<T>(arg).get())>(std::forward<T>(arg).get()));
}

struct Arg
{
	int i = 1;
	int& get() &&
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return i;
	} // call to this overload is rvalue
	int& get() &
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return i;
	} // call to this overload is lvalue
};

int main()
{
	std::cout << "lvalue reference:" << std::endl;
	Arg a;
	wrapper(a);
	std::cout << "rvalue reference:" << std::endl;
	wrapper(Arg());
}

// g++ --std=c++11 test.cpp
```

> NOTE: 输出如下
>
> ```c++
> lvalue reference:
> int& Arg::get() &
> void foo(int&) 1
> rvalue reference:
> int& Arg::get() &&
> void foo(int&) 1
> ```



#### Example

```c++
#include <iostream>
#include <memory>
#include <utility>

struct A
{
	A(int&& n)
	{
		std::cout << __PRETTY_FUNCTION__ << ", n=" << n << "\n";
	}
	A(int& n)
	{
		std::cout << __PRETTY_FUNCTION__ << ", n=" << n << "\n";
	}
};

class B
{
public:
	template<class T1, class T2, class T3>
	B(T1&& t1, T2&& t2, T3&& t3)
			:
					a1_ { std::forward<T1>(t1) },
					a2_ { std::forward<T2>(t2) },
					a3_ { std::forward<T3>(t3) }
	{
	}

private:
	A a1_, a2_, a3_;
};

template<class T, class U>
std::unique_ptr<T> make_unique1(U&& u)
{
	return std::unique_ptr<T>(new T(std::forward<U>(u)));
}

template<class T, class ... U>
std::unique_ptr<T> make_unique2(U&&... u)
{
	return std::unique_ptr<T>(new T(std::forward<U>(u)...));
}

int main()
{
	auto p1 = make_unique1<A>(2); // rvalue
	int i = 1;
	auto p2 = make_unique1<A>(i); // lvalue

	std::cout << "B\n";
	auto t = make_unique2<B>(2, i, 3);
}

// g++ --std=c++11 test.cpp
```

> NOTE: 输入如下:
>
> ```c++
> A::A(int&&), n=2
> A::A(int&), n=1
> B
> A::A(int&&), n=2
> A::A(int&), n=1
> A::A(int&&), n=3
> ```
>
> 

### Implementation

### libstdc++

[gcc](https://github.com/gcc-mirror/gcc)/[libstdc++-v3](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3)/[include](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include)/[bits](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include/bits)/[move.h](https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/move.h)

```c++
/**
 *  @brief  Forward an lvalue.
 *  @return The parameter cast to the specified type.
 *
 *  This function is used to implement "perfect forwarding".
 */
template<typename _Tp>
constexpr _Tp&&
forward(typename std::remove_reference<_Tp>::type& __t) noexcept
{
	return static_cast<_Tp&&>(__t);
}

/**
 *  @brief  Forward an rvalue.
 *  @return The parameter cast to the specified type.
 *
 *  This function is used to implement "perfect forwarding".
 */
template<typename _Tp>
constexpr _Tp&&
forward(typename std::remove_reference<_Tp>::type&& __t) noexcept
{
	static_assert(!std::is_lvalue_reference<_Tp>::value, "template argument"
			" substituting _Tp is an lvalue reference type");
	return static_cast<_Tp&&>(__t);
}

```

#### non-deducing context 

bajamircea [C++ std::move and std::forward](http://bajamircea.github.io/coding/cpp/2016/04/07/move-forward.html)中此有描述：

> The type `T` is not deduced, therefore we had to specify it when **using** `std::forward`.

thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c)中此有描述：

> Another thing I want to mention is the use of `std::remove_reference<T>`. In fact, if you think about it, `forward` could do without it. **Reference collapsing** does the job already, so `std::remove_reference<T>` is superfluous（多余的）. It's there to turn the `T& t` into a **non-deducing context** (according to the C++ standard, section 14.8.2.5), thus forcing us to explicitly specify the **template parameter** when calling `std::forward`.



关于 **non-deducing context** ，参见cppreference [Template argument deduction#Non-deduced contexts](https://en.cppreference.com/w/cpp/language/template_argument_deduction#Non-deduced_contexts)，显然`std::forward`符合第一条：

> everything to the left of the scope resolution operator `::`

#### Stop at compile time attempts to convert from an `rvalue` to an `lvalue` 

The `static_assert` is there to stop at compile time attempts to convert from an `rvalue` to an `lvalue` (that would have the dangling reference problem: a reference pointing to a temporary long gone). This is explained in more details in [N2835](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2009/n2835.html), but the gist is:

```c++
forward<const Y&>(Y()); // does not compile
// static assert in forward triggers compilation failure for line above
// with "invalid rvalue to lvalue conversion"
```



Some non-obvious properties of `std::forward` are that the return value can be more cv-qualified (i.e. can add a `const`). Also it allows for the case where the argument and return are different e.g. to forward expressions from **derived type** to it’s **base type** (even some scenarios where the base is derived from as `private`).



### `std::forward` does not **forward**

在bajamircea [C++ std::move and std::forward](http://bajamircea.github.io/coding/cpp/2016/04/07/move-forward.html)中，有这样的描述：

> C++ `std::move` does not **move** and `std::forward` does not **forward**.





## Examples

#### 一般形式

stackoverflow [What are the main purposes of using std::forward and which problems it solves?](https://stackoverflow.com/questions/3582001/what-are-the-main-purposes-of-using-stdforward-and-which-problems-it-solves)

```cpp
template <typename T1, typename T2>
void outer(T1&& t1, T2&& t2) 
{
    inner(std::forward<T1>(t1), std::forward<T2>(t2));
}
```

cpppatterns [Perfect forwarding](https://cpppatterns.com/patterns/perfect-forwarding.html)

```c++
#include <iostream>
#include <utility>

template <typename T, typename U>
std::pair<T, U> make_pair_wrapper(T&& t, U&& u) {
  return std::make_pair(std::forward<T>(t), std::forward<U>(u));
}

int main() {
  auto p = make_pair_wrapper(1, 2);
  std::cout << p.first << " " << p.second << std::endl;
}
// g++ --std=c++11  test.cpp
```



#### [variadic templates](http://eli.thegreenplace.net/2014/variadic-templates-in-c/) and perfect forwarding 



- `emplace_back`，在thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c/)中，将它作为引言
- [`std::make_unique`](http://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique)
- [`std::make_shared`](http://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared) 




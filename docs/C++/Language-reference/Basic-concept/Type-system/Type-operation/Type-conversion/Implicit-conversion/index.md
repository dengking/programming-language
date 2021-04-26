# cppreference [Implicit conversions](https://en.cppreference.com/w/cpp/language/implicit_conversion)

> NOTE: 
>
> 1、在阅读本文前，建议先阅读accu [Overload Resolution - Selecting the Function](https://accu.org/journals/overload/13/66/kilpelainen_268/)，在`C++\Language-reference\Functions\Overload-resolution\accu-Overload-Resolution`中收录了这篇文章，并对其进行了详细注解。

Implicit conversions are performed whenever an expression of some type `T1` is used in context that does not accept that type, but accepts some other type `T2`; in particular:

> NOTE: 下面罗列了各种可能的场景

| situation                                                    | 注解                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| when the expression is used as the argument when calling a function that is declared with `T2` as parameter |                                                              |
| when the expression is used as an operand with an operator that expects `T2` | operator其实和前面所述的function是非常类似的（参见`C++\Language-reference\Expressions\Operators\index.md`中总结的“Operator is a kind of function”思想） |
| when initializing a new object of type `T2`, including `return` statement in a function returning `T2` | `return` statement应该说的是这种情况:<br> `T2 Foo(){ return T1 }` |
| when the expression is used in a `switch` statement (`T2` is **integral typ**e) |                                                              |
| when the expression is used in an `if` statement or a loop (`T2` is `bool`) |                                                              |



## Order of the conversions

> NOTE:
>
> | conversion sequence          | 注解 |
> | ---------------------------- | ---- |
> | Implicit conversion sequence |      |
> | Standard conversion sequence |      |
>
> 从下面的描述来看，Implicit conversion sequence 包含: 
>
> - Standard conversion sequence
> - User-defined conversion

### Implicit conversion sequence

**Implicit conversion sequence** consists of the following, in this order:

| order | explanation                                 | 注解                                    |
| ----- | ------------------------------------------- | --------------------------------------- |
| 1)    | zero or one *standard conversion sequence*; |                                         |
| 2)    | zero or one *user-defined conversion*;      | 参见下面的“User-defined conversion”章节 |
| 3）   | zero or one *standard conversion sequence*. |                                         |

> NOTE: 为了便于理解Implicit conversion sequence的含义，我们看看涉及到它的章节:
>
> | 章节                                                         | 内容                                                         |
> | ------------------------------------------------------------ | ------------------------------------------------------------ |
> | [user-defined conversion function#Explanation](https://en.cppreference.com/w/cpp/language/cast_operator#Explanation) | User-defined conversion function is invoked on the second stage of the [implicit conversion](https://en.cppreference.com/w/cpp/language/implicit_cast). |
> | [Overload resolution#Ranking of implicit conversion sequences](https://en.cppreference.com/w/cpp/language/overload_resolution#Ranking_of_implicit_conversion_sequences) |                                                              |
>
> 

When considering the **argument** to a **constructor** or to a **user-defined conversion function**, only a **standard conversion sequence** is allowed (otherwise **user-defined conversions** could be effectively chained). 

> NOTE: 上述argument，说明是调用函数的时候；上述constructor、user-defined conversion function，说明是user-defined conversion。
>
> 上面这段话，没有具体的例子，难以准确弄清楚作者想要表达的含义。

When converting from one built-in type to another built-in type, only a **standard conversion sequence** is allowed.

### Standard conversion sequence 

A **standard conversion sequence** consists of the following, in this order:

| order | explanation                                                  | 注解                                                         |
| ----- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| 1)    | zero or one conversion from the following set: <br>- *lvalue-to-rvalue conversion*<br/>- *array-to-pointer conversion*<br/>- *function-to-pointer conversion*; | 这些conversion是**value transformation**，在下面的”**Value transformations**“中进行了详细介绍 |
| 2)    | zero or one:<br>- *numeric promotion* <br/>- *numeric conversion*; |                                                              |
| 3)    | zero or one *function pointer conversion*;                   | (since C++17)                                                |
| 4)    | zero or one *qualification conversion*.                      |                                                              |

### User-defined conversion

> NOTE: 关于User-defined conversion，参见 `C++\Language-reference\Classes\Special-member-functions\Constructor\User-defined-conversion` 章节

| conversion                                                   | 注解 |
| ------------------------------------------------------------ | ---- |
| **non-explicit** single-argument [converting constructor](https://en.cppreference.com/w/cpp/language/converting_constructor) |      |
| **non-explicit** [conversion function](https://en.cppreference.com/w/cpp/language/cast_operator) call |      |

> NOTE: 
>
> 需要注意的是: 本文描述的是implicit conversion，因此它是不允许**explicit conversion**的，在 `C++\Language-reference\Classes\Special-member-functions\Constructor\User-defined-conversion` 章节中给出了explicit conversion example。
>
> 

### Implicitly convertible

An expression `e` is said to be *implicitly convertible to `T2`* if and only if `T2` can be [copy-initialized](https://en.cppreference.com/w/cpp/language/copy_initialization) from `e`, that is the declaration `T2 t = e;` is well-formed (can be compiled), for some invented temporary `t`. Note that this is different from [direct initialization](https://en.cppreference.com/w/cpp/language/direct_initialization) (`T2 t(e)`), where **explicit** constructors and conversion functions would additionally be considered.

| expression  | initialization                                               | conversion              |
| ----------- | ------------------------------------------------------------ | ----------------------- |
| `T2 t = e;` | [copy-initialized](https://en.cppreference.com/w/cpp/language/copy_initialization) | **implicit** conversion |
| `T2 t(e)`   | [direct initialization](https://en.cppreference.com/w/cpp/language/direct_initialization) | **explicit** conversion |

### Contextual conversions(since C++11)

**Contextual conversion to bool **:

> NOTE: 本节的小标题是我添加的，原文中，并没有。

In the following contexts, the type `bool` is expected and the **implicit conversion** is performed if the declaration `bool t(e);` is well-formed (that is, an **explicit conversion function** such as `explicit T::operator bool() const;` is considered). Such expression `e` is said to be *contextually converted to bool*.

> NOTE: 
>
> 一、关于`bool t(e);`，参见上一段。
>
> 可以看到，下面描述的这些context，可以看到这些context都是期望`bool`的，我们可以将它们简称为bool context 。
>
> 二、这是比较特殊的，在下面文章中，涉及了这个内容
>
> stackoverflow [Conversion function for error checking considered good?](https://stackoverflow.com/questions/6242296/conversion-function-for-error-checking-considered-good) # [A](https://stackoverflow.com/a/6242355)
>
> In C++11 you can use an explicit conversion:
>
> ```cpp
> explicit operator bool() const
> {
>     // verify if valid
>     return is_valid;
> }
> ```
>
> This way you need to be explicit about the conversion to bool, so you can no longer do crazy things by accident (in C++ you can always do crazy things on purpose):
>
> ```cpp
> int x = my_object; // does not compile because there's no explicit conversion
> bool y = bool(my_object); // an explicit conversion does the trick
> ```
>
> This still works as normal in places like `if` and `while` that require a boolean expression, because the condition of those statements is *contextually converted* to bool:
>
> ```cpp
> // this uses the explicit conversion "implicitly"
> if (my_object)
> {
>     ...
> }
> ```
>
> 三、显然C++这样做，是为了简便、向后兼容

| context                                                      | 注解 |
| ------------------------------------------------------------ | ---- |
| the controlling expression of `if`, `while`, `for`;          |      |
| the operands of the built-in logical operators `!`, `&&` and `||`; |      |
| the first operand of the conditional operator `?:`;          |      |
| the predicate in a [`static_assert`](https://en.cppreference.com/w/cpp/language/static_assert) declaration; |      |
| the expression in a [`noexcept`](https://en.cppreference.com/w/cpp/language/noexcept_spec) specifier; |      |
| the expression in an [`explicit`](https://en.cppreference.com/w/cpp/language/explicit) specifier;(since C++20) |      |



```C++
#include <cassert>
#include <iostream>

template<typename T>
class zero_init
{
	T val;
public:
	zero_init() :
			val(static_cast<T>(0))
	{
	}
	zero_init(T val) :
			val(val)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	operator T&()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return val;
	}
	operator T() const
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return val;
	}
};

int main()
{
	zero_init<int> i;
	assert(i == 0);
	i = 7;
	assert(i == 7);
	switch (i)
	{
	}     // error until C++14 (more than one conversion function)
		  // OK since C++14 (both functions convert to the same type int)
	switch (i + 0)
	{
	} // always okay (implicit conversion)
}
// g++ test.cpp

```

> NOTE: 上述程序编译报错如下:
>
> ```C++
> test.cpp: In function ‘int main()’:
> test.cpp:36:11: error: ambiguous default type conversion from ‘zero_init<int>’
>   switch (i)
>            ^
> test.cpp:36:11: error:   candidate conversions include ‘zero_init<T>::operator T&() [with T = int]’ and ‘zero_init<T>::operator T() const [with T = int]’
> ```
>
> 下面是在C++11中可以编译通过的版本: 

```C++
#include <cassert>
#include <iostream>

template<typename T>
class zero_init
{
	T val;
public:
	zero_init() :
			val(static_cast<T>(0))
	{
	}
	zero_init(T val) :
			val(val)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	operator T&()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return val;
	}
	operator T() const
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return val;
	}
};

int main()
{
	zero_init<int> i;
	std::cout << __FILE__ << " " << __LINE__ << " " << std::endl;
	assert(i == 0);
	std::cout << __FILE__ << " " << __LINE__ << " " << std::endl;
	i = 7;
	std::cout << __FILE__ << " " << __LINE__ << " " << std::endl;
	assert(i == 7);
//	switch (i)
//	{
//	}     // error until C++14 (more than one conversion function)
	// OK since C++14 (both functions convert to the same type int)
	std::cout << __FILE__ << " " << __LINE__ << " " << std::endl;
	switch (i + 0)
	{
	} // always okay (implicit conversion)
}
// g++ test.cpp

```

> NOTE: 上述程序的输出如下:
>
> ```C++
> test.cpp 33 
> zero_init<T>::operator T&() [with T = int]
> test.cpp 35 
> zero_init<T>::zero_init(T) [with T = int]
> test.cpp 37 
> zero_init<T>::operator T&() [with T = int]
> test.cpp 43 
> zero_init<T>::operator T&() [with T = int]
> ```
>
> 可以看到，选择的conversion function是`operator T() const`

**context-specific type `T`**

In the following contexts, a context-specific type `T` is expected, and the expression `e` of class type `E` is only allowed if `E` has a single non-explicit [user-defined conversion function](https://en.cppreference.com/w/cpp/language/cast_operator) to an allowable type (until C++14)there is exactly one type `T` among the allowable types such that `E` has **non-explicit conversion functions** whose return types are (possibly cv-qualified) `T` or reference to (possibly cv-qualified) `T`, and `e` is implicitly convertible to `T` (since C++14). 

> NOTE: 上面所述的是: `E` **->** `T`的conversion。需要注意的是，上面这段话虽然说得比较绕，但是它想要表达的意思是非常简单的: 如果有 **non-explicit** user-define conversion function来实现 `E` **->** `T`的conversion，则contextual conversion是允许的，这就是下面这段话中所说的:  *contextually implicitly converted* 。

Such expression `e` is said to be *contextually implicitly converted* to the specified type `T`. Note that **explicit conversion functions** are not considered, even though they are considered in contextual conversions to bool. (since C++11)

> NOTE: 为什么explicit conversion function是不考虑的？这个问题在“User-defined conversion”段中已经回答了。

| context                                                      | explanation                                                  | 注解                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| the argument of the [delete-expression](https://en.cppreference.com/w/cpp/language/delete) | `T` is any object pointer type                               | 其实说白了就是delete的时候，如果`T`是可以可以*contextually implicitly converted* to pointer type的话，则compiler会执行这个conversion |
| [integral constant expression](https://en.cppreference.com/w/cpp/language/constant_expression#Integral_constant_expression), where a literal class is used | `T` is any integral or unscoped enumeration type, the selected user-defined conversion function must be [constexpr](https://en.cppreference.com/w/cpp/language/constexpr) | 关于constant expression，参见`C++\Language-reference\Expressions\Constant-expressions`章节 |
| the controlling expression of the [`switch`](https://en.cppreference.com/w/cpp/language/switch) statement | `T` is any integral or enumeration type                      |                                                              |



## Value transformations



### Lvalue to rvalue conversion

> NOTE: 参见"Lvalue-to-rvalue-conversion"章节

### Array to pointer conversion

> NOTE: 参见 `Resource-management\Memory-management\Memory-access\Array` 章节

### C++17 Temporary materialization



### Function to pointer



## Numeric promotions

### Integral conversions



### Floating-point conversions



## The safe bool problem

> NOTE: 这部分内容放到了 `operator-bool` 章节
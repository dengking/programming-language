# Perfect forwarding

## 内容梳理

Perfect forwarding是一直以来困扰C++ programmer的问题（在文章thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c)的“The perfect forwarding problem”段中对它进行了说明），C++11对该问题进行了解决；

要想搞清楚perfect forwarding，需要首先搞清楚如下内容：

| 内容                                               | application                      |      |
| -------------------------------------------------- | -------------------------------- | ---- |
| Reference collapsing rule                          | 主要用于实现`std::forward`       |      |
| Special type deduction rules for rvalue references | 主要用于实现forwarding reference |      |

上述两者，在文章thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c) 的"Reference collapsing and special type deduction for rvalues"中进行了总结；

forwarding reference既可以reference **lvalue**又可以reference **rvalue**，所以它是universal，这是它的另外一个名称universal reference中universal的含义；

> NOTE: 这是一种uniform

实现perfect forwarding：

> perfect forwarding = forwarding reference + `std::forward`；

关于perfect forwarding的含义，在stackoverflow [Advantages of using forward](https://stackoverflow.com/questions/3582001/advantages-of-using-forward)的[回答](https://stackoverflow.com/a/3582313)中有着较好的解释：

> Why is this useful? Because combined we maintain the ability to keep track of the value category of a type: if it was an **lvalue**, we have an **lvalue-reference parameter**, otherwise we have an **rvalue-reference parameter**.



在文章thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c)中，对它们有着较好的介绍，所以以它为入门读物；



## stackoverflow [Advantages of using forward](https://stackoverflow.com/questions/3582001/advantages-of-using-forward)

https://stackoverflow.com/a/3582313

### Perfect forwarding problem

Basically, given the expression `E(a, b, ... , c)`, we want the expression `f(a, b, ... , c)` to be equivalent. In C++03, this is impossible. There are many attempts, but they all fail in some regard.

#### Use lvalue-reference

The simplest is to use an lvalue-reference:

```cpp
template <typename A, typename B, typename C>
void f(A& a, B& b, C& c)
{
    E(a, b, c);
}
```

But this fails to handle temporary values: `f(1, 2, 3);`, as those cannot be bound to an lvalue-reference.

#### Use const lvalue-reference

The next attempt might be:

```cpp
template <typename A, typename B, typename C>
void f(const A& a, const B& b, const C& c)
{
    E(a, b, c);
}
```

Which fixes the above problem, but flips flops. It now fails to allow `E` to have non-const arguments:

```cpp
int i = 1, j = 2, k = 3;
void E(int&, int&, int&); f(i, j, k); // oops! E cannot modify these
```

#### Use const lvalue-reference and `const_cast`

The third attempt accepts const-references, but then `const_cast`'s the `const` away:

```cpp
template <typename A, typename B, typename C>
void f(const A& a, const B& b, const C& c)
{
    E(const_cast<A&>(a), const_cast<B&>(b), const_cast<C&>(c));
}
```

This accepts all values, can pass on all values, but potentially leads to **undefined behavior**:

```cpp
const int i = 1, j = 2, k = 3;
E(int&, int&, int&); f(i, j, k); // ouch! E can modify a const object!
```

#### Brute-force

A final solution handles everything correctly...at the cost of being impossible to maintain. You provide overloads of `f`, with *all* combinations of const and non-const:

```cpp
template <typename A, typename B, typename C>
void f(A& a, B& b, C& c);

template <typename A, typename B, typename C>
void f(const A& a, B& b, C& c);

template <typename A, typename B, typename C>
void f(A& a, const B& b, C& c);

template <typename A, typename B, typename C>
void f(A& a, B& b, const C& c);

template <typename A, typename B, typename C>
void f(const A& a, const B& b, C& c);

template <typename A, typename B, typename C>
void f(const A& a, B& b, const C& c);

template <typename A, typename B, typename C>
void f(A& a, const B& b, const C& c);

template <typename A, typename B, typename C>
void f(const A& a, const B& b, const C& c);
```

N arguments require $2^N$ combinations, a nightmare. We'd like to do this automatically.

(This is effectively what we get the **compiler** to do for us in C++11.)

> NOTE: 由compiler来自动地为我们生成这些

------

### Solution

In C++11, we get a chance to fix this. [One solution modifies template deduction rules on existing types, but this potentially breaks a great deal of code.](https://stackoverflow.com/questions/3591832/perfect-forwarding-in-c03/3591909#3591909) So we have to find another way.

#### Reference collapsing rule

The solution is to instead use the newly added **rvalue-references**; we can introduce new rules when deducing rvalue-reference types and create any desired result. After all, we cannot possibly break code now.

> NOTE: 语言的演进，往往是需要考虑多重元素的：
>
> - 实现者的难易程度
> - 对现有规则的影响
>
> 显然，语言的演进往往是在多重元素中进行trade-off

If given a reference to a reference (note reference is an encompassing term meaning both `T&` and `T&&`), we use the following rule to figure out the resulting type:

> "[given] a type TR that is a reference to a type T, an attempt to create the type “lvalue reference to cv TR” creates the type “lvalue reference to T”, while an attempt to create the type “rvalue reference to cv TR” creates the type TR."

Or in tabular form:

```cpp
TR   R

T&   &  -> T&  // lvalue reference to cv TR -> lvalue reference to T
T&   && -> T&  // rvalue reference to cv TR -> TR (lvalue reference to T)
T&&  &  -> T&  // lvalue reference to cv TR -> lvalue reference to T
T&&  && -> T&& // rvalue reference to cv TR -> TR (rvalue reference to T)
```

#### Special type deduction rules for rvalue references 

Next, with template argument deduction: if an argument is an lvalue `A`, we supply the template argument with an lvalue reference to `A`. Otherwise, we deduce normally. This gives so-called **universal references** (the term [*forwarding reference*](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4164.pdf) is now the official one).

#### Combine: perfect forwarding

Why is this useful? Because combined we maintain the ability to keep track of the value category of a type: if it was an **lvalue**, we have an **lvalue-reference parameter**, otherwise we have an **rvalue-reference parameter**.

> NOTE: 上面这段话中的combined的意思是：结合Reference collapsing rule和Special type deduction rules for rvalue references 

In code:

```cpp
#include <iostream>

template<typename T>
void deduce(T&& x)
{
	std::cout << x << std::endl;
}

int main()
{
	int i = 0;
	deduce(i); // deduce<int&>(int& &&) -> deduce<int&>(int&)
	deduce(1); // deduce<int>(int&&)
}
// g++ --std=c++11  test.cpp 
```

The last thing is to "forward" the **value category** of the variable. Keep in mind, once inside the function the parameter could be passed as an **lvalue** to anything:

```cpp
#include <iostream>

void foo(int& x)
{
	std::cout << x << std::endl;
}
template<typename T>
void deduce(T&& x)
{
	foo(x); // fine, foo can refer to x
}
int main()
{
	int i = 0;
	deduce(i); // deduce<int&>(int& &&) -> deduce<int&>(int&)
	deduce(1); // deduce<int>(int&&)
}
// g++ --std=c++11 test.cpp 
```

> NOTE: 上述程序可以编译通过，`deduce(1);`



That's no good. `E` needs to get the same kind of value-category that we got! The solution is this:

```cpp
static_cast<T&&>(x);
```

What does this do? Consider we're inside the `deduce` function, and we've been passed an lvalue. This means `T` is a `A&`, and so the target type for the static cast is `A& &&`, or just `A&`. Since `x` is already an `A&`, we do nothing and are left with an lvalue reference.

When we've been passed an rvalue, `T` is `A`, so the target type for the static cast is `A&&`. The cast results in an rvalue expression, *which can no longer be passed to an lvalue reference*. We've maintained the value category of the parameter.

Putting these together gives us "perfect forwarding":

```cpp
template <typename A>
void f(A&& a)
{
    E(static_cast<A&&>(a)); 
}
```

When `f` receives an lvalue, `E` gets an lvalue. When `f` receives an rvalue, `E` gets an rvalue. Perfect.

------

And of course, we want to get rid of the ugly. `static_cast<T&&>` is cryptic and weird to remember; let's instead make a utility function called `std::forward`, which does the same thing:

```cpp
std::forward<A>(a);
// is the same as
static_cast<A&&>(a);
```

## Examples

- `emplace_back`，在thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c/)中，将它作为引言
- [`std::make_unique`](http://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique)
- [`std::make_shared`](http://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared) 

### 一般形式

[What are the main purposes of using std::forward and which problems it solves?](https://stackoverflow.com/questions/3582001/what-are-the-main-purposes-of-using-stdforward-and-which-problems-it-solves)

```cpp
template <typename T1, typename T2>
void outer(T1&& t1, T2&& t2) 
{
    inner(std::forward<T1>(t1), std::forward<T2>(t2));
}
```



### [variadic templates](http://eli.thegreenplace.net/2014/variadic-templates-in-c/) and perfect forwarding 

TODO

### cpppatterns [Perfect forwarding](https://cpppatterns.com/patterns/perfect-forwarding.html)



```c++
#include <iostream>
#include <utility>

template<typename T, typename U>
std::pair<T, U> make_pair_wrapper(T&& t, U&& u)
{
	return std::make_pair(std::forward<T>(t),
			std::forward<U>(u));
}

int main()
{
	auto p = make_pair_wrapper(1, 2);
	std::cout << p.first << " " << p.second << std::endl;
}
// g++ --std=c++11  test.cpp
```



## draft

下面是非常好的例子:

### stackoverflow [How to bind function to an object by reference?](https://stackoverflow.com/questions/26187192/how-to-bind-function-to-an-object-by-reference) # A

**Note on using `std::forward`**

First of all, `std::forward` is meant to be used for [perfect forwarding](https://stackoverflow.com/q/3582001/873025), i.e. to forward the reference type (l-value or r-value).

If you pass an *l-value* reference to `std::forward` that is what is returned, and likewise if an *r-value* reference is passed then an r-value is returned. This works as opposed to `std::move` that will always return an r-value reference. Also remember that *named* r-value references are l-value references.

```c++
/* Function that takes r-value reference. */
void func(my_type&& t) {
    // 't' is named and thus is an l-value reference here.

    // Pass 't' as lvalue reference.
    some_other_func(t);
    // Pass 't' as rvalue reference (forwards rvalueness).
    some_other_func(std::forward<my_type>(t));
    // 'std::move' should be used instead as we know 't' is always an rvalue.
    // e.g. some_other_func(std::move(t));
}
```
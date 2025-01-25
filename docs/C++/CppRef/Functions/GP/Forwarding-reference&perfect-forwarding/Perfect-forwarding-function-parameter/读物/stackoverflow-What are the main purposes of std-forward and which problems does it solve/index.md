# stackoverflow [What are the main purposes of std::forward and which problems does it solve?](https://stackoverflow.com/questions/3582001/what-are-the-main-purposes-of-stdforward-and-which-problems-does-it-solve) # [A](https://stackoverflow.com/a/3582313) 



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

If given a reference to a reference (note reference is an encompassing(包容的、宽泛的) term meaning both `T&` and `T&&`), we use the following rule to figure out the resulting type:

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

Why is this useful? Because combined(结合起来) we maintain the ability to keep track of the value category of a type: if it was an **lvalue**, we have an **lvalue-reference parameter**, otherwise we have an **rvalue-reference parameter**.

> NOTE: 上面这段话中的combined的意思是：结合Reference collapsing rule和Special type deduction rules for rvalue references 

In code:

```cpp
#include <iostream>

template<typename T>
void deduce(T &&x) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    std::cout << x << std::endl;
}

int main() {
    int i = 0;
    deduce(i); // deduce<int&>(int& &&) -> deduce<int&>(int&)
    deduce(1); // deduce<int>(int&&)
}
// g++ --std=c++11  test.cpp

/**
输出如下:
void deduce(T &&) [T = int &]
0
void deduce(T &&) [T = int]
1
**/
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


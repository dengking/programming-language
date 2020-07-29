# Perfect forwarding

## 内容梳理

Perfect forwarding是一直以来困扰C++ programmer的问题（在文章thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c)的“The perfect forwarding problem”段中对它进行了说明），C++11对该问题进行了解决；

要想搞清楚perfect forwarding，需要首先搞清楚如下内容：

- Reference collapsing rule
- Special type deduction rules for rvalue references

上述两者，在文章thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c)的Reference collapsing and special type deduction for rvalues中进行了总结；

在建立了上述认知后，我们就可以知道forwarding reference了：

> forwarding reference = reference collapsing rule + special type deduction rules for rvalue references；

forwarding reference既可以reference **lvalue**又可以reference **rvalue**，所以它是universal，这是它的另外一个名称universal reference中universal的含义；

实现perfect forwarding：

> perfect forwarding = forwarding reference + `std::forward`；



在文章thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c)中，对它们有着较好的介绍，所以以它为入门读物；

## thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c)

### The perfect forwarding problem

Let `func(E1, E2, ..., En)` be an arbitrary function call with generic parameters `E1, E2, ..., En`. We'd like to write a function `wrapper` such that `wrapper(E1, E2, ..., En)` is equivalent to `func(E1, E2, ..., En)`. In other words, we'd like to define a function with generic parameters that **forwards** its parameters *perfectly* to some other function.

> NOTE: *perfectly* 的 含义是什么？其实就是 保持入参的**value category**。

To have something concrete to relate this definition to, think of the `emplace_back` method discussed above. `vector<T>::emplace_back` **forwards** its parameters to a constructor of `T`, without actually knowing how `T` looks like.

Next, I'm going to show a few examples of how we might approach this in pre-11 C++. For simplicity's sake, I'll put variadic templates aside; let's assume all we need to forward is two arguments.

#### First approach: pass-by-value

The first approach that comes to mind is:

```c++
template <typename T1, typename T2>
void wrapper(T1 e1, T2 e2) {
    func(e1, e2);
}
```

This will obviously not work if `func` accepts its parameters by **reference**, since `wrapper` introduces a **by-value** passing step. If `func` modifies its **by-reference** parameter, it won't be visible in the caller of `wrapper` (only the copy created by `wrapper` itself will be affected).

#### Second approach: pass-by-reference

OK, then, we can make `wrapper` accept its parameters by reference. This should not interfere with `func`'s taking parameters by value, because the call to `func` within `wrapper` will create the required copy.

```
template <typename T1, typename T2>
void wrapper(T1& e1, T2& e2) {
    func(e1, e2);
}
```

This has another problem, though. **Rvalues** cannot be bound to function parameters that are **references**, so the following completely reasonable calls will now fail:

```c++
wrapper(42, 3.14f);                  // error: invalid initialization of
                                     //        non-const reference from
                                     //        an rvalue

wrapper(i, foo_returning_float());   // same error
```

And no, making those reference parameters `const` won't cut it either, because `func` may legitimately want to accept non-`const` reference parameters.

#### Third approach: brute-force 

What remains is the **brute-force** approach taken by some libraries: define overloads for both `const` and non-`const` references:

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

Exponential explosion. You can imagine how much fun this becomes when we want to cover some reasonable amount of function parameters. To make things worse, C++11 adds **rvalue references** to the mix (which we'd also want to **forward** correctly), and this clearly isn't a scalable solution.



### Reference collapsing and special type deduction for rvalues

To explain how C++11 solves the **perfect forwarding** problem, we have to first understand two new rules that were added to the language.

#### Reference collapsing rule

**Reference collapsing** is the easier one to explain, so let's start with it. Taking a reference to a reference is illegal in C++. However, it can sometimes arise in the context of templates and type deduction:

```c++
template <typename T>
void baz(T t) {
  T& k = t;
}
```

What happens if we call this function as follows:

```c++
int ii = 4;
baz<int&>(ii);
```

In the template instantiation, `T` is explicitly set to `int&`. So what is the type of `k` inside? What the compiler "sees" is `int& &` - while this isn't something the user is allowed to write in code, the compiler simply infers a single reference from this. In fact, prior to C++11 this wasn't standardized, but many compilers accepted such code anyway because these cases occasionally arise in template metaprogramming. With the addition of [rvalue references](http://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c) in C++11, it became important to define what happens when various **reference types** augment (e.g. what does `int&& &` mean?).



The result is the *reference collapsing* rule. The rule is very simple. `&` always wins. So `& &` is `&`, and so are `&& &` and `& &&`. The only case where `&&` emerges from collapsing is `&& &&`. You can think of it as a logical-OR, with `&` being 1 and `&&` being 0.

> NOTE: 在 https://stackoverflow.com/a/40835219 中，对其的总结如下：
>
> - `&& && = &&`
> - `&& & = &`
> - `& && = &`
> - `& & = &`

#### Special type deduction rules for rvalue references 

The other addition to C++11 relevant to this article is **special type deduction rules** for **rvalue references** in some cases [[1\]](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c#id5). Given a function template like:

```c++
template <class T>
void func(T&& t) {
}
```

Don't let `T&&` fool you here - `t` is not an **rvalue reference** [[2\]](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c#id6). When it appears in a **type-deducing context**, `T&&` acquires a special meaning. When `func` is instantiated, `T` depends on whether the argument passed to `func` is an **lvalue** or an **rvalue**. If it's an **lvalue** of type `U`, `T` is deduced to `U&`. If it's an **rvalue**, `T` is deduced to `U`:

```c++
func(4);            // 4 is an rvalue: T deduced to int

double d = 3.14;
func(d);            // d is an lvalue; T deduced to double&

float f() {...}
func(f());          // f() is an rvalue; T deduced to float

int bar(int i) {
  func(i);          // i is an lvalue; T deduced to int&
}
```

> NOTE:   
>
> ## Example
>
> 上面的例子不够详细，在 https://stackoverflow.com/a/40835219 中给出了一个更加详细的例子:
>
> ```c++
> #include <iostream>
> 
> template<class T>
> void foo(T&& t)
> {
> 	std::cout << t << std::endl;
> }
> int main()
> {
> 	//...
> 	const int i = 42;
> 	foo(i); // the compiler will defines T = const int &
> 			//          T&&  = const int & && = const int &
> 			// => the compiler instantiates void foo<const int &>(const int &);
> 	foo(6 * 7);	         // the compiler will defines T = int
> 						 //          T&&  = int &&
> 						 // the compiler instantiates  void foo<int>(int &&);
> }
> // g++ --std=c++11  test.cpp
> ```
>
> 输出如下:
>
> ```
> 42
> 42
> ```
>
> 上述例子中，`t`就是所谓的**forwarding reference**，通过上述例子可以看出，`t`既可以bind to lvalue，也可以bind to rvalue，通过上述例子可以看出，**forwarding reference**的实现是有赖于前面提到的reference collapsing rule、special type deduction rules for rvalue references ，这就是在**内容梳理**中总结的：
>
> > forwarding reference = reference collapsing rule + special type deduction rules for rvalue references；



This rule may seem unusual and strange. That's because it is. However, it starts making sense when we realize it was designed to solve the **perfect forwarding** problem.



### Solving perfect forwarding with `std::forward`

Let's get back to our original `wrapper` template. Here's how it should be written in C++11:

```c++
template <typename T1, typename T2>
void wrapper(T1&& e1, T2&& e2) {
    func(forward<T1>(e1), forward<T2>(e2));
}
```

And this is `forward`:

```c++
/**
 *  @brief  Forward an lvalue.
 *  @return The parameter cast to the specified type.
 *
 *  This function is used to implement "perfect forwarding".
 */
template<class T>
T&& forward(typename std::remove_reference<T>::type& t) noexcept
{
	return static_cast<T&&>(t);
}
/**
 *  @brief  Forward an rvalue.
 *  @return The parameter cast to the specified type.
 *
 *  This function is used to implement "perfect forwarding".
 */
template<class T>
T&& forward(typename std::remove_reference<T>::type&& t) noexcept
{
	return static_cast<T&&>(t);
}

```

> NOTE: 上述给出的其实是[libstdc++ std::forward](https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/move.h)的实现；

#### Perfect forwarding lvalue

Let's say we call:

```c++
int ii ...;
float ff ...;
wrapper(ii, ff);
```

Examining the first argument (since the second is handled similarly): `ii` is an lvalue, so `T1` is deduced to `int&` following the special deduction rules. We get the call `func(forward<int&>(e1), ...)`. Therefore, `forward` is instantiated with `int&` and we get this version of it:

```c++
int& && forward(int& t) noexcept {
    return static_cast<int& &&>(t);
}
```

Now it's time to apply the reference collapsing rule:

```c++
int& forward(int& t) noexcept {
    return static_cast<int&>(t);
}
```

In other words, the argument is passed on by reference to `func`, as needed for lvalues.

#### Perfect forwarding rvalue

The other case to handle is:

```c++
wrapper(42, 3.14f);
```

Here the arguments are rvalues, so `T1` is deduced to `int`. We get the call `func(forward<int>(e1), ...)`. Therefore, `forward` is instantiated with `int` and we get this version of it [[3\]](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c#id7):

```c++
int&& forward(int&& t) noexcept {
    return static_cast<int&&>(t);
}
```

#### Source code

使用`std::forward`

```c++
#include <iostream>
#include <utility>

template<class T1, typename T2>
void func(T1&& t1, T2&& t2)
{
	std::cout << t1 << " " << t2 << std::endl;
}

template<typename T1, typename T2>
void wrapper(T1&& e1, T2&& e2)
{
	func(std::forward<T1>(e1), std::forward<T2>(e2));
}

int main()
{
	// forwarding lvalue
	int ii = 42;
	float ff = 3.14;
	wrapper(ii, ff);
	// forwarding rvalue
	wrapper(42, 3.14f);
}
// g++ --std=c++11  test.cpp
```

不使用`std::forward`

```c++
#include <iostream>
#include <utility>

template<class T1, typename T2>
void func(T1&& t1, T2&& t2)
{
	std::cout << t1 << " " << t2 << std::endl;
}

template<typename T1, typename T2>
void wrapper(T1&& e1, T2&& e2)
{
	func(e1, e2);
	// func(std::forward<T1>(e1), std::forward<T2>(e2));
}

int main()
{
	// forwarding lvalue
	int ii = 42;
	float ff = 3.14;
	wrapper(ii, ff);
	// forwarding rvalue
	wrapper(42, 3.14f);
}
// g++ --std=c++11  test.cpp

```

与上面是相同的运行结果；

#### `std::forward`

One can see `forward` as a pretty wrapper around `static_cast<T&&>(t)` when `T` can be deduced to either `U&` or `U&&`, depending on the kind of argument to the wrapper (lvalue or rvalue). Now we get `wrapper` as a single template that handles all kinds of forwarding cleanly.

The `forward` template exists in C++11, in the `<utility>` header, as `std::forward`.

Another thing I want to mention is the use of `std::remove_reference<T>`. In fact, if you think about it, `forward` could do without it. **Reference collapsing** does the job already, so `std::remove_reference<T>` is superfluous（多余的）. It's there to turn the `T& t` into a **non-deducing context** (according to the C++ standard, section 14.8.2.5), thus forcing us to explicitly specify the **template parameter** when calling `std::forward`.



### Universal references

In his talks, blog posts and book, Scott Myers gave the name "universal references" to **rvalues** that appear in a **type-deducing context**. Whether this is a useful mnemonic（助记） or not depends on the beholder（旁观者）; personally, when I first read the relevant chapters of the new "Effective C++", I found myself seriously confused on this topic. Only later, when I understood the underlying mechanisms (**reference collapsing** and special **deduction rules**), the topic became somewhat clearer.

The trap（问题） is that saying "universal references" [[4\]](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c#id8) is surely more succinct（简明的） and nice than "rvalue references in a **type deducing context**", but once you want to really understand some piece of code (rather than just cargo-culting boilerplate), you'll find that avoiding the full definition is impossible.

> 翻译：问题是，说“通用引用”[4]肯定比“类型推断上下文中的右值引用”更简洁、更漂亮，但是一旦您想真正理解某段代码(而不是仅仅是一些样板文件)，您将发现避免完整的定义是不可能的。

### Examples of using perfect forwarding

**Perfect forwarding** is extremely useful, because it enables a kind of **higher order programming**. *Higher order functions* are functions that may take other functions as arguments or return them. Without **perfect forwarding**, **higher order functions** are cumbersome（笨重的） because there is no convenient way to forward arguments to wrapped functions. And by "functions" here I mean classes as well, whose constructors are still functions.

In the beginning of the article I mentioned the `emplace_back` method of containers. Another good examples is `make_unique`, which I described in the [previous article](http://eli.thegreenplace.net/2014/variadic-templates-in-c/):

```c++
template<typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args)
{
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}
```

There, I pleaded to ignore the strange `&&` syntax and focus on the variadic template packs, but now there's no trouble fully understanding the code. It goes without saying that perfect forwarding and variadic templates very often go hand in hand, because we generally don't know how many arguments the functions or constructors we pass around accept.

For a significantly more complex use of **perfect forwarding**, you may also want to take a look at `std::bind`.





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

In C++11, we get a chance to fix this. [One solution modifies template deduction rules on existing types, but this potentially breaks a great deal of code.](https://stackoverflow.com/questions/3591832/perfect-forwarding-in-c03/3591909#3591909) So we have to find another way.

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

Next, with template argument deduction: if an argument is an lvalue A, we supply the template argument with an lvalue reference to A. Otherwise, we deduce normally. This gives so-called **universal references** (the term [*forwarding reference*](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4164.pdf) is now the official one).

Why is this useful? Because combined we maintain the ability to keep track of the value category of a type: if it was an lvalue, we have an **lvalue-reference parameter**, otherwise we have an **rvalue-reference parameter**.

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

The last thing is to "forward" the value category of the variable. Keep in mind, once inside the function the parameter could be passed as an lvalue to anything:

```cpp
void foo(int&);

template <typename T>
void deduce(T&& x)
{
    foo(x); // fine, foo can refer to x
}

deduce(1); // okay, foo operates on x which has a value of 1
```

That's no good. E needs to get the same kind of value-category that we got! The solution is this:

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

And of course, we want to get rid of the ugly. `static_cast<T&&>` is cryptic and weird to remember; let's instead make a utility function called `forward`, which does the same thing:

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



## C++ std::move and std::forward

http://bajamircea.github.io/coding/cpp/2016/04/07/move-forward.html



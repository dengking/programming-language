# The interface principle

## 前言

Interface Principle使我们从一个更高的角度来思考C++语言的设计与实现，它拓宽了我们对OOP的认知，它让我更加深刻地感受了：“设计与实现”，或者说：“理念与实现” 之间的关系，OOP是理念，它的实现方式可以有多种；通过[Herb Sutter](http://en.wikipedia.org/wiki/Herb_Sutter)的文章[What's In a Class? - The Interface Principle](http://www.gotw.ca/publications/mill02.htm)，我们可以看到C++对OOP的支持是非常广泛、灵活的；从这两篇文章，我们可以得知：

> C++ ADL是增强其对Interface Principle的支持，是为了更好、更灵活地支持OOP。

Interface是一种抽象，显然interface principle，其实遵循了[Abstraction principle](https://en.wikipedia.org/wiki/Abstraction_principle_(computer_programming))。Interface principle make C++ program generic、extensible。Interface principle是behavior defined（参见`Theory\Programming-paradigm\Generic-programming\Templates-and-Duck-TypingTemplates-and-Duck-Typing.md`）。

### Interface principle VS C++ template

- 都遵循[Abstraction principle](https://en.wikipedia.org/wiki/Abstraction_principle_(computer_programming))
-  **behavior-based**

### ADL VS C++ template

- 都是static的，即发生于compile-time



### Interface principle make C++ generic and extensible

维基百科[Argument-dependent name lookup#Interfaces](https://en.wikipedia.org/wiki/Argument-dependent_name_lookup#Interfaces)：

> Functions found by ADL are considered part of a class's interface. In the C++ Standard Library, several algorithms use unqualified calls to `swap` from within the `std` namespace. As a result, the generic `std::swap` function is used if nothing else is found, but if these algorithms are used with a third-party class, `Foo`, found in another namespace that also contains `swap(Foo&, Foo&)`, that overload of `swap` will be used.



## gotw [What's In a Class? - The Interface Principle](http://www.gotw.ca/publications/mill02.htm)

I'll start off with a deceptively（看似） simple question:

o     What's in a class? That is, what is "part of" a class and its interface?

The deeper questions are:

o     How does this answer fit with C-style object-oriented programming?

o     How does it fit with C++'s Koenig lookup? with the Myers Example? (I'll describe both.)

o     How does it affect the way we analyze class dependencies and design object models?



### So, "What's In a Class?"

First, recall a traditional definition of a class:

> **Class (definition)
> A class describes a set of data along with the functions that operate on that data.**

Programmers often unconsciously（不知不觉） misinterpret this definition, saying instead: "Oh yeah, a class, that's what appears in the class definition -- the **member data** and the **member functions**." But that's not the same thing, because it limits the word "**functions**" to mean just "**member functions**." Consider:

> NOTE: 作者想要表达的意图是：function不仅仅局限于member function，还包括non-member function；

#### Example 1 (a)

```c++
//*** Example 1 (a)
class X
{ /*...*/
};

/*...*/

void f(const X&);
```

The question is: Is `f` part of `X`? Some people will automatically say "No" because `f` is a **nonmember function** (or "free function"). Others might realize something fundamentally important: If the **Example 1 (a)** code appears together in one header file, it is not significantly different from:

#### Example 1 (b)

```c++
//*** Example 1 (b)
class X
{ /*...*/
public:
	void f() const;
};
```

Think about this for a moment. Besides access rights,**[[1\]](http://www.gotw.ca/publications/mill02.htm#1)** `f` is still the same, taking a pointer/reference to `X`. The this parameter is just implicit in the second version, that's all. So, if **Example 1 (a)** all appears in the same header, we're already starting to see that even though `f` is not a member of `X`, it's nonetheless strongly related to `X`. I'll show what exactly that relationship is in the next section.

> NOTE: 在**Example 1 (a)**中，`f`和`X`密切相关。

On the other hand, if `X` and `f` do not appear together in the same header file, then `f` is just some old client function, not a part of `X` (even if `f` is intended to augment `X`). We routinely write functions with parameters whose types come from library headers, and clearly our custom functions aren't part of those library classes.

### The Interface Principle

With that example in mind, I'll propose the **Interface Principle**:

> The **Interface Principle**
> For a class `X`, all functions, including free functions, that both
> (a) "mention" `X`, and
> (b) are "supplied with" `X`
> are logically part of `X`, because they form part of the interface of `X`.

By definition every member function is "part of" `X`:

(a) every **member function** must "mention" `X` (a nonstatic member function has an implicit this parameter of type `X*` or `const X*`; a static member function is in the scope of `X`); and

(b) every member function must be "supplied with" `X` (in `X`'s definition).

Applying the **Interface Principle** to **Example 1 (a)** gives the same result as our original analysis: Clearly, `f` mentions `X`. If `f` is also "supplied with" `X` (for example, if they come in the same **header file** and/or **namespace** [2](http://www.gotw.ca/publications/mill02.htm#2)), then according to the Interface Principle `f` is logically part of `X` because it forms part of the interface of `X`.

> NOTE: 上面作者对supply进行了详细的解释。

So the **Interface Principle** is a useful touchstone（试金石） to determine what is really "part of" a class. Do you find it unintuitive（不直观） that a free function should be considered part of a class? Then let's give real weight to this example by giving a more common name to `f`:

#### Example 1 (c)

```c++
#include <iostream>

//*** Example 1 (c)
class X
{ /*...*/
};

/*...*/

std::ostream& operator<<(std::ostream&, const X&);
```

Here the **Interface Principle**'s rationale is perfectly clear, because we understand how this particular free function works: If `operator<<` is "supplied with" `X` (for example, in the same header and/or namespace), then `operator<<` is logically part of `X` because it forms part of the interface of `X`. That makes sense even though the function is a nonmember, because we know that it's common practice for a class' author to provide `operator<<`. If instead `operator<<` comes, not from `X`'s author, but from client code, then it's not part of `X` because it's not "supplied with" `X`.**[[3\]](http://www.gotw.ca/publications/mill02.htm#3)**

In this light, then, let's return to the traditional definition of a class:

> Class (definition)
> A class describes a set of data along with the functions that operate on that data.

That definition is exactly right, for it doesn't say a thing about whether the "functions" in question are members or not.

### Is the IP an OO Principle, or Just a C++-Specific Principle?

I've been using C++ terms like "namespace" to describe what "supplied with" means, so is the IP C++-specific? Or is it a general OO principle that can apply in other languages?

Consider a familiar example from another (in fact, a non-OO) language: C.

#### Example 2 (a)

```c++
/*** Example 2 (a) ***/

struct _iobuf
{ /*...data goes here...*/
};
typedef struct _iobuf FILE;

FILE* fopen(const char* filename,
		const char* mode);
int fclose(FILE* stream);
int fseek(FILE* stream,
		long offset,
		int origin);
long ftell(FILE* stream);
/* etc. */
```

This is the standard "handle technique" for writing **OO** code in a language that doesn't have classes: You provide a structure that holds the object's data, and functions -- necessarily nonmembers -- that take or return **pointers** to that structure. These free functions construct (`fopen`), destroy (`fclose`), and manipulate (`fseek`, `ftell`, etc.) the data.

This technique has disadvantages (for example, it relies on client programmers to refrain from fiddling with the data directly), but it's still "real" OO code -- after all, a class is "a set of data along with the functions that operate on that data." In this case of necessity the functions are all nonmembers, but they are still part of the interface of `FILE`.

Now consider an "obvious" way to rewrite **Example 2 (a)** in a language that does have classes:

#### Example 2 (b) 

```c++
//*** Example 2 (b)
class FILE
{
public:
	FILE(const char* filename,
			const char* mode);
	~FILE();
	int fseek(long offset, int origin);
	long ftell();
	/* etc. */
private:
	/*...data goes here...*/
};
```

The `FILE*` parameters have just become implicit this parameters. Here it's clear that `fseek` is part of `FILE`, just as it was in Example 2 (a) even though there it was a nonmember. We can even merrily make some functions members and some not:

#### Example 2 (c)

```c++
//*** Example 2 (c)
class FILE
{
public:
	FILE(const char* filename,
			const char* mode);
	~FILE();
	long ftell();
	/* etc. */
private:
	/*...data goes here...*/
};

int fseek(FILE* stream,
		long offset,
		int origin);
```

It really doesn't matter whether or not the functions are members. As long as they "mention" `FILE` and are "supplied with" `FILE`, they really are part of `FILE`. In Example 2 (a), all of the functions were nonmembers because in C they have to be. Even in C++, some functions in a class' interface have to be (or should be) nonmembers: `operator<<` can't be a member because it requires a stream as the left-hand argument, and `operator+` shouldn't be a member in order to allow conversions on the left-hand argument.

### Introducing Koenig Lookup

The **Interface Principle** makes even more sense when you realize that it does exactly the same thing as **Koenig lookup**.**[[4\]](http://www.gotw.ca/publications/mill02.htm#4)** Here, I'll use two examples to illustrate and define **Koenig lookup**. In the next section, I'll use the **Myers Example** to show why this is directly related to the **Interface Principle**.

Here's why we need Koenig lookup, using an example right out of the standards document:

#### Example 3 (a)

```c++
//*** Example 3 (a)
#include <iostream>

namespace NS
{
class T
{
};
void f(T)
{
	std::cout << "NS::f" << std::endl;
}
}

NS::T parm;

int main()
{
	f(parm);    // OK: calls NS::f
}
```

> NOTE: 编译`g++ test.cpp`，输出如下:
>
> ```
> NS::f
> ```

Pretty nifty, isn't it? "Obviously" the programmer shouldn't have to explicitly write `NS::f(parm)`, because just `f(parm)` "obviously" means `NS::f(parm)`, right? But what's obvious to us isn't always obvious to a compiler, especially considering that there's nary（没有） a "using" in sight to bring the name `f` into scope. Koenig lookup lets the compiler do the right thing.

Here's how it works: Recall that "name lookup" just means that, whenever you write a call like "`f(parm)`", the compiler has to figure out which function named `f` you want. (With overloading and scoping there could be several functions named `f`.) **Koenig lookup** says that, if you supply a function argument of class type (here parm, of type `NS::T`), then to find the function name the compiler is required to look, not just in the usual places like the local scope, but also in the namespace (here `NS`) that contains the argument's type.**[[5\]](http://www.gotw.ca/publications/mill02.htm#5)** And so Example 3 (a) works: The parameter being passed to `f` is a `T`, `T` is defined in namespace `NS`, and the compiler can consider the `f` in namespace `NS` -- no fuss, no muss.

It's good that we don't have to explicitly qualify `f`, because sometimes we *can't* easily qualify a function name:

#### Example 3 (b)

```c++
//*** Example 3 (b)
#include <iostream>
#include <string>  // this header
//   declares the free function
//   std::operator<< for strings

int main()
{
	std::string hello = "Hello, world";
	std::cout << hello; // OK: calls
}                     //   std::operator<<
```

Here the compiler has no way to find `operator<<` without **Koenig lookup**, because the `operator<<` we want is a free function that's made known to us only as part of the **string** package. It would be disgraceful if the programmer were forced to qualify this function name, because then the last line couldn't use the operator naturally. Instead, we would have to write either "`std::operator<<( std::cout, hello );`" or "`using namespace std;`". If those options send shivers down your spine, you understand why we need Koenig lookup.

> NOTE: ” the `operator<<` we want is a free function that's made known to us only as part of the **string** package “的意思是 " 我们需要的`operator<<` 是在 string package 中定义的。

Summary: If in the same namespace you supply a class and a free function that mentions that class,**[[6\]](http://www.gotw.ca/publications/mill02.htm#6)** the compiler will enforce a strong relationship between the two.**[[7\]](http://www.gotw.ca/publications/mill02.htm#7)** And that brings us back to the Interface Principle, because of the Myers Example:

### More Koenig Lookup: The Myers Example

Consider first a (slightly) simplified example:

#### Example 4 (a)

```c++
#include <iostream>

//*** Example 4 (a)
namespace NS
{ // typically from some
class T
{
};
//   header T.h
}

void f(NS::T)
{
	std::cout << "f(NS::T)" << std::endl;
}

int main()
{
	NS::T parm;
	f(parm);     // OK: calls global f
}
```

> NOTE: 编译`g++ test.cpp`，输出如下:
>
> ```
> f(NS::T)
> ```

Namespace `NS` supplies a type `T`, and the outside code provides a global function `f` that happens to take a `T`. This is fine, the sky is blue, the world is at peace, and everything is wonderful.

Time passes. One fine day, the author of NS helpfully adds a function:

#### Example 4 (b)

```c++
#include <iostream>

//*** Example 4 (b)
namespace NS
{ // typically from some
class T
{
};
//   header T.h
void f(T) // <-- new function
{
	std::cout << "NS::f" << std::endl;
}
}

void f(NS::T)
{
	std::cout << "f(NS::T)" << std::endl;
}

int main()
{
	NS::T parm;
	f(parm);     // ambiguous: NS::f
}              //   or global f?
```

> NOTE: 编译`g++ test.cpp`，报错如下:
>
> ```c++
> test.cpp: 在函数‘int main()’中:
> test.cpp:24:8: 错误：调用重载的‘f(NS::T&)’有歧义
> f(parm);     // ambiguous: NS::f
>      ^
> test.cpp:24:8: 附注：备选是：
> test.cpp:16:6: 附注：void f(NS::T)
> void f(NS::T)
>    ^
> test.cpp:10:6: 附注：void NS::f(NS::T)
> void f(T) // <-- new function
> ```
>
> 

Adding a function in a namespace scope "broke" code outside the namespace, even though the client code didn't write `using` to bring `NS`'s names into its scope! But wait, it gets better -- Nathan Myers**[[8\]](http://www.gotw.ca/publications/mill02.htm#8)** pointed out the following interesting behaviour with namespaces and Koenig lookup:

#### The Myers Example: "Before"

```c++
#include <iostream>

//*** The Myers Example: "Before"
namespace A
{
class X
{
};
}

namespace B
{
void f(A::X)
{
	std::cout << "B::f(A::X)" << std::endl;
}
void g(A::X parm)
{
	f(parm);   // OK: calls B::f
}
}
int main()
{
	B::g(A::X());
}
```

> NOTE: 编译`g++ test.cpp`，输出如下:
>
> ```
> B::f(A::X)
> ```
>
> 

This is fine, the sky is blue, etc. One fine day, the author of `A` helpfully adds another function:

#### The Myers Example: "After"

```c++
#include <iostream>

//*** The Myers Example: "After"
namespace A
{
class X
{
};
void f(X) // <-- new function
{
	std::cout << "A::f(X)" << std::endl;
}
}

namespace B
{
void f(A::X)
{
	std::cout << "B::f(X)" << std::endl;
}
void g(A::X parm)
{
	f(parm);   // ambiguous: A::f or B::f?
}
}

int main()
{
	B::g(A::X());
}
```

> NOTE: 编译`g++ test.cpp`，报错如下:
>
> ```c++
> test.cpp: 在函数‘void B::g(A::X)’中:
> test.cpp:23:8: 错误：调用重载的‘f(A::X&)’有歧义
> f(parm);   // ambiguous: A::f or B::f?
>      ^
> test.cpp:23:8: 附注：备选是：
> test.cpp:17:6: 附注：void B::f(A::X)
> void f(A::X)
>    ^
> test.cpp:9:6: 附注：void A::f(A::X)
> void f(X) // <-- new function
> 
> ```
>
> 

"Huh?" you might ask. "The whole point of namespaces is to prevent name collisions, isn't it? But adding a function in one namespace actually seems to 'break' code in a completely separate namespace." True, namespace `B`'s code seems to "break" merely because it mentions a type from `A`. `B`'s code didn't write a `using` namespace `A`; anywhere. It didn't even write `using A::X`;.

This is not a problem, and `B` is not "broken." This is in fact *exactly* what should happen.**[[9\]](http://www.gotw.ca/publications/mill02.htm#9)** If there's a function `f(X)` in the same namespace as `X`, then, according to the **Interface Principle**, `f` is part of the interface of `X`. It doesn't matter a whit that `f` happens to be a free function; to see clearly that it's nonetheless logically part of `X`, again just give it another name:

```c++
#include <iostream>
using namespace std;
//*** Restating the Myers Example: "After"
namespace A
{
class X
{
};
ostream& operator<<(ostream& S, const X&)
{
	return S<<"A::operator<<";
}
}

namespace B
{
ostream& operator<<(ostream& S, const A::X&)
{
	return S<<"B::operator<<";
}
void g(A::X parm)
{
	cout << parm; // ambiguous:
}               //   A::operator<< or
}                 //   B::operator<<?

int main()
{
	B::g(A::X());
}

```

> NOTE: 编译指令`g++ test.cpp`，编译报错如下:
>
> ```c++
> test.cpp: 在函数‘void B::g(A::X)’中:
> test.cpp:29:7: 错误：ambiguous overload for ‘operator<<’ (operand types are ‘std::ostream {aka std::basic_ostream<char>}’ and ‘A::X’)
> cout << parm; // ambiguous:
>     ^
> test.cpp:29:7: 附注：备选是：
> test.cpp:23:10: 附注：std::ostream& B::operator<<(std::ostream&, const A::X&)
> ostream& operator<<(ostream& S, const A::X&)
>        ^
> test.cpp:15:10: 附注：std::ostream& A::operator<<(std::ostream&, const A::X&)
> ostream& operator<<(ostream& S, const X&)
> ```
>
> 

If client code supplies a function which mentions `X` and matches the signature of one provided in the same namespace as `X`, the call *should* be ambiguous. `B` *should* be forced to say which competing function it means, its own or that supplied with `X`. This is exactly what we should expect given the IP:

> **The Interface Principle
> For a class X, all functions, including free functions, that both
> (a) "mention" X, and
> (b) are "supplied with" X
> are logically part of X, because they form part of the interface of X.**

In short, it's no accident that the Interface Principle works exactly the same way as **Koenig lookup**. **Koenig lookup** works the way that it does fundamentally *because* of the **Interface Principle**.

(The box *"How Strong Is the* *Relationship?"* shows why a member function is still more strongly related to a class than a nonmember.)

---

### How Strong Is the "Part Of" Relationship?

While the Interface Principle states that both member and nonmember functions can be logically "part of" a class, it doesn't claim that members and nonmembers are equivalent. For example, member functions automatically have full access to class internals whereas nonmembers only have such access if they're made friends. Likewise for name lookup, including Koenig lookup, the `C++` language deliberately says that a member function is to be considered more strongly related to a class than a nonmember:

```c++
#include <iostream>
using namespace std;
//*** NOT the Myers Example
namespace A
{
class X
{
};
void f(X)
{

}
}

class B
{
public:
// class, not namespace
	void f(A::X)
	{
		std::cout << "B::f";
	}
	void g(A::X parm)
	{
		f(parm); // OK: B::f,
				 // not ambiguous
	}
};

int main()
{
	B b;
	b.g(A::X());
}

```

> NOTE: 编译`g++ test.cpp`，输出如下:
>
> ```
> B::f
> ```
>
> 

Now that we're talking about a *class* B, rather than a *namespace* B, there's no ambiguity: When the compiler finds a member named `f`, it won't bother trying to use Koenig lookup to find free functions.

So in two major ways -- access rules and lookup rules -- even when a function is "part of" a class according to the Interface Principle, a member is more strongly related to the class than a nonmember.



---

### What Does a Class Depend On?

"What's in a class?" isn't just a philosophical question. It's a fundamentally practical question, because without the correct answer we can't properly analyze class dependencies.

To demonstrate this, consider a seemingly unrelated problem: What `operator<<` for a class? There are two main ways, both of which involve tradeoffs. I'll analyze both, and in the end we'll find that we're back to the **Interface Principle** and that it has given us important guidance to analyze the tradeoffs correctly.

Here's the first way:

#### Example 5 (a)

```c++
#include <iostream>
using namespace std;
//*** Example 5 (a) -- nonvirtual streaming

class X
{
	/*...ostream is never mentioned here...*/
};

ostream& operator<<(ostream& o, const X& x)
{
	/* code to output an X to a stream */
	return o;
}

int main()
{
}
```

Here's the second:

#### Example 5 (b)

```c++
#include <iostream>
using namespace std;

//*** Example 5 (b) -- virtual streaming

class X
{
public:
	virtual ostream& print(ostream& o) const
	{
		/* code to output an X to a stream */
		return o;
	}
};

ostream& operator<<(ostream& o, const X& x)
{
	return x.print(o);
}

int main()
{
}

```

> NOTE:编译`g++ test.cpp`

Assume that in both cases the class and the function appear in the same header and/or namespace. Which one would you choose? What are the tradeoffs? Historically, experienced C++ programmers have analyzed these options this way:

o     Option (a)'s advantage is that `X` has fewer dependencies. Because no member function of `X` mentions `ostream`, `X` does not *[appear to]* depend on `ostream`. Option (a) also avoids the overhead of an extra virtual function call.

o     Option (b)'s advantage is that any `DerivedX` will also print correctly, even when an `X&` is passed to `operator<<`.

This analysis is flawed. Armed with the **Interface Principle**, we can see why -- the first advantage in Option (a) is a phantom（幻觉）, as indicated by the comments in italics:

\1.    According to the IP, as long as `operator<<` both "mentions" `X` (true in both cases) and is "supplied with" `X` (true in both cases), it is logically part of `X`.

\2.    In both cases `operator<<` mentions ostream, so `operator<<` depends on ostream.

\3.    Since in both cases `operator<<` is logically part of `X` and `operator<<` depends on `ostream`, therefore in both cases `X` depends on `ostream`.

So what we've traditionally thought of as Option (a)'s main advantage is not an advantage at all -- in both cases `X` still in fact depends on `ostream` anyway! If, as is typical, `operator<<` and `X` appear in the same header `X.h`, then both `X`'s own implementation module and all client modules that use `X` physically depend on `ostream` and require at least its forward declaration in order to compile.

With Option (a)'s first advantage exposed as a phantom, the choice really boils down to just the virtual function call overhead. Without applying the **Interface Principle**, though, we would not have been able to as easily analyze the true dependencies (and therefore the true tradeoffs) in this common real-world example.

Bottom line, it's not always useful to distinguish between members and nonmembers, especially when it comes to analyzing dependencies, and that's exactly what the **Interface Principle** implies.

### Some Interesting (and Even Surprising) Results

In general, if `A` and `B` are classes and `f(A,B)` is a free function:

-  If `A` and `f` are supplied together, then `f` is part of `A` and so `A` depends on `B`.
- If `B` and `f` are supplied together, then `f` is part of `B` and so `B` depends on `A`.
- If `A`, `B`, and `f` are supplied together, then `f` is part of both `A` and `B`, and so `A` and `B` are interdependent. This has long made sense on an instinctive level... if the library author supplies two classes and an operation that uses both, the three are probably intended to be used together. Now, however, the Interface Principle has given us a way to rigorously prove this interdependency.

Finally, we get to the really interesting case. In general, if `A` and `B` are classes and `A::g(B)` is a member function of `A`:

- Because `A::g(B)` exists, clearly `A` always depends on `B`. No surprises so far.
- If `A` and `B` are supplied together, then of course `A::g(B)` and `B` are supplied together. Therefore, because `A::g(B)` both "mentions" `B` and is "supplied with" `B`, then according to the **Interface Principle** it follows (perhaps surprisingly, at first!) that `A::g(B)` is part of `B` and, because `A::g(B)` uses an (implicit) `A*` parameter, `B` depends on `A`. Because `A` also depends on `B`, this means that `A` and `B` are interdependent.

At first, it might seem like a stretch to consider a member function of one class as also part of another class, but this is only true if A and B are also *supplied together*. Consider: If A and B are supplied together (say, in the same header file) and A mentions B in a member function like this, "gut feel" already usually tells us A and B are probably interdependent. They are certainly strongly coupled and cohesive, and the fact that they are supplied together and interact means that: (a) they are intended to be used together, and (b) changes to one affect the other.

The problem is that, until now, it's been hard to prove `A` and `B`'s interdependence with anything more substantial than "gut feel." Now their interdependence can be demonstrated as a direct consequence of the Interface Principle.

Note that, unlike classes, namespaces don't need to be declared all at once, and what's "supplied together" depends on what parts of the namespace are visible:

```c++
//*** Example 6 (a)

//---file a.h---
namespace N
{
class B;
} // forward decl
namespace N
{
class A;
} // forward decl
class N::A
{
public:
	void g(B);
};

//---file b.h---
namespace N
{
class B
{
	/**/
};
}
```



Clients of `A` include `a.h`, so for them `A` and `B` are supplied together and are interdependent. Clients of `B` include `b.h`, so for them `A` and `B` are not supplied together. 

### Summary

I'd like you to take away three thoughts:

o     The Interface Principle: For a class `X`, all functions, including free functions, that both (a) "mention" `X`, and (b) are "supplied with" `X` are logically part of `X`, because they form part of the interface of `X`.

o     Therefore both member *and nonmember* functions can be logically "part of" a class. A member function is still more strongly related to a class than is a nonmember, however.

o     In the **Interface Principle**, a useful way to interpret "supplied with" is "appears in the same **header** and/or **namespace**." If the function appears in the same header as the class, it is "part of" the class in terms of dependencies. If the function appears in the same namespace as the class, it is "part of" the class in terms of object use and name lookup.



## gotw [Namespaces and the Interface Principle](http://www.gotw.ca/publications/mill08.htm)

Use **namespaces** wisely. If you put a class into a namespace, be sure to put all **helper functions** and operators into the same **namespace** too. If you don't, you may discover surprising effects in your code.

### The Interface Principle Revisited



This column goes further, and shows why you should always put helper functions, especially operators, in the same namespace as a class. That's a simple rule, but it's not entirely obvious why it's necessary, so I'll begin with a motivating example to demonstrate a hidden pitfall that comes from the way C++ namespaces and name lookup interact.

> NOTE: 这段话描述了作者写作这篇文章的意图

#### Example 1

```c++
// Example 1: Will this compile?
//
// a mainline to exercise it
#include <numeric>
#include <iostream>

// in some library header
namespace N
{
class C
{
};
}

int operator+(int i, N::C)
{
	std::cout << i << std::endl;
	return i + 1;
}

int main()
{
	N::C a[10];
	std::accumulate(a, a + 10, 0);
}

```

> NOTE: 上述代码在`g++`中无法编译通过，编译报错如下：
>
> ```c++
> In file included from /usr/include/c++/4.8.2/numeric:62:0,
>                  from test.cpp:3:
> /usr/include/c++/4.8.2/bits/stl_numeric.h: In instantiation of ‘_Tp std::accumulate(_InputIterator, _InputIterator, _Tp) [with _InputIterator = N::C*; _Tp = int]’:
> test.cpp:20:29:   required from here
> /usr/include/c++/4.8.2/bits/stl_numeric.h:127:18: 错误：no match for ‘operator+’ (operand types are ‘int’ and ‘N::C’)
>   __init = __init + *__first;
> 
> ```
>
> 正确的写法如下：
>
> ```c++
> // Example 1: Will this compile?
> //
> // a mainline to exercise it
> #include <numeric>
> #include <iostream>
> 
> // in some library header
> namespace N
> {
> class C
> {
> };
> int operator+(int i, N::C)
> {
> 	std::cout << i << std::endl;
> 	return i + 1;
> }
> 
> }
> 
> int main()
> {
> 	N::C a[10];
> 	std::accumulate(a, a + 10, 0);
> }
> 
> ```
>
> 





### Digression: Recap of a Familiar Inheritance Issue

> NOTE: 题外话:重述一个熟悉的继承问题

#### Example 2a

```c++
#include <string>

// Example 2a: Hiding a name
//             from a base class
//
struct B
{
	int f(int);
	int f(double);
	int g(int);
};

struct D: public B
{
private:
	int g(std::string, bool);
};

int main()
{

	D d;
	int i;
	d.f(i);    // ok, means B::f(int)
	d.g(i);    // error: g takes 2 args
}
```

> NOTE: 编译报错如下:
>
> ```c++
> test.cpp: 在函数‘int main()’中:
> test.cpp:25:7: 错误：对‘D::g(int&)’的调用没有匹配的函数
>   d.g(i);    // error: g takes 2 args
>        ^
> test.cpp:25:7: 附注：备选是：
> test.cpp:16:6: 附注：int D::g(std::string, bool)
>   int g(std::string, bool);
>       ^
> test.cpp:16:6: 附注： 备选需要 2 实参，但提供了 1 个
> 
> ```



Most of us should be used to seeing this kind of name hiding, although the fact that the last line won't compile surprises most new C++ programmers. In short, when we declare a function named `g` in the derived class `D`, it automatically hides all functions with the same name in all direct and indirect **base classes**. It doesn't matter a whit that `D::g` "obviously" can't be the function that the programmer meant to call (not only does `D::g` have the wrong signature, but it's private and therefore inaccessible to boot), because `B::g` is hidden and can't be considered by name lookup.

> NOTE: 在derived class中，不管`g`的access为何，它都会hide base class中的所有的`g`。

To see what's really going on, let's look in a little more detail at what the compiler does when it encounters the function call `d.g(i)`. First, it looks in the immediate scope, in this case the scope of class `D`, and makes a list of all functions it can find that are named `g` (regardless of whether they're accessible or even take the right number of parameters). *Only if it doesn* does it then continue "outward" into the next enclosing scope and repeat--in this case, the scope of the base class `B`--until eventually it either runs out of scopes without having found a function with the right name or else finds a scope that contains at least one candidate function. If a scope is found that has one or more candidate functions, the compiler then stops searching and works with the candidates that it's found, performing **overload resolution** and applying **access rules**.

There are very good reasons why the language must work this way.**[[3\]](http://www.gotw.ca/publications/mill08.htm#3)** To take the extreme case, it makes intuitive sense that a member function that's a near-exact match ought to be preferred over a global function that would have been a perfect match had we considered the parameter types only.

Of course, there are the two usual ways around the **name-hiding problem** in Example 2a. First, the calling code can simply say which one it wants, and force the compiler to look in the right scope:

#### Example 2b

```c++
#include <string>

// Example 2b: Asking for a name
//             from a base class
struct B
{
	int f(int)
	{
	}
	int f(double)
	{
	}
	int g(int)
	{
	}
};

struct D: public B
{
private:
	int g(std::string, bool)
	{
	}
};

int main()
{

	D d;
	int i;
	d.f(i);    // ok, means B::f(int)
	d.B::g(i);    // error: g takes 2 args
}

```

Second, and usually more appropriate, the designer of class `D` can make `B::g` visible with a using-declaration. This allows the compiler to consider `B::g` in the same scope as `D::g` for the purposes of name lookup and subsequent overload resolution:

```c++
#include <string>

// Example 2c: Un-hiding a name
//             from a base class
//
struct B
{
	int f(int)
	{
	}
	int f(double)
	{
	}
	int g(int)
	{
	}
};

struct D: public B
{
	using B::g;
private:
	int g(std::string, bool)
	{
	}
};

int main()
{

	D d;
	int i;
	d.f(i);    // ok, means B::f(int)
	d.g(i);    // error: g takes 2 args
}
// g++ test.cpp
```

Either of these gets around the hiding problem in the original Example 2a code.

### Back On Topic: Name Hiding in Nested Namespaces

Well, at first glance, it sure looks legal. But will it compile? The answer is probably surprising: Maybe it will compile, or maybe not. It depends entirely on your implementation, and I know of standard-conforming implementations that will compile this program correctly and equally standard-conforming implementations that won't. Gather 'round, and I'll show you why.

The key to understanding the answer is understanding what the compiler has to do inside [`std::accumulate`](https://en.cppreference.com/w/cpp/algorithm/accumulate) . The `std::accumulate` template looks something like this:

```c++
namespace std {
  template<class Iter, class T>
  inline T accumulate( Iter first,
                       Iter last,
                       T    value ) {
    while( first != last ) {
      value = value + *first;   // 1
      ++first;
    }
    return value;
  }
}
```

The code in Example 1 actually calls `std::accumulate<N::C*,int>`. In line 1 above, how should the compiler interpret the expression `value + *first`? Well, it's got to look for an `operator+` that takes an `int` and an `N::C` (or parameters that can be converted to int and `N::C`). Hey, it just so happens that we have just such an `operator+(int,N::C)` at global scope! Look, there it is! Cool. So everything must be fine, right?

The problem is that the compiler may or may not be able to see the `operator+(int,N::C)` at global scope, depending on what other functions have already been seen to be declared in namespace `std` at the point where `std::accumulate<N::C*,int>` is instantiated.

To see why, consider that the same name hiding the we observed with derived classes happens with any nested scopes, including namespaces, and consider *where* the compiler starts looking for a suitable `operator+`. (Now I'm going to reuse my explanation from the previous section, only with a few names substituted:) First, it looks in the immediate scope, in this case the scope of namespace `std`, and makes a list of all functions it can find that are named `operator+` (regardless of whether they're accessible or even take the right number of parameters). *Only if it doesn* does it then continue "outward" into the next **enclosing scope** and repeat--in this case, the scope of the next enclosing namespace outside `std`, which happens to be the global scope--until eventually it either runs out of scopes without having found a function with the right name or else finds a scope that contains at least one candidate function. If a scope is found that has one or more candidate functions, the compiler then stops searching and works with the candidates that it's found, performing **overload resolution** and applying **access rules**.

In short, whether Example 1 will compile depends entirely on whether this implementation's version of the standard header `numeric`: 

a) declares an `operator+` (any `operator+`, suitable or not, accessible or not); or 

b) includes any other standard header that does so. 

Unlike Standard C, Standard C++ does not specify which standard headers may or may not include each other, so when you include `numeric` you may or may not get header iterator too, for example, which does define several `operator+` functions. I know of C++ products that won't compile Example 1, others that will compile Example 1 but balk once you add the line `#include <vector>`, and so on.

### Some Fun With Compilers

It's bad enough that the compiler can't find the right function if there happens to be another `operator+` in the way, but typically the `operator+` that does get encountered in a standard header is a template, and compilers generate notoriously（众所周知地） difficult-to-read error messages when templates are involved. For example, one popular implementation reports the following errors when compiling Example 1 (note that in this implementation the header `numeric` does in fact include the header `iterator`):

```c++
error C2784: 'class std::reverse_iterator<`template-parameter-1', `template-parameter-2', `template-parameter-3', `template-parameter-4', `template-parameter-5'> __cdecl std::operator +(template-parameter-5, const class std::reverse_iterator< `template-parameter-1', `template-parameter-2', `template-parameter-3', `template-parameter-4', `template-parameter-5'>&)' : could not deduce template argument for 'template-parameter-5' from 'int'

error C2677: binary '+' : no global operator defined which takes type 'class N::C' (or there is no acceptable conversion)
```

How is a mortal programmer ever to decipher what's going wrong here? And, once he does, how loudly is he likely to curse the author of class `N::C`? Best to avoid the problem completely, as we shall now see.

### The Solution

When we encountered this problem in the familiar guise of base/derived name hiding, we had two possible solutions: either have the calling code explicitly say which function it wants (Example 2b), or write a using-declaration to make the desired function visible in the right scope (Example 2c). Neither solution works in this case; the first is possible**[[4\]](http://www.gotw.ca/publications/mill08.htm#4)** but places an unacceptable burden on the programmer, while the second is impossible.

The real solution is to put our `operator+` where it has always truly belonged and should have been put in the first place: in namespace `N`.

#### Example 1b

```c++
// Example 1b: Solution
//
// a mainline to exercise it
#include <numeric>
#include <iostream>

// in some library header
namespace N
{
class C
{
};
int operator+(int i, N::C)
{
	std::cout << i << std::endl;
	return i + 1;
}

}

int main()
{
	N::C a[10];
	std::accumulate(a, a + 10, 0);
}
// g++ test.c
```

This code is portable and will compile on all conforming compilers, regardless of what happens to be already defined in `std` or any other namespace. Now that the `operator+` is in the same namespace as the second parameter, when the compiler tries to resolve the "`+`" call inside `std::accumulate` it is able to see the right `operator+` because of **Koenig lookup**. Recall that Koenig lookup says that, in addition to looking in all the usual scopes, the compiler shall also look in the scopes of the function's parameter types to see if it can find a match. `N::C` is in namespace `N`, so the compiler looks in namespace `N`, and happily finds exactly what it needs no matter how many other `operator+`'s happen to be lying around and cluttering up namespace `std`.

### Conclusion

The problem arose because Example 1 did not follow the Interface Principle:

> The Interface Principle
>
> For a class X, all functions, including free functions, that both
>   (a) "mention" X, and
>   (b) are "supplied with" X
> are logically part of X, because they form part of the interface of X.
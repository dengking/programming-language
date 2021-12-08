# stackoverflow [What are copy elision and return value optimization?](https://stackoverflow.com/questions/12953127/what-are-copy-elision-and-return-value-optimization)

What is copy elision? What is (named) return value optimization? What do they imply?

In what situations can they occur? What are limitations?

- If you were referenced to this question, you're probably looking for [**the introduction**](https://stackoverflow.com/a/12953129/673730).
- For a technical overview, see [**the standard reference**](https://stackoverflow.com/a/12953145/673730).
- See [**common cases** here](https://stackoverflow.com/a/12953150/673730).

## [A](https://stackoverflow.com/a/12953129)

### Introduction

For a technical overview - [skip to this answer](https://stackoverflow.com/a/12953145/673730).

For common cases where copy elision occurs - [skip to this answer](https://stackoverflow.com/a/12953150/673730).

Copy elision is an optimization implemented by most compilers to prevent extra (potentially expensive) copies in certain situations. It makes returning by value or pass-by-value feasible in practice (restrictions apply).

> NOTE: 这是C++对value semantic的增强

It's the only form of optimization that elides(不考虑) (ha!) the as-if rule - **copy elision can be applied even if copying/moving the object has side-effects**.

> NOTE: copy elision是打破"as-if rule"的一种optimization，在下面的文章中，也谈到了这个问题:
>
> 1、cppreference [Copy elision](https://en.cppreference.com/w/cpp/language/copy_elision)

The following example taken from [Wikipedia](http://en.wikipedia.org/wiki/Return_value_optimization):

```cpp
struct C {
  C() {}
  C(const C&) { std::cout << "A copy was made.\n"; }
};
 
C f() {
  return C();
}
 
int main() {
  std::cout << "Hello World!\n";
  C obj = f();
}
```

Depending on the compiler & settings, the following outputs **are all valid**:

> Hello World!
> A copy was made.
> A copy was made.

------

> Hello World!
> A copy was made.

------

> Hello World!

This also means fewer objects can be created, so you also can't rely on a specific number of destructors being called. You shouldn't have critical logic inside copy/move-constructors or destructors, as you can't rely on them being called.

> NOTE: cross-plateform

If a call to a copy or move constructor is elided, that constructor must still exist and must be accessible. This ensures that copy elision does not allow copying objects which are not normally copyable, e.g. because they have a private or deleted copy/move constructor.

> NOTE: 所以这就涉及到一个问题：如何来确定是否会发生Copy Elision，在这篇文章中也给出了非常详细的介绍：

**C++17**: As of C++17, Copy Elision is guaranteed when an object is returned directly:

```cpp
struct C {
  C() {}
  C(const C&) { std::cout << "A copy was made.\n"; }
};

C f() {
  return C(); //Definitely performs copy elision
}
C g() {
    C c;
    return c; //Maybe performs copy elision
}

int main() {
  std::cout << "Hello World!\n";
  C obj = f(); //Copy constructor isn't called
}
```



## [A](https://stackoverflow.com/a/12953145)

### Standard reference

**Copy elision** is defined in the standard in:

#### 12.8 Copying and moving class objects [class.copy]

as

> 31) When certain criteria are met, an implementation is allowed to omit the **copy/move construction** of a class object, even if the copy/move constructor and/or destructor for the object have side effects. In such cases, the implementation treats the source and target of the omitted copy/move operation as simply two different ways of referring to the same object, and the destruction of that object occurs at the later of the times when the two objects would have been destroyed without the optimization.This elision of copy/move operations, called *copy elision*, is permitted in the following circumstances (which may be combined to eliminate multiple copies):
>
> — in a return statement in a function with a class return type, when the expression is the name of a **non-volatile** **automatic object** (other than a function or catch-clause parameter) with the same **cv-unqualified** type as the function return type, the copy/move operation can be omitted by constructing the **automatic object** directly into the function’s return value
>
> — in a throw-expression, when the **operand** is the name of a non-volatile automatic object (other than a function or catch-clause parameter) whose scope does not extend beyond the end of the innermost enclosing try-block (if there is one), the copy/move operation from the operand to the exception object (15.1) can be omitted by constructing the automatic object directly into the **exception object**
>
> — when a **temporary class object** that has not been bound to a reference (12.2) would be copied/moved to a class object with the same **cv-unqualified type**, the copy/move operation can be omitted by constructing the temporary object directly into the target of the omitted copy/move
>
> — when the exception-declaration of an exception handler (Clause 15) declares an object of the same type (except for cv-qualification) as the exception object (15.1), the copy/move operation can be omitted by treating the exception-declaration as an alias for the exception object if the meaning of the program will be unchanged except for the execution of constructors and destructors for the object declared by the exception-declaration.
>
> 123) Because only one object is destroyed instead of two, and one copy/move constructor is not executed, there is still one object destroyed for each one constructed.

The example given is:

```cpp
#include <iostream>
#include <utility>
class Thing
{
public:
	Thing()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	~Thing()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	Thing(const Thing&)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};
Thing f()
{
	Thing t;
	return t;
}

int main()
{

	Thing t2 = f();
}
// g++ -std=c++11  -Wall -pedantic -pthread main.cpp && ./a.out

```

> NOTE: 输出如下:
>
> ```
> Thing::Thing()
> Thing::~Thing()
> 
> ```
>
> 

and explained:

> Here the criteria for elision can be combined to eliminate two calls to the copy constructor of class `Thing`: the copying of the local automatic object `t` into the temporary object for the return value of function `f()` and the copying of that temporary object into object `t2`. Effectively, the construction of the local object `t` can be viewed as directly initializing the global object `t2`, and that object’s destruction will occur at program exit. Adding a move constructor to Thing has the same effect, but it is the move construction from the temporary object to `t2` that is elided.

> NOTE: 
>
> 上述例子，所解释的其实就是: "Multiple copy elisions may be chained to eliminate multiple copies."

## [A](https://stackoverflow.com/a/12953150)

### Common forms of copy elision

(Named) Return value optimization is a common form of copy elision. It refers to the situation where an object returned by value from a method has its copy elided. The example set forth in the standard illustrates **named return value optimization**, since the object is named.

```cpp
#include <iostream>
#include <utility>
class Thing
{
public:
	Thing()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	~Thing()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	Thing(const Thing&)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};
Thing f()
{
	Thing t;
	return t;
}

int main()
{

	Thing t2 = f();
}
// g++ -std=c++11  -Wall -pedantic -pthread main.cpp && ./a.out

```

Regular **return value optimization** occurs when a temporary is returned:

```cpp
#include <iostream>
#include <utility>
class Thing
{
public:
	Thing()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	~Thing()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	Thing(const Thing&)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};
Thing f()
{
	return Thing();
}

int main()
{

	Thing t2 = f();
}
// g++ -std=c++11  -Wall -pedantic -pthread main.cpp && ./a.out

```

Other common places where copy elision takes place is when a **temporary is passed by value**:

```cpp
class Thing {
public:
  Thing();
  ~Thing();
  Thing(const Thing&);
};
void foo(Thing t);

foo(Thing());
```

or when an **exception is thrown and caught by value**:

```cpp
struct Thing{
  Thing();
  Thing(const Thing&);
};

void foo() {
  Thing c;
  throw c;
}

int main() {
  try {
    foo();
  }
  catch(Thing c) {  
  }             
}
```

[Common limitations of copy elision are:](http://msdn.microsoft.com/en-us/library/ms364057%28v=vs.80%29.aspx#nrvo_cpp05_topic3)

- multiple return points
- conditional initialization

Most commercial-grade compilers support copy elision & (N)RVO (depending on optimization settings).


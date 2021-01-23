# Copy elision

Copy elision是对compiler optimization principle的遵循，参见 `C-and-C++\From-source-code-to-exec\Compile\Optimization` 章节。



## by value and by reference

显然by value涉及到copy，并且构造的对象在函数调用结束后就会被release掉；但是by reference则不存在。

> This is not just a matter of convenience but in fact an optimization. If the parameter (s) binds to an **lvalue** (another non-const object), a copy of the object is made automatically while creating the parameter (s). However, when s binds to an rvalue (temporary object, literal), the copy is typically elided, which saves a call to a copy constructor and a destructor. In the earlier version of the assignment operator where the parameter is accepted as **const reference**, **copy elision** does not happen when the **reference** binds to an **rvalue**. This results in an additional object being created and destroyed.

## wikipedia [Copy elision](https://en.wikipedia.org/wiki/Copy_elision#Return_value_optimization)

In [C++](https://en.wikipedia.org/wiki/C%2B%2B) [computer programming](https://en.wikipedia.org/wiki/Computer_programming), **copy elision** refers to a [compiler optimization](https://en.wikipedia.org/wiki/Compiler_optimization) technique that eliminates unnecessary [copying of objects](https://en.wikipedia.org/wiki/Object_copy). The `C++` language standard generally allows implementations to perform any optimization, provided the resulting program(优化后的程序)'s observable behavior is the same *as if*, i.e. pretending, the program were executed exactly as mandated by the standard.

> NOTE: (C ++语言标准通常允许实现执行任何优化，前提是结果程序的可观察行为与假定完全按照标准强制执行的程序相同)
>
> 思考：为什么要消除copy？
>
> copy elision其实省略的是copy constructor

The standard also describes a few situations where copying can be eliminated even if this would alter the program's behavior, the most common being the [return value optimization](https://en.wikipedia.org/wiki/Return_value_optimization). Another widely implemented optimization, described in the [C++ standard](https://en.wikipedia.org/wiki/ISO/IEC_14882), is when a [temporary object](https://en.wikipedia.org/wiki/Temporary_variable) of [class type](https://en.wikipedia.org/wiki/Class_(computing)) is copied to an object of the same type.[[1\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-C++03_12.8/15-1) As a result, *copy-initialization* is usually equivalent to *direct-initialization* in terms of performance, but not in semantics; *copy-initialization* still requires an [accessible](https://en.wikipedia.org/wiki/Member_accessibility) [copy constructor](https://en.wikipedia.org/wiki/Copy_constructor).[[2\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-moreexcept-2) The optimization can not be applied to a temporary object that has been bound to a reference.

### Example

```cpp
#include <iostream>

int n = 0;
struct C {
  explicit C(int) {}
  C(const C&) { ++n; } // the copy constructor has a visible side effect
};                     // it modifies an object with static storage duration

int main() {
  C c1(42); // direct-initialization, calls C::C(42)
  C c2 = C(42); // copy-initialization, calls C::C( C(42) )
                // temporary object is copied to an object
  std::cout << n << std::endl; // prints 0 if the copy was elided, 1 otherwise
  return 0;
}
```

### Return value optimization

In the context of the [C++](https://en.wikipedia.org/wiki/C%2B%2B) [programming language](https://en.wikipedia.org/wiki/Programming_language), **return value optimization** (**RVO**) is a [compiler optimization](https://en.wikipedia.org/wiki/Compiler_optimization) that involves eliminating the [temporary object](https://en.wikipedia.org/wiki/Temporary_variable) created to hold a [function](https://en.wikipedia.org/wiki/Subroutine)'s return value.[[6\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-moreeffcpp-6) **RVO** is particularly notable for being allowed to change the observable behaviour of the resulting [program](https://en.wikipedia.org/wiki/Computer_program) by the [C++ standard](https://en.wikipedia.org/wiki/ISO/IEC_14882).[[7\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-andrei-7)

#### Summary

In general, the C++ standard allows a [compiler](https://en.wikipedia.org/wiki/Compiler) to perform any optimization, provided the resulting [executable](https://en.wikipedia.org/wiki/Executable) exhibits the same observable behaviour *as if* (i.e. pretending) all the requirements of the standard have been fulfilled. This is commonly referred to as the "[as-if rule](https://en.wikipedia.org/wiki/As-if_rule)".[[8\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-C++03_1.9/1-8) The term *return value optimization* refers to a special clause in the [C++ standard](https://en.wikipedia.org/wiki/ISO/IEC_14882) that goes even further than the "as-if" rule: an implementation may omit a **copy operation** resulting from a [return statement](https://en.wikipedia.org/wiki/Return_statement), even if the [copy constructor](https://en.wikipedia.org/wiki/Copy_constructor) has [side effects](https://en.wikipedia.org/wiki/Side_effect_(computer_science)).[[1\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-C++03_12.8/15-1)

The following example demonstrates a scenario where the implementation may eliminate one or both of the copies being made, even if the **copy constructor** has a visible side effect (printing text).[[1\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-C++03_12.8/15-1) The first copy that may be eliminated is the one where a nameless temporary `C` could be copied into the function `f`'s [return value](https://en.wikipedia.org/wiki/Return_value). The second copy that may be eliminated is the copy of the temporary object returned by `f` to `obj`(下面这个程序涉及到的两处copy constructor).

```cpp
#include <iostream>

struct C {
  C() {}
  C(const C&) { std::cout << "A copy was made.\n"; }
};

C f() {
  return C();
}//此函数设计copy constructor，因为该函数的是by-value返回的，所以函数内构造的temporary object需要被copy出来

int main() {
  std::cout << "Hello World!\n";
  C obj = f();
  return 0;
}
```

Depending upon the [compiler](https://en.wikipedia.org/wiki/Compiler), and that compiler's settings, the resulting [program](https://en.wikipedia.org/wiki/Computer_program) may display any of the following outputs:

```cpp
Hello World!
A copy was made.
A copy was made.
```
```cpp
Hello World!
A copy was made.
```
```cpp
Hello World!
```

#### Background

Returning an object of [built-in type](https://en.wikipedia.org/wiki/Built-in_type) from a [function](https://en.wikipedia.org/wiki/Subroutine) usually carries little to no overhead(开销), since the object typically fits in a [CPU register](https://en.wikipedia.org/wiki/Processor_register). Returning a larger object of [class type](https://en.wikipedia.org/wiki/Class_(computing)) may require more expensive copying from one memory location to another. To avoid this, an implementation may create a **hidden object** in the caller's [stack frame](https://en.wikipedia.org/wiki/Stack_frame), and pass the address of this object to the function. The function's return value is then copied into the **hidden object**.[[9\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-efficient-9) Thus, code such as this:

```cpp
struct Data { 
  char bytes[16]; 
};

Data f() {
  Data result = {};
  // generate result
  return result;
}

int main() {
  Data d = f();
}
```

May generate code equivalent to this:

```cpp
struct Data { 
  char bytes[16]; 
};

Data * f(Data * _hiddenAddress) {
  Data result = {};
  // copy result into hidden object
  *_hiddenAddress = result;
  return _hiddenAddress;
}

int main() {
  Data _hidden; // create hidden object
  Data d = *f(&_hidden); // copy the result into d
}
```

which causes the `Data` object to be copied twice.

In the early stages of the evolution of [C++](https://en.wikipedia.org/wiki/C%2B%2B), the language's inability to efficiently return an object of [class type](https://en.wikipedia.org/wiki/Class_(computing)) from a function was considered a weakness.[[10\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-lippman-10) Around 1991, [Walter Bright](https://en.wikipedia.org/wiki/Walter_Bright) implemented a technique to minimize copying, effectively replacing the **hidden object** and the **named object** inside the function with the object used for holding the result(使用object used for holding the result来代替hidden object和named object):[[11\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-d20-11)

```cpp
struct Data { 
  char bytes[16]; 
};

void f(Data *p) {
  // generate result directly in *p
}

int main() {
  Data d;
  f(&d);
}
```

Bright implemented this optimization in his [Zortech C++](https://en.wikipedia.org/wiki/Digital_Mars) compiler.[[10\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-lippman-10) This particular technique was later coined "**Named return value optimization**", referring to the fact that the copying of a **named object** is elided.[[11\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-d20-11)

#### Compiler support

**Return value optimization** is supported on most compilers.[[6\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-moreeffcpp-6)[[12\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-vc8-12)[[13\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-gcc-13) There may be, however, circumstances where the compiler is unable to perform the optimization. One common case is when a function may return different named objects depending on the path of execution:[[9\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-efficient-9)[[12\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-vc8-12)[[14\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-n1377-14)

```cpp
#include <string>
std::string f(bool cond = false) {
  std::string first("first");
  std::string second("second");
  // the function may return one of two named objects
  // depending on its argument. RVO might not be applied
  return cond ? first : second;
}

int main() {
  std::string result = f();
}
```


## stackoverflow [What are copy elision and return value optimization?](https://stackoverflow.com/questions/12953127/what-are-copy-elision-and-return-value-optimization)
### Introduction
This also means fewer objects can be created, so you also can't rely on a specific number of destructors being called. You shouldn't have critical logic inside copy/move-constructors or destructors, as you can't rely on them being called.
    
If a call to a copy or move constructor is elided, that constructor must still exist and must be accessible. This ensures that copy elision does not allow copying objects which are not normally copyable, e.g. because they have a private or deleted copy/move constructor.

所以这就涉及到一个问题：如何来确定是否会发生Copy Elision，在这篇文章中也给出了非常详细的介绍：

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
class Thing {
public:
  Thing();
  ~Thing();
  Thing(const Thing&);
};
Thing f() {
  Thing t;
  return t;
}
Thing t2 = f();
```

and explained:

> Here the criteria for elision can be combined to eliminate two calls to the copy constructor of class `Thing`: the copying of the local automatic object `t` into the temporary object for the return value of function `f()` and the copying of that temporary object into object `t2`. Effectively, the construction of the local object `t` can be viewed as directly initializing the global object `t2`, and that object’s destruction will occur at program exit. Adding a move constructor to Thing has the same effect, but it is the move construction from the temporary object to `t2` that is elided.

### Common forms of copy elision

(Named) Return value optimization is a common form of copy elision. It refers to the situation where an object returned by value from a method has its copy elided. The example set forth in the standard illustrates **named return value optimization**, since the object is named.

```cpp
class Thing {
public:
  Thing();
  ~Thing();
  Thing(const Thing&);
};
Thing f() {
  Thing t;
  return t;
}
Thing t2 = f();
```

Regular **return value optimization** occurs when a temporary is returned:

```cpp
class Thing {
public:
  Thing();
  ~Thing();
  Thing(const Thing&);
};
Thing f() {
  return Thing();
}
Thing t2 = f();
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


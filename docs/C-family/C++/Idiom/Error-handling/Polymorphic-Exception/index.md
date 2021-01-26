# Polymorphic Exception



## More C++ Idioms/[Polymorphic Exception](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Polymorphic_Exception)

> NOTE: exception class之间存在着inheritance关系，那如何polymorphism throw exception并且polymorphism catch exception呢？这是这个idiom所解决的问题；`throw *this`即由exception object来throw，类似于visitor pattern。

### Motivation

[Dependency Inversion Principle (DIP)](https://en.wikipedia.org/wiki/Dependency_inversion_principle), a popular object-oriented software design guideline states that higher level modules should not depend directly on lower level modules. Instead, both should depend on common abstractions (captured in the form of well-defined interfaces). For example, an object of type *Person* (say John) should not create and use an object of type *HondaCivic* but instead John should simply commit to a *Car* interface, which is an abstract base class of *HondaCivic*. This allows John to upgrade to a *Corvette* easily in future without any changes to the class *Person*. John can be "configured" with a concrete instance of a car (any car) using [dependency injection](https://en.wikipedia.org/wiki/Dependency_injection) technique. Use of DIP leads to flexible and extensible modules that are easy to unit test. Unit testing is simplified by DIP because real objects can be easily replaced with mock objects using dependency injection.

> NOTE: 上面这一段关于DIP、DI的描述是非常好的

However, there are several occasions when DIP is violated: 

(1) while using the Singleton pattern 

(2) while throwing exceptions! 

Singleton pattern breaks DIP because it forces the use of the concrete class name while accessing the static *instance()* function. A singleton should be passed as a parameter while calling a function or a constructor. A similar situation arises while dealing with exceptions in C++. The *throw* clause in C++ requires a concrete type name (class) to raise an exception. For example,

```c++
throw MyConcreteException("Big Bang!");
```

Any module that throws exceptions like this immediately results into a violation of DIP. Naturally, it is harder to unit test such a module because real exception objects cannot easily be replaced with mock exception objects. A solution like the one below fails miserably as *throw* in C++ uses static typing and knows nothing about polymorphism.

```c++
#include<iostream>
struct ExceptionBase { };
struct ExceptionDerived : ExceptionBase { };
 
void foo(ExceptionBase& e)
{
   throw e; // Uses static type of e while rasing an exception.
}
int main (void)
{
  ExceptionDerived e;
  try {
    foo(e);
  }
  catch (ExceptionDerived& e) {
    // Exception raised in foo does not match this catch.
      std::cout<<"ExceptionDerived"<<std::endl;      
  }
  catch (...) {
    // Exception raised in foo is caught here.
      std::cout<<"ExceptionBase"<<std::endl;
  }
}
```

> NOTE: `g++ test.cpp`
>
> 上述程序输出如下：
>
> ```
> ExceptionBase
> ```
>
> 显然，没有发生polymorphism。c++中的polymorphism仅仅发生于virtual method通过reference或者pointer来被调用。

Polymorphic exception idiom addresses the issue.

### Solution and Sample Code

**Polymorphic exception idiom** simply delegates the job of raising the exception back to the derived class using a virtual function *raise()*

```c++
struct ExceptionBase 
{ 
  virtual void raise() { throw *this; }
  virtual ~ExceptionBase() {} 
};
struct ExceptionDerived : ExceptionBase 
{ 
  virtual void raise() { throw *this; }
};
 
void foo(ExceptionBase& e)
{
   e.raise(); // Uses dynamic type of e while raising an exception.
}
int main (void)
{
  ExceptionDerived e;
  try {
    foo(e);
  }
  catch (ExceptionDerived& e) {
    // Exception raised in foo now matches this catch.
  }
  catch (...) {
    // not here anymore!
  }
}
```

The *throw* statement has been moved into virtual functions. The *raise* function invoked in function *foo* is polymorphic and selects the implementation in either *ExceptionBase* or *ExceptionDerived* class depending upon what is passed as a parameter (dependency injection). Type of `*this` is obviously known at compile-time, which results into raising a polymorphic exception. The structure of this idiom is very similar to that of [Virtual Constructor](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Virtual_Constructor) idiom.

#### Propagating a polymorphic exception

Quite often an exception is handled in multiple catch statements to treat it differently in different layers of the program/library. In such cases, the earlier catch blocks need to rethrow the exception so that the outer catch blocks, if any, may take the necessary actions. When a polymorphic exception is involved, inner catch block(s) may modify the exception object before passing it on to the catch blocks up in the stack. In such cases, care must be taken to ensure that the original exception object is propagated. Consider a seemingly innocuous looking program below, which fails to do that.

```c++
try {
    foo(e); // throws an instance of ExceptionDerived as before.
  }
  catch (ExceptionBase& e) // Note the base class. Exception is caught polymorphically.
  {
    // Handle the exception. May modify the original exception object.
    throw e; // Warning! Object slicing is underway.
  }
```

The *throw e* statement does not throw the original exception object. Instead, it throws a sliced copy (only `ExceptionBase` part) of the original object because it considers the static type of the expression in front of it. Silently, the derived exception object was not only lost but also translated into the base type exception object. The catch blocks up in the stack do not have access to the same information this catch had. There are two ways to address the problem.

- Simply use `throw`; (without any expression following it). It will rethrow the original exception object.
- Use polymorphic exception idiom again. It will throw a copy of the original exception object because the *raise()* virtual function uses `throw *this`.

Use of `throw`; should be strongly preferred in practice, because depending on the implementation, it may preserve the original throw-location in case the exception goes unhandled and the program dumps core, thus easing post mortem（事后检验） analysis of the problem.

```c++
try {
    foo(e); // throws an instance of ExceptionDerived as before.
  }
  catch (ExceptionBase& e) // Note the base class. Exception is caught polymorphically.
  {
    // Handle the exception. May modify the original exception object.
    // Use only one of the following two.
    throw;      // Option 1:  Original derived exception is thrown.
    e.raise();  // Option 2:  A copy of the original derived exception object is thrown.
  }
```

### References

- [How do I throw polymorphically? (parashift)](http://www.parashift.com/c++-faq-lite/exceptions.html#faq-17.10)
- [How do I throw polymorphically? (NCTU)](http://www.cis.nctu.edu.tw/chinese/doc/research/c++/C++FAQ-English/exceptions.html#faq-17.10)
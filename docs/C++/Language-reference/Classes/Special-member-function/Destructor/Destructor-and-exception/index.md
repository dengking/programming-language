# Destructor and exception

## cppreference [Destructors](https://en.cppreference.com/w/cpp/language/destructor) # [Exceptions](https://en.cppreference.com/w/cpp/language/destructor#Exceptions)

As any other function, a destructor may terminate by throwing an [exception](https://en.cppreference.com/w/cpp/language/exceptions) (this usually requires it to be explicitly declared `noexcept(false))` (since C++11), however if this destructor happens to be called during [stack unwinding](https://en.cppreference.com/w/cpp/language/throw#Stack_unwinding), [std::terminate](https://en.cppreference.com/w/cpp/error/terminate) is called instead.

> NOTE: 
>
> 1、在cppreference中，[stack unwinding](https://en.cppreference.com/w/cpp/language/throw#Stack_unwinding) 特指由于exception而导致的提前退出

Although [std::uncaught_exception](https://en.cppreference.com/w/cpp/error/uncaught_exception) may sometimes be used to detect stack unwinding in progress, it is generally considered bad practice to allow any destructor to terminate by throwing an exception. This functionality is nevertheless(然而) used by some libraries, such as [SOCI](https://github.com/SOCI/soci) and [Galera 3](http://galeracluster.com/downloads/), which rely on the ability of the destructors of nameless temporaries to throw exceptions at the end of the full expression that constructs the temporary.

> NOTE: 
>
> 1、" [std::uncaught_exception](https://en.cppreference.com/w/cpp/error/uncaught_exception) may sometimes be used to detect stack unwinding in progress"，参见 `Scope-Guard` 章节
>
> 2、 "This functionality is nevertheless(然而) used by some libraries, such as [SOCI](https://github.com/SOCI/soci) and [Galera 3](http://galeracluster.com/downloads/), which rely on the ability of the destructors of nameless temporaries to throw exceptions at the end of the full expression that constructs the temporary."
>
> 这段话的意思是: 一些libraries，比如  [SOCI](https://github.com/SOCI/soci) and [Galera 3](http://galeracluster.com/downloads/)，它们通过让destructor 抛出exception来实现特殊的概念

## ibm [Stack unwinding (C++ only)](https://www.ibm.com/support/knowledgecenter/SSLTBW_2.4.0/com.ibm.zos.v2r4.cbclx01/cplr155.htm) # Destructor and exception

If during **stack unwinding** a **destructor** throws an **exception** and that exception is not handled, the `terminate()`function is called. The following example demonstrates this:

```cpp
#include <iostream>
using namespace std;

struct E {
  const char* message;
  E(const char* arg) : message(arg) { }
};

void my_terminate() {
  cout << "Call to my_terminate" << endl;
};

struct A {
  A() { cout << "In constructor of A" << endl; }
  ~A() {
    cout << "In destructor of A" << endl;
    throw E("Exception thrown in ~A()");
  }
};

struct B {
  B() { cout << "In constructor of B" << endl; }
  ~B() { cout << "In destructor of B" << endl; }
};

int main() {
  set_terminate(my_terminate);

  try {
    cout << "In try block" << endl;
    A a;
    B b;
    throw("Exception thrown in try block of main()");
  }
  catch (const char* e) {
    cout << "Exception: " << e << endl;
  }
  catch (...) {
    cout << "Some exception caught in main()" << endl;
  }

  cout << "Resume execution of main()" << endl;
}
```

The output of this example:

```cpp
In try block
In constructor of A
In constructor of B
In destructor of B
In destructor of A
Call to my_terminate
```

In the try block, two automatic objects are created:`a` and `b`. The **try block** throws an **exception** of type `const char*`. The handler catch (`const char* e`)  catches this **exception**. The C++ run time unwinds the stack, calling the destructors for `a ` and `b` in reverse order of their construction. The destructor for `a` throws an exception. Since there is no handler in the program that can handle this exception, the C++ run time calls `terminate()`. (The function `terminate()` calls the function specified as the argument to `set_terminate()`. In this example, `terminate()` has been specified to call `my_terminate()`.)

## stackoverflow [What is stack unwinding?](https://stackoverflow.com/questions/2331316/what-is-stack-unwinding) # [A](https://stackoverflow.com/a/2332865)

program consistency: the C++ specifications state that you should never throw an exception before any existing exception has been handled. This means that **the stack unwinding process should never throw an exception** (either use only code guaranteed not to throw in destructors, or surround everything in destructors with `try {` and `} catch(...) {}`).

> NOTE: 它说明了坚持"A destructor may not fail"的原因: program consistency。



## CppCoreGuidelines [C.36: A destructor may not fail](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c36-a-destructor-must-not-fail)

### Reason

In general we do not know how to write error-free code if a destructor should fail. The standard library requires that all classes it deals with have destructors that do not exit by throwing.

> NOTE: 其实原因在前面已经总结了

### Example



```c++
class X {
public:
    ~X() noexcept;
    // ...
};

X::~X() noexcept
{
    // ...
    if (cannot_release_a_resource) terminate();
    // ...
}
```

> NOTE: 上述程序其实是一个非常好的示例

### Note

Many have tried to devise a **fool-proof**(万无一失的) scheme for dealing with failure in destructors. None have succeeded to come up with a general scheme. This can be a real practical problem: For example, what about a socket that won’t close? The writer of a destructor does not know why the destructor is called and cannot “refuse to act” by throwing an exception. See [discussion](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Sd-never-fail). To make the problem worse, many “close/release” operations are not retryable. If at all possible, consider failure to close/cleanup a fundamental design error and terminate.

> NOTE: 翻译如下:
>
> "许多人试图设计一种万无一失的方案来处理析构函数中的故障。没有人能成功地提出一个总体方案。这可能是一个真正的实际问题:例如，一个套接字不会关闭怎么办?析构函数的编写者不知道为什么要调用析构函数，也不能通过抛出异常来“拒绝行动”。看到讨论。使问题更糟的是，许多“关闭/发布”操作是不可重试的。如果可能的话，将关闭/清除失败视为基本设计错误并终止。"
>
> 1、最后一句话对应的是上述example

### Note

Declare a destructor `noexcept`. That will ensure that it either completes normally or terminates the program.

> NOTE: 这其实对应的是上述example

### Note

If a resource cannot be released and the program may not fail, try to signal the failure to the rest of the system somehow (maybe even by modifying some global state and hope something will notice and be able to take care of the problem). Be fully aware that this technique is special-purpose and error-prone. Consider the “my connection will not close” example. Probably there is a problem at the other end of the connection and only a piece of code responsible for both ends of the connection can properly handle the problem. The destructor could send a message (somehow) to the responsible part of the system, consider that to have closed the connection, and return normally.

> NOTE: 翻译如下:
>
> "如果一个资源不能被释放，并且程序不能失败，尝试以某种方式向系统的其他部分发出失败的信号(甚至可能通过修改一些全局状态，希望某些东西会注意到并能够处理这个问题)。要充分认识到，这种技术是专用的，而且容易出错。考虑“my connection will not close”示例。可能在连接的另一端有问题，只有一段负责连接两端的代码才能正确处理这个问题。析构函数可以(以某种方式)向负责的部分o发送一条消息"
>
> 1、结合我的实践来看，有的时候，将resource release放到一个显式的`release`函数中，由programmer来进行调用，而不是放到destructor中；

### Note

If a destructor uses operations that may fail, it can catch exceptions and in some cases still complete successfully (e.g., by using a different clean-up mechanism from the one that threw an exception).

### Enforcement

(Simple) A destructor should be declared `noexcept` if it could throw.

> NOTE: 这岂不是违背了原则？

## CppCoreGuidelines [C.37: Make destructors `noexcept`](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c37-make-destructors-noexcept)

### Reason

[A destructor may not fail](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-dtor-fail). If a destructor tries to exit with an exception, it’s a bad design error and the program had better terminate.



### Note

A destructor (either user-defined or compiler-generated) is implicitly declared `noexcept` (independently of what code is in its body) if all of the members of its class have `noexcept` destructors. By explicitly marking destructors `noexcept`, an author guards against(防止) the destructor becoming implicitly `noexcept(false)` through the addition or modification of a class member.

> NOTE: 翻译如下:
>
> "如果类的所有成员都有`noexcept '析构函数，则析构函数(用户定义的或编译器生成的)将隐式声明为`noexcept '(与主体中的代码无关)。
> 通过显式地标记析构函数' noexcept '，作者可以防止析构函数通过添加或修改类成员而隐式地变成' noexcept(false) '。"

### Example

 Not all destructors are `noexcept` by default; one throwing member poisons(毒害) the whole class hierarchy

> NOTE: 
>
> 1、这个例子所展示的"one throwing member"即`Details x`导致compiler将`struct X` 的destructor  `~X()` 声明为 `noexcept(false)` aka can throw

```c++
struct X {
    Details x;  // happens to have a throwing destructor
    // ...
    ~X() { }    // implicitly noexcept(false); aka can throw
};
```

So, if in doubt, declare a destructor `noexcept`.

### Note

 Why not then declare all destructors `noexcept`? Because that would in many cases – especially simple cases – be distracting clutter.



### Enforcement

 (Simple) A destructor should be declared `noexcept` if it could throw.



## 我的实践

今天在使用pclint来检查待目的时候，有如下告警:

```c++
D:\hs\UST\OptUst\trunk\Sources\sys\opt\api\HSTradeApi\cpp_sqlite3\CppSQLite3.cpp  
    142  warning 1551: function 'CppSQLite3Buffer::clear' called outside of a 
    try block in destructor 'CppSQLite3Buffer::~CppSQLite3Buffer' is not 
    declared as never throwing
        clear();
```




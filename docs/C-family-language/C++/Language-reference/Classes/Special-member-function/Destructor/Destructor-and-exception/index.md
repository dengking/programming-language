# Destructor and exception

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

## CppCoreGuidelines 

### [C.36: A destructor may not fail](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c36-a-destructor-may-not-fail)

#### Reason

 In general we do not know how to write error-free code if a destructor should fail. The standard library requires that all classes it deals with have destructors that do not exit by throwing.



#### Example



```
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

#### Note

 

Many have tried to devise a fool-proof scheme for dealing with failure in destructors. None have succeeded to come up with a general scheme. This can be a real practical problem: For example, what about a socket that won’t close? The writer of a destructor does not know why the destructor is called and cannot “refuse to act” by throwing an exception. See [discussion](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Sd-never-fail). To make the problem worse, many “close/release” operations are not retryable. If at all possible, consider failure to close/cleanup a fundamental design error and terminate.



#### Note

 

Declare a destructor `noexcept`. That will ensure that it either completes normally or terminates the program.



#### Note

 

If a resource cannot be released and the program may not fail, try to signal the failure to the rest of the system somehow (maybe even by modifying some global state and hope something will notice and be able to take care of the problem). Be fully aware that this technique is special-purpose and error-prone. Consider the “my connection will not close” example. Probably there is a problem at the other end of the connection and only a piece of code responsible for both ends of the connection can properly handle the problem. The destructor could send a message (somehow) to the responsible part of the system, consider that to have closed the connection, and return normally.



#### Note

 

If a destructor uses operations that may fail, it can catch exceptions and in some cases still complete successfully (e.g., by using a different clean-up mechanism from the one that threw an exception).



#### Enforcement

 

(Simple) A destructor should be declared `noexcept` if it could throw.



### [C.37: Make destructors `noexcept`](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c37-make-destructors-noexcept)

#### Reason

 [A destructor may not fail](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-dtor-fail). If a destructor tries to exit with an exception, it’s a bad design error and the program had better terminate.



#### Note

 

A destructor (either user-defined or compiler-generated) is implicitly declared `noexcept` (independently of what code is in its body) if all of the members of its class have `noexcept` destructors. By explicitly marking destructors `noexcept`, an author guards against the destructor becoming implicitly `noexcept(false)` through the addition or modification of a class member.



#### Example

 

Not all destructors are noexcept by default; one throwing member poisons the whole class hierarchy



```
struct X {
    Details x;  // happens to have a throwing destructor
    // ...
    ~X() { }    // implicitly noexcept(false); aka can throw
};
```

So, if in doubt, declare a destructor noexcept.

#### Note

 

Why not then declare all destructors noexcept? Because that would in many cases – especially simple cases – be distracting clutter.



#### Enforcement

 

(Simple) A destructor should be declared `noexcept` if it could throw.
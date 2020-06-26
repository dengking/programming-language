# Attach by Initialization



## More C++ Idioms/[Attach by Initialization](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Attach_by_Initialization)

> NOTE: 在下面的solution and sample code段描述了这个idiom的思想：充分利用**objects of static storage duration**的“initialized before `main` begins execution”特性，即它的构造函数会在`main`函数开始之前被执行的特性来“Attach a user-defined object to a framework before program execution begins.”

### Intent

Attach a user-defined object to a framework before program execution begins.

### Also Known As

Static-object-with-constructor

> NOTE: 这个名称中的**constructor**是有特殊含义的：Static object的constructor会在main函数之前被执行。

### Motivation

Certain application programming frameworks, such as GUI frameworks (e.g., Microsoft MFC) and object request brokers (e.g., some CORBA implementations) use their own internal message loops (a.k.a. event loops) to control the entire application. Application programmers may or may not have the freedom to write the application-level `main` function. Often, the `main` function is buried deep inside the application framework (e.g, `AfxWinMain` in case of MFC). Lack of access to `main` keeps programmers from writing application-specific initialization code before the main event loop begins. Attach-by-initialization idiom is a way of executing application-specific code before the execution of framework-controlled loop begins.

### Solution and Sample Code

In C++, **global objects** and **static objects in global namespace** are initialized before `main` begins execution. These objects are also known as **objects of static storage duration**. This property of objects of static storage duration can be used to attach an object to a *system* if programmers are not allowed to write their own `main` function. For instance, consider the following (smallest possible) example using Microsoft Foundation Classes (MFC).

```c++
///// File = Hello.h
class HelloApp: public CWinApp
{
public:
  virtual BOOL InitInstance ();
};
///// File = Hello.cpp

#include <afxwin.h>
#include "Hello.h"
HelloApp myApp; // Global "application" object
BOOL HelloApp::InitInstance ()
{
 m_pMainWnd = new CFrameWnd();
 m_pMainWnd->Create(0,"Hello, World!!");
 m_pMainWnd->ShowWindow(SW_SHOW);
 return TRUE;
}
```

The above example creates a window titled "Hello, World!" and nothing more. The key thing to note here is the global object `myApp` of type `HelloApp`. The `myApp` object is **default-initialized** before the execution of `main`. As a side-effect of initializing the objects, the constructor of `CWinApp `is also called. The `CWinApp `class is a part of the framework and invokes constructors of several other classes in the framework. During the execution of these constructors, the **global object** is attached to the **framework**. The object is later on retrieved by `AfxWinMain`, which is an equivalent of regular `main` in MFC. The `HelloApp::InitInstance `member function is shown just for the sake of completeness and is not an integral part of the idiom. This function is called after `AfxWinMain` begins execution.

**Global and static objects** can be initialized in several ways: [default constructor](https://en.cppreference.com/w/cpp/language/default_constructor), constructors with parameters, assignment from a return value of a function, [dynamic initialization](https://en.cppreference.com/w/cpp/language/initialization#Dynamic_initialization), etc.

**Caveat**

In C++, objects in the same compilation unit are created in order of definition. However, the order of initialization of objects of static storage duration across different compilation units is not well defined. Objects in a namespace are created before any function/variable in that namespace is accessed. This may or may not be before main. Order of destruction is the reverse of initialization order but the initialization order itself is not standardized. Because of this undefined behavior, *static initialization order problem* comes up when a constructor of a static object uses another static object that has not been initialized yet. This idiom makes it easy to run into this **trap** because it depends on an object of static storage duration.

### References

[Proposed C++ language extension to improve portability of the Attach by Initialization idiom](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/1995/N0717.htm)
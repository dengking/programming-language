# Exception and stack unwind



## geeksforgeeks [Stack Unwinding in C++](https://www.geeksforgeeks.org/stack-unwinding-in-c/)

The process of removing function entries from function call stack at run time is called [Stack Unwinding](http://en.wikipedia.org/wiki/Call_stack#Unwinding). Stack Unwinding is generally related to **Exception Handling**. In C++, when an exception occurs, the function call stack is linearly searched for the **exception handler**, and all the entries before the function with exception handler are removed from the function call stack. So exception handling involves Stack Unwinding if exception is not handled in same function (where it is thrown).

## microsoft [Exceptions and Stack Unwinding in C++](https://docs.microsoft.com/en-us/cpp/cpp/exceptions-and-stack-unwinding-in-cpp?view=msvc-160)





## ibm [Stack unwinding (C++ only)](https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.3.0/com.ibm.zos.v2r3.cbclx01/cplr155.htm) 

When an **exception** is thrown and control passes from a **try block** to a **handler**, the `C++` run time calls **destructors** for all **automatic objects** constructed since the beginning of the try block. This process is called ***stack unwinding***. The **automatic objects** are destroyed in reverse order of their construction. (**Automatic objects** are **local objects** that have been declared **auto** or **register**, or not declared **static** or **extern**. An automatic object `x` is deleted whenever the program exits the block in which `x` is declared.)

If an **exception** is thrown during construction of an object consisting of subobjects or array elements, destructors are only called for those subobjects or array elements successfully constructed **before** the exception was thrown. A destructor for a **local static object** will only be called if the object was successfully constructed.

### Destructor and exception

> NOTE: 这部分内容放到了`C++\Language-reference\Classes\Special-member-function\Destructor\Destructor-and-exception`章节。

### Delegating constructor and exception

When the **delegating constructors feature** is enabled, if an exception is thrown in the body of a **delegating constructor**, the **destructors** of the objects constructed through **target constructor** will be invoked automatically. The **destructors** must be called in such a way that it calls the destructors of subobjects as appropriate. In particular, it should call the destructors for **virtual base classes** if the **virtual base classes** are created through the **target constructor**.

If an exception is thrown in the body of a **delegating constructor**, the destructor is invoked for the object created by the **target constructor**. If an exception escapes from a non-delegating constructor, the unwinding mechanism will call the destructors for the completely constructed subobjects. The following example demonstrates this:

```cpp
class D{
  D():D('a') { printf("D:D().\n");}

  D:D(char) try: D(55){
    printf("D::D(char). Throws.\n");
    throw 0;
  }
  catch(...){
    printf("D::D(char).Catch block.\n");
  }

  D:D(int i):i(i_) {printf("D::D(int).\n");}

  D:~D() {printf("D::~D().\n");}
}

int main(void){
  D d;
}
```

The output of the example is:

```cpp
D::D(int).
D::D(char).Throws.
D::~D().
D::D(char).Catch block.
```

In this example, an exception occurs in the delegating constructor `D:D(char)`, so destructor `D:~D()` is invoked for object `d`.

For more information, see [Delegating constructors (C++11)](https://www.ibm.com/support/knowledgecenter/SSLTBW_2.3.0/com.ibm.zos.v2r3.cbclx01/delegating_ctors.htm?view=kc)



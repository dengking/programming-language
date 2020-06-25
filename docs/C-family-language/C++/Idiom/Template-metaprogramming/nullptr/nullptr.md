# nullptr



## More C++ Idioms/[nullptr](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/nullptr)

### Intent

To distinguish between an integer 0 and a null pointer.

### Motivation

```c++
class C
{
  public:
    void func(){}
};
int main()
{
// if the following were a valid definition of NULL in C++
#define NULL ((void *)0)

// then...

char * str = NULL; // Can't automatically convert void * to char *
void (C::*pmf) () = &C::func;
if (pmf == NULL) {} // Can't automatically convert from void * to pointer to member function.
}

```

> NOTE: 上述代码编译报错如下：
>
> ```c++
> test.cpp: In function ‘int main()’:
> test.cpp:9:24: error: invalid conversion from ‘void*’ to ‘char*’ [-fpermissive]
>  #define NULL ((void *)0)
>                         ^
> test.cpp:13:14: note: in expansion of macro ‘NULL’
>  char * str = NULL; // Can't automatically convert void * to char *
>               ^
> test.cpp:9:24: error: invalid operands of types ‘void (C::*)()’ and ‘void*’ to binary ‘operator==’
>  #define NULL ((void *)0)
>                         ^
> test.cpp:15:12: note: in expansion of macro ‘NULL’
>  if (pmf == NULL) {} // Can't automatically convert from void * to pointer to member function.
> 
> ```
>
> 



```c++
class C
{
  public:
    void func(){}
};
int main()
{
// if the following were a valid definition of NULL in C++
#define NULL 0

// then...

char * str = NULL; // Can't automatically convert void * to char *
void (C::*pmf) () = &C::func;
if (pmf == NULL) {} // Can't automatically convert from void * to pointer to member function.
}

```

> NOTE: 上述代码是能够编译通过的

The crux（关键） of the matter, in fact, is that `C++` disallows conversions from `void *`, even when the value is a constant zero, **but**, for constant zero, introduces a special case anyway: *int* to pointer (actually several of them: *short* to pointer, *long* to pointer, etc.). This is, in fact, even worse than allowing (for the constant case) the former exception, the more so given the support of function overloading in the language.

> NOTE: 事实上，问题的关键是c++不允许从`void *`转换，即使值是常量0，但是，对于常量0，无论如何都引入了一种特殊情况:int到指针(实际上有几种:short到指针，long到指针，等等)。事实上，这甚至比允许(对于常量情况)前一个异常更糟糕，尤其是考虑到语言中对函数重载的支持。

```C++
void func(int);
void func(double *);
int main()
{
  func (static_cast <double *>(0)); // calls func(double *) as expected
  func (0); // calls func(int) but the programmer might have intended double *, because 0 IS also a null pointer constant (or the reader might be misled).
}
```

The use of the macro `NULL` has its own set of problems as well. C++ requires that macro `NULL` be defined as an **integral constant expression** having the value of 0. So unlike in C, `NULL` cannot be defined as `(void *)0` in the C++ standard library. Furthermore, the exact form of definition is left to the particular implementation, which means that e.g. both 0 and 0L are viable options, among some others. This is a trouble as it can cause confusion in overload resolution. Worse, the way confusing overload resolution manifests itself will vary depending on the compiler and its settings used. An illustrative case is shown in this slight modification of the example above:

```C++
#include <cstddef>

void func(int);
void func(double *);

int main()
{
  func (static_cast <double *>(0)); // calls func(double *) as expected
  func (0); // calls func(int) but double * may be desired because 0 IS also a null pointer

  func (NULL) // calls func(int) if NULL is defined as 0 (confusion, func(double *) was intended!) - logic error at runtime,
              // but the call is ambiguous if NULL is defined as 0L (yet more confusion to the unwary!) - compilation error
}
```

### Solution and Sample Code

The nullptr idiom solves some of the above problems and can be put in reusable form, as a library solution. It is a very close approximation of a "null keyword", by using only pre-C++11 standard features.

The following is such a library solution as mostly found in Effective C++ by Scott Meyers, Second Edition, Item 25 (it is not present in the third edition of the book).

```C++
const // It is a const object...
class nullptr_t 
{
  public:
    template<class T>
    inline operator T*() const // convertible to any type of null non-member pointer...
    { return 0; }

    template<class C, class T>
    inline operator T C::*() const   // or any type of null member pointer...
    { return 0; }

  private:
    void operator&() const;  // Can't take address of nullptr

} nullptr = {};
```

The following code illustrates some usage cases (and assumes the class template above has already been #included).

```C++
#include <typeinfo>



struct C
{
  void func();
};

template<typename T> 
void g( T* t ) {}

template<typename T> 
void h( T t ) {}

void func (double *) {}
void func (int) {}

int main(void)
{
  char * ch = nullptr;        // ok
  func (nullptr);             // Calls func(double *)
  func (0);                   // Calls func(int)
  void (C::*pmf2)() = 0;      // ok
  void (C::*pmf)() = nullptr; // ok
  nullptr_t n1, n2;
  n1 = n2;
  //nullptr_t *null = &n1;    // Address can't be taken.
  if (nullptr == ch) {}       // ok
  if (nullptr == pmf) {}      // Valid statement; but fails on g++ 4.1.1-4.5 due to bug #33990
// for GCC 4: if ((typeof(pmf))nullptr == pmf) {}
  const int n = 0;
  if (nullptr == n) {}        // Should not compile; but only Comeau shows an error.
  //int p = 0;
  //if (nullptr == p) {}      // not ok
  //g (nullptr);              // Can't deduce T
  int expr = 0;
  char* ch3 = expr ? nullptr : nullptr; // ch3 is the null pointer value
  //char* ch4 = expr ? 0 : nullptr;     // error, types are not compatible
  //int n3 = expr ? nullptr : nullptr;  // error, nullptr can’t be converted to int
  //int n4 = expr ? 0 : nullptr;        // error, types are not compatible

  h( 0 );                // deduces T = int
  h( nullptr );          // deduces T = nullptr_t
  h( (float*) nullptr ); // deduces T = float*

  sizeof( nullptr );     // ok
  typeid( nullptr );     // ok
  throw nullptr;         // ok
}
```

Unfortunately, there seems to be a [bug](http://gcc.gnu.org/bugzilla/show_bug.cgi?id=33990) in gcc 4.1.1 compiler that does not recognize the comparison of nullptr with point to member function (pmf). The above code compiles on VC++ 8.0 and Comeau 4.3.10.1 beta.

Note that `nullptr` idioms makes use of the [Return Type Resolver](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Return_Type_Resolver) idiom to automatically deduce a null pointer of the correct type depending upon the type of the instance it is assigning to. For example, if `nullptr` is being assigned to a character pointer, a char type instantiation of the templatized conversion function is created.

### Consequences 

There are some disadvantages of this technique and are discussed in the [N2431 proposal](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2431.pdf) draft. In summary, the disadvantages are

- A header must be included to use nullptr. In C++11, `nullptr` itself is a keyword and requires no headers (although `std::nullptr_t` does).
- Compilers have historically produced (arguably) unsatisfactory diagnostics when using the code proposed.
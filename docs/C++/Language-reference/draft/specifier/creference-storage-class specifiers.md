[TOC]



# [Storage-class specifiers](https://en.cppreference.com/w/c/language/storage_duration)



Specify *storage duration* and *linkage* of objects and functions:

- `auto` - automatic duration and no linkage
- `register` - automatic duration and no linkage; address of this variable cannot be taken
- `static` - static duration and internal linkage (unless at block scope)
- `extern` - static duration and external linkage (unless already declared internal)
- `_Thread_local` - thread storage duration



其实这是非常简单的，一个name，要么是在本translation unit中定义，要么是在另外 一个translation unit中定义；如果一个name是在另外一个translation unit中定义的，那么在本[translation unit](https://en.wikipedia.org/wiki/Translation_unit_(programming))中它就是external linkage；

***SUMMARY*** : `_Thread_local`和前面的三个`specifier`不同的是它仅仅specify storage duration而没有specify linkage；如下面的[Explanation](#Explanation)中所介绍的，当还需要指定linkage的时候，需要和`static`和`extern`一起来使用；

***SUMMARY*** : c语言的设计者提供这些key word来允许用户对variable的storage duration或[Lifetime](https://en.cppreference.com/w/c/language/lifetime)和linkage进行控制，这种通过specifier来控制program的runtime行为的思想在《how to master programming language.md》中已经描述了；storage duration或[Lifetime](https://en.cppreference.com/w/c/language/lifetime)是runtime概念；

***SUMMARY*** : 拥有linkage的entity的storage duration都是static duration；

### Explanation

**Storage-class specifiers** appear in [declarations](https://en.cppreference.com/w/c/language/declarations). At most one specifier may be used, except that `_Thread_local` may be combined with `static` or `extern` to adjust **linkage** (since C11). The **storage-class specifiers** determine two independent properties of the names they declare: *storage duration* and *linkage*.

1) The `auto` specifier is only allowed for objects declared at **block scope** (except function parameter lists). It indicates **automatic storage duration** and **no linkage**, which are the **defaults** for these kinds of declarations.

2) The `register` specifier is only allowed for objects declared at **block scope**, including **function parameter lists**. It indicates automatic storage duration and no linkage (which is the default for these kinds of declarations), but additionally **hints** the optimizer to store the value of this variable in a CPU register if possible. Regardless of whether this optimization takes place or not, variables declared `register` cannot be used as arguments to the [address-of operator](https://en.cppreference.com/w/c/language/operator_member_access), cannot use [alignas](https://en.cppreference.com/w/c/language/_Alignas) (since C11), and `register` arrays are not convertible to pointers.

***SUMMARY*** : 从这里也可以看出C语言的能力是有限的，它还依赖于compiler对它的翻译，而programmer只能够给compiler一些hint；

3) The `static` specifier specifies both static storage duration (unless combined with `_Thread_local` 此处unless表示的是对前面的论断的否定，即它不符合前面的论断) (since C11)and internal linkage (unless used at block scope). It can be used with functions at **file scope** and with variables at both **file** and **block scope**, but not in function parameter lists.



4) The `extern` specifier specifies static storage duration (unless combined with `_Thread_local`) (since C11) and external linkage. It can be used with function and object declarations in both file and block scope (excluding function parameter lists). If `extern` appears on a redeclaration of an identifier that was already declared with internal linkage, the linkage remains internal. Otherwise (if the prior declaration was external, no-linkage, or is not in scope), the linkage is external.



5) `_Thread_local` indicates *thread storage duration*. It cannot be used with function declarations. If it is used on a declaration of an object, it must be present on every declaration of the same object. If it is used on a block-scope declaration, it must be combined with either `static` or `extern` to decide linkage.



If no storage-class specifier is provided, the defaults are:

- `extern` for all functions
- `extern` for objects at file scope
- `auto` for objects at block scope

For any `struct` or `union` declared with a storage-class specifier, the storage duration (but not linkage) applies to their members, recursively.

Function declarations at **block scope** can use `extern` or none at all（不能够使用`static`）. Function declarations at file scope can use `extern`or `static`.

Function parameters cannot use any storage-class specifiers other than `register`. Note that `static` has special meaning in function parameters of array type.

### Storage duration

Every [object](https://en.cppreference.com/w/c/language/object) has a property called *storage duration*, which limits the object [lifetime](https://en.cppreference.com/w/c/language/lifetime). There are four kinds of storage duration in C:

- ***automatic*** storage duration. The storage is allocated when the [block](https://en.cppreference.com/w/c/language/statements#Compound_statements) in which the object was declared is entered and deallocated when it is **exited** by any means ([goto](https://en.cppreference.com/w/c/language/goto), [return](https://en.cppreference.com/w/c/language/return), reaching the end). One exception is the [VLAs](https://en.cppreference.com/w/c/language/array#Variable-length_arrays); their storage is allocated when the declaration is executed, not on block entry, and deallocated when the declaration goes out of scope, not than when the block is **exited** (since C99). If the block is entered recursively, a new allocation is performed for every recursion level. All function parameters and non-`static` block-scope objects have this storage duration, as well as [compound literals](https://en.cppreference.com/w/c/language/compound_literal) used at block scope.

  ***SUMMARY*** : 上面这段话没有搞懂，其实根源在于对 [VLAs](https://en.cppreference.com/w/c/language/array#Variable-length_arrays)没有理解透彻；

  ***SUMMARY*** : automatic的object的一种典型的案例就是：位于stack上的，它们是在函数被调用的时候被执行的；最后一段话的总结是非常好的；

- ***static*** storage duration. The storage duration is the entire execution of the program, and the value stored in the object is initialized only once, prior to [main function](https://en.cppreference.com/w/c/language/main_function). All objects declared `static` and all objects with either internal or external linkage that aren't declared `_Thread_local` (since C11) have this storage duration.

  ***SUMMARY*** : 关于`static`和`_Thread_local`在[Explanation](#Explanation)中的解释是非常清楚的；

- ***thread*** storage duration. The storage duration is the entire execution of the thread in which it was created, and the value stored in the object is initialized when the thread is started. Each thread has its own, distinct, object. If the thread that executes the expression that accesses this object is not the thread that executed its（指代前面的this object） initialization, the behavior is implementation-defined. All objects declared `_Thread_local` have this storage duration.

  ***SUMMARY*** : 最后一句话没有搞懂

- ***allocated*** storage duration. The storage is allocated and deallocated on request, using [dynamic memory allocation](https://en.cppreference.com/w/c/memory) functions.





#### Linkage

Linkage refers to the ability of an identifier (variable or function) to be referred to in other scopes. If a variable or function with the same identifier is declared in several scopes, but cannot be referred to from all of them, then several instances of the variable are generated. The following linkages are recognized:

- ***no linkage***. The identifier can be referred to only from the **scope** it is in. All function parameters and all non-`extern` block-scope variables (including the ones declared `static`) have this linkage.
- ***internal linkage***. The identifier can be referred to from **all scopes** in the **current translation unit**. All `static` file-scope identifiers (both functions and variables) have this linkage.
- ***external linkage***. The identifier can be referred to from any other translation units in the entire program. All non-`static` functions, all `extern` variables (unless earlier declared `static`), and all file-scope non-`static`variables have this linkage.





If the same identifier appears with both internal and external linkage in the same translation unit, the behavior is undefined. This is possible when [tentative definitions](https://en.cppreference.com/w/c/language/extern) are used.



#### Linkage and libraries

Declarations with **external linkage** are commonly made available in header files so that all translation units that [#include](https://en.cppreference.com/w/c/preprocessor/include) the file may refer to the same identifier that are defined elsewhere.

Any declaration with **internal linkage** that appears in a header file results in a separate and distinct object in each translation unit that includes that file.

***SUMMARY*** : linkage和`#include`

Library interface:

```c
// flib.h
#ifndef FLIB_H
#define FLIB_H
void f(void);              // function declaration with external linkage
extern int state;          // variable declaration with external linkage
static const int size = 5; // definition of a read-only variable with internal linkage
enum { MAX = 10 };         // constant definition
inline int sum (int a, int b) { return a+b; } // inline function definition
#endif // FLIB_H
```

Library implementation:

```c
// flib.c
#include "flib.h"
static void local_f(int s) {}  // definition with internal linkage (only used in this file)
static int local_state;        // definition with internal linkage (only used in this file)
 
int state;                     // definition with external linkage (used by main.c)
void f(void) {local_f(state);} // definition with external linkage (used by main.c)
```

Application code:

```c
// main.c 
#include "flib.h"
int main(void)
{
    int x[MAX] = {size}; // uses the constant and the read-only variable
    state = 7;           // modifies state in flib.c
    f();                 // calls f() in flib.c
}
```

### Keywords

[`auto`](https://en.cppreference.com/w/c/keyword/auto), [`register`](https://en.cppreference.com/w/c/keyword/register), [`static`](https://en.cppreference.com/w/c/keyword/static), [`extern`](https://en.cppreference.com/w/c/keyword/extern), [`_Thread_local`](https://en.cppreference.com/w/c/keyword/_Thread_local)

### Notes

The keyword `_Thread_local` is usually used through the convenience macro [thread_local](https://en.cppreference.com/w/c/thread/thread_local), defined in the header `threads.h`.

The [typedef](https://en.cppreference.com/w/c/language/typedef) specifier is formally listed as a storage-class specifier in the C language grammar, but it is used to declare type names and does not specify storage.

Names at **file scope** that are `const` and not `extern` have external linkage in C (as the default for all file-scope declarations), but internal linkage in C++.

### Example

```c
#include <stdio.h>
#include <stdlib.h>
 
/* static storage duration */
int A;
 
int main(void)
{
    printf("&A = %p\n", (void*)&A);
 
    /* automatic storage duration */
    int A = 1;   // hides global A
    printf("&A = %p\n", (void*)&A);
 
    /* allocated storage duration */
    int *ptr_1 = malloc(sizeof(int));   /* start allocated storage duration */
    printf("address of int in allocated memory = %p\n", (void*)ptr_1);
    free(ptr_1);                        /* stop allocated storage duration  */
 
}
```


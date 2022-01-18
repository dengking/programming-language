# Non-local variables initialization

1、static initialization和dynamic initialization是两个非常重要的概念，对于static storage object的initialization往往会涉及到此；

2、在 pabloariasal [C++ - Initialization of Static Variables](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/) 中，对这个问题有着非常好的描述

## cppreference [Initialization](https://en.cppreference.com/w/cpp/language/initialization) # Non-local variables

> NOTE: 
>
> 一、相关章节: 
>
> 1、`C++\Language-reference\Basic-concept\Object\Lifetime-and-storage-duration\Static-storage-duration\Non-local-object` 
>
> 二、在 zhihu [用std::once_flag来std::call_once，运行时却执行了2次？](https://zhuanlan.zhihu.com/p/408838350) 中，有对下面内容的翻译

All non-local variables with static [storage duration](https://en.cppreference.com/w/cpp/language/storage_duration) are initialized as part of program startup, before the execution of the [main function](https://en.cppreference.com/w/cpp/language/main_function) begins (unless deferred, see below). 

All non-local variables with **thread-local storage duration** are initialized as part of thread launch, sequenced-before the execution of the thread function begins. 

> NOTE: 
>
> 一、上面讨论了两种non-local variable:
>
> 1、with static [storage duration](https://en.cppreference.com/w/cpp/language/storage_duration) 
>
> 2、with **thread-local storage duration**
>
> 原文的内容，很多都会对这两种情况进行讨论。

For both of these classes of variables, initialization occurs in two distinct stages:

> NOTE: 
>
> 一、order: 首先执行static initialization然后执行dynamic initialization。

### Static initialization

There are two forms of static initialization:

1) If relevant, [constant initialization](https://en.cppreference.com/w/cpp/language/constant_initialization) is applied.

> NOTE: 
>
> 此处的"relevant"是什么含义？

2) Otherwise, non-local static and thread-local variables are [zero-initialized](https://en.cppreference.com/w/cpp/language/zero_initialization).

> NOTE: 
>
> 两种类型都会被zero-initialized

In practice:

1、**Constant initialization** is usually applied at compile time. Pre-calculated object representations are stored as part of the program image. If the compiler doesn't do that, it must still guarantee that the initialization happens before any dynamic initialization.

2、Variables to be zero-initialized are placed in the `.bss` segment of the program image, which occupies no space on disk and is zeroed out by the OS when loading the program.

> NOTE: 
>
> 是由OS来进行 [zero-initialization](https://en.cppreference.com/w/cpp/language/zero_initialization) 的

### Dynamic initialization

After all **static initialization** is completed, **dynamic initialization** of **non-local variables** occurs in the following situations:

1) *Unordered dynamic initialization*, which applies only to (static/thread-local) class template [static data members](https://en.cppreference.com/w/cpp/language/static) and [variable templates](https://en.cppreference.com/w/cpp/language/variable_template) (since C++14) that aren't [explicitly specialized](https://en.cppreference.com/w/cpp/language/template_specialization).

> NOTE: 
>
> 这种情况仅限于template

Initialization of such static variables is indeterminately sequenced with respect to all other dynamic initialization except if the program starts a thread before a variable is initialized, in which case its initialization is unsequenced (since C++17). 

Initialization of such thread-local variables is unsequenced with respect to all other dynamic initialization.

2) *Partially-ordered dynamic initialization* (since C++17)



3) *Ordered dynamic initialization*, which applies to all other non-local variables: within a single translation unit, initialization of these variables is always [sequenced](https://en.cppreference.com/w/cpp/language/eval_order) in exact order their definitions appear in the source code.

Initialization of static variables in different translation units is indeterminately sequenced. 

Initialization of thread-local variables in different translation units is unsequenced.

> NOTE: 
>
> 上面这段话其实讨论了三种情况



## stackoverflow [What is dynamic initialization of object in c++?](https://stackoverflow.com/questions/5945897/what-is-dynamic-initialization-of-object-in-c)

[A](https://stackoverflow.com/a/5945936)

> NOTE: 
>
> 1、这个回答是较好的

Dynamic initialization is that in which initialization value isn't known at compile-time. It's computed at runtime to initialize the variable.

Example,

```cpp
int factorial(int n)
{
     if ( n < 0 )       return -1; //indicates input error
     else if ( n == 0 ) return 1;
     else               return n * factorial(n-1);
}

int const a = 10 ; //static initialization 
             //10 is known at compile time. Its 10!

int const b = factorial(8); //dynamic initialization 
                      //factorial(8) isn't known at compile time,
                      //rather it's computed at runtime.
```

That is, static-initialization *usually* involves constant-expression (which is known at compile-time), while dynamic-initialization involves non-constant expression.

So there are two kind of initializations:

- Static initialization : Its either zero-initialization or initialization with a constant expression
- Any other initialization is dynamic initialization.

> NOTE: 
>
> 1、总结的非常好

## TODO



http://cplusplus.bordoon.com/static_initialization.html

http://laristra.github.io/flecsi/src/developer-guide/patterns/meyers_singleton.html

http://cplusplus.bordoon.com/static_initialization.html

https://stackoverflow.com/questions/42386329/c-non-local-static-object-vs-local-static-object

http://the-algo-blog.blogspot.com/2011/07/c-non-local-static-object.html

https://www.geeksforgeeks.org/static-objects-destroyed/

http://www.devx.com/tips/Tip/13634
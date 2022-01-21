# creference [Storage-class specifiers](https://en.cppreference.com/w/c/language/storage_duration)

Specify *storage duration* and *linkage* of objects and functions:

- `auto` - automatic duration and no linkage
- `register` - automatic duration and no linkage; address of this variable cannot be taken
- `static` - static duration and internal linkage (unless at block scope)
- `extern` - static duration and external linkage (unless already declared internal)
- `_Thread_local` - thread storage duration



> NOTE: 

> NOTE: `_Thread_local`和前面的三个`specifier`不同的是它仅仅specify storage duration而没有specify linkage；如下面的[Explanation](#Explanation)中所介绍的，当还需要指定linkage的时候，需要和`static`和`extern`一起来使用；

> NOTE: c语言的设计者提供这些key word来允许用户对variable的storage duration或[Lifetime](https://en.cppreference.com/w/c/language/lifetime)和linkage进行控制，这种通过specifier来控制program的runtime行为的思想在`Theory\Programming-language\How-to-master-programming-language.md`中已经描述了；
>
> storage duration或[Lifetime](https://en.cppreference.com/w/c/language/lifetime)是runtime概念；linkage是link-time 概念。

> NOTE : 
>
> 总共有三种linkage：
>
> - no linkage
> - internal linkage
> - external linkage
>
> 拥有linkage（包括internal linkage、external linkage）的entity的storage duration都是static duration；

### Explanation

**Storage-class specifiers** appear in [declarations](https://en.cppreference.com/w/c/language/declarations). At most one specifier may be used, except that `_Thread_local` may be combined with `static` or `extern` to adjust **linkage** (since C11). The **storage-class specifiers** determine two independent properties of the names they declare: *storage duration* and *linkage*.

1) The `auto` specifier is only allowed for objects declared at **block scope** (except function parameter lists). It indicates **automatic storage duration** and **no linkage**, which are the **defaults** for these kinds of declarations.

2) The `register` specifier is only allowed for objects declared at **block scope**, including **function parameter lists**. It indicates automatic storage duration and no linkage (which is the default for these kinds of declarations), but additionally **hints** the optimizer to store the value of this variable in a CPU register if possible. Regardless of whether this optimization takes place or not, variables declared `register` cannot be used as arguments to the [address-of operator](https://en.cppreference.com/w/c/language/operator_member_access), cannot use [alignas](https://en.cppreference.com/w/c/language/_Alignas) (since C11), and `register` arrays are not convertible to pointers.

> NOTE : 从这里也可以看出C语言的能力是有限的，它还依赖于compiler对它的翻译，而programmer只能够给compiler一些hint；

3) The `static` specifier specifies both static storage duration (unless combined with `_Thread_local` 此处unless表示的是对前面的论断的否定，即它不符合前面的论断) (since C11)and internal linkage (unless used at block scope). It can be used with functions at **file scope** and with variables at both **file** and **block scope**, but not in function parameter lists.



4) The `extern` specifier specifies static storage duration (unless combined with `_Thread_local`) (since C11) and external linkage. It can be used with function and object declarations in both file and block scope (excluding function parameter lists). If `extern` appears on a redeclaration of an identifier that was already declared with internal linkage, the linkage remains internal. Otherwise (if the prior declaration was external, no-linkage, or is not in scope), the linkage is external.



5) `_Thread_local` indicates *thread storage duration*. It cannot be used with function declarations. If it is used on a declaration of an object, it must be present on every declaration of the same object. If it is used on a block-scope declaration, it must be combined with either `static` or `extern` to decide linkage.






# Type safety



## wikipedia [Type safety](https://en.wikipedia.org/wiki/Type_safety)

In [computer science](https://en.wikipedia.org/wiki/Computer_science), **type safety** is the extent to which a [programming language](https://en.wikipedia.org/wiki/Programming_language) discourages or prevents **type errors**. A **type error** is erroneous or undesirable program behaviour caused by a discrepancy（不相符） between differing [data types](https://en.wikipedia.org/wiki/Data_type) for the program's constants, variables, and methods (functions), e.g., treating an integer (**int**) as a floating-point number (**float**). Type safety is sometimes alternatively considered to be a property of a computer program rather than the language in which that program is written; that is, some languages have type-safe facilities that can be circumvented（绕开）by programmers who adopt practices that exhibit poor type safety. The formal [type-theoretic](https://en.wikipedia.org/wiki/Type_theory) definition of type safety is considerably stronger than what is understood by most programmers.

[Type enforcement](https://en.wikipedia.org/wiki/Type_enforcement) can be static, catching potential errors at [compile time](https://en.wikipedia.org/wiki/Compile_time), or dynamic, associating type information with values at [run-time](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase)) and consulting them as needed to detect imminent（即将到来的） errors, or a combination of both.

The behaviors classified as **type errors** by a given programming language are usually those that result from attempts to perform operations on [values](https://en.wikipedia.org/wiki/Value_(computer_science)) that are not of the appropriate [data type](https://en.wikipedia.org/wiki/Data_type). This classification is partly based on opinion; it may imply that any operation not leading to program crashes, security flaws or other obvious failures is legitimate and need not be considered an error, or it may imply that any contravention of the programmer's explicit intent (as communicated via typing annotations) to be erroneous and not "type-safe".

In the context of **static (compile-time) type systems**, type safety usually involves (among other things) a guarantee that the **eventual value** of any [expression](https://en.wikipedia.org/wiki/Expression_(programming)) will be a legitimate member of that **expression's static type**. The precise requirement is more subtle than this — see, for example, [subtype](https://en.wikipedia.org/wiki/Subtype) and [polymorphism](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)) for complications.

> nNOTE: type of expression；

Type safety is closely linked to *memory safety*, a restriction on the ability to copy arbitrary bit patterns from one memory location to another. For instance, in an implementation of a language that has some type {\displaystyle t}![t](https://wikimedia.org/api/rest_v1/media/math/render/svg/65658b7b223af9e1acc877d848888ecdb4466560), such that some sequence of bits (of the appropriate length) does not represent a legitimate member of {\displaystyle t}![t](https://wikimedia.org/api/rest_v1/media/math/render/svg/65658b7b223af9e1acc877d848888ecdb4466560), if that language allows data to be copied into a [variable](https://en.wikipedia.org/wiki/Variable_(programming)) of type {\displaystyle t}![t](https://wikimedia.org/api/rest_v1/media/math/render/svg/65658b7b223af9e1acc877d848888ecdb4466560), then it is not type-safe because such an operation might assign a non-{\displaystyle t}![t](https://wikimedia.org/api/rest_v1/media/math/render/svg/65658b7b223af9e1acc877d848888ecdb4466560) value to that variable. Conversely, if the language is type-unsafe to the extent of allowing an arbitrary integer to be used as a [pointer](https://en.wikipedia.org/wiki/Pointer_(computer_programming)), then it is not memory-safe.

> NOTE: 
>
> 1、type safe是memory safe的一个充分条件；

Most **statically typed languages** provide a degree of **type safety** that is strictly stronger than **memory safety**, because their type systems enforce the proper use of [abstract data types](https://en.wikipedia.org/wiki/Abstract_data_type) defined by programmers even when this is not strictly necessary for memory safety or for the prevention of any kind of catastrophic failure.



## 素材

### C++ parameter pack VS C variadic parameter

最能够体现type safety的就是"C++ parameter pack VS C variadic parameter"，在下面文章中，讨论了这个topic: 

1、eli.thegreenplace [Variadic templates in C++](https://eli.thegreenplace.net/2014/variadic-templates-in-c/)

这篇文章有着较好的论述。

2、stackoverflow [Variable number of arguments in C++?](https://stackoverflow.com/questions/1657883/variable-number-of-arguments-in-c) # [A](https://stackoverflow.com/a/1657924)

3、cppreference [Variadic arguments](https://en.cppreference.com/w/cpp/language/variadic_arguments)




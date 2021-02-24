# CppCoreGuidelines [R: Resource management](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#S-resource)

The fundamental aim is to ensure that we don't leak any resources and that we don't hold a resource longer than we need to. An entity that is responsible for releasing a resource is called an **owner**.

> NOTE: 
>
> 1、"don't leak any resources"需要 strong exception safety
>
> 2、resource ownership: 这里提出了**owner**的概念



1、Resource management rule summary:

[R.1: Manage resources automatically using resource handles and RAII (Resource Acquisition Is Initialization)](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-raii)

[R.2: In interfaces, use raw pointers to denote individual objects (only)](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-use-ptr)

[R.3: A raw pointer (a `T*`) is non-owning](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-ptr)

[R.4: A raw reference (a `T&`) is non-owning](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-ref)

[R.5: Prefer scoped objects, don't heap-allocate unnecessarily](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-scoped)

[R.6: Avoid non-`const` global variables](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-global)

2、Allocation and deallocation rule summary:

[R.10: Avoid `malloc()` and `free()`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-mallocfree)

[R.11: Avoid calling `new` and `delete` explicitly](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-newdelete)

[R.12: Immediately give the result of an explicit resource allocation to a manager object](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-immediate-alloc)

[R.13: Perform at most one explicit resource allocation in a single expression statement](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-single-alloc)

[R.14: Avoid `[\]` parameters, prefer `span`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-ap)

[R.15: Always overload matched allocation/deallocation pairs](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-pair)

3、Smart pointer rule summary

> NOTE: 这部分放到了smart pointer章节





## [R.1: Manage resources automatically using resource handles and RAII (Resource Acquisition Is Initialization)](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-raii)



### Reason

To avoid leaks and the complexity of manual resource management. C++'s language-enforced constructor/destructor symmetry mirrors the symmetry inherent in resource acquire/release function pairs such as `fopen`/`fclose`, `lock`/`unlock`, and `new`/`delete`. Whenever you deal with a resource that needs paired acquire/release function calls, encapsulate that resource in an object that enforces pairing for you -- acquire the resource in its constructor, and release it in its destructor.

> NOTE: 
>
> 总结的非常好。
>
> 1、RAII idiom、resource handle
>
> 2、object-based resource management
>
> 最终目的:
>
> 1、保证strong exception safety
>
> 2、no resource leakage
>
> 3、simplify
>
> 



### Example, bad

> NOTE: 原因给出的例子一般，比较容易理解

### Note

Where a resource is "ill-behaved" in that it isn't represented as a class with a destructor, wrap it in a class or use [`finally`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Re-finally)

> NOTE: 
>
> 1、上面这段话中的"ill-behaved"如何理解？
>
> 2、 [`finally`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Re-finally) 其实就是guard

## TODO

1、modernescpp [C++ Core Guidelines: Rules to Resource Management](https://www.modernescpp.com/index.php/c-core-guidelines-rules-to-resource-management)
# CppCoreGuidelines [R: Resource management](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#S-resource)

The fundamental aim is to ensure that we don't leak any resources and that we don't hold a resource longer than we need to. An entity that is responsible for releasing a resource is called an owner.





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



## TODO

1、modernescpp [C++ Core Guidelines: Rules to Resource Management](https://www.modernescpp.com/index.php/c-core-guidelines-rules-to-resource-management)
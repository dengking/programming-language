# Strong exception safety

无论是`std::uniqure_ptr`还是`std::shared_ptr`都涉及dynamic memory allocation，因此需要考虑exception safety的问题，programmer在使用的时候，需要保证strong exception safety。

## CppCoreGuidelines [R: Resource management](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#S-resource)

Allocation and deallocation rule summary:

- [R.10: Avoid `malloc()` and `free()`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-mallocfree)
- [R.11: Avoid calling `new` and `delete` explicitly](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-newdelete)

Smart pointer rule summary:

- [R.22: Use `make_shared()` to make `shared_ptr`s](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-make_shared)
- [R.23: Use `make_unique()` to make `unique_ptr`s](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-make_unique)

## 具体的guideline

本章的后续章节会给出实现strong exception safety的一些guideline:

1、Prefer-use-object-generator

2、Prefer-not-use-reset



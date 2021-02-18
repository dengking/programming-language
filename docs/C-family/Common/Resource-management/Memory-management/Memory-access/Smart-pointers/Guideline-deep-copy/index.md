# Deep copy smart pointer

本章讨论如何deep copy smart pointer 所指向的内容。

## 起因

在阅读 wikipedia [Opaque pointer](https://en.wikipedia.org/wiki/Opaque_pointer) 的 "C++ Example: **d-pointer pattern**" 时，其中给出的例子是典型的 "Copy constructor for a class with `unique_ptr`"，按照 stackoverflow [Copy constructor for a class with unique_ptr](https://stackoverflow.com/questions/16030081/copy-constructor-for-a-class-with-unique-ptr) 中的说法，显然这种情况下是需要deep copy的。

## 相关文章、章节

1、wikipedia [Opaque pointer](https://en.wikipedia.org/wiki/Opaque_pointer)

在这篇文章中，给出了非常好的样例，它收录于 `Opaque-data-type\Opaque-pointer` 章节。

2、stackoverflow [Copy constructor for a class with unique_ptr](https://stackoverflow.com/questions/16030081/copy-constructor-for-a-class-with-unique-ptr) 

在这篇文章中，给出了非常好的样例，它收录于 `Smart-pointers\unique_ptr\unique_ptr-as-member\Copy-constructor-for-a-class-with-unique_ptr` 章节。

3、`Guideline-strong-exception-safety\Prefer-use-object-generator\std-make_unique\Deep-copy` 

其中讨论了`std::make_unique` deep copy的内容。
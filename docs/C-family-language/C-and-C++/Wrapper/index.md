# Wrapper

标准library中提供的很多class、template，其实都可以看做是**wrapper**，每个**wrapper**都有对应的**target**。

## Why we need wrapper?

1) for abstraction

wrapper可以看做是一种**abstraction**。

2) use as object

object是first class citizen，通过wrapper，可以将target转换为first class citizen，典型的例子就是`std::function`、`std::mem_fu`。

## How to?

如何实现wrapper？下面是一些需要考虑问题:

如何描述它们的类型？

## Summary of wrapper in C++ standard library

下面是对C++ standard library中的总结:

### Function wrapper



| wrapper         | target          | 章节                                                         |
| --------------- | --------------- | ------------------------------------------------------------ |
| `std::function` | function        | `C++\Language-reference\Functions\Function-as-first-class-citizen\Function-objects\std-utility-library-function-object` |
| `std::mem_fn`   | member function | `C++\Language-reference\Functions\Function-as-first-class-citizen\Function-objects\std-utility-library-function-object` |



### Thread wrapper



| wrapper       | target | 章节                                  |
| ------------- | ------ | ------------------------------------- |
| `std::thread` | thread | `C++\Library\Standard-library\Thread` |



### Reference wrapper



| wrapper                                                      | target    | 章节 |
| ------------------------------------------------------------ | --------- | ---- |
| [reference_wrapper](https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper) (C++11) | reference |      |



### String wrapper

参见`C-and-C++\String`。
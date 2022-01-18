# User-defined conversion 

本章讨论C++中的user-defined conversion，它包括:

1、`User-defined-conversion-function`

2、`Converting-constructor`

## Converting constructor VS User-defined conversion function



1、Converting constructor: 使用其他类型的object，来构造本类型的object

2、User-defined conversion function: 将本类型的object转换为其他类型的object，它会构建一个新的object

## Explicit and  implicit

### C++11 explicit specifier

参见 `C++11-Explicit-specifier` 章节



1、[explicit constructors](https://en.cppreference.com/w/cpp/language/explicit) 不是 [Converting constructor](https://en.cppreference.com/w/cpp/language/converting_constructor)

2、一个class，应该不能够同时既定义 [explicit constructors](https://en.cppreference.com/w/cpp/language/explicit)  又定义 [Converting constructor](https://en.cppreference.com/w/cpp/language/converting_constructor) ，从下面的描述来看，两者之间存在着一定的对立。






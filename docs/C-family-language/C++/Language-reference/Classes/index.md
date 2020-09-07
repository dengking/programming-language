# 关于本章

本章描述如何使用C++进行OOP。本章基于cppreference **[Classes](https://en.cppreference.com/w/cpp/language/classes)**的内容进行了重新整理，章节主要包含如下内容：

| 章节                                                         | 简介                                                         | 主要内容                                                     |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| [Members](./Members/index.md)                                | 类成员                                                       | [Data members](https://en.cppreference.com/w/cpp/language/data_members) - [Member functions](https://en.cppreference.com/w/cpp/language/member_functions)<br/>[Static members](https://en.cppreference.com/w/cpp/language/static) - [Nested classes](https://en.cppreference.com/w/cpp/language/nested_types) |
| [Special-member-functions](./Special-member-functions/index.md) | 特殊的成员函数                                               | [Constructors and member initializer lists](https://en.cppreference.com/w/cpp/language/constructor)<br/>[Default constructor](https://en.cppreference.com/w/cpp/language/default_constructor) - [Destructor](https://en.cppreference.com/w/cpp/language/destructor)<br/>[Copy constructor](https://en.cppreference.com/w/cpp/language/copy_constructor) - [Copy assignment](https://en.cppreference.com/w/cpp/language/copy_assignment)<br/>[Move constructor](https://en.cppreference.com/w/cpp/language/move_constructor)(C++11)<br/>[Move assignment](https://en.cppreference.com/w/cpp/language/move_assignment)(C++11)<br/>[Converting constructor](https://en.cppreference.com/w/cpp/language/converting_constructor) - [`explicit` specifier](https://en.cppreference.com/w/cpp/language/explicit) |
| Special-data-member                                          | 特殊数据成员                                                 | [Bit fields](https://en.cppreference.com/w/cpp/language/bit_field) - [The `this` pointer](https://en.cppreference.com/w/cpp/language/this) |
| Member-access                                                | 成员的访问，主要包括权限                                     | [Member access](https://en.cppreference.com/w/cpp/language/access) - [`friend`](https://en.cppreference.com/w/cpp/language/friend) |
| [Subtype-polymorphism](./Subtype-polymorphism/index.md)      | 按照在`Theory\Programming-paradigm\Object-oriented-programming\Polymorphism`中总结的polymorphism分类方法，将C++中和Subtype-polymorphism的内容置于该目录下 | [Derived class](https://en.cppreference.com/w/cpp/language/derived_class) - [`using`-declaration](https://en.cppreference.com/w/cpp/language/using_declaration)<br>[Virtual function](https://en.cppreference.com/w/cpp/language/virtual) - [Abstract class](https://en.cppreference.com/w/cpp/language/abstract_class)<br/>[`override`](https://en.cppreference.com/w/cpp/language/override)(C++11) - [`final`](https://en.cppreference.com/w/cpp/language/final)(C++11) |

## 基于class hierarchy来思考

OOP的内容非常多，以**结构化思维**来进行思考：基于class hierarchy来思考。

- initialization order（construction order）

- destruction order

- virtual function find order
## 关于本章

本章对C++的type conversion进行总结。

## cppreference [Conversions expression](https://en.cppreference.com/w/cpp/language/expressions#Conversions)

>  cppreference中，将type conversion放到了[Expressions](https://en.cppreference.com/w/cpp/language/expressions)章节中。

| conversion                                                   | 说明                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [standard conversions](https://en.cppreference.com/w/cpp/language/implicit_conversion) | implicit(隐式) conversions from one type to another          |
| [`const_cast` conversion](https://en.cppreference.com/w/cpp/language/const_cast) |                                                              |
| [`static_cast` conversion](https://en.cppreference.com/w/cpp/language/static_cast) |                                                              |
| [`dynamic_cast` conversion](https://en.cppreference.com/w/cpp/language/dynamic_cast) |                                                              |
| [`reinterpret_cast` conversion](https://en.cppreference.com/w/cpp/language/reinterpret_cast) |                                                              |
| [explicit cast](https://en.cppreference.com/w/cpp/language/explicit_cast) | conversion using C-style cast notation and functional notation |
| [user-defined conversion](https://en.cppreference.com/w/cpp/language/cast_operator) | makes it possible to specify conversion from user-defined classes |



## 分类

C++ 的type system提供了非常丰富的type conversion支持，下面是简单的分类，通过这些分类，能够帮助我们全面地理解type conversion。

### 根据implicit/explicit进行分类

转换方式是

| implicit/explicit                                            | 细分                                                         | 说明 |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ---- |
| [explicit cast](https://en.cppreference.com/w/cpp/language/explicit_cast) | - [static_cast](https://en.cppreference.com/w/cpp/language/static_cast) <br/>- [const_cast](https://en.cppreference.com/w/cpp/language/const_cast) <br/>- [dynamic_cast](https://en.cppreference.com/w/cpp/language/dynamic_cast) <br/>- [reinterpret_cast](https://en.cppreference.com/w/cpp/language/reinterpret_cast) <br/>- C-style cast |      |
| [implicit conversion](https://en.cppreference.com/w/cpp/language/implicit_cast) |                                                              |      |

与explicit相关的有:

- `explicit` specifier: `C++\Language-reference\Classes\Members\explicit-specifier.md`

### 根据built-in/user-defined进行分类

| built-in/user-defined   | 细分                                                         | 说明                                                         |
| ----------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| user defined conversion | - [user-defined conversion function](https://en.cppreference.com/w/cpp/language/cast_operator) <br>- [Converting constructor](https://en.cppreference.com/w/cpp/language/converting_constructor) | 既可以用于[explicit cast](https://en.cppreference.com/w/cpp/language/explicit_cast)，也可以用于[implicit conversion](https://en.cppreference.com/w/cpp/language/implicit_cast) |
| built-in                |                                                              | 基本上是[implicit conversions](https://en.cppreference.com/w/cpp/language/implicit_cast)中的一些conversion |





## Type conversion and overload resolution

阅读overload resolution可知，其中的涉及了非常多的type conversion内容，并且通过overload resolution的学习，可以进一步地深入对type conversion的认知。overload resolution在`C++\Language-reference\Functions\Overload-resolution`章节中描述的。

非常建议阅读accu [Overload Resolution - Selecting the Function](https://accu.org/journals/overload/13/66/kilpelainen_268/)，它对两者总结得非常好。
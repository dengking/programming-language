# Constant

本节对C++中constant进行总结。

## C++中的constant

本节总结C++中的constant: 

|                     | explanation                                                  | 章节                                                         |
| ------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| `const`             | cppreference [cv (const and volatile) type qualifiers](https://en.cppreference.com/w/cpp/language/cv): <br>type qualifier, defines that the type is *constant*. | `C++\Language-reference\Basic-concept\Type-system\Type-system\Type-qualifier` |
| enumerator          | cppreference [Enumeration declaration](https://en.cppreference.com/w/cpp/language/enum): <br>An *enumeration* may include several explicitly named constants ("*enumerators*"). | `C++\Language-reference\Enum`                                |
| constant expression | cppreference [Constant expressions](https://en.cppreference.com/w/cpp/language/constant_expression) | `C++\Language-reference\Expressions\Constant-expressions`    |

### Value category of constant

这个问题，还比较复杂，后续进行补充。



## Constant initialization

另外，一个与constant有关的是[Constant initialization](https://en.cppreference.com/w/cpp/language/constant_initialization)。



## TODO Constant and literal

literal type和constant之间的关系。



## TODO enum VS const VS constexpr



参考

https://isocpp.org/blog/2016/05/quick-q-static-constexpr-int-vs-old-fashioned-enum-when-and-why

https://stackoverflow.com/questions/22867654/enum-vs-constexpr-for-actual-static-constants-inside-classes


https://stackoverflow.com/questions/2636303/how-to-initialize-a-private-static-const-map-in-c



### enum VS constexpr

enum、constexpr都可以用于in-class static constant





## Application

主要用于compile time，参见`C-and-C++\Compile-time-and-run-time`，其中非常重要的一点是：static polymorphism。





## TODO

c++ 11 initialize a dict constexpr

https://stackoverflow.com/questions/28530278/undefined-reference-to-static-constexpr-char

https://devblogs.microsoft.com/cppblog/constexpr-and-aggregate-initialization/
# Constant

本节对C++中constant进行总结。

## C++中的constant

本节总结C++中的constant: 

|                     | explanation                                                  | 章节                                                         |
| ------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| `const`             | cppreference [cv (const and volatile) type qualifiers](https://en.cppreference.com/w/cpp/language/cv): <br>type qualifier, defines that the type is *constant*. | `C++\Language-reference\Basic-concept\Type-system\Type-system\Type-qualifier` |
| enumerator          | cppreference [Enumeration declaration](https://en.cppreference.com/w/cpp/language/enum): <br>An *enumeration* may include several explicitly named constants ("*enumerators*"). | `C++\Language-reference\Enum`                                |
| constant expression | cppreference [Constant expressions](https://en.cppreference.com/w/cpp/language/constant_expression) | `C++\Language-reference\Expressions\Constant-expressions`    |

## Constant initialization

另外，一个与constant有关的是[Constant initialization](https://en.cppreference.com/w/cpp/language/constant_initialization)。



## TODO Constant and literal





## TODO enum VS const VS constexpre

draft:

为每个trait class添加一个表示其类型的常量，然后添加判断条件来判断它属于哪种类型？


比如我定义有三种类型：

```
enum EServiceType
{
    
   TradeService=1;//交易类服务 
   QueryService=2;//查询类服务
};

constexpr bool IsTradeService(EServiceType ServiceType)
{
    return ServiceType == TradeService;
}
```
这些都需要在编译时完成

参考

https://isocpp.org/blog/2016/05/quick-q-static-constexpr-int-vs-old-fashioned-enum-when-and-why

https://stackoverflow.com/questions/22867654/enum-vs-constexpr-for-actual-static-constants-inside-classes




https://stackoverflow.com/questions/2636303/how-to-initialize-a-private-static-const-map-in-c





## Application

主要用于compile time，参见`C-and-C++\Compile-time-and-run-time`，其中非常重要的一点是：static polymorphism。





## TODO

c++ 11 initialize a dict constexpr

https://stackoverflow.com/questions/28530278/undefined-reference-to-static-constexpr-char

https://devblogs.microsoft.com/cppblog/constexpr-and-aggregate-initialization/
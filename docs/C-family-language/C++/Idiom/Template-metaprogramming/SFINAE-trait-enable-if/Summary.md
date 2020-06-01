# Summary

SFINAE、trait、enable_if，三者是实现template metaprogramming的基础，实现上，往往是三者一起使用才能够发挥出c++的强大之处。

对三者总结的最好的文章是：boost [enable_if](https://www.boost.org/doc/libs/1_73_0/libs/core/doc/html/core/enable_if.html)，它讲到了精髓，下面是我对三者的总结：

`enable_if`基于trait来对SFINAE进行控制。

这是c++ metaprogramming的精髓所在。

下面是一些典型的用法：

## Class template SFINAE

https://cpppatterns.com/patterns/class-template-sfinae.html

[Template specialization and enable_if problems [duplicate]](https://stackoverflow.com/questions/29502052/template-specialization-and-enable-if-problems)

## Function template SFINAE

[Using SFINAE to specialize template methods, depending on the type of container std::map or std::vector, passed as an argument](https://evileg.com/en/post/468/)



[Enable_if for template function specialization](https://stackoverflow.com/questions/48097889/enable-if-for-template-function-specialization)



[std::enable_if to conditionally compile a member function](https://stackoverflow.com/questions/6972368/stdenable-if-to-conditionally-compile-a-member-function)



### Example

```C++

#include <iostream>
#include <type_traits>

struct Stock
{
};
struct Derivative
{
};

/**
 *
 */
struct NoIndexMsg
{
	constexpr static int ServiceMsgType = 1;

};

/**
 *
 */
struct TradeMsg
{
	constexpr static int ServiceMsgType = 2;
};
/**
 *
 */
struct QueryMsg
{
	constexpr static int ServiceMsgType = 3;
};

/**
 * 主推类消息
 */
struct PushMsg
{
	constexpr static int ServiceMsgType = 4;
};

template<typename UstTag>
struct LoginServiceTrait: std::true_type, NoIndexMsg
{
	constexpr static int FuncNo = 1;
};

template<typename UstTag>
struct OrderInsertServiceTrait: std::true_type, TradeMsg
{
	constexpr static int FuncNo = 2;
};

/**
 * trade消息的请求
 * @param ReqField 请求字段
 * @return
 */
template<template<class > class ServiceTraitType, typename UstTag>
auto Req()->typename std::enable_if<ServiceTraitType<UstTag>::ServiceMsgType==NoIndexMsg::ServiceMsgType, int>::type
{
	std::cout<<ServiceTraitType<UstTag>::FuncNo<<std::endl;
	return 0;
}
/**
 * trade消息的请求
 * @param ReqField 请求字段
 * @return
 */
template<template<class > class ServiceTraitType, typename UstTag>
auto Req()->typename std::enable_if<ServiceTraitType<UstTag>::ServiceMsgType==TradeMsg::ServiceMsgType, int>::type
{
	std::cout<<ServiceTraitType<UstTag>::FuncNo<<std::endl;
	return 0;
}

int main()
{
	Req<OrderInsertServiceTrait, Stock>();
}

```



## TO READ

[Template SFINAE & type-traits](https://shaharmike.com/cpp/sfinae/)

 
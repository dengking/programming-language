# Class template SFINAE



## cppreference [sfinae](https://en.cppreference.com/w/cpp/language/sfinae) # SFINAE in partial specializations

Deduction and substitution also occur while determining whether a specialization of a class or variable (since C++14) template is generated by some [partial specialization](https://en.cppreference.com/w/cpp/language/partial_specialization) or the primary template. Compilers do not treat a substitution failure as a hard-error during such determination, but ignore the corresponding partial specialization declaration instead, as if in the overload resolution involving function templates.

```C++
#include <type_traits>
#include <iostream>
// primary template handles non-referenceable types:
template<class T, class = void>
struct reference_traits
{
	using add_lref = T;
	using add_rref = T;
};

// specialization recognizes referenceable types:
template<class T>
struct reference_traits<T, std::void_t<T&>>
{
	using add_lref = T&;
	using add_rref = T&&;
};

template<class T>
using add_lvalue_reference_t = typename reference_traits<T>::add_lref;

template<class T>
using add_rvalue_reference_t = typename reference_traits<T>::add_rref;
int main()
{
	std::cout << std::boolalpha << std::is_reference_v<add_lvalue_reference_t<void>> << std::endl;   // false;
	std::cout << std::boolalpha << std::is_reference_v<add_lvalue_reference_t<int>> << std::endl;   // true;
}
// g++   --std=c++17 -Wall -pedantic -pthread main.cpp && ./a.out

```

> NOTE: 
>
> 1、输出如下:
>
> ```c++
> false
> true
> ```
>
> 

## cpppatterns [Class template SFINAE](https://cpppatterns.com/patterns/class-template-sfinae.html)

```c++
#include <type_traits>
template <typename T, typename Enable = void>
class foo;
template <typename T>
class foo<T, typename std::enable_if<std::is_integral<T>::value>::type>
{ };
template <typename T>
class foo<T, typename std::enable_if<std::is_floating_point<T>::value>::type>
{ };
```

### Requires

[c++11](https://cpppatterns.com/#/search/c++11) or newer.

> NOTE: 需要c++11的原因是`type_traits`library是C++11中添加的

### INTENT

Conditionally instantiate a class template depending on the template arguments.

### DESCRIPTION

We provide two partial specializations of the `foo` class template:

1. The template on [lines 6–8](https://cpppatterns.com/patterns/class-template-sfinae.html#line6) will only be instantiated when `T` is an integral type.
2. The template on [lines 10–12](https://cpppatterns.com/patterns/class-template-sfinae.html#line10) will only be instantiated when `T` is a floating point type.

This allows us to provide different implementations of the `foo` class depending on the template arguments it is instantiated with.

We have used [`std::enable_if`](http://en.cppreference.com/w/cpp/types/enable_if) on [line 7](https://cpppatterns.com/patterns/class-template-sfinae.html#line7) and [line 11](https://cpppatterns.com/patterns/class-template-sfinae.html#line11) to force instantiation to succeed only for the appropriate template arguments. This relies on [Substitution Failure Is Not An Error](https://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error) (SFINAE), which states that failing to instantiate a template with some particular template arguments does not result in an error and simply discards that instantiation.

> NOTE: 如果对[`std::enable_if`](http://en.cppreference.com/w/cpp/types/enable_if)不熟悉，则可能会对上面这段话的解释产生质疑，我第一次就是如此。我以为`enable_if`为第二个template parameter `T`提供了default argument `void`，则在第一个template parameter `B`的argument为`false`的时候，则它提供的类型是`void`，则上述代码会与第一个class template匹配。这是错误的。阅读[`std::enable_if`](http://en.cppreference.com/w/cpp/types/enable_if)可知：
>
> If `B` is true, **std::enable_if** has a public member typedef `type`, equal to `T`; otherwise, there is no member typedef.
>
> 所以上述程序的实现所依赖的是SFINAE。

If you want to simply prevent a template from being instantiated for certain template arguments, consider using [`static_assert`](http://en.cppreference.com/w/cpp/language/static_assert) instead.

[Template specialization and enable_if problems [duplicate]](https://stackoverflow.com/questions/29502052/template-specialization-and-enable-if-problems)



## 实现分析

1、SFINAE作用于argument of type template parameter。

2、它是基于partial template specialization



## 其他的实现模式

### [jni.hpp](https://github.com/mapbox/jni.hpp)/[include](https://github.com/mapbox/jni.hpp/tree/master/include)/[jni](https://github.com/mapbox/jni.hpp/tree/master/include/jni)/[tagging.hpp](https://github.com/mapbox/jni.hpp/blob/master/include/jni/tagging.hpp)

```C++
template<class Tag, class = int>
struct SuperTag
{
	using Type = ObjectTag;
};

template<class Tag>
struct SuperTag<Tag, decltype(std::declval<typename Tag::SuperTag>(), 0)>
{
	using Type = typename Tag::SuperTag;
};

```

### [ricab](https://github.com/ricab)/**[scope_guard](https://github.com/ricab/scope_guard)**/[scope_guard.hpp](https://github.com/ricab/scope_guard/blob/master/scope_guard.hpp)

```C++
    // Type trait determining whether a type is callable with no arguments
    template<typename T, typename = void>
    struct is_noarg_callable_t
      : public std::false_type
    {}; // in general, false

    template<typename T>
    struct is_noarg_callable_t<T, decltype(std::declval<T&&>()())>
      : public std::true_type
    {}; // only true when call expression valid
```



## Application

1、Class template SFINAE是实现trait的基础。



## 我的实践

```C++
/**
 * 根据消息类型获得对应的序列化器、反序列化器 类型
 */
template<int ServiceMsgType, typename ReqFieldType, typename Enable = void>
struct MsgPackerTrait;

/**
 * 根据消息类型获得对应的序列化器、反序列化器 类型
 */
template<int ServiceMsgType, typename RspFieldType, typename Enable = void>
struct MsgUnpackerTrait;

template<int ServiceMsgType, typename ReqFieldType>
struct MsgPackerTrait<ServiceMsgType, ReqFieldType, typename std::enable_if<IsTradeMsg(ServiceMsgType)>::type>
{
	typedef CNoBizHeadMsgPacker<ReqFieldType> PackerType;

};

template<int ServiceMsgType, typename RspFieldType>
struct MsgUnpackerTrait<ServiceMsgType, RspFieldType, typename std::enable_if<IsTradeMsg(ServiceMsgType)>::type>
{

	typedef CNoBizHeadMsgUnpacker<RspFieldType> UnpackerType;
};

```


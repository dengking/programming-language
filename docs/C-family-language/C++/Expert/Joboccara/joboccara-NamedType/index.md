# [joboccara](https://github.com/joboccara)/**[NamedType](https://github.com/joboccara/NamedType)**

> NOTE: 
>
> 在阅读 fluentcpp [Variadic CRTP: An Opt-in for Class Features, at Compile Time](https://www.fluentcpp.com/2018/06/22/variadic-crtp-opt-in-for-class-features-at-compile-time/) 时，发现的它；

A **strong type** is a type used in place of another type to carry specific **meaning** through its **name**.

> NOTE: 对strong type的解释是非常准确的

```C++
using Meter = NamedType<double, MeterTag, Addable, Printable>
```

## embeddedartistry [NamedType: The Easy Way to Use Strong Types in C++](https://embeddedartistry.com/blog/2018/04/23/namedtype-the-easy-way-to-use-strong-types-in-c/)



## Read code

> NOTE: 它的实现点有:
>
> 一、CRTP
>
> 二、用了两次CRTP:
>
> 1、[NamedType](https://github.com/joboccara/NamedType)/[include](https://github.com/joboccara/NamedType/tree/master/include)/[NamedType](https://github.com/joboccara/NamedType/tree/master/include/NamedType)/[crtp.hpp](https://github.com/joboccara/NamedType/blob/master/include/NamedType/crtp.hpp)
>
> 每个skill继承它来获得 `underlying()` 接口
>
> 2、[NamedType](https://github.com/joboccara/NamedType)/[include](https://github.com/joboccara/NamedType/tree/master/include)/[NamedType](https://github.com/joboccara/NamedType/tree/master/include/NamedType)/[**named_type_impl.hpp**](https://github.com/joboccara/NamedType/blob/master/include/NamedType/named_type_impl.hpp) `NamedType` 
>
> 可以继承多个skill来获得skill。
>
> 三、policy based design
>
> 四、mixin from above

### [NamedType](https://github.com/joboccara/NamedType)/[include](https://github.com/joboccara/NamedType/tree/master/include)/[NamedType](https://github.com/joboccara/NamedType/tree/master/include/NamedType)/[**named_type_impl.hpp**](https://github.com/joboccara/NamedType/blob/master/include/NamedType/named_type_impl.hpp)

```C++
template <typename T, typename Parameter, template <typename> class... Skills>
class FLUENT_EBCO NamedType : public Skills<NamedType<T, Parameter, Skills...>>...
{

};
```

> NOTE: 
>
> 
>
> `using Meter = NamedType<double, MeterTag, Addable, Printable>`
>
> `Skill`只有一个template parameter，它的template parameter是一个template，这个template接收一个template parameter: `NamedType`
>
> 



### [NamedType](https://github.com/joboccara/NamedType)/[include](https://github.com/joboccara/NamedType/tree/master/include)/[NamedType](https://github.com/joboccara/NamedType/tree/master/include/NamedType)/[crtp.hpp](https://github.com/joboccara/NamedType/blob/master/include/NamedType/crtp.hpp)

```C++
#ifndef CRTP_HPP
#define CRTP_HPP

namespace fluent
{

template <typename T, template <typename> class crtpType>
struct crtp
{
    constexpr T& underlying()
    {
        return static_cast<T&>(*this);
    }
    constexpr T const& underlying() const
    {
        return static_cast<T const&>(*this);
    }
};

} // namespace fluent

#endif-
```

> NOTE: 思考: template parameter `crtpType` 有什么用？

### [NamedType](https://github.com/joboccara/NamedType)/[include](https://github.com/joboccara/NamedType/tree/master/include)/[NamedType](https://github.com/joboccara/NamedType/tree/master/include/NamedType)/[**underlying_functionalities.hpp**](https://github.com/joboccara/NamedType/blob/master/include/NamedType/underlying_functionalities.hpp)



```C++
namespace fluent
{

template <typename T>
struct PreIncrementable : crtp<T, PreIncrementable>
{
    IGNORE_SHOULD_RETURN_REFERENCE_TO_THIS_BEGIN

    FLUENT_CONSTEXPR17 T& operator++()
    {
        ++this->underlying().get();
        return this->underlying();
    }

    IGNORE_SHOULD_RETURN_REFERENCE_TO_THIS_END
};

}
```


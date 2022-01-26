# Type traits (since C++11)

## cppreference [Type support # Type traits (since C++11)](https://pd.codechef.com/docs/cpp/cpp/types.html)

> NOTE: 关于trait，参见`C++\Idiom\Generic-programming\SFINAE-trait-enable-if`。
> 
>这些type trait的实现可以作为学习generic programming的素材；

Type traits defines a compile-time template-based interface to **query** or **modify** the properties of types.

Attempting to specialize a template defined in the `<type_traits>` header results in undefined behavior, except that [std::common_type](https://en.cppreference.com/w/cpp/types/common_type) may be specialized [as described in its description](https://en.cppreference.com/w/cpp/types/common_type#Specializations).

A template defined in the `<type_traits>` header may be instantiated with an **incomplete type** unless otherwise specified, notwithstanding(虽然) the general prohibition(禁止) against instantiating standard library templates with incomplete types.

> NOTE: 
>
> 一、翻译如下:
>
> "<type_traits>头文件中定义的模板可以用不完整类型实例化，除非另有说明，尽管一般禁止用不完整类型实例化标准库模板。"



### Type properties

> NOTE: 可以看到这些Type properties API都是以 `is_`打头的，显然它们都是提供给programmer进行查询的(参见`C++\Language-reference\Basic-concept\Type-system\Type-operation`)，它们基于了programmer static reflection的能力(`C++\Language-reference\Basic-concept\Type-system\Type-operation\Reflection`)。
>
> property包括:
>
> 1) Type category
>
> 2) Type propertity
>
> 3) Supported operations



### Type modifications

> NOTE: type modification API是进行generic programming的重要功能；
>
> 从原文所总结的API来看，C++ standard library提供了add、remove一对相反的操作。

Type modification templates create new type definitions by applying modifications on a template parameter. The resulting type can then be accessed through `type` member typedef.




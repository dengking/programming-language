# [cppreference Metaprogramming library (since C++11) # Type traits](https://en.cppreference.com/w/cpp/meta) 

[Standard library header <type_traits> (C++11)](https://en.cppreference.com/w/cpp/header/type_traits) 



> NOTE: 关于trait，参见`C++\Idiom\Generic-programming\SFINAE-trait-enable-if`。
> 
>这些type trait的实现可以作为学习generic programming的素材；

Type traits define compile-time template-based interfaces to **query** the properties of types.

Attempting to specialize a template defined in the `<type_traits>` header results in **undefined behavior**, except that the following may be specialized [as described in its description](https://en.cppreference.com/w/cpp/types/common_type#Specializations):

- [std::common_type](https://en.cppreference.com/w/cpp/types/common_type) 
- [`std::basic_common_reference`](https://en.cppreference.com/w/cpp/types/common_reference)(since C++20) 



A template defined in the `<type_traits>` header may be instantiated with an **incomplete type** unless otherwise specified, notwithstanding(虽然) the general prohibition(禁止) against instantiating standard library templates with incomplete types.

> NOTE: 
>
> 一、翻译如下:
>
> "<type_traits>头文件中定义的模板可以用不完整类型实例化，除非另有说明，尽管一般禁止用不完整类型实例化标准库模板。"



### Base classes





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



# type trait



#### Base classes

#### Unary type traits([*UnaryTypeTrait*](https://en.cppreference.com/w/cpp/named_req/UnaryTypeTrait))

**Unary type traits** can be used to query the boolean properties of a type at compile time.

All these type traits satisfy [*UnaryTypeTrait*](https://en.cppreference.com/w/cpp/named_req/UnaryTypeTrait), the base characteristic of each type trait is either [std::true_type](https://en.cppreference.com/w/cpp/types/integral_constant) or [std::false_type](https://en.cppreference.com/w/cpp/types/integral_constant), depending on whether the corresponding condition is met.



##### Primary type categories

##### Composite type categories

##### Type properties

##### Supported operations

#### Property queries(查询属性,  [*UnaryTypeTrait*](https://en.cppreference.com/w/cpp/named_req/UnaryTypeTrait))

**Property query traits** can be used to query the integral(完整) properties of a type at compile time.

All these type traits satisfy [*UnaryTypeTrait*](https://en.cppreference.com/w/cpp/named_req/UnaryTypeTrait), the base characteristic of each type trait is [std::integral_constant](http://en.cppreference.com/w/cpp/types/integral_constant)<[std::size_t](http://en.cppreference.com/w/cpp/types/size_t), Value>, where `Value` is the query result of the corresponding property.



#### Type relationships([*BinaryTypeTrait*](https://en.cppreference.com/w/cpp/named_req/BinaryTypeTrait))

Type relationship traits can be used to query relationships between types at compile time.

All these type traits satisfy [*BinaryTypeTrait*](https://en.cppreference.com/w/cpp/named_req/BinaryTypeTrait), the base characteristic of each type trait is either [std::true_type](https://en.cppreference.com/w/cpp/types/integral_constant) or [std::false_type](https://en.cppreference.com/w/cpp/types/integral_constant), depending on whether the corresponding condition is met.



#### Type transformations([*TransformationTrait*](https://en.cppreference.com/w/cpp/named_req/TransformationTrait))

Type transformation traits transform one type to another following some predefined rules.

All these type traits satisfy [*TransformationTrait*](https://en.cppreference.com/w/cpp/named_req/TransformationTrait).

Const-volatility specifiers



#### Logical operations



#### Member relationships



#### Miscellaneous transformations

> NOTE: 这些transformation是非常有用的；

#### Operations on traits

> NOTE: variadic logical AND/OR/NOT metafunction


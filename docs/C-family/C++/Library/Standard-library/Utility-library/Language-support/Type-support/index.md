# 关于本章

本章描述C++的 [Type support (basic types, RTTI, type traits)](https://en.cppreference.com/w/cpp/types)，查看原文可知，C++ type support library提供了丰富的API，我们首先需要建立高屋建瓴的视角来全面了解这些API，然后逐个深入。

## cppreference [Type support (basic types, RTTI, type traits)](https://en.cppreference.com/w/cpp/types) 

> NOTE: 概述:
>
> | 章节                        | 说明    |
> | --------------------------- | ------- |
> | Basic types                 |         |
> | Runtime type identification | dynamic |
> | Type traits                 | static  |
>
> 



### Basic types

#### [Fundamental types defined by the language](https://en.cppreference.com/w/cpp/language/types)

> NOTE: C++语言内置的type，可以直接使用，不需要使用`std::`。

#### Additional basic types and macros

> NOTE: C++语言定义的类型，这些类型非常重要，往往是以standard library的方式提供，需要使用`std::`



#### [Fixed width integer types](https://en.cppreference.com/w/cpp/types/integer) (since C++11)

> NOTE: 这是C++11新增的，对fixed width integer type进行了统一。

### Numeric limits

|                                                              | 简介                |
| ------------------------------------------------------------ | ------------------- |
| [numeric_limits](https://en.cppreference.com/w/cpp/types/numeric_limits) | C++ trait 风格的API |
| [C numeric limits interface](https://en.cppreference.com/w/cpp/types/climits) | 传统C风格的API      |



### Runtime type identification

> NOTE: RTTI特性

### Type traits (since C++11)

> NOTE: 关于trait，参见`C++\Idiom\Generic-programming\SFINAE-trait-enable-if`。
>
> 这些type trait的实现可以作为学习generic programming的素材；

Type traits defines a compile-time template-based interface to **query** or **modify** the properties of types.

Attempting to specialize a template defined in the `<type_traits>` header results in undefined behavior, except that [std::common_type](https://en.cppreference.com/w/cpp/types/common_type) may be specialized [as described in its description](https://en.cppreference.com/w/cpp/types/common_type#Specializations).

A template defined in the `<type_traits>` header may be instantiated with an **incomplete type** unless otherwise specified, notwithstanding(虽然) the general prohibition(禁止) against instantiating standard library templates with incomplete types.

> NOTE: 没有读懂



#### Type properties

> NOTE: 可以看到这些Type properties API都是以 `is_`打头的，显然它们都是提供给programmer进行查询的(参见`C++\Language-reference\Basic-concept\Type-system\Type-operation`)，它们基于了programmer static reflection的能力(`C++\Language-reference\Basic-concept\Type-system\Type-operation\Reflection`)。
>
> 
>
> property包括:
>
> 1) Type category
>
> 2) Type propertity
>
> 3) Supported operations

##### Primary type categories

> NOTE: reflect/query type category

##### Composite type categories

> NOTE: NOTE: reflect/query type category

##### Type properties

> NOTE: reflect/query type property

##### Supported operations

> NOTE: reflect/query operation supportted by the type

##### Property queries



##### Type relationships

> NOTE: 非常重要



#### Type modifications

> NOTE: type modification API是进行generic programming的重要功能；
>
> 从原文所总结的API来看，C++ standard library提供了add、remove一对相反的操作。

Type modification templates create new type definitions by applying modifications on a template parameter. The resulting type can then be accessed through `type` member typedef.

##### Const-volatility specifiers

##### References

##### Pointers

##### Sign modifiers

##### Arrays



#### Miscellaneous transformations

> NOTE: 这些transformation是非常有用的；

#### Operations on traits

> NOTE: variadic logical AND/OR/NOT metafunction

#### Helper classes

### Constant evaluation context (since C++20)

Detects whether the call occurs within a constant-evaluated context;
# `void_t`



## cppreference [`std::void_t`](https://en.cppreference.com/w/cpp/types/void_t) (since C++17)



```c++
template< class... >
using void_t = void;
```

> NOTE:  需要注意的是，上面使用的是[**alias template**](https://en.cppreference.com/w/cpp/language/type_alias) 

Utility **metafunction** that maps a sequence of any types to the type void. This metafunction is a convenient way to leverage [SFINAE](https://en.cppreference.com/w/cpp/language/sfinae) prior to C++20's [concepts](https://en.cppreference.com/w/cpp/language/constraints), in particular for conditionally removing functions from the [candidate set](https://en.cppreference.com/w/cpp/language/overload_resolution) based on whether an expression is valid in the [unevaluated context](https://en.cppreference.com/w/cpp/language/expressions#Unevaluated_expressions) (such as operand to [`decltype`](https://en.cppreference.com/w/cpp/language/decltype) expression), allowing to exist(存在) separate function overloads or specializations based on supported operations.

> NOTE: 最后一句话的含义"允许基于支持的操作存在单独的函数重载或特化"



## Application&Example

> NOTE: 
>
> 本段内容基于cppreference [`std::void_t`](https://en.cppreference.com/w/cpp/types/void_t) (since C++17)，并进行了一定的扩充。
>
> 用法总结:
>
> riptutorial [C++ `void_t`](https://riptutorial.com/cplusplus/example/3778/void-t) 中给出了很好的总结: "The primary application of `void_t` is writing **type traits** that check **validity** of a statement"，显然通过"check **validity** of a statement"，我们可以实现判定是否存在member function、member type、type是否支持指定的操作，显然这是一种compile time的introspection(自省)，但是可以想见: 它能够支持的statement是有限的。显然这种用法是基于SFINAE，正如cppreference [`std::void_t`](https://en.cppreference.com/w/cpp/types/void_t) (since C++17)中所述"This metafunction is used in template metaprogramming to detect ill-formed types in SFINAE context"，从下面的例子可以看出在使用  [`std::void_t`](https://en.cppreference.com/w/cpp/types/void_t) 的时候，它是基于primary template和template specialization的，它使用primary template来作为fallback。
>

This metafunction is used in template metaprogramming to detect ill-formed types in SFINAE context:



### Member detection idiom

member detection idiom，参见`C++\Language-reference\Template\Programming-paradigm\Idioms\Detection`章节。

#### Detect member type 

```C++
#include <type_traits>

// primary template handles types that have no nested ::type member:
template<class, class = void>
struct has_type_member : std::false_type {
};

// specialization recognizes types that do have a nested ::type member:
template<class T>
struct has_type_member<T, std::void_t<typename T::type>> : std::true_type {
};
```



#### Detect member function

在 riptutorial [C++ `void_t`](https://riptutorial.com/cplusplus/example/3778/void-t) 中也给出了一些详细的例子: 

> The primary application of `void_t` is writing type traits that check **validity** of a statement. 

For example, let's check if a type has a member function `foo()` that takes no arguments:

```c++
#include <utility> // std::declval
#include <type_trait> // std::false_type

using namespace std;
template<class T, class = void>
struct has_foo : std::false_type {
};

template<class T>
struct has_foo<T, void_t<decltype(std::declval<T &>().foo())>> : std::true_type {
};
```



### Detect validity of an expression

It can also be used to detect validity of an expression:

```C++
#include <type_traits>

// primary template handles types that do not support pre-increment:
template<class, class = void>
struct has_pre_increment_member : std::false_type {
};
// specialization recognizes types that do support pre-increment:
template<class T>
struct has_pre_increment_member<T,
        std::void_t<decltype(++std::declval<T &>())>
> : std::true_type {
};

```

### Example

```C++
#include <iostream>
#include <type_traits>
#include <vector>
#include <map>

template<typename ...Ts>
struct make_void {
    using type = void;
};
template<typename ...Ts>
using void_t = typename make_void<Ts...>::type;

class A {
};

template<typename T, typename = void>
struct is_iterable : std::false_type {
};

template<typename T>
struct is_iterable<T, void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end())>> : std::true_type {
};

// An iterator trait which value_type is always the value_type of the
// iterated container, even with back_insert_iterator which value_type is void
template<typename T, typename = void>
struct iterator_trait : std::iterator_traits<T> {
};

template<typename T>
struct iterator_trait<T, void_t<typename T::container_type>>
        : std::iterator_traits<typename T::container_type::iterator> {
};

int main() {
    std::cout << std::boolalpha;
    std::cout << is_iterable<std::vector<double>>::value << '\n';
    std::cout << is_iterable<std::map<int, double>>::value << '\n';
    std::cout << is_iterable<double>::value << '\n';
    std::cout << is_iterable<A>::value << '\n';

    std::vector<int> v;

    std::cout << std::is_same<
            iterator_trait<decltype(std::back_inserter(v))>::value_type,
            iterator_trait<decltype(v.cbegin())>::value_type
    >::value << '\n';
}
// g++ --std=c++11 test.cpp

```

> NOTE: 
>
> 1、输出如下:
>
> ```C++
> true
> true
> false
> false
> true
> 
> ```
>
> 关于上述程序中`decltype`、`std::declval`的分析，参见`C++\Language-reference\Basic-concept\Type-system\Type-inference\decltype`章节。
>
> 2、上述code是为了detect type `T`是否有method `begin`、method `end`，由于没有object，只有type，所以需要使用`std::declva(T)()`。



## Implementation

> NOTE: 本段内容基于cppreference [`std::void_t`](https://en.cppreference.com/w/cpp/types/void_t) (since C++17)，并进行了一定的扩充。

Until [CWG 1558](https://wg21.cmeerw.net/cwg/issue1558) (a C++11 defect), unused parameters in [alias templates](https://en.cppreference.com/w/cpp/language/type_alias) were not guaranteed to ensure SFINAE and could be ignored, so earlier compilers require a more complex definition of `void_t`, such as

> NOTE: 由于这个原因，所以在C++11中，使用下面的"C++14 variable template"段中的实现方式是无法生效的。

```C++
template<typename... Ts> struct make_void { typedef void type;};
template<typename... Ts> using void_t = typename make_void<Ts...>::type;
```

关于C++17 `std::void_t`的实现，参见：

1) stackoverflow [How does `void_t` work](https://stackoverflow.com/questions/27687389/how-does-void-t-work)，在`C++\Language-reference\Template\Implementation\index.md`中收录了这篇文章

2) riptutorial [C++ `void_t`](https://riptutorial.com/cplusplus/example/3778/void-t) ，在`C++\Language-reference\Template\Implementation\index.md`中收录了这篇文章



### C++11 implementation

nlohmann [`void_t.hpp`](https://github.com/nlohmann/json/blob/develop/include/nlohmann/detail/meta/void_t.hpp) 

```C++
#pragma once

namespace nlohmann {
    namespace detail {
        template<typename ...Ts>
        struct make_void {
            using type = void;
        };
        template<typename ...Ts> using void_t = typename make_void<Ts...>::type;
    } // namespace detail
}  // namespace nlohmann

```



### C++17 [alias templates](https://en.cppreference.com/w/cpp/language/type_alias) 

```C++
template< class... >
using void_t = void;
```

> NOTE: 需要注意的是，这种实现方式在C++11中是无法生效的，具体原因参见本节第一段。



## TO READ

提及`void_t`的文章：

- https://foonathan.net/2015/11/overload-resolution-4/
- https://www.bfilipek.com/2016/02/notes-on-c-sfinae.html#voidt


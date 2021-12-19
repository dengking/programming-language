# Type list



## `DropTypeListItem`

这个meta function的功能是从给定的`TypeList`中，drop掉前 n 个 list item:

```C++
// A type-level function that drops |n| list item from given TypeList.
template <size_t n, typename List>
using DropTypeListItem = typename DropTypeListItemImpl<n, List>::Type;
```

一、`List`的类型是`TypeList`，即: 

```C++
template <typename... Types>
struct TypeList
```

二、`DropTypeListItemImpl<n, List>` 首先匹配的是:

```C++
template <size_t n, typename T, typename... List>
struct DropTypeListItemImpl<n, TypeList<T, List...>>
    : DropTypeListItemImpl<n - 1, TypeList<List...>>
{
};
```

显然，它继承自 `DropTypeListItemImpl<n - 1, TypeList<List...>>` ，表明它剥离了 `T`，上述过程一直递归，直至 n 为0，此时就触达了base condition，此时就得到了目标的类型，总的来说，它的实现过程是逐个剥离直至base condition:

```C++
template <typename T, typename... List>
struct DropTypeListItemImpl<0, TypeList<T, List...>>
{
    using Type = TypeList<T, List...>;
};

template <>
struct DropTypeListItemImpl<0, TypeList<>>
{
    using Type = TypeList<>;
};
```

### 完整测试代码

```C++
#include <iostream>
#include <utility>
#include <typeinfo>
// Packs a list of types to hold them in a single type.
template <typename... Types>
struct TypeList
{
    static constexpr std::size_t size() noexcept
    {
        return sizeof...(Types);
    }
    static void print()
    {
        std::cout << size() << std::endl;
        ((std::cout << typeid(Types).name() << ' '), ...);
        std::cout << std::endl;
    }
};

// Used for DropTypeListItem implementation.
template <size_t n, typename List>
struct DropTypeListItemImpl;

// Do not use enable_if and SFINAE here to avoid MSVC2013 compile failure.
template <size_t n, typename T, typename... List>
struct DropTypeListItemImpl<n, TypeList<T, List...>>
    : DropTypeListItemImpl<n - 1, TypeList<List...>>
{
};

template <typename T, typename... List>
struct DropTypeListItemImpl<0, TypeList<T, List...>>
{
    using Type = TypeList<T, List...>;
};

template <>
struct DropTypeListItemImpl<0, TypeList<>>
{
    using Type = TypeList<>;
};

// A type-level function that drops |n| list item from given TypeList.
template <size_t n, typename List>
using DropTypeListItem = typename DropTypeListItemImpl<n, List>::Type;

int main()
{
    using T1 = TypeList<int, float, double, char>;
    T1::print();
    using T2 = DropTypeListItem<1, T1>;
    T2::print();
}
//g++ test.cpp --std=c++17 -pedantic -Wall -Wextra

```

输出如下:

```
4
i f d c
3
f d c
```



## `TakeTypeListItem`

这个meta function的功能是从给定的`TypeList`中，截取掉前 n 个 list item(显然它和`DropTypeListItem`的功能，某种程度上是相反的):

```C++
// A type-level function that takes first |n| list item from given TypeList.
// E.g. TakeTypeListItem<3, TypeList<A, B, C, D>> is evaluated to
// TypeList<A, B, C>.
template <size_t n, typename List>
using TakeTypeListItem = typename TakeTypeListItemImpl<n, List>::Type;
```

`TakeTypeListItemImpl` 首先匹配的是:

```C++
// Do not use enable_if and SFINAE here to avoid MSVC2013 compile failure.
template <size_t n, typename T, typename... List, typename... Accum>
struct TakeTypeListItemImpl<n, TypeList<T, List...>, Accum...>
    : TakeTypeListItemImpl<n - 1, TypeList<List...>, Accum..., T>
{
};
```

显然，它继承自 TakeTypeListItemImpl<n - 1, TypeList<List...>, Accum..., T> ，它会从 `List` 中剥离出 `T`，然后将 `T` 放到 `Accum` (它是accumulate的缩写)中； 上述过程一直递归，直至 n 为0，此时就触达了base condition，此时就得到了目标的类型，总的来说，它的实现过程是逐个剥离并逐步添加直至base condition:

```C++
template <typename T, typename... List, typename... Accum>
struct TakeTypeListItemImpl<0, TypeList<T, List...>, Accum...>
{
    using Type = TypeList<Accum...>;
};

template <typename... Accum>
struct TakeTypeListItemImpl<0, TypeList<>, Accum...>
{
    using Type = TypeList<Accum...>;
};
```

### 完整测试代码

```C++
#include <iostream>
#include <utility>
#include <typeinfo>
// Packs a list of types to hold them in a single type.
template <typename... Types>
struct TypeList
{
    static constexpr std::size_t size() noexcept
    {
        return sizeof...(Types);
    }
    static void print()
    {
        std::cout << size() << std::endl;
        ((std::cout << typeid(Types).name() << ' '), ...);
        std::cout << std::endl;
    }
};

// Used for TakeTypeListItem implementation.
template <size_t n, typename List, typename... Accum>
struct TakeTypeListItemImpl;

// Do not use enable_if and SFINAE here to avoid MSVC2013 compile failure.
template <size_t n, typename T, typename... List, typename... Accum>
struct TakeTypeListItemImpl<n, TypeList<T, List...>, Accum...>
    : TakeTypeListItemImpl<n - 1, TypeList<List...>, Accum..., T>
{
};

template <typename T, typename... List, typename... Accum>
struct TakeTypeListItemImpl<0, TypeList<T, List...>, Accum...>
{
    using Type = TypeList<Accum...>;
};

template <typename... Accum>
struct TakeTypeListItemImpl<0, TypeList<>, Accum...>
{
    using Type = TypeList<Accum...>;
};

// A type-level function that takes first |n| list item from given TypeList.
// E.g. TakeTypeListItem<3, TypeList<A, B, C, D>> is evaluated to
// TypeList<A, B, C>.
template <size_t n, typename List>
using TakeTypeListItem = typename TakeTypeListItemImpl<n, List>::Type;

int main()
{
    using T1 = TypeList<int, float, double, char>;
    T1::print();
    using T2 = TakeTypeListItem<1, T1>;
    T2::print();
}
// g++ test.cpp --std=c++17 -pedantic -Wall -Wextra

```

输出如下:

```
4
i f d c
1
i
```



## `ConcatTypeLists`

这个的实现是最简单的。

### 完整测试代码

```C++
#include <iostream>
#include <utility>
#include <typeinfo>
// Packs a list of types to hold them in a single type.
template <typename... Types>
struct TypeList
{
    static constexpr std::size_t size() noexcept
    {
        return sizeof...(Types);
    }
    static void print()
    {
        std::cout << size() << std::endl;
        ((std::cout << typeid(Types).name() << ' '), ...);
        std::cout << std::endl;
    }
};

// Used for ConcatTypeLists implementation.
template <typename List1, typename List2>
struct ConcatTypeListsImpl;

template <typename... Types1, typename... Types2>
struct ConcatTypeListsImpl<TypeList<Types1...>, TypeList<Types2...>>
{
    using Type = TypeList<Types1..., Types2...>;
};

// A type-level function that concats two TypeLists.
template <typename List1, typename List2>
using ConcatTypeLists = typename ConcatTypeListsImpl<List1, List2>::Type;

int main()
{
    using T1 = TypeList<int, float, double, char>;
    T1::print();
    using T2 = TypeList<double, char>;
    T2::print();
    using T3 = ConcatTypeLists<T1, T2>;
    T3::print();
}
// g++ test.cpp --std=c++17 -pedantic -Wall -Wextra

```


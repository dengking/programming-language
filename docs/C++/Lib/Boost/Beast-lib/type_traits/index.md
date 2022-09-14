# `type_trait`



## max

```C++
template<class U>
std::size_t constexpr
max_sizeof()
{
    return sizeof(U);
}

template<class U0, class U1, class... Us>
std::size_t constexpr
max_sizeof()
{
    return
        max_sizeof<U0>() > max_sizeof<U1, Us...>() ?
        max_sizeof<U0>() : max_sizeof<U1, Us...>();
}

template<class U>
std::size_t constexpr
max_alignof()
{
    return alignof(U);
}

template<class U0, class U1, class... Us>
std::size_t constexpr
max_alignof()
{
    return
        max_alignof<U0>() > max_alignof<U1, Us...>() ?
        max_alignof<U0>() : max_alignof<U1, Us...>();
}
```



## `make_void` and `void_t`

```c++
// (since C++17)
template<class... Ts>
using make_void = boost::make_void<Ts...>;
template<class... Ts>
using void_t = boost::void_t<Ts...>;
```



## `aligned_union`

```C++
// (since C++11) missing from g++4.8
template<std::size_t Len, class ... Ts>
struct aligned_union
{
	static std::size_t constexpr alignment_value = max_alignof<Ts...>();

	using type = typename std::aligned_storage<
	(Len > max_sizeof<Ts...>()) ? Len : (max_sizeof<Ts...>()),
	alignment_value>::type;
};

template<std::size_t Len, class ... Ts>
using aligned_union_t =
typename aligned_union<Len, Ts...>::type;

```





## `is_contiguous_container`

```C++
// for span
template<class T, class E, class = void>
struct is_contiguous_container: std::false_type {};

template<class T, class E>
struct is_contiguous_container<T, E, void_t<
    decltype(
        std::declval<std::size_t&>() = std::declval<T const&>().size(),
        std::declval<E*&>() = std::declval<T&>().data()),
    typename std::enable_if<
        std::is_same<
            typename std::remove_cv<E>::type,
            typename std::remove_cv<
                typename std::remove_pointer<
                    decltype(std::declval<T&>().data())
                >::type
            >::type
        >::value
    >::type>>: std::true_type
{};
```

span中的用法:

https://www.boost.org/doc/libs/master/boost/beast/core/span.hpp

```C++
    /** Assignment

        @param container The container to assign from
    */
    template<class ContiguousContainer>
#if BOOST_BEAST_DOXYGEN
    span&
#else
    typename std::enable_if<detail::is_contiguous_container<
        ContiguousContainer, T>::value,
    span&>::type
#endif
    operator=(ContiguousContainer&& container)
    {
        data_ = container.data();
        size_ = container.size();
        return *this;
    }
```

## `launder_cast`

```C++
template <class T, class U>
T launder_cast(U* u)
{
#if defined(__cpp_lib_launder) && __cpp_lib_launder >= 201606
    return std::launder(reinterpret_cast<T>(u));
#elif defined(BOOST_GCC) && BOOST_GCC_VERSION > 80000
    return __builtin_launder(reinterpret_cast<T>(u));
#else
    return reinterpret_cast<T>(u);
#endif
}
```


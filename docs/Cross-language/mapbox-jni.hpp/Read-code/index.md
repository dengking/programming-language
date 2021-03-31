# Read code



## [jni.hpp](https://github.com/mapbox/jni.hpp)/[include](https://github.com/mapbox/jni.hpp/tree/master/include)/[jni](https://github.com/mapbox/jni.hpp/tree/master/include/jni)/[**tagging.hpp**](https://github.com/mapbox/jni.hpp/blob/master/include/jni/tagging.hpp)



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

上述实现:

1、class SFINAE

2、detection idiom: expression SFINAE: `decltype`+ `std::declval`

3、`decltype(std::declval<typename Tag::SuperTag>(), 0)` 中的 comma 是 "built-in comma operator"，它的return value是 `0`，所以 `decltype(std::declval<typename Tag::SuperTag>(), 0)` 取的是 `0` 的 type，即 `int`

## [jni.hpp](https://github.com/mapbox/jni.hpp)/[include](https://github.com/mapbox/jni.hpp/tree/master/include)/[jni](https://github.com/mapbox/jni.hpp/tree/master/include/jni)/[**traits.hpp**](https://github.com/mapbox/jni.hpp/blob/master/include/jni/traits.hpp)



```C++
#include<type_traits>
template<class ... > struct Conjunction: std::true_type
{
};
template<class B1> struct Conjunction<B1> : B1
{
};
template<class B1, class ... Bn> struct Conjunction<B1, Bn...> : std::conditional_t<bool(B1::value), Conjunction<Bn...>, B1>
{
};

```

上述实现:

1、recursive class template
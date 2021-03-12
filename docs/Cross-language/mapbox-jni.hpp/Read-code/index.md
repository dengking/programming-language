# Read code



## [jni.hpp](https://github.com/mapbox/jni.hpp)/[include](https://github.com/mapbox/jni.hpp/tree/master/include)/[jni](https://github.com/mapbox/jni.hpp/tree/master/include/jni)/[**tagging.hpp**](https://github.com/mapbox/jni.hpp/blob/master/include/jni/tagging.hpp)



## [jni.hpp](https://github.com/mapbox/jni.hpp)/[include](https://github.com/mapbox/jni.hpp/tree/master/include)/[jni](https://github.com/mapbox/jni.hpp/tree/master/include/jni)/[**traits.hpp**](https://github.com/mapbox/jni.hpp/blob/master/include/jni/traits.hpp)



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


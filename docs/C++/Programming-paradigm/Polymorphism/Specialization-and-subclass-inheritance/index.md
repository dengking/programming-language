# OOP subclass VS GP specialization 

template  specializaiton无法实现inheritance的code reusage。

素材:

1、`thegreenplace-Dependent-name-lookup-for-C++templates`

其中探讨了inheritance + specialization。



## 使用模式example

[jni.hpp](https://github.com/mapbox/jni.hpp)/[include](https://github.com/mapbox/jni.hpp/tree/master/include)/[jni](https://github.com/mapbox/jni.hpp/tree/master/include/jni)/[traits.hpp](https://github.com/mapbox/jni.hpp/blob/master/include/jni/traits.hpp)

```C++
namespace jni
{
    template < class T > struct IsPrimitive : std::false_type {};
    template <> struct IsPrimitive< jboolean > : std::true_type {};
    template <> struct IsPrimitive< jbyte    > : std::true_type {};
    template <> struct IsPrimitive< jchar    > : std::true_type {};
    template <> struct IsPrimitive< jshort   > : std::true_type {};
    template <> struct IsPrimitive< jint     > : std::true_type {};
    template <> struct IsPrimitive< jlong    > : std::true_type {};
    template <> struct IsPrimitive< jfloat   > : std::true_type {};
    template <> struct IsPrimitive< jdouble  > : std::true_type {};

    template < class T > struct IsReference : std::false_type {};
    template <> struct IsReference< jobject    > : std::true_type {};
    template <> struct IsReference< jclass     > : std::true_type {};
    template <> struct IsReference< jstring    > : std::true_type {};
    template <> struct IsReference< jthrowable > : std::true_type {};
}

```


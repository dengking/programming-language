# OOP subclass VS GP specialization 

## 从code reuse的角度来进行分析

template  specializaiton无法实现inheritance的code reusage

素材:

1、`thegreenplace-Dependent-name-lookup-for-C++templates`

其中探讨了inheritance + specialization。



## 使用模式

### Primary class template 和 specialization 继承不同的 parent class



#### [jni.hpp](https://github.com/mapbox/jni.hpp)/[include](https://github.com/mapbox/jni.hpp/tree/master/include)/[jni](https://github.com/mapbox/jni.hpp/tree/master/include/jni)/[traits.hpp](https://github.com/mapbox/jni.hpp/blob/master/include/jni/traits.hpp)

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



#### wikipedia [Substitution failure is not an error](https://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error)



```c++
#include <iostream>
#include <type_traits>

template <typename... Ts>
using void_t = void;

template <typename T, typename = void>
struct has_typedef_foobar : std::false_type {};

template <typename T>
struct has_typedef_foobar<T, void_t<typename T::foobar>> : std::true_type {};

struct foo {
  using foobar = float;
};

int main() {
  std::cout << std::boolalpha;
  std::cout << has_typedef_foobar<int>::value << std::endl;
  std::cout << has_typedef_foobar<foo>::value << std::endl;
}
// g++ --std=c++11 test.cpp

```



正确版本如下:

```C++
#include <iostream>
#include <type_traits>

template<typename ...Ts> struct make_void
{
	using type = void;
};
template<typename ...Ts> using void_t = typename make_void<Ts...>::type;

template<typename T, typename = void>
struct has_typedef_foobar: std::false_type
{
};

template<typename T>
struct has_typedef_foobar<T, void_t<typename T::foobar>> : std::true_type
{
};

struct foo
{
	using foobar = float;
};

int main()
{
	std::cout << std::boolalpha;
	std::cout << has_typedef_foobar<int>::value << std::endl;
	std::cout << has_typedef_foobar<foo>::value << std::endl;
}
// g++ --std=c++11 test.cpp

```

#### cppreference [std::is_member_function_pointer](https://en.cppreference.com/w/cpp/types/is_member_function_pointer)

参见 `std-is_member_function_pointer` 章节。
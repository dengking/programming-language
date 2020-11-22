# Restrict  template parameter

在使用template的过程中，我们常常需要对template parameter添加一些restriction，比如希望它具备某种特性，显然这就是`c++` concept所要解决的问题，它是`c++`20的feature，那在此之前，我们要如何实现呢？本文对这两种实现方式进行总结。



## stackoverflow [C++ templates that accept only certain types](https://stackoverflow.com/questions/874298/c-templates-that-accept-only-certain-types)



### [回答](https://stackoverflow.com/a/874337)

> NOTE: Boost's [static assert](http://www.boost.org/doc/libs/1_39_0/doc/html/boost_staticassert.html)，在c++11中已经包含[`static_assert`](https://en.cppreference.com/w/cpp/language/static_assert) 

```cpp
template<typename T> class my_template;     // Declare, but don't define

// int is a valid type
template<> class my_template<int> {
    ...
};

// All pointer types are valid
template<typename T> class my_template<T*> {
    ...
};

// All other types are invalid, and will cause linker error messages.
```



### [回答](https://stackoverflow.com/a/25216349)

This typically is unwarranted in C++, as other answers here have noted. In C++ we tend to define generic types based on other constraints other than "inherits from this class". If you really wanted to do that, it's quite easy to do in C++11 and `type_traits`:

> NOTE: generic type，基于inheritance和基于template

```cpp
#include <type_traits>

template<typename T>
class observable_list {
    static_assert(std::is_base_of<list, T>::value, "T must inherit from list");
    // code here..
};
```

This breaks a lot of the concepts that people expect in C++ though. It's better to use tricks like defining your own **traits**. For example, maybe `observable_list` wants to accept any type of container that has the typedefs `const_iterator` and a `begin` and `end` member function that returns `const_iterator`. If you restrict this to classes that inherit from `list` then a user who has their own type that doesn't inherit from `list` but provides these member functions and typedefs would be unable to use your `observable_list`.

> NOTE: 这段话对比了基于inheritance的generic programming和基于template的generic programming，显然相比于基于inheritance的generic programming，基于template的generic programming具备更多的优势

There are two solutions to this issue, one of them is to not constrain anything and rely on duck typing. A big con to this solution is that it involves a massive amount of errors that can be hard for users to grok. Another solution is to define **traits** to constrain the type provided to meet the **interface requirements**. The big con for this solution is that involves extra writing which can be seen as annoying. However, the positive side is that you will be able to write your own error messages a la `static_assert`.

For completeness, the solution to the example above is given:

```cpp
#include <type_traits>

template<typename...>
struct void_ {
    using type = void;
};

template<typename... Args>
using Void = typename void_<Args...>::type;

template<typename T, typename = void>
struct has_const_iterator : std::false_type {};

template<typename T>
struct has_const_iterator<T, Void<typename T::const_iterator>> : std::true_type {};

struct has_begin_end_impl {
    template<typename T, typename Begin = decltype(std::declval<const T&>().begin()),
                         typename End   = decltype(std::declval<const T&>().end())>
    static std::true_type test(int);
    template<typename...>
    static std::false_type test(...);
};

template<typename T>
struct has_begin_end : decltype(has_begin_end_impl::test<T>(0)) {};

template<typename T>
class observable_list {
    static_assert(has_const_iterator<T>::value, "Must have a const_iterator typedef");
    static_assert(has_begin_end<T>::value, "Must have begin and end member functions");
    // code here...
};
```

There are a lot of concepts shown in the example above that showcase C++11's features. Some search terms for the curious are **variadic templates**, SFINAE, **expression SFINAE**, and type traits.



## [How do I restrict a template class to certain built-in types?](https://stackoverflow.com/questions/16976720/how-do-i-restrict-a-template-class-to-certain-built-in-types)



### [回答](https://stackoverflow.com/a/16977241)

> NOTE: `static_assert`比本回答中描述的方案是更好的，因为使用`static_assert`是支持打印出提示信息的



## [restrict a template function, to only allow certain types](https://stackoverflow.com/questions/32267324/restrict-a-template-function-to-only-allow-certain-types)
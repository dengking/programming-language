# tartanllama [Detection Idiom - A Stopgap for Concepts](https://blog.tartanllama.xyz/detection-idiom/)

> NOTE: 
>
> 1、"Stopgap"的含义是"临时替代品"

[Concepts](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4641.pdf) is a proposed C++ feature which allows succinct, expressive, and powerful constraining of templates. They have been threatening to get in to C++ for a long time now, with the first proposal being rejected for C++11. They were finally [merged in to C++20](https://herbsutter.com/2017/07/15/trip-report-summer-iso-c-standards-meeting-toronto/) a few months ago, which means we need to hold on for another few years before they’re in the official standard rather than a [Technical Specification](https://stackoverflow.com/questions/21342077/what-is-c-technical-specification). In the mean time, there have been various attempts to implement parts of concepts as a library so that we can have access to some of the power of Concepts without waiting for the new standard. The [detection idiom](http://en.cppreference.com/w/cpp/experimental/is_detected) – designed by [Walter Brown](http://open-std.org/JTC1/SC22/WG21/docs/papers/2015/n4502.pdf) and part of the [Library Fundamentals 2 Technical Specification](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/n4617.pdf) – is one such solution. This post will outline the utility of it, and show the techniques which underlie its implementation.

## A problem

```cpp
template <class T>
int calculate_foo_factor (const T& t);
```



```cpp
struct has_special_support {
  int get_foo() const;
};

struct will_need_generic_calculation {
  // no get_foo member function
};
```

### Using C++20 concept

Using concepts we could write `calculate_foo_factor` like so:

```C++
template <class T>
concept bool SupportsFoo = requires (T t) {
  { t.get_foo() } -> int;
};

template <SupportsFoo T>
int calculate_foo_factor (const T& t) {
    return t.get_foo();
}

template <class T>
int calculate_foo_factor (const T& t) {
    // insert generic calculation here
    return 42;
}
```

This is quite succinct and clear: `SupportsFoo` is a concept which checks that we can call `get_foo` on `t` with no arguments, and that the type of that expression is `int`. The first `calculate_foo_factor` will be selected by overload resolution for types which satisfy the `SupportsFoo` concept, and the second will be chosen for those which don’t.



Unfortunately, our library has to support C++14. We’ll need to try something different. I’ll demonstrate a bunch of possible solutions to this problem in the next section. Some of them may seem complex if you aren’t familiar with the metaprogramming techniques used, but for now, just note the differences in complexity and abstraction between them. The metaprogramming tricks will all be explained in the following section.



## Solutions

```C++
namespace detail {
  template <class T>
  auto calculate_foo_factor (const T& t, int)
    -> decltype(t.get_foo()) {
    return t.get_foo();
  }

  template <class T>
  int calculate_foo_factor (const T& t, ...) {
    // insert generic calculation here
    return 42;
  }
}

template <class T>
int calculate_foo_factor (const T& t) {
  return detail::calculate_foo_factor(t, 0);
}
```

Well, it works, but it’s not exactly clear. What’s the `int` and `...` there for? Why do we need an extra overload? The answers are not the important part here; what is important is that unless you’ve got a reasonable amount of metaprogramming experience, it’s unlikely you’ll be able to write this code offhand, or even copy-paste it without error.

The code could be improved by abstracting out the check for the presence of the member function into its own metafunction:

```c++
template <class... Ts>
using void_t = void;

template <class T, class=void>
struct supports_foo : std::false_type{};

template <class T>
struct supports_foo<T, void_t<decltype(std::declval<T>().get_foo())>>
: std::true_type{};
```



Again, some more expert-only template trickery which I’ll explain later. Using this trait, we can use [`std::enable_if`](http://en.cppreference.com/w/cpp/types/enable_if) to enable and disable the overloads as required:

```C++
template <class T, std::enable_if_t<supports_foo<T>::value>* = nullptr>
auto calculate_foo_factor (const T& t) {
  return t.get_foo();
}

template <class T, std::enable_if_t<!supports_foo<T>::value>* = nullptr>
int calculate_foo_factor (const T& t) {
  // insert generic calculation here
  return 42;
}
```



## Metaprogramming demystified

### Type traits and `_v` and `_t` suffixes

> NOTE: 
>
> 1、这部分内容，收录于 `C++-name-convention` 章节了

### `decltype` specifiers



### `std::declval` trickery



### SFINAE and `std::enable_if`



### Expression SFINAE



### `void_t` magic
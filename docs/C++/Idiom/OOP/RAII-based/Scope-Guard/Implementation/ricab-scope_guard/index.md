# [ricab](https://github.com/ricab)/[scope_guard](https://github.com/ricab/scope_guard)

A public, general, simple, and fast C++11 scope guard that defends against implicitly ignored returns and optionally enforces `noexcept` at compile time (in C++17), all in a SFINAE-friendly maner.

> NOTE: 如何实现"enforces `noexcept` at compile time (in C++17)"？

## Introduction

A scope guard is an object that employs RAII to execute a provided callback when leaving scope, be it through a *fall-through*, a return, or an exception. That callback can be a function, a function pointer, a functor, a lambda, a bind result, a `std::function`, a reference to any of these, or any other callable, as long as it respects a few [preconditions](https://github.com/ricab/scope_guard/blob/master/docs/precond.md) – most of which are enforced during compilation, the rest being hopefully intuitive(直观的).

> NOTE: 初读的时候，"That callback can be a function, a function pointer, a functor, a lambda, a bind result, a `std::function`, a reference to any of these, or any other callable"是非常吸引我的，显然这是实现first-class function的最终目的，那它是如何实现的呢？提供"Read code"，发现它的实现其实是非常简单的: 使用template。



## [Documentation](https://ricab.github.io/scope_guard/)



## Read code

[scope_guard](https://github.com/ricab/scope_guard)/[scope_guard.hpp](https://github.com/ricab/scope_guard/blob/master/scope_guard.hpp)



### Concept、constrain

1、添加了一些custom type concept、constrain来对callback进行constrain。

2、这些concept的实现，都是基于"decltype-declval detection concept"的

#### Concept

`is_noarg_callable_t` 

```C++
#include <type_traits>
#include <utility>
// Type trait determining whether a type is callable with no arguments
template<typename T, typename = void>
struct is_noarg_callable_t: public std::false_type
{
};
// in general, false

template<typename T>
struct is_noarg_callable_t<T, decltype(std::declval<T&&>()())> : public std::true_type
{
};
```

`returns_void_t`

```C++
#include <type_traits>
#include <utility>

// Type trait determining whether a no-argument callable returns void
template<typename T>
struct returns_void_t: public std::is_same<void, decltype(std::declval<T&&>()())>
{
};
```

`is_nothrow_invocable_if_required_t`

```C++
#include <type_traits>
#include <utility>

/* Type trait determining whether a no-arg callable is nothrow invocable if
 required. This is where SG_REQUIRE_NOEXCEPT logic is encapsulated. */
template<typename T>
struct is_nothrow_invocable_if_required_t: public
#ifdef SG_REQUIRE_NOEXCEPT
          std::is_nothrow_invocable<T> /* Note: _r variants not enough to
                                          confirm void return: any return can be
                                          discarded so all returns are
                                          compatible with void */
#else
				std::true_type
#endif
{
};

```

#### logic AND of two or more type traits

```C++
#include <type_traits>
#include <utility>

// logic AND of two or more type traits
template<typename A, typename B, typename ... C>
struct and_t: public and_t<A, and_t<B, C...>>
{
};
// for more than two arguments

template<typename A, typename B>
struct and_t<A, B> : public std::conditional<A::value, B, A>::type
{
};
// for two arguments

```

1、理解`struct and_t<A, B>`是关键

对于`and_t<A, B>`:

如果`A::value`为true，则它的值由`B`决定；

如果`A::value`为false，则它的值由`A`决定，即false；



#### Assemble multiple concept

使用`and_t`来assemble前面介绍的concept。

```C++
// Type trait determining whether a type is a proper scope_guard callback.
template<typename T>
struct is_proper_sg_callback_t: public and_t<is_noarg_callable_t<T>, returns_void_t<T>, is_nothrow_invocable_if_required_t<T>, std::is_nothrow_destructible<T>>
{
};

```



### Separation of declaration and definition

它的实现方式是非常值得借鉴的:

```c++
/* --- The actual scope_guard template --- */
template<typename Callback, typename = typename std::enable_if<is_proper_sg_callback_t<Callback>::value>::type>
class scope_guard;

 /* --- The template specialization that actually defines the class --- */
template<typename Callback>
class scope_guard<Callback> final
{ };
```


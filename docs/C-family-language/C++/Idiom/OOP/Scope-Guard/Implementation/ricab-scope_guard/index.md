# [ricab](https://github.com/ricab)/[scope_guard](https://github.com/ricab/scope_guard)

A public, general, simple, and fast C++11 scope guard that defends against implicitly ignored returns and optionally enforces `noexcept` at compile time (in C++17), all in a SFINAE-friendly maner.

> NOTE: 如何实现"enforces `noexcept` at compile time (in C++17)"？

## Introduction

A scope guard is an object that employs RAII to execute a provided callback when leaving scope, be it through a *fall-through*, a return, or an exception. That callback can be a function, a function pointer, a functor, a lambda, a bind result, a `std::function`, a reference to any of these, or any other callable, as long as it respects a few [preconditions](https://github.com/ricab/scope_guard/blob/master/docs/precond.md) – most of which are enforced during compilation, the rest being hopefully intuitive(直观的).

> NOTE: 初读的时候，"That callback can be a function, a function pointer, a functor, a lambda, a bind result, a `std::function`, a reference to any of these, or any other callable"是非常吸引我的，显然这是实现first-class function的最终目的，那它是如何实现的呢？提供"Read code"，发现它的实现其实是非常简单的: 使用template。



## [Documentation](https://ricab.github.io/scope_guard/)



## Read code

[scope_guard](https://github.com/ricab/scope_guard)/[scope_guard.hpp](https://github.com/ricab/scope_guard/blob/master/scope_guard.hpp)

添加了一些custom type traits来对callback进行constrain。

另外，它的实现方式是非常值得借鉴的:



### Separation of declaration and definition

```c++
/* --- The actual scope_guard template --- */
template<typename Callback, typename = typename std::enable_if<is_proper_sg_callback_t<Callback>::value>::type>
class scope_guard;

 /* --- The template specialization that actually defines the class --- */
template<typename Callback>
class scope_guard<Callback> final
{ };
```


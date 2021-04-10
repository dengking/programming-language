# Assertions: [`assert.h`](https://github.com/microsoft/GSL/blob/main/include/gsl/assert)

实现从最最基础的、被使用最多的开始阅读，即[`assert.h`](https://github.com/microsoft/GSL/blob/main/include/gsl/assert)，其中的`Expects(cond)`、`Ensures(cond)`在整个library中被广泛使用，这是典型的design by contract。



## stackoverflow [Ensures() - guideline support library](https://stackoverflow.com/questions/36349523/ensures-guideline-support-library)



[A](https://stackoverflow.com/a/36349768)

Are you using the [Microsoft GSL implementation](https://github.com/Microsoft/GSL)? Then if you check [the `gsl_assert.h` header file](https://github.com/Microsoft/GSL/blob/master/include/gsl/gsl_assert) you will see that if `GSL_TERMINATE_ON_CONTRACT_VIOLATION` is defined (which is default) then `Ensures` will call [`std::terminate`](http://en.cppreference.com/w/cpp/error/terminate) which will give you the error you get.

If you want an exception to be thrown (with file and line-number information) then you need to define `GSL_THROW_ON_CONTRACT_VIOLATION` before including the GSL.

As for if you're using `Ensures` properly, then yes you are.



## github [Clarification requested for Expects/Ensures contract violation behavior #1561](https://github.com/isocpp/CppCoreGuidelines/issues/1561)



## Read the code

### `Expects` and `Expects` macro

```C++
namespace gsl
{

namespace details
{
#if defined(GSL_MSVC_USE_STL_NOEXCEPTION_WORKAROUND)

    typedef void(__cdecl* terminate_handler)();

    // clang-format off
    GSL_SUPPRESS(f.6) // NO-FORMAT: attribute
    // clang-format on
    [[noreturn]] inline void __cdecl default_terminate_handler()
    {
        __fastfail(RANGE_CHECKS_FAILURE);
    }

    inline gsl::details::terminate_handler& get_terminate_handler() noexcept
    {
        static terminate_handler handler = &default_terminate_handler;
        return handler;
    }

#endif // defined(GSL_MSVC_USE_STL_NOEXCEPTION_WORKAROUND)

    [[noreturn]] inline void terminate() noexcept
    {
#if defined(GSL_MSVC_USE_STL_NOEXCEPTION_WORKAROUND)
        (*gsl::details::get_terminate_handler())();
#else
        std::terminate();
#endif // defined(GSL_MSVC_USE_STL_NOEXCEPTION_WORKAROUND)
    }

} // namespace details
} // namespace gsl

#define GSL_CONTRACT_CHECK(type, cond)                                                             \
    (GSL_LIKELY(cond) ? static_cast<void>(0) : gsl::details::terminate())

#define Expects(cond) GSL_CONTRACT_CHECK("Precondition", cond)
#define Ensures(cond) GSL_CONTRACT_CHECK("Postcondition", cond)
```

1、上述 `static_cast<void>(0)` 是典型的cast to void来实现discard value expression，这样compiler将不会生成任何code。

2、可以看到，`Expects` 和 `Ensures` 本质上做的都是相同的工作，但是它们的语义是不同的。



### `GSL_LIKELY` and `GSL_UNLIKELY` macro

```C++
#if defined(__clang__) || defined(__GNUC__)
#define GSL_LIKELY(x) __builtin_expect(!!(x), 1)
#define GSL_UNLIKELY(x) __builtin_expect(!!(x), 0)

#else

#define GSL_LIKELY(x) (!!(x))
#define GSL_UNLIKELY(x) (!!(x))
#endif // defined(__clang__) || defined(__GNUC__)
```



### `GSL_SUPPRESS`

> NOTE: 
>
> 1、参见`gsl-suppress`章节

```C++
//
// make suppress attributes parse for some compilers
// Hopefully temporary until suppression standardization occurs
//
#if defined(__clang__)
#define GSL_SUPPRESS(x) [[gsl::suppress("x")]]
#else
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
#define GSL_SUPPRESS(x) [[gsl::suppress(x)]]
#else
#define GSL_SUPPRESS(x)
#endif // _MSC_VER
#endif // __clang__
```

### `GSL_ASSUME`

```C++
//
// GSL_ASSUME(cond)
//
// Tell the optimizer that the predicate cond must hold. It is unspecified
// whether or not cond is actually evaluated.
//
#ifdef _MSC_VER
#define GSL_ASSUME(cond) __assume(cond)
#elif defined(__GNUC__)
#define GSL_ASSUME(cond) ((cond) ? static_cast<void>(0) : __builtin_unreachable())
#else
#define GSL_ASSUME(cond) static_cast<void>((cond) ? 0 : 0)
#endif

```


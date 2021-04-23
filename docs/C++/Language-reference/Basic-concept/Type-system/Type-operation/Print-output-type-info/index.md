# Print/output type info

## stackoverflow [Is it possible to print a variable's type in standard C++?](https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c)



### Comments

Here is a summary of Howard's long form solution but implemented with a heretical one-line macro: `#define DEMANGLE_TYPEID_NAME(x) abi::__cxa_demangle(typeid((x)).name(), NULL, NULL, NULL)`. If you need cross-platform support: Use `#ifdef`, `#else`, `#endif` to provide one macros for other platforms like MSVC. – [Trevor Boyd Smith](https://stackoverflow.com/users/52074/trevor-boyd-smith) [Jul 5 '16 at 15:01](https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c/56766138#comment63837522_81870)



### [A](https://stackoverflow.com/a/20170989)

> NOTE: 
>
> 1、这个回答其实展示了各种封装C-string的方式，tag 为 "C string wrapper"

C++11 update to a very old question: Print variable type in C++.

The accepted (and good) answer is to use `typeid(a).name()`, where `a` is a variable name.

Now in C++11 we have `decltype(x)`, which can turn an expression into a type. And `decltype()` comes with its own set of very interesting rules. For example `decltype(a)` and `decltype((a))` will generally be different types (and for good and understandable reasons once those reasons are exposed).

Will our trusty `typeid(a).name()` help us explore this brave new world?

No.

But the tool that will is not that complicated. And it is that tool which I am using as an answer to this question. I will compare and contrast this new tool to `typeid(a).name()`. And this new tool is actually built on top of `typeid(a).name()`.

#### The fundamental issue:

```cpp
typeid(a).name()
```

throws away cv-qualifiers, references, and lvalue/rvalue-ness. For example:

```cpp
const int ci = 0;
std::cout << typeid(ci).name() << '\n';
```

For me outputs:

```cpp
i
```

and I'm guessing on MSVC outputs:

```cpp
int
```

I.e. the `const` is gone. This is not a QOI (Quality Of Implementation) issue. The standard mandates this behavior.

What I'm recommending below is:

```cpp
template <typename T> std::string type_name();
```

which would be used like this:

```cpp
const int ci = 0;
std::cout << type_name<decltype(ci)>() << '\n';
```

and for me outputs:

```cpp
int const
```

`<disclaimer>` I have not tested this on MSVC. `</disclaimer>` But I welcome feedback from those who do.

#### The C++11 Solution

> NOTE: 
>
> 1、理解下面这段代码的前提是理解 [__cxa_demangle](https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.3/a01696.html) 
>
> > **Returns:**
> >
> > A pointer to the start of the NUL-terminated demangled name, or NULL if the demangling fails. The caller is responsible for deallocating this memory using `free`.
>
> 即需要由调用者来释放返回的内存。
>
> 这让我想起了resource return，从下面作者的封装方式来看，它使用的是"unique_ptr custom deleter-RAII"方式。
>
> 2、下面的实现有一个非常不好的是: `std::string r = own != nullptr ? own.get() : typeid(TR).name();` 进行了deep copy，其实这一次deep copy是可以不必的。

I am using `__cxa_demangle` for non-MSVC platforms as recommend by [ipapadop](https://stackoverflow.com/users/487362/ipapadop) in his answer to demangle types. But on MSVC I'm trusting `typeid` to demangle names (untested). And this core is wrapped around some simple testing that detects, restores and reports cv-qualifiers and references to the input type.

```cpp
#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
#   include <cxxabi.h>
#endif
#include <memory>
#include <string>
#include <cstdlib>

template <class T>
std::string
type_name()
{
    typedef typename std::remove_reference<T>::type TR;
    std::unique_ptr<char, void(*)(void*)> own
           (
#ifndef _MSC_VER
                abi::__cxa_demangle(typeid(TR).name(), nullptr,
                                           nullptr, nullptr),
#else
                nullptr,
#endif
                std::free
           );
    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}
```

**The Results**

With this solution I can do this:

```cpp
int& foo_lref();
int&& foo_rref();
int foo_value();

int
main()
{
    int i = 0;
    const int ci = 0;
    std::cout << "decltype(i) is " << type_name<decltype(i)>() << '\n';
    std::cout << "decltype((i)) is " << type_name<decltype((i))>() << '\n';
    std::cout << "decltype(ci) is " << type_name<decltype(ci)>() << '\n';
    std::cout << "decltype((ci)) is " << type_name<decltype((ci))>() << '\n';
    std::cout << "decltype(static_cast<int&>(i)) is " << type_name<decltype(static_cast<int&>(i))>() << '\n';
    std::cout << "decltype(static_cast<int&&>(i)) is " << type_name<decltype(static_cast<int&&>(i))>() << '\n';
    std::cout << "decltype(static_cast<int>(i)) is " << type_name<decltype(static_cast<int>(i))>() << '\n';
    std::cout << "decltype(foo_lref()) is " << type_name<decltype(foo_lref())>() << '\n';
    std::cout << "decltype(foo_rref()) is " << type_name<decltype(foo_rref())>() << '\n';
    std::cout << "decltype(foo_value()) is " << type_name<decltype(foo_value())>() << '\n';
}
```

and the output is:

```cpp
decltype(i) is int
decltype((i)) is int&
decltype(ci) is int const
decltype((ci)) is int const&
decltype(static_cast<int&>(i)) is int&
decltype(static_cast<int&&>(i)) is int&&
decltype(static_cast<int>(i)) is int
decltype(foo_lref()) is int&
decltype(foo_rref()) is int&&
decltype(foo_value()) is int
```

Note (for example) the difference between `decltype(i)` and `decltype((i))`. The former is the type of the *declaration* of `i`. The latter is the "type" of the *expression* `i`. (expressions never have reference type, but as a convention `decltype` represents lvalue expressions with lvalue references).

Thus this tool is an excellent vehicle just to learn about `decltype`, in addition to exploring and debugging your own code.

In contrast, if I were to build this just on `typeid(a).name()`, without adding back lost cv-qualifiers or references, the output would be:

```cpp
decltype(i) is int
decltype((i)) is int
decltype(ci) is int
decltype((ci)) is int
decltype(static_cast<int&>(i)) is int
decltype(static_cast<int&&>(i)) is int
decltype(static_cast<int>(i)) is int
decltype(foo_lref()) is int
decltype(foo_rref()) is int
decltype(foo_value()) is int
```

I.e. Every reference and cv-qualifier is stripped off.

#### C++14 Update

Just when you think you've got a solution to a problem nailed, someone always comes out of nowhere and shows you a much better way. :-)

[This answer](https://stackoverflow.com/a/35943472/576911) from [Jamboree](https://stackoverflow.com/users/2969631/jamboree) shows how to get the type name in C++14 at compile time. It is a brilliant solution for a couple reasons:

1、It's at compile time!

2、You get the compiler itself to do the job instead of a library (even a std::lib). This means more accurate results for the latest language features (like lambdas).

[Jamboree's](https://stackoverflow.com/users/2969631/jamboree) [answer](https://stackoverflow.com/a/35943472/576911) doesn't quite lay everything out for VS, and I'm tweaking his code a little bit. But since this answer gets a lot of views, take some time to go over there and upvote his answer, without which, this update would never have happened.

```cpp
#include <cstddef>
#include <stdexcept>
#include <cstring>
#include <ostream>

#ifndef _MSC_VER
#  if __cplusplus < 201103
#    define CONSTEXPR11_TN
#    define CONSTEXPR14_TN
#    define NOEXCEPT_TN
#  elif __cplusplus < 201402
#    define CONSTEXPR11_TN constexpr
#    define CONSTEXPR14_TN
#    define NOEXCEPT_TN noexcept
#  else
#    define CONSTEXPR11_TN constexpr
#    define CONSTEXPR14_TN constexpr
#    define NOEXCEPT_TN noexcept
#  endif
#else  // _MSC_VER
#  if _MSC_VER < 1900
#    define CONSTEXPR11_TN
#    define CONSTEXPR14_TN
#    define NOEXCEPT_TN
#  elif _MSC_VER < 2000
#    define CONSTEXPR11_TN constexpr
#    define CONSTEXPR14_TN
#    define NOEXCEPT_TN noexcept
#  else
#    define CONSTEXPR11_TN constexpr
#    define CONSTEXPR14_TN constexpr
#    define NOEXCEPT_TN noexcept
#  endif
#endif  // _MSC_VER

class static_string
{
    const char* const p_;
    const std::size_t sz_;

public:
    typedef const char* const_iterator;

    template <std::size_t N>
    CONSTEXPR11_TN static_string(const char(&a)[N]) NOEXCEPT_TN
        : p_(a)
        , sz_(N-1)
        {}

    CONSTEXPR11_TN static_string(const char* p, std::size_t N) NOEXCEPT_TN
        : p_(p)
        , sz_(N)
        {}

    CONSTEXPR11_TN const char* data() const NOEXCEPT_TN {return p_;}
    CONSTEXPR11_TN std::size_t size() const NOEXCEPT_TN {return sz_;}

    CONSTEXPR11_TN const_iterator begin() const NOEXCEPT_TN {return p_;}
    CONSTEXPR11_TN const_iterator end()   const NOEXCEPT_TN {return p_ + sz_;}

    CONSTEXPR11_TN char operator[](std::size_t n) const
    {
        return n < sz_ ? p_[n] : throw std::out_of_range("static_string");
    }
};

inline
std::ostream&
operator<<(std::ostream& os, static_string const& s)
{
    return os.write(s.data(), s.size());
}

template <class T>
CONSTEXPR14_TN
static_string
type_name()
{
#ifdef __clang__
    static_string p = __PRETTY_FUNCTION__;
    return static_string(p.data() + 31, p.size() - 31 - 1);
#elif defined(__GNUC__)
    static_string p = __PRETTY_FUNCTION__;
#  if __cplusplus < 201402
    return static_string(p.data() + 36, p.size() - 36 - 1);
#  else
    return static_string(p.data() + 46, p.size() - 46 - 1);
#  endif
#elif defined(_MSC_VER)
    static_string p = __FUNCSIG__;
    return static_string(p.data() + 38, p.size() - 38 - 7);
#endif
}
```

This code will auto-backoff on the `constexpr` if you're still stuck in ancient C++11. And if you're painting on the cave wall with C++98/03, the `noexcept` is sacrificed as well.

#### C++17 Update

In the comments below [Lyberta](https://stackoverflow.com/users/3624760/lyberta) points out that the new `std::string_view` can replace `static_string`:

```cpp
template <class T>
constexpr
std::string_view
type_name()
{
    using namespace std;
#ifdef __clang__
    string_view p = __PRETTY_FUNCTION__;
    return string_view(p.data() + 34, p.size() - 34 - 1);
#elif defined(__GNUC__)
    string_view p = __PRETTY_FUNCTION__;
#  if __cplusplus < 201402
    return string_view(p.data() + 36, p.size() - 36 - 1);
#  else
    return string_view(p.data() + 49, p.find(';', 49) - 49);
#  endif
#elif defined(_MSC_VER)
    string_view p = __FUNCSIG__;
    return string_view(p.data() + 84, p.size() - 84 - 7);
#endif
}
```

I've updated the constants for VS thanks to the very nice detective work by Jive Dadson in the comments below.

#### Update:

Be sure to check out [this rewrite below](https://stackoverflow.com/a/56766138/576911) which eliminates the unreadable magic numbers in my latest formulation.



### [A](https://stackoverflow.com/a/81886)

Try:

```cpp
#include <typeinfo>

// …
std::cout << typeid(a).name() << '\n';
```
# Scope guard

## 发展历程

在 [ricab](https://github.com/ricab)/**[scope_guard](https://github.com/ricab/scope_guard)** 中给出了总结:

> The concept of "scope guard" was [proposed](http://drdobbs.com/184403758) by Andrei Alexandrescu and Petru Marginean and it is well known in the C++ community. It has been proposed for standardization (see [N4189](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4189.pdf)) but is still not part of the standard library, as of March 2018.

下面是总结: 

### 时间: 1998/2000



| time | 简介                 | 作者                                    |                                                              |
| ---- | -------------------- | --------------------------------------- | ------------------------------------------------------------ |
| 2000 | 首次提出`ScopeGuard` | Andrei Alexandrescu and Petru Marginean | drdobbs [Generic: Change the Way You Write Exception-Safe Code — Forever](https://www.drdobbs.com/cpp/generic-change-the-way-you-write-excepti/184403758) |



### 时间: 2012



| time | 简介               | 作者                |                                                              |
| ---- | ------------------ | ------------------- | ------------------------------------------------------------ |
| 2012 | `ScopeGuard` C++11 | Andrei Alexandrescu | - channel9 [C++ and Beyond 2012: Andrei Alexandrescu - Systematic Error Handling in C++](https://channel9.msdn.com/Shows/Going+Deep/C-and-Beyond-2012-Andrei-Alexandrescu-Systematic-Error-Handling-in-C) |

使用C++11的如下特性对`ScopeGuard`的实现进行升级:

1、parameter pack

2、perfect forwarding

### 时间: 2015



| time | 简介                                                         | 作者                |                                                              |
| ---- | ------------------------------------------------------------ | ------------------- | ------------------------------------------------------------ |
| 2015 | [CppCon 2015](https://channel9.msdn.com/Events/CPP/CppCon-2015)  提出 "Declarative Control Flow" | Andrei Alexandrescu | - channel9 [Declarative Control Flow](https://channel9.msdn.com/Events/CPP/CppCon-2015/CPPConD03V023) <br>- youtu [Andrei Alexandrescu “Declarative Control Flow"](https://youtu.be/WjTrfoiB0MQ) |

实现automatic commit/rollback，使用C++17: 

1、`std::uncaught_exceptions`

## stackoverflow [What is ScopeGuard in C++?](https://stackoverflow.com/questions/31365013/what-is-scopeguard-in-c)

**COMMENTS**

1、What is unclear about the existing SO questions regarding this topic? If you read them you'll also see that this may be referred to as RAII (Resource Acquisition Is Initialization). For example, [Does ScopeGuard use really lead to better code?](http://stackoverflow.com/questions/48647/does-scopeguard-use-really-lead-to-better-code). – [James Adkison](https://stackoverflow.com/users/4505712/james-adkison) [Jul 12 '15 at 6:33](https://stackoverflow.com/questions/31365013/what-is-scopeguard-in-c#comment50710306_31365013) 

> NOTE: stackoverflow [Does ScopeGuard use really lead to better code?](http://stackoverflow.com/questions/48647/does-scopeguard-use-really-lead-to-better-code) 也是一篇非常好的文章，下面收录了这篇文章。

2、No, **scope guards** *are based on* RAII, just as e.g. a `for` loop is based on jumps, but you wouldn't call a `for` loop a jump, would you? `for` loops are at a higher level of abstraction, and are a more specialized concept, than jumps. Scope guards are at a higher level of abstraction, and are a more specialized concept, than RAII. – [Cheers and hth. - Alf](https://stackoverflow.com/users/464581/cheers-and-hth-alf) [Jul 12 '15 at 7:57](https://stackoverflow.com/questions/31365013/what-is-scopeguard-in-c#comment50711299_31365013) 

> NOTE: 这一段对"Scope guard and RAII"的总结是非常好的，显然scope guard是一个非常好的abstraction

3、I have a modern, simple, documented, and carefully tested implementation [here](https://ricab.github.io/scope_guard/) – [ricab](https://stackoverflow.com/users/563765/ricab) [Aug 15 '18 at 22:20](https://stackoverflow.com/questions/31365013/what-is-scopeguard-in-c#comment90686382_31365013)

> NOTE: 收录了这个implementation

[A](https://stackoverflow.com/a/31365171)

`ScopeGuard` was once a particular implementation of scope guards [by Petru Marginean and Andrei Alexandrescu](http://www.drdobbs.com/cpp/generic-change-the-way-you-write-excepti/184403758). The idea is to let the destructor of a guard object call a user specified cleanup action at the end of a scope (read: block), unless the scope guard is *dismissed*. Marginean came up with an ingenious(有独创性的) idea of declaring a scope guard object for C++03, based on lifetime extension of a reference to `const`.

Today “scope guard” is more the general idea.

Scope guards are based on RAII (automatic destructor calls used for cleanup), just as e.g. a `for` loop is based on jumps, but one wouldn't ordinarly call a `for` loop a jump-based piece of code, because that loses most of the information of what it is about, and likewise one does not ordinarily refer to scope guards as RAII. `for` loops are at a higher level of abstraction, and are a more specialized concept, than jumps. Scope guards are at a higher level of abstraction, and are a more specialized concept, than RAII.

> NOTE: 和前面的comment 2重复；

------

In C++11 scope guards can be trivially implemented in terms of `std::function`, with the cleanup action supplied in each place via a lambda expression.

Example:

```cpp
#include <functional>       // std::function
#include <utility>          // std::move

namespace my {
    using std::function;
    using std::move;

    class Non_copyable
    {
    private:
        auto operator=( Non_copyable const& ) -> Non_copyable& = delete;
        Non_copyable( Non_copyable const& ) = delete;
    public:
        auto operator=( Non_copyable&& ) -> Non_copyable& = default;
        Non_copyable() = default;
        Non_copyable( Non_copyable&& ) = default;
    };

    class Scope_guard
        : public Non_copyable
    {
    private:
        function<void()>    cleanup_;

    public:
        friend
        void dismiss( Scope_guard& g ) { g.cleanup_ = []{}; }

        ~Scope_guard() { cleanup_(); }

        template< class Func >
        Scope_guard( Func const& cleanup )
            : cleanup_( cleanup )
        {}

        Scope_guard( Scope_guard&& other )
            : cleanup_( move( other.cleanup_ ) )
        { dismiss( other ); }
    };

}  // namespace my

#include <iostream>
void foo() {}
auto main() -> int
{
    using namespace std;
    my::Scope_guard const final_action = []{ wclog << "Finished! (Exit from main.)\n"; };

    wcout << "The answer is probably " << 6*7 << ".\n";
}
// g++ --std=c++11 test.cpp

```

> NOTE: 上述程序编译报错如下:
>
> ```C++
> test.cp: In function ‘int main()’:
> test.cp:48:88: error: use of deleted function ‘my::Scope_guard::Scope_guard(const my::Scope_guard&)’
>      my::Scope_guard const final_action = []{ wclog << "Finished! (Exit from main.)\n"; };
>                                                                                         ^
> test.cp:19:11: note: ‘my::Scope_guard::Scope_guard(const my::Scope_guard&)’ is implicitly declared as deleted because ‘my::Scope_guard’ declares a move constructor or move assignment operator
>      class Scope_guard
> 
> ```
>
> 下面是正确版本:

```C++
#include <functional>       // std::function
#include <utility>          // std::move

namespace my {
    using std::function;
    using std::move;

    class Non_copyable
    {
    private:
        auto operator=( Non_copyable const& ) -> Non_copyable& = delete;
        Non_copyable( Non_copyable const& ) = delete;
    public:
        auto operator=( Non_copyable&& ) -> Non_copyable& = default;
        Non_copyable() = default;
        Non_copyable( Non_copyable&& ) = default;
    };

    class Scope_guard
        : public Non_copyable
    {
    private:
        function<void()>    cleanup_;

    public:
        friend
        void dismiss( Scope_guard& g ) { g.cleanup_ = []{}; }

        ~Scope_guard() { cleanup_(); }

        template< class Func >
        Scope_guard( Func const& cleanup )
            : cleanup_( cleanup )
        {}

        Scope_guard( Scope_guard&& other )
            : cleanup_( move( other.cleanup_ ) )
        { dismiss( other ); }
    };

}  // namespace my

#include <iostream>
void foo() {}
auto main() -> int
{
    using namespace std;
    my::Scope_guard const final_action([]{ wclog << "Finished! (Exit from main.)\n"; });

    wcout << "The answer is probably " << 6*7 << ".\n";
}
// g++ --std=c++11 test.cpp

```

> NOTE: 输出如下:
>
> ```C++
> The answer is probably 42.
> Finished! (Exit from main.)
> ```
>
> 

The rôle of the `function` here is to avoid templating so that `Scope_guard` instances can be declared as such, and passed around. An alternative, slightly more complex and with slightly constrained usage, but possibly marginally more efficient, is to have a class templated on a functor type, and use C++11 `auto` for declarations, with the scope guard instance created by a factory function. Both these techniques are simple C++11 ways to do what Marginean did with reference lifetime extension for C++03.

> NOTE: 比较了两种实现first-class function的方式。

## stackoverflow [Does ScopeGuard use really lead to better code?](https://stackoverflow.com/questions/48647/does-scopeguard-use-really-lead-to-better-code)

[A](https://stackoverflow.com/a/48663)

> NOTE: 作者的观点是: 使用RAII，而不是try...catch...

It definitely improves your code. Your tentatively(短暂的) formulated claim, that it's obscure and that code would merit(值得) from a `catch` block is simply not true in C++ because RAII is an established idiom. Resource handling in C++ *is* done by resource acquisition and garbage collection is done by implicit destructor calls.

On the other hand, explicit `catch` blocks would bloat the code and introduce subtle errors because the code flow gets much more complex and resource handling has to be done explicitly.

RAII (including `ScopeGuard`s) isn't an obscure technique in C++ but firmly established best-practice.

[A](https://stackoverflow.com/a/550945)

Yes.

If there is one single piece of C++ code that I could recommend every C++ programmer spend 10 minutes learning, it is `ScopeGuard` (now part of the freely available [Loki library](http://loki-lib.sourceforge.net/)).

I decided to try using a (slightly modified) version of ScopeGuard for a smallish Win32 GUI program I was working on. Win32 as you may know has many different types of resources that need to be closed in different ways (e.g. kernel handles are usually closed with `CloseHandle()`, GDI `BeginPaint()` needs to be paired with `EndPaint()`, etc.) I used ScopeGuard with all these resources, and also for allocating working buffers with `new` (e.g. for character set conversions to/from Unicode).

**What amazed me was how much *shorter* the program was.** Basically, it's a win-win: your code gets shorter and more robust at the same time. Future code changes *can't leak anything*. They just can't. How cool is that?

[A](https://stackoverflow.com/a/64824020)

> NOTE: 比较了Scope guard and RAII

## Scope guard and RAII

1、stackoverflow [What is ScopeGuard in C++?](https://stackoverflow.com/questions/31365013/what-is-scopeguard-in-c) 

其中阐述了scope guard 和 RAII之间的关系

2、stackoverflow [Does ScopeGuard use really lead to better code?](https://stackoverflow.com/questions/48647/does-scopeguard-use-really-lead-to-better-code) # [A](https://stackoverflow.com/a/64824020)

其中比较了Scope guard and RAII

3、[ricab](https://github.com/ricab)/**[scope_guard](https://github.com/ricab/scope_guard)**

在其中的introduction段，有着非常好的介绍。



## Implementation

下面是一些implementation，在`./Implementation`中，也给出了一些implementation。

pizer.wordpress. [scope guards revisited [C++0x style]](https://pizer.wordpress.com/2008/11/22/scope-guards-revisited-c0x-style/)

https://www.boost.org/doc/libs/1_75_0/libs/scope_exit/doc/html/scope_exit/tutorial.html

https://en.cppreference.com/w/cpp/experimental/scope_exit

## TODO

stackoverflow [C++11 scope exit guard, a good idea?](https://stackoverflow.com/questions/3669833/c11-scope-exit-guard-a-good-idea)

https://metacpan.org/pod/Scope::Guard
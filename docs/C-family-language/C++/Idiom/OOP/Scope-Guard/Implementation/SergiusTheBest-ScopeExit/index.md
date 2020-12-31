# [SergiusTheBest](https://github.com/SergiusTheBest)/[ScopeExit](https://github.com/SergiusTheBest/ScopeExit)

## Introduction

ScopeExit library provides an efficient and convenient way to execute statements when execution flow leaves current scope. It implements a so-called [scope guard idiom](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Scope_Guard) and defines 3 type of guards:

- `SCOPE_EXIT` - statements are always executed on scope exit
- `SCOPE_SUCCESS` - statements are executed on scope exit when no exceptions have been thrown
- `SCOPE_FAILURE` - statements are executed when scope is leaving due to an exception

## Competing C++ scope exit/guard libraries

- [offa/scope-guard](https://github.com/offa/scope-guard)
- [Neargye/scope_guard](https://github.com/Neargye/scope_guard)
- [ricab/scope_guard](https://github.com/ricab/scope_guard)
- [Simple-Look Scope Guard for Visual C++ 2010](https://www.codeproject.com/Articles/124130/Simple-Look-Scope-Guard-for-Visual-C)

## Other resources

- [Scope guards in D Programming Language](https://tour.dlang.org/tour/en/gems/scope-guards)
- [Andrei Alexandrescu “Declarative Control Flow"](https://youtu.be/WjTrfoiB0MQ)

## Sample

Here is a simple sample that prints htlm code. Scope exit blocks make sure that html tags are properly closed:

```c++

#include <iostream>
#include <ScopeExit/ScopeExit.h>

using namespace std;

int main()
{
    cout << "<html>" << endl;
    SCOPE_EXIT{ cout << "</html>" << endl; };

    {
        cout << "<head>" << endl;
        SCOPE_EXIT{ cout << "</head>" << endl; };

        cout << "<title>Hello</title>" << endl;
    }

    cout << "<body>" << endl;
    SCOPE_EXIT{ cout << "</body>" << endl; };

    cout << "<h1>Hello World!</h1>" << endl;

    return 0;
}
```

> NOTE: 输出如下:
>
> ```html
> <html>
> <head>
> <title>Hello</title>
> </head>
> <body>
> </body>
> <h1>Hello World!</h1>
> </html>
> ```
>
> 

## Read code



### [ScopeExit](https://github.com/SergiusTheBest/ScopeExit)/[include](https://github.com/SergiusTheBest/ScopeExit/tree/master/include)/[ScopeExit](https://github.com/SergiusTheBest/ScopeExit/tree/master/include/ScopeExit)/[ScopeExit.h](https://github.com/SergiusTheBest/ScopeExit/blob/master/include/ScopeExit/ScopeExit.h)

它的实现原理是在object的destructor中调用call back。

```C++
#pragma once

//
// Usage:
//
// SCOPE_EXIT{ cout << "hello"; }; // will be called at the scope exit
//

#define SCOPE_EXIT_CAT2(x, y) x##y
#define SCOPE_EXIT_CAT(x, y) SCOPE_EXIT_CAT2(x, y)
#define SCOPE_EXIT const auto SCOPE_EXIT_CAT(scopeExit_, __COUNTER__) = ScopeExit::MakeScopeExit() += [&]

namespace ScopeExit
{
    template<typename F>
    class ScopeExit 
    {
    public:
        explicit ScopeExit(F&& fn) : m_fn(fn)
        {
        }
        
        ~ScopeExit()
        { 
            m_fn();
        }

        ScopeExit(ScopeExit&& other) : m_fn(std::move(other.m_fn))
        {
        }

    private:
        ScopeExit(const ScopeExit&);
        ScopeExit& operator=(const ScopeExit&);

    private:
        F m_fn;
    };

    struct MakeScopeExit
    {
        template<typename F>
        ScopeExit<F> operator+=(F&& fn)
        {
            return ScopeExit<F>(std::move(fn));
        }
    };
}
```

需要结合usage来理解`SCOPE_EXIT`的实现: 

`SCOPE_EXIT{ cout << "hello"; }` 会被替换为:

```C++
const auto SCOPE_EXIT_CAT(scopeExit_, __COUNTER__) =  ScopeExit::MakeScopeExit() += [&]{ cout << "hello"; }
```

`SCOPE_EXIT_CAT` 用于生成object的name。



### [ScopeExit](https://github.com/SergiusTheBest/ScopeExit)/[include](https://github.com/SergiusTheBest/ScopeExit/tree/master/include)/[ScopeExit](https://github.com/SergiusTheBest/ScopeExit/tree/master/include/ScopeExit)/[ScopeFailure.h](https://github.com/SergiusTheBest/ScopeExit/blob/master/include/ScopeExit/ScopeFailure.h)

和 `ScopeExit` 类似，所不同的是在destructor中，会通过调用 `std::uncaught_exception()` 来判断是否抛出了exception。

```C++
#pragma once
#include <exception>

//
// Usage:
//
// SCOPE_FAILURE{ cout << "hello"; }; // will be called at the scope exit in case of failure (exception is thrown)
//

#define SCOPE_FAILURE_CAT2(x, y) x##y
#define SCOPE_FAILURE_CAT(x, y) SCOPE_FAILURE_CAT2(x, y)
#define SCOPE_FAILURE const auto SCOPE_FAILURE_CAT(scopeFailure_, __COUNTER__) = ScopeExit::MakeScopeFailure() += [&]

namespace ScopeExit
{
    template<typename F>
    class ScopeFailure 
    {
    public:
        explicit ScopeFailure(F&& fn) : m_fn(fn)
        {
        }
        
        ~ScopeFailure()
        { 
            if (std::uncaught_exception())
            {
                m_fn();
            }
        }

        ScopeFailure(ScopeFailure&& other) : m_fn(std::move(other.m_fn))
        {
        }

    private:
        ScopeFailure(const ScopeFailure&);
        ScopeFailure& operator=(const ScopeFailure&);

    private:
        F m_fn;
    };

    struct MakeScopeFailure
    {
        template<typename F>
        ScopeFailure<F> operator+=(F&& fn)
        {
            return ScopeFailure<F>(std::move(fn));
        }
    };
}
```

### [ScopeExit](https://github.com/SergiusTheBest/ScopeExit)/[include](https://github.com/SergiusTheBest/ScopeExit/tree/master/include)/[ScopeExit](https://github.com/SergiusTheBest/ScopeExit/tree/master/include/ScopeExit)/[ScopeSuccess.h](https://github.com/SergiusTheBest/ScopeExit/blob/master/include/ScopeExit/ScopeSuccess.h)

```C++
#pragma once
#include <exception>

//
// Usage:
//
// SCOPE_SUCCESS{ cout << "hello"; }; // will be called at the scope success in case of success (no exception is thrown)
//

#define SCOPE_SUCCESS_CAT2(x, y) x##y
#define SCOPE_SUCCESS_CAT(x, y) SCOPE_SUCCESS_CAT2(x, y)
#define SCOPE_SUCCESS const auto SCOPE_SUCCESS_CAT(scopeSuccess_, __COUNTER__) = ScopeExit::MakeScopeSuccess() += [&]

namespace ScopeExit
{
    template<typename F>
    class ScopeSuccess 
    {
    public:
        explicit ScopeSuccess(F&& fn) : m_fn(fn)
        {
        }
        
        ~ScopeSuccess()
        { 
            if (!std::uncaught_exception())
            {
                m_fn();
            }
        }

        ScopeSuccess(ScopeSuccess&& other) : m_fn(std::move(other.m_fn))
        {
        }

    private:
        ScopeSuccess(const ScopeSuccess&);
        ScopeSuccess& operator=(const ScopeSuccess&);

    private:
        F m_fn;
    };

    struct MakeScopeSuccess
    {
        template<typename F>
        ScopeSuccess<F> operator+=(F&& fn)
        {
            return ScopeSuccess<F>(std::move(fn));
        }
    };
}
```


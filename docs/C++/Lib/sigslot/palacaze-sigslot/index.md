# [palacaze](https://github.com/palacaze)/**[sigslot](https://github.com/palacaze/sigslot)**

> NOTE: 
>
> signal需要保存slot



## [Documentation](https://github.com/palacaze/sigslot#documentation)

Right now there are two limitations that I can think of with respect to callable handling: default arguments and function overloading. Both are working correctly in the case of function objects but will fail to compile with static and member functions, for different but related reasons.

### Signal with arguments

#### Coping with overloaded functions

```C++
#include <sigslot/signal.hpp>

template <typename... Args, typename C>
constexpr auto overload(void (C::*ptr)(Args...)) {
    return ptr;
}

template <typename... Args>
constexpr auto overload(void (*ptr)(Args...)) {
    return ptr;
}

struct obj {
    void operator()(int) const {}
    void operator()() {}
};

struct foo {
    void bar(int) {}
    void bar() {}

    static void baz(int) {}
    static void baz() {}
};

void moo(int) {}
void moo() {}

int main() {
    sigslot::signal<int> sig;

    // connect the slots, casting to the right overload if necessary
    foo ff;
    sig.connect(overload<int>(&foo::bar), &ff);
    sig.connect(overload<int>(&foo::baz));
    sig.connect(overload<int>(&moo));
    sig.connect(obj());

    sig(0);

    return 0;
}

```

> NOTE: 
>
> 
>
> ```C++
> template <typename... Args, typename C>
> constexpr auto overload(void (C::*ptr)(Args...)) {
>     return ptr;
> }
> 
> template <typename... Args>
> constexpr auto overload(void (*ptr)(Args...)) {
>     return ptr;
> }
> ```
>
> 对上述程序的理解:
>
> 需要注意的是: 加入 `overload` 的目的是: 区分`&foo::bar`。
>
> ```C++
> struct foo {
>     void bar(double d);
>     void bar();
> };
> ```
>
> 那如何来进行区分呢？方法是由programmer来指定参数类型来进行区分，模板参数 `Args` 指定是的参数类型，显然，这样就可以完成重载的区分了。
>
> ```C++
> template <typename... Args, typename C>
> constexpr auto overload(void (C::*ptr)(Args...)) {
>     return ptr;
> }
> ```
>
> 它所指定的是member method；
>
> ```C++
> template <typename... Args>
> constexpr auto overload(void (*ptr)(Args...)) {
>     return ptr;
> }
> ```
>
> 它所指定的是free function。
>
> 

#### Coping with function with default arguments

A simple work around for this use case would is to create a bind adapter, in fact we can even make it quite generic like so:

```C++
#include <sigslot/signal.hpp>

#define ADAPT(func) \
    [=](auto && ...a) { (func)(std::forward<decltype(a)>(a)...); }

void foo(int &i, int b = 1) {
    i += b;
}

int main() {
    int i = 0;

    // fine, all the arguments are handled
    sigslot::signal<int&, int> sig1;
    sig1.connect(foo);
    sig1(i, 2);

    // must wrap in an adapter
    i = 0;
    sigslot::signal<int&> sig2;
    sig2.connect(ADAPT(foo));
    sig2(i);

    return 0;
}

```

> NOTE: 
>
> 上述解决方法其实非常简单: 创建一个新的lambda。

### Connection management

A `sigslot::connection` does not tie a connection to a scope: this is not a RAII object, which explains why it can be copied. It can be however implicitly converted into a `sigslot::scoped_connection` which destroys the connection when going out of scope.

#### Automatic slot lifetime tracking

> NOTE: 
>
> 是对slot 的 lifetime进行track

```C++
#include "signal.hpp"
using namespace sigslot;
int sum = 0;

struct s {
    void f(int i) { sum += i; }
};

struct dummy {};

int main() {
    sum = 0;
    signal<int> sig;

    // track lifetime of object and also connect to a member function
    auto p = std::make_shared<s>();
    sig.connect(&s::f, p);

    sig(1);     // sum == 1
    p.reset();
    sig(1);     // sum == 1

    // track an unrelated object lifetime
    auto l = [&](int i) { sum += i; };

    auto d = std::make_shared<dummy>();
    sig.connect(l, d);
    sig(1);     // sum == 2
    d.reset();
    sig(1);     // sum == 2

}
```

> NOTE: 
>
> 上述程序是对原文中的程序的改编，它能够编译通过



#### Intrusive slot lifetime tracking

> NOTE: 
>
> "intrusive"的意思是"侵入的"



## Read code

### `is_callable` concept

```C++
template <typename, typename, typename = void, typename = void>
struct is_callable : std::false_type {};

template <typename F, typename P, typename... T>
struct is_callable<F, P, typelist<T...>,
        void_t<decltype(((*std::declval<P>()).*std::declval<F>())(std::declval<T>()...))>>
    : std::true_type {};

template <typename F, typename... T>
struct is_callable<F, typelist<T...>,
        void_t<decltype(std::declval<F>()(std::declval<T>()...))>>
    : std::true_type {};

```





```c++
template <typename F, typename P, typename... T>
struct is_callable<F, P, typelist<T...>,
        void_t<decltype(((*std::declval<P>()).*std::declval<F>())(std::declval<T>()...))>>
    : std::true_type {};
```

对应的是member method；

### union `func_ptr`

```C++
/*
 * This union is used to compare function pointers
 * Generic callables cannot be compared. Here we compare pointers but there is
 * no guarantee that this always works.
 */
union SIGSLOT_MAY_ALIAS func_ptr {
    void* value() {
        return &data[0];
    }

    const void* value() const {
        return &data[0];
    }

    template <typename T>
    T& value() {
        return *static_cast<T*>(value());
    }

    template <typename T>
    const T& value() const {
        return *static_cast<const T*>(value());
    }

    inline explicit operator bool() const {
        return value() != nullptr;
    }

    inline bool operator==(const func_ptr &o) const {
        return std::equal(std::begin(data), std::end(data), std::begin(o.data));
    }

    mock::fun_types _;
    char data[sizeof(mock::fun_types)];
};
```

`data` 就是指针对象，下面函数帮助理解:

```C++
#include<iostream>

int main() {
    int array[]={0, 1, 2, 3, 4};
    std::cout<<array<<std::endl;
    std::cout<<&array[0]<<std::endl;
}
```


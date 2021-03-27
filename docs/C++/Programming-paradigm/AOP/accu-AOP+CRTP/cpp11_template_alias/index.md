# [MetatronEX](https://github.com/MetatronEX)/[cpp-aop](https://github.com/MetatronEX/cpp-aop)/[cpp11_template_alias](https://github.com/MetatronEX/cpp-aop/tree/master/cpp11_template_alias)/[aop.h](https://github.com/MetatronEX/cpp-aop/blob/master/cpp11_template_alias/aop.h)



```C++
#ifndef AOP_H
#define AOP_H

namespace aop
{

template <class A>
class NullAspect
{};

template <template <template <class> class> class Base>
struct Decorate
{
private:
    struct None {};

    template <template <class> class ... Aspects>
    struct Apply;

    template <template <class> class T>
    struct Apply<T>
    {
        template <class E>
        using Type = T<E>;
    };

    template<template <class> class A1, template <class> class ... Aspects>
    struct Apply<A1, Aspects...>
    {
        template <class T>
        using Type = A1<typename Apply<Aspects...>::template Type<T>>;
    };

public:
    template<template <class> class ... Aspects>
    struct with
    {
        template <class T>
        using AspectsCombination = typename Apply<Aspects...>::template Type<T>;

        typedef AspectsCombination<Base<AspectsCombination>> Type;
    };
};
}
#endif
```

1、`struct Apply`做的事情是比较简单的，即将所有的aspect串联起来，这是典型的recursion，形成类似如如下的效果:


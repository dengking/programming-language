/*
    Copyright (C) 2011, 2012 Hugo Arregui

    This file is part of the cpp-aop Library.

    cpp-aop is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    cpp-aop is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with cpp-aop.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef AOP_H
#define AOP_H

namespace aop
{

template <class A>
class NullAspect
{};

template <template <template <class> class> class Base, template <class> class A>
struct BaseAopData
{
    typedef typename A<Base<A>>::Type Type;
};

template <template <template <class> class> class Base>
struct BaseAopData<Base, NullAspect>
{
    typedef Base<NullAspect> Type;
};

template <template <class> class Aspect, class A>
struct AspectAopData
{
    typedef typename A::AopData::Type Type;
    typedef Aspect<A> AspectType;
};

template <template <template <class> class> class Base>
struct Decorate
{
private:
    struct None {};

    template <template <class> class A, class B = None>
    struct Binder
    {
        template <class T>
        struct Binding
        {
            typedef typename Binder<A>::template Binding<typename B::template Binding<T>::Type>::Type Type;
        };
    };

    template<template <class> class T>
    struct Binder<T, None>
    {
        template <class P>
        struct Binding
        {
            typedef T<P> Type;
        };
    };

    template <template <class> class ... Aspects>
    struct Apply;

    template <template <class> class T>
    struct Apply<T>
    {
        typedef Binder<T> Type;
    };

    template<template <class> class A1, template <class> class ... Aspects>
    struct Apply<A1, Aspects...>
    {
        typedef Binder<A1, typename Apply<Aspects...>::Type> Type;
    };

public:
    template<template <class> class ... Aspects>
    struct with
    {
        typedef typename Apply<Aspects...>::Type TypeP;
        typedef typename TypeP::template Binding<Base<TypeP::template Binding>>::Type Type;
    };
};
}
#endif

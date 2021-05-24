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

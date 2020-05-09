# Object

需要注意的是，本文中的 object 并不是我们 object-oriented programming 中所指的 object；本文的 object是指“region of storage”，在C中，有同样的概念，参见[Objects and alignment](https://en.cppreference.com/w/c/language/object)。显然，它的含义与object-oriented programming 中所指的 object 完全不同；

此处的 object 是一个 runtime 概念，因为只有当program运行的时候，object才会被创建。

> NOTE: 上面这段话的是不准确的，c++语言重复发挥了compile-time computation，所有有多object是在运行时就已经创建了

object概念对于理解后面的内容非常重要，可以说，后续的很多概念都是建立在object之上的。

## cppreference [Object](https://en.cppreference.com/w/cpp/language/object)

C++ programs create, destroy, refer to, access, and manipulate *objects*.

> NOTE: 在上一节我们描述了C++ program的组成（是静态的、compile-time的），上面这段话描述了C++ program在runtime所做的事情。上述描述和C的[Objects and alignment](https://en.cppreference.com/w/c/language/object)中的描述相同。

An object, in C++ , is a *region of storage* that (until C++14) has

- size (can be determined with [sizeof](https://en.cppreference.com/w/cpp/language/sizeof));
- alignment requirement (can be determined with [alignof](https://en.cppreference.com/w/cpp/language/alignof));
- [storage duration](https://en.cppreference.com/w/cpp/language/storage_duration) (automatic, static, dynamic, thread-local);
- [lifetime](https://en.cppreference.com/w/cpp/language/lifetime) (bounded by storage duration or temporary);
- [type](https://en.cppreference.com/w/cpp/language/type);
- value (which may be indeterminate, e.g. for [default-initialized](https://en.cppreference.com/w/cpp/language/default_initialization) non-class types);
- optionally, a [name](https://en.cppreference.com/w/cpp/language/name).

> NOTE: 上述都是对object的属性的描述

The following entities are not objects: value, reference, function, enumerator, type, non-static class member, template, class or function template specialization, namespace, parameter pack, and this.

> NOTE: 上面这段话有些多余，按照[Basic concepts](https://en.cppreference.com/w/cpp/language/basic_concepts)中的描述，object和这些entity之间是并列关系，所以显然它们都不是object。

A *variable* is an object or a reference that is not a non-static data member, that is introduced by a [declaration](https://en.cppreference.com/w/cpp/language/declarations).

> NOTE: “ a reference that is not a non-static data member”这段话是比较绕的，“non-static data member”指的是class的哪些没有使用`static`修饰的data member，“not a non-static data member”就相当于是否定的否定是肯定，即是static data member，则它的意思是：variable可以是static data member。

### Object representation and value representation



#### [Subobjects](https://en.cppreference.com/w/cpp/language/object#Subobjects)



#### [Polymorphic objects](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects)



#### [Strict aliasing](https://en.cppreference.com/w/cpp/language/object#Strict_aliasing)



#### Alignment

## Lifetime of object

object的initialization。

## Variable and object

两种都是runtime概念，variable是一种object，但是不是所有的object都是variable。
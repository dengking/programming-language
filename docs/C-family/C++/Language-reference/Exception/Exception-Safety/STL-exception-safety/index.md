# STL exception safety

在下面文章中对STL exception safety进行了讨论:

1、boost [Lessons Learned from Specifying Exception-Safety for the C++ Standard Library](https://www.boost.org/community/exception_safety.html)

这篇文章非常好，讨论地非常深入，在如下段落中，有着非常好的总结:

a、`3 A contractual basis for exception-safety # Strong guarantee` 

>  In the C++ standard library, nearly all of the operations on the node-based containers list, set, multiset, map, and multimap provide the *strong* guarantee.[4](https://www.boost.org/community/exception_safety.html#footnote4)

b、`6 Should we take everything we can get? # Invariant` 

> We absolutely depend on the *no-throw* guarantee from `set<T>::erase`.[9](https://www.boost.org/community/exception_safety.html#footnote9)

2、stackexchange [DeepPtr: a deep-copying unique_ptr wrapper in C++](https://codereview.stackexchange.com/questions/103744/deepptr-a-deep-copying-unique-ptr-wrapper-in-c) # [A](https://codereview.stackexchange.com/a/103792)

> This does a copy on move construction. Also you should mark you move constructor as `noexcept`. This will give you several advantages when using the object with STL containers (the STL will default to copy even if move is available if the move is not not marked `noexcept`. This is to provide the strong exception guarantee).

## STL iterator exception safety

本节对STL iterator的exception safety进行总结。

1、boost [Lessons Learned from Specifying Exception-Safety for the C++ Standard Library](https://www.boost.org/community/exception_safety.html) 

在其 "6 Should we take everything we can get?" 中，有着这样的描述: 

> we need the *no-throw* guarantee from the copy constructor of `set<T>::iterator`.

显然，copy constructor of `set<T>::iterator`能够提供*no-throw* guarantee 。

2、stackoverflow [How can an implementation guarantee that copy constructor of an iterator is no throw?](https://stackoverflow.com/questions/13428851/how-can-an-implementation-guarantee-that-copy-constructor-of-an-iterator-is-no-t)

Clause 23.2.1.10 of C++11 standard says that

> "no copy ctor of a returned iterator throws an exception"

Does this basically state that is it possible for a copy ctor of an iterator not to throw even a `bad_alloc` presumably(推测起来、想必) (leaving the case where iterator could be just a pointer and here no issues) because it will use the information already constructed in the "returned iterator"? because it is passed by value will the stack be allocated in the called function hence can **guarantee** no memory issues ?

[A](https://stackoverflow.com/a/13430036)

That paragraph talks about the iterators used by the containers in the standard library. These iterators are known to be implementable in ways so that they don't throw exception while being copied. For example, none of them have to use any dynamically allocated memory.

> NOTE: 
>
> 1、最后一句话"For example, none of them have to use any dynamically allocated memory"，让我想到的是: 
>
> 一般，如果不dynamically allocated memory，基本上能够保证no throw guarantee

The guarantee is just for these iterators, not for iterators in general (even though it is a good idea to follow the example).


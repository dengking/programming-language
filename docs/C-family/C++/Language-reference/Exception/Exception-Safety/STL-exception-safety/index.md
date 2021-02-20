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
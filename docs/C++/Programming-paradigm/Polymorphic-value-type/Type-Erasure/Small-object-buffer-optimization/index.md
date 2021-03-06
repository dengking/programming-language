# Small object optimization in type-erasure /concept-based polymorphism

1、是在阅读 open-std [P1144R2 Object relocation in terms of move plus destroy](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html) 时，其中提及的:

> Given a reliable way of detecting trivial relocatability, we can de-duplicate the code generated by small-buffer-optimized (SBO) type-erasing wrappers such as `std::function` and `std::any`. 

2、关于这个topic的非常好的文章:

a、sean-parent [Polymorphic Task Template in Ten](https://sean-parent.stlab.cc/presentations/2017-11-11-lightning-talk/lightning-talk.pdf)

b、sodocumentation [C++Type Erasure](https://sodocumentation.net/cplusplus/topic/2872/type-erasure)

3、Small buffer optimization是一种场景的optimization technique，由于type erasure中，通常涉及dynamic allocation，因此可以使用small buffer optimization来进行优化。





## stackoverflow [Easy way to implement small buffer optimization for arbitrary type erasure (like in std::function.)](https://stackoverflow.com/questions/33582786/easy-way-to-implement-small-buffer-optimization-for-arbitrary-type-erasure-like)

[A](https://stackoverflow.com/a/33583906)

I know nothing about the small buffer optimization *required* by the standard or any proposal, though it is often allowed or encouraged.

On the other hand, you can just make your own solution from scratch, based on the standard library (e.g. `std::aligned_storage`). This may still verbose from the view of users, but not too hard.

Actually I implemented (not proposed then) [`any` with such optimization](https://bitbucket.org/FrankHB/yslib/src/0c73b4bfda5e0ff3a41d81b890aee8cf479cfdf0/YBase/include/ystdex/any.h?at=master&fileviewer=file-view-default) and [some related utilities](https://bitbucket.org/FrankHB/yslib/src/0c73b4bfda5e0ff3a41d81b890aee8cf479cfdf0/YBase/include/ystdex/any_iterator.hpp?at=master) several years ago. Lately, libstdc++'s implementation of `std::experimental::any` used the technique almost exactly as this (however, `__` prefixed internal names are certainly not good for ordinary library users).

My implementation now uses [some common helpers](https://bitbucket.org/FrankHB/yslib/src/0c73b4bfda5e0ff3a41d81b890aee8cf479cfdf0/YBase/include/ystdex/type_pun.hpp?at=master&fileviewer=file-view-default#type_pun.hpp-227) to deal with the storage. These helpers do ease to implement the type erasure storage strategy (at least fit for something similar to `any` enough). But I am still interested in more general high-level solution to simplify the interface redirecting.
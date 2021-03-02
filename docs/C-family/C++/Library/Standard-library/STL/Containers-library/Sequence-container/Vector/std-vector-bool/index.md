# `std::vector<bool>`



## cppreference [`std::vector<bool>`](https://en.cppreference.com/w/cpp/container/vector_bool) 



## `std::vector<bool>` is not a container

在阅读[boost iterator](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/index.html)的[Standard Proposal For New-Style Iterators](https://www.boost.org/doc/libs/release/libs/iterator/doc/new-iter-concepts.html) ([PDF](https://www.boost.org/doc/libs/release/libs/iterator/doc/new-iter-concepts.pdf))时，其中有关于`std::vector<bool>` 的这样的描述：

> For example, `vector<bool>::iterator` is almost a random access iterator, but the return type is not `bool&` (see [issue 96](http://www.open-std.org/JTC1/SC22/WG21/docs/lwg-active.html#96) and Herb Sutter's paper J16/99-0008 = WG21 N1185). Therefore, the iterators of `vector<bool>` only meet the requirements of input iterator and output iterator.



在下面文章中对此进行了解释：

### stackoverflow [Why is vector not a STL container?](https://stackoverflow.com/questions/17794569/why-is-vectorbool-not-a-stl-container)

> For space-optimization reasons, the C++ standard (as far back as C++98) explicitly calls out `vector<bool>` as a special standard container where each bool uses only one bit of space rather than one byte as a normal bool would (implementing a kind of "dynamic bitset"). In exchange for this optimization it doesn't offer all the capabilities and interface of a normal standard container.
>
> In this case, since you can't take the address of a bit within a byte, things such as `operator[]` can't return a `bool&` but instead return a proxy object that allows to manipulate the particular bit in question. Since this proxy object is not a `bool&`, you can't assign its address to a `bool*` like you could with the result of such an operator call on a "normal" container. In turn this means that `bool *pb =&v[0];` isn't valid code.
>
> On the other hand `deque` doesn't have any such specialization called out so each bool takes a byte and you can take the address of the value return from `operator[]`.
>
> Finally note that the MS standard library implementation is (arguably) suboptimal in that it uses a small chunk size for deques, which means that using deque as a substitute isn't always the right answer.

### [When Is a Container Not a Container?](http://www.gotw.ca/publications/mill09.htm)



## [On `vector<bool>` -- Howard Hinnant](https://isocpp.org/blog/2012/11/on-vectorbool)
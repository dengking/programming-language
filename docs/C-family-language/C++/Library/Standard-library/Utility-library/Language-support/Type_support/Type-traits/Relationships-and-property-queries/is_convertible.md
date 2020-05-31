# is_convertible



## cppreference [`std::is_convertible`, `std::is_nothrow_convertible`](https://en.cppreference.com/w/cpp/types/is_convertible)





在阅读[The Boost.Iterator Library](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/index.html)的[Standard Proposal For New-Style Iterators](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/new-iter-concepts.html) ([PDF](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/new-iter-concepts.pdf))，其中有这样的描述：

> This proposal also includes a concept for specifying when an iterator is interoperable with another iterator, in the sense that `int*` is interoperable with `int const*`.
>
> - Interoperable Iterators

后来阅读源代码，实现是这样的：

```c++
  template <typename A, typename B>
  struct is_interoperable
# ifdef BOOST_NO_STRICT_ITERATOR_INTEROPERABILITY
    : mpl::true_
# else
    : mpl::or_<
          is_convertible< A, B >
        , is_convertible< B, A > >
# endif
  {
  };
```



结合实现和前面聚的例子：`int*` is interoperable with `int const*`，我思考：CV qualified type和原type之间是否一定是convertible的？查了一下“is_convertible and const ”，下面是相关的：

[When would std::is_convertible_v be false?](https://stackoverflow.com/questions/58722313/when-would-stdis-convertible-vconst-type-type-be-false)
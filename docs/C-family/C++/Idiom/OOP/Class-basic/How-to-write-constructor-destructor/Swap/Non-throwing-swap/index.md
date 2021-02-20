# Non-throwing swap



## More C++ Idioms [Non-throwing swap](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-throwing_swap)

参见 `./More-C++Idioms-Non-throwing swap` 章节。

## CppCoreGuidelines [C.85: Make `swap` `noexcept`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-swap-noexcept)





## Non-throw swap idiom and move semantic

正如More C++ Idioms [Non-throwing swap](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Non-throwing_swap)的Solution and Sample Code所述：

> Non-throwing swap idiom uses [Handle Body](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Handle_Body) idiom to achieve the desired effect.

这种设计就使得它非常适合于move semantic，move semantic有着non-throw的特性，所以对它的swap是non-throw的，关于此，在下面的[Why implementing swap() as non-throwing](https://stackoverflow.com/questions/44042043/why-implementing-swap-as-non-throwing)中还会有描述。



## stackoverflow [Why implementing swap() as non-throwing](https://stackoverflow.com/questions/44042043/why-implementing-swap-as-non-throwing)

[A](https://stackoverflow.com/a/44042914)

> My question is **why** we should implement our `swap()` function as a **non-throwing** one

1、Because `swap` is completely useless if it might throw.

Consider: you `swap` two instances, and the operation throws. Now, what state are they in?

The strong guarantee is that there are no side-effects if an exception was thrown, meaning both original objects are left in their original state.

If we *can't* meet the strong guarantee, we simply can't use `swap` in many cases, because there's no way to recover usefully from a failure, and there's no point in writing that version of `swap` at all.

2、Because there's no reason for it to throw.

The trivial implementation of `swap` (now) uses **move-assignment** and **move-construction**.

There's generally no reason for a **move-constructor** to throw: it doesn't allocate anything new, it's just re-seating existing data. There's generally no reason for **move-assignment** to throw (as above), and destructors should never throw - and those are the only operations required.

> NOTE: 需要结合`std::move`的实现来看，gcc `std::swap`的源码路径：https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/move.h
>
> ```c++
>   /**
>    *  @brief Swaps two values.
>    *  @param  __a  A thing of arbitrary type.
>    *  @param  __b  Another thing of arbitrary type.
>    *  @return   Nothing.
>   */
>   template<typename _Tp>
>     _GLIBCXX20_CONSTEXPR
>     inline
> #if __cplusplus >= 201103L
>     typename enable_if<__and_<__not_<__is_tuple_like<_Tp>>,
> 			      is_move_constructible<_Tp>,
> 			      is_move_assignable<_Tp>>::value>::type
> #else
>     void
> #endif
>     swap(_Tp& __a, _Tp& __b)
>     _GLIBCXX_NOEXCEPT_IF(__and_<is_nothrow_move_constructible<_Tp>,
> 				is_nothrow_move_assignable<_Tp>>::value)
>     {
> #if __cplusplus < 201103L
>       // concept requirements
>       __glibcxx_function_requires(_SGIAssignableConcept<_Tp>)
> #endif
>       _Tp __tmp = _GLIBCXX_MOVE(__a);
>       __a = _GLIBCXX_MOVE(__b);
>       __b = _GLIBCXX_MOVE(__tmp);
>     }
> 
>   // _GLIBCXX_RESOLVE_LIB_DEFECTS
>   // DR 809. std::swap should be overloaded for array types.
>   /// Swap the contents of two arrays.
>   template<typename _Tp, size_t _Nm>
>     _GLIBCXX20_CONSTEXPR
>     inline
> #if __cplusplus >= 201103L
>     typename enable_if<__is_swappable<_Tp>::value>::type
> #else
>     void
> #endif
>     swap(_Tp (&__a)[_Nm], _Tp (&__b)[_Nm])
>     _GLIBCXX_NOEXCEPT_IF(__is_nothrow_swappable<_Tp>::value)
>     {
>       for (size_t __n = 0; __n < _Nm; ++__n)
> 	swap(__a[__n], __b[__n]);
>     }
> 
>   /// @} group utilities
> _GLIBCXX_END_NAMESPACE_VERSION
> } // namespace
> ```
>
> 可以看到，它充分发挥了move semantic，正如上面的描述所言。



## cpptruths [tale of noexcept swap for user defined](http://cpptruths.blogspot.com/2011/09/tale-of-noexcept-swap-for-user-defined.html)
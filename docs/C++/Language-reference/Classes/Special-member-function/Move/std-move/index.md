# `std::move`



## cppreference [std::move](https://en.cppreference.com/w/cpp/utility/move)

`std::move` is used to *indicate* that an object `t` may be "moved from", i.e. allowing the efficient transfer of resources from `t` to another object.

> NOTE: 上面这段话中的“indicate”如何来理解？在 https://stackoverflow.com/a/11540204 中对此进行了回答：
>
> > Sometimes, we want to move from **lvalues**. That is, sometimes we want the compiler to treat an **lvalue** as if it were an **rvalue**, so it can invoke the **move constructor**, even though it could be potentially unsafe. For this purpose, `C++11` offers a standard library function template called `std::move` inside the header `<utility>`. This name is a bit unfortunate, because `std::move` simply casts an **lvalue** to an **rvalue**; it does *not* move anything by itself. It merely *enables* moving. Maybe it should have been named `std::cast_to_rvalue` or `std::enable_move`, but we are stuck with the name by now.

In particular, `std::move` produces an [xvalue expression](../language/value_category.html) that identifies its argument `t`. It is exactly equivalent to a `static_cast` to an rvalue reference type.

> NOTE:  为什么`std::move` produces an [xvalue expression](../language/value_category.html) ？这要如何来解释？
>
> - 入参是lvalue reference to an object，`std::move`的返回值是一个rvalue reference to an object，则此时compiler就可以选择move semantic function，在这些function中，可以将这个object给move走；显然，这个object满足两个条件：`im`，所以它是一个xvalue
> - 入参是rvalue reference to an object，这种情况允许吗？是允许的，因为它的入参是forwarding reference；



## Implementation

### [gcc](https://github.com/gcc-mirror/gcc)/[libstdc++-v3](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3)/[include](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include)/[bits](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include/bits)/[move.h](https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/move.h)

```c++
/**
 *  @brief  Convert a value to an rvalue.
 *  @param  __t  A thing of arbitrary type.
 *  @return The parameter cast to an rvalue-reference to allow moving it.
 */
template<typename _Tp>
constexpr typename std::remove_reference<_Tp>::type&&
move(_Tp&& __t) noexcept
{	return static_cast<typename std::remove_reference<_Tp>::type&&>(__t);}
```

#### `constexpr`

说明它发生在compile-time；

#### Forwarding reference

入参：`_Tp&& __t`，说明它使用了forwarding / universal reference，所以它的入参既可以是rvalue reference也可以是lvalue reference，关于这一点，在文章bajamircea [std::move and std::forward](http://bajamircea.github.io/coding/cpp/2016/04/07/move-forward.html)中有描述：

> First of all `std::move` is a template with a `forwarding reference` argument which means that it can be called with either a `lvalue` or an `rvalue`, and the reference collapsing rules apply.

#### `static_cast`

cppreference [static_cast conversion](https://en.cppreference.com/w/cpp/language/static_cast)的`3`专门对此进行了描述；

#### `remove_reference`

在文章bajamircea [std::move and std::forward](http://bajamircea.github.io/coding/cpp/2016/04/07/move-forward.html)中有描述：

> The `remove_reference` template specializations are used to get the underlying type for `T` without any references, and that type is decorated with `&&` for the `static_cast` and return type.



在 https://stackoverflow.com/a/11540204 中，对`std::move` 的实现进行了分析。

> `std::move(some_lvalue)` casts an lvalue to an rvalue, thus enabling a subsequent move.

需要注意的是， 仅仅是rvalue，而不是prvalue，因为xvalue也是rvalue；



`C++11`给予programmer可以引用prvalue的权利，这就是rvalue reference，对于prvalue，programmer是可以安全地将其move走的，这是在C++语言级别支持的（compiler能够识别）。同时C++还给予了programmer将一些**可以安全地移走的glvalue**也move走的权利，这些**可以安全地移走的glvalue**就是**xvalue**，显然，这些**可以安全地移走的glvalue**即具备 **im** 属性，所以它，为了支持这个，C++语言做了如下变动：

- 引入了xvalue的概念，xvalue既可以归入glvalue，也可以归入rvalue，通过`std::move`，programmer告诉compiler将其当做rvalue来使用，以充分发挥move semantic
- rvalue reference 安全地move prvalue

rvalue reference的syntax是`&&`。

## C++ `std::move` does not **move** 

bajamircea [C++ std::move and std::forward](http://bajamircea.github.io/coding/cpp/2016/04/07/move-forward.html)：

> C++ `std::move` does not **move** and `std::forward` does not **forward**. 



## [What is std::move(), and when should it be used?](https://stackoverflow.com/questions/3413470/what-is-stdmove-and-when-should-it-be-used)






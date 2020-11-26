# `std::decay`



在阅读 arne-mertz [Modern C++ Features – decltype and std::declval](https://arne-mertz.de/2017/01/decltype-declval/) 时，其中有用到`std::decay_t`:

> But, also with C++14, we got `generic lambdas`. Those are basically lambdas with a templated function call operator, but we don’t get to declare any **template parameters**. Actually working with the type of whatever was passed to the lambda requires `decltype`:
>
> ```C++
> auto make_multiples = [](auto const& x, std::size_t n) { 
>   return std::vector<std::decay_t<decltype(x)>>(n, x); 
> };
> ```
>
> Here, `std::decay_t` will strip the `const&` from the type given by `decltype`, because `decltype(x)` will not result in what would have been `T` in a template, but in what would have been `T const&`.





## stackoverflow [What is std::decay and when it should be used?](https://stackoverflow.com/questions/25732386/what-is-stddecay-and-when-it-should-be-used)

> NOTE: 这篇文章的例子讲清楚了为什么需要使用`std::decay`。

**COMMENTS**: 

1) `decay_t<decltype(...)>` is a nice combination, to see what `auto` would deduce.

2) `std::decay()` can do three things. 

1 It is able to convert an array of `T` to `T*`; 

2 It can remove cv qualifier and reference; 

3 It converts function `T` to `T*`. e.g `decay(void(char))` -> `void(*)(char)`. 



### [A](https://stackoverflow.com/a/25732651)

`<joke>` It's obviously used to decay radioactive `std::atomic` types into non-radioactive ones.`</joke>` 

[N2609](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n2069.html) is the paper that proposed `std::decay`. The paper explains:

> Simply put, `decay<T>::type` is the **identity type-transformation** except if `T` is an array type or a reference to a function type. In those cases the `decay<T>::type` yields a pointer or a pointer to a function, respectively.

The motivating example is C++03 `std::make_pair`:

```cpp
template <class T1, class T2> 
inline pair<T1,T2> make_pair(T1 x, T2 y)
{ 
    return pair<T1,T2>(x, y); 
}
```

which accepted its parameter**s by value** to make string literals work:

```cpp
std::pair<std::string, int> p = make_pair("foo", 0);
```

If it accepted its parameters by reference, then `T1` will be deduced as an array type, and then constructing a `pair<T1, T2>` will be ill-formed.

But obviously this leads to significant inefficiencies. Hence the need for `decay`, to apply the set of transformations that occurs when pass-by-value occurs, allowing you to get the efficiency of taking the parameters by reference, but still get the type transformations needed for your code to work with string literals, array types, function types and the like:

```cpp
template <class T1, class T2> 
inline pair< typename decay<T1>::type, typename decay<T2>::type > 
make_pair(T1&& x, T2&& y)
{ 
    return pair< typename decay<T1>::type, 
                 typename decay<T2>::type >(std::forward<T1>(x), 
                                            std::forward<T2>(y)); 
}
```

**Note:** this is not the actual C++11 `make_pair` implementation - the C++11 `make_pair` also unwraps `std::reference_wrapper`s.



## [N2609](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n2069.html) Yet another type-trait: decay

The latest draft of the C++ standard ([n2009](http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2006/n2009.pdf)) includes a new section on type traits. The type traits provide a consistent and rich interface for compile-time type inspection and manipulation.

However, at least one very common trait is missing: `decay`. The purpose of decay is best seen in connection `std::make_pair()` which is defined as follows:

```C++
template <class T1, class T2> 
inline pair<T1,T2> make_pair(T1 x, T2 y)
{ 
    return pair<T1,T2>(x, y); 
}
```

The arguments are **passed by value** primarily to ensure string literals decay to `const char*` so as to make the following code work:

```C++
std::pair<std::string,int> p = std::make_pair("foo", 42);
```

If the arguments to `make_pair()` were passed **by const reference**, the above code would fail to compile because we suddenly create a temporary `pair` object of the type `std::pair<const char[4],int>` (and this instantiation fails because arrays are not constructible with the `T()` syntax, nor are they copy-constructible).

The problem with the current definition of `std::make_pair()` is that it leads to excessive **copying** of objects and hence great inefficiencies. With the advent of **Rvalue References** and **perfect forwarding** (see [n2027](http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2006/n2027.html)) we can easily remove all these inefficiencies:

```c++
template <class T1, class T2> 
inline pair<T1,T2> make_pair(T1&& x, T2&& y)
{ 
    return pair<T1, T2>(std::forward<T1>(x), std::forward<T2>(y)); 
}
```

But alas(悲哀), the above will still not compile and the reason is the same as before: we instantiate `pair` with an array type. However, with the proposed type-trait, the efficient version of `std::make_pair()` may be specified as follows:

```C++
template <class T1, class T2> 
inline pair< typename decay<T1>::type, typename decay<T2>::type > 
make_pair(T1&& x, T2&& y)
{ 
    return pair< typename decay<T1>::type, 
                 typename decay<T2>::type >(std::forward<T1>(x), 
                                            std::forward<T2>(y)); 
}
```

Simply put, `decay<T>::type` is the **identity type-transformation** except if `T` is an array type or a reference to a function type. In those cases the `decay<T>::type` yields a pointer or a pointer to a function, respectively.

I believe the reason [n1856](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2005/n1856.html) does not provide an efficient version of `std::make_pair()` is because of the lack of `decay`.

The type-trait has been part of boost since 1.33.0, and is used in libraries such as [Boost.Assign](http://www.boost.org/libs/assign/doc/index.html).

The implementation may be found [here](http://boost.cvs.sourceforge.net/boost/boost/boost/type_traits/decay.hpp?view=markup).

## cppreference [std::decay](https://en.cppreference.com/w/cpp/types/decay)

Applies 1) lvalue-to-rvalue, 2) array-to-pointer, and 3) function-to-pointer **implicit conversions** to the type `T`, removes cv-qualifiers, and defines the resulting type as the member typedef `type`.

> NOTE: cppreference [`implicit conversion`](https://en.cppreference.com/w/cpp/language/implicit_cast)

Formally:

- If `T` names the type "array of `U`" or "reference to array of `U`", the member typedef `type` is U*.

- Otherwise, if `T` is a function type `F` or a reference thereto, the member typedef `type` is [std::add_pointer](http://en.cppreference.com/w/cpp/types/add_pointer)<F>::type.

- Otherwise, the member typedef `type` is [std::remove_cv](http://en.cppreference.com/w/cpp/types/remove_cv) `<` [std::remove_reference](http://en.cppreference.com/w/cpp/types/remove_reference)`<T>::type>::type`.



These conversions model the **type conversion** applied to all **function arguments** when **passed by value**.

### Possible implementation

```C++
#include <type_traits>

template< class T >
struct decay {
private:
    typedef typename std::remove_reference<T>::type U;
public:
    typedef typename std::conditional<
        std::is_array<U>::value,
        typename std::remove_extent<U>::type*,
        typename std::conditional<
            std::is_function<U>::value,
            typename std::add_pointer<U>::type,
            typename std::remove_cv<U>::type
        >::type
    >::type type;
};
// g++ --std=c++11 test.cpp

```


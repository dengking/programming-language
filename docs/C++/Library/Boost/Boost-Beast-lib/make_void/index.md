# `make_void`

https://www.boost.org/doc/libs/master/boost/type_traits/make_void.hpp



```C++
template<class...>
struct make_void {
    typedef void type;
};

template<class... Ts>
using void_t = typename make_void<Ts...>::type;
```


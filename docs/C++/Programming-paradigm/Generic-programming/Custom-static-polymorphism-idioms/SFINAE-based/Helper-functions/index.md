# Helper functions

正如boost [enable_if](https://www.boost.org/doc/libs/1_73_0/libs/core/doc/html/core/enable_if.html) 中所述"The `enable_if` templates are tools for controlled creation of the **SFINAE conditions**"，这两个meta function都能够帮助定义SFINAE condition。

## `std::enable_if` VS `std::void_t`

### `std::enable_if`

condition 

passible implementation

```c++
template<bool B, class T = void>
struct enable_if_c {
    typedef T type;
};

template<class T>
struct enable_if_c<false, T> {
};

template<class Cond, class T = void>
struct enable_if : public enable_if_c<Cond::value, T> {
};
```



### `std::void_t`

statement



#### C++11 implementation

nlohmann [`void_t.hpp`](https://github.com/nlohmann/json/blob/develop/include/nlohmann/detail/meta/void_t.hpp) 

```C++
#pragma once

namespace nlohmann {
    namespace detail {
        template<typename ...Ts>
        struct make_void {
            using type = void;
        };
        template<typename ...Ts> using void_t = typename make_void<Ts...>::type;
    } // namespace detail
}  // namespace nlohmann

```



#### C++17 [alias templates](https://en.cppreference.com/w/cpp/language/type_alias) 

```C++
template< class... >
using void_t = void;
```

> NOTE: 需要注意的是，这种实现方式在C++11中是无法生效的，具体原因参见本节第一段。


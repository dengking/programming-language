# `std::any`

`std::any` type erasure，是对`void *`的增强。

> NOTE: 关于type erasure，参见`C++\Idiom\Type-Erasure`章节。

## microsoft [std::any: How, when, and why](https://devblogs.microsoft.com/cppblog/stdany-how-when-and-why/)



## bfilipek [Everything You Need to Know About std::any from C++17](https://www.bfilipek.com/2018/06/any.html)





## stackoverflow [When should I use std::any](https://stackoverflow.com/questions/52715219/when-should-i-use-stdany)



## cppreference [std::any](https://en.cppreference.com/w/cpp/utility/any)



## open-std [*Any* Library Proposal (Revision 3)](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3804.html)



## Implementation



### stackoverflow [std::any without RTTI, how does it work?](https://stackoverflow.com/questions/51361606/stdany-without-rtti-how-does-it-work)

```cpp
int main()
{   
    std::any x;
    x=9.9;
    std::cout << std::any_cast<double>(x) << std::endl;
}
```

But how `std::any` stores the type information? As I see, if I call `std::any_cast` with the "wrong" type I got `std::bad_any_cast` exception as expected.

> NOTE: 
>
> 非常神奇，相当于将类型存储起来了，然后运行时在使用。但是它的implementation可以不使用RTTI

#### comment

Boost has its own typeinfo that replaces RTTI, that's why `boost::any` does not need it. Generaly I do not see other possibility than implementing one's own typeinfo that does not depend on RTTI – [bartop](https://stackoverflow.com/users/3414900/bartop)

> NOTE: boost type_info source here: [github.com/boostorg/core/blob/develop/include/boost/core/…](https://github.com/boostorg/core/blob/develop/include/boost/core/typeinfo.hpp) enjoy :)

#### [A](https://stackoverflow.com/a/51362647/10173843)

`std::any` holds a pointer to a static member function of a templated class. This function can perform many operations and is specific to a given type since the actual instance of the function depends on the template arguments of the class.

> NOTE: 
>
> compile-time constant



### fluentcpp [How std::any Works](https://www.fluentcpp.com/2021/02/05/how-stdany-works/)

small object optimisation



### stackexchange [Implementation of std::any](https://codereview.stackexchange.com/questions/219075/implementation-of-stdany)





### [gcc](https://github.com/gcc-mirror/gcc)/[libstdc++-v3](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3)/[include](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include)/[std](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include/std)/[any](https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/std/any)



### github [thelink2012](https://github.com/thelink2012)/**[any](https://github.com/thelink2012/any)**

参见 `library-thelink2012-any` 章节。
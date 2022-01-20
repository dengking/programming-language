# std::integral_constant

是在阅读维基百科[Substitution failure is not an error#C++11 simplification](https://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error#C++11_simplification)，其中的例子使用`std::false_type`、`std::true_type`，后来查阅文档方知它们是是`typedef`

## cppreference [std::integral_constant](https://en.cppreference.com/w/cpp/types/integral_constant)

**std::integral_constant** wraps a static constant of specified type. It is the base class for the C++ type traits.

> NOTE: 帮助首先type trait



## `std::true_type` and `std::false_type`

### stackoverflow [What is std::false_type or std::true_type?](https://stackoverflow.com/questions/58694521/what-is-stdfalse-type-or-stdtrue-type)

> NOTE: 介绍的一般



## `std::integral_constant` over `constexpr`

### stackoverflow [When would I use std::integral_constant over constexpr?](https://stackoverflow.com/questions/20368187/when-would-i-use-stdintegral-constant-over-constexpr)

```c++
#include <iostream>
#include <type_traits>

int main(){

    //creating an integral constant with constexpr
    constexpr unsigned int speed_of_light{299792458};

    //creating an integral constant with std::integral_constant
    typedef std::integral_constant<unsigned int, 299792458> speed_of_light_2;

    //using them
    std::cout << speed_of_light/2 << '\n';
    std::cout << speed_of_light_2::value/2 << '\n';

}
```



[A](https://stackoverflow.com/a/20368362/10173843)

> NOTE: 总结的还是非常好的

Template `integral_constant` defines a type, keyword `constexpr` defines a constant. For example `std::true_type` is `std::integral_constant<bool, true>`.

One of the usage examples is `tag-dispatching`.

```C++
#include <type_traits>
#include <iostream>
 
template<typename T>
void use_impl(const T&, std::false_type)
{
	std::cout << "use_impl(false)" << std::endl;
}
 
template<typename T>
void use_impl(const T&, std::true_type)
{
	std::cout << "use_impl(true)" << std::endl;
}
 
template<typename T>
void use(const T& v)
{
   use_impl(v, typename std::is_integral<T>::type());
}
 
int main()
{
   use(1);
   use(1.2);
}
// g++ test.cpp --std=c++17 -pedantic -Wall -Wextra
```


# 关于本章

## SFINAE+trait+`enable_if`

SFINAE、trait、`enable_if`，三者是实现template metaprogramming的基础，实现上，往往是三者一起使用才能够发挥出c++的强大之处。

对三者总结的最好的文章是：boost [enable_if](https://www.boost.org/doc/libs/1_73_0/libs/core/doc/html/core/enable_if.html)，它讲到了精髓，下面是cppreference中对三者的总结:

cppreference `SFINAE#Library support`段中关于`enable_if`和SFINAE的总结，原文描述如下：

> The standard library component `std::enable_if` allows for creating a substitution failure in order to enable or disable particular overloads based on a condition evaluated at compile time.

cppreference `std::enable_if`段中关于`SFINAE `、trait的总结，原文描述如下：

> This metafunction is a convenient way to leverage [SFINAE](../language/sfinae.html) to conditionally remove functions from [overload resolution](../language/overload_resolution.html) based on type traits and to provide separate function overloads and specializations for different type traits. 

cppreference `SFINAE#Library support`段中关于trait和SFINAE的总结，原文描述如下：

> In addition, many [type traits](../types.html) are implemented using SFINAE.



SFINAE+trai+`enable_if`是C++ metaprogramming的精髓所在。



## `void_t` and SFINAE 

cppreference `SFINAE#Library support`段中的总结如下：

> The standard library component [std::void_t](../types/void_t.html) is another utility metafunction that simplifies SFINAE applications.



## TO READ

[Template SFINAE & type-traits](https://shaharmike.com/cpp/sfinae/)

 

[How to Make SFINAE Pretty – Part 2: the Hidden Beauty of SFINAE](https://www.fluentcpp.com/2018/05/18/make-sfinae-pretty-2-hidden-beauty-sfinae/)


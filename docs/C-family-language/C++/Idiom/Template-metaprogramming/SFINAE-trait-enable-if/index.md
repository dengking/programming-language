# 关于本章

SFINAE、trait、enable_if，三者是实现template metaprogramming的基础，实现上，往往是三者一起使用才能够发挥出c++的强大之处。

对三者总结的最好的文章是：boost [enable_if](https://www.boost.org/doc/libs/1_73_0/libs/core/doc/html/core/enable_if.html)，它讲到了精髓，下面是我对三者的总结：

`enable_if`基于trait来对SFINAE进行控制。

这是c++ metaprogramming的精髓所在。



## TO READ

[Template SFINAE & type-traits](https://shaharmike.com/cpp/sfinae/)

 

[How to Make SFINAE Pretty – Part 2: the Hidden Beauty of SFINAE](https://www.fluentcpp.com/2018/05/18/make-sfinae-pretty-2-hidden-beauty-sfinae/)


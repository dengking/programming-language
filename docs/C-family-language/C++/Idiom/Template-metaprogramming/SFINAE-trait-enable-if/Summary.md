# Summary

SFINAE、trait、enable_if，三者是实现template metaprogramming的基础，实现上，往往是三者一起使用才能够发挥出c++的强大之处。

对三者总结的最好的文章是：boost [enable_if](https://www.boost.org/doc/libs/1_73_0/libs/core/doc/html/core/enable_if.html)，它讲到了精髓，下面是我对三者的总结：

`enable_if`基于trait来对SFINAE进行控制。

这是c++ metaprogramming的精髓所在。

下面是一些典型的用法：

## Class template SFINAE

https://cpppatterns.com/patterns/class-template-sfinae.html

[Template specialization and enable_if problems [duplicate]](https://stackoverflow.com/questions/29502052/template-specialization-and-enable-if-problems)

## Function template SFINAE

[Using SFINAE to specialize template methods, depending on the type of container std::map or std::vector, passed as an argument](https://evileg.com/en/post/468/)



[Enable_if for template function specialization](https://stackoverflow.com/questions/48097889/enable-if-for-template-function-specialization)



[std::enable_if to conditionally compile a member function](https://stackoverflow.com/questions/6972368/stdenable-if-to-conditionally-compile-a-member-function)





## TO READ

[Template SFINAE & type-traits](https://shaharmike.com/cpp/sfinae/)

 
# 关于本章

c++的value semantic是不支持runtime polymorphsim的，reference semantic是支持runtime polymorphsim的，所以需要通过reference才能够实现runtime polymorphysim的。

c++的很多问题都是由它的value semantic而引入的，比如它的value category、reference。



value semantic：

https://stackoverflow.com/questions/3106110/what-is-move-semantics/3109981#3109981



https://www.zhihu.com/question/22111546



https://isocpp.org/wiki/faq/value-vs-ref-semantics#virt-data



http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2002/n1385.htm



https://herbsutter.com/2014/05/21/cppcon-my-proposed-talks-part-2/



http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2002/n1385.htm



https://marcbeauchesne.com/return-value-optimization-copy-elision/



https://www.fluentcpp.com/2016/11/28/return-value-optimizations/



https://stackoverflow.com/questions/18335861/why-is-enum-class-preferred-over-plain-enum



https://stackoverflow.com/questions/11711034/stdshared-ptr-of-this

https://stackoverflow.com/questions/142391/getting-a-boostshared-ptr-for-this



C++的value semantic，要求programmer来管理object的lifetime，这就引入了一系列问题：

- RAII：基于scope的lifetime管理

- MOVE：cross scope，即越过scope的限制

- dangling pointer、dangling reference

- smart pointer



等等，这些问题环绕中programmer。

目前我已经对这类问题进行了部分总结：

- GCC pure virtual method called

- 
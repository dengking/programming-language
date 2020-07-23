# Overload VS override VS name hiding

在阅读[How to Return a Smart Pointer AND Use Covariance](https://www.fluentcpp.com/2017/09/12/how-to-return-a-smart-pointer-and-use-covariance/)时，其中提及了name hiding，这让我想起了前段时间Google过overload by return type，显然c++是不允许overload by return type的。



## overload

[Overloading by return type](https://stackoverflow.com/questions/9568852/overloading-by-return-type)

[Why isn't the overloading with return types allowed? (at least in usually used languages)](https://softwareengineering.stackexchange.com/questions/317082/why-isnt-the-overloading-with-return-types-allowed-at-least-in-usually-used-l)

## override



## name hiding

在[How to Return a Smart Pointer AND Use Covariance](https://www.fluentcpp.com/2017/09/12/how-to-return-a-smart-pointer-and-use-covariance/)中，对name hiding的解释如下：

> And using a little (usually) annoying feature in C++, name hiding (i.e. when declaring a name in a derived class, this name hides all the symbols with the same name in the base class), we hide (not override) the `clone()` member function to return a smart pointer of the exact type we wanted.

[What Is Name Hiding in C++?](https://itinterviewguide.com/name-hiding/)

[Name hiding (C++ only)](https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.3.0/com.ibm.zos.v2r3.cbclx01/name_hiding.htm)



关于name hiding，在gotw [Namespaces and the Interface Principle](http://www.gotw.ca/publications/mill08.htm)中也对它进行了介绍，并且列举了具体的例子，以及对原因进行了比较深刻的剖析，在[The-interface-principle](./The-interface-principle.md)中，收录了这篇文章。




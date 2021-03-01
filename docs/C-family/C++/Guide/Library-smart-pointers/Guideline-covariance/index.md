# Smart pointer and covariance



## stackoverflow [How can I use covariant return types with smart pointers?](https://stackoverflow.com/questions/196733/how-can-i-use-covariant-return-types-with-smart-pointers)



[A](https://stackoverflow.com/a/197157)



Alternatively, you can use the visitor pattern or something like my [Dynamic Double Dispatch](http://www.ddj.com/dept/cpp/184429055) technique to pass a callback in to the returned object which can then invoke the callback with the correct type.

> NOTE: 
>
> 1、double dispatch，github [CppCodeReviewers](https://github.com/CppCodeReviewers)/**[Covariant-Return-Types-and-Smart-Pointers](https://github.com/CppCodeReviewers/Covariant-Return-Types-and-Smart-Pointers)** 中就是采用的这种思路



## 两篇文章给出的解决方案总结

1、github [CppCodeReviewers](https://github.com/CppCodeReviewers)/**[Covariant-Return-Types-and-Smart-Pointers](https://github.com/CppCodeReviewers/Covariant-Return-Types-and-Smart-Pointers)**

2、fluentcpp [How to Return a Smart Pointer AND Use Covariance](https://www.fluentcpp.com/2017/09/12/how-to-return-a-smart-pointer-and-use-covariance/)

上述两篇中，给出了实现方案，下面对它们的实现进行对比。

### 相同点

下面总结了两种实现方式的一些共性

1、下面两篇都使用virtual constructor来作为例子进行说明

2、都使用了CRTP

3、virtual clone implementation都没有public，都是因为使用了"raw pointer weak strong exception ownership transfer"，因此不将其public能够在一定程度上环节，并且virtual clone implementation都使用了C++原生的pointer covariance

4、都涉及构建class hierarchy

5、都涉及multiple polymorphic

### 不同点

下面是一些差异: 

1、github [CppCodeReviewers](https://github.com/CppCodeReviewers)/**[Covariant-Return-Types-and-Smart-Pointers](https://github.com/CppCodeReviewers/Covariant-Return-Types-and-Smart-Pointers)**

double dispatch/polymorphism，相对比较简单

2、fluentcpp [How to Return a Smart Pointer AND Use Covariance](https://www.fluentcpp.com/2017/09/12/how-to-return-a-smart-pointer-and-use-covariance/)

它的实现比较复杂



## TODO

1、arne-mertz.de [Covariance with Smart Pointers](https://arne-mertz.de/2016/05/covariant-smart-pointers/)


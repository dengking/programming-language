# Smart pointer and covariance



## stackoverflow [How can I use covariant return types with smart pointers?](https://stackoverflow.com/questions/196733/how-can-i-use-covariant-return-types-with-smart-pointers)



[A](https://stackoverflow.com/a/197157)



Alternatively, you can use the visitor pattern or something like my [Dynamic Double Dispatch](http://www.ddj.com/dept/cpp/184429055) technique to pass a callback in to the returned object which can then invoke the callback with the correct type.

> NOTE: 
>
> 1、double dispatch，github [CppCodeReviewers](https://github.com/CppCodeReviewers)/**[Covariant-Return-Types-and-Smart-Pointers](https://github.com/CppCodeReviewers/Covariant-Return-Types-and-Smart-Pointers)** 中就是采用的这种思路



## 解决方案

1、github [CppCodeReviewers](https://github.com/CppCodeReviewers)/**[Covariant-Return-Types-and-Smart-Pointers](https://github.com/CppCodeReviewers/Covariant-Return-Types-and-Smart-Pointers)**

double dispatch/polymorphism，相对比较简单

2、fluentcpp [How to Return a Smart Pointer AND Use Covariance](https://www.fluentcpp.com/2017/09/12/how-to-return-a-smart-pointer-and-use-covariance/)

比较复杂

## TODO

1、arne-mertz.de [Covariance with Smart Pointers](https://arne-mertz.de/2016/05/covariant-smart-pointers/)


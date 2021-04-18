# quuxplusone [What is Type Erasure?](https://quuxplusone.github.io/blog/2019/03/18/what-is-type-erasure/)

> NOTE: 
>
> 1、这篇文章，内容一般

## C++ type erasure is not Java type erasure

First of all, in some languages, such as Java, “type erasure” [means something completely different](https://docs.oracle.com/javase/tutorial/java/generics/erasure.html). In Java, it means the procedure applied by the compiler when you write a “generic” function — which looks deceptively similar to a C++ template, *but is not a template!*
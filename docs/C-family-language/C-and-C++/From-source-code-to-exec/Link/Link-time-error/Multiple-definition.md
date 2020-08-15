# Multiple definition





## [c++ linking error of multiple definition](https://ubuntuforums.org/showthread.php?t=1660856)



## [Solving multiple definition](http://www.cplusplus.com/forum/unices/5784/)



## [Error with multiple definitions of function](https://stackoverflow.com/questions/17904643/error-with-multiple-definitions-of-function)



## [问题解决： multiple definition of XXX](https://blog.csdn.net/liyuefeilong/article/details/44071053)





## 问题分析

当多个source file/translation unit中，包含有相同name的definition时，就会出现这个错误，这是因为c和c++中，遵循[ODR](https://en.cppreference.com/w/cpp/language/definition)。



TO READ:

https://stackoverflow.com/questions/17904643/error-with-multiple-definitions-of-function
# extern



## `extern "C"`

C、C++都是基于header file、source file的，C++要兼容C，但是C++的name mangling可能导致无法兼容，这就是`extern "C"`所要解决的问题。

cnblogs [extern “C”的作用详解](https://www.cnblogs.com/xiangtingshen/p/10980055.html) 这篇文章介绍地还可以。

cnblogs [extern "C"的用法解析](https://www.cnblogs.com/rollenholt/archive/2012/03/20/2409046.html) 这篇更好。

geeksforgeeks [Name Mangling and extern “C” in C++](https://www.geeksforgeeks.org/extern-c-in-c/) 结合了具体的例子，比较好。

通过在header file中，添加`extern "C"`的声明，可以是在C++中使用C。

反过来，在C中，可以使用C++吗？这个问题应该是：有些是可以的（比如function），但是有些无法实现的（比如OOP）。由于C++是C的超集，因此当混合使用两者的时候，需要使用g++来进行编译；

对于C++中想要暴露给C使用的，需要wrap为function，并且需要使用`extern "C"`。



本地的下面文章中涉及extern: 

`E:\github\dengking\programming-language-theory\programming-language\docs\C-family-language\C-and-C++\From-source-code-to-exec\ABI\Name-mangling\Name-mangling-and-extern.md`

`E:\github\dengking\programming-language-theory\programming-language\docs\C-family-language\C++\Pattern\Singleton\Extern-and-singleton.md`

`E:\github\dengking\programming-language-theory\programming-language\docs\C-family-language\C++\Language-reference\Namespace\Namespace-and-extern.md`



## practice: sqlite extension

我需要写一个sqlite extension，它需要被编译为一个shared library，由于这个shared library需要被dynamic load，因此它没有header file，无法对name mangling进行控制，所以它必须用C来实现。所以的C++，都需要进行wrap。
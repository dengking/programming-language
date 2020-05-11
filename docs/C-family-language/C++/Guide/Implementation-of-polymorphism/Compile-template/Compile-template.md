# Compile template

编译器如何编译template？这个问题其实是在探讨编译器是如何实现模板的。这个问题可以通过如下的方式进行Google：

- Google c++ template mechanism
- Google c++ compiler template

知道template的实现方式是非常重要的，因为目前我还在为template中，头文件包含的问题而困扰。

## [Why can templates only be implemented in the header file?](https://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file)

20200427: 对这个问题的分析，需要从compiler的compile model入手，在Why can’t I separate the definition of my templates class from its declaration and put it inside a .cpp file? [¶](https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl) [Δ](https://isocpp.org/wiki/faq/templates#)中对此的分析是非常好的。

## [C++: “undefined reference to” templated class function](https://bytefreaks.net/programming-2/c/c-undefined-reference-to-templated-class-function)



## [“Undefined symbols” linker error with simple template class](https://stackoverflow.com/questions/999358/undefined-symbols-linker-error-with-simple-template-class)



## [Template instantiation details of GCC and MS compilers](https://stackoverflow.com/questions/7182359/template-instantiation-details-of-gcc-and-ms-compilers)



## [Compiling c++ template is very slow.](https://gcc.gnu.org/legacy-ml/gcc/2018-03/msg00108.html)



## [Chapter 7 Compiling Templates](https://docs.oracle.com/cd/E19205-01/819-5267/bkage/index.html)



## [Template Compilation](https://stackoverflow.com/questions/19798325/template-compilation)



## https://accu.org/index.php/journals/427



## Substitution

compiler编译template的过程其实可以看做是substitution的过程，关于这一点，在下面的文章中提及了：

- [SFINAE and enable_if](https://eli.thegreenplace.net/2014/sfinae-and-enable_if/)
- [Substitution failure is not an error](https://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error)
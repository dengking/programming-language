# Reflection

C++ 支持:

- static reflection 
- dynamic reflection

## Static reflection

static reflection主要依赖于type trait，参见 `C++\Library\Standard-library\Utility-library\Language-support\Type-support` 。

https://gracicot.github.io/reflection/2018/04/03/reflection-present.html

https://woboq.com/blog/reflection-in-cpp-and-qt-moc.html

http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3814.html

### Static reflection and TMP、GP

Static reflection 是实现 TMP、GP的基础，因为它需要基于type来进行很多的处理。

## Dynamic reflection

1、Dynamic reflection主要依赖于RTTI，参见  `C++\Library\Standard-library\Utility-library\Language-support\Type-support` 。

2、无论哪种实现方式，本质上都是在类型中添加描述其类型信息的meta-data，对于python类的语言，它的语言本身是携带有这些信息的，而对于c++这样的语言，语言并没有内置这些数据，所以需要由programmer来创建这些信息。



## TODO

### gracicot.github.io [Reflection in C++](https://gracicot.github.io/reflection/2018/04/03/reflection-present.html)





### isocpp [N3996- Static reflection](https://isocpp.org/files/papers/n3996.pdf)





### [C++ Reflection in under 100 lines of code](http://pfultz2.com/blog/2012/07/31/reflection-in-under-100-lines/)





### open-std [Scalable Reflection in C++](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1240r0.pdf)



### preshing  [A Flexible Reflection System in C++](https://preshing.com/20180116/a-primitive-reflection-system-in-cpp-part-1/)




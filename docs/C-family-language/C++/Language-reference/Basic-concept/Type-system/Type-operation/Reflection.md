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

Dynamic reflection主要依赖于RTTI，参见  `C++\Library\Standard-library\Utility-library\Language-support\Type-support` 。


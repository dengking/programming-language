# Reflection 

C++ 支持:

- static reflection 
- dynamic reflection

## Static reflection

static reflection主要依赖于type trait。

https://gracicot.github.io/reflection/2018/04/03/reflection-present.html

https://woboq.com/blog/reflection-in-cpp-and-qt-moc.html

http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3814.html

### Metaprogramming and static reflection

template metaprogramming 是必须要支持reflection的，因为它需要基于type来进行很多的处理。

## Dynamic reflection

dynamic reflection主要依赖于RTTI。
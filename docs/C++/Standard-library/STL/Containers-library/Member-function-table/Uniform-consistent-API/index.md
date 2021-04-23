# Uniform consist API、interface

1、提供了统一的API

2、符合"program to abstraction and polymorphism"原则

3、AOP

## 分类 与 总结

从目前接触的各种API、interface

### Range access

cppreference 中将它放到了 "[Iterator library](https://en.cppreference.com/w/cpp/iterator) # Range access"章节。

### Element access

2、AOP、为所有的container都涉及相同的操作都添加上某个功能，比如为了bound safety需要为element access添加上bounds checking，`gsl::at`就是这样做的。


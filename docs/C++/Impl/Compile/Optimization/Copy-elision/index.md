# 关于本章

本章讨论Copy elision:

Copy elision是compiler optimization 的一种常用手段，显然它是avoid copy optimization technique在C++ compiler中的运用。在cppreference中，将"Copy-elision"放到了"Initialization"章节中，所以本章内容原本是放在 `C++\Language-reference\Initialization\Copy-elision` 章节的。

## Copy elision in function return value optimization

Copy elision是一种重要的optimization思路，在C++ implementation中，有着非常广泛的应用；其中非常重要的一个应用就是: function return value optimization:

1、RVO

2、NRVO

当然function return value optimization，除了使用copy elision外，还应用了一些其它的technique，在下一章节中会进行梳理。

## 参考文章

本章重要参考的文章有:

1、cppreference [Copy elision](https://en.cppreference.com/w/cpp/language/copy_elision)

2、stackoverflow [What are copy elision and return value optimization?](https://stackoverflow.com/questions/12953127/what-are-copy-elision-and-return-value-optimization)

3、cppreference [`return` statement](https://en.cppreference.com/w/cpp/language/return)

4、wikipedia [Copy elision](https://en.wikipedia.org/wiki/Copy_elision#Return_value_optimization)

本章对上述这些文章进行了一定的整理。



## wikipedia [Copy elision](https://en.wikipedia.org/wiki/Copy_elision#Return_value_optimization)

In [C++](https://en.wikipedia.org/wiki/C%2B%2B) [computer programming](https://en.wikipedia.org/wiki/Computer_programming), **copy elision** refers to a [compiler optimization](https://en.wikipedia.org/wiki/Compiler_optimization) technique that eliminates unnecessary [copying of objects](https://en.wikipedia.org/wiki/Object_copy). The `C++` language standard generally allows implementations to perform any optimization, provided the resulting program(优化后的程序)'s observable behavior is the same *as if*, i.e. pretending, the program were executed exactly as mandated by the standard.

> NOTE: (C ++语言标准通常允许实现执行任何优化，前提是结果程序的可观察行为与假定完全按照标准强制执行的程序相同)
>
> 思考：为什么要消除copy？
>
> copy elision其实省略的是copy constructor

The standard also describes a few situations where copying can be eliminated even if this would alter the program's behavior, the most common being the [return value optimization](https://en.wikipedia.org/wiki/Return_value_optimization). Another widely implemented optimization, described in the [C++ standard](https://en.wikipedia.org/wiki/ISO/IEC_14882), is when a [temporary object](https://en.wikipedia.org/wiki/Temporary_variable) of [class type](https://en.wikipedia.org/wiki/Class_(computing)) is copied to an object of the same type.[[1\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-C++03_12.8/15-1) As a result, *copy-initialization* is usually equivalent to *direct-initialization* in terms of performance, but not in semantics; *copy-initialization* still requires an [accessible](https://en.wikipedia.org/wiki/Member_accessibility) [copy constructor](https://en.wikipedia.org/wiki/Copy_constructor).[[2\]](https://en.wikipedia.org/wiki/Copy_elision#cite_note-moreexcept-2) The optimization can not be applied to a temporary object that has been bound to a reference.

### Example

```cpp
#include <iostream>

int n = 0;
struct C {
  explicit C(int) {}
  C(const C&) { ++n; } // the copy constructor has a visible side effect
};                     // it modifies an object with static storage duration

int main() {
  C c1(42); // direct-initialization, calls C::C(42)
  C c2 = C(42); // copy-initialization, calls C::C( C(42) )
                // temporary object is copied to an object
  std::cout << n << std::endl; // prints 0 if the copy was elided, 1 otherwise
  return 0;
}
```


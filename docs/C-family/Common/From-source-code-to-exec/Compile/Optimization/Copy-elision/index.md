# Copy elision

Copy elision是对compiler optimization principle的遵循，参见 `C-and-C++\From-source-code-to-exec\Compile\Optimization` 章节。

参见 `C++\Language-reference\Initialization\Copy-elision` 章节。



## 增强value semantic

C++如下是对value semantic的增强:

1、copy elision

2、move semantic

### by value and by reference

显然by value涉及到copy，并且构造的对象在函数调用结束后就会被release掉；但是by reference则不存在。

> This is not just a matter of convenience but in fact an optimization. If the parameter (s) binds to an **lvalue** (another non-const object), a copy of the object is made automatically while creating the parameter (s). However, when s binds to an rvalue (temporary object, literal), the copy is typically elided, which saves a call to a copy constructor and a destructor. In the earlier version of the assignment operator where the parameter is accepted as **const reference**, **copy elision** does not happen when the **reference** binds to an **rvalue**. This results in an additional object being created and destroyed.



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


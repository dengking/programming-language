# Construct On First Use



## [More C++ Idioms/Construct On First Use](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Construct_On_First_Use)

### Motivation

**Static objects** that have **non-trivial constructors** must be initialized before they are used. It is possible to access an uninitialized **non-local static object** before its initialization if proper care is not exercised.

> NOTE: **non-trivial constructors**

```c++
#include <iostream>
using namespace std;
struct Bar {
Bar () {
cout << "Bar::Bar()\n";
}
void f () {
cout << "Bar::f()\n";
}
};
struct Foo {
Foo () {
bar_.f ();
}
static Bar bar_;
};
Foo f;
Bar Foo::bar_; // definition of static member bar_
int main () 
{
    cout << "main\n";
}
```

> NOTE: 编译`g++ test.cpp`，输出如下：
>
> ```
> Bar::f()
> Bar::Bar()
> ```
>
> 显然，在构造函数执行之前，就已经执行了函数`f()`，显然这是错误的。
>
> 静态成员，内存应该已经分配了，直到运行的时候，才调用构造函数对它进行初始化。



## isocpp What’s the “`static` initialization order ‘fiasco’ (problem)”? [¶](https://isocpp.org/wiki/faq/ctors#static-init-order) [Δ](https://isocpp.org/wiki/faq/ctors#)
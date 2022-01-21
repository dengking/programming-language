# Static Initialization Order Fiasco

“Static Initialization Order Fiasco”即“静态初始化顺序失败”。

## What is Static Initialization Order Fiasco

在下面文章中，讨论了"Static Initialization Order Fiasco"

1、isocpp What’s the “`static` initialization order ‘fiasco’ (problem)”? [¶](https://isocpp.org/wiki/faq/ctors#static-init-order) [Δ](https://isocpp.org/wiki/faq/ctors#)

> To prevent the [static initialization order problem](https://isocpp.org/wiki/faq/ctors#static-init-order), use the *Construct On First Use Idiom*, described below.

2、参见 `Construct-On-First-Use-Idiom` 章节。

3、stackoverflow [extern vs Singleton class](https://stackoverflow.com/questions/12247912/extern-vs-singleton-class) # [A](https://stackoverflow.com/a/12248004)

4、pabloariasal [C++ - Initialization of Static Variables](https://pabloariasal.github.io/2020/01/02/static-variable-initialization/)



### stackoverflow [Prevent static initialization order “fiasco”, C++](https://stackoverflow.com/questions/29822181/prevent-static-initialization-order-fiasco-c)



[A](https://stackoverflow.com/a/29822275)

**The modern, more pattern-oriented way is \*not to use globals in the first place\*.**

There's no other way around it.

It wouldn't be much of a "fiasco", otherwise!

> NOTE: 关于global 和 singleton，参见 `Global-VS-singleton` 章节



## Example

1、`Construct-On-First-Use-Idiom`

2、zhihu [用std::once_flag来std::call_once，运行时却执行了2次？](https://zhuanlan.zhihu.com/p/408838350)



## TODO

stackoverflow [Finding C++ static initialization order problems](https://stackoverflow.com/questions/335369/finding-c-static-initialization-order-problems)


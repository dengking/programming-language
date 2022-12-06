# As-if rule

## zhihu [现代C++的内存模型](https://zhuanlan.zhihu.com/p/382372072)

为什么它们执行了一个不一样的程序，我们竟然不知情？其实，主要原因是编译器和CPU优化都遵循了一个同样的原则（[As-if rule](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/As-if_rule)），即优化后的代码，[若是单线程执行，要与原有代码行为保持一致](https://link.zhihu.com/?target=https%3A//preshing.com/20120625/memory-ordering-at-compile-time/)。再加之多线程环境我们使用的互斥锁，其对编译器和CPU优化做了很多限制，可以让我们对线程间的执行顺序进行同步，进而保证了即使被优化成了另一个程序，仍然有相同的执行结果。



## wikipedia [As-if rule](https://en.wikipedia.org/wiki/As-if_rule)

The [standard](https://en.wikipedia.org/wiki/C%2B%2B_standard) for the [C++](https://en.wikipedia.org/wiki/C%2B%2B) programming language allows [compilers](https://en.wikipedia.org/wiki/Compiler) for this language to apply any [optimizing](https://en.wikipedia.org/wiki/Compiler_optimization)[[1\]](https://en.wikipedia.org/wiki/As-if_rule#cite_note-1) transformation to a program during compilation, provided that such optimizations make no change in the "observable behavior" of the program, as specified in the standard; this mostly means that any actions the program performs on its environment occur in the specified order. This rule is commonly referred to as the **as-if rule**.[[2\]](https://en.wikipedia.org/wiki/As-if_rule#cite_note-2)

> NOTE: 
>
> 这对于理解很多compiler optimization是非常重要的





## cppreference [The as-if rule](https://en.cppreference.com/w/cpp/language/as_if)

### Explanation

> NOTE: 此处的解释是更加详细的。



## Exception

本段总结打破"As-if rule"的一些optimization，重要参考了如下文章:

1、wikipedia [As-if rule](https://en.wikipedia.org/wiki/As-if_rule)

2、cppreference [Copy elision](https://en.cppreference.com/w/cpp/language/copy_elision) # [Notes](https://en.cppreference.com/w/cpp/language/copy_elision#Notes) 

3、stackoverflow [What are copy elision and return value optimization?](https://stackoverflow.com/questions/12953127/what-are-copy-elision-and-return-value-optimization) # [A](https://stackoverflow.com/a/12953129)

### wikipedia [As-if rule](https://en.wikipedia.org/wiki/As-if_rule)

The rule has three main exceptions. 

The first is that programs exhibiting(展示) [undefined behavior](https://en.wikipedia.org/wiki/Undefined_behavior) are exempt(豁免的); since the observable behavior is not well-defined anyway, *any* transformation is valid. 

> NOTE: 首先，程序表现出不明确的行为是豁免的; 由于可观察行为没有明确定义，*任何*转换都是有效的。

The other two exceptions concern the copying of objects, and are called [copy elision](https://en.wikipedia.org/wiki/Copy_elision) and the [return value optimization](https://en.wikipedia.org/wiki/Return_value_optimization).

> NOTE: 通过下面的内容可知，上述总结是不全面的，或者说不是最新的

### cppreference [Copy elision](https://en.cppreference.com/w/cpp/language/copy_elision) # [Notes](https://en.cppreference.com/w/cpp/language/copy_elision#Notes) 

Copy elision is the only allowed form of optimization (until C++14)one of the two allowed forms of optimization, alongside [allocation elision and extension](https://en.cppreference.com/w/cpp/language/new#Allocation), (since C++14) that can change the observable side-effects. Because some compilers do not perform copy elision in every situation where it is allowed (e.g., in debug mode), programs that rely on the side-effects of copy/move constructors and destructors are not portable.

> NOTE: 上面这段话，初读是难以理解的，下面是注解版本:
>
> Optimization that can change the observable side-effects: 
>
> 一、until C++14
>
> 1、Copy elision 
>
> 二、since C++14
>
> 1、Copy elision 
>
> 2、[Allocation elision and extension](https://en.cppreference.com/w/cpp/language/new#Allocation)
>
> "Optimization that can change the observable side-effects"意味中打破As-if-rule，因此，这段话总结了compiler打破As-if rule的optimization。
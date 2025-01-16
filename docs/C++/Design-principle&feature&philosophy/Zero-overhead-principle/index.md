# Zero-overhead principle

1、C++的design是遵循Zero-overhead principle，作为programmer，在开发的时候，也需要遵循zero overhead principle，即"You don't pay for what you don't use"

2、C++ Zero-overhead principle其实就是optimization principle

## zhihu [每个C++工程师都要了解的十个性能陷阱](https://zhuanlan.zhihu.com/p/569174076)

C++“信徒”们常常鼓吹C++的“零成本抽象（Zero Cost Abstraction）”。然而对于“零成本抽象”这个概念存在很多误解。比如有的新手会认为：“使用C++的任何特性都没有成本”。那显然是大错特错的，比如使用模版就会导致编译时间变慢的编译期成本，而且我花了21天时间精通C++的时间成本也是成本啊（狗头）。有些经验的C++程序员会解释为”使用C++的任何特性都没有**运行时**成本“，这也是对C++最常见的误解。C++的创始人Bjarne Stroustrup是这样解释“零成本抽象“的：

1、你不会为任何你没有使用的特性付出任何成本。

2、对于你使用的特性，你只会付出最小运行时成本。

简单来说，就是C++不会背着你偷偷干坏事（比如垃圾回收），而你指定C++干的活，C++会尽量在编译期做，保证在运行期只会做“最少”的工作。连小学生都应该知道，“最少”并不等于“零”，所以“零成本抽象”其实是一种谎言，Google的C++负责人Chandler Carruth就曾经在CppCon 2019说过：[C++根本不存在”零成本抽象“](https://link.zhihu.com/?target=https%3A//www.youtube.com/watch%3Fv%3DrHIkrotSwcc)。

显然，C++的很多特性是有性能成本的，而且，这些成本往往出现在你“没有写”的代码里，即C++帮你添加的隐形代码。作为C++工程师，我们就必须了解每个特性会带来的性能损耗，在做代码设计时尽量选择损耗小的特性。

## isocpp [Big Picture Issues](https://isocpp.org/wiki/faq/big-picture) # [What is the zero-overhead principle?](https://isocpp.org/wiki/faq/big-picture#zero-overhead-principle)

The zero-overhead principle is a guiding principle for the design of C++. It states that: *What you don’t use, you don’t pay for* (in time or space) and further: *What you do use, you couldn’t hand code any better.*

In other words, no feature should be added to C++ which would make any existing code (not using the new feature) larger or slower, nor should any feature be added for which the compiler would generate code that is not as good as a programmer would create without using the feature.

## stackoverflow [What does 'Zero Cost Abstraction' mean?](https://stackoverflow.com/questions/69178380/what-does-zero-cost-abstraction-mean)

[A](https://stackoverflow.com/a/69178445/10173843)

Zero Cost Abstractions means adding higher-level programming concepts, like generics, collections and so on do not come with a run-time cost, only compiler time cost (the code will be slower to compile). Any operation on zero-cost abstractions is as fast as you would write out matching functionality by hand using lower-level programming concepts (for loops, counters, ifs and so on).

> NOTE: 
>
> 一、上面这段话是从compile time和run time来进行分析的，显然使用compile time实现的concept是"zero cost abstraction"

## cppreference [Zero-overhead principle](https://en.cppreference.com/w/cpp/language/Zero-overhead_principle)

The *zero-overhead principle* is a C++ design principle that states:

1、You don't pay for what you don't use.

2、What you do use is just as efficient as what you could reasonably write by hand.

In general, this means that no feature should be added to C++ that would impose any overhead, whether in time or space, greater than a programmer would introduce without using the feature.

The only two features in the language that do not follow the zero-overhead principle are [runtime type identification](https://en.cppreference.com/w/cpp/language/typeid) and [exceptions](https://en.cppreference.com/w/cpp/language/exceptions), and are why most compilers include a switch to turn them off.

## 例证

一、C++ data model: object的memory不会占用超过data member，C++没有额外导致它的扩大

二、static polymorphism

## See also

quora [**What are zero-cost abstractions in programming languages?**](https://www.quora.com/What-are-zero-cost-abstractions-in-programming-languages)
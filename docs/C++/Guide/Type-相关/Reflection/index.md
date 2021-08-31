# Reflection

按照 wikipedia [Reflection](https://en.wikipedia.org/wiki/Reflection_(computer_programming)) 中关于 reflection 的严格定义，C++其实并不能够直接支持reflection，如果我们从广义的角度来进行思考，会发现C++在一定程度是能够reflection的，本文从广义的reflection的角度对这个C++reflection进行讨论，根据static、dynamic进行划分:

1、static reflection 

2、dynamic reflection

## zhihu [C/C++有没有类似能实现反射、内省和代理之类的技术？](https://www.zhihu.com/question/30608733)



### [徐辰的回答](https://www.zhihu.com/question/30608733/answer/94982704) 

编译期的static reflection目前[已有提案](https://link.zhihu.com/?target=https%3A//isocpp.org/files/papers/n3996.pdf)，C++17当然是没戏了，不过20也许有希望。
运行期的reflection可能不太合C++的口味。

### [左轻侯的回答](https://www.zhihu.com/question/30608733/answer/49093857) 

我的C++知识基本上还停留在十几年前，不过Bjarne当年回答过类似的问题，我凭印象试着回答一下……

这个问题是，C++为什么不支持更多的RTTI（Run-Time Type Information，或Run-time type identification）特性？C++98对RTTI的支持是相当有限的，基本上只有typeid和dynamic_cast这两个operator。为什么不象Java/C#那样，增加对reflection和其它特性支持？

回答是（不是Bjarne的原文，是我的理解），C++是一门面向系统编程的语言，也许这样说不合适，那么，它是一门支持系统编程，实际上也被广泛地应用于系统编程的语言。C++程序员往往要尽可能地直接和操作系统甚至硬件打交道。这个定位决定了**C++只需要最小限度的runtime**，同样地，**只需要最小限度的RTTI支持**。一个庞大的runtime会在性能和可移植性方面付出很大的成本，这是和C++的宗旨相违背的。另一方面，C++的风格是尽可能通过库来实现功能，而在语法和编译器标准上非常谨慎。C++不喜欢编译器魔法。

当然，RTTI的一些特性是很难仅仅通过库来实现的。Bjarne并不反对C++的具体实现扩充RTTI特性。例如，Borland C++ Builder和C++/CLI都大幅度地增强了RTTI支持，以支持VCL/.NET framework。代价是移植性的丧失。

再次说明，这是来自2002年左右的回答，我不知道C++11在这方面有没有大的变动（好象是没有）。

## GP: Static reflection

一、Static reflection 是实现 TMP、GP的基础，因为它需要基于type来进行很多的处理。

二、static reflection主要依赖于各种GP 技巧，下面进行了简单的罗列:

1、type trait，参见 `C++\Library\Standard-library\Utility-library\Language-support\Type-support` 。

2、detection idiom

3、C++20 concept

4、SFINAE

三、Query-type

1、`decltype`，参见 `Query-type # decltype` 章节 

## Dynamic reflection

一、C++遵循"minimize runtime"设计原则，因此C++对dynamic reflection的支持是非常有限的:

1、对于polymorphic type，`typeid`

2、Dynamic reflection主要依赖于RTTI，参见  `C++\Library\Standard-library\Utility-library\Language-support\Type-support` 。

二、无论哪种实现方式，本质上都是在类型中添加描述其类型信息的meta-data，对于python类的语言，它的语言本身是携带有这些信息的，而对于c++这样的语言，语言并没有内置这些数据。

### 借助auxiliary compiler来实现dynamic reflection

在下面章节中，对此进行了讨论:

1、`Auxiliary-compiler`

2、`wikipedia-Reflection`

### 其他的实现方式

gracicot.github.io [Reflection in C++](https://gracicot.github.io/reflection/2018/04/03/reflection-present.html)

pfultz2 [C++ Reflection in under 100 lines of code](http://pfultz2.com/blog/2012/07/31/reflection-in-under-100-lines/)

preshing  [A Flexible Reflection System in C++](https://preshing.com/20180116/a-primitive-reflection-system-in-cpp-part-1/)

https://woboq.com/blog/reflection-in-cpp-and-qt-moc.html



## 提案

open-std [Scalable Reflection in C++](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1240r0.pdf)

isocpp [N3996- Static reflection](https://isocpp.org/files/papers/n3996.pdf)

open-std [Call for Compile-Time Reflection Proposals](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3814.html)


# Visitor pattern

“visitor”的意思是“访问者”，在Visitor pattern中，有专门的class来“扮演” “visitor”的角色。

## 维基百科-[Visitor pattern](https://en.wikipedia.org/wiki/Visitor_pattern)

> It is one way to follow the [open/closed principle](https://en.wikipedia.org/wiki/Open/closed_principle).

这句话提示我们visitor pattern的终极目标所在，即遵循 [open/closed principle](https://en.wikipedia.org/wiki/Open/closed_principle)。

## [Refactoring.Guru](https://refactoring.guru/)-[Visitor](https://refactoring.guru/design-patterns/visitor)

> **Visitor** is a behavioral design pattern that lets you separate algorithms from the objects on which they operate.

原文在Problem章节给出了不允许修改node classes的原因：

- The code was already in **production** and he（system architect） didn’t want to risk breaking it because of a potential bug in your changes.
- The primary job of these classes was to work with geodata. The XML export behavior would look alien there.
- to provide the ability to export into a different format

原文的Solution章节描述了如何使用visitor patter够解决上述问题。

需要解决的一个问题是：如何构建node class到export方法的映射。原文给出了两种方式：

- conditional
- [Double Dispatch](https://refactoring.guru/design-patterns/visitor-double-dispatch)

显然[Double Dispatch](https://refactoring.guru/design-patterns/visitor-double-dispatch)有着明显优势。

在原文的Problem章节提出的问题和Real-World Analogy章节提出的例子，存在着一定的共性，通过两者我们可以总结适合使用visitor pattern来解决的问题：

- 需要遍历不同类型的object
- 对不同类型的object需要执行特定的操作

其实原文的Intent章节中的配图已经形象的地展示出了visitor pattern。在原文的Applicability章节对此进行了总结。

不可能完全不修改node classes，而是少量修改，可控修改。
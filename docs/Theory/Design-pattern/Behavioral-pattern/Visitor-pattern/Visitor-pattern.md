# Visitor pattern

“visitor”的意思是“访问者”，在Visitor pattern中，有专门的class来“扮演” “visitor”的角色。

## 维基百科的[Visitor pattern](https://en.wikipedia.org/wiki/Visitor_pattern)

> It is one way to follow the [open/closed principle](https://en.wikipedia.org/wiki/Open/closed_principle).

这句话提示我们visitor pattern的终极目标所在，即遵循 [open/closed principle](https://en.wikipedia.org/wiki/Open/closed_principle)。

### [C++ example](https://en.wikipedia.org/wiki/Visitor_pattern#C++_example)

在阅读这段的例子的时候，我想到了一个问题：

`ArchivedFile`、`SplitFile`、`ExtractedFile`都是`File`的子类，它们都override了基类的`Accept`方法，可以看到它们的`Accept`方法的实现都是：

```c++
  void Accept(AbstractDispatcher& dispatcher) override {
    dispatcher.Dispatch(*this);
  }
```

那能否将这个方法放到基类中呢？应该是不能的，这就是double dispatch所解决的问题。

## [Refactoring.Guru](https://refactoring.guru/)的[Visitor](https://refactoring.guru/design-patterns/visitor)

> **Visitor** is a behavioral design pattern that lets you separate algorithms from the objects on which they operate.

在原文的Solution章节告诉了我们：由visitor类来实现“algorithm”

> The Visitor pattern suggests that you place the new behavior into a separate class called *visitor*, instead of trying to integrate it into existing classes. 

原文在Problem章节给出了不允许修改node classes的原因：

- The code was already in **production** and he（system architect） didn’t want to risk breaking it because of a potential bug in your changes.
- The primary job of these classes was to work with geodata. The XML export behavior would look alien there.
- to provide the ability to export into a different format



原文的Solution章节描述了如何使用visitor patter够解决上述问题。

需要解决的一个问题是：如何构建node class到export方法的映射，即为不同的node class指定对应的export方法。原文给出了两种方式：

- conditional，即通过`if else`语句来实现映射
- [Double Dispatch](https://refactoring.guru/design-patterns/visitor-double-dispatch)

显然[Double Dispatch](https://refactoring.guru/design-patterns/visitor-double-dispatch)有着明显优势，下面对此进行分析



### Double dispatch

> Instead of letting the client select a proper version of the method to call, how about we delegate this choice to objects we’re passing to the visitor as an argument? Since the objects know their own classes, they’ll be able to pick a proper method on the visitor less awkwardly. They “accept” a visitor and tell it what visiting method should be executed.

```pseudocode
// Client code
foreach (Node node in graph)
    node.accept(exportVisitor)

// City
class City is
    method accept(Visitor v) is
        v.doForCity(this)
    // ...

// Industry
class Industry is
    method accept(Visitor v) is
        v.doForIndustry(this)
    // ...
```





### Applicability

在原文的Problem章节提出的问题和Real-World Analogy章节提出的例子之间存在着一定的共性，通过两者我们可以总结适合使用visitor pattern来解决的问题：

- 需要遍历不同类型的object
- 对不同类型的object需要执行特定的操作

其实原文的Intent章节中的配图已经形象的地展示出了visitor pattern。在原文的Applicability章节对此进行了总结。



不可能完全不修改node classes，而是少量修改，可控修改。

## Example

### In compiler

在维基百科[Abstract syntax tree](https://en.wikipedia.org/wiki/Abstract_syntax_tree)的[Design patterns](https://en.wikipedia.org/wiki/Abstract_syntax_tree#Design_patterns)章节，总结了 [visitor pattern](https://en.wikipedia.org/wiki/Visitor_pattern) 在compiler的实现中的应用：

> Because the compiler traverses the tree several times to determine syntactic correctness, it is important to make traversing the tree a simple operation. The compiler executes a specific set of operations, depending on the type of each node, upon reaching it, so it often makes sense to use the [visitor pattern](https://en.wikipedia.org/wiki/Visitor_pattern).

这段话中的“the tree”只的是compiler构造的abstract syntax tree。compiler需要多次遍历这棵树，每次遍历执行一些操作，显然，这种场景是非常适合于使用visitor pattern的。

### Hierarchy structure

对具有hierarchy structure的数据进行操作，都可以使用visitor pattern。

比如

- [JSqlParser](https://github.com/JSQLParser/JSqlParser/wiki)
- [Mach7](https://github.com/solodon4/Mach7)
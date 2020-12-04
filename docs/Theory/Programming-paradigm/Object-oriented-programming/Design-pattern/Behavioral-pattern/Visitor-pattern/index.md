# Visitor pattern

## Principle in visitor pattern

Visitor pattern是典型的遵循:

1 separation of algorithm and structure principle

2 program to abstraction principle

Abstract visitor class抽象地表示behavior/algorithm。

遵循上述separation principle和 abstraction principle的visitor pattern能够实现:

1 open-closed：对于已知的structure(所有的node type是已知的)，可以扩展新的algorithm(添加新的visitor)，而无需修改structure。

2 `N * M` code reuse:  对于已有的structure，可以添加新的visitor，扩展新的algorithm

## 实现概括

下面是对它的概括: 

1 Traverse: 对structure进行traverse，以遍历(iteration)的方式来实现

> NOTE: 对structure的iteration是一种非常常见的computation，参见工程discrete的`Relation-structure-computation\Computation\Computation-on-structure`章节

2 Many-to-many: structure由多种类型的node组成(所有的node type是已知的)，对每种类型的node，可以执行多种类型的behavior/algorithm；每种behavior/algorithm，需要支持所有类型的node。

3 由visitor来抽象地表示behavior/algorithm，对每种类型的node，它定义了visiting method来统一描述对该类型node的的computation，显然，visitor class中，会定义一系列的visiting method: 

对于支持overload的programming language，显然这些visiting method的name可以一致；

对于不支持overload的programming language，显然这些visiting method的name无法一致；



### Dispatch in visitor pattern

由于是many-to-many关系，肯定需要进行两次dispatch:

1 abstract visitor to concrete visitor(select visitor implementation)，通常采用subtyping polymorphism来实现

2 为node选择visiting method(select visiting method)



下面是各种可能的实现方式: 

#### Conditional if + subtyping polymorphism

client 根据node type进行conditional if，select visiting method；

abstract visitor class的subtyping polymorphism

#### Double dispatch 

由node来`accept` abstract visitor object，在 node的`accept` method中，select proper visiting method: 

对于支持 function overload 的programming language: 

mix static polymorphism(function overload) 和 dynamic polymorphism(virtual function)

对于不支持 function overload 的programming language: 



这样做的好处是能够实现clean client；

## [Refactoring.Guru](https://refactoring.guru/) # [Visitor](https://refactoring.guru/design-patterns/visitor)

> NOTE: 这篇文章非常好，循序渐进地介绍visitor pattern

**Visitor** is a behavioral design pattern that lets you separate algorithms from the objects on which they operate.

> 在原文的Solution章节告诉了我们：由visitor类来实现“algorithm”

### Problem

> NOTE: 原文所描述的例子是: *Exporting the graph into XML*

At some point, you got a task to implement exporting the graph into XML format. At first, the job seemed pretty straightforward. You planned to add an `export` method to each node class and then leverage recursion to go over each node of the graph, executing the `export` method. The solution was simple and elegant: thanks to **polymorphism**, you weren’t coupling the code which called the export method to concrete classes of nodes.

> NOTE: 上诉思路是: 所有的node class都继承自同一个base node class，在这base node class中定义有`virtual export`方法，各个node subclass override `virtual export`方法。然后在上层application，遍历 graph，每个node都调用`export`方法。

Unfortunately, the **system architect** refused to allow you to alter existing node classes. He said that the code was already in production and he didn’t want to risk breaking it because of a potential bug in your changes.

Besides, he questioned whether it makes sense to have the XML export code within the node classes. The primary job of these classes was to work with geodata. The XML export behavior would look alien there.

There was another reason for the refusal. It was highly likely that after this feature was implemented, someone from the marketing department would ask you to provide the ability to export into a different format, or request some other weird stuff. This would force you to change those precious and fragile classes again.

> NOTE: 上述原因1、原因3是典型的 open/close principle。



![The XML export method had to be added into all node classes](./problem2-en.png)

> The XML `export` method had to be added into all node classes, which bore the risk of breaking the whole application if any bugs slipped through along with the change.

### Solution

> NOTE: 在visitor pattern中需要解决的一个问题是：如何构建node class到visiting method(`export`)的映射，即为不同的node class指定对应的export方法。原文给出了两种方式：
>
> - conditional，即通过`if else`语句来实现映射
> - [Double Dispatch](https://refactoring.guru/design-patterns/visitor-double-dispatch)
>
> 显然[Double Dispatch](https://refactoring.guru/design-patterns/visitor-double-dispatch)有着明显优势，下面对此进行分析

The **Visitor pattern** suggests that you place the new **behavior** into a separate class called *visitor*, instead of trying to integrate it into existing classes. The original object that had to perform the **behavior** is now passed to one of the visitor’s methods as an **argument**, providing the method access to all necessary data contained within the object.

> NOTE: 上诉 "behavior" 其实就是 algorithm。

Now, what if that behavior can be executed over objects of different classes? For example, in our case with XML export, the actual implementation will probably be a little bit different across various **node classes**. Thus, the visitor class may define not one, but a set of methods, each of which could take arguments of different types, like this:

> NOTE: 对每类node，可以指定不同的behavior/algorithm。

```C++
class ExportVisitor implements Visitor is
    method doForCity(City c) { ... }
    method doForIndustry(Industry f) { ... }
    method doForSightSeeing(SightSeeing ss) { ... }
    // ...
```

But how exactly would we call these methods, especially when dealing with the whole graph? These methods have different signatures, so we can’t use **polymorphism**. To pick a proper **`visitor`** method that’s able to process a given object, we’d need to check its class. Doesn’t this sound like a nightmare?

> NOTE: 由于 subtyping polymorphism 要求function signature的一致，所以"we can’t use **polymorphism**"。

```c++
foreach (Node node in graph)
    if (node instanceof City)
        exportVisitor.doForCity((City) node)
    if (node instanceof Industry)
        exportVisitor.doForIndustry((Industry) node)
    // ...
}
```

You might ask, why don’t we use **method overloading**? That’s when you give all methods the same name, even if they support different sets of parameters. Unfortunately, even assuming that our programming language supports it at all (as Java and `C#` do), it won’t help us. Since the exact class of a node object is unknown in advance, the **overloading mechanism** won’t be able to determine the correct method to execute. It’ll default to the method that takes an object of the base `Node` class.

> NOTE: 下面是采用overload的样例代码: 
>
> ```c++
> class ExportVisitor implements Visitor is
>     method do(City c) { ... }
>     method do(Industry f) { ... }
>     method do(SightSeeing ss) { ... }
>     // ...
>     
> foreach (Node node in graph)
> 	exportVisitor.do(node)
> 	// ...
> }    
> ```
>
> overload mechanism是static polymorphism，compiler只有知道准确的type才能够进行准确的static dispatch，在`foreach`中，`node`的static type是`Node`类型(abstract)，不是`City`、`Industry`、`SightSeeing`等 concrete type，因此compiler无法进行准确的static dispatch。
>
> 关于static type，参见 `C++\Language-reference\Basic-concept\Type-system\Type-system\OOP-class-type`



#### Double dispatch

However, the **Visitor pattern** addresses this problem. It uses a technique called [Double Dispatch](https://refactoring.guru/design-patterns/visitor-double-dispatch), which helps to execute the proper method on an object without cumbersome conditionals. 

Instead of letting the client **select** a proper version of the method to call, how about we **delegate** this choice to **objects** we’re passing to the **visitor** as an argument? Since the **objects** know their own classes, they’ll be able to pick a proper method on the **visitor** less awkwardly. They “**accept**” a visitor and tell it what **visiting method** should be executed.

> NOTE: 这段总结的非常好

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

## wikipedia [Visitor pattern](https://en.wikipedia.org/wiki/Visitor_pattern)

In [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming) and [software engineering](https://en.wikipedia.org/wiki/Software_engineering), the **visitor** [design pattern](https://en.wikipedia.org/wiki/Software_design_pattern) is a way of separating an [algorithm](https://en.wikipedia.org/wiki/Algorithm) from an [object](https://en.wikipedia.org/wiki/Object_(computer_science)) structure on which it operates. 

> NOTE: 参见`./Separation-of-algorithm-and structure`

A practical result of this separation is the ability to add new operations to existing object structures without modifying the structures. It is one way to follow the [open/closed principle](https://en.wikipedia.org/wiki/Open/closed_principle).

> NOTE: 这句话提示我们visitor pattern的终极目标所在，即遵循 [open/closed principle](https://en.wikipedia.org/wiki/Open/closed_principle)，visitor pattern中，open的是可以添加新的visitor，即实现新的algorithm，close的是对element的修改，即添加新的algorithm，不会导致element的变更。

### [C++ example](https://en.wikipedia.org/wiki/Visitor_pattern#C++_example)

```c++
#include <iostream>
#include <vector>

class AbstractDispatcher;  // Forward declare AbstractDispatcher

class File {  // Parent class for the elements (ArchivedFile, SplitFile and
              // ExtractedFile)
 public:
  // This function accepts an object of any class derived from
  // AbstractDispatcher and must be implemented in all derived classes
  virtual void Accept(AbstractDispatcher& dispatcher) = 0;
};

// Forward declare specific elements (files) to be dispatched
class ArchivedFile;
class SplitFile;
class ExtractedFile;

class AbstractDispatcher {  // Declares the interface for the dispatcher
 public:
  // Declare overloads for each kind of a file to dispatch
  virtual void Dispatch(ArchivedFile& file) = 0;
  virtual void Dispatch(SplitFile& file) = 0;
  virtual void Dispatch(ExtractedFile& file) = 0;
};

class ArchivedFile : public File {  // Specific element class #1
 public:
  // Resolved at runtime, it calls the dispatcher's overloaded function,
  // corresponding to ArchivedFile.
  void Accept(AbstractDispatcher& dispatcher) override {
    dispatcher.Dispatch(*this);
  }
};

class SplitFile : public File {  // Specific element class #2
 public:
  // Resolved at runtime, it calls the dispatcher's overloaded function,
  // corresponding to SplitFile.
  void Accept(AbstractDispatcher& dispatcher) override {
    dispatcher.Dispatch(*this);
  }
};

class ExtractedFile : public File {  // Specific element class #3
 public:
  // Resolved at runtime, it calls the dispatcher's overloaded function,
  // corresponding to ExtractedFile.
  void Accept(AbstractDispatcher& dispatcher) override {
    dispatcher.Dispatch(*this);
  }
};

class Dispatcher : public AbstractDispatcher {  // Implements dispatching of all
                                                // kind of elements (files)
 public:
  void Dispatch(ArchivedFile&) override {
    std::cout << "dispatching ArchivedFile" << std::endl;
  }

  void Dispatch(SplitFile&) override {
    std::cout << "dispatching SplitFile" << std::endl;
  }

  void Dispatch(ExtractedFile&) override {
    std::cout << "dispatching ExtractedFile" << std::endl;
  }
};

int main() {
  ArchivedFile archived_file;
  SplitFile split_file;
  ExtractedFile extracted_file;

  std::vector<File*> files = {
      &archived_file,
      &split_file,
      &extracted_file,
  };

  Dispatcher dispatcher;
  for (File* file : files) {
    file->Accept(dispatcher);
  }
}
```

> NOTE: 在阅读这段的例子的时候，我想到了一个问题：
>
> `ArchivedFile`、`SplitFile`、`ExtractedFile`都是`File`的子类，它们都override了基类的`Accept`方法，可以看到它们的`Accept`方法的实现都是：
>
> ```C++
>   void Accept(AbstractDispatcher& dispatcher) override {
>     dispatcher.Dispatch(*this);
>   }
> ```
>
> 那能否将这个方法放到基类中呢？应该是不能的，这就是double dispatch所解决的问题。





## stackoverflow [Visitor Pattern VS Iterator Pattern: visiting across hierarchy class?](https://stackoverflow.com/questions/28319129/visitor-pattern-vs-iterator-pattern-visiting-across-hierarchy-class)

### [A](https://stackoverflow.com/a/28321270)

这个回答是非常好的，在[Summary-of-design-pattern](../../Summary-of-design-pattern.md)中对它进行了分析。

## Visitor pattern and double dispatch

在visitor pattern中，由于存在着多种element，存在着多种visitor，所以最终到底要调用哪个algorithm，依赖于这两者，在文章[A polyglot's guide to multiple dispatch](https://eli.thegreenplace.net/2016/a-polyglots-guide-to-multiple-dispatch/)中对此进行了详细的分析。在[Multiple-dispatch](../../../Programming-paradigm/Object-oriented-programming/Polymorphism/Subtyping-polymorphism/Multiple-dispatch.md)中对此文章进行了收录。



## Example

### In compiler

#### wikipedia [Abstract syntax tree](https://en.wikipedia.org/wiki/Abstract_syntax_tree) # [Design patterns](https://en.wikipedia.org/wiki/Abstract_syntax_tree#Design_patterns)

在wikipedia [Abstract syntax tree](https://en.wikipedia.org/wiki/Abstract_syntax_tree)的[Design patterns](https://en.wikipedia.org/wiki/Abstract_syntax_tree#Design_patterns)章节，总结了 [visitor pattern](https://en.wikipedia.org/wiki/Visitor_pattern) 在compiler的实现中的应用：

> Because the compiler traverses the tree several times to determine syntactic correctness, it is important to make traversing the tree a simple operation. The compiler executes a specific set of operations, depending on the type of each node, upon reaching it, so it often makes sense to use the [visitor pattern](https://en.wikipedia.org/wiki/Visitor_pattern).

这段话中的“the tree”只的是compiler构造的abstract syntax tree。compiler需要多次遍历这棵树，每次遍历执行一些操作，显然，这种场景是非常适合于使用visitor pattern的。



#### stackoverflow [Visitor Pattern VS Iterator Pattern: visiting across hierarchy class?](https://stackoverflow.com/questions/28319129/visitor-pattern-vs-iterator-pattern-visiting-across-hierarchy-class)的[A](https://stackoverflow.com/a/37236417)

在这回答中给出了:

- LLVM [source code](http://llvm.org/docs/doxygen/html/InstVisitor_8h_source.html) 
- CPython  AST 

The other good example I know of shows up in abstract syntax tree manipulations. CPython and LLVM both use visitors. Using a visitor here prevents code that manipulates certain AST nodes from needing to know how to iterate over all the various AST nodes that might branch in complicated ways. 

### Hierarchy structure

对具有hierarchy structure的数据进行操作，都可以使用visitor pattern。

比如

- [JSqlParser](https://github.com/JSQLParser/JSqlParser/wiki)
- [Mach7](https://github.com/solodon4/Mach7)
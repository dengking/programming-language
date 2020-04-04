# Summary of design pattern



## Iterator pattern VS visitor pattern



Iterator pattern是指创建iterator以便于遍历container，visitor pattern则是指为不同类型的元素使用不同的算法。

正如在[refactoring Iterator](https://refactoring.guru/design-patterns/iterator)中所总结的：

> You can use [Visitor](https://refactoring.guru/design-patterns/visitor) along with [Iterator](https://refactoring.guru/design-patterns/iterator) to traverse a complex data structure and execute some operation over its elements, even if they all have different classes.



### [Visitor Pattern VS Iterator Pattern: visiting across hierarchy class?](https://stackoverflow.com/questions/28319129/visitor-pattern-vs-iterator-pattern-visiting-across-hierarchy-class)

#### [A](https://stackoverflow.com/a/28321270)

First, you should know what these patterns are for.

The **Iterator Pattern** is used to access an aggregate sequentially without exposing its underlying representation. So you could Hide a List or array or similar aggregates behind an Iterator.

Visitor Pattern is used to perform an action on a structure of elements without changing the implementation of the **elements** themselves.

So you use the patterns in two different situations and not as alternatives to each other.

In the **Visitor Pattern** you implement an Interface `IAcceptor` in each element you want to visit. So the **Visitor Pattern** doesn't rely on a **superclass** but on **Interfaces**

```c#
public interface IAcceptor
{
    public void Accept(IVisitor visitor);
}
```

So if you have a List of objects you can iterate over it and visit the objects implementing `IAcceptor`

```c#
public VisitorExample()
{
    MyVisitorImplementation visitor = new MyVisitorImplementation();
    List<object> objects = GetList();
    foreach(IAcceptor item in objects)
        item.Accept(visitor);
}


public interface IVisitor
{
    public void Visit(MyAcceptorImplementation item);
    public void Visit(AnotherAcceptorImplementation item);
}

public class MyAcceptorImplementation : IAcceptor
{ 
    //Some Code ...
    public void Accept(IVisitor visitor)
    {
        visitor.Visit(this);
    }
}
```

To complete the code here is Visitor to write to Console if it visits my or another implementation of an acceptor.

```c#
public class MyVisitorImplementation : IVisitor
{
        public void Visit(MyAcceptorImplementation item)
        {
            Console.WriteLine("Mine");
        }
        public void Visit(AnotherAcceptorImplementation item)
        {
            Console.WriteLine("Another");
        }
}
```

For more useful examples and better explanation have a look at [Visitor Pattern](http://www.dofactory.com/net/visitor-design-pattern) and [Iterator Pattern](http://www.dofactory.com/net/iterator-design-pattern)

EDIT: Here an example using both, the visitor and Iterator. The iterator is just the logic how to move through your aggregate. It would make more sense with a hierarchical structure.

```c#
public VisitorExample2()
{
    MyVisitorImplementation visitor = new MyVisitorImplementation();
    List<object> myListToHide = GetList();

    //Here you hide that the aggregate is a List<object>
    ConcreteIterator i = new ConcreteIterator(myListToHide);

    IAcceptor item = i.First();
    while(item != null)
    {
       item.Accept(visitor);
       item = i.Next();
    }
    //... do something with the result
}
```



#### [A](https://stackoverflow.com/a/37236417)

There are two good examples I know of where visitor is clearly preferable to iterator.

The first is interacting with some unknown set of class members, in particular in C++. For example, here's a visitor that prints out all the members of other classes. Imagine you're the author of `Printer` and someone you're unacquainted with is the author of `Heterogeneous3Tuple`.

```c++
#include <iostream>

template<class ElemType1, class ElemType2, class ElemType3>
class Heterogeneous3Tuple
{
public:
    Heterogeneous3Tuple(ElemType1 elem1, ElemType2 elem2, ElemType3 elem3)
        : elem1_(std::move(elem1)), elem2_(std::move(elem2)), elem3_(std::move(elem3))
    {}

    template<class Visitor>
    void accept(const Visitor& visitor)
    {
        visitor(elem1_);
        visitor(elem2_);
        visitor(elem3_);
    }

private:
        ElemType1 elem1_;
        ElemType2 elem2_;
        ElemType3 elem3_;
};

class Printer
{
public:
    template<class VisitedElemType>
    void operator()(const VisitedElemType& visitee) const
    {
        std::cout << visitee << std::endl;
    }

private:
};


int main() {
    Heterogeneous3Tuple<char, int, double> h3t('a', 0, 3.14);
    Printer p;
    h3t.accept(p);
}

a
0
3.14
```

[coliru](http://coliru.stacked-crooked.com/a/031dcab3719821a7)

There's no sensible way to get an iterator to work here. Without even knowing what types our `Printer` class might interact with this works so long as the visitor is `accept()`ed and the elements all interact in a similar way with `operator <<` and a stream.

The other good example I know of shows up in abstract syntax tree manipulations. CPython and LLVM both use visitors. Using a visitor here prevents code that manipulates certain AST nodes from needing to know how to iterate over all the various AST nodes that might branch in complicated ways. The LLVM [source code](http://llvm.org/docs/doxygen/html/InstVisitor_8h_source.html) goes into more detail. Here's the highlight:

```C++
/// Instruction visitors are used when you want to perform different actions
/// for different kinds of instructions without having to use lots of casts
/// and a big switch statement (in your code, that is).
///
/// To define your own visitor, inherit from this class, specifying your
/// new type for the 'SubClass' template parameter, and "override" visitXXX
/// functions in your class. I say "override" because this class is defined
/// in terms of statically resolved overloading, not virtual functions.
///
/// For example, here is a visitor that counts the number of malloc
/// instructions processed:
///
///  /// Declare the class.  Note that we derive from InstVisitor instantiated
///  /// with _our new subclasses_ type.
///  ///
///  struct CountAllocaVisitor : public InstVisitor<CountAllocaVisitor> {
///    unsigned Count;
///    CountAllocaVisitor() : Count(0) {}
///
///    void visitAllocaInst(AllocaInst &AI) { ++Count; }
///  };
///
///  And this class would be used like this:
///    CountAllocaVisitor CAV;
///    CAV.visit(function);
///    NumAllocas = CAV.Count;
```

### TO READ

[Visitor Pattern VS Iterator Pattern: visiting across hierarchy class? - iterator](https://html.developreference.com/article/19209136/Visitor+Pattern+VS+Iterator+Pattern%3a+visiting+across+hierarchy+class%3f)

[Combination of visitor and iterator pattern](https://softwareengineering.stackexchange.com/questions/386077/combination-of-visitor-and-iterator-pattern)

[Iterator vs Visitor Design Pattern and How](https://stackoverflow.com/questions/21933629/iterator-vs-visitor-design-pattern-and-how)



## Observer pattern and visitor pattern

### Observer pattern + Visitor pattern for message system

https://stackoverflow.com/questions/32079697/observer-pattern-visitor-pattern-for-message-system



## Design pattern让我们避免使用if-else分支

“Design pattern让我们避免使用if-else分支”，这是我学习了各种各样的设计模式后，产生的一种想法，在下面文章中，都谈到了这一点：

- [Java设计模式——状态模式（STATE PATTERN）](https://blog.csdn.net/u012401711/article/details/52675873)
- [Refactoring.Guru](https://refactoring.guru/)的[Visitor](https://refactoring.guru/design-patterns/visitor)

这让我反思，使用if-else的坏处：

- 如果情况少，还比较好处理，一旦情况非常多，那么无论是编程、还是维护都非常难
- 使用if是不好扩展的
- 性能（这一点需要证明），使用if条件判断，是否有dynamic dispatch或者static dispatch性能好呢？



design pattern充分利用dynamic dispatch和static dispatch，主要我们遵循design-by-context，那么就可以在不修改的design（抽象模型）的情况下，进行扩展，或者所，dispatch是建立抽象与具体的桥梁。
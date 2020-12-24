# Computation on structure

Visitor pattern非常适合用于computation on structure，visitor所对应的是一个function pointer、computation，下面是典型的例子。

## Examples

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

1) [JSqlParser](https://github.com/JSQLParser/JSqlParser/wiki)

2) [Mach7](https://github.com/solodon4/Mach7)

3) visitor pattern的一个例子是: 在Stack Overflow中列举的：遍历一个目录结构，然后对每个file/dir都执行一个操作

### [The Boost Graph Library (BGL)](https://www.boost.org/doc/libs/1_75_0/libs/graph/doc/index.html)

#### Extension through Visitors

Second, the graph algorithms of the BGL are extensible. The BGL introduces the notion of a *visitor*, which is just a function object with multiple methods. In graph algorithms, there are often several key “event points” at which it is useful to insert user-defined operations. The visitor object has a different method that is invoked at each event point. The particular event points and corresponding visitor methods depend on the particular algorithm. They often include methods like `start_vertex()`, `discover_vertex()`, `examine_edge()`, `tree_edge()`, and `finish_vertex()`.

### C++ `std::visit`



## Visitor pattern and FP(functional programming)

在client code，可以使用FP。


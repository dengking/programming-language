# Matching the Clang AST

在编写基于clang的tool的时候，非常重要的一个部分就是match clang AST节点。

matcher所匹配的是node。

## docs [Tutorial for building tools using LibTooling and LibASTMatchers](https://clang.llvm.org/docs/LibASTMatchersTutorial.html) # [Intermezzo: Learn AST matcher basics](https://clang.llvm.org/docs/LibASTMatchersTutorial.html#intermezzo-learn-ast-matcher-basics)

Clang recently introduced the [ASTMatcher library](https://clang.llvm.org/docs/LibASTMatchers.html) to provide a simple, powerful, and concise way to describe specific patterns in the AST. Implemented as a DSL powered by macros and templates (see [ASTMatchers.h](https://clang.llvm.org/doxygen/ASTMatchers_8h_source.html) if you’re curious), matchers offer the feel of algebraic data types common to functional programming languages.

> NOTE: 
>
> 这段话总结地非常好

```c++
binaryOperator(hasOperatorName("+"), hasLHS(integerLiteral(equals(0))))
```

> NOTE: 
>
> 匹配一个binary operator

```C++
variable(hasType(isInteger())).bind("intvar")
```

> NOTE: 
>
> 匹配一个int variable

All matcher that are nouns describe entities in the AST and can be bound, so that they can be referred to whenever a match is found. To do so, simply call the method `bind` on these matchers, e.g.:

```c++
variable(hasType(isInteger())).bind("intvar")
```



## docs [Matching the Clang AST](https://clang.llvm.org/docs/LibASTMatchers.html#matching-the-clang-ast)

> NOTE: 
>
> github [ahorn](https://github.com/ahorn)/**[native-symbolic-execution-clang](https://github.com/ahorn/native-symbolic-execution-clang)** 就是利用的[LibASTMatchers](http://clang.llvm.org/docs/LibASTMatchers.html) library





## docs [AST Matcher Reference](https://clang.llvm.org/docs/LibASTMatchersReference.html)

There are three different basic categories of matchers:

1、[Node Matchers:](https://clang.llvm.org/docs/LibASTMatchersReference.html#decl-matchers) Matchers that match a specific type of AST node.



2、[Narrowing Matchers:](https://clang.llvm.org/docs/LibASTMatchersReference.html#narrowing-matchers) Matchers that match attributes on AST nodes.



3、[Traversal Matchers:](https://clang.llvm.org/docs/LibASTMatchersReference.html#traversal-matchers) Matchers that allow traversal between AST nodes.



## code

https://clang.llvm.org/doxygen/ASTMatchers_8h_source.html



## Python binding

### [AndrewWalker](https://github.com/AndrewWalker)/**[clast](https://github.com/AndrewWalker/clast)**



## clang-query


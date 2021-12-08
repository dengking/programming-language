# `ASTFrontendAction`、`ASTConsumer`、`RecursiveASTVisitor`

设计思想介绍: 典型的IoC，显然是采用的visitor pattern，下面是具体的分析: 

|                | doxygen                                                      | 简介                                                         | scope、级别                                                  |
| -------------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| Action class   | [**ASTFrontendAction**](https://clang.llvm.org/doxygen/classclang_1_1ASTFrontendAction.html) | Action class的`CreateASTConsumer`是一个创造Consumer class object的factory method | 工具、application级别，即一个tool，会有一个对应的**`ASTFrontendAction`** 对象；它给予了programmer对不同的translation unit创建不同的consumer的能力 |
| Consumer class | [**ASTConsumer**](https://clang.llvm.org/doxygen/classclang_1_1ASTConsumer.html) | Consumer has-a Visitor                                       | translation unit级别，每个TU都会有一个对应的对象；           |
| Visitor class  | [**RecursiveASTVisitor**](https://clang.llvm.org/doxygen/classclang_1_1RecursiveASTVisitor.html) | "provides hooks of the form `bool VisitNodeType(NodeType *)` for most AST nodes" |                                                              |



## docs [How to write RecursiveASTVisitor based ASTFrontendActions](https://clang.llvm.org/docs/RAVFrontendAction.html#how-to-write-recursiveastvisitor-based-astfrontendactions)

In this tutorial you will learn how to create a `FrontendAction` that uses a `RecursiveASTVisitor` to find `CXXRecordDecl` AST nodes with a specified name.

### Creating a `FrontendAction`[¶](https://clang.llvm.org/docs/RAVFrontendAction.html#creating-a-frontendaction)

The only part left is to implement the `CreateASTConsumer` method that returns an `ASTConsumer` per translation unit.

> NOTE: 
>
> 一、每个translation unit有一个 `ASTConsumer` object，其实这对应了clang的"separate-compilation-model"，另外从他的

```C++
class FindNamedClassAction : public clang::ASTFrontendAction {
public:
  virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
    clang::CompilerInstance &Compiler, llvm::StringRef InFile) {
    return std::make_unique<FindNamedClassConsumer>();
  }
};
```

> NOTE: 
>
> `FindNamedClassAction::CreateASTConsumer` 就相当于 factory method

### Creating an `ASTConsumer`[¶](https://clang.llvm.org/docs/RAVFrontendAction.html#creating-an-astconsumer)

> NOTE: 
>
> 在上面有对它的介绍

```C++
class FindNamedClassConsumer : public clang::ASTConsumer {
public:
  virtual void HandleTranslationUnit(clang::ASTContext &Context) {
    // Traversing the translation unit decl via a RecursiveASTVisitor
    // will visit all nodes in the AST.
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }
private:
  // A RecursiveASTVisitor implementation.
  FindNamedClassVisitor Visitor;
};
```

> NOTE: 
>
> consumer has-a visitor

### Using the `RecursiveASTVisitor`[¶](https://clang.llvm.org/docs/RAVFrontendAction.html#using-the-recursiveastvisitor)

> NOTE: 
>
> 这一段给出的例子是非常好的

Let’s start by writing a `RecursiveASTVisitor` that visits all `CXXRecordDecl`’s.

```c++
class FindNamedClassVisitor
  : public RecursiveASTVisitor<FindNamedClassVisitor> {
public:
  bool VisitCXXRecordDecl(CXXRecordDecl *Declaration) {
    // For debugging, dumping the AST nodes will show which nodes are already
    // being visited.
    Declaration->dump();

    // The return value indicates whether we want the visitation to proceed.
    // Return false to stop the traversal of the AST.
    return true;
  }
};
```

In the methods of our `RecursiveASTVisitor` we can now use the full power of the Clang AST to drill through to the parts that are interesting for us. For example, to find all class declaration with a certain name, we can check for a specific qualified name:

```c++
bool VisitCXXRecordDecl(CXXRecordDecl *Declaration) {
  if (Declaration->getQualifiedNameAsString() == "n::m::C")
    Declaration->dump();
  return true;
}
```

### Accessing the `SourceManager` and `ASTContext`[¶](https://clang.llvm.org/docs/RAVFrontendAction.html#accessing-the-sourcemanager-and-astcontext)

Some of the information about the AST, like **source locations** and **global identifier information**, are not stored in the **AST nodes** themselves, but in the `ASTContext` and its associated source manager. To retrieve them we need to hand the `ASTContext` into our `RecursiveASTVisitor` implementation.



```C++
virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
  clang::CompilerInstance &Compiler, llvm::StringRef InFile) {
  return std::make_unique<FindNamedClassConsumer>(&Compiler.getASTContext());
}

bool VisitCXXRecordDecl(CXXRecordDecl *Declaration) {
  if (Declaration->getQualifiedNameAsString() == "n::m::C") {
    // getFullLoc uses the ASTContext's SourceManager to resolve the source
    // location and break it up into its line and column parts.
    FullSourceLoc FullLocation = Context->getFullLoc(Declaration->getBeginLoc());
    if (FullLocation.isValid())
      llvm::outs() << "Found declaration at "
                   << FullLocation.getSpellingLineNumber() << ":"
                   << FullLocation.getSpellingColumnNumber() << "\n";
  }
  return true;
}
```



## doxygen [**clang::ASTFrontendAction**](https://clang.llvm.org/doxygen/classclang_1_1ASTFrontendAction.html)

[ASTFrontendAction](https://clang.llvm.org/doxygen/classclang_1_1ASTFrontendAction.html) - Abstract base class to use for AST consumer-based frontend actions.

> NOTE: 
>
> 上面这段话中的 "consumer-based" 是什么含义？对应的是 [**ASTConsumer**](https://clang.llvm.org/doxygen/classclang_1_1ASTConsumer.html) 。



## doxygen  [**clang::ASTConsumer**](https://clang.llvm.org/doxygen/classclang_1_1ASTConsumer.html)



[ASTConsumer](https://clang.llvm.org/doxygen/classclang_1_1ASTConsumer.html) - This is an abstract interface that should be implemented by clients that read ASTs.

This abstraction layer allows the client to be independent of the AST producer (e.g. parser vs AST dump file reader, etc).
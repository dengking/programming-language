# Using Clang as a Library[¶](https://clang.llvm.org/docs/index.html#using-clang-as-a-library)

 

- [Choosing the Right Interface for Your Application](https://clang.llvm.org/docs/Tooling.html)
- [External Clang Examples](https://clang.llvm.org/docs/ExternalClangExamples.html)
- [Introduction to the Clang AST](https://clang.llvm.org/docs/IntroductionToTheClangAST.html)
- [LibTooling](https://clang.llvm.org/docs/LibTooling.html)
- [LibFormat](https://clang.llvm.org/docs/LibFormat.html)
- [Clang Plugins](https://clang.llvm.org/docs/ClangPlugins.html)
- [How to write RecursiveASTVisitor based ASTFrontendActions.](https://clang.llvm.org/docs/RAVFrontendAction.html)
- [Tutorial for building tools using LibTooling and LibASTMatchers](https://clang.llvm.org/docs/LibASTMatchersTutorial.html)
- [Matching the Clang AST](https://clang.llvm.org/docs/LibASTMatchers.html)
- [ASTImporter: Merging Clang ASTs](https://clang.llvm.org/docs/LibASTImporter.html)
- [How To Setup Clang Tooling For LLVM](https://clang.llvm.org/docs/HowToSetupToolingForLLVM.html)
- [JSON Compilation Database Format Specification](https://clang.llvm.org/docs/JSONCompilationDatabase.html)
- [Clang’s refactoring engine](https://clang.llvm.org/docs/RefactoringEngine.html)





## LibFormat[¶](https://clang.llvm.org/docs/LibFormat.html#libformat)



## Clang Plugins[¶](https://clang.llvm.org/docs/ClangPlugins.html#clang-plugins)

Clang Plugins run FrontendActions over code. See the [FrontendAction tutorial](https://clang.llvm.org/docs/RAVFrontendAction.html) on how to write a `FrontendAction` using the `RecursiveASTVisitor`. In this tutorial, we’ll demonstrate how to write a simple clang plugin.

> NOTE: 
>
> 关于`FrontendAction`，参加: llvm [clang::FrontendAction (**abstract**)](https://clang.llvm.org/doxygen/classclang_1_1FrontendAction.html)
>
> 



## How to write RecursiveASTVisitor based ASTFrontendActions.[¶](https://clang.llvm.org/docs/RAVFrontendAction.html#how-to-write-recursiveastvisitor-based-astfrontendactions)

> NOTE: 
>
> 显然是采用的visitor pattern
>
> |                |                                                              |                                                              |
> | -------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
> | Action class   | [**ASTFrontendAction**](https://clang.llvm.org/doxygen/classclang_1_1ASTFrontendAction.html) | Action class的`CreateASTConsumer`是一个创造Consumer class object的factory method |
> | Consumer class | [**ASTConsumer**](https://clang.llvm.org/doxygen/classclang_1_1ASTConsumer.html) | Consumer has-a Visitor                                       |
> | Visitor class  | [**RecursiveASTVisitor**](https://clang.llvm.org/doxygen/classclang_1_1RecursiveASTVisitor.html) |                                                              |
>
> [ASTFrontendAction](https://clang.llvm.org/doxygen/classclang_1_1ASTFrontendAction.html) - Abstract base class to use for AST consumer-based frontend actions.
>
> > NOTE: 
> >
> > 上面这段话中的 "consumer-based" 是什么含义？
>
> [ASTConsumer](https://clang.llvm.org/doxygen/classclang_1_1ASTConsumer.html) - This is an abstract interface that should be implemented by clients that read ASTs.
>
> This abstraction layer allows the client to be independent of the AST producer (e.g. parser vs AST dump file reader, etc).
>
> 
>
> 

In this tutorial you will learn how to create a `FrontendAction` that uses a `RecursiveASTVisitor` to find `CXXRecordDecl` AST nodes with a specified name.

### Creating a FrontendAction[¶](https://clang.llvm.org/docs/RAVFrontendAction.html#creating-a-frontendaction)



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

### Creating an ASTConsumer[¶](https://clang.llvm.org/docs/RAVFrontendAction.html#creating-an-astconsumer)

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

### Using the RecursiveASTVisitor[¶](https://clang.llvm.org/docs/RAVFrontendAction.html#using-the-recursiveastvisitor)

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

### Accessing the SourceManager and ASTContext[¶](https://clang.llvm.org/docs/RAVFrontendAction.html#accessing-the-sourcemanager-and-astcontext)

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



## Tutorial for building tools using LibTooling and LibASTMatchers[¶](https://clang.llvm.org/docs/LibASTMatchersTutorial.html#tutorial-for-building-tools-using-libtooling-and-libastmatchers)





## libclang comment

stackoverflow [How to extract comments and match to declaration with RecursiveASTVisitor in libclang c++?](https://stackoverflow.com/questions/25275212/how-to-extract-comments-and-match-to-declaration-with-recursiveastvisitor-in-lib)

[A](https://stackoverflow.com/a/41736499)

**"-fparse-all-comments"**



itbaoku [如何使用libclang c ++中的RecursiveASTVisitor提取注释并与声明匹配?](https://www.itbaoku.cn/post/1816672/How-to-extract-comments-and-match-to-declaration-with-RecursiveASTVisitor-in-libclang-c)

stackoverflow [Retrieving comments using python libclang](https://stackoverflow.com/questions/19079070/retrieving-comments-using-python-libclang)



## libclang  pre-processor

stackoverflow [Retrieve information about pre-processor directives](https://stackoverflow.com/questions/13881506/retrieve-information-about-pre-processor-directives)
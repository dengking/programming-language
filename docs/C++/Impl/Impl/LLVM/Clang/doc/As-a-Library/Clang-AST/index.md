# clang AST

## docs [Introduction to the Clang AST](https://clang.llvm.org/docs/IntroductionToTheClangAST.html)

> NOTE: 
>
> 既然是AST，显然它的组成就是node

Documentation for all Clang AST nodes is available via the generated [Doxygen](https://clang.llvm.org/doxygen).





## doxygen AST node

下面是各种各样的node: 

一、[TranslationUnitDecl](https://clang.llvm.org/doxygen/classclang_1_1TranslationUnitDecl.html) (translation unit)

二、[Decl](https://clang.llvm.org/doxygen/classclang_1_1Decl.html) (declarations )

This represents one declaration (or definition)



> NOTE: 
>
> 一、其中给出了非常详细的class hierarchy
>
> 二、definition也是属于declaration

三、[Stmt](https://clang.llvm.org/doxygen/classclang_1_1Stmt.html) (statements )

[Expr](https://clang.llvm.org/doxygen/classclang_1_1Expr.html) (expressions ) 

[Expr](https://clang.llvm.org/doxygen/classclang_1_1Expr.html)'s are subclasses of [Stmt](https://clang.llvm.org/doxygen/classclang_1_1Stmt.html). This allows an expression to be transparently used any place a [Stmt](https://clang.llvm.org/doxygen/classclang_1_1Stmt.html) is required



[DeclContext](https://clang.llvm.org/doxygen/classclang_1_1DeclContext.html) 

> NOTE: 
>
> 它表示的是一类declaration，这类declaration可以包含其他的declaration，比如namespace、class，通过它的class hierarchy是可以看出的
>
> 



## doxygen [Type](https://clang.llvm.org/doxygen/classclang_1_1Type.html)


# Scope

本文讨论c++的scope。

## cppreference [Scope](https://en.cppreference.com/w/cpp/language/scope)

> NOTE: 

### Block scope

> NOTE: block scope非常重要，它是后面将要介绍的这些scope的基石，后面会对其进行专门的介绍。

The potential scope of a variable introduced by a declaration in a [block (compound statement)](https://en.cppreference.com/w/cpp/language/statements#Compound_statements) begins at the point of declaration and ends at the end of the block. Actual scope is the same as potential scope unless there is a nested block with a declaration that introduces identical name

> NOET: 上面这一段引入了potential scope和actual scope的概念，这两个概念其实是为了描述具备nesting关系的两个scope中定义了两个相同的variable的情况。原文给出了例子。



### Function parameter scope



### Function scope

> NOTE: 原文特别强调了label。

### Namespace scope

The top-level scope of a translation unit ("file scope" or "global scope") is also a namespace and is properly called "global namespace scope". The potential scope of any entity declared in the global namespace scope begins at the declaration and continues to the end of the translation unit.

### Class scope

### Enumeration scope

### Template parameter scope

### Point of declaration



## Rule of c++ scope

关于rule of c++ scope，cppreference [Scope](https://en.cppreference.com/w/cpp/language/scope)虽然信息描述c++语言中的各种scope，但是并没有描述清楚这些scope之间的关系。我们已经知道，scope之间是具备nesting关系（参见工程[data-structure](https://dengking.github.io/data-structure/)）的，所以我们需要搞清楚c++的各种scope之间的nesting关系。

### Nesting relation of c++ scope

其实，梳理清楚c++的scope之间的关系非常简单：思考能否在一个scope中声明另外一个scope，比如：能否在function中定义class、能否在class中定义namespace。下面按照自顶向下的顺序来描述c++的scope的nesting关系：

1) file scope / global scope

2) namespace scope

3) class scope 

4) Enumeration scope

5) function scope

6) block scope

需要注意的是，上述的nesting关系仅仅描述了部分，而没有包含所有的。



## Block scope

c++中使用`{}`来定义block，也就是在c++中`{}`所包围的区域就是一个scope，c++是允许nesting block的。

按照这个原则，我们再来联系c++的function、class 、namespace 、Enumeration 的declaration，显然它们都是使用的`{}`，所以它们都是一个scope。



## Scope and object lifetime

关于scope和object lifetime，参见:

1) [Declarations](https://en.cppreference.com/w/cpp/language/declarations)的Notes段中有描述

2) 参见 `C++\Language-reference\Basic-concept\Object\Lifetime-and-storage-duration` 章节

### Scope and resource management

c++中的RAII就是基于scope的，所以c++的scope和resource management之间有着强烈的关联。
# Linkage 

一、需要从完整的编译过程（是指从source file到最终的object file）来分析linkage。和storage duration不同的是，linkage描述的是“name”，所以它既可以指代function，也可以指代object。	

二、linkage是建立translation unit之间的概念，参见`Phases-of-translation\Separate-compilation-model`。

## 编译模型

完整的编译过程在`Phases-of-translation`中描述的；

## What is linkage？

本节讨论what is linkage，参考如下文章:

1、microsoft [Translation units and linkage](https://docs.microsoft.com/en-us/cpp/cpp/program-and-linkage-cpp?view=vs-2019)

2、wikipedia [Linkage (software)](http://en.wikipedia.org/wiki/Linkage_(software))

3、cppreference [Storage class specifiers](https://en.cppreference.com/w/cpp/language/storage_duration)

linkage是和programming language的编译过程密切相关的，C和C++语言的 [translation unit](https://en.wikipedia.org/wiki/Translation_unit_(programming)) 是`.c`和`cpp`文件，即源代码文件，这些源代码文件经过preprocess、compile后生成`.o` object files，然后由[linker](https://en.wikipedia.org/wiki/Linker_(computing))对这些`.o` object files进行link等步骤，最后生成executable或者shared library，linkage是由设计者提供给programmer用于控制link步骤的；

## microsoft [Translation units and linkage](https://docs.microsoft.com/en-us/cpp/cpp/program-and-linkage-cpp?view=vs-2019)

A program consists of one or more *translation units*. A translation unit consists of an implementation file and all the headers that it includes directly or indirectly. Implementation files typically have a file extension of *cpp* or *cxx*. Header files typically have an extension of *h* or *hpp*. Each translation unit is compiled independently by the compiler. After the compilation is complete, the linker merges the compiled translation units into a single *program*. Violations of the ODR rule typically show up as linker errors. Linker errors occur when the same name has two different definitions in different translation units.

> NOTE: 
>
> 关于Violations of the ODR rule ，参见`./Link-time-error/Violations-of-ODR`章节

In some cases it may be necessary to declare a **global variable** or class in a *cpp* file. In those cases, you need a way to tell the compiler and linker what kind of *linkage* the name has. The type of **linkage** specifies whether the name of the object applies just to the **one file**, or to **all files**. The concept of **linkage** applies only to **global names**. The concept of linkage does not apply to names that are declared within a **scope**. A scope is specified by a set of enclosing braces such as in function or class definitions.

> NOTE: 
>
> 这段总结是非常好的:
>
> "The concept of **linkage** applies only to **global names**"
>
> 但是需要注意的是随着C++语言的发展，linkage也在不断地变化。

### External vs. internal linkage

A *free function* is a function that is defined at global or namespace scope. **Non-const global variables** and **free functions** by default have *external linkage*; they are visible from any **translation unit** in the program. Therefore, no other global object can have that name. A symbol with *internal linkage* or *no linkage* is visible only within the translation unit in which it is declared. When a name has **internal linkage**, the same name may exist in another translation unit. **Variables declared within class definitions or function bodies have no linkage**.

> NOTE: 
>
> 一、"**Variables declared within class definitions or function bodies have no linkage**"是和前面的"The concept of **linkage** applies only to **global names**"相对应的

## wikipedia [Linkage (software)](http://en.wikipedia.org/wiki/Linkage_(software))

In programming languages, particularly the compiled ones like [C](https://en.wikipedia.org/wiki/C_(programming_language)), [C++](https://en.wikipedia.org/wiki/C%2B%2B), and [D](https://en.wikipedia.org/wiki/D_(programming_language)), **linkage** describes how **names** can or can not refer to the same entity throughout the whole program or one single [translation unit](https://en.wikipedia.org/wiki/Translation_unit_(programming)).

> NOTE: 
>
> 一、
>
> external linkage： throughout the whole program；典型的例子就是“Example1”；
>
> internal linkage： one single [translation unit](https://en.wikipedia.org/wiki/Translation_unit_(programming))；典型的例子就是“Example2”；
>
> 对于automatic variable（automatic storage duration）只能够局限在一个block scope，而不是file scope，它是没有linkage的。
>
> 二、在[Translation unit (programming)](https://en.wikipedia.org/wiki/Translation_unit_(programming))中也谈到了linkage；其实这篇文章中对linkage的解释都偏向于translation unit，而没有说到点子上，而在[C Storage-class specifiers](https://en.cppreference.com/w/c/language/storage_duration)中对linkage的解释则比这篇文章更加清晰，建议去看其中对linkage的解释；

The [`static` keyword](https://en.wikipedia.org/wiki/Static_(keyword)) is used in C to restrict the visibility of a function or variable to its **translation unit**. This is also valid in `C++`. (`C++` 98/03 deprecated this usage in favor of [anonymous namespaces](https://en.wikipedia.org/wiki/Anonymous_namespace), but is no longer deprecated in C++ 11.) Also, `C++` implicitly treats any `const` namespace-scope variable as having **internal linkage** unless it is explicitly declared `extern`, unlike C.

> NOTE: 
>
> 上面总结了一些常用的linkage规则

A name's **linkage** is related to, but distinct from, its [scope](https://en.wikipedia.org/wiki/Scope_(programming)). The **scope** of a name is the part of a **translation unit** where it is visible. For instance, a name with **global scope** (which is the same as file-scope in C and the same as the global namespace-scope in C++) is visible in any part of the file. Its scope will end at the end of the translation unit, whether or not that name has been given external or **internal linkage**.



If the name has **external linkage**, the entity（可能是变量，可能是函数） that name denotes may be referred to from **another translation unit** using a **distinct declaration** for that **same name**（名字必须相同，declaration可以不同）, and from **other scopes** within the **same translation unit** using distinct declarations. 

Were the name given **internal linkage**, such a declaration would denote a distinct entity, although using the same name, but its entity could be referred to by distinct declarations within the same translation unit.

A name that **has no linkage** at all cannot be referred to from declarations in different scopes, not even from within the same **translation unit**. Examples of such names are parameters of functions and local variables. The details differ between C (where only objects and functions - but not types - have linkage) and `C++` and between this simplified overview.

> NOTE: 在 [C Storage-class specifiers](https://en.cppreference.com/w/c/language/storage_duration) 中对上面这段话的说明更加清晰一些，建议去看那里；

> NOTE: 通过`extern`来告诉compiler，这个name是一个external symbol；

Linkage between languages must be done with some care, as different languages [adorn](https://en.wikipedia.org/wiki/Name_mangling) their external symbols differently. A common idiom uses `extern "C"` to link C++ and C code.

> NOTE: 
>
> 上述“adorn”的含义是“装饰”，一个典型的例子就是C++的[name mangling](http://en.wikipedia.org/wiki/Name_mangling)。



## 从 definition 和 declaration 来思考 linkage

一个entity，可以有多个declaration，但是只能够有一个definition，这些declarations可以位于同一个translation unit也可以位于不同的translation unit；

在每个translation-unit中，都需要declare这个entity；在compile-time，compiler通过declaration来进行grammar校验；在link-time，linker通过合并object file来找到这个entity的definition；

一个name，要么是在本translation unit中定义，要么是在另外 一个translation unit中定义；如果一个name是在另外一个translation unit中定义的，那么在本[translation unit](https://en.wikipedia.org/wiki/Translation_unit_(programming))中它需要指定它为**external linkage**；

在learncpp [6.6 — Internal linkage](https://www.learncpp.com/cpp-tutorial/internal-linkage/)中对此进行了非常好的总结：

> In lesson [6.3 -- Local variables](https://www.learncpp.com/cpp-tutorial/local-variables/), we said, “An identifier’s linkage determines whether other declarations of that name refer to the same object or not”

## Classification of linkage and specifiers

对linkage的分类是以**translation unit**为单位的，正如[learncpp 6.6 — Internal linkage](https://www.learncpp.com/cpp-tutorial/internal-linkage/)

> An identifier with **internal linkage** can be seen and used within a single file, but it is not accessible from other files (that is, it is not exposed to the linker). This means that if two files have identically named identifiers with internal linkage, those identifiers will be treated as independent.



### cppreference [Storage class specifiers # Linkage](https://en.cppreference.com/w/cpp/language/storage_duration#Linkage)

A name that denotes object, reference, function, type, template, namespace, or value, may have ***linkage***. If a name has linkage, it refers to the **same entity** as the same name introduced by a declaration in another scope. If a variable, function, or another entity with the same name is declared in several scopes, but does not have sufficient linkage, then several instances of the entity are generated.

> NOTE: 
>
> 一、上面这段话其实涉及到了 once

#### No linkage

> NOTE: 
>
> local variable一般是no linkage的。

The name can be referred to only from the scope it is in.

Any of the following names declared at block scope have no linkage:

1、variables that aren't explicitly declared `extern` (regardless of the `static` modifier);

> NOTE: 
>
> 在local scope，只要没有 "explicitly declared `extern`"，不管是否declared `static` ，它都是no linkage

2、[local classes](https://en.cppreference.com/w/cpp/language/class#Local_classes) and their member functions;

3、other names declared at block scope such as typedefs, enumerations, and enumerators.

Names not specified with external, module(since C++20) or internal linkage also have no linkage, regardless of which scope they are declared in.

> NOTE: 
>
> 上面这段话是需要结合前面的内容来进行理解的:
>
> 1、microsoft [Translation units and linkage](https://docs.microsoft.com/en-us/cpp/cpp/program-and-linkage-cpp?view=vs-2019) 
>
> > **Non-const global variables** and **free functions** by default have *external linkage*; they are visible from any **translation unit** in the program.
>
> 

#### Internal linkage

The name can be referred to from all scopes in the current translation unit.

Any of the following names declared at namespace scope have internal linkage:

1、variables, variable templates (since C++14), functions, or function templates declared `static`;

2、non-volatile non-template (since C++14) non-inline (since C++17) non-[exported](https://en.cppreference.com/w/cpp/language/modules) (since C++20) [const-qualified](https://en.cppreference.com/w/cpp/language/cv) variables (including [constexpr](https://en.cppreference.com/w/cpp/language/constexpr)) that aren't declared `extern` and aren't previously declared to have external linkage;

3、data members of [anonymous unions](https://en.cppreference.com/w/cpp/language/union).

4、(since C++11) In addition, all names declared in [unnamed namespace](https://en.cppreference.com/w/cpp/language/namespace) or a namespace within an unnamed namespace, even ones explicitly declared `extern`, have internal linkage.

#### External linkage

The name can be referred to from the scopes in the other translation units. Variables and functions with external linkage also have [language linkage](https://en.cppreference.com/w/cpp/language/language_linkage), which makes it possible to link translation units written in different programming languages.

> NOTE: 
>
> 典型的跨语言

#### Module linkage(since C++20)

> NOTE: 
>
> 未阅读

#### Notes

Names at the top-level namespace scope (file scope in C) that are `const` and not `extern` have external linkage in C, but internal linkage in C++.

### see also

stackoverflow [What is external linkage and internal linkage?](https://stackoverflow.com/questions/1358400/what-is-external-linkage-and-internal-linkage)

geeksforgeeks [Internal Linkage and External Linkage in C](https://www.geeksforgeeks.org/internal-linkage-external-linkage-c/)

learncpp [6.6 — Internal linkage](https://www.learncpp.com/cpp-tutorial/internal-linkage/)

learncpp [6.7 — External linkage](https://www.learncpp.com/cpp-tutorial/external-linkage/)

http://www.goldsborough.me/c/c++/linker/2016/03/30/19-34-25-internal_and_external_linkage_in_c++/

## const and linkage

参考 `const-static-and-linkage`。



## `#include` and linkage  

在进入linkage的描述之前，需要首先区分linkage和 `#include`：

差异性：

1、`#include`发生于preprocess阶段，`#include`的对象是header file

2、linkage发生于compile、link阶段，link的对象是`.o` object files

相似性：从**关系**的角度来看，都是将多个file合并为一个file，即many-to-one

## Scope and linkage

在wikipedia [Linkage (software)](http://en.wikipedia.org/wiki/Linkage_(software))中，也对scope和linkage进行了对比。

scope是compile-time概念，它所描述的在一个translation unit内；

linkage是link-time概念，它描述的是多个object file之间；

compile-time发生在link-time之前，所以scope的检查发生在linkage的检查之前。



关于scope and linkage，在文章geeksforgeeks [Internal Linkage and External Linkage in C](https://www.geeksforgeeks.org/internal-linkage-external-linkage-c/)中对此进行了描述：

> In C and `C++`, a program that consists of multiple source code files is compiled *one at a time*. Until the compilation process, a variable can be described by it’s scope. It is only when the **linking process** starts, that **linkage property** comes into play. Thus, **scope is a property handled by compiler, whereas linkage is a property handled by linker.**
>
> The Linker links the resources together in the *linking* stage of compilation process. The [Linker](https://en.wikipedia.org/wiki/Linker_(computing)) is a program that takes multiple machine code files as input, and produces an executable object code. It resolves symbols (i.e, fetches definition of symbols such as “+” etc..) and arranges objects in **address space**.
>
> **Linkage** is a property that describes how variables should be linked by the **linker**. Should a variable be available for another file to use? Should a variable be used only in the file declared? Both are decided by **linkage**.
>
> **Linkage** thus allows you to couple names together on a per file basis, scope determines visibility of those names.

"因此，链接允许您在每个文件的基础上将名称耦合在一起（这句话的意思是通过linkage来将多个文件结合成同一个文件，更加直白的说其实是将多个不同文件在文件中的name合成同一个），范围确定这些名称的可见性（其实范围仅仅在一个**Translation Unit **中有效）"



## Examples

### Example1 

以下是我添加的一些例子。

```c
/* file demo1.c */
#include <stdio.h>
/* extern */
void foo(void); /* extern optional - it's the default */
extern int i;

int main(void)
{
	foo();
	printf("%d\n", i);
	return 0;
}

```

```c
/* file demo2.c */
#include<stdio.h>
int i = 0;
void foo(void)
{
	printf("hello world\n");
}

```

使用如下命令来进行编译：

```
gcc demo1.c demo2.c
```



可以`demo2.c`通过添加上`static`来将`i`和`foo`设置为internal linkage；如下：

```c
/* file demo2.c */
#include<stdio.h>
static int i=0;
static void foo(void)
{
  printf("hello world\n");
}
```

此时再进行编译，则进行编译就会报如下错误：

```c
/tmp/cc5DRif9.o：在函数‘main’中：
demo.c:(.text+0x5)：对‘foo’未定义的引用
demo.c:(.text+0xb)：对‘i’未定义的引用
collect2: 错误：ld 返回 1

```

### Example2

这个例子抄自geeksforgeeks [Internal Linkage and External Linkage in C](https://www.geeksforgeeks.org/internal-linkage-external-linkage-c/) 。

```c
// C code to illustrate External Linkage 
#include <stdio.h> 
  
int x = 10; 
int z = 5; 
  
int main() 
{ 
  
    extern int y; // line 2 
    extern int z; 
    printf("%d %d %d", x, y, z); 
} 
  
int y = 2; 
```



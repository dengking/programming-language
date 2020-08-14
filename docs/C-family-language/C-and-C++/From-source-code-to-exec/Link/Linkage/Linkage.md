# Linkage 

需要从完整的编译过程（是指从source file到最终的object file）来分析linkage。

## 编译模型

完整的编译过程在`C-and-C++\From-source-code-to-exec\Phases-of-translation.md`中描述的；

编译模型在`C-and-C++\From-source-code-to-exec\Compile\Compilation-model.md`中描述的；

## Linkage

linkage是和programming language的编译过程密切相关的，C和C++语言的 [translation unit](https://en.wikipedia.org/wiki/Translation_unit_(programming)) 是`.c`和`cpp`文件，即源代码文件，这些源代码文件经过preprocess、compile后生成`.o` object files，然后由[linker](https://en.wikipedia.org/wiki/Linker_(computing))对这些`.o` object files进行link等步骤，最后生成executable或者shared library；linkage是由设计者提供给programmer用于控制link步骤的；

### `#include` and linkage  

在进入linkage的描述之前，需要首先区分linkage和 `#include`：

差异性：

- `#include`发生于preprocess阶段，`#include`的对象是header file
- linkage发生于link阶段，linkage的对象是`.o` object files

相似性：

- 从**关系**的角度来看，都是将多个file合并为一个file，即many-to-one

### scope and linkage

在wikipedia [Linkage (software)](http://en.wikipedia.org/wiki/Linkage_(software))中，也对scope和linkage进行了对比。

scope是compile-time概念，它所描述的在一个translation unit内；

linkage是link-time概念，它描述的是多个object file之间；



关于这一点，在文章geeksforgeeks [Internal Linkage and External Linkage in C](https://www.geeksforgeeks.org/internal-linkage-external-linkage-c/)中对此进行了描述：

In C and `C++`, a program that consists of multiple source code files is compiled *one at a time*. Until the compilation process, a variable can be described by it’s scope. It is only when the **linking process** starts, that **linkage property** comes into play. Thus, **scope is a property handled by compiler, whereas linkage is a property handled by linker.**

> NOTE : 关于第一句话的理解参见[One-pass compiler](https://en.wikipedia.org/wiki/One-pass_compiler) 。

The Linker links the resources together in the *linking* stage of compilation process. The [Linker](https://en.wikipedia.org/wiki/Linker_(computing)) is a program that takes multiple machine code files as input, and produces an executable object code. It resolves symbols (i.e, fetches definition of symbols such as “+” etc..) and arranges objects in **address space**.

**Linkage** is a property that describes how variables should be linked by the **linker**. Should a variable be available for another file to use? Should a variable be used only in the file declared? Both are decided by **linkage**.

**Linkage** thus allows you to couple names together on a per file basis, scope determines visibility of those names.

> NOTE: 因此，链接允许您在每个文件的基础上将名称耦合在一起（这句话的意思是通过linkage来将多个文件结合成同一个文件，更加直白的说其实是将多个不同文件在文件中的name合成同一个），范围确定这些名称的可见性（其实范围仅仅在一个**Translation Unit **中有效）。



### wikipedia [Linkage (software)](http://en.wikipedia.org/wiki/Linkage_(software))

In programming languages, particularly the compiled ones like [C](https://en.wikipedia.org/wiki/C_(programming_language)), [C++](https://en.wikipedia.org/wiki/C%2B%2B), and [D](https://en.wikipedia.org/wiki/D_(programming_language)), **linkage** describes how **names** can or can not refer to the same entity throughout the whole program or one single [translation unit](https://en.wikipedia.org/wiki/Translation_unit_(programming)).

> NOTE: 
>
> external linkage： throughout the whole program；典型的例子就是“Example1”；
>
> internal linkage： one single [translation unit](https://en.wikipedia.org/wiki/Translation_unit_(programming))；典型的例子就是“Example2”；
>
> 对于automatic variable（automatic storage duration）只能够局限在一个block scope，而不是file scope，它是没有linkage的。

> NOTE: 在[Translation unit (programming)](https://en.wikipedia.org/wiki/Translation_unit_(programming))中也谈到了linkage；其实这篇文章中对linkage的解释都偏向于translation unit，而没有说到点子上，而在[C Storage-class specifiers](https://en.cppreference.com/w/c/language/storage_duration)中对linkage的解释则比这篇文章更加清晰，建议去看其中对linkage的解释；



The [`static` keyword](https://en.wikipedia.org/wiki/Static_(keyword)) is used in C to restrict the visibility of a function or variable to its **translation unit**. This is also valid in `C++`. (`C++` 98/03 deprecated this usage in favor of [anonymous namespaces](https://en.wikipedia.org/wiki/Anonymous_namespace), but is no longer deprecated in C++ 11.) Also, `C++` implicitly treats any `const` namespace-scope variable as having **internal linkage** unless it is explicitly declared `extern`, unlike C.

A name's **linkage** is related to, but distinct from, its [scope](https://en.wikipedia.org/wiki/Scope_(programming)). The **scope** of a name is the part of a **translation unit** where it is visible. For instance, a name with **global scope** (which is the same as file-scope in C and the same as the global namespace-scope in C++) is visible in any part of the file. Its scope will end at the end of the translation unit, whether or not that name has been given external or **internal linkage**.



If the name has **external linkage**, the entity（可能是变量，可能是函数） that name denotes may be referred to from **another translation unit** using a **distinct declaration** for that **same name**（名字必须相同，declaration可以不同）, and from **other scopes** within the **same translation unit** using distinct declarations. Were the name given **internal linkage**, such a declaration would denote a distinct entity, although using the same name, but its entity could be referred to by distinct declarations within the same translation unit. A name that **has no linkage** at all cannot be referred to from declarations in different scopes, not even from within the same **translation unit**. Examples of such names are parameters of functions and local variables. The details differ between C (where only objects and functions - but not types - have linkage) and `C++` and between this simplified overview.

> NOTE: 在[C Storage-class specifiers](https://en.cppreference.com/w/c/language/storage_duration)中对上面这段话的说明更加清晰一些，建议去看那里；

> NOTE: 通过`extern`来告诉compiler，这个name是一个external symbol；

Linkage between languages must be done with some care, as different languages [adorn](https://en.wikipedia.org/wiki/Name_mangling) their external symbols differently. A common idiom uses `extern "C"` to link C++ and C code.

> NOTE: 上述“adorn”的含义是“装饰”，一个典型的例子就是C++的[name mangling](http://en.wikipedia.org/wiki/Name_mangling)。



### linkage and storage duration specifiers

无论是`C++`还是C，都没有专门描述linkage的specifier，而是将描述**storage duration**和描述**linkage**的specifier合并在一起，对于linkage，并没有单独描述它的specifier，但是，compiler提供了default linkage；关于这一点，我们需要仔细阅读cppreference [Storage class specifiers](https://en.cppreference.com/w/cpp/language/storage_duration) 和 creference [Storage-class specifiers](https://en.cppreference.com/w/c/language/storage_duration)：

cppreference [Storage class specifiers](https://en.cppreference.com/w/cpp/language/storage_duration) 中，对specifiers的描述如下：

> The **storage class specifiers** are a part of the *decl-specifier-seq* of a name's [declaration syntax](https://en.cppreference.com/w/cpp/language/declarations). Together with the [scope](https://en.cppreference.com/w/cpp/language/scope) of the name, they control two independent properties of the name: its *storage duration* and its *linkage*.

在`C++`中，将这些specifier称为 storage class specifier。

creference [Storage-class specifiers](https://en.cppreference.com/w/c/language/storage_duration) 中，对specifiers的描述如下：

> Specify *storage duration* and *linkage* of objects and functions

在`C`中，将这些specifier称为 storage class specifier。



我们需要深入思考：为什么将linkage和storage duration的specifier合并？



另外需要注意的是， 对于function而言，它是否有storage duration？对于object而言，它既有linkage又有storage duration。对于function而言，讨论它的storage duration是没有意义的。



### Classification of linkage and specifiers

本文描述linkage的分类以及如何通过specifier来控制linkage。

参考：

- stackoverflow [What is external linkage and internal linkage?](https://stackoverflow.com/questions/1358400/what-is-external-linkage-and-internal-linkage)
- geeksforgeeks [Internal Linkage and External Linkage in C](https://www.geeksforgeeks.org/internal-linkage-external-linkage-c/)
- learncpp [6.6 — Internal linkage](https://www.learncpp.com/cpp-tutorial/internal-linkage/)
- learncpp [6.7 — External linkage](https://www.learncpp.com/cpp-tutorial/external-linkage/)

对linkage的分类是以translation unit为单位的，



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

这个例子抄自[Internal Linkage and External Linkage in C](https://www.geeksforgeeks.org/internal-linkage-external-linkage-c/) 。

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







## 从linkage来思考definition 和 declaration

一个entity，可以有多个declaration，但是只能够有一个definition；

在每个translation-unit中，都需要declare这个entity；在compile-time，compiler通过declaration来进行grammar校验；在link-time，linker通过合并object file来找到这个entity的definition；

一个name，要么是在本translation unit中定义，要么是在另外 一个translation unit中定义；如果一个name是在另外一个translation unit中定义的，那么在本[translation unit](https://en.wikipedia.org/wiki/Translation_unit_(programming))中它就是external linkage；

思考：ODR 的判断是发生于哪个阶段？

在learncpp [6.6 — Internal linkage](https://www.learncpp.com/cpp-tutorial/internal-linkage/)中对ODR进行了分析：



# THINKING

## linkage and linker

关于linkage和linker，在[Internal Linkage and External Linkage in C](https://www.geeksforgeeks.org/internal-linkage-external-linkage-c/)中给出了非常详细的分析；



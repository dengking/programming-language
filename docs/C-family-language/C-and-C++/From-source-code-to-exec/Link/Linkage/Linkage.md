# Linkage 

需要从完整的编译过程（是指从source file到最终的object file）来分析linkage。

## 编译模型

完整的编译过程在`C-and-C++\From-source-code-to-exec\Phases-of-translation.md`中描述的；

编译模型在`C-and-C++\From-source-code-to-exec\Compile\Compilation-model.md`中描述的；

## `#include` VS linkage  

在进入linkage的描述之前，需要首先区分linkage和 `#include`：

差异性：

- `#include`发生于preprocess阶段

- linkage发生于link阶段

相似性：

- 从关系的角度来看，都是将多个file合并为一个file，即many-to-one

## scope VS linkage

在wikipedia [Linkage (software)](http://en.wikipedia.org/wiki/Linkage_(software))中，也对scope和linkage进行了对比。

## Linkage

linkage是和programming language的编译过程密切相关的，C和C++语言的 [translation unit](https://en.wikipedia.org/wiki/Translation_unit_(programming)) 是`.c`和`cpp`文件，即源代码文件，这些源代码文件经过preprocess、compile后生成`.o`目标文件，然后由[linker](https://en.wikipedia.org/wiki/Linker_(computing))对这些目标文件进行link等步骤，最后生成executable或者shared library；linkage是由设计者提供给programmer用于控制link步骤的；



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

> NOTE : 
>
> scope是compile-time概念，它所描述的在一个translation unit内
>
> linkage是link-time概念，它描述的是多个object file之间
>
> 

If the name has **external linkage**, the entity（可能是变量，可能是函数） that name denotes may be referred to from **another translation unit** using a **distinct declaration** for that **same name**（名字必须相同，declaration可以不同）, and from **other scopes** within the **same translation unit** using distinct declarations. Were the name given **internal linkage**, such a declaration would denote a distinct entity, although using the same name, but its entity could be referred to by distinct declarations within the same translation unit. A name that **has no linkage** at all cannot be referred to from declarations in different scopes, not even from within the same **translation unit**. Examples of such names are parameters of functions and local variables. The details differ between C (where only objects and functions - but not types - have linkage) and `C++` and between this simplified overview.

> NOTE: 在[C Storage-class specifiers](https://en.cppreference.com/w/c/language/storage_duration)中对上面这段话的说明更加清晰一些，建议去看那里；

> NOTE: 通过`extern`来告诉compiler，这个name是一个external symbol；

Linkage between languages must be done with some care, as different languages [adorn](https://en.wikipedia.org/wiki/Name_mangling) their external symbols differently. A common idiom uses `extern "C"` to link C++ and C code.

> NOTE: 上述“adorn”的含义是“装饰”，一个典型的例子就是C++的[name mangling](http://en.wikipedia.org/wiki/Name_mangling)。

#### Linkage in C

Definition of 'linkage' quoted from ISO/IEC 9899:TC3 (C99 Standard). C uses the term "identifier" where this article uses "name" (the latter of which is what C++ uses to formalize linkage):

> An identifier declared in different scopes or in the same scope more than once can be made to refer to the same object or function by a process called linkage.[[1\]](https://en.wikipedia.org/wiki/Linkage_(software)#cite_note-C99-1)

The following is a common example of linkage:

```c
/* file demo1.c */
  
/* extern */ void foo(void); /* extern optional - it's the default */
  
int main(void)
{
  foo();
  return 0;  
}
/* file demo2.c */
  
void foo(void)
{
  ...
}
```

Function `foo` is declared in two files, with its function body defined in `demo2.c`. Via **linkage**, `foo` called in `main()` inside `demo1.c` refers to `foo` in `demo2.c`. This is an example of external linkage for a function.



### Classification

### stackoverflow [What is external linkage and internal linkage?](https://stackoverflow.com/questions/1358400/what-is-external-linkage-and-internal-linkage)



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

思考：ODR 的判断是发生于哪个阶段？link-time。



# THINKING

## linkage and linker

关于linkage和linker，在[Internal Linkage and External Linkage in C](https://www.geeksforgeeks.org/internal-linkage-external-linkage-c/)中给出了非常详细的分析；



## linkage and include

两者发生的阶段不同，linkage是发生在linker，而`#include`则发生在`preprocessor`。


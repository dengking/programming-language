# Linkage 

需要从完整的编译过程（是指从source file到最终的object file）来分析linkage。和storage duration不同的是，linkage描述的是“name”，所以它既可以指代function，也可以指代object。	

首先需要搞清楚的是，linkage涉及的是translation unit

## 编译模型

完整的编译过程在`C-and-C++\From-source-code-to-exec\Phases-of-translation.md`中描述的；

编译模型在`C-and-C++\From-source-code-to-exec\Compile\Compilation-model.md`中描述的；

## Linkage

linkage是和programming language的编译过程密切相关的，C和C++语言的 [translation unit](https://en.wikipedia.org/wiki/Translation_unit_(programming)) 是`.c`和`cpp`文件，即源代码文件，这些源代码文件经过preprocess、compile后生成`.o` object files，然后由[linker](https://en.wikipedia.org/wiki/Linker_(computing))对这些`.o` object files进行link等步骤，最后生成executable或者shared library；linkage是由设计者提供给programmer用于控制link步骤的；

### `#include` and linkage  

在进入linkage的描述之前，需要首先区分linkage和 `#include`：

差异性：

- `#include`发生于preprocess阶段，`#include`的对象是header file
- linkage发生于compile、link阶段，link的对象是`.o` object files

相似性：

- 从**关系**的角度来看，都是将多个file合并为一个file，即many-to-one

### scope and linkage

在wikipedia [Linkage (software)](http://en.wikipedia.org/wiki/Linkage_(software))中，也对scope和linkage进行了对比。

scope是compile-time概念，它所描述的在一个translation unit内；

linkage是link-time概念，它描述的是多个object file之间；

compile-time发生在link-time之前，所以scope的检查发生在linkage的检查之前。



关于scope and linkage，在文章geeksforgeeks [Internal Linkage and External Linkage in C](https://www.geeksforgeeks.org/internal-linkage-external-linkage-c/)中对此进行了描述：

In C and `C++`, a program that consists of multiple source code files is compiled *one at a time*. Until the compilation process, a variable can be described by it’s scope. It is only when the **linking process** starts, that **linkage property** comes into play. Thus, **scope is a property handled by compiler, whereas linkage is a property handled by linker.**

> NOTE : 关于第一句话的理解参见[One-pass compiler](https://en.wikipedia.org/wiki/One-pass_compiler) 。

The Linker links the resources together in the *linking* stage of compilation process. The [Linker](https://en.wikipedia.org/wiki/Linker_(computing)) is a program that takes multiple machine code files as input, and produces an executable object code. It resolves symbols (i.e, fetches definition of symbols such as “+” etc..) and arranges objects in **address space**.

**Linkage** is a property that describes how variables should be linked by the **linker**. Should a variable be available for another file to use? Should a variable be used only in the file declared? Both are decided by **linkage**.

**Linkage** thus allows you to couple names together on a per file basis, scope determines visibility of those names.

> NOTE: 因此，链接允许您在每个文件的基础上将名称耦合在一起（这句话的意思是通过linkage来将多个文件结合成同一个文件，更加直白的说其实是将多个不同文件在文件中的name合成同一个），范围确定这些名称的可见性（其实范围仅仅在一个**Translation Unit **中有效）。

### What is linkage？

本节讨论what is linkage。

#### wikipedia [Linkage (software)](http://en.wikipedia.org/wiki/Linkage_(software))

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



#### 从 definition 和 declaration 来思考 linkage

一个entity，可以有多个declaration，但是只能够有一个definition，这些declarations可以位于同一个translation unit也可以位于不同的translation unit；

在每个translation-unit中，都需要declare这个entity；在compile-time，compiler通过declaration来进行grammar校验；在link-time，linker通过合并object file来找到这个entity的definition；

一个name，要么是在本translation unit中定义，要么是在另外 一个translation unit中定义；如果一个name是在另外一个translation unit中定义的，那么在本[translation unit](https://en.wikipedia.org/wiki/Translation_unit_(programming))中它需要指定它为**external linkage**；

在learncpp [6.6 — Internal linkage](https://www.learncpp.com/cpp-tutorial/internal-linkage/)中对此进行了非常好的总结：

> In lesson [6.3 -- Local variables](https://www.learncpp.com/cpp-tutorial/local-variables/), we said, “An identifier’s linkage determines whether other declarations of that name refer to the same object or not”



### linkage and storage duration specifiers

关于此，在`C++\Language-reference\Basic-concept\Data-model\Object\Object-storage-duration-and-lifetime.md`中进行了详细描述。



### Classification of linkage and specifiers

本文描述linkage的分类以及如何通过specifier来控制linkage。

参考：

- stackoverflow [What is external linkage and internal linkage?](https://stackoverflow.com/questions/1358400/what-is-external-linkage-and-internal-linkage)
- geeksforgeeks [Internal Linkage and External Linkage in C](https://www.geeksforgeeks.org/internal-linkage-external-linkage-c/)
- learncpp [6.6 — Internal linkage](https://www.learncpp.com/cpp-tutorial/internal-linkage/)
- learncpp [6.7 — External linkage](https://www.learncpp.com/cpp-tutorial/external-linkage/)

对linkage的分类是以**translation unit**为单位的，正如[learncpp 6.6 — Internal linkage](https://www.learncpp.com/cpp-tutorial/internal-linkage/)

> An identifier with **internal linkage** can be seen and used within a single file, but it is not accessible from other files (that is, it is not exposed to the linker). This means that if two files have identically named identifiers with internal linkage, those identifiers will be treated as independent.

#### Internal linkage



#### External linkage



TO READ:

http://www.goldsborough.me/c/c++/linker/2016/03/30/19-34-25-internal_and_external_linkage_in_c++/

microsoft [Translation units and linkage](https://docs.microsoft.com/en-us/cpp/cpp/program-and-linkage-cpp?view=vs-2019)

https://aticleworld.com/linkage-in-c/



### const and linkage

参考[const-static-and-linkage](./const-static-and-linkage.md)。

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









## THINKING

### linkage and linker

关于linkage和linker，在geeksforgeeks [Internal Linkage and External Linkage in C](https://www.geeksforgeeks.org/internal-linkage-external-linkage-c/)中给出了非常详细的分析；

### [How does linker handle variables with different linkages?](https://stackoverflow.com/questions/51737002/how-does-linker-handle-variables-with-different-linkages)

In C and C++ we can manipulate a variable's **linkage**. There are three kinds of linkage: **no linkage**, **internal linkage**, and **external linkage**. My question is probably related to why these are called "linkage" (How is that related to the **linker**).

I understand a **linker** is able to handle variables with **external linkage**, because references to this variable is not confined（限制于） within a single **translation unit**, therefore not confined within a single **object file**. How that actually works under the hood is typically discussed in courses on operating systems.

But how does the linker handle variables (1) with no linkage and (2) with internal linkage? What are the differences in these two cases?

***COMMENTS*** 

The **linker** does not handle variables with internal or no linkage. The compiler can generate code to access those types of variables without any help from the linker. (Linkage and linker are not closely related concepts.) – [markgz](https://stackoverflow.com/users/583570/markgz) [Aug 8 '18 at 0:42](https://stackoverflow.com/questions/51737002/how-does-linker-handle-variables-with-different-linkages#comment90434932_51737002)



#### [A](https://stackoverflow.com/a/51737215)

The **linker** isn't normally involved in either **internal linkage** or **no linkage**--they're resolved entirely by the **compiler**, before the **linker** gets into the act at all.

**Internal linkage** means two declarations at different scopes in the same **translation unit** can refer to the same thing.

##### No Linkage

No linkage means two declarations at different scopes in the same translation unit can't refer to the same thing.

So, if I have something like:

```c
int f() { T
    static int x; // no linkage
}
```

...no other declaration of `x` in any other scope can refer to this `x`. The linker is involved only to the degree that it typically has to produce a field in the executable telling it the **size** of static space needed by the executable, and that will include space for this variable. Since it can never be referred to by any other declaration, there's no need for the linker to get involved beyond that though (in particular, the linker has nothing to do with resolving the name).

##### Internal linkage

Internal linkage means declarations at different scopes in the same translation unit can refer to the same object. For example:

```C++
static int x;  // a namespace scope, so `x` has internal linkage

int f() { 
    extern int x; // declaration in one scope
}

int g() { 
    extern int x; // declaration in another scope
}
```

Assuming we put these all in one file (i.e., they end up as a single translation unit), the declarations in both `f()` and `g()` refer to the same thing--the `x` that's defined as `static` at namespace scope.

For example, consider code like this:

```c++
#include <iostream>

static int x; // a namespace scope, so `x` has internal linkage

int f()
{
    extern int x;
    ++x;
}

int g()
{
    extern int x;
    std::cout << x << '\n';
}

int main() {
    g();
    f();
    g();
}
```

This will print:

```
0
1
```

...because the `x` being incremented in `f()` is the same `x` that's being printed in `g()`.

The linker's involvement（参与） here can be (and usually is) pretty much the same as in the **no linkage** case--the variable `x` needs some space, and the linker specifies that space when it creates the executable. It does *not*, however, need to get involved in determining that when `f()` and `g()` both declare `x`, they're referring to the same `x`--the compiler can determine that.

We can see this in the generated code. For example, if we compile the code above with `gcc`, the relevant bits for `f()` and `g()` are these.

f:

```assembly
    movl    _ZL1x(%rip), %eax
    addl    $1, %eax
    movl    %eax, _ZL1x(%rip)
```

That's the increment of `x` (it uses the name `_ZL1x` for it).

g:

```assembly
    movl    _ZL1x(%rip), %eax
    [...]
    call    _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_c@PLT
```

So that's basically loading up `x`, then sending it to `std::cout` (I've left out code for other parameters we don't care about here).

The important part is that the code refers to `_ZL1x`--the same name as `f` used, so both of them refer to the same object.

The **linker** isn't really involved, because all it sees is that this file has requested space for one statically allocated variable. It makes space for that, but doesn't have to do anything to make `f` and `g` refer to the same thing--that's already handled by the compiler.



#### [A](https://stackoverflow.com/a/51737185)

As far as C++ itself goes, this does not matter: the only thing that matters is the behavior of the system as a whole. Variables with no linkage should not be linked; variables with internal linkage should not be linked across translation units; and variables with external linkage should be linked across translation units. (Of course, as the person writing the C++ code, you must obey all of *your*constraints as well.)

Inside a compiler and linker suite of programs, however, we certainly *do* have to care about this. The method by which we achieve the desired result is up to us. One traditional method is pretty simple:

- Identifiers with no linkage are never even passed through to the linker.
- Identifiers with internal linkage are not passed through to the linker either, or *are* passed through to the linker but marked "for use within this one translation unit only". That is, there is no `.global`declaration for them, or there is a `.local` declaration for them, or similar.
- Identifiers with external linkage are passed through to the linker, and if internal linkage identifiers are seen by the linker, these external linkage symbols are marked differently, e.g., have a `.global` declaration or no `.local` declaration.

If you have a Linux or Unix like system, run `nm` on object (`.o`) files produced by the compiler. Note that some symbols are annotated with uppercase letters like `T` and `D` for text and data: these are global. Other symbols are annotated with lowercase letters like `t` and `d`: these are local. So these systems are using the "pass internal linkage to the linker, but mark them differently from external linkage" method.


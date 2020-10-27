# Declaration and definition

在文章[language](https://dengking.github.io/Post/Language/Language/)中，我们已经总结了，我们作为programmer在使用programming language来“描述”世界，这里的描述，使用更加专业的属于是declaration 和 definition，大多数programming language都会涉及到definition的概念，像c和c++这样的语言，区分declaration和definition，而像python和C++这样的语言，则不区分，是否区分，有利有弊，后面我们会进行分析。



## What declaration and definition mean for compiler?



有代码如下：
`a.c`

```C++
void test_a()//定义函数test_a
{
     return;
}
```

`b.c`

```C++
void test_a();

void test_b()
{
    test_a();    // 由于上面已经声明了，所以可以使用 
}

void main()
{
    test_b();
}
```

编译命令如下

```C++
gcc a.c b.c -o b
```

按照这种写法是可以正确编译出可执行文件b的。

如果我将编译命令修改为如下形式：

```shell
gcc b.c -o b
```

则编译器会报如下错误：

```
/tmp/cc91umxf.o：在函数‘test_b’中：
b.c:(.text+0xa)：对‘test_a’未定义的引用
collect2: 错误：ld 返回 1

```

这是链接错误。

也就是找不到函数`test_a`的定义。

这个问题引发了我这样的思考：对于编译器而言，声明和定义分别意味着什么？编译器对函数声明，函数定义的处理和对变量声明，变量定义的处理是相同的吗？

显然是不相同的。

### 函数声明对于编译器意味着什么

如果在`b.c`中将函数`test_a`的声明去除掉的话，再进行编译，我发现函数也能够编译通过，并且能够正确执行。



## Declaration



### wikipedia [Declaration (computer programming)](https://en.wikipedia.org/wiki/Declaration_(computer_programming))




In [computer programming](https://en.wikipedia.org/wiki/Computer_programming), a **declaration** is a [language construct](https://en.wikipedia.org/wiki/Language_construct) that specifies properties of an [identifier](https://en.wikipedia.org/wiki/Identifier_(computer_programming)): it declares what a word (identifier) "means". Declarations are most commonly used for [functions](https://en.wikipedia.org/wiki/Subroutine), [variables](https://en.wikipedia.org/wiki/Variable_(computer_science)), [constants](https://en.wikipedia.org/wiki/Constant_(computer_programming)), and [classes](https://en.wikipedia.org/wiki/Class_(computer_programming)), but can also be used for other entities such as enumerations and type definitions. Beyond the name (the identifier itself) and the kind of entity (function, variable, etc.), declarations typically specify the [data type](https://en.wikipedia.org/wiki/Data_type) (for variables and constants), or the [type signature](https://en.wikipedia.org/wiki/Type_signature) (for functions); types may also include dimensions, such as for arrays. A declaration is used to announce the existence of the entity to the [compiler](https://en.wikipedia.org/wiki/Compiler); this is important in those [strongly typed](https://en.wikipedia.org/wiki/Strongly_typed) languages that require functions, variables, and constants, and their types to be specified with a declaration before use, and is used in [forward declaration](https://en.wikipedia.org/wiki/Forward_declaration). The term "declaration" is frequently contrasted with the term "definition", but meaning and usage varies significantly between languages; see below.

Declarations are particularly prominent in languages in the [ALGOL](https://en.wikipedia.org/wiki/ALGOL) tradition, including the [BCPL](https://en.wikipedia.org/wiki/BCPL) family, most prominently [C](https://en.wikipedia.org/wiki/C_(programming_language)) and [C++](https://en.wikipedia.org/wiki/C%2B%2B), and also [Pascal](https://en.wikipedia.org/wiki/Pascal_(programming_language)). [Java](https://en.wikipedia.org/wiki/Java_(programming_language)) uses the term "declaration", though Java does not have separate declarations and definitions.

### wikipedia [Forward declaration](https://en.wikipedia.org/wiki/Forward_declaration)

In [computer programming](https://en.wikipedia.org/wiki/Computer_programming), a **forward declaration** is a [declaration](https://en.wikipedia.org/wiki/Declaration_(computer_science)) of an [identifier](https://en.wikipedia.org/wiki/Identifier_(computer_programming)) (denoting an entity such as a type, a variable, a constant, or a function) for which the programmer has not yet given a complete [definition](https://en.wikipedia.org/wiki/Definition).

It is required for a [compiler](https://en.wikipedia.org/wiki/Compiler) to know certain properties of an identifier (size for [memory allocation](https://en.wikipedia.org/wiki/Memory_allocation), [data type](https://en.wikipedia.org/wiki/Data_type) for type checking, such as [type signature](https://en.wikipedia.org/wiki/Type_signature) of functions), but not other details, like the particular value it holds (in case of variables or constants) or definition (in the case of functions). This is particularly useful for [one-pass compilers](https://en.wikipedia.org/wiki/One-pass_compiler) and [separate compilation](https://en.wikipedia.org/w/index.php?title=Separate_compilation&action=edit&redlink=1).

Forward declaration is used in languages that require declaration before use; it is necessary for [mutual recursion](https://en.wikipedia.org/wiki/Mutual_recursion) in such languages, as it is impossible to define such functions (or data structures) without a forward reference in one definition: one of the functions (respectively, data structures) must be defined first. It is also useful to allow flexible code organization, for example if one wishes to place the main body at the top, and called functions below it.

In other languages forward declarations are not necessary, which generally requires instead a [multi-pass compiler](https://en.wikipedia.org/wiki/Multi-pass_compiler) and for some compilation to be deferred to [link time](https://en.wikipedia.org/wiki/Link_time). In these cases identifiers must be defined (variables initialized, functions defined) before they are used in execution, but do not need to be defined before they are used in source code for compilation or interpretation: identifiers do not need to be immediately resolved to an existing entity.

> NOTE: 显然forward declaration是compiler需要的，而linker是不需要的




# 5 SWIG Basics



## 5.1 Running SWIG



### 5.1.1 Input format

As input, SWIG expects a file containing ISO C/C++ declarations and special SWIG directives.

> NOTE: 
>
> 一、SWIG的interface file的主要内容:
>
> 1、ISO C/C++ declarations 
>
> 2、special SWIG directives

Everything in the `%{ ... %}` block is simply copied verbatim(逐字的、原样的) to the resulting wrapper file created by SWIG. This section is almost always used to include header files and other declarations that are required to make the generated wrapper code compile. 

> NOTE: 
>
> 上面这段话介绍了"`%{ ... %}` block "的作用

It is important to emphasize that just because you include a declaration in a SWIG input file, that declaration does not automatically appear in the generated wrapper code---therefore you need to make sure you include the proper header files in the `%{ ... %}` section. It should be noted that the text enclosed in `%{ ... %}` is not parsed or interpreted by SWIG. The `%{...%}` syntax and semantics in SWIG is analogous to that of the declarations section used in input files to parser generation tools such as yacc or bison.

> NOTE: 
>
> 可以将header file中的include放到 `%{ ... %}` block 中

### **5.1.2 SWIG Output** 

For many target languages SWIG will also generate proxy class files in the target language. The default output directory for these language specific files is the same directory as the generated C/C++ file. This can be modified using the **-outdir** option. For example:

```shell
$ swig -c++ -python -outdir pyfiles -o cppfiles/example_wrap.cpp example.i
```

If the directories **cppfiles** and **pyfiles** exist, the following will be generated: 

```shell
cppfiles/example_wrap.cpp 
pyfiles/example.py
```



### 5.1.4 C Preprocessor

Like C, SWIG preprocesses all input files through an enhanced version of the C preprocessor. All standard preprocessor features are supported including file inclusion, conditional compilation and macros. However, `#include` statements are ignored unless the `-includeall` command line option has been supplied. The reason for disabling includes is that SWIG is sometimes used to process raw C header files. In this case, you usually only want the extension module to include functions in the supplied header file rather than everything that might be included by that header file (i.e., system headers, C library functions, etc.). It should also be noted that the SWIG preprocessor skips all text enclosed inside a `%{...%}` block. In addition, the preprocessor includes a number of macro handling enhancements that make it more powerful than the normal C preprocessor. These extensions are described in the "Preprocessor" chapter.

> NOTE: 
>
> 它是如何处理STL的C++类型的呢？因为这些类型都是在对应的header file中定义的。关于此，可以参见 "11 SWIG library" 章节

### 5.1.5 SWIG Directives

These directives are used to give SWIG hints or to alter SWIG's parsing behavior in some manner.

```c++
/* header.h --- Some header file */ 
/* SWIG directives -- only seen if SWIG is running */ 
#ifdef SWIG 
%module foo 
#endif
```



### 5.1.6 Parser Limitations

> NOTE: 
>
> swig不支持的特性

## 5.2 Wrapping Simple C Declarations

## 5.2.1 Basic Type Handling

> NOTE: 这一段所讲的其实是type map



The `char *` datatype is handled as a **NULL-terminated ASCII string**. SWIG maps this into a 8-bit character string in the target scripting language. SWIG converts character strings in the target language to **NULL terminated strings** before passing them into C/C++. The default handling of these strings does not allow them to have embedded NULL bytes. Therefore, the **`char *`**  datatype is not generally suitable for passing binary data. However, it is possible to change this behavior by defining a SWIG typemap. See the chapter on Typemaps for details about this.

> NOTE: 这段话描述了SWIG默认对" `char *` datatype"的处理。

### 5.2.2 Global Variables

### 5.2.3 Constants

> NOTE: 

### 5.2.4 A brief word about `const`

Starting with SWIG-1.3, all variable declarations, regardless of any use of `const`, are wrapped as **global variables**. If a declaration happens to be declared as `const`, it is wrapped as a read-only variable. 

### 5.2.5 A cautionary tale of `char *`

> NOTE: 这段话的意思是: 关于 `char *` 的警示故事

Before going any further, there is one bit of caution involving `char *` that must now be mentioned. When strings are passed from a scripting language to a C `char *`, the pointer usually points to string data stored inside the interpreter. It is almost always a really bad idea to modify this data. Furthermore, some languages may explicitly disallow it. For instance, in Python, strings are supposed to be immutable. If you violate this, you will probably receive a vast amount of wrath(愤怒) when you unleash(发布、释放) your module on the world.



## 5.3 Pointers and complex objects

Most C programs manipulate arrays, structures, and other types of objects. This section discusses the handling of these **datatypes**. 

### 5.3.1 Simple pointers

> NOTE: 
>
> 这一段描述了SWIG的实现细节，下面highlight了一些内容:
>
> 一、"All pointers are treated as opaque objects by SWIG"

Pointers to primitive C datatypes such as 

```c++
int * 
double *** 
char **
```

are fully supported by SWIG. Rather than trying to convert the data being pointed to into a scripting representation, SWIG simply encodes the pointer itself into a representation that contains the actual value of the pointer and a **type-tag**. Thus, the SWIG representation of the above pointers (in Tcl), might look like this: 

```c++
_10081012_p_int 
_1008e124_ppp_double 
_f8ac_pp_char
```

> NOTE: 从上述内容可知，SWIG是将pointee的值和它的类型信息组成了一个**字符串**来表示pointer的

A NULL pointer is represented by the string "NULL" or the value 0 encoded with type information. 

All pointers are treated as **opaque objects** by SWIG. Thus, a pointer may be returned by a function and passed around to other C functions as needed. For all practical purposes, the scripting language interface works in exactly the same way as you would use the pointer in a C program. The only difference is that there is no mechanism for dereferencing the pointer since this would require the target language to understand the memory layout of the underlying object. 

> NOTE: 其实在script language中，压根就不知道pointer，所以在其中"dereferencing the pointer"也是没有意义的

### 5.3.2 Run time pointer type checking

> NOTE: 可以实现dynamic type safety

By allowing pointers to be manipulated from a scripting language, extension modules effectively bypass compile-time type checking in the C/C++ compiler. To prevent errors, a type signature is encoded into all pointer values and is used to perform run-time type checking. This type-checking process is an integral part of SWIG and can not be disabled or modified without using typemaps (described in later chapters). 



### 5.3.3 Derived types, structs, and classes

For everything else (structs, classes, arrays, etc...) SWIG applies a very simple rule :

**Everything else is a pointer**

> NOTE: 
>
> 一、中文含义是: **其他一切都是指针**，它的含义是swig会将所有它不认识的类型作为pointer。在 "5.3.4 Undefined datatypes" 中，有更加准确的描述:
>
> **all unspecified types are internally handled as pointers to structures or classes!** 
>
> 

In other words, SWIG manipulates everything else by reference. This model makes sense because most C/C++ programs make heavy use of pointers and SWIG can use the type-checked pointer mechanism already present for handling pointers to basic datatypes.

> NOTE: 
>
> 一、它是源于opaque pointer

Although this probably sounds complicated, it's really quite simple. Suppose you have an interface file like this :

```C
%module fileio
FILE *fopen(char *, char *);
int fclose(FILE *);
unsigned fread(void *ptr, unsigned size, unsigned nobj, FILE *);
unsigned fwrite(void *ptr, unsigned size, unsigned nobj, FILE *);
void *malloc(int nbytes);
void free(void *);
```

In this file, SWIG doesn't know what a `FILE` is, but since it's used as a pointer, so it doesn't really matter what it is. If you wrapped this module into Python, you can use the functions just like you expect :

```Python
# Copy a file
def filecopy(source, target):
    f1 = fopen(source, "r")
    f2 = fopen(target, "w")
    buffer = malloc(8192)
    nbytes = fread(buffer, 8192, 1, f1)
    while (nbytes > 0):
    fwrite(buffer, 8192, 1, f2)
    nbytes = fread(buffer, 8192, 1, f1)
    free(buffer)
```

> NOTE: 
>
> 显然，swig充分利用了opaque pointer的特性

In this case `f1`, `f2`, and `buffer` are all opaque objects containing C pointers. It doesn't matter what value they contain--our program works just fine without this knowledge.

> NOTE: 
>
> 一、opaque pointer对应目标语言的opaque object

The scripting language representation of a pointer value should never be manipulated directly. 

### 5.3.4 Undefined datatypes

> NOTE: 
>
> 这一节描述的是SWIG的默认行为，这一节其实是符合前面所提的"Everything else is a pointer"

An important detail to mention is that SWIG will gladly generate wrappers for an interface when there are unspecified type names. However, **all unspecified types are internally handled as pointers to structures or classes!** 



### 5.3.5 Typedef

Like C, `typedef` can be used to define new type names in SWIG. For example:

```c++
typedef unsigned int size_t;
```

**typedef** definitions appearing in a **SWIG interface** are not propagated to the generated wrapper code. Therefore, they either need to be defined in an included header file or placed in the declarations section like this: 

```c++
%{
/* Include in the generated wrapper file */ 
typedef unsigned int size_t; 
%}
/* Tell SWIG about it */ 
typedef unsigned int size_t;
```

or 

```c++
%inline %{ 
  typedef unsigned int size_t; 
%}
```



> NOTE: 
>
> 一、这说明SWIG提供typedef的目的是用来告诉它准确的类型是什么
>
> 二、"they either need to be defined in an included header file" 这段话要如何理解？对于include的文件，SWIG不是不解析吗？

In certain cases, you might be able to include other header files to collect type information. For example: 

```c++
%module example 
%import "sys/types.h"
```

In this case, you might run SWIG as follows: 

```shell
$ swig -I/usr/include -includeall example.i
```



It should be noted that your mileage will vary greatly here. System headers are notoriously complicated and may rely upon a variety of non-standard C coding extensions (e.g., such as special directives to GCC). Unless you exactly specify the right include directories and preprocessor symbols, this may not work correctly (you will have to experiment). 

> NOTE: 翻译如下:
>
> "需要注意的是，您的里程在这里会有很大差异。众所周知，系统头文件非常复杂，并且可能依赖于各种非标准 C 编码扩展（例如，GCC 的特殊指令）。除非您准确指定正确的包含目录和预处理器符号，否则这可能无法正常工作（您必须进行试验）。"

## 5.4 Other Practicalities

> NOTE: 
>
> 在script language中，可能并没有"by value" semantic，那如何来进行映射呢？原文给出了解答。
>
> 这一段内容是非常好的，它对相关问题的总结是非常值得借鉴的，在进行wrapper编写的时候，是常常会碰到类似的问题的。

### 5.4.1 Passing structures by value

```C++
double dot_product(Vector a, Vector b);
```



```c++
double wrap_dot_product(Vector *a, Vector *b) {
    Vector x = *a;
    Vector y = *b;
    return dot_product(x, y);
}
```



### 5.4.2 Return by value

```C
Vector cross_product(Vector v1, Vector v2);
```



```C
Vector *wrap_cross_product(Vector *v1, Vector *v2) {
    Vector x = *v1;
    Vector y = *v2;
    Vector *result;
    result = (Vector *) malloc(sizeof(Vector));
    *(result) = cross(x, y);
    return result;
}
```



In both cases, SWIG allocates a new object and returns a reference to it. It is up to the user to delete the returned object when it is no longer in use. Clearly, this will leak memory if you are unaware of the implicit memory allocation and don't take steps to free the result. That said, it should be noted that some language modules can now automatically track newly created objects and reclaim memory for you. Consult the documentation for each language module for more details.

It should also be noted that the handling of pass/return by value in C++ has some special cases. For example, the above code fragments don't work correctly if Vector doesn't define a default constructor. The section on SWIG and C++ has more information about this case.



### 5.4.3 Linking to structure variables

> NOTE: 
>
> 这里透露了swig的实现细节

### 5.4.4 Linking to `char *`

When a global variable of type `char *` appears, SWIG uses `malloc()` or `new` to allocate memory for the new value.

> NOTE: 
>
> deep copy to avoid dangling reference



### 5.4.5 Arrays

Arrays are fully supported by SWIG, but they are always handled as pointers instead of mapping them to a special array object or list in the target language.

> NOTE: 
>
> 依然是遵循 **Everything else is a pointer**

Array variables are supported, but are read-only by default. For example:

```C
int a[100][200];
```

In this case, reading the variable '`a`' returns a pointer of type `int (*)[200]` that points to the first element of the array `&a[0][0]`. Trying to modify '`a`' results in an error. This is because SWIG does not know how to copy data from the target language into the array. To work around this limitation, you may want to write a few simple assist functions like this:



### 5.4.7 Renaming and ignoring declarations 



### 5.4.9 Pointers to functions and callbacks

Although SWIG does not normally allow **callback functions** to be written in the target language, this can be accomplished with the use of typemaps and other advanced SWIG features. See the Typemaps chapter for more about typemaps and individual target language chapters for more on callbacks. The 'director' feature can be used to make callbacks from C/C++ into the target language, see Callbacks to the target language.



## 5.5 Structures and unions


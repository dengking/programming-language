# [SWIG-4.0 Documentation](http://www.swig.org/Doc4.0/SWIGDocumentation.html#CONTENTS)



## 术语

1、director

2、C-style-accessor-function



## 5 SWIG Basics

### 5.1 Running SWIG

#### 5.1.1 Input format

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

#### 5.1.4 C Preprocessor

Like C, SWIG preprocesses all input files through an enhanced version of the C preprocessor. All standard preprocessor features are supported including file inclusion, conditional compilation and macros. However, `#include` statements are ignored unless the `-includeall` command line option has been supplied. The reason for disabling includes is that SWIG is sometimes used to process raw C header files. In this case, you usually only want the extension module to include functions in the supplied header file rather than everything that might be included by that header file (i.e., system headers, C library functions, etc.). It should also be noted that the SWIG preprocessor skips all text enclosed inside a `%{...%}` block. In addition, the preprocessor includes a number of macro handling enhancements that make it more powerful than the normal C preprocessor. These extensions are described in the "Preprocessor" chapter.

> NOTE: 
>
> 它是如何处理STL的C++类型的呢？因为这些类型都是在对应的header file中定义的。关于此，可以参见 "11 SWIG library" 章节

#### 5.1.5 SWIG Directives

These directives are used to give SWIG hints or to alter SWIG's parsing behavior in some manner.

#### 5.1.6 Parser Limitations

> NOTE: 
>
> swig不支持的特性

### 5.2 Wrapping Simple C Declarations

#### 5.2.1 Basic Type Handling

#### 5.2.2 Global Variables

#### 5.2.3 Constants

> NOTE: 

#### 5.2.4 A brief word about `const`

Starting with SWIG-1.3, all variable declarations, regardless of any use of `const`, are wrapped as **global variables**. If a declaration happens to be declared as `const`, it is wrapped as a read-only variable. 

#### 5.2.5 A cautionary tale of `char *`

Before going any further, there is one bit of caution involving `char *` that must now be mentioned. When strings are passed from a scripting language to a C `char *`, the pointer usually points to string data stored inside the interpreter. It is almost always a really bad idea to modify this data. Furthermore, some languages may explicitly disallow it. For instance, in Python, strings are supposed to be immutable. If you violate this, you will probably receive a vast amount of wrath(愤怒) when you unleash(发布、释放) your module on the world.

### 5.3 Pointers and complex objects

#### 5.3.1 Simple pointers

> NOTE: 
>
> 这一段描述了SWIG的实现细节

#### 5.3.3 Derived types, structs, and classes

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

#### 5.3.4 Undefined datatypes

> NOTE: 
>
> 这一节其实是符合前面所提的"Everything else is a pointer"

#### 5.3.5 Typedef

> NOTE: 
>
> 通过typedef来告诉SWIG，准确的类型是什么

Like C, `typedef` can be used to define new type names in SWIG. For example:



### 5.4 Other Practicalities

> NOTE: 
>
> 在script language中，可能并没有"by value" semantic，那如何来进行映射呢？原文给出了解答。
>
> 这一段内容是非常好的，它对相关问题的总结是非常值得借鉴的，在进行wrapper编写的时候，是常常会碰到类似的问题的。

#### 5.4.1 Passing structures by value

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



#### 5.4.2 Return by value

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



#### 5.4.3 Linking to structure variables

> NOTE: 
>
> 这里透露了swig的实现细节

#### 5.4.4 Linking to `char *`

When a global variable of type `char *` appears, SWIG uses `malloc()` or `new` to allocate memory for the new value.

> NOTE: 
>
> deep copy to avoid dangling reference



#### 5.4.5 Arrays

Arrays are fully supported by SWIG, but they are always handled as pointers instead of mapping them to a special array object or list in the target language.

> NOTE: 
>
> 依然是遵循 **Everything else is a pointer**

Array variables are supported, but are read-only by default. For example:

```C
int a[100][200];
```

In this case, reading the variable '`a`' returns a pointer of type `int (*)[200]` that points to the first element of the array `&a[0][0]`. Trying to modify '`a`' results in an error. This is because SWIG does not know how to copy data from the target language into the array. To work around this limitation, you may want to write a few simple assist functions like this:

#### 5.4.9 Pointers to functions and callbacks

Although SWIG does not normally allow callback functions to be written in the target language, this can be accomplished with the use of typemaps and other advanced SWIG features. See the Typemaps chapter for more about typemaps and individual target language chapters for more on callbacks. The 'director' feature can be used to make callbacks from C/C++ into the target language, see Callbacks to the target language.

## 6 SWIG and C++

### 6.1 Comments on C++ Wrapping

> NOTE: 
>
> 这段论述还是不错的

### 6.2 Approach

To wrap C++, SWIG uses a layered approach to code generation. At the lowest level, SWIG generates a collection of procedural ISO C style wrappers. These wrappers take care of basic type conversion, type checking, error handling, and other low-level details of the C++ binding. These wrappers are also sufficient to bind C++ into any target language that supports built-in procedures. In some sense, you might view this layer of wrapping as providing a C library interface to C++. On top of the low-level procedural (flattened) interface, SWIG generates proxy classes that provide a natural object-oriented (OO) interface to the underlying code. The proxy classes are typically written in the target language itself. For instance, in Python, a real Python class is used to provide a wrapper around the underlying C++ object.

> NOTE: 
>
> 这一节主要描述SWIG的原理，在 Klaus Kämpf 的 [SWIG Generating language bindings for C/C++ libraries](https://en.opensuse.org/images/e/eb/Kkaempf_KnowledgeSharing_Swig.pdf) 中有着更好的图示。
>
> 一、layer
>
> 1、ISO C style wrappers
>
> 2、**proxy classes** that provide a natural object-oriented (OO) interface to the underlying code

### 6.3 Supported C++ features

As a rule of thumb, SWIG should not be used on raw C++ source files, use header files only.

### 6.5 Proxy classes

#### 6.5.1 Construction of proxy classes

Proxy classes are always constructed as an extra layer of wrapping that uses low-level accessor functions. To illustrate, suppose you had a C++ class like this:

```C++
class Foo {
public:
    Foo();
    ~Foo();
    int bar(int x);

    int x;
};
```

Using C++ as pseudocode, a proxy class looks something like this:

```C++
class FooProxy {
private:
	Foo *self;
public:
    FooProxy() {
	    self = new_Foo();
    }
    ~FooProxy() {
    	delete_Foo(self);
    }
    int bar(int x) {
    	return Foo_bar(self, x);
    }
    int x_get() {
    	return Foo_x_get(self);
    }
    void x_set(int x) {
    	Foo_x_set(self, x);
    }
};
```

Of course, always keep in mind that the real proxy class is written in the target language. For example, in Python, the proxy might look roughly like this:

```Python
class Foo:
    def __init__(self):
        self.this = new_Foo()

    def __del__(self):
        delete_Foo(self.this)

    def bar(self, x):
        return Foo_bar(self.this, x)

    def __getattr__(self, name):
        if name == 'x':
            return Foo_x_get(self.this)

    # ...
    def __setattr__(self, name, value):
        if name == 'x':
            Foo_x_set(self.this, value)

```

#### 6.5.2 Resource management in proxies

A major issue with proxies concerns the memory management of wrapped objects. Consider the following C++ code:

```C++
class Foo {
public:
	Foo();
    ~Foo();
    int bar(int x);
    int x;
};
class Spam {
public:
    Foo *value;
    ...
};
```

Consider some script code that uses these classes:

```Python
f = Foo() # Creates a new Foo
s = Spam() # Creates a new Spam
s.value = f # Stores a reference to f inside s
g = s.value # Returns stored reference
g = 4 # Reassign g to some other value
del f # Destroy f
```

### 6.6 Simple C++ wrapping



#### 6.6.3 When constructor wrappers aren't created



### 6.25 C++ reference counted objects - ref/unref feature

### 6.29 Callbacks to the target language

> NOTE: 
>
> "Callbacks to the target language" 指的是在source language的程序中，执行target language程序中的函数

C/C++ function pointers are often used for callbacks and this is discussed in the Pointers to functions and callbacks section. The callback techniques described therein provide a way to control callbacks to a C/C++ function but not callbacks into the target language. The techniques described below show how the director feature can be used to support callbacks from C/C++ to the target language.

> NOTE: 
>
> 一、翻译如下:
>
> "C/C++函数指针通常用于回调，这将在函数指针和回调一节中讨论。这里描述的回调技术提供了一种方法来控制对C/C++函数的回调，而不是对目标语言的回调。下面描述的技术展示了如何使用director特性来支持从C/C++到目标语言的回调。"

#### 6.29.1 Introduction to director classes

The **director feature** enables the ability for a target language class to derive from a **wrapped C++ class**. The target language can override virtual methods of a wrapped C++ class, thereby supporting **cross-language polymorphism**. Code can 'call up' from C++ into the target language by simply calling a virtual method overridden in a derived class in the target language. The **wrapped C++ classes** that have this ability are termed 'director' classes. The director feature is documented individually in each target language and the reader should locate and read this to obtain a full understanding of directors.

> NOTE: 
>
> 目标语言中的类来实现源语言C++中的virtual class，实现**cross-language polymorphism**。

#### 6.29.2 Using directors and target language callbacks

SWIG's primary goal is to make it possible to call C/C++ code from a target language, however, the director feature enables the reverse. While there isn't simple direct support for calling target language code from C, the director feature makes this possible. It does require some work and additional wrapper code to be provided by the user. The additional code required must be C++ and not C code and hence may introduce a small dependency on C++ if using a pure C project. In a nutshell, the user must create a C++ base class and turn it into a **director class**. A **virtual method** in the director base class is required. SWIG generates the code to call up into the target language when wrapping the **director virtual method**.

> NOTE: 
>
> 所谓 "director class"，其实就是接口类

## 11 SWIG library

### 11.1 The %include directive and library search path

Library files are included using the `%include` directive.

```shell
swig -swiglib
```



## 14 Customization Features



## 19 Working with Modules

### 19.1 Modules Introduction

Each invocation of SWIG requires a **module name** to be specified. The **module name** is used to name the resulting target language extension module.

> NOTE: 
>
> 目标语言的module

## 32 SWIG and Python
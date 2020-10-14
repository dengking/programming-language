# Language linkage

## Mix C and C++

本节标题的含义是: Use C in C++、Use C++ in C

C++的设计目标之一是要兼容C，C++继承了非常C的大部分内容，所以大多数情况下C++是可以兼容C的，比如: 

1) C、C++都是基于header file、source file的

2) ......

但是随着语言的发展，两门语言出现了一些分歧，这在`C-and-C++\VS-C-VS-C++`中进行了讨论。

本文讨论的主题是: 如何在这两门语言相互使用，这大概就是本文的标题Language linkage，跨语言进行link的含义。

### Use C in C++

本节讨论在C++中使用C的情况，前面我们已经介绍了: 大多数情况下C++是可以兼容C的，但是对于一些特殊情况，则需要进行特殊处理，这是本节重点关注的内容。

### Use C function in C++

#### `extern "C"` ?

C++的name mangling(参见`C-and-C++\From-source-code-to-exec\ABI\Name-mangling`)导致在C++中无法使用C function无法兼容，这就是 `extern "C"` 所要解决的问题: 

对于一个C的header file，通过在**header file**中的function declaration中添加`extern "C"`的声明，告诉 compiler对于指定的function，不要使用name mangling。这样，当一个C++ compiler编译一个包含这个C header file的C++ source file时候，就不会执行name mangling，这样compiler就能够正确地链接到对应C函数的implementation。这就实现了在C++中使用C的目标。

显然`extern "C"`就是告诉C++ compiler不要进行name mangling，声明如下:

```C++
#ifdef __cplusplus  
extern "C" { 
#endif 
    /* Declarations of this file */
#ifdef __cplusplus 
} 
#endif 
```

上述声明，能够保证use C function in C++，也能够实现use C++ function in C，这在下一节也会进行介绍。

参见: 

cnblogs [extern “C”的作用详解](https://www.cnblogs.com/xiangtingshen/p/10980055.html) 这篇文章介绍地还可以。

cnblogs [extern "C"的用法解析](https://www.cnblogs.com/rollenholt/archive/2012/03/20/2409046.html) 这篇更好。

geeksforgeeks [Name Mangling and extern “C” in C++](https://www.geeksforgeeks.org/extern-c-in-c/) 结合了具体的例子，比较好。





### Use C++ in C

反过来，在C中，可以使用C++吗？这个问题应该是：有些是可以的（比如function），但是有些无法实现的（比如OOP）。由于C++是C的超集，因此当混合使用两者的时候，需要使用g++来进行编译；

### Use C++ function in C

对于C++中想要暴露给C使用的，需要wrap为C function，并且不能够使用C++的一些特性，比如function overload，并且在function declaration中需要使用`extern "C"`，告诉compiler不要进行name mangling。如果不这样做，则无法实现:

当使用C++ compiler编译带有`extern "C"`的function的时候，compiler不会进行name mangling；

当使用C compiler进行编译包含了上述function的header file的C source file的时候，C compiler是不会进行name mangling的，这样进行链接的时候，就能够找到function的implementation。



### Practice: sqlite extension

我需要写一个sqlite extension，它需要被编译为一个shared library，由于这个shared library需要被dynamic load，因此它没有header file，无法对name mangling进行控制，所以它必须用C来实现。所有的C++，都需要进行wrap。



### TODO: embeddedartistry [Mixing C and C++: extern C](https://embeddedartistry.com/blog/2017/05/01/mixing-c-and-c-extern-c/)



### Examples

本节介绍一些使用了`extern "C"`的例子:

[`sqlite3.h`](https://github.com/LuaDist/libsqlite3/blob/master/sqlite3.h) 

wikipedia [Name mangling](https://en.wikipedia.org/wiki/Name_mangling)

> NOTE: 其中也列举了很多案例

## cppreference [Language linkage](https://en.cppreference.com/w/cpp/language/language_linkage)



## TODO: Namespace and extern

[extern “C” linkage inside C++ namespace?](https://stackoverflow.com/questions/28996944/extern-c-linkage-inside-c-namespace)

['extern "C"' should not be used with namespaces](https://rules.sonarsource.com/cpp/RSPEC-3732)
# 关于本章

本章探讨C++ ABI，重点介绍给广泛采用的: [Itanium C++ ABI](https://itanium-cxx-abi.github.io/cxx-abi/abi.html)。

## oracle [Stability of the C++ ABI: Evolution of a Programming Language](https://www.oracle.com/technical-resources/articles/it-infrastructure/stable-cplusplus-abi.html)

> NOTE:
>
> 这篇文章对C++ ABI涉及的内容进行了非常好的论述
>
> 

### The C ABI

The Oracle Solaris ABI is also the C ABI, because C is the standard UNIX implementation language. Among other things, the C ABI specifies:

1、Size and layout of predefined types (char, int, float, and so on)

2、Layout of compound types (arrays and structs)

3、External (linker-visible) spelling of programmer-defined names

4、Machine-code function-calling sequence

5、Stack layout

6、Register usage

### The C++ ABI

The C++ ABI includes the C ABI. In addition, it covers the following features:

1、Layout of hierarchical class objects, that is, base classes and virtual base classes

2、Layout of pointer-to-member

3、Passing of hidden function parameters (for example, `this`)

4、How to call a virtual function:

- Vtable contents and layout
- Location in objects of pointers to vtables
- Finding the adjustment for the `this` pointer

4、Finding base-class offsets

5、Calling a function via pointer-to-member

6、Managing template instances

7、External spelling of names ("name mangling")

8、Construction and destruction of static objects

9、Throwing and catching exceptions

10、Some details of the standard library:

- Implementation-defined details
- typeinfo and run-time type information
- Inline function access to members

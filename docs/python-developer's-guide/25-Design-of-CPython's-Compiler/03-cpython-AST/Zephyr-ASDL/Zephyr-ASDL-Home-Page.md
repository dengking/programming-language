# [Zephyr ASDL Home Page](http://asdl.sourceforge.net/)

# Introduction

The [Zephyr](http://www.cs.virginia.edu/zephyr) Abstract Syntax Description Language (ASDL) is a **language** designed to describe the tree-like data structures in compilers. Its main goal is to provide a method for compiler components written in different languages to interoperate.  ASDL makes it easier for applications written in a variety of programming languages to communicate complex **recursive** data structures.

`asdlGen` is a tool that takes ASDL descriptions and produces implementations of those descriptions in a variety of popular languages. ASDL and `asdlGen` together provide the following advantages

- Concise descriptions of important data structures.
- Automatic generation of data structure implementations for C, `C++`, Java, Standard ML, and Haskell.
- Automatic generation of functions to read and write the data structures to disk in a machine and language independent way.

ASDL descriptions describe the tree-like data structures such as abstract syntax trees (ASTs) and compiler intermediate representations (IRs). Tools such as `asdlGen` automatically produce the equivalent data structure definitions for C, `C++`, Java, Standard ML, OCaml, and Haskell. `asdlGen` also produces functions for each language that read and write the data structures to and from a platform and language independent sequence of bytes. The sequence of bytes is called a *pickle*.

ASDL pickles can be interactively viewed and edited with a graphical [browser](http://www.cs.princeton.edu/zephyr/browser), or pretty printed into a simple textual format. The browser provides some advanced features such as display styles and tree based versions of standard unix tools such as `diff` and `grep`. ASDL was part of the Zephyr [National Compiler Infrastructure](http://www.cs.virginia.edu/nci) project.
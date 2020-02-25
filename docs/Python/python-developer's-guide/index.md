This record my exploration the implementation of [cpython](https://github.com/python/cpython), the most widely used python implementation. Following this, you will know how  a python program is executed.



关于cpython的实现的介绍非常多，本节将从软件工程的角度来分析cpython的实现。它作为一个非常成功的大型软件，其实现方式是值的软件工程师学习的，除此之外，其管理方式，质量保证等方面也是非常值得学习的。

cpython的实现可以作为一个成功的软件工程典范来进行学习：

- 如何做软件工程的
- 如何保证质量的
- 如何实现的
- 如何来进行协作开发
- 如何跨平台

这一系列都是值的我们学习的

主要以Unix-like system为例来进行说明，Windows平台的忽略。



# 这个系列我决定从 如下方面进行组织：

## [Part 1: Introduction to CPython](https://realpython.com/cpython-source-code-guide/#part-1-introduction-to-cpython)

介绍python和cpython

## Source Code

描述cpython的源代码组织，官方介绍：

- 23.1. CPython Source Code Layout[¶](https://devguide.python.org/exploring/#cpython-source-code-layout)
- 1.8. Directory structure[¶](https://devguide.python.org/setup/#directory-structure)

## cpython build system

描述cpython的build system，这部分是很多文章中都没有的，需要好好的进行介绍





## Compiling CPython linux





## The Python Language Specification

参见https://realpython.com/cpython-source-code-guide/#part-1-introduction-to-cpython

需要对其中涉及到的知识进行说明。

官方资源：

24. Changing CPython’s Grammar[¶](https://devguide.python.org/grammar/#changing-cpython-s-grammar)



## cpython实现分析

### 架构

compiler + [interpreter](https://en.wikipedia.org/wiki/Interpreter_(computing))/[virtual machine](https://en.wikipedia.org/wiki/Virtual_machine)

两者之间的接口是Python bytecode：

- [`dis`](https://docs.python.org/3/library/dis.html#module-dis) — Disassembler for Python bytecode[¶](https://docs.python.org/3/library/dis.html#module-dis)
- [Bytecode](https://en.wikipedia.org/wiki/Bytecode)

### 入口

沿着python程序的执行来逐步进行分析

参见[Part 2: The Python Interpreter Process](https://realpython.com/cpython-source-code-guide/#part-2-the-python-interpreter-process)

#### Memory Management in CPython





### bytecode

[`dis`](https://docs.python.org/3/library/dis.html#module-dis) — Disassembler for Python bytecode[¶](https://docs.python.org/3/library/dis.html#module-dis)









# TODO 

## python AST

https://greentreesnakes.readthedocs.io/en/latest/



## cpython的指令是怎样的？

从AST-->CFG-->byte code

cpython提供了一些列的build-in type，build-in function供programmer使用，byte code是否是基于这些build-in的？即byte code最终被解释执行的时候其实调用的还是只写build-in的方法？我觉得应该是这样的。





参考内容：


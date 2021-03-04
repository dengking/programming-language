# C++ iteration statement

Iteration 是最最常见的computation，本章讨论C++ iteration statement，另外在 `Computation-on-structure\Iteration` 章节中，也对iteration进行了讨论。

## 发展概述

C++11: range-based for

1、简化语法: 去除显式的iteration操作

2、simplify C++

隐藏iterator操作，其实是一种sugar，使code更加expressive、abstract。基于range的generic programming，正重塑C++ STL。

C++ range与Python非常类似了，能够让C++像Python那样进行编程。
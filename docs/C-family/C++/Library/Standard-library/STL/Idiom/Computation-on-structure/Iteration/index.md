# 关于本章

本章讨论C++ iteration。

## 发展历程

朝着range的方向在发展:

C++11: range-based for

C++20: range library

关于range，参见`./Range`。

为什么会朝着range的方向发展呢？下面进行了介绍:

## Range over iterator

隐藏iterator、隐藏iterator操作，其实是一种sugar，使code更加expressive、abstract。基于range的generic programming，正重塑C++ STL。

C++ range与Python非常类似了，能够让C++像Python那样进行编程。
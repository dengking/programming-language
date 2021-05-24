# 关于本章

本章描述function template。

## Overload and specialization

function template其实是两种static polymorphism的交汇点:

a、template specialization，C++ function template只支持full specialization，不支持partial specialization

b、overload

那compiler是如何来进行编译的呢？答案在 `cppreference-Function-template # Function overloads vs function specializations` 段，其中对这个问题进行了深入探讨。



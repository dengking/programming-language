# Strict aliasing

在C++、C中，都描述了strict aliasing内容。

## cppreference `Object#Strict aliasing`

Accessing an object using an expression of a type other than the type with which it was created is **undefined behavior** in many cases, see [reinterpret_cast](reinterpret_cast.html#Type_aliasing) for the list of exceptions and examples.



## creference `Objects and alignment#Strict aliasing`

Given an object with *effective type* `T1`, using an lvalue expression (typically, dereferencing a pointer) of a different type `T2` is **undefined behavior**



## Why?

alignment requirement。


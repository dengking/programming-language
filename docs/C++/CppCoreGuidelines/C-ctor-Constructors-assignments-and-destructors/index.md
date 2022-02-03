# CppCoreGuidelines [C.ctor: Constructors, assignments, and destructors](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#cctor-constructors-assignments-and-destructors)

These functions control the lifecycle of objects: creation, copy, move, and destruction. Define constructors to guarantee and simplify initialization of classes.

> NOTE: 
>
> 一、这一段总结是非常好的，将object lifetime 和这些special member function统一起来了



## Set of default operations rules:



### [C.20: If you can avoid defining default operations, do](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-zero)

> NOTE: 
>
> 一、这其实就是rule-of-zero

## Constructor rules:

### [C.50: Use a factory function if you need "virtual behavior" during initialization](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c50-use-a-factory-function-if-you-need-virtual-behavior-during-initialization)

> NOTE:
>
> 1、其实就是`Calling-Virtual-During-Initialization-idiom`

### [C.52: Use inheriting constructors to import constructors into a derived class that does not need further explicit initialization](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c52-use-inheriting-constructors-to-import-constructors-into-a-derived-class-that-does-not-need-further-explicit-initialization)

> NOTE: 
>
> 还未搞懂



## Copy and move rules:

> NOTE: 
>
> 后续整理
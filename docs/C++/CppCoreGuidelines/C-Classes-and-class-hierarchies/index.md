# CppCoreGuidelines [C: Classes and class hierarchies](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-class)



## Class rule summary:

### [C.2: Use `class` if the class has an invariant; use `struct` if the data members can vary independently](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c2-use-class-if-the-class-has-an-invariant-use-struct-if-the-data-members-can-vary-independently)



## C.concrete: Concrete types

> NOTE: 
>
> 1、Concrete types 一般 满足 rule of six，也就是含有"C.ctor: Constructors, assignments, and destructors"中总结的六种operation。

One ideal for a class is to be a regular type. That means roughly "behaves like an `int`." A concrete type is the simplest kind of class. A value of regular type can be copied and the result of a copy is an independent object with the same value as the original. 

If a concrete type has both `=` and `==`, `a = b` should result in `a == b` being `true`. 

> NOTE: `a = b` 表示将 `b` 赋值给 `a`，`a == b` 表示 `a`  和 `b` 的 value相等。

Concrete classes without assignment and equality can be defined, but they are (and should be) rare. The C++ built-in types are regular, and so are standard-library classes, such as `string`, `vector`, and `map`. 

> NOTE: 

Concrete types are also often referred to as value types to distinguish them from types used as part of a hierarchy.

> NOTE: 
>
> 1、"types used as part of a hierarchy" 是指什么？应该是polymorphic class



## C.ctor: Constructors, assignments, and destructors

These functions control the lifecycle of objects: creation, copy, move, and destruction. 

> NOTE: 
>
> 1、总结的非常好



These are *default operations*:

11、a default constructor: `X()`

2、a copy constructor: `X(const X&)`

3、a copy assignment: `operator=(const X&)`

4、a move constructor: `X(X&&)`

5、a move assignment: `operator=(X&&)`

6、a destructor: `~X()`



### C.defop: Default Operations





# `assign` and `operator=`

1、是在思考virtual `operator=`时，在 CppCoreGuidelines [C.60: Make copy assignment non-`virtual`, take the parameter by `const&`, and return by non-`const&`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c60-make-copy-assignment-non-virtual-take-the-parameter-by-const-and-return-by-non-const)

中，有这样的描述:

> **Alternatives**: If you think you need a `virtual` assignment operator, and understand why that's deeply problematic, don't call it `operator=`. Make it a named function like `virtual void assign(const Foo&)`. See [copy constructor vs. `clone()`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-copy-virtual)

这引发了我对`assign` and `operator=`的思考





## stackoverflow [std::string::assign vs std::string::operator=](https://stackoverflow.com/questions/34196053/stdstringassign-vs-stdstringoperator)



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


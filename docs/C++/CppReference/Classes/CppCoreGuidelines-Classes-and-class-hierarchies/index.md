# CppCoreGuidelines [C: Classes and class hierarchies](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c-classes-and-class-hierarchies)

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
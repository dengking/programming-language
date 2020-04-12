# VS C VS C++

## 思路

c++在c上所增强的，主要方面：

### OOP



### Type system

### safety

new和malloc

string literal：c中的类型`char *`，c++中是`const char *`。

### Generic programming

主要是template

### Library

c++的standard library提供的功能过多。



## name space and scope

`c++`中可以显式定义name space，`c`中的name space是隐式的；

### name space

c[namespace](https://en.cppreference.com/w/c/language/name_space)

`c++`[namespace](https://en.cppreference.com/w/cpp/language/lookup)

### scope

在c中，`struct`并不是一个scope；在`c++`中，`struct`是一个scope；

以下是取自[Lookup and name spaces in C](https://en.cppreference.com/w/c/language/name_space):

Unlike in `C++`, **enumeration constants** are not `struct` members, and their **name space** is the name space of ordinary identifiers, and since there is no `struct` scope in C, their scope is the scope in which the struct declaration appears:
```C
struct tagged_union 
{
   enum {INT, FLOAT, STRING} type;
   int integer;
   float floating_point;
   char *string;
} tu;
 
tu.type = INT; // OK in C, error in C++
```
所以在c中enum就非常奇怪；

## words
[c](https://en.cppreference.com/w/c/language/basic_concepts)和[c++](https://en.cppreference.com/w/cpp/language/basic_concepts)的source code中的word的分类相同：
- keyword
- identifier

## entity

### [C](https://en.cppreference.com/w/c/language/identifier)

C中并没有给出entity的定义。

### [C++](https://en.cppreference.com/w/cpp/language/basic_concepts)

The *entities* of a C++ program are values, [objects](https://en.cppreference.com/w/cpp/language/objects), [references](https://en.cppreference.com/w/cpp/language/reference), [structured bindings](https://en.cppreference.com/w/cpp/language/structured_binding) (since `C++17`), [functions](https://en.cppreference.com/w/cpp/language/functions), [enumerators](https://en.cppreference.com/w/cpp/language/enum), [types](https://en.cppreference.com/w/cpp/language/type), class members, [templates](https://en.cppreference.com/w/cpp/language/templates), [template specializations](https://en.cppreference.com/w/cpp/language/template_specialization), [namespaces](https://en.cppreference.com/w/cpp/language/namespace), and [parameter packs](https://en.cppreference.com/w/cpp/language/parameter_pack). Preprocessor [macros](https://en.cppreference.com/w/cpp/preprocessor/replace) are not C++ entities.

Entities are introduced by [declarations](https://en.cppreference.com/w/cpp/language/declarations), which associate them with [names](https://en.cppreference.com/w/cpp/language/name) and define their properties. The declarations that define all properties required to use an entity are [definitions](https://en.cppreference.com/w/cpp/language/definition). A program must contain only one definition of any non-inline function or variable that is [odr-used](https://en.cppreference.com/w/cpp/language/definition).



### SUMMARY

显然，两种语言对entity的定义不同，c中包含了[macro](https://en.cppreference.com/w/c/preprocessor/replace),，而c++中则将其剔除了；entity是编译时概念，而不是preprocessor概念；



## `T::F`

[flexible array member](https://gustedt.wordpress.com/2011/03/14/flexible-array-member/)中有这样的一段描述：

> which is probably not something that you want to write on a daily base. A particularity in that expression is `P99_SIZEOF(T, F[0])` which stands for the size of the element `F[0]` inside the `struct` type `T`. C doesn’t have the possibility as C++ to refer to a field in a type with something like `T::F`.
>
> Something similar can be obtained in C99 with the magic formula `sizeof((T){ 0 }.F[0]) `: define a compound literal `(T){ 0 }` of type `T` and take its field `F`. The `sizeof` operator actually ensures that this compound literal is never allocated, only the field `F` is taken for its type and size. This magic works in function scope (the compound literal would be of storage class `auto`) and in file scope (it would be `static`).

## Assignment operators

In c:

The [value category](https://en.cppreference.com/w/c/language/value_category) of the assignment operator is non-lvalue (so that expressions such as `(a=b)=c` are invalid).


In `c++`:

In C++, assignment operators are lvalue expressions, not so in C


## Increment/decrement operators

https://en.cppreference.com/w/c/language/operator_incdec

Unlike `C++` (and some implementations of C), the increment/decrement expressions are never themselves lvalues: `&++a` is invalid.
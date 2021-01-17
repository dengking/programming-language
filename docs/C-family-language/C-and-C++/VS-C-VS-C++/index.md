# VS C VS C++



## What C++ enhance?

思路: C++在C上所增强的，主要方面：

### OOP

C++支持OOP，因此很多概念就需要向OOP扩展，在`C++\Language-reference\Basic-concept\index.md`中对这个问题进行了深入分析。



### Type system

C++在C的基础上对type system做了很多改进，使得相比于C而言，C++有着更好的type safety，主要是通过: 充分发挥static type实现的；

> NOTE: 可以作为static type VS dynamic type的素材。

在wikipedia [Type safety](https://en.wikipedia.org/wiki/Type_safety#C++) 中对此进行了非常好的总结:

> Some features of C++ that promote more type-safe code:
>
> - The [new](https://infogalactic.com/info/New_(C%2B%2B)) operator returns a pointer of type based on operand, whereas [malloc](https://infogalactic.com/info/Malloc) returns a void pointer.
> - C++ code can use virtual functions and [templates](https://infogalactic.com/info/Template_(programming)) to achieve polymorphism without void pointers.
> - [Preprocessor](https://infogalactic.com/info/C_macro) constants (without type) can be rewritten as [const](https://infogalactic.com/info/Constant_(programming)) variables (typed).
> - [Preprocessor](https://infogalactic.com/info/C_macro) macro functions (without type) can be rewritten as [inline](https://infogalactic.com/info/Inline_function) functions (typed). The flexibility of accepting and returning different types can still be obtained by function overloading.
> - Safer casting operators, such as [dynamic cast](https://infogalactic.com/info/Dynamic_cast) that performs run-time type checking.

#### Effective type

c中有[effective type](https://en.cppreference.com/w/c/language/object)的概念，而`c++`中则没有；

`C++`中并没有effective type的概念；其实主要原因在于c++中使用了`new`，而c中则是`malloc`，显然这是`c++`在type safety上的一些改善，这一点在wikipedia [Type safety](https://en.wikipedia.org/wiki/Type_safety#C++)中有解释；

#### `new`和`malloc`

这在 `Resource-management\Memory-management\Allocation-and-deallocation\VS-malloc-VS-new` 章节进行了介绍。

#### CV

C++在CV方面，比C要严格：

1、string literal：c中的类型`char *`，c++中是`const char *`

2、C-style cast会drop掉CV，C++的各种type cast函数如`reinterpret_cast`、`static_cast`等，都需要保持CV，C++中需要通过`const_cast`来drop掉CV，相比于C的implicit，C++的是explicit，这一点，在`C++\Language-reference\Basic-concept\Type-system\Type-conversion\reinterpret_cast.md#CV`段中进行了详细介绍

#### Type conversion

关于这一点，在`C++\Language-reference\Basic-concept\Type-system\Type-conversion\Cast-operator\index.md`中进行了详细比较。

#### Type punning

关于这一点，在`C++\Language-reference\Basic-concept\Type-system\Type-punning`中进行了详细比较。

##### `std::variant` VS union

C++17引入的`std::variant` 是对union的改进，关于此，参见`C++\Library\Standard-library\Utility-library\Common-vocabulary-types`。

#### `std::any` VS `void*`

C++17引入的`std::any` 是对`void*`的改进，关于此，参见`C++\Library\Standard-library\Utility-library\Common-vocabulary-types`。



#### Variadic template VS ellipsis 

参见 `C++\Language-reference\Template\Parameter-pack\Variadic template-VS-ellipsis` 章节。

### Generic programming 

主要是template-meta programming。

### Library

c++的standard library提供的功能过多。



## name space and scope

`c++`中可以显式定义name space，`c`中的name space是隐式的；

### name space

c [namespace](https://en.cppreference.com/w/c/language/name_space)

`c++` [namespace](https://en.cppreference.com/w/cpp/language/lookup)

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



## linkage规则的差异

参见[Storage class specifiers#Notes](https://en.cppreference.com/w/cpp/language/storage_duration#Notes)。
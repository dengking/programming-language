# Type system

C++在C的基础上对type system做了很多改进，使得相比于C而言，C++有着更好的type safety，主要是通过: 

1、充分发挥static type实现的

> NOTE: 可以作为static type VS dynamic type的素材。

2、执行更加严格的类型检查

在wikipedia [Type safety](https://en.wikipedia.org/wiki/Type_safety#C++) 中对此进行了非常好的总结:

## wikipedia [Type safety](https://en.wikipedia.org/wiki/Type_safety#C++) 

Some features of C++ that promote more type-safe code:

1、The [new](https://infogalactic.com/info/New_(C%2B%2B)) operator returns a pointer of type based on operand, whereas [malloc](https://infogalactic.com/info/Malloc) returns a void pointer.

2、C++ code can use virtual functions and [templates](https://infogalactic.com/info/Template_(programming)) to achieve polymorphism without void pointers.

> NOTE: 
>
> 1、void pointer 是典型的type erasure，关于void pointer 和 template的对比，参见 `C++\Programming-paradigm\Type-erasure-OOP-GP`，其中的`ixodoi.expertscolumn-C++ Programming-void-pointer-VS-template` 对此进行了非常深入的分析

3、[Preprocessor](https://infogalactic.com/info/C_macro) constants (without type) can be rewritten as [const](https://infogalactic.com/info/Constant_(programming)) variables (typed).



4、[Preprocessor](https://infogalactic.com/info/C_macro) macro functions (without type) can be rewritten as [inline](https://infogalactic.com/info/Inline_function) functions (typed). The flexibility of accepting and returning different types can still be obtained by function overloading.

5、Safer casting operators, such as [dynamic cast](https://infogalactic.com/info/Dynamic_cast) that performs run-time type checking.

## Effective type

c中有[effective type](https://en.cppreference.com/w/c/language/object)的概念，而`c++`中则没有；

`C++`中并没有effective type的概念；其实主要原因在于c++中使用了`new`，而c中则是`malloc`，显然这是`c++`在type safety上的一些改善，这一点在wikipedia [Type safety](https://en.wikipedia.org/wiki/Type_safety#C++)中有解释；

## `new`和`malloc`

这在 `Resource-management\Memory-management\Allocation-and-deallocation\VS-malloc-VS-new` 章节进行了介绍。

## CV

C++在CV方面，比C要严格：

1、string literal：c中的类型`char *`，c++中是`const char *`

2、C-style cast会drop掉CV，C++的各种type cast函数如`reinterpret_cast`、`static_cast`等，都需要保持CV，C++中需要通过`const_cast`来drop掉CV，相比于C的implicit，C++的是explicit，这一点，在`C++\Language-reference\Basic-concept\Type-system\Type-conversion\reinterpret_cast.md#CV`段中进行了详细介绍

## Type conversion

关于这一点，在`C++\Language-reference\Basic-concept\Type-system\Type-conversion\Cast-operator\index.md`中进行了详细比较。

## Type punning

关于这一点，在`C++\Language-reference\Basic-concept\Type-system\Type-punning`中进行了详细比较。

### `std::variant` VS union

C++17引入的`std::variant` 是对union的改进，关于此，参见`C++\Library\Standard-library\Utility-library\Common-vocabulary-types`。

### `std::any` VS `void*`

C++17引入的`std::any` 是对`void*`的改进，关于此，参见`C++\Library\Standard-library\Utility-library\Common-vocabulary-types`。



## Variadic template VS ellipsis 

参见 `C++\Language-reference\Functions\Generic-programming\Variadic-function` 章节。



## Void pointer conversion

参见 :

1、`thegreenplace-void-pointer-and-casts-in-C-and-C++`
[TOC]



# [What does “static” mean in C?](https://stackoverflow.com/questions/572547/what-does-static-mean-in-c)

I've seen the word `static` used in different places in C code; is this like a static function/class in C# (where the implementation is shared across objects)?









# [Static (keyword)](https://en.wikipedia.org/wiki/Static_%28keyword%29)

In the [C programming language](https://en.wikipedia.org/wiki/C_(programming_language)) (and its close descendants such as [C++](https://en.wikipedia.org/wiki/C%2B%2B) and [Objective-C](https://en.wikipedia.org/wiki/Objective-C)), **static** is a [reserved word](https://en.wikipedia.org/wiki/Reserved_word) controlling both lifetime (as a [static variable](https://en.wikipedia.org/wiki/Static_variable)) and visibility (depending on *linkage*). The word `static` is also used in languages influenced by C, such as [Java](https://en.wikipedia.org/wiki/Java_(programming_language)).

In C, `static` is a [storage class](https://en.wikipedia.org/wiki/Storage_class) (not to be confused with [classes in object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming#Class)), as are `extern`, `auto` and `register` (which are also reserved words). Every variable and function has one of these **storage classes**; if a declaration does not specify the storage class, a context-dependent default is used:

- `extern` for all top-level declarations in a source file,
- `auto` for variables declared in function bodies.

| Storage class      | Lifetime                                                     | Visibility                                                   |
| :----------------- | :----------------------------------------------------------- | :----------------------------------------------------------- |
| `extern`           | program execution                                            | external (whole program)                                     |
| `static`           | program execution                                            | internal ([translation unit](https://en.wikipedia.org/wiki/Translation_unit_(programming)) only) |
| `auto`, `register` | [function execution](https://en.wikipedia.org/wiki/Automatic_variable) | (none)                                                       |

In these languages, the term "static variable" has two meanings which are easy to confuse:

1. A variable with the same lifetime as the program, as described above (language-independent); *or*
2. (C-family-specific) A variable declared with storage class `static`.

Variables with storage class `extern`, which include variables declared at top level without an explicit storage class, are `static` in the first meaning but not the second.


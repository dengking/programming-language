# [Forward declaration](https://en.wikipedia.org/wiki/Forward_declaration)

In [computer programming](https://en.wikipedia.org/wiki/Computer_programming), a **forward declaration** is a [declaration](https://en.wikipedia.org/wiki/Declaration_(computer_science)) of an [identifier](https://en.wikipedia.org/wiki/Identifier_(computer_programming)) (denoting an entity such as a type, a variable, a constant, or a function) for which the programmer has not yet given a complete [definition](https://en.wikipedia.org/wiki/Definition).

It is required for a [compiler](https://en.wikipedia.org/wiki/Compiler) to know certain properties of an identifier (size for [memory allocation](https://en.wikipedia.org/wiki/Memory_allocation), [data type](https://en.wikipedia.org/wiki/Data_type) for type checking, such as [type signature](https://en.wikipedia.org/wiki/Type_signature) of functions), but not other details, like the particular value it holds (in case of variables or constants) or definition (in the case of functions). This is particularly useful for [one-pass compilers](https://en.wikipedia.org/wiki/One-pass_compiler) and [separate compilation](https://en.wikipedia.org/w/index.php?title=Separate_compilation&action=edit&redlink=1).

Forward declaration is used in languages that require declaration before use; it is necessary for [mutual recursion](https://en.wikipedia.org/wiki/Mutual_recursion) in such languages, as it is impossible to define such functions (or data structures) without a forward reference in one definition: one of the functions (respectively, data structures) must be defined first. It is also useful to allow flexible code organization, for example if one wishes to place the main body at the top, and called functions below it.

In other languages forward declarations are not necessary, which generally requires instead a [multi-pass compiler](https://en.wikipedia.org/wiki/Multi-pass_compiler) and for some compilation to be deferred to [link time](https://en.wikipedia.org/wiki/Link_time). In these cases identifiers must be defined (variables initialized, functions defined) before they are used in execution, but do not need to be defined before they are used in source code for compilation or interpretation: identifiers do not need to be immediately resolved to an existing entity.

***SUMMARY*** : 显然forward declaration是compiler需要的，而linker是不需要的


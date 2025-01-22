# [cppreference Metaprogramming library (since C++11)](https://en.cppreference.com/w/cpp/meta)

C++ provides metaprogramming facilities, such as :

- type traits
- compile-time rational arithmetic
- compile-time integer sequences



## Definitions

The following types are collectively called *referenceable types*:

- [object types](https://en.cppreference.com/w/cpp/language/type#Object_type)
- [function types](https://en.cppreference.com/w/cpp/language/function#Function_type) without *cv* and *ref*
- [reference types](https://en.cppreference.com/w/cpp/language/reference)

For any referenceable type `T`, a reference to it can be created[[1\]](https://en.cppreference.com/w/cpp/meta#cite_note-1).

1. [↑](https://en.cppreference.com/w/cpp/meta#cite_ref-1) For reference types, this can be done via [reference collapsing](https://en.cppreference.com/w/cpp/language/reference#Reference_collapsing).



## Type traits

参见`C++11-Type-traits`章节



## [Compile-time rational arithmetic](https://en.cppreference.com/w/cpp/numeric/ratio) 

The header [`<ratio`>](https://en.cppreference.com/w/cpp/header/ratio) provides [types and functions for manipulating and storing compile-time ratios](https://en.cppreference.com/w/cpp/numeric/ratio).

## Compile-time integer sequences

Defined in header [`<utility>`](https://en.cppreference.com/w/cpp/header/utility)

| function                                                     |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [integer_sequence](https://en.cppreference.com/w/cpp/utility/integer_sequence)(C++14) | implements compile-time sequence of integers (class template) |
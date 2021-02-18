# Opaque pointer

1、使用C/C++ programming language的术语来说明，"Opaque pointer"是"incomplete type"。

## wikipedia [Opaque pointer](https://en.wikipedia.org/wiki/Opaque_pointer)

In [computer programming](https://en.wikipedia.org/wiki/Computer_programming), an **opaque pointer** is a special case of an [opaque data type](https://en.wikipedia.org/wiki/Opaque_data_type), a [data type](https://en.wikipedia.org/wiki/Data_type) declared to be a [pointer](https://en.wikipedia.org/wiki/Pointer_(computer_science)) to a [record](https://en.wikipedia.org/wiki/Record_(computer_science)) or [data structure](https://en.wikipedia.org/wiki/Data_structure) of some unspecified type.

Opaque pointers are present in several [programming languages](https://en.wikipedia.org/wiki/Programming_language) including [Ada](https://en.wikipedia.org/wiki/Ada_(programming_language)), [C](https://en.wikipedia.org/wiki/C_(programming_language)), [C++](https://en.wikipedia.org/wiki/C%2B%2B), [D](https://en.wikipedia.org/wiki/D_(programming_language)) and [Modula-2](https://en.wikipedia.org/wiki/Modula-2).

> NOTE: 
>
> 1、使用C/C++ programming language的术语来说明，"Opaque pointer"是"incomplete type"。

If the language is [strongly typed](https://en.wikipedia.org/wiki/Strong_typing), [programs](https://en.wikipedia.org/wiki/Computer_program) and [procedures](https://en.wikipedia.org/wiki/Subroutine) that have no other information about an opaque pointer type *T* can still declare [variables](https://en.wikipedia.org/wiki/Variable_(programming)), [arrays](https://en.wikipedia.org/wiki/Array_data_structure), and record fields of type *T*, assign values of that type, and compare those values for equality. However, they will not be able to [de-reference](https://en.wikipedia.org/wiki/Reference_(computer_science)) such a pointer, and can only change the object's content by calling some procedure that has the missing information.

> NOTE: 
>
> 1、结合C/C++的内容来看，上面这段话是容易理解的

Opaque pointers are a way to hide the [implementation](https://en.wikipedia.org/wiki/Implementation_(computing)) details of an [interface](https://en.wikipedia.org/wiki/Interface_(computer_science)) from ordinary clients, so that the [implementation](https://en.wikipedia.org/wiki/Implementation_(computing)) may be changed without the need to recompile the [modules](https://en.wikipedia.org/wiki/Module_(programming)) using it. This benefits the programmer as well since a simple interface can be created, and most details can be hidden in another file.[[1\]](https://en.wikipedia.org/wiki/Opaque_pointer#cite_note-1) This is important for providing [binary code compatibility](https://en.wikipedia.org/wiki/Binary_code_compatibility) through different versions of a [shared library](https://en.wikipedia.org/wiki/Shared_library), for example.

This technique is described in *[Design Patterns](https://en.wikipedia.org/wiki/Design_Patterns)* as the [Bridge pattern](https://en.wikipedia.org/wiki/Bridge_pattern). It is sometimes referred to as "**[handle](https://en.wikipedia.org/wiki/Handle_(computing)) classes**",[[2\]](https://en.wikipedia.org/wiki/Opaque_pointer#cite_note-eckel20000-2) the "**Pimpl idiom**" (for "pointer to implementation idiom"),[[3\]](https://en.wikipedia.org/wiki/Opaque_pointer#cite_note-3) "**Compiler firewall idiom**",[[4\]](https://en.wikipedia.org/wiki/Opaque_pointer#cite_note-4) "**d-pointer"** or "**Cheshire Cat**", especially among the C++ community.[[2\]](https://en.wikipedia.org/wiki/Opaque_pointer#cite_note-eckel20000-2)
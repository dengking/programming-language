# Dynamic memory allocation



## wikipedia [C dynamic memory allocation](https://en.wikipedia.org/wiki/C_dynamic_memory_allocation)

**C dynamic memory allocation** refers to performing [manual memory management](https://en.wikipedia.org/wiki/Manual_memory_management) for [dynamic memory allocation](https://en.wikipedia.org/wiki/Dynamic_memory_allocation) in the [C programming language](https://en.wikipedia.org/wiki/C_(programming_language)) via a group of functions in the [C standard library](https://en.wikipedia.org/wiki/C_standard_library), namely malloc, realloc, calloc and free.[[1\]](https://en.wikipedia.org/wiki/C_dynamic_memory_allocation#cite_note-c99-1)[[2\]](https://en.wikipedia.org/wiki/C_dynamic_memory_allocation#cite_note-2)[[3\]](https://en.wikipedia.org/wiki/C_dynamic_memory_allocation#cite_note-3)

The [C++](https://en.wikipedia.org/wiki/C%2B%2B) programming language includes these functions; however, the operators [new and delete](https://en.wikipedia.org/wiki/New_and_delete_(C%2B%2B)) provide similar functionality and are recommended by that language's authors.[[4\]](https://en.wikipedia.org/wiki/C_dynamic_memory_allocation#cite_note-4) Still, there are several situations in which using `new/delete` is not applicable, such as garbage collection code or performance-sensitive code, and a combination of `malloc` and `placement new` may be required instead of the higher-level `new` operator.

> NOTE: 上面这一段描述，显然比较了`new/delete` 和 `malloc`

Many different implementations of the actual memory allocation mechanism, used by malloc, are available. Their performance varies in both execution time and required memory.



### Implementations
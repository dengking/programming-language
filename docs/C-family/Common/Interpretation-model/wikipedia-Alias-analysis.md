# Alias analysis

## wikipedia [Alias analysis](https://en.wikipedia.org/wiki/Alias_analysis)

**Alias analysis** is a technique in [compiler theory](https://en.wikipedia.org/wiki/Compiler_theory), used to determine if a storage location may be accessed in more than one way. Two pointers are said to be [aliased](https://en.wikipedia.org/wiki/Aliasing_(computing)) if they point to the same location.

Alias analysis techniques are usually classified by flow-sensitivity and context-sensitivity. They may determine may-alias or must-alias information. The term **alias analysis** is often used interchangeably with [points-to analysis](https://en.wikipedia.org/wiki/Pointer_analysis), a specific case.

Alias analysers intend to make and compute useful information for understanding [aliasing](https://en.wikipedia.org/wiki/Aliasing_(computing)) in programs.

## Overview

In general, alias analysis determines whether or not separate memory references point to the same area of memory. This allows the compiler to determine what variables in the program will be affected by a statement. For example, consider the following section of code that accesses members of structures:

```c
p.foo = 1;
q.foo = 2;
i = p.foo + 3;
```

There are three possible alias cases here:

1. The variables `p` and `q` cannot alias (i.e., they never point to the same memory location).
2. The variables `p` and `q` must alias (i.e., they always point to the same memory location).
3. It cannot be conclusively determined at compile time if `p` and `q` alias or not.

If `p` and `q` cannot alias, then `i = p.foo + 3;` can be changed to `i = 4`. If `p` and `q` must alias, then `i = p.foo + 3;` can be changed to `i = 5` because `p.foo + 3` = `q.foo + 3`. In both cases, we are able to perform optimizations from the alias knowledge (assuming that no other [thread](https://en.wikipedia.org/wiki/Thread_(computing)) updating the same locations can interleave with the current thread, or that the language [memory model](https://en.wikipedia.org/wiki/Memory_model_(programming)) permits those updates [to be not immediately visible](https://en.wikipedia.org/wiki/Out-of-order_execution) to the current thread in absence of explicit [synchronization constructs](https://en.wikipedia.org/wiki/Memory_barrier)). On the other hand, if it is not known if `p` and `q` alias or not, then no optimizations can be performed and the whole of the code must be executed to get the result. Two memory references are said to have a *may-alias* relation if their aliasing is unknown.

## Performing alias analysis

In **alias analysis**, we divide the program's memory into *alias classes*. Alias classes are disjoint sets of locations that cannot alias to one another. For the discussion here, it is assumed that the optimizations done here occur on a low-level [intermediate representation](https://en.wikipedia.org/wiki/Intermediate_language) of the program. This is to say that the program has been compiled into binary operations, jumps, moves between registers, moves from registers to memory, moves from memory to registers, branches, and function calls/returns.
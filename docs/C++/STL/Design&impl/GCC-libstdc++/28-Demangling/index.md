# gcc.gnu [Chapter 28. Demangling](https://gcc.gnu.org/onlinedocs/libstdc++/manual/ext_demangling.html)

>  在 stackoverflow [Is it possible to print a variable's type in standard C++?](https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c) # [A](https://stackoverflow.com/a/20170989) 中 使用了`__cxa_demangle` 

Transforming C++ ABI identifiers (like RTTI symbols) into the original C++ source identifiers is called “demangling.”





## [abi Namespace Reference](https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.3/a01696.html)

**Returns:**

A pointer to the start of the NUL-terminated demangled name, or NULL if the demangling fails. The caller is responsible for deallocating this memory using `free`.

> NOTE: 
>
> 1、resource return
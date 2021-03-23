# `uintptr_t` 

## stackoverflow [What is uintptr_t data type](https://stackoverflow.com/questions/1845482/what-is-uintptr-t-data-type)



### [A](https://stackoverflow.com/a/1845491)

`uintptr_t` is an unsigned integer type that is capable of storing a data pointer. Which typically means that it's the same size as a pointer.

[It is optionally defined in C++11 and later standards.](https://en.cppreference.com/w/cpp/types/integer)

A common reason to want an integer type that can hold an architecture's pointer type is to perform integer-specific operations on a pointer, or to obscure the type of a pointer by providing it as an integer "handle".



### [A](https://stackoverflow.com/a/1846648)

First thing, at the time the question was asked, `uintptr_t` was not in C++. It's in C99, in `<stdint.h>`, as an optional type. Many C++03 compilers do provide that file. It's also in C++11, in `<cstdint>`, where again it is optional, and which refers to C99 for the definition.

In C99, it is defined as "an unsigned integer type with the property that any valid pointer to void can be converted to this type, then converted back to pointer to void, and the result will compare equal to the original pointer".

Take this to mean what it says. It doesn't say anything about size.

`uintptr_t` might be the same size as a `void*`. It might be larger. It could conceivably be smaller, although such a C++ implementation approaches perverse. For example on some hypothetical platform where `void*` is 32 bits, but only 24 bits of virtual address space are used, you could have a 24-bit `uintptr_t` which satisfies the requirement. I don't know why an implementation would do that, but the standard permits it.

#### comments

To [allocate aligned memory](http://stackoverflow.com/q/227897/183120), among other uses, for instance? – [legends2k](https://stackoverflow.com/users/183120/legends2k) [Jul 21 '15 at 7:36](https://stackoverflow.com/questions/1845482/what-is-uintptr-t-data-type#comment51025968_1846648)

## cppreference [Fixed width integer types (since C++11)](https://en.cppreference.com/w/cpp/types/integer)
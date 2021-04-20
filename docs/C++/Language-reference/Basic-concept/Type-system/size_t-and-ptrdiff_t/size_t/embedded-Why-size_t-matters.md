
# [Why size_t matters](https://www.embedded.com/electronics-blogs/programming-pointers/4026076/Why-size-t-matters)

**Using size_t appropriately can improve the portability, efficiency, or readability of your code. Maybe even all three.**



Numerous functions in the Standard C library accept arguments or return values that represent **object sizes** in bytes. For example, the lone argument in `malloc(n)` specifies the size of the object to be allocated, and the last argument in `memcpy(s1, s2, n)`specifies the size of the object to be copied. The **return value** of `strlen(s)` yields the length of (the number of characters in) null-terminated character array `s` excluding the **null character**, which isn't exactly the size of `s`, but it's in the ballpark.



You might reasonably expect these parameters and return types that represent sizes to be declared with type **int** (possibly **long** and/or **unsigned**), but they aren't. Rather, the C standard declares them as type **size_t**. According to the standard, the declaration for **`malloc`** should appear in **`<stdlib.h>`** as something equivalent to:



```C
void *malloc(size_t n);
```



and the declarations for **`memcpy`** and **`strlen`** should appear in **`<string.h>`** looking much like:



```C
void *memcpy(void *s1, void const *s2, size_t n);
size_t strlen(char const *s);
```





The type **size_t** also appears throughout the `C++` standard library. In addition, the `C++` library uses a related symbol **size_type**, possibly even more than it uses **size_t**.



In my experience, most C and `C++` programmers are aware that the standard libraries use **size_t**, but they really don't know what **size_t** represents or why the libraries use **size_t** as they do. Moreover, they don't know if and when they should use **size_t** themselves.



In this column, I'll explain what **size_t** is, why it exists, and how you should use it in your code.



## A portability problem

Classic C (the early dialect of C described by Brian Kernighan and Dennis Ritchie in *The C Programming Language*, Prentice-Hall, 1978) didn't provide **size_t**. The C standards committee introduced **size_t** to eliminate a portability problem, illustrated by the following example.



Let's examine the problem of writing a portable declaration for the standard **`memcpy`** function. We'll look at a few different declarations and see how well they work when compiled for different architectures with **different-sized address spaces** and data paths.

***SUMMARY*** : `size_t`和address length密切相关

Recall that calling **`memcpy(s1, s2, n)`** copies the first n bytes from the object pointed to by **`s2`** to the object pointed to by **`s1`**, and returns **`s1`**. The function can copy objects of any type, so the pointer parameters and return type should be declared as "pointer to void." Moreover, `memcpy` doesn't modify the source object, so the second parameter should really be "pointer to `const void`." None of this poses a problem.



The real concern is how to declare the function's third parameter, which represents the size of the source object. I suspect many programmers would choose plain **int**, as in:

```c
void *memcpy(void *s1, void const *s2, int n);
```

which works fine most of the time, but it's not as general as it could be. Plain **int** is signed--it can represent negative values. However, sizes are never negative. Using **unsigned int** instead of **int** as the type of the third parameter lets **`memcpy`** copy larger objects, at no additional cost.



On most machines, the largest **unsigned int** value is roughly twice the largest positive **int** value. For example, on a 16-bit twos-complement machine, the largest **unsigned int** value is 65,535 and the largest positive **int** value is 32,767. Using an **unsigned int** as **`memcpy`**'s third parameter lets you copy objects roughly twice as big as when using **int**.



Although the size of an **int** varies among C implementations, on any given implementation **int** objects are always the same size as **unsigned int** objects. Thus, passing an **unsigned int** argument is always the same cost as passing an **int**.



Using **unsigned int** as the parameter type, as in:

```c
void *memcpy(void *s1, void const *s2, unsigned int n);
```

works just dandy on any platform in which an **`s`** unsigned int can represent the size of the largest data object. This is generally the case on any platform in which integers and pointers have the same size, such as `IP16`, in which both integers and pointers occupy 16 bits, or `IP32`, in which both occupy 32 bits. (See the sidebar on *C data model notation*.)



> **C data model notation** 
>
> Of late, I've run across several articles that employ a compact notation for describing the C language data representation on different target platforms. I have yet to find the origins of this notation, a formal syntax, or even a name for it, but it appears to be simple enough to be usable without a formal definition. The general form of the notation appears to be: $I n_I L n_L LL n_{LL} P n_P$
>
> where each capital letter (or pair thereof) represents a **C data type**, and each corresponding `n` is the number of bits that the type occupies. `I` stands for `int`, `L` stands for `long`, `LL` stands for `long long`, and `P` stands for `pointer` (to data, not pointer to function). Each letter and number is optional.  For example, an `I16P32` architecture supports 16-bit int and 32-bit pointers, without describing whether it supports long or long long. If two consecutive types have the same size, you typically omit the first number. For example, you typically write `I16L32P32` as `I16LP32`, which is an architecture that supports 16-bit int, 32-bit long, and 32-bit pointers.  The notation typically arranges the letters so their corresponding numbers appear in ascending order. For example, `IL32LL64P32` denotes an architecture with 32-bit int, 32-bit long, 64-bit long long, and 32-bit pointers; however, it appears more commonly as `ILP32LL64`.

Unfortunately, this declaration for **`memcpy`** comes up short on an `I16LP32` processor (16-bits for int and 32-bits for long and pointers), such as the first generation Motorola 68000. In this case, the processor can copy objects larger than 65,536 bytes, but this **`memcpy`** can't because parameter **`n`** can't handle values that large.








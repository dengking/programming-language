# kb.iu.edu [Identify what's causing segmentation faults (segfaults)](https://kb.iu.edu/d/aqsj)

> NOTE: 
>
> 1、通过这篇文章可知，和memory相关的错误，都输segment fault

## Overview

A segmentation fault (aka segfault) is a common condition that causes programs to crash; they are often associated with a file named `core`. Segfaults are caused by a program trying to read or write an illegal memory location.

Program memory is divided into different segments: a text segment for program instructions, a data segment for variables and arrays defined at compile time, a stack segment for temporary (or automatic) variables defined in subroutines and functions, and a heap segment for variables allocated during runtime by functions, such as `malloc` (in C) and `allocate` (in Fortran). For more, see [About program segments](https://kb.iu.edu/d/areb).

A segfault occurs when a reference to a variable falls outside the segment where that variable resides, or when a write is attempted to a location that is in a read-only segment. In practice, segfaults are almost always due to trying to read or write a non-existent array element, not properly defining a pointer before using it, or (in C programs) accidentally using a variable's value as an address ([see the `scanf` example below](https://kb.iu.edu/d/aqsj#scanf)).

[Back to top](https://kb.iu.edu/d/aqsj#top)

## Examples of common segfaults

### access null pointer

For example, calling  `memset()` as shown below would cause a program to segfault:

```C++
memset((char *)0x0, 1, 100);
```

### array out of bound

```c++
/* "Array out of bounds" error valid indices for array foo are 0, 1, ... 999 */ int foo[1000]; for (int i = 0; i <= 1000 ; i++) foo[i] = i;
```



```c++
/* Illegal memory access if value of n is not in the range 0, 1, ... 999 */ int n; int foo[1000]; for (int i = 0; i < n ; i++) foo[i] = i;
```



```c++
/* Illegal memory access because no memory is allocated for foo2 */ float *foo, *foo2; foo = (float*)malloc(1000); foo2[0] = 1.0;
```



### integer as address

Another common programming error that leads to segfaults is oversight in the use of pointers. For example, the C function `scanf()` expects the address of a variable as its second parameter; therefore, the following will likely cause the program to crash with a segfault:

```C++
int foo = 0; scanf("%d", foo); /* Note missing & sign ; correct usage would have been &foo */
```

### write a read-only location

A segfault will occur when a program attempts to operate on a memory location in a way that is not allowed (for example, attempts to write a read-only location would result in a segfault).



### stack overflow

Segfaults can also occur when your program runs out of stack space. This may not be a bug in your program, but may be due instead to your shell setting the stack size limit too small.
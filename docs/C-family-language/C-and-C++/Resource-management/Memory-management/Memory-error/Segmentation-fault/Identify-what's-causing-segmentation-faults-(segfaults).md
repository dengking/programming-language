# [Identify what's causing segmentation faults (segfaults)](https://kb.iu.edu/d/aqsj)

On this page:

- [Overview](https://kb.iu.edu/d/aqsj#over)
- [Examples of common segfaults](https://kb.iu.edu/d/aqsj#ex)
- [Find out-of-bounds array references](https://kb.iu.edu/d/aqsj#array)
- [Check shell limits](https://kb.iu.edu/d/aqsj#shell)
- [Use debuggers to diagnose segfaults](https://kb.iu.edu/d/aqsj#debug)

------

## Overview

A segmentation fault (aka segfault) is a common condition that causes programs to crash; they are often associated with a file named `core`. Segfaults are caused by a program trying to read or write an illegal memory location.

Program memory is divided into different segments: a text segment for program instructions, a data segment for variables and arrays defined at compile time, a stack segment for temporary (or automatic) variables defined in subroutines and functions, and a heap segment for variables allocated during runtime by functions, such as `malloc` (in C) and `allocate` (in Fortran). For more, see [About program segments](https://kb.iu.edu/d/areb).

A segfault occurs when a reference to a variable falls outside the segment where that variable resides, or when a write is attempted to a location that is in a read-only segment. In practice, segfaults are almost always due to trying to read or write a non-existent array element, not properly defining a pointer before using it, or (in C programs) accidentally using a variable's value as an address ([see the `scanf` example below](https://kb.iu.edu/d/aqsj#scanf)).

[Back to top](https://kb.iu.edu/d/aqsj#top)

## Examples of common segfaults

- For example, calling

   

  ```
  memset()
  ```

   

  as shown below would cause a program to segfault:

  ```
  memset((char *)0x0, 1, 100);
  ```

- The following three cases illustrate the most common types of array-related segfaults:

  |                            Case A                            |
  | :----------------------------------------------------------: |
  | `/* "Array out of bounds" error valid indices for array foo are 0, 1, ... 999 */ int foo[1000]; for (int i = 0; i <= 1000 ; i++) foo[i] = i;` |
  |                            Case B                            |
  | `/* Illegal memory access if value of n is not in the range 0, 1, ... 999 */ int n; int foo[1000]; for (int i = 0; i < n ; i++) foo[i] = i;` |
  |                            Case C                            |
  | `/* Illegal memory access because no memory is allocated for foo2 */ float *foo, *foo2; foo = (float*)malloc(1000); foo2[0] = 1.0;` |

  - In case A, array `foo` is defined for `index = 0, 1, 2, ... 999`. However, in the last iteration of the `for`loop, the program tries to access `foo[1000]`. This will result in a segfault if that memory location lies outside the memory segment where `foo` resides. Even if it doesn't cause a segfault, it is still a bug.
  - In case B, integer `n `could be any random value. As in case A, if it is not in the range `0, 1, ... 999`, it might cause a segfault. Whether it does or not, it is certainly a bug.
  - In case C, allocation of memory for variable `foo2` has been overlooked, so `foo2` will point to a random location in memory. Accessing `foo2[0]` will likely result in a segfault.

- Another common programming error that leads to segfaults is oversight in the use of pointers. For example, the C function

   

  ```
  scanf()
  ```

   

  expects the address of a variable as its second parameter; therefore, the following will likely cause the program to crash with a segfault:

  ```
  int foo = 0; scanf("%d", foo); /* Note missing & sign ; correct usage would have been &foo */
  ```

  The variable `foo` might be defined at memory location `1000`, but the above function call would try to read integer data into memory location `0` according to the definition of `foo`.

- A segfault will occur when a program attempts to operate on a memory location in a way that is not allowed (for example, attempts to write a read-only location would result in a segfault).

- Segfaults can also occur when your program runs out of stack space. This may not be a bug in your program, but may be due instead to your shell setting the stack size limit too small.

[Back to top](https://kb.iu.edu/d/aqsj#top)

## Find out-of-bounds array references

Most Fortran compilers have an option that will insert code to do bounds checking on all array references during runtime. If an access falls outside the index range defined for an array, the program will halt and tell you where this occurs. For most Fortran compilers, the option is `-C`, or `-check` followed by a keyword. See your compiler's user guide to get the exact option. Use bounds checking only when debugging, since it will slow down your program. Some C compilers also have a bounds checking option.

[Back to top](https://kb.iu.edu/d/aqsj#top)

## Check shell limits

As noted in the last example [above](https://kb.iu.edu/d/aqsj#limit), some segfault problems are not due to bugs in your program, but are caused instead by system memory limits being set too low. Usually it is the limit on stack size that causes this kind of problem. To check memory limits, use the `ulimit` command in `bash` or `ksh`, or the `limit` command in `csh` or `tcsh`. Try setting the stacksize higher, and then re-run your program to see if the segfault goes away.

[Back to top](https://kb.iu.edu/d/aqsj#top)

## Use debuggers to diagnose segfaults

If you can't find the problem any other way, you might try a debugger. For example, you could use GNU's well-known debugger `GDB` to view the backtrace of a `core` file dumped by your program; whenever programs segfault, they usually dump the content of (their section of the) memory at the time of the crash into a `core` file. Start your debugger with the command `gdb core`, and then use the `backtrace` command to see where the program was when it crashed. This simple trick will allow you to focus on that part of the code.

If using `backtrace` on the `core`g file doesn't find the problem, you might have to run the program under debugger control, and then step through the code one function, or one source code line, at a time. To do this, you will need to compile your code without optimization, and with the `-g` flag, so information about source code lines will be embedded in the executable file. For more, see [Step-by-step example for using GDB within Emacs to debug a C or C++ program](https://kb.iu.edu/d/aqsy).

[Back to top](https://kb.iu.edu/d/aqsj#top)

## Related documents

[Step-by-step example for using GDB within Emacs to debug a C or C++ program](https://kb.iu.edu/d/aqsy)
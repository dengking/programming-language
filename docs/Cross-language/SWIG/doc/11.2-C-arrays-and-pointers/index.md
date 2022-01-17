# 11.2 C arrays and pointers

> NOTE: 
>
> 基于了在Python中new、delete的能力，这是典型的从Python->C++

This section describes library modules for manipulating low-level C arrays and pointers. The primary use of these modules is in supporting C declarations that manipulate bare pointers such as `int *`, `double *`, or `void *`. The modules can be used to allocate memory, manufacture pointers, dereference memory, and wrap pointers as class-like objects. Since these functions provide direct access to memory, their use is potentially unsafe and you should exercise caution.




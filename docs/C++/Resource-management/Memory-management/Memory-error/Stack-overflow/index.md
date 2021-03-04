# wikipedia [Stack overflow](https://en.wikipedia.org/wiki/Stack_overflow)

In software, a **stack overflow** occurs if the [call stack](https://en.wikipedia.org/wiki/Call_stack) pointer exceeds the [stack](https://en.wikipedia.org/wiki/Stack_(abstract_data_type)) bound. The call stack may consist of a limited amount of [address space](https://en.wikipedia.org/wiki/Address_space), often determined at the start of the program. The size of the **call stack** depends on many factors, including the programming language, machine architecture, multi-threading, and amount of available memory. When a program attempts to use more space than is available on the call stack (that is, when it attempts to access memory beyond the call stack's bounds, which is essentially a [buffer overflow](https://en.wikipedia.org/wiki/Buffer_overflow)), the stack is said to *overflow*, typically resulting in a program crash.[[1\]](https://en.wikipedia.org/wiki/Stack_overflow#cite_note-fortran1-1)



## Infinite recursion

Main article: [Infinite recursion](https://en.wikipedia.org/wiki/Infinite_recursion)

The most common cause of stack overflow is excessively deep or infinite recursion, in which a function calls itself so many times that the space needed to store the variables and information associated with each call is more than can fit on the stack.[[2\]](https://en.wikipedia.org/wiki/Stack_overflow#cite_note-segFault-2)

An example of infinite recursion in [C](https://en.wikipedia.org/wiki/C_(programming_language)).

```c
int foo() 
{
     return foo();
}
```

The function *foo*, when it is invoked, continues to invoke itself, allocating additional space on the stack each time, until the stack overflows resulting in a [segmentation fault](https://en.wikipedia.org/wiki/Segmentation_fault).[[2\]](https://en.wikipedia.org/wiki/Stack_overflow#cite_note-segFault-2) However, some compilers implement [tail-call optimization](https://en.wikipedia.org/wiki/Tail-call_optimization), allowing infinite recursion of a specific sort—[tail recursion](https://en.wikipedia.org/wiki/Tail_recursion)—to occur without stack overflow. This works because **tail-recursion calls** do not take up additional stack space.[[3\]](https://en.wikipedia.org/wiki/Stack_overflow#cite_note-tailRecur-3)

Some C compiler options will effectively enable [tail-call optimization](https://en.wikipedia.org/wiki/Tail-call_optimization); for example, compiling the above simple program using [gcc](https://en.wikipedia.org/wiki/GNU_Compiler_Collection) with `-O1` will result in a segmentation fault, but not when using `-O2` or `-O3`, since these optimization levels imply the `-foptimize-sibling-calls` compiler option[[4\]](https://en.wikipedia.org/wiki/Stack_overflow#cite_note-4). Other languages, such as [Scheme](https://en.wikipedia.org/wiki/Scheme_(programming_language)), require all implementations to include tail-recursion as part of the language standard.[[5\]](https://en.wikipedia.org/wiki/Stack_overflow#cite_note-5)

## Very deep recursion

A **recursive function** that terminates in theory but causes a call stack buffer overflow in practice can be fixed by transforming the **recursion** into a **loop** and storing the function arguments in an explicit stack (rather than the implicit use of the call stack). This is always possible, because the class of [primitive recursive functions](https://en.wikipedia.org/wiki/Primitive_recursive_function) is equivalent to the class of LOOP computable functions. Consider this example in [C++](https://en.wikipedia.org/wiki/C%2B%2B)-like pseudocode:

```c++

void function (argument) 
{
  if (condition)
    function (argument.next);

}
```



```c++
stack.push(argument);
while (!stack.empty())
{
  argument = stack.pop();
  if (condition)
    stack.push(argument.next);
}
```




A **primitive recursive function** like the one on the left side can always be transformed into a loop like on the right side.

A function like the example above on the left would not be a problem in an environment supporting [tail-call optimization](https://en.wikipedia.org/wiki/Tail-call_optimization); however it is still possible to create a recursive function that may result in a stack overflow in these languages. Consider the example below of two simple integer exponentiation functions.

```c++

int pow(int base, int exp) {
    if (exp > 0)
        return base * pow(base, exp - 1);
    else
        return 1;
}
```



```c++
int pow(int base, int exp) {
    return pow_accum(base, exp, 1);
}

int pow_accum(int base, int exp, int accum) {
    if (exp > 0)
        return pow_accum(base, exp - 1, accum * base);
    else
        return accum;
}
```



Both `pow(base, exp)` functions above compute an equivalent result, however, the one on the left is prone to causing a stack overflow because tail-call optimization is not possible for this function. During execution, the stack for these functions will look like this:

```

pow(5, 4)
5 * pow(5, 3)
5 * (5 * pow(5, 2))
5 * (5 * (5 * pow(5, 1)))
5 * (5 * (5 * (5 * pow(5, 0))))
5 * (5 * (5 * (5 * 1)))
625
```

```
pow(5, 4)
pow_accum(5, 4, 1)
pow_accum(5, 3, 5)
pow_accum(5, 2, 25)
pow_accum(5, 1, 125)
pow_accum(5, 0, 625)
625
```



Notice that the function on the left must store in its stack `exp` number of integers, which will be multiplied when the recursion terminates and the function returns 1. In contrast, the function at the right must only store 3 integers at any time, and computes an intermediary result which is passed to its following invocation. As no other information outside of the current function invocation must be stored, a tail-recursion optimizer can "drop" the prior stack frames, eliminating the possibility of a stack overflow.



## Very large stack variables

The other major cause of a stack overflow results from an attempt to allocate more memory on the stack than will fit, for example by creating local array variables that are too large. For this reason some authors recommend that arrays larger than a few kilobytes should be [allocated dynamically](https://en.wikipedia.org/wiki/C_dynamic_memory_allocation) instead of as a local variable.[[6\]](https://en.wikipedia.org/wiki/Stack_overflow#cite_note-onlamp-6)

An example of a very large stack variable in [C](https://en.wikipedia.org/wiki/C_(programming_language)):

```c
int foo() 
{
     double x[1048576];
}
```

On a C implementation with 8 byte [double-precision floats](https://en.wikipedia.org/wiki/Double-precision_floating-point_format), the declared array consumes 8 [mebibytes](https://en.wikipedia.org/wiki/Mebibytes) of data; if this is more memory than is available on the stack (as set by thread creation parameters or operating system limits), a stack overflow will occur.

Stack overflows are made worse by anything that reduces the effective stack size of a given program. For example, the same program being run **without** multiple threads might work fine, but as soon as multi-threading is enabled the program will crash. This is because most programs with threads have less stack space per thread than a program with no threading support. Because kernels are generally multi-threaded, people new to [kernel](https://en.wikipedia.org/wiki/Kernel_(computer_science)) development are usually discouraged from using recursive algorithms or large stack buffers.[[7\]](https://en.wikipedia.org/wiki/Stack_overflow#cite_note-apple1-7)

## See also


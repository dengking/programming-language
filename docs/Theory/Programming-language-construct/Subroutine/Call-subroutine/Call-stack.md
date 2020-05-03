# Call stack

## 维基百科[Call stack](https://en.wikipedia.org/wiki/Call_stack)

In [computer science](https://en.wikipedia.org/wiki/Computer_science), a **call stack** is a [stack](https://en.wikipedia.org/wiki/Stack_(abstract_data_type)) [data structure](https://en.wikipedia.org/wiki/Data_structure) that stores information about the active [subroutines](https://en.wikipedia.org/wiki/Subroutine) of a [computer program](https://en.wikipedia.org/wiki/Computer_program). This kind of stack is also known as an **execution stack**, **program stack**, **control stack**, **run-time stack**, or **machine stack**, and is often shortened to just "the stack". Although maintenance of the call stack is important for the proper functioning of most [software](https://en.wikipedia.org/wiki/Software), the details are normally hidden and automatic in [high-level programming languages](https://en.wikipedia.org/wiki/High-level_programming_language). Many computer [instruction sets](https://en.wikipedia.org/wiki/Instruction_set) provide special instructions for manipulating stacks.

> NOTE: 比如[x86的PUSH 和 POP指令](https://en.wikipedia.org/wiki/Stack_register)。

A **call stack** is used for several related purposes, but the main reason for having one is to keep track of the point to which each **active subroutine** should **return control** when it finishes executing(参见[call trace](https://en.wikipedia.org/wiki/Subroutine#Advantages)). An **active subroutine** is one that has been called but is yet to complete execution after which control should be handed back to the point of call. Such activations of subroutines may be nested to any level (recursive as a special case), hence the stack structure. If, for example, a subroutine `DrawSquare` calls a subroutine `DrawLine` from four different places, `DrawLine` must know where to return when its execution completes. To accomplish this, the [address](https://en.wikipedia.org/wiki/Memory_address) following the call [instruction](https://en.wikipedia.org/wiki/Instruction_(computer_science)), the *return address*, is pushed onto the call stack with each call(参见procedure's [prologue and epilogue](https://en.wikipedia.org/wiki/Function_prologue) ）.

### Description

Since the call stack is organized as a [stack](https://en.wikipedia.org/wiki/Stack_(abstract_data_type)), the caller pushes the **return address** onto the stack, and the called subroutine, when it finishes, [pulls or pops](https://en.wikipedia.org/wiki/Pop_(computer_programming)) the **return address** off the call stack and transfers control to that address. If a called subroutine calls on yet another subroutine, it will push another return address onto the call stack, and so on, with the information **stacking** up and **unstacking** as the program dictates. If the pushing consumes all of the space allocated for the call stack, an error called a [stack overflow](https://en.wikipedia.org/wiki/Stack_overflow) occurs, generally causing the program to [crash](https://en.wikipedia.org/wiki/Crash_(computing)). Adding a subroutine's entry to the call stack is sometimes called "winding"; conversely, removing entries is "unwinding".

There is usually exactly one call stack associated with a running program (or more accurately, with each [task](https://en.wikipedia.org/wiki/Task_(computers)) or [thread](https://en.wikipedia.org/wiki/Thread_(computer_science)) of a [process](https://en.wikipedia.org/wiki/Process_(computing))), although additional stacks may be created for [signal](https://en.wikipedia.org/wiki/Signal_(computing)) handling or [cooperative multitasking](https://en.wikipedia.org/wiki/Cooperative_multitasking) (as with [setcontext](https://en.wikipedia.org/wiki/Setcontext)). Since there is only one in this important context, it can be referred to as *the* stack (implicitly, "of the task"); however, in the [Forth programming language](https://en.wikipedia.org/wiki/Forth_programming_language) the *data stack* or *parameter stack* is accessed more explicitly than the call stack and is commonly referred to as *the* stack (see below).

In [high-level programming languages](https://en.wikipedia.org/wiki/High-level_programming_language), the specifics of the call stack are usually hidden from the programmer. They are given access only to a set of functions, and not the memory on the stack itself. This is an example of [abstraction](https://en.wikipedia.org/wiki/Abstraction_(computer_science)). Most [assembly languages](https://en.wikipedia.org/wiki/Assembly_language), on the other hand, require programmers to be involved with manipulating the stack. The actual details of the stack in a [programming language](https://en.wikipedia.org/wiki/Programming_language) depend upon the [compiler](https://en.wikipedia.org/wiki/Compiler), [operating system](https://en.wikipedia.org/wiki/Operating_system), and the available [instruction set](https://en.wikipedia.org/wiki/Instruction_set).

### [Functions of the call stack](https://en.wikipedia.org/wiki/Call_stack#Functions_of_the_call_stack)

> NOTE: 原文中本段的“**return state**”概念需要注意，它包含了return address。



## Thoughts of call stack

### Call stack of recursion function 

递归函数的调用栈可以使用递归调用树来进行描述，参见《`recursion-analysis-and-representation.md`》

### How to using user stack to replace the call stack of recursion function

如何使用用户自定义的栈来替代call stack，参见《`recursion-to-iteration.md`》


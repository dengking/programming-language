# Calling convention

## wikipedia [Calling convention](https://en.wikipedia.org/wiki/Calling_convention)

In [computer science](https://en.wikipedia.org/wiki/Computer_science), a **calling convention** is an implementation-level (low-level) scheme for how [subroutines](https://en.wikipedia.org/wiki/Subroutine) receive parameters from their caller and how they return a result. Differences in various implementations include where parameters, [return values](https://en.wikipedia.org/wiki/Return_value), return addresses and scope links are placed, and how the tasks of preparing for a function call and restoring the environment afterward are divided between the caller and the callee(以及如何在调用者和被调用者之间划分准备函数调用和恢复环境的任务).

**Calling conventions** may be related to a particular programming language's [evaluation strategy](https://en.wikipedia.org/wiki/Evaluation_strategy) but most often are not considered part of it (or vice versa), as the evaluation strategy is usually defined on a higher abstraction level and seen as a part of the language rather than as a low-level implementation detail of a particular language's [compiler](https://en.wikipedia.org/wiki/Compiler).

### Variations

Calling conventions may differ in:

- Where parameters, return values and return addresses are placed (in [registers](https://en.wikipedia.org/wiki/Processor_register), on the [call stack](https://en.wikipedia.org/wiki/Call_stack), a mix of both, or in other memory structures)
- The order in which actual arguments for formal parameters are passed (or the parts of a large or complex argument)
- How a (possibly long or complex) return value is delivered from the callee back to the caller (on the stack, in a register, or within the heap)
- How the task of setting up for and cleaning up after a function call is divided between the caller and the callee
- Whether and how [metadata](https://en.wikipedia.org/wiki/Metadata) describing the arguments is passed
- Where the previous value of the **frame pointer** is stored, which is used to restore the frame pointer when the routine ends (in the stack frame, or in some register)
- Where any static scope links for the routine's non-local data access are placed (typically at one or more positions in the stack frame, but sometimes in a general register, or, for some architectures, in special-purpose registers)
- How local variables are allocated can sometimes also be part of the calling convention (when the caller allocates for the callee)

In some cases, differences also include the following:

- Conventions on which registers may be directly used by the callee, without being preserved (otherwise regarded as an [ABI](https://en.wikipedia.org/wiki/Application_binary_interface) detail)
- Which registers are considered to be volatile and, if volatile, need not be restored by the callee (often regarded as an ABI detail)

#### Compiler variation

Although *some*[*which?*] languages actually may specify this partially in the [programming language](https://en.wikipedia.org/wiki/Programming_language) specification (or in some pivotal implementation), different implementations of such languages (i.e. different [compilers](https://en.wikipedia.org/wiki/Compiler)) may typically still use various **calling conventions**, often selectable. Reasons for this are performance, frequent adaptation to the conventions of other popular languages (with or without technical reasons), and restrictions or conventions imposed by various "platforms" (combinations of [CPU architectures](https://en.wikipedia.org/wiki/CPU_architecture) and [operating systems](https://en.wikipedia.org/wiki/Operating_system)).

#### Architecture variation

CPU architectures always have more than one possible calling convention[*why?*]. With many general-purpose registers and other features, the potential number of calling conventions is large, although some[*which?*] architectures are formally specified to use only one calling convention, supplied by the architect.

##### x86 (32-bit)

*Main article:* [x86 calling conventions](https://en.wikipedia.org/wiki/X86_calling_conventions)

The [x86 architecture](https://en.wikipedia.org/wiki/X86_architecture) is used with many different calling conventions. Due to the small number of architectural registers, the x86 calling conventions mostly pass arguments on the stack, while the return value (or a pointer to it) is passed in a register. Some conventions use registers for the first few parameters, which may improve performance for short and simple leaf-routines very frequently invoked (i.e. routines that do not call other routines and do not have to be [reentrant](https://en.wikipedia.org/wiki/Reentrant_(subroutine))).

**Example call:**

```assembly
 push EAX            ; pass some register result
 push byte[EBP+20]   ; pass some memory variable (FASM/TASM syntax)
 push 3              ; pass some constant
 call calc           ; the returned result is now in EAX
```

**Typical callee structure:** (*some or all (except ret) of the instructions below may be optimized away in simple procedures*)

```assembly
calc:
  push EBP            ; save old frame pointer
  mov EBP,ESP         ; get new frame pointer
  sub ESP,localsize   ; reserve place for locals
  .
  .                   ; perform calculations, leave result in EAX
  .
  mov ESP,EBP         ; free space for locals
  pop EBP             ; restore old frame pointer
  ret paramsize       ; free parameter space and return
```

##### ARM (A32)

The standard 32-bit [ARM](https://en.wikipedia.org/wiki/ARM_architecture) calling convention allocates the 15 general-purpose registers as:

- r14 is the link register. (The BL instruction, used in a subroutine call, stores the return address in this register).
- r13 is the stack pointer. (The Push/Pop instructions in "Thumb" operating mode use this register only).
- r12 is the Intra-Procedure-call scratch register.
- r4 to r11: used to hold local variables.
- r0 to r3: used to hold argument values passed to a subroutine, and also hold results returned from a subroutine.

The 16th register, r15, is the program counter.

If the type of value returned is too large to fit in r0 to r3, or whose size cannot be determined statically at compile time, then the caller must allocate space for that value at run time, and pass a pointer to that space in r0.

Subroutines must preserve the contents of r4 to r11 and the stack pointer. (Perhaps by saving them to the stack in the function prologue, then using them as scratch space, then restoring them from the stack in the function epilogue). In particular, subroutines that call other subroutines *must* save the return address in the link register r14 to the stack before calling those other subroutines. However, such subroutines do not need to return that value to r14—they merely need to load that value into r15, the program counter, to return.

The ARM calling convention mandates using a full-descending stack.[[1\]](https://en.wikipedia.org/wiki/Calling_convention#cite_note-1)

This calling convention causes a "typical" ARM subroutine to

- In the prologue, push r4 to r11 to the stack, and push the return address in r14, to the stack. (This can be done with a single STM instruction).
- copy any passed arguments (in r0 to r3) to the local scratch registers (r4 to r11).
- allocate other local variables to the remaining local scratch registers (r4 to r11).
- do calculations and call other subroutines as necessary using BL, assuming r0 to r3, r12 and r14 will not be preserved.
- put the result in r0
- In the epilogue, pull r4 to r11 from the stack, and pull the return address to the program counter r15. (This can be done with a single LDM instruction).



## osdev [Calling Conventions](https://wiki.osdev.org/Calling_Conventions)



## draft



往更加宽泛来所，其实是application binary interface，因为programing language的很多东西最终都需要翻译为指令，而application binary interface则是这类场景的总体描述。


函数调用对应的是JMP指令，那声明一个`int`类型的变量对应的是什么指令呢？与此类似的一个问题是：函数调用的时候，需要分配栈空间，那这是如何实现的？

push 指令就可以实现

process在运行过程中的主要活动其实就是不断地函数调用，所以搞清楚函数调用的过程对理解process是非常重要的。龙书的chapter 7就是介绍此的非常好的内容。这些内容我觉得全部都整理到OS book中去。
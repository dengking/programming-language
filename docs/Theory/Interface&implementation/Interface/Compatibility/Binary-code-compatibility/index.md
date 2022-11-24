# Binary-code compatibility

在某些情况下，我们期望"binary-code compatibility"，因为它有着非常好的特性，本节介绍它 ，并且介绍如何来实现它。

## wikipedia [Binary-code compatibility](https://en.wikipedia.org/wiki/Binary-code_compatibility)

**Binary-code compatibility** (binary compatible or [object-code](https://en.wikipedia.org/wiki/Object_code)-compatible) is a property of [computer systems](https://en.wikipedia.org/wiki/Computing) meaning that they can run the same [executable code](https://en.wikipedia.org/wiki/Executable), typically [machine code](https://en.wikipedia.org/wiki/Machine_code) for a [general-purpose computer](https://en.wikipedia.org/wiki/Computer) [CPU](https://en.wikipedia.org/wiki/Central_processing_unit). [Source-code compatibility](https://en.wikipedia.org/wiki/Source-code_compatibility), on the other hand, means that [recompilation](https://en.wikipedia.org/wiki/Code_compilation) or [interpretation](https://en.wikipedia.org/wiki/Interpreter_(computing)) is necessary before the program can be run.

> NOTE: 
>
> 1、通过阅读上面这段话，我的认知是: [Binary-code compatibility](https://en.wikipedia.org/wiki/Binary-code_compatibility) 意味着 不需要  [recompilation](https://en.wikipedia.org/wiki/Code_compilation) or [interpretation](https://en.wikipedia.org/wiki/Interpreter_(computing)) 

For a compiled program on a general operating system, binary compatibility often implies that not only the CPUs ([instruction sets](https://en.wikipedia.org/wiki/Instruction_set_architecture)) of the two computers are binary compatible, but also that interfaces and behaviours of the operating system and [APIs](https://en.wikipedia.org/wiki/Application_programming_interface), and the [ABIs](https://en.wikipedia.org/wiki/Application_binary_interface) corresponding to those APIs, are sufficiently equal, i.e. "compatible".

> NOTE: 
>
> 1、通过阅读上面这段话，我的认知是: [Binary-code compatibility](https://en.wikipedia.org/wiki/Binary-code_compatibility) 意味着:
>
> a、the CPUs ([instruction sets](https://en.wikipedia.org/wiki/Instruction_set_architecture)) of the two computers are binary compatible
>
> b、interfaces and behaviours of the operating system  are sufficiently equal, i.e. "compatible"
>
> c、 [APIs](https://en.wikipedia.org/wiki/Application_programming_interface), and the [ABIs](https://en.wikipedia.org/wiki/Application_binary_interface) are sufficiently equal, i.e. "compatible"
>
> 显然， [Binary-code compatibility](https://en.wikipedia.org/wiki/Binary-code_compatibility) 有着非常严苛的要求。

## Implementation

如何来实现binary-code compatibility呢？在下面的文章中探讨了这个问题:

1、[Opaque pointer](https://en.wikipedia.org/wiki/Opaque_pointer)，收录在 `C++\Idiom\OOP\Pointer-to-implementation-PImpl\Opaque-data-type\Opaque-pointer` 章节
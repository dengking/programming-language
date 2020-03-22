[TOC]

# [Relocation (computing)](https://en.wikipedia.org/wiki/Relocation_(computing))

**Relocation** is the process of assigning **load addresses** for **position-dependent code** and **data** of a program and adjusting the code and data to reflect the **assigned addresses**.[[1\]](https://en.wikipedia.org/wiki/Relocation_(computing)#cite_note-Intel_iRMX-1)[[2\]](https://en.wikipedia.org/wiki/Relocation_(computing)#cite_note-Levine_1999_CH1-2) Prior to the advent of multiprocess systems, and still in many embedded systems the addresses for objects were absolute starting at a known location, often zero. Since multiprocessing systems dynamically link and switch between programs it became necessary to be able to relocate objects using [position-independent code](https://en.wikipedia.org/wiki/Position-independent_code). A [linker](https://en.wikipedia.org/wiki/Linker_(computing)) usually performs relocation in conjunction with **symbol resolution**, the process of searching files and libraries to replace symbolic references or names of [libraries](https://en.wikipedia.org/wiki/Library_(computer_science)) with actual usable addresses in [memory](https://en.wikipedia.org/wiki/Primary_storage) before running a program.

Relocation is typically done by the linker at [link time](https://en.wikipedia.org/wiki/Link_time), but it can also be done at [load time](https://en.wikipedia.org/wiki/Load_time) by a relocating [loader](https://en.wikipedia.org/wiki/Loader_(computing)), or at [run time](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase)) by the running program itself. Some architectures avoid relocation entirely by deferring [address assignment](https://en.wikipedia.org/w/index.php?title=Address_assignment&action=edit&redlink=1) to run time; this is known as [zero address arithmetic](https://en.wikipedia.org/wiki/Zero_address_arithmetic).[*which?*]

***THINKING*** : 编译生成的executable，它们也是有地址空间的

## Segmentation

[Object files](https://en.wikipedia.org/wiki/Object_file) are segmented into various [memory segment](https://en.wikipedia.org/wiki/Memory_segmentation) types. Example segments include [code segment(.text)](https://en.wikipedia.org/wiki/Code_segment), [initialized data segment(.data)](https://en.wikipedia.org/wiki/Data_segment), [uninitialized data segment(.bss](https://en.wikipedia.org/wiki/.bss)), or others.[*clarification needed*]

## Relocation table

The **relocation table** is a list of [pointers](https://en.wikipedia.org/wiki/Pointer_(computer_programming)) created by the translator (a [compiler](https://en.wikipedia.org/wiki/Compiler) or [assembler](https://en.wikipedia.org/wiki/Assembler_(computer_programming)#Assembler)) and stored in the object or executable file. Each entry in the table, or "fixup", is a [pointer](https://en.wikipedia.org/wiki/Pointer_(computer_programming)) to an **absolute address** in the object code that must be changed when the **loader** relocates the program so that it will refer to the correct location. Fixups are designed to support relocation of the program as a complete unit. In some cases, each fixup in the table is itself relative to a **base address** of zero, so the fixups themselves must be changed as the loader moves through the table.[[3\]](https://en.wikipedia.org/wiki/Relocation_(computing)#cite_note-Levine_1999_CH3-3)

In some architectures a fixup that crosses certain boundaries (such as a segment boundary) or that is not aligned on a word boundary is illegal and flagged as an error by the linker.[[4\]](https://en.wikipedia.org/wiki/Relocation_(computing)#cite_note-Borland_2007-4)

***SUMMARY*** : 显然，每个executable都包含一个relocation table。

### Unix-like systems

The [Executable and Linkable Format](https://en.wikipedia.org/wiki/Executable_and_Linkable_Format) (ELF) executable format and shared library format used by most Unix-like systems allows several types of relocation to be defined.[[5\]](https://en.wikipedia.org/wiki/Relocation_(computing)#cite_note-ELF-5)



## Relocation procedure

The **linker** reads **segment information** and **relocation tables** in the object files and performs relocation by:

- merging all **segments** of common type into a single segment of that type
- assigning **unique run time addresses** to each **section** and each symbol, giving all code (functions) and data (global variables) unique run time addresses
- referring to the **relocation table** to modify[*why?*] symbols so that they point to the correct[*clarification needed*] run time addresses.



## Example

The following example uses [Donald Knuth](https://en.wikipedia.org/wiki/Donald_Knuth)'s [MIX](https://en.wikipedia.org/wiki/MIX) architecture and MIXAL assembly language. The principles are the same for any architecture, though the details will change.

[![Relocation example.tif](https://upload.wikimedia.org/wikipedia/commons/thumb/b/b1/Relocation_example.tif/lossy-page1-334px-Relocation_example.tif.jpg)](https://en.wikipedia.org/wiki/File:Relocation_example.tif)

- (A) Program *SUBR* is compiled to produce object file (B), shown as both machine code and assembler. The compiler may start the compiled code at an arbitrary location, often location zero as shown. Location 13 contains the machine code for the jump instruction to statement *ST* in location 5.
- (C) If *SUBR* is later linked with other code it may be stored at a location other than zero. In this example the linker places it at location 120. The address in the jump instruction, which is now at location 133, must be **relocated** to point to the new location of the code for statement *ST*, now 125. [1 61 shown in the instruction is the MIX machine code representation of 125].
- (D) When the program is loaded into memory to run it may be loaded at some location other than the one assigned by the linker. This example shows *SUBR* now at location 300. The address in the jump instruction, now at 313, needs to be relocated again so that it points to the updated location of *ST*, 305. [4 49 is the MIX machine representation of 305].



# [G53OPS - Operating Systems](http://www.cs.nott.ac.uk/~pszgxk/courses/g53ops/Memory%20Management/MM04-relocation.html)

## Relocation and Protection

As soon as we introduce multiprogramming we have two problems that we need to address.

**Relocation** : When a program is run it does not know in advance what **location** it will be loaded at. Therefore, the program cannot simply generate static addresses (e.g. from jump instructions). Instead, they must be made relative to where the program has been loaded.

***SUMMARY*** : 在编译阶段，编译器是无法得知其生成的executable在运行时的location的，因此编译器生成的executable不能够使用static address，它只能够使用relative address；编译器实现relative address的方式是使用symbol。这是使用relocation的原因。

**Protection** : Once you can have two programs in memory at the same time there is a danger that one program can write to the address space of another program. This is obviously dangerous and should be avoided.

In order to cater(迎合) for **relocation** we could make the **loader** modify all the relevant addresses as the binary file is loaded. The OS/360 worked in this way but the scheme suffers from the following problems

> · The program cannot be moved, after it has been loaded without going through the same process.
> · Using this scheme does not help the protection problem as the program can still generate illegal addresses (maybe by using absolute addressing).
> · The program needs to have some sort of **map** that tells the **loader** which addresses need to be modified.

***SUMMARY*** : 上面这段话介绍的内容指出了是由**loader**来执行relocation，以及执行的时机。它还涉及到了relocation的一些实现方法，如引入relocation table。

A solution, which solves both the relocation and protection problem is to equip(配备) the machine with two registers called the ***base*** and ***limit*** registers.

The **base register** stores the start address of the partition and the limit register holds the length of the partition. Any address that is generated by the program has the base register added to it. In addition, all addresses are checked to ensure they are within the range of the partition.

An additional benefit of this scheme is that if a program is moved within memory, only its base register needs to be amended. This is obviously a lot quicker than having to modify every address reference within the program.

The IBM PC uses a scheme similar to this, although it does not have a limit register.

# [What does 'relocation' mean?](https://cs.stackexchange.com/questions/86653/what-does-relocation-mean)


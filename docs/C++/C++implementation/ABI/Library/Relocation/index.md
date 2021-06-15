# wikipedia [Relocation (computing)](https://en.wikipedia.org/wiki/Relocation_(computing))

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



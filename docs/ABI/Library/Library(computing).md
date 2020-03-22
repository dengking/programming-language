# [Library (computing)](https://en.wikipedia.org/wiki/Library_(computing)#Shared_libraries)

![img](https://upload.wikimedia.org/wikipedia/commons/thumb/d/df/Ogg_vorbis_libs_and_application_dia.svg/277px-Ogg_vorbis_libs_and_application_dia.svg.png)



Illustration of an application which uses libvorbisfile to play an [Ogg Vorbis](https://en.wikipedia.org/wiki/Ogg_Vorbis) file

In [computer science](https://en.wikipedia.org/wiki/Computer_science), a **library** is a collection of [non-volatile resources](https://en.wikipedia.org/wiki/Non-volatile_memory) used by [computer programs](https://en.wikipedia.org/wiki/Computer_program), often for [software development](https://en.wikipedia.org/wiki/Software_development). These may include configuration data, documentation, help data, message templates, [pre-written code](https://en.wikipedia.org/wiki/Code_reuse) and [subroutines](https://en.wikipedia.org/wiki/Subroutine), [classes](https://en.wikipedia.org/wiki/Class_(computer_science)), [values](https://en.wikipedia.org/wiki/Value_(computer_science)) or [type](https://en.wikipedia.org/wiki/Data_type) specifications. In [IBM's OS/360 and its successors](https://en.wikipedia.org/wiki/OS/360_and_successors) they are referred to as [partitioned data sets](https://en.wikipedia.org/wiki/Data_set_(IBM_mainframe)#Partitioned_datasets).

A **library** is also a collection of implementations of behavior, written in terms of a language, that has a well-defined **interface** by which the behavior is invoked. For instance, people who want to write a higher level program can use a library to make [system calls](https://en.wikipedia.org/wiki/System_call) instead of implementing those system calls over and over again. In addition, the behavior is provided for reuse by multiple independent programs. A program invokes the library-provided behavior via a mechanism of the language. For example, in a simple [imperative language](https://en.wikipedia.org/wiki/Imperative_language) such as C, the behavior in a library is invoked by using C's normal function-call. What distinguishes the call as being to a library function, versus being to another function in the same program, is the way that the code is organized in the system.

**Library code** is organized in such a way that it can be used by multiple programs that have no connection to each other, while code that is part of a program is organized to be used only within that one program. This distinction can gain a hierarchical notion when a program grows large, such as a multi-million-line program. In that case, there may be internal libraries that are reused by independent sub-portions of the large program. The distinguishing feature is that a library is organized for the purposes of being reused by independent programs or sub-programs, and the user only needs to know the interface and not the internal details of the library.

The value of a library lies in the reuse of the behavior. When a program invokes a library, it gains the behavior implemented inside that library without having to implement that behavior itself. Libraries encourage the **sharing** of code in a [modular](https://en.wikipedia.org/wiki/Modular_programming) fashion, and ease the distribution of the code.

The behavior implemented by a library can be connected to the invoking program at different [program lifecycle phases](https://en.wikipedia.org/wiki/Program_lifecycle_phase). If the code of the library is accessed during the build of the invoking program, then the library is called a [static library](https://en.wikipedia.org/wiki/Static_library).[[1\]](https://en.wikipedia.org/wiki/Library_(computing)#cite_note-Static_Libraries-1) An alternative is to **build** the executable of the invoking program and distribute that, independently of the library implementation. The library behavior is connected after the executable has been invoked to be executed, either as part of the process of starting the execution, or in the middle of execution. In this case the library is called a [dynamic library](https://en.wikipedia.org/wiki/Dynamic_library) (loaded at [run time](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase))). A **dynamic library** can be loaded and linked when preparing a program for execution, by the [linker](https://en.wikipedia.org/wiki/Linker_(computing)). Alternatively, in the middle of execution, an application may explicitly request that a module be [loaded](https://en.wikipedia.org/wiki/Dynamic_loading).

Most [compiled languages](https://en.wikipedia.org/wiki/Compiled_language) have a [standard library](https://en.wikipedia.org/wiki/Standard_library) although programmers can also create their own custom libraries. Most modern [software systems](https://en.wikipedia.org/wiki/Software_system) provide libraries that implement the majority of the system services. Such libraries have [commoditized](https://en.wikipedia.org/wiki/Commoditization) the services which a modern application requires. As such, most code used by modern applications is provided in these system libraries.



## Linking

Main articles: [Link time](https://en.wikipedia.org/wiki/Link_time) and [Linker (computing)](https://en.wikipedia.org/wiki/Linker_(computing))

Libraries are important in the program *linking* or *binding* process, which resolves references known as *links* or *symbols* to library modules. The linking process is usually automatically done by a *linker* or *binder* program that searches a set of libraries and other modules in a given order. Usually it is not considered an error if a link target can be found multiple times in a given set of libraries. Linking may be done when an executable file is created, or whenever the program is used at [run time](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase)).

The references being resolved may be addresses(地址) for jumps and other routine calls. They may be in the main program, or in one module depending upon another. They are resolved into fixed or relocatable addresses (from a common base) by allocating runtime memory for the [memory segments](https://en.wikipedia.org/wiki/Memory_segment) of each module referenced.

Some programming languages may use a feature called *smart linking* whereby the linker is aware of or integrated with the compiler, such that the linker knows how **external references** are used, and code in a library that is never actually *used*, even though internally referenced, can be discarded from the compiled application. For example, a program that only uses integers for arithmetic, or does no arithmetic operations at all, can exclude floating-point library routines. This smart-linking feature can lead to smaller application file sizes and reduced memory usage.



## Relocation

Main article: [Relocation (computer science)](https://en.wikipedia.org/wiki/Relocation_(computer_science))

Some references in a program or library module are stored in a relative or symbolic form which cannot be resolved until all code and libraries are assigned **final static addresses**. *Relocation* is the process of adjusting these references, and is done either by the linker or the [loader](https://en.wikipedia.org/wiki/Loader_(computing)). In general, **relocation** cannot be done to individual libraries themselves because the addresses in memory may vary depending on the program using them and other libraries they are combined with. [Position-independent code](https://en.wikipedia.org/wiki/Position-independent_code) avoids references to absolute addresses and therefore does not require relocation.



## See also

[Program Library HOWTO](http://tldp.org/HOWTO/Program-Library-HOWTO/index.html)

https://stackoverflow.com/questions/480764/linux-error-while-loading-shared-libraries-cannot-open-shared-object-file-no-s
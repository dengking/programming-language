# Opaque data type

1、在学习"pointer to implementation idiom"的时候，发现了Opaque    pointer，进而又发现了"Opaque data type"。

2、"opaque"的中文意思是"晦涩的、不透明的"；显然它的反义词是"transparent"，即透明的。

3、显然，"opaque"就意味着information hiding: information是"opaque"，则意味着hiding。

## wikipedia [Opaque data type](https://en.wikipedia.org/wiki/Opaque_data_type)

In [computer science](https://en.wikipedia.org/wiki/Computer_science), an **opaque data type** is a [data type](https://en.wikipedia.org/wiki/Data_type) whose concrete [data structure](https://en.wikipedia.org/wiki/Data_structure) is not defined in an [interface](https://en.wikipedia.org/wiki/Interface_(computing)). This enforces [information hiding](https://en.wikipedia.org/wiki/Information_hiding), since its [values](https://en.wikipedia.org/wiki/Value_(computer_science)) can only be manipulated by calling [subroutines](https://en.wikipedia.org/wiki/Subroutines) that have access to the missing information. The concrete representation of the type is hidden from its users, and the visible implementation is incomplete. A data type whose representation is visible is called **transparent**.[[1\]](https://en.wikipedia.org/wiki/Opaque_data_type#cite_note-1) Opaque data types are frequently used to implement [abstract data types](https://en.wikipedia.org/wiki/Abstract_data_type).

> NOTE: 
>
> 1、有了前面的描述，再来看 'transparent' 就非常容易理解了

Typical examples of opaque data types include [handles](https://en.wikipedia.org/wiki/Handle_(computing)) for [resources](https://en.wikipedia.org/wiki/Resource_(computer_science)) provided by an [operating system](https://en.wikipedia.org/wiki/Operating_system) to [application software](https://en.wikipedia.org/wiki/Application_software). For example, the [POSIX standard for threads](https://en.wikipedia.org/wiki/POSIX_Threads) defines an [application programming interface](https://en.wikipedia.org/wiki/Application_programming_interface) based on a number of opaque types that represent [threads](https://en.wikipedia.org/wiki/Thread_(computer_science)) or [synchronization primitives](https://en.wikipedia.org/wiki/Synchronization_primitive) like [mutexes](https://en.wikipedia.org/wiki/Mutex) or [condition variables](https://en.wikipedia.org/wiki/Condition_variable).[[2\]](https://en.wikipedia.org/wiki/Opaque_data_type#cite_note-2)

> NOTE: 
>
> 1、这一段的描述中的handle让我想到了file descriptor
>
> 2、这一段总结非常好，它说明清楚了很多C library的做法

An [opaque pointer](https://en.wikipedia.org/wiki/Opaque_pointer) is a special case of an opaque data type, a datatype that is declared to be a [pointer](https://en.wikipedia.org/wiki/Pointer_(computer_programming)) to a [record](https://en.wikipedia.org/wiki/Record_(computer_science)) or [data structure](https://en.wikipedia.org/wiki/Data_structure) of some unspecified data type. For example, the [standard library](https://en.wikipedia.org/wiki/Standard_library) that forms part of the specification of the [C programming language](https://en.wikipedia.org/wiki/C_(programming_language)) provides functions for [file](https://en.wikipedia.org/wiki/Computer_file) [input and output](https://en.wikipedia.org/wiki/Input_and_output) that return or take values of type "pointer to `FILE`" that represent file streams (see [C file input/output](https://en.wikipedia.org/wiki/C_file_input/output)), but the concrete implementation of the type `FILE` is not specified.[[3\]](https://en.wikipedia.org/wiki/Opaque_data_type#cite_note-3)



### Uses in various languages

Some languages, such as [C](https://en.wikipedia.org/wiki/C_programming_language), allow the declaration of opaque [records](https://en.wikipedia.org/wiki/Record_(computer_science)) (structs), whose size and fields are hidden from the client. The only thing that the client can do with an object of such a type is to take its [memory address](https://en.wikipedia.org/wiki/Memory_address), to produce an [opaque pointer](https://en.wikipedia.org/wiki/Opaque_pointer).
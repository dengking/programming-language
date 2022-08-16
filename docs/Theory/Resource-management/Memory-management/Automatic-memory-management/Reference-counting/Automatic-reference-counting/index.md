# Automatic Reference Counting



## wikipedia [Automatic Reference Counting](https://en.wikipedia.org/wiki/Automatic_Reference_Counting)

**Automatic Reference Counting** (**ARC**) is a [memory management](https://en.wikipedia.org/wiki/Memory_management) feature of the [Clang](https://en.wikipedia.org/wiki/Clang) [compiler](https://en.wikipedia.org/wiki/Compiler) providing automatic [reference counting](https://en.wikipedia.org/wiki/Reference_counting) for the [Objective-C](https://en.wikipedia.org/wiki/Objective-C) and [Swift](https://en.wikipedia.org/wiki/Swift_(programming_language)) [programming languages](https://en.wikipedia.org/wiki/Programming_language). At compile time, it inserts into the [object code](https://en.wikipedia.org/wiki/Object_code) [messages](https://en.wikipedia.org/wiki/Object-oriented_programming#Dynamic_dispatch/message_passing) `retain` and `release`[[1\]](https://en.wikipedia.org/wiki/Automatic_Reference_Counting#cite_note-Ars-1)[[2\]](https://en.wikipedia.org/wiki/Automatic_Reference_Counting#cite_note-Kochan-2) which increase and decrease the reference count at run time, marking for [deallocation](https://en.wikipedia.org/wiki/Deallocation) those [objects](https://en.wikipedia.org/wiki/Object_(computer_science)) when the number of references to them reaches zero.


# Handle 



## wikipedia [Handle (computing)](https://en.wikipedia.org/wiki/Handle_(computing))

In [computer programming](https://en.wikipedia.org/wiki/Computer_programming), a **handle** is an abstract [reference](https://en.wikipedia.org/wiki/Reference_(computer_science)) to a [resource](https://en.wikipedia.org/wiki/System_resource) that is used when [application software](https://en.wikipedia.org/wiki/Application_software) references blocks of [memory](https://en.wikipedia.org/wiki/Memory_(computing)) or objects that are managed by another system like a [database](https://en.wikipedia.org/wiki/Database) or an [operating system](https://en.wikipedia.org/wiki/Operating_system).

A **resource handle** 

1、can be an [opaque](https://en.wikipedia.org/wiki/Opaque_data_type) [identifier](https://en.wikipedia.org/wiki/Identifier), in which case it is often an [integer](https://en.wikipedia.org/wiki/Integer) number (often an [array index](https://en.wikipedia.org/wiki/Array_index) in an array or "table" that is used to manage that type of resource), or it 

2、can be a [pointer](https://en.wikipedia.org/wiki/Pointer_(computer_programming)) that allows access to further information. 

Common resource handles include [file descriptors](https://en.wikipedia.org/wiki/File_descriptor), [network sockets](https://en.wikipedia.org/wiki/Network_socket), [database connections](https://en.wikipedia.org/wiki/Database_connection), [process identifiers](https://en.wikipedia.org/wiki/Process_identifier) (PIDs), and [job IDs](https://en.wikipedia.org/wiki/Job_ID). PIDs and job IDs are explicitly visible integers; while file descriptors and sockets (which are often implemented as a form of file descriptor) are represented as integers, they are typically considered opaque. In traditional implementations, file descriptors are indices into a (per-process) [file descriptor table](https://en.wikipedia.org/wiki/File_descriptor_table), thence a (system-wide) [file table](https://en.wikipedia.org/wiki/File_table).

> NOTE: 
>
> 1、通过前面的描述可以看出，handle是无处不在的，尤其是在OS、DBMS中
>
> 2、opaque VS visible

### Comparison to pointers

> NOTE: 
>
> 1、handle is a good abstraction、indirection
>
> 2、从下面这一段可以看出，
>
> 使用handle的优势:
>
> a、opacity，因此可以避免dangling
>
> 使用handle的劣势:
>
> a、"extra layer of [indirection](https://en.wikipedia.org/wiki/Indirection) "，显然增加了computation overhead

While a [pointer](https://en.wikipedia.org/wiki/Pointer_(computer_programming)) contains the [address](https://en.wikipedia.org/wiki/Memory_address) of the item to which it refers, a handle is an [abstraction](https://en.wikipedia.org/wiki/Abstraction_(computer_science)) of a reference which is managed externally(外部的); its opacity(不透明性) allows the referent to be **relocated** in memory by the system without invalidating the handle, which is impossible with pointers. The extra layer of [indirection](https://en.wikipedia.org/wiki/Indirection) also increases the control that the managing system has over the operations performed on the referent. Typically the handle is an index or a pointer into a global array of [tombstones](https://en.wikipedia.org/wiki/Tombstone_(programming)).

> NOTE: 
>
> 1、"tombstone"的表面意思是"墓石"
>
> 2、最后一段话，点明了handle的实现

A [handle leak](https://en.wikipedia.org/wiki/Handle_leak) is a type of [software bug](https://en.wikipedia.org/wiki/Software_bug) that occurs when a computer program asks for a handle to a resource but does not free the handle when it is no longer used; this is a form of [resource leak](https://en.wikipedia.org/wiki/Resource_leak), similar to a [memory leak](https://en.wikipedia.org/wiki/Memory_leak) for a pointer to memory.

### Security

> NOTE: 
>
> 1、"Security"是一个非常重要的问题
>
> 2、a handle functions as a *[capability](https://en.wikipedia.org/wiki/Capability-based_security)*: it not only identifies an object, but also associates [access rights](https://en.wikipedia.org/wiki/Access_control)".
>
> 这段话非常重要

In [secure computing](https://en.wikipedia.org/wiki/Computer_security) terms, because access to a resource via a handle is mediated(中转) by another system, a handle functions as(充当) a *[capability](https://en.wikipedia.org/wiki/Capability-based_security)*: it not only identifies an object, but also associates [access rights](https://en.wikipedia.org/wiki/Access_control). For example, while a filename is forgeable(可以伪造的) (it is just a guessable(可以推测的) identifier), a handle is *given* to a user by an external system, and thus represents not just identity, but also *granted* access.

For example, if a program wishes to read the system password file (`/etc/passwd`) in read/write mode (`O_RDWR`), it could try to open the file via the following call:

```c++
int fd = open("/etc/passwd", O_RDWR);
```

This call asks the operating system to open the specified file with the specified access rights. If the OS allows this, then it opens the file (creates an entry in the per-process [file descriptor table](https://en.wikipedia.org/wiki/File_descriptor_table)) and returns a handle (file descriptor, index into this table) to the user: the actual access is controlled by the OS, and the handle is a [token](https://en.wikipedia.org/wiki/Access_token) of that. Conversely, the OS may deny access, and thus neither open the file nor return a handle.

#### In a capability-based system

In a capability-based system, handles can be passed between processes, with associated access rights. Note that in these cases the handle must be something other than a system-wide-unique small integer, otherwise it is forgeable. Such an integer may nevertheless be used to identify a capability inside a process; e.g., file descriptor in Linux is unforgeable because its numerical value alone is meaningless, and only in the process context may refer to anything. Transferring such a handle requires special care though, as its value often has to be different in the sending and receiving processes.

> NOTE: 
>
> 1、翻译如下:
>
> "在基于功能的系统中，句柄可以在进程之间传递，并具有关联的访问权限。注意，在这些情况下，句柄必须不是系统唯一的小整数，否则它是可伪造的。然而，这样的整数可用于标识进程内部的一种能力; 例如，Linux中的文件描述符是不可伪造的，因为它的数值本身是没有意义的，而且只有在进程上下文中才可以引用任何东西。传输这样一个句柄需要特别小心，因为它的值在发送和接收过程中经常是不同的。"
>
> 2、解释如下:
>
> a、理解上面这段话，需要对Linux OS有非常好的理解，我们需要由如下的认知:
>
> a1、system-wide scope
>
> a2、process-wide scope
>
> b、"Note that in these cases the handle must be something other than a system-wide-unique small integer, otherwise it is forgeable. "
>
> 这段话是容易理解的: 如果handle是一个"system-wide-unique small integer"，那么我们就可以非常容易伪造出一个handle了，这就是"forgeable"
>
> c、"**file descriptor in Linux is unforgeable** because its numerical value alone is meaningless, and only in the process context may refer to anything. Transferring such a handle requires special care though, as its value often has to be different in the sending and receiving processes."
>
> 这段话的意思是: file descriptor 是 unforgeable 

#### In non-capability-based systems

> NOTE: 这段话没有理解

In non-capability-based systems, on the other hand, each process must acquire its own separate handle, by specifying the identity of the resource and the desired access rights (e.g., each process must open a file itself, by giving the filename and access mode). Such usage is more common even in modern systems that do support passing handles, but it is subject to vulnerabilities like the [confused deputy problem](https://en.wikipedia.org/wiki/Confused_deputy_problem).

### Examples

Handles were a popular solution to [memory management](https://en.wikipedia.org/wiki/Memory_management) in operating systems of the 1980s, such as [Mac OS](https://en.wikipedia.org/wiki/Mac_OS)[[1\]](https://en.wikipedia.org/wiki/Handle_(computing)#cite_note-1) and [Windows](https://en.wikipedia.org/wiki/Microsoft_Windows). 

The FILE data structure in the [C standard I/O library](https://en.wikipedia.org/wiki/Stdio) is a [file handle](https://en.wikipedia.org/wiki/File_handle), abstracting from the underlying file representation (on [Unix](https://en.wikipedia.org/wiki/Unix) these are [file descriptors](https://en.wikipedia.org/wiki/File_descriptor)). 

Like other [desktop environments](https://en.wikipedia.org/wiki/Desktop_environment), the [Windows API](https://en.wikipedia.org/wiki/Windows_API) heavily uses handles to represent objects in the system and to provide a communication pathway between the operating system and [user space](https://en.wikipedia.org/wiki/User_space). For example, a window on the [desktop](https://en.wikipedia.org/wiki/Desktop_metaphor) is represented by a handle of type `HWND` (handle, window).

[Doubly indirect](https://en.wikipedia.org/wiki/Pointer_(computer_programming)#Multiple_indirection) handles have fallen out of favour in recent times, as increases in available memory and improved [virtual memory](https://en.wikipedia.org/wiki/Virtual_memory) algorithms have made the use of the simpler pointer more attractive. However, many operating systems still apply the term to pointers to opaque, "private" [data structures](https://en.wikipedia.org/wiki/Data_structure)—[opaque pointers](https://en.wikipedia.org/wiki/Opaque_pointer)—or to indexes into internal arrays passed from one [process](https://en.wikipedia.org/wiki/Process_(computing)) to its [client](https://en.wikipedia.org/wiki/Client_(computing)).

> NOTE: 翻译如下:
>
> "最近，由于可用内存的增加和改进的虚拟内存算法使得使用更简单的指针变得更有吸引力，双重间接句柄已经不再受欢迎。然而，许多操作系统仍然将这个术语应用于指向不透明的“私有”数据结构的指针——不透明指针——或者从一个进程传递到其客户机的内部数组的索引。"
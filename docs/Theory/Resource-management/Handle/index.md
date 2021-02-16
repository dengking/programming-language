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
> 1、handle is a good abstraction
>
> 2、从下面这一段可以看出，使用handle的优势

While a [pointer](https://en.wikipedia.org/wiki/Pointer_(computer_programming)) contains the [address](https://en.wikipedia.org/wiki/Memory_address) of the item to which it refers, a handle is an [abstraction](https://en.wikipedia.org/wiki/Abstraction_(computer_science)) of a reference which is managed externally(外部的); its opacity(不透明性) allows the referent to be relocated in memory by the system without invalidating the handle, which is impossible with pointers. The extra layer of [indirection](https://en.wikipedia.org/wiki/Indirection) also increases the control that the managing system has over the operations performed on the referent. Typically the handle is an index or a pointer into a global array of [tombstones](https://en.wikipedia.org/wiki/Tombstone_(programming)).

A [handle leak](https://en.wikipedia.org/wiki/Handle_leak) is a type of [software bug](https://en.wikipedia.org/wiki/Software_bug) that occurs when a computer program asks for a handle to a resource but does not free the handle when it is no longer used; this is a form of [resource leak](https://en.wikipedia.org/wiki/Resource_leak), similar to a [memory leak](https://en.wikipedia.org/wiki/Memory_leak) for a pointer to memory.
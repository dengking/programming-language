# [Out of memory](https://en.wikipedia.org/wiki/Out_of_memory)

**Out of memory** (**OOM**) is an often undesired state of computer operation where no additional memory can be allocated for use by programs or the operating system. Such a system will be unable to load any additional programs, and since many programs may load additional data into memory during execution, these will cease(终止) to function correctly. This usually occurs because all available memory, including disk [swap space](https://en.wikipedia.org/wiki/Swap_space), has been allocated.



## History

Historically, the out of memory condition was more common than it is now, since early computers and [operating systems](https://en.wikipedia.org/wiki/Operating_system) were limited to small amounts of physical [random-access memory](https://en.wikipedia.org/wiki/Random-access_memory) (RAM) due to the inability of early processors to address large amounts of memory, as well as cost considerations. Since the advent of [virtual memory](https://en.wikipedia.org/wiki/Virtual_memory) opened the door for the usage of **swap space**, the condition is less frequent. Almost all modern programs expect to be able to allocate and deallocate memory freely at run-time, and tend to fail in uncontrolled ways (crash) when that expectation is not met; older ones often allocated memory only once, checked whether they got enough to do all their work, and then expected no more to be forthcoming. Therefore, they would either fail immediately with an "out of memory" error message, or work as expected.[*citation needed*]

Early operating systems such as [MS-DOS](https://en.wikipedia.org/wiki/MS-DOS) lacked support for [multitasking](https://en.wikipedia.org/wiki/Computer_multitasking). Programs were allocated physical memory that they could use as they needed. Physical memory was often a scarce resource, and when it was exhausted by applications such as those with [Terminate and Stay Resident](https://en.wikipedia.org/wiki/Terminate_and_Stay_Resident) functionality, no further applications could be started until running applications were closed.

Modern operating systems provide **virtual memory**, in which processes are given a range of memory, but where the memory does not directly correspond to actual physical RAM. Virtual memory can be backed by physical RAM, a **disk file** via [mmap](https://en.wikipedia.org/wiki/Mmap), or **swap space**, and the operating system can move **virtual memory pages** around as it needs. Because **virtual memory** does not need to be backed by physical memory, exhaustion of it is rare, and usually there are other limits imposed by the operating system on resource consumption.[*citation needed*]

***SUMMARY*** : 参见[Virtual address space](https://en.wikipedia.org/wiki/Virtual_address_space) 

As predicted by [Moore's law](https://en.wikipedia.org/wiki/Moore's_law), the amount of physical memory in all computers has grown almost exponentially, although this is offset to some degree by programs and files themselves becoming larger. In some cases, a computer with virtual memory support where the majority of the loaded data resides on the hard disk may run out of **physical memory** but not **virtual memory**, thus causing excessive(过度的) [paging](https://en.wikipedia.org/wiki/Paging). This condition, known as [thrashing](https://en.wikipedia.org/wiki/Thrashing_(computer_science)), usually renders the computer unusable until some programs are closed or the machine is rebooted. Due to these reasons, an out of memory message is rarely encountered by applications with modern computers.[*citation needed*]

***SUMMARY*** : 正如摩尔定律所预测的那样，所有计算机的物理内存几乎呈指数级增长，尽管这在一定程度上被程序和文件本身变大所抵消。在某些情况下，具有虚拟内存支持的计算机(其中大多数加载的数据驻留在硬盘上)可能会耗尽物理内存，而不是虚拟内存，从而导致过度分页。这种情况称为抖动，通常使计算机无法使用，直到一些程序关闭或重新启动计算机。由于这些原因，使用现代计算机的应用程序很少会遇到内存不足的消息。

It is, however, still possible to encounter an OOM condition with a modern computer. The typical OOM case in modern computers happens when the operating system is unable to create any more virtual memory, because all of its potential **backing devices** have been filled or the end-user has disabled them.



## Out of memory management

The kernels of operating systems such as [Linux](https://en.wikipedia.org/wiki/Linux_Kernel) will attempt to recover from this type of OOM condition by terminating one or more processes, a mechanism known as the *OOM Killer*.[[1\]](https://en.wikipedia.org/wiki/Out_of_memory#cite_note-How_to_Configure_the_Linux_Out-of-Memory_Killer-1) [Linux 4.6](https://en.wikipedia.org/wiki/Linux_kernel#4.x.y_releases) (released in May 2016) introduced changes in OOM situations, improving detection and reliability.[[2\]](https://en.wikipedia.org/wiki/Out_of_memory#cite_note-2)[[3\]](https://en.wikipedia.org/wiki/Out_of_memory#cite_note-3), [cgroup](https://en.wikipedia.org/wiki/Cgroup) awareness in OOM killer was implemented in Linux Kernel 4.19 released in October 2018, which adds an ability to kill a `cgroup` as a single unit [[4\]](https://en.wikipedia.org/wiki/Out_of_memory#cite_note-4).



## Per-process memory limits

Apart from the system-wide physical memory limits, some systems limit the amount of memory each process can use. Usually a matter of policy, such a limitation can also happen when the OS has a larger address space than is available at the process level. Some high-end [32-bit](https://en.wikipedia.org/wiki/32-bit) systems (such as those with [Physical Address Extension](https://en.wikipedia.org/wiki/Physical_Address_Extension) enabled) come with 8 [gigabytes](https://en.wikipedia.org/wiki/Gigabyte) or more of system memory, even though any single process can only access 4GB of it in a 32-bit [flat memory model](https://en.wikipedia.org/wiki/Flat_memory_model).

A process which exceeds its per-process limit and then attempts to allocate further memory will encounter an error condition. For example, the [C standard function](https://en.wikipedia.org/wiki/C_standard_function) for allocating memory, `malloc()`, will return [NULL](https://en.wikipedia.org/wiki/Null_pointer) and a well-behaved application should handle this situation.

***THINKING*** : `C++`中的`new`是否需要做NULL的检查


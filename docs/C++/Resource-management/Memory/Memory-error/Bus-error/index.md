# Bus error





## geeksforgeeks [Segmentation Fault (SIGSEGV) vs Bus Error (SIGBUS)](https://www.geeksforgeeks.org/segmentation-fault-sigsegv-vs-bus-error-sigbus/)

**Segmentation fault(SIGSEGV)** and **Bus error(SIGBUS)** are signals generated when serious program error is detected by the operating system and there is no way the program could continue to execute because of these errors.

### Segmentation Fault

**1) [Segmentation Fault](https://www.geeksforgeeks.org/core-dump-segmentation-fault-c-cpp/)** (also known as SIGSEGV and is usually signal 11) occur when the program tries to write/read outside the memory allocated for it or when writing memory which can only be read.

In other words when the program tries to access the memory to which it doesn’t have access to. 

SIGSEGV is abbreviation for “Segmentation Violation”. 

Few cases where SIGSEGV signal generated are as follows,

-> Using uninitialized pointer

-> De-referencing a NULL pointer

-> Trying to access memory that the program doesn’t own (eg. trying to access an array element
out of array bounds).

-> Trying to access memory which is already de-allocated (trying to use dangling pointers).
Please refer [this](https://www.geeksforgeeks.org/core-dump-segmentation-fault-c-cpp/) article for examples.

### Bus Error

**2) [Bus Error](https://en.wikipedia.org/wiki/Bus_error)** (also known as SIGBUS and is usually signal 10) occur when a process is trying to access memory that the CPU cannot physically address. In other words the memory tried to access by the program is not a valid memory address. It caused due to alignment issues with the CPU (eg. trying to read a long from an address which isn’t a multiple of 4). 

SIGBUS is abbrivation for “Bus Error”.

SIGBUS signal occurs in below cases,

-> Program instructs the CPU to read or write a specific physical memory address which is not valid / Requested physical address is unrecognized by the whole computer system.

-> Unaligned access of memory (For example, if multi-byte accesses must be 16 bit-aligned, addresses (given in bytes) at 0, 2, 4, 6, and so on would be considered aligned and therefore accessible, while addresses 1, 3, 5, and so on would be considered unaligned.)

### The main difference

**The main difference** between Segmentation Fault and Bus Error is that Segmentation Fault indicates an invalid access to a valid memory, while Bus Error indicates an access to an invalid address.
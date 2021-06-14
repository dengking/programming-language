# Chapter 20. The `bitmap_allocator`

## Design

As this name suggests, this allocator uses a **bit-map** to keep track of the used and unused memory locations for its book-keeping purposes.



The bitmapped allocator's internal pool is exponentially growing. Meaning that internally, the blocks acquired from the Free List Store will double every time the bitmapped allocator runs out of memory.



## Implementation

### Free List Store



### Super Block



### Super Block Data Layout





## codescracker [C++ Free Store](https://codescracker.com/cpp/cpp-free-store.htm)

**Free Store** is a pool of unallocated heap memory given to a program that is used by the program for **dynamic allocation** during the execution of program.

Every program is provided with a pool of unallocated heap memory that it may utilize during the execution. This pool of available memory is referred to as free store of the program.


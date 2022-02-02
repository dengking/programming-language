# Chapter 20. The `bitmap_allocator`

## Design

As this name suggests, this allocator uses a **bit-map** to keep track of the used and unused memory locations for its book-keeping purposes.



The bitmapped allocator's internal pool is exponentially growing. Meaning that internally, the blocks acquired from the Free List Store will double every time the bitmapped allocator runs out of memory.



## Implementation

### Free List Store



### Super Block



### Super Block Data Layout





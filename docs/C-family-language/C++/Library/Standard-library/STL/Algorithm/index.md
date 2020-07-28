# 关于本章

本章描述C++ algorithms library。



## cppreference [Algorithms library](https://en.cppreference.com/w/cpp/algorithm)

The **algorithms library** defines functions for a variety of purposes (e.g. searching, sorting, counting, manipulating) that operate on **ranges** of elements. Note that a range is defined as `[first, last)` where `last` refers to the element *past* the last element to inspect or modify.

> NOTE: 关于range，参见`C++\Library\Standard-library\STL\Range`。



### Non-modifying sequence operations

Defined in header `<algorithm>` 

### Modifying sequence operations

Defined in header `<algorithm>` 

### Partitioning operations

Defined in header `<algorithm>` 

### Sorting operations

Defined in header `<algorithm>` 

#### Binary search operations (on sorted ranges)

Defined in header `<algorithm>` 

#### Other operations on sorted ranges

Defined in header `<algorithm>` 

### Set operations (on sorted ranges)

Defined in header `<algorithm>` 

### Heap operations

Defined in header `<algorithm>` 

### Minimum/maximum operations

Defined in header `<algorithm>` 

### Comparison operations



### Permutation operations



### Numeric operations

Defined in header `<numeric>`

### Operations on uninitialized memory

Defined in header `<memory>` 



## Implementation

### gcc libstdc++

[gcc](https://github.com/gcc-mirror/gcc/tree/d9375e490072d1aae73a93949aa158fcd2a27018)/[libstdc++-v3](https://github.com/gcc-mirror/gcc/tree/d9375e490072d1aae73a93949aa158fcd2a27018/libstdc%2B%2B-v3)/[include](https://github.com/gcc-mirror/gcc/tree/d9375e490072d1aae73a93949aa158fcd2a27018/libstdc%2B%2B-v3/include)/[bits](https://github.com/gcc-mirror/gcc/tree/d9375e490072d1aae73a93949aa158fcd2a27018/libstdc%2B%2B-v3/include/bits)/**stl_algo.h**



### llvm libcxx

[libcxx](https://github.com/llvm-mirror/libcxx/tree/a12cb9d211019d99b5875b6d8034617cbc24c2cc)/[include](https://github.com/llvm-mirror/libcxx/tree/a12cb9d211019d99b5875b6d8034617cbc24c2cc/include)/**algorithm**



### MSVC stdlib

[MSVC stdlib](https://github.com/microsoft/STL/blob/ff83542af4b683fb2f2dea1423fd6c50fe3e13b0/stl/inc/algorithm)




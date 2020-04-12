# Example of undefined behavior

## Attempting to modify a string literal

cppreference c++ [string literal](https://en.cppreference.com/w/cpp/language/string_literal) 和 cppreference c [string literals](https://en.cppreference.com/w/c/language/string_literal)中，都对此进行了说明，cppreference c [string literals](https://en.cppreference.com/w/c/language/string_literal)对此进行了解释：

> String literals are **not modifiable** (and in fact may be placed in read-only memory such as `.rodata`).

`.rodata`中`ro`的含义是read only。



## Overflow and underflow
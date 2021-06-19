# AddressSanitizer[¶](https://clang.llvm.org/docs/AddressSanitizer.html#addresssanitizer)

> NOTE: 
>
> 能够检查出，大多数的memory error
>
> 

## [Introduction](https://clang.llvm.org/docs/AddressSanitizer.html#id1)

AddressSanitizer is a fast memory error detector. It consists of a compiler instrumentation module and a run-time library. The tool can detect the following types of bugs:

- Out-of-bounds accesses to heap, stack and globals
- Use-after-free
- Use-after-return (runtime flag ASAN_OPTIONS=detect_stack_use_after_return=1)
- Use-after-scope (clang flag -fsanitize-address-use-after-scope)
- Double-free, invalid free
- Memory leaks (experimental)
# Assembling a Complete Toolchain

clang仅仅是一个front end，要想完全替代gcc还需要结合本文的内容。

## [Assembling a Complete Toolchain](https://clang.llvm.org/docs/Toolchain.html)

### [Introduction](https://clang.llvm.org/docs/Toolchain.html#id8)[¶](https://clang.llvm.org/docs/Toolchain.html#introduction)

#### [Tools](https://clang.llvm.org/docs/Toolchain.html#id9)[¶](https://clang.llvm.org/docs/Toolchain.html#tools)



- **Preprocessor**

- **Parsing**

- **IR generation**

- **Compiler backend**

- **Assembler**

- **Linker**

Clang provides all of these pieces other than the linker.

### [Runtime libraries](https://clang.llvm.org/docs/Toolchain.html#id14)[¶](https://clang.llvm.org/docs/Toolchain.html#runtime-libraries)

A number of different runtime libraries are required to provide different layers of support for C family programs. Clang will implicitly link an appropriate implementation of each runtime library, selected based on target defaults or explicitly selected by the `--rtlib=` and `--stdlib=` flags.

> NOTE: 需要做的是使用`--rtlib=` and `--stdlib=` flags来使用llvm的library。

#### [Compiler runtime](https://clang.llvm.org/docs/Toolchain.html#id15)[¶](https://clang.llvm.org/docs/Toolchain.html#compiler-runtime)



##### compiler-rt (LLVM)[¶](https://clang.llvm.org/docs/Toolchain.html#compiler-rt-llvm)

##### libgcc_s (GNU)[¶](https://clang.llvm.org/docs/Toolchain.html#libgcc-s-gnu)

#### [Atomics library](https://clang.llvm.org/docs/Toolchain.html#id16)[¶](https://clang.llvm.org/docs/Toolchain.html#atomics-library)

##### compiler-rt (LLVM)[¶](https://clang.llvm.org/docs/Toolchain.html#id1)

##### libatomic (GNU)[¶](https://clang.llvm.org/docs/Toolchain.html#libatomic-gnu)
[TOC]



# [Clang vs Other Open Source Compilers](https://clang.llvm.org/comparison.html)

Building an entirely new compiler front-end is a big task, and it isn't always clear to people why we decided to do this. Here we compare Clang and its **goals** to other open source compiler front-ends that are available. We restrict the discussion to very specific objective points to avoid controversy where possible. Also, software is infinitely mutable, so we don't talk about little details that can be fixed with a reasonable amount of effort: we'll talk about issues that are difficult to fix for architectural or political reasons.

The goal of this list is to describe how **differences in goals lead to different strengths and weaknesses**, not to make some compiler look bad. This will hopefully help you to evaluate whether using Clang is a good idea for your personal goals. Because we don't know specifically what *you* want to do, we describe the features of these compilers in terms of *our* goals: if you are only interested in static analysis, you may not care that something lacks codegen support, for example.

Please email [cfe-dev](https://clang.llvm.org/get_involved.html) if you think we should add another compiler to this list or if you think some characterization is unfair here.

- [Clang vs GCC](https://clang.llvm.org/comparison.html#gcc) (GNU Compiler Collection)
- [Clang vs Elsa](https://clang.llvm.org/comparison.html#elsa) (Elkhound-based C++ Parser)
- [Clang vs PCC](https://clang.llvm.org/comparison.html#pcc) (Portable C Compiler)

## Clang vs GCC (GNU Compiler Collection)

Pro's of GCC vs Clang:

- GCC supports languages that Clang does not aim to, such as Java, Ada, FORTRAN, Go, etc.
- GCC supports more targets than LLVM.
- GCC supports many language extensions, some of which are not implemented by Clang. For instance, in C mode, GCC supports [nested functions](https://gcc.gnu.org/onlinedocs/gcc/Nested-Functions.html) and has an [extension allowing VLAs in structs](https://gcc.gnu.org/onlinedocs/gcc/Variable-Length.html).

Pro's of Clang vs GCC:

- The Clang [ASTs](https://en.wikipedia.org/wiki/Abstract_syntax_tree) and design are intended to be [easily understandable](https://clang.llvm.org/features.html#simplecode) by anyone who is familiar with the languages involved and who has a basic understanding of how a compiler works. GCC has a very old codebase which presents a steep learning curve to new developers.
- Clang is designed as an API from its inception, allowing it to be reused by source analysis tools, refactoring, IDEs (etc) as well as for code generation. GCC is built as a monolithic static compiler, which makes it extremely difficult to use as an API and integrate into other tools. Further, its historic design and [current](https://gcc.gnu.org/ml/gcc/2007-11/msg00460.html) [policy](https://gcc.gnu.org/ml/gcc/2004-12/msg00888.html) makes it difficult to decouple the front-end from the rest of the compiler.
- Various GCC design decisions make it very difficult to reuse: its build system is difficult to modify, you can't link multiple targets into one binary, you can't link multiple front-ends into one binary, it uses a custom garbage collector, uses global variables extensively, is not reentrant or multi-threadable, etc. Clang has none of these problems.
- Clang does not implicitly simplify code as it parses it like GCC does. Doing so causes many problems for source analysis tools: as one simple example, if you write "x-x" in your source code, the GCC AST will contain "0", with no mention of 'x'. This is extremely bad for a refactoring tool that wants to rename 'x'.
- Clang can serialize its AST out to disk and read it back into another program, which is useful for whole program analysis. GCC does not have this. GCC's PCH mechanism (which is just a dump of the compiler memory image) is related, but is architecturally only able to read the dump back into the exact same executable as the one that produced it (it is not a structured format).
- Clang is [much faster and uses far less memory](https://clang.llvm.org/features.html#performance) than GCC.
- Clang has been designed from the start to provide extremely clear and concise diagnostics (error and warning messages), and includes support for [expressive diagnostics](https://clang.llvm.org/diagnostics.html). Modern versions of GCC have made significant advances in this area, incorporating various Clang features such as preserving typedefs in diagnostics and showing macro expansions, but GCC is still catching up.
- GCC is licensed under the GPL license. [Clang uses a BSD license,](https://clang.llvm.org/features.html#license) which allows it to be embedded in software that is not GPL-licensed.
- Clang inherits a number of features from its use of LLVM as a backend, including support for a bytecode representation for intermediate code, pluggable optimizers, link-time optimization support, Just-In-Time compilation, ability to link in multiple code generators, etc.
- [Clang's support for C++](https://clang.llvm.org/compatibility.html#cxx) is more compliant than GCC's in many ways.
- Clang supports [many language extensions](https://clang.llvm.org/docs/LanguageExtensions.html), some of which are not implemented by GCC. For instance, Clang provides attributes for checking thread safety and extended vector types.



> NOTE: 关于gcc架构设计的缺陷，在[LLVM](http://www.aosabook.org/en/llvm.html)中也给出了非常好的解释。
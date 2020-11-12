# LLVM Language Reference Manual[¶](http://llvm.org/docs/LangRef.html#llvm-language-reference-manual)

## [Abstract](http://llvm.org/docs/LangRef.html#id1242)[¶](http://llvm.org/docs/LangRef.html#abstract)

LLVM is a **Static Single Assignment** (SSA) based representation that provides type safety, low-level operations, flexibility, and the capability of representing ‘all’ high-level languages cleanly. It is the common code representation used throughout all phases of the LLVM compilation strategy.

## [Introduction](http://llvm.org/docs/LangRef.html#id1243)[¶](http://llvm.org/docs/LangRef.html#introduction)

The LLVM code representation is designed to be used in three different forms: 

1) as an in-memory compiler IR

2) as an on-disk bitcode representation (suitable for fast loading by a Just-In-Time compiler)

3) as a human readable assembly language representation.



The LLVM representation aims to be light-weight and low-level while being expressive, **typed**, and extensible at the same time.

> “universal IR”
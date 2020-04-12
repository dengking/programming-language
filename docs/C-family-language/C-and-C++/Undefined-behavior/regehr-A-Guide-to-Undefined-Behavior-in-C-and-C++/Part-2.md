# [A Guide to Undefined Behavior in C and C++, Part 2](https://blog.regehr.org/archives/226)

Also see [Part 1](https://blog.regehr.org/archives/213) and [Part 3](http://blog.regehr.org/archives/232).

When tools like the bounds checking `GCC`, `Purify`, `Valgrind`, etc. first showed up, it was interesting to run a random UNIX utility under them. The output of the checker showed that these utility programs, despite working perfectly well, executed a ton of memory safety errors such as **use of uninitialized data**, **accesses beyond the ends of arrays**, etc. Just running `grep` or whatever would cause tens or hundreds of these errors to happen.

当像GCC，Purify，Valgrind等界限这样的工具首次出现时，在它们下面运行一个随机的UNIX实用程序很有意思。 检查器的输出显示这些实用程序，尽管工作得很好，但执行了大量的内存安全错误，例如使用未初始化的数据，超出数组末端的访问等。只运行grep或任何会导致数十或数百的 这些错误发生了。

What was going on? Basically, incidental properties of the C/UNIX execution environment caused these errors to (often) be benign. For example, blocks returned by `malloc()` generally contain some padding before and/or after; the padding can soak up out-of-bounds stores, as long as they aren’t too far outside the allocated area. Was it worth eliminating these bugs? Sure. First, an execution environment with different properties, such as a `malloc()` for an embedded system that happens to provide less padding, could turn **benign near-miss array writes** into **nasty heap corruption bugs**. Second, the same benign bugs could probably, under different circumstances, cause a crash or corruption error even in the same execution environment. Developers generally find these kinds of arguments to be compelling and these days most UNIX programs are relatively Valgrind-clean.

Tools for finding **integer undefined behaviors** are less well-developed than are memory-unsafety checkers. **Bad integer behaviors** in C and `C++` include **signed overflow**, **divide by zero**, **shift-past-bitwidth**, etc. These have become a more serious problem in recent years because:

- Integer flaws are a source of serious security problems
- C compilers have become considerably more aggressive in their exploitation of integer undefined behaviors to generate efficient code

Recently my student Peng Li implemented a checking tool for integer undefined behaviors. Using it, we have found that many programs contain these bugs. For example, more than half of the SPECINT2006 benchmarks execute **integer undefined behaviors** of one kind or another. In many ways the situation for integer bugs today seems similar to the situation for memory bugs around 1995. Just to be clear, integer checking tools do exist, but they do not seem to be in very widespread use and also a number of them operate on binaries, which is too late. You have to look at the source code before the compiler has had a chance to exploit — and thus eliminate — operations with undefined behavior.

The rest of this post explores a few integer undefined behaviors that we found in LLVM: a medium-sized (~800 KLOC) open source `C++` code base. Of course I’m not picking on LLVM here: it’s very high-quality code. The idea is that by looking at some problems that were lurking undetected in this well-tested code, we can hopefully learn how to avoid writing these bugs in the future.

As a random note, if we consider the LLVM code to be C++0x rather than C++98, then a large number of additional shift-related undefined behaviors appear. I’ll talk about the new shift restrictions (which are identical to those in C99) in a subsequent post here.

I’ve cleaned up the tool output slightly to improve readability.

## Integer Overflow #1

Error message:

> ```
> UNDEFINED at <BitcodeWriter.cpp, (740:29)> :
> Operator: -
> Reason: Signed Subtraction Overflow
> left (int64): 0
> right (int64): -9223372036854775808
> ```

Code:

> ```c
> int64_t V = IV->getSExtValue();
> if (V >= 0)
>   Record.push_back(V << 1);
> else
>   Record.push_back((-V << 1) | 1);  <<----- bad line
> ```


# Undefined behavior

## cppreference [Undefined behavior](https://en.cppreference.com/w/c/language/behavior)

The C language standard precisely specifies the [observable behavior](https://en.cppreference.com/mwiki/index.php?title=c/language/as_if&action=edit&redlink=1) of C language programs, except for the ones in the following categories:

1、*undefined behavior* - there are no restrictions on the **behavior** of the program. Examples of **undefined behavior** are 

- memory accesses outside of array bounds, 

- signed integer overflow, 

- null pointer dereference, 

- modification of the same scalar [more than once](https://en.cppreference.com/w/c/language/eval_order) in an expression without sequence points, 

- access to an object through a pointer of a different type, etc. 

  

  Compilers are not required to diagnose **undefined behavior** (although many simple     situations are diagnosed), and the compiled program is not required to do anything meaningful.

2、*unspecified behavior* - two or more behaviors are permitted and the implementation is not required to document the effects of each behavior. For example, [order of evaluation](https://en.cppreference.com/w/c/language/eval_order), whether identical [string literals](https://en.cppreference.com/w/c/language/string_literal) are distinct, etc. Each **unspecified behavior** results in one of a set of valid results and may produce a different result when repeated in the same program.

3、*implementation-defined behavior* - unspecified behavior where each implementation documents how the choice is made. For example, number of bits in a byte, or whether **signed integer** right shift is arithmetic or logical.

> NOTE: [GNU C Implementation-Defined Behavior](https://gcc.gnu.org/onlinedocs/gcc/C-Implementation.html#C-Implementation)

4、*locale-specific behavior* - implementation-defined behavior that depends on the [currently chosen locale](https://en.cppreference.com/w/c/locale/setlocale). For example, whether [islower](https://en.cppreference.com/w/c/string/byte/islower) returns true for any character other than the 26 lowercase Latin letters.

(Note: [Strictly conforming](https://en.cppreference.com/w/c/language/conformance) programs do not depend on any unspecified, undefined, or implementation-defined behavior)

The compilers are required to issue diagnostic messages (either errors or warnings) for any programs that violates any C syntax rule or semantic constraint, even if its **behavior** is specified as undefined or implementation-defined or if the compiler provides a language extension that allows it to accept such program. Diagnostics for undefined behavior are not otherwise required.

### UB and optimization

Because correct C programs are free of **undefined behavior**, compilers may produce unexpected results when a program that actually has UB is compiled with optimization enabled:

For example,

#### Signed overflow

```c++
int foo(int x) {
    return x+1 > x; // either true or UB due to signed overflow
}
```

may be compiled as ([demo](https://godbolt.org/z/D249FL))

```assembly
foo:
        movl    $1, %eax
        ret
```



#### Access out of bounds

```c
int table[4] = {0};
int exists_in_table(int v)
{
    // return true in one of the first 4 iterations or UB due to out-of-bounds access
    for (int i = 0; i <= 4; i++) {
        if (table[i] == v) return 1;
    }
    return 0;
}
```

May be compiled as ([demo](https://godbolt.org/z/qoD2uP))

```assembly
exists_in_table:
        movl    $1, %eax
        ret
```





### External links

- [What Every C Programmer Should Know About Undefined Behavior #1/3](http://blog.llvm.org/2011/05/what-every-c-programmer-should-know.html)
- [What Every C Programmer Should Know About Undefined Behavior #2/3](http://blog.llvm.org/2011/05/what-every-c-programmer-should-know_14.html)
- [What Every C Programmer Should Know About Undefined Behavior #3/3](http://blog.llvm.org/2011/05/what-every-c-programmer-should-know_21.html)
- [Undefined behavior can result in time travel (among other things, but time travel is the funkiest)](http://blogs.msdn.com/b/oldnewthing/archive/2014/06/27/10537746.aspx)
- [Understanding Integer Overflow in C/C++](http://www.cs.utah.edu/~regehr/papers/overflow12.pdf)
- [Undefined Behavior and Fermat’s Last Theorem](http://kukuruku.co/hub/cpp/undefined-behavior-and-fermats-last-theorem)
- [Fun with NULL pointers, part 1](http://lwn.net/Articles/342330/) (local exploit in Linux 2.6.30 caused by UB due to null pointer dereference)



## wikipedia [Undefined behavior](https://en.wikipedia.org/wiki/Undefined_behavior)

In [computer programming](https://en.wikipedia.org/wiki/Computer_programming), **undefined behavior** (**UB**) is the result of executing computer code whose [behavior](https://en.wikipedia.org/wiki/Behavior) is not prescribed(规定的) by the [language specification](https://en.wikipedia.org/wiki/Language_specification) to which the code adheres(遵循), for the current state of the program. This happens when the [translator](https://en.wikipedia.org/wiki/Translator_(computing)) of the source code makes certain assumptions, but these assumptions are not satisfied during execution.

The behavior of some programming languages—most famously [C](https://en.wikipedia.org/wiki/C_(programming_language)) and [C++](https://en.wikipedia.org/wiki/C%2B%2B)—is **undefined** in some cases. In the standards for these languages the semantics of certain operations is described as *undefined*. These cases typically represent unambiguous [bugs](https://en.wikipedia.org/wiki/Software_bugs) in the code, for example indexing an array outside of its bounds. An implementation is allowed to **assume** that such operations never occur in correct standard-conforming program code. In the case of C/C++, the compiler is allowed to give a compile-time diagnostic in these cases, but is not required to: the implementation will be considered correct whatever it does in such cases, analogous to [don't-care terms](https://en.wikipedia.org/wiki/Don't-care_term) in digital logic. It is the responsibility of the programmer to write code that never invokes **undefined behavior**, although compiler implementations are allowed to issue diagnostics when this happens. This **assumption**（和前面的**assume**相对应） can make various [program transformations](https://en.wikipedia.org/wiki/Program_transformations) valid or simplify their proof of correctness, giving flexibility to the implementation. As a result, the [compiler](https://en.wikipedia.org/wiki/Compiler) can often make more optimizations. It also allows more compile-time checks by both compilers and [static program analysis](https://en.wikipedia.org/wiki/Static_program_analysis).

> NOTE: 一些编程语言的行为 - 最着名的是C和C ++  - 在某些情况下是不确定的。[1]在这些语言的标准中，某些操作的语义被描述为未定义。这些情况通常代表代码中的明确错误，例如将数组索引到其边界之外。**允许实现假设这样的操作永远不会出现在正确的符合标准的程序代码中**。在C / C ++的情况下，允许编译器在这些情况下提供编译时诊断，但不要求：在这种情况下，实现将被认为是正确的，类似于无关紧要的术语在数字逻辑中。程序员有责任编写永远不会调用未定义行为的代码，尽管允许编译器实现在发生这种情况时发出诊断信息。这种**假设**可以使各种程序转换有效或简化其正确性证明，从而为实现提供灵活性。因此，编译器通常可以进行更多优化。它还允许编译器和静态程序分析进行更多的编译时检查。

> NOTE: 编译器一直假设这种情况永远都不会发生，但是它又不提供任何支持，实际上这是编译器将责任推给programmer。有了这种假设，编译器就可以得到很多的简化；

In the C community, undefined behavior may be humorously referred to as "**nasal demons**", after a [comp.std.c](https://en.wikipedia.org/wiki/Usenet) post that explained **undefined behavior** as allowing the compiler to do anything it chooses, even "to make demons fly out of your nose".[[2\]](https://en.wikipedia.org/wiki/Undefined_behavior#cite_note-2)Under some circumstances there can be specific restrictions on undefined behavior. For example, the [instruction set](https://en.wikipedia.org/wiki/Instruction_set) specifications of a [CPU](https://en.wikipedia.org/wiki/Central_processing_unit) might leave the behavior of some forms of an instruction undefined, but if the CPU supports [memory protection](https://en.wikipedia.org/wiki/Memory_protection) then the specification will probably include a [blanket rule](https://www.gapfillers.com/article/blanket-rule/4733) stating that no user-accessible instruction may cause a hole in the [operating system](https://en.wikipedia.org/wiki/Operating_system)'s security; so an actual CPU would be permitted to corrupt user registers in response to such an instruction, but would not be allowed to, for example, switch into [supervisor mode](https://en.wikipedia.org/wiki/Supervisor_mode).

### Benefits

Documenting an operation as **undefined behavior** allows compilers to assume that this operation will never happen in a conforming program. This gives the compiler more information about the code and this information can lead to more **optimization** opportunities.

An example for the C language:

```c
int foo(unsigned char x)
{
     int value = 2147483600; /* assuming 32 bit int */
     value += x;
     if (value < 2147483600)
        bar();
     return value;
}
```

The value of `x` cannot be negative and, given that signed [integer overflow](https://en.wikipedia.org/wiki/Integer_overflow) is undefined behavior in C, the compiler can assume that `value < 2147483600` will always be false. Thus the `if` statement, including the call to the function `bar`, can be ignored by the compiler since the test expression in the `if` has no **side effects** and its condition will never be satisfied. The code is therefore semantically equivalent to:

```c
int foo(unsigned char x)
{
     int value = 2147483600;
     value += x;
     return value;
}
```

Had the compiler been forced to assume that signed integer overflow has *wraparound* behavior, then the transformation above would not have been legal.

> NOTE: 如果编译器被迫假设有符号整数溢出具有回绕行为，那么上面的转换就不合法了。这句话的意思是指将*wraparound* behavior作为 signed integer overflow的行为，其实这就说明此时 signed integer overflow 已经不是undefined behavior，而是defined behavior了；

Such optimizations become hard to spot by humans when the code is more complex and other optimizations, like [inlining](https://en.wikipedia.org/wiki/Inlining), take place.

Another benefit from allowing signed integer overflow to be undefined is that it makes it possible to store and manipulate a variable's value in a [processor register](https://en.wikipedia.org/wiki/Processor_register) that is larger than the size of the variable in the source code. For example, if the type of a variable as specified in the source code is narrower than the native register width (such as "[int](https://en.wikipedia.org/wiki/C_data_types#Basic_types)" on a [64-bit](https://en.wikipedia.org/wiki/64-bit) machine, a common scenario), then the compiler can safely use a signed 64-bit integer for the variable in the [machine code](https://en.wikipedia.org/wiki/Machine_code) it produces, without changing the defined behavior of the code. If a program depended on the behavior of a 32-bit integer overflow, then a compiler would have to insert additional logic when compiling for a 64-bit machine, because the **overflow behavior** of most machine instructions depends on the **register width**. 

A further important benefit of undefined signed integer overflow is that it enables, though does not require, erroneous overflows to be detected at compile-time or by [static program analysis](https://en.wikipedia.org/wiki/Static_program_analysis), or by run-time checks such as the [Clang](https://en.wikipedia.org/wiki/Clang) and [GCC](https://en.wikipedia.org/wiki/GNU_Compiler_Collection) sanitizers and [valgrind](https://en.wikipedia.org/wiki/Valgrind); if such overflow were defined with semantics such as **wrap-around** then compile-time checks would not be possible.

### Risks







## summary of undefined behavior

http://www.eskimo.com/~scs/readings/undef.950311.html

### [1](https://blog.regehr.org/archives/959)

if you cast a pointer to a different kind of pointer and then dereference it, then your program has executed an [undefined behavior](http://blog.regehr.org/archives/213). 

这种情况的一个典型例子就是：[Sockets example](https://en.wikipedia.org/wiki/Type_punning)

其实上面这段话的意思和[What is the Strict Aliasing Rule and Why do we care?](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8)中所描述的是一致的：

In C and `C++` **aliasing** has to do with what **expression types** we are allowed to access stored values through. In both C and `C++` the standard specifies which **expression types** are allowed to alias which types. The compiler and optimizer are allowed to assume we follow the **aliasing rules** strictly, hence the term *strict aliasing rule*. If we attempt to access a **value** using a **type** not allowed it is classified as [undefined behavior](http://en.cppreference.com/w/cpp/language/ub)(**UB**). Once we have **undefined behavior** all bets are off, the results of our program are no longer reliable.

### 6.2.4 Storage durations of objects

The lifetime of an object is the portion of program execution during which storage is guaranteed to be reserved for it. An object exists, has a constant address, 33) and retains its last-stored value throughout its lifetime. 34) If an object is referred to outside of its lifetime, the behavior is undefined. The value of a pointer becomes indeterminate when the object it points to (or just past) reaches the end of its lifetime.



### J.2 Undefined behavior
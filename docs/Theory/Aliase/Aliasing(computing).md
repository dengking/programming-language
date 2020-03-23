# Aliasing 

“aliasing”即“别名”，其实就是多个name都指向同一个object，aliasing在programming language中广泛存在，最最典型的就是c中的pointer、c++中的reference，在python中，同一个object可以bind到多个name，这也是一种aliasing。

## 维基百科[Aliasing (computing)](https://en.wikipedia.org/wiki/Aliasing_(computing))

In [computing](https://en.wikipedia.org/wiki/Computing), **aliasing** describes a situation in which a data location in [memory](https://en.wikipedia.org/wiki/Memory_(computers)) can be accessed through different **symbolic names** in the program. Thus, modifying the data through one name implicitly modifies the values associated with all **aliased names**, which may not be expected by the programmer. As a result, **aliasing** makes it particularly difficult to understand, analyze and optimize programs. [Aliasing analysers](https://en.wikipedia.org/wiki/Alias_analysis) intend to make and compute useful information for understanding aliasing in programs.

> NOTE: 典型的情况就是：两个指向同一个memory的pointer；

### Examples

### Buffer overflow

For example, most implementations of the [C programming language](https://en.wikipedia.org/wiki/C_(programming_language)) do not perform [array bounds checking](https://en.wikipedia.org/wiki/Index_checking). One can then exploit the implementation of the programming language by the compiler and the computer architecture's [assembly language](https://en.wikipedia.org/wiki/Assembly_language) conventions, to achieve **aliasing effects** by writing outside of the array (a type of [buffer overflow](https://en.wikipedia.org/wiki/Buffer_overflow)). This invokes [undefined behaviour](https://en.wikipedia.org/wiki/Undefined_behaviour) according to the C language specification; however many implementations of C will show the **aliasing effects** described here.

If an array is created on the [stack](https://en.wikipedia.org/wiki/Call_stack), with a variable laid out in memory directly beside that [array](https://en.wikipedia.org/wiki/Array_data_structure), one could index outside the array and directly change the variable by changing the relevant array element. For example, if we have an `int` array of size 2 (for this example's sake, calling it `arr`), next to another `int` variable (call it `i`), `arr[2]` (i.e. the 3rd element) would be aliased to `i` if they are adjacent in memory.

```C
# include <stdio.h>

int main()
{
 int arr[2] = { 1, 2 };
 int i=10;

 /* Write beyond the end of arr. Undefined behaviour in standard C, will write to i in some implementations. */
 arr[2] = 20;

 printf("element 0: %d \t", arr[0]); // outputs 1
 printf("element 1: %d \t", arr[1]); // outputs 2
 printf("element 2: %d \t", arr[2]); // outputs 20, if aliasing occurred
 printf("i: %d \t\t", i); // might also output 20, not 10, because of aliasing, but the compiler might have i stored in a register and print 10
 /* arr size is still 2. */
 printf("arr size: %d \n", (sizeof(arr) / sizeof(int)));
}
```

This is possible in some implementations of C because an array is a block of contiguous memory, and array elements are merely referenced by offsets off the address of the beginning of that block multiplied by the size of a single element. Since C has no bounds checking, indexing and addressing outside of the array is possible. Note that the aforementioned aliasing behaviour is [undefined behaviour](https://en.wikipedia.org/wiki/Undefined_behavior). Some implementations may leave space between arrays and variables on the stack, for instance, to align variables to memory locations that are a multiple of the architecture's native [word size](https://en.wikipedia.org/wiki/Word_size). The C standard does not generally specify how data is to be laid out in memory. (ISO/IEC 9899:1999, section 6.2.6.1).

It is not erroneous for a compiler to omit **aliasing effects** for accesses that fall outside the bounds of an array.

编译器为超出数组范围的访问省略别名效应并不是错误的。

#### Aliased pointers

*See also:* [Pointer aliasing](https://en.wikipedia.org/wiki/Pointer_aliasing)

Another variety of aliasing can occur in any language that can refer to one location in memory with more than one name (for example, with [pointers](https://en.wikipedia.org/wiki/Pointer_(computer_programming))). See the C [example](https://en.wikipedia.org/wiki/Xor_swap_algorithm#Code_example) of the `xor` swap algorithm that is a function; it assumes the two pointers passed to it are distinct, but if they are in fact equal (or aliases of each other), the function fails. This is a common problem with functions that accept pointer arguments, and their tolerance (or the lack thereof) for aliasing must be carefully documented, particularly for functions that perform complex manipulations on memory areas passed to them.

## Specified aliasing

Controlled aliasing behaviour may be desirable in some cases (that is, aliasing behaviour that is specified, unlike that enabled by memory layout in C). It is common practice in [Fortran](https://en.wikipedia.org/wiki/Fortran). The [Perl](https://en.wikipedia.org/wiki/Perl) [programming language](https://en.wikipedia.org/wiki/Programming_language) specifies, in some constructs, aliasing behaviour, such as in `foreach` loops. This allows certain data structures to be modified directly with less code. For example,

```
my @array = (1, 2, 3);

foreach my $element (@array) {
   # Increment $element, thus automatically
   # modifying @array, since $element is ''aliased''
   # to each of @array's elements in turn.
   $element++;
}

 print "@array \n";
```

will print out "2 3 4" as a result. If one wanted to bypass aliasing effects, one could copy the contents of the index variable into another and change the copy.

***SUMMARY*** : 看了上面这段话，我发现：c++这的reference和alias非常类似

## Conflicts with optimization

[Optimizers](https://en.wikipedia.org/wiki/Optimizing_compiler) often have to make conservative assumptions about variables in the presence of pointers. For example, knowing the value of a variable (such as `x` is 5) normally allows certain optimizations (such as [constant propagation](https://en.wikipedia.org/wiki/Constant_folding#Constant_propagation)). However, the compiler cannot use this information after an assignment to another variable (for example, in C, `*y = 10`) because it could be that `*y` is an alias of `x`. This could be the case after an assignment like `y = &x`. As an effect of this assignment to `*y`, the value of x would be changed as well, so propagating the information that `x` is 5 to the statements following `*y = 10` would be potentially wrong (if `*y` is indeed an alias of `x`). However, if we have information about pointers, the constant propagation process could make a query like: can `x` be an alias of `*y`? Then, if the answer is no, `x = 5` can be propagated safely.

Another optimization impacted by aliasing is code reordering. If the compiler decides that `x` is not aliased by `*y`, then code that uses or changes the value of `x` can be moved before the assignment `*y = 10`, if this would improve [scheduling](https://en.wikipedia.org/wiki/Instruction_scheduling) or enable more [loop optimizations](https://en.wikipedia.org/wiki/Loop_optimization) to be carried out.

To enable such optimizations in a predictable manner, [the ISO standard](https://en.wikipedia.org/wiki/C_(programming_language)#ANSI_C_and_ISO_C) for the [C programming language](https://en.wikipedia.org/wiki/C_(programming_language)) (including its newer [C99](https://en.wikipedia.org/wiki/C_(programming_language)#C99) edition, see section 6.5, paragraph 7) specifies that it is illegal (with some exceptions) to access the same memory location using pointers of different types. A compiler may therefore assume that such pointers do not alias. This rule, known as the **strict aliasing rule**, sometimes allows for impressive increases in performance,[[1\]](https://en.wikipedia.org/wiki/Aliasing_(computing)#cite_note-1) but has been known to break some otherwise valid code. Several software projects intentionally violate this portion of the C99 standard. For example, [Python 2.x](https://en.wikipedia.org/wiki/CPython) did so to implement reference counting,[[2\]](https://en.wikipedia.org/wiki/Aliasing_(computing)#cite_note-2) and required changes to the basic object structs in Python 3 to enable this optimization. The [Linux kernel](https://en.wikipedia.org/wiki/Linux_kernel) does this because strict aliasing causes problems with optimization of inlined code.[[3\]](https://en.wikipedia.org/wiki/Aliasing_(computing)#cite_note-3) In such cases, when compiled with [gcc](https://en.wikipedia.org/wiki/GNU_Compiler_Collection), the option `-fno-strict-aliasing` is invoked to prevent unwanted optimizations that could yield unexpected code.



## Hardware aliasing

The term *aliasing* is also used to describe the situation where, due to either a hardware design choice or a hardware failure, one or more of the available address bits is not used in the memory selection process.[[4\]](https://en.wikipedia.org/wiki/Aliasing_(computing)#cite_note-4) This may be a design decision if there are more address bits available than are necessary to support the installed memory device(s). In a failure, one or more address bits may be shorted together, or may be forced to [ground](https://en.wikipedia.org/wiki/Ground_(electricity)) (logic 0) or the supply voltage (logic 1).

- Example

For this example, we assume a memory design with 8 locations, requiring only 3 address lines (or [bits](https://en.wikipedia.org/wiki/Bit)) since 23 = 8). Address bits (named A2 through A0) are decoded to select unique memory locations as follows, in standard [binary counter](https://en.wikipedia.org/wiki/Counter_(digital)) fashion:

|  A2  |  A1  |  A0  | Memory location |
| :--: | :--: | :--: | :-------------: |
|  0   |  0   |  0   |        0        |
|  0   |  0   |  1   |        1        |
|  0   |  1   |  0   |        2        |
|  0   |  1   |  1   |        3        |
|  1   |  0   |  0   |        4        |
|  1   |  0   |  1   |        5        |
|  1   |  1   |  0   |        6        |
|  1   |  1   |  1   |        7        |

In the table above, each of the 8 unique combinations of address bits selects a different memory location. However, if one address bit (say A2) were to be shorted to ground, the table would be modified as follows:

|  A2   |  A1  |  A0  | Memory location |
| :---: | :--: | :--: | :-------------: |
|   0   |  0   |  0   |        0        |
|   0   |  0   |  1   |        1        |
|   0   |  1   |  0   |        2        |
|   0   |  1   |  1   |        3        |
| **0** |  0   |  0   |      **0**      |
| **0** |  0   |  1   |      **1**      |
| **0** |  1   |  0   |      **2**      |
| **0** |  1   |  1   |      **3**      |

In this case, with A2 always being zero, the first four memory locations are duplicated and appear again as the second four. Memory locations 4 through 7 have become inaccessible.

If this change occurred to a different address bit, the decoding results would be different, but in general the effect would be the same: the loss of a single address bit cuts the available memory space in half, with resulting duplication (aliasing) of the remaining space.

## See also

- [Aliasing](https://en.wikipedia.org/wiki/Aliasing) for uses of the word when applied to signal processing, including computer graphics
- [Pointer alias](https://en.wikipedia.org/wiki/Pointer_alias)

## References

1.  Mike Acton (2006-06-01). ["Understanding Strict Aliasing"](http://cellperformance.beyond3d.com/articles/2006/06/understanding-strict-aliasing.html).
2. **^** Neil Schemenauer (2003-07-17). ["ANSI strict aliasing and Python"](http://mail.python.org/pipermail/python-dev/2003-July/036898.html).
3. **^** Linus Torvalds (2003-02-26). ["Re: Invalid compilation without -fno-strict-aliasing"](https://lkml.org/lkml/2003/2/26/158).
4. **^** Michael Barr (2012-07-27). ["Software Based Memory Testing"](http://www.esacademy.com/en/library/technical-articles-and-documents/miscellaneous/software-based-memory-testing.html).



## External links

- [Understanding Strict Aliasing](http://cellperformance.beyond3d.com/articles/2006/06/understanding-strict-aliasing.html) – article by Mike Acton
- [Aliasing, pointer casts and gcc 3.3](http://mail-index.netbsd.org/tech-kern/2003/08/11/0001.html) – informational article on NetBSD mailing list
- [Type-based alias analysis in C++](http://www.ddj.com/cpp/184404273;jsessionid=NV5BWY3EOHMFSQSNDLPCKH0CJUNN2JVN?_requestid=510121) – Informational article on type-based alias analysis in C++
- [Understand C/C++ Strict Aliasing](http://dbp-consulting.com/tutorials/StrictAliasing.html) – article on strict aliasing originally from the boost developer's wiki
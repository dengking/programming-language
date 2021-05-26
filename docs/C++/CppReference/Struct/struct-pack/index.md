# 关于本章

本章讨论struct alignment，其实它最终的落脚点是memory access。



## gcc [6.58.8 Structure-Packing Pragmas](https://gcc.gnu.org/onlinedocs/gcc-4.8.4/gcc/Structure-Packing-Pragmas.html)

1、`#pragma pack(`n`)` simply sets the new alignment.

2、`#pragma pack()` sets the alignment to the one that was in effect when compilation started (see also command-line option `-fpack-struct[=n]` see [Code Gen Options](https://gcc.gnu.org/onlinedocs/gcc-4.8.4/gcc/Code-Gen-Options.html#Code-Gen-Options)).

3、`#pragma pack(push[,`n`])` pushes the current alignment setting on an internal stack and then optionally sets the new alignment.

4、`#pragma pack(pop)` restores the alignment setting to the one saved at the top of the internal stack (and removes that stack entry). Note that `#pragma pack([`n`])` does not influence this internal stack; thus it is possible to have `#pragma pack(push)` followed by multiple `#pragma pack(`n`)` instances and finalized by a single `#pragma pack(pop)`.

Some targets, e.g. i386 and PowerPC, support the `ms_struct` `#pragma` which lays out a structure as the documented `__attribute__ ((ms_struct))`.



## gcc [6.35.1 Common Type Attributes](https://gcc.gnu.org/onlinedocs/gcc/Common-Type-Attributes.html)

`aligned`

`aligned (alignment)`



```C++
struct __attribute__ ((aligned (8))) S { short f[3]; };
typedef int more_aligned_int __attribute__ ((aligned (8)));
```





## TODO

songho [Data Alignment](http://www.songho.ca/misc/alignment/dataalign.html)


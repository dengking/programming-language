# delete-null-pointer-checks

"delete-null-pointer-checks" 表示要删除null pointer check，"no-delete-null-pointer-checks"表示不要删除null pointer check。

## [[llvm-dev] RFC: Implementing -fno-delete-null-pointer-checks in clang](https://lists.llvm.org/pipermail/llvm-dev/2018-April/122632.html)

Linux kernel uses this flag to keep null pointer checks from getting optimized away.

> NOTE:
>
> 上面这段话的意思是：Linux kernel使用这个flag来避免compiler将null pointer check给优化掉。

[**[clang]: Add support for "-fno-delete-null-pointer-checks"**](https://reviews.llvm.org/D47894)



gcc.gnu [3.11 Options That Control Optimization](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html)


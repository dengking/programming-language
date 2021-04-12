# Likely and unlikely

这是C++中用于进行branch prediction，关于branch prediction，参见工程hardware的`CPU\Execution-of-instruction\Speculative\Branch-predictor`章节。

## cppreference [C++ attribute: likely, unlikely (since C++20)](https://en.cppreference.com/w/cpp/language/attributes/likely)



```C++
int f(int i)
{
    if (i < 0) [[unlikely]] {
        return 0;
    }
 
    return 1;
}
```





## stackoverflow [How do the likely/unlikely macros in the Linux kernel work and what is their benefit?](https://stackoverflow.com/questions/109710/how-do-the-likely-unlikely-macros-in-the-linux-kernel-work-and-what-is-their-ben)

I've been digging through some parts of the Linux kernel, and found calls like this:

```c
if (unlikely(fd < 0))
{
    /* Do something */
}
```

or

```c
if (likely(!err))
{
    /* Do something */
}
```

I've found the definition of them:

```c
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)
```

I know that they are for optimization, but how do they work? And how much performance/size decrease can be expected from using them? And is it worth the hassle (and losing the portability probably) at least in bottleneck code (in userspace, of course).

COMMENTS: 

1 The paper [What every Programmer should know about Memory](http://people.redhat.com/drepper/cpumemory.pdf) (p. 57) contains an in-depth explanation. – [Torsten Marek](https://stackoverflow.com/users/9567/torsten-marek) [Sep 20 '08 at 23:15](https://stackoverflow.com/questions/109710/how-do-the-likely-unlikely-macros-in-the-linux-kernel-work-and-what-is-their-ben#comment51454254_109710)



### [A](https://stackoverflow.com/a/109721)

They are **hint** to the **compiler** to emit instructions that will cause **branch prediction** to favour the "likely" side of a jump instruction. This can be a big win, if the prediction is correct it means that the jump instruction is basically free and will take zero cycles. On the other hand if the prediction is wrong, then it means the processor pipeline needs to be flushed and it can cost several cycles. So long as the prediction is correct most of the time, this will tend to be good for performance.

Like all such performance optimisations you should only do it after extensive profiling to ensure the code really is in a bottleneck, and probably given the micro nature, that it is being run in a tight loop. Generally the Linux developers are pretty experienced so I would imagine they would have done that. They don't really care too much about portability as they only target gcc, and they have a very close idea of the assembly they want it to generate.

> NOTE: compiler的optimization原则



### [A](https://stackoverflow.com/a/31133787)

**Let's decompile to see what GCC 4.8 does with it**

**Without `__builtin_expect`**

```c
#include "stdio.h"
#include "time.h"

int main() {
    /* Use time to prevent it from being optimized away. */
    int i = !time(NULL);
    if (i)
        printf("%d\n", i);
    puts("a");
    return 0;
}
```

Compile and decompile with GCC 4.8.2 x86_64 Linux:

```c
gcc -c -O3 -std=gnu11 main.c
objdump -dr main.o
```

Output:

```c
0000000000000000 <main>:
   0:       48 83 ec 08             sub    $0x8,%rsp
   4:       31 ff                   xor    %edi,%edi
   6:       e8 00 00 00 00          callq  b <main+0xb>
                    7: R_X86_64_PC32        time-0x4
   b:       48 85 c0                test   %rax,%rax
   e:       75 14                   jne    24 <main+0x24>
  10:       ba 01 00 00 00          mov    $0x1,%edx
  15:       be 00 00 00 00          mov    $0x0,%esi
                    16: R_X86_64_32 .rodata.str1.1
  1a:       bf 01 00 00 00          mov    $0x1,%edi
  1f:       e8 00 00 00 00          callq  24 <main+0x24>
                    20: R_X86_64_PC32       __printf_chk-0x4
  24:       bf 00 00 00 00          mov    $0x0,%edi
                    25: R_X86_64_32 .rodata.str1.1+0x4
  29:       e8 00 00 00 00          callq  2e <main+0x2e>
                    2a: R_X86_64_PC32       puts-0x4
  2e:       31 c0                   xor    %eax,%eax
  30:       48 83 c4 08             add    $0x8,%rsp
  34:       c3                      retq
```

The instruction order in memory was unchanged: first the `printf` and then `puts` and the `retq` return.

**With `__builtin_expect`**

Now replace `if (i)` with:

```c
if (__builtin_expect(i, 0))
```

and we get:

```c
0000000000000000 <main>:
   0:       48 83 ec 08             sub    $0x8,%rsp
   4:       31 ff                   xor    %edi,%edi
   6:       e8 00 00 00 00          callq  b <main+0xb>
                    7: R_X86_64_PC32        time-0x4
   b:       48 85 c0                test   %rax,%rax
   e:       74 11                   je     21 <main+0x21>
  10:       bf 00 00 00 00          mov    $0x0,%edi
                    11: R_X86_64_32 .rodata.str1.1+0x4
  15:       e8 00 00 00 00          callq  1a <main+0x1a>
                    16: R_X86_64_PC32       puts-0x4
  1a:       31 c0                   xor    %eax,%eax
  1c:       48 83 c4 08             add    $0x8,%rsp
  20:       c3                      retq
  21:       ba 01 00 00 00          mov    $0x1,%edx
  26:       be 00 00 00 00          mov    $0x0,%esi
                    27: R_X86_64_32 .rodata.str1.1
  2b:       bf 01 00 00 00          mov    $0x1,%edi
  30:       e8 00 00 00 00          callq  35 <main+0x35>
                    31: R_X86_64_PC32       __printf_chk-0x4
  35:       eb d9                   jmp    10 <main+0x10>
```

The `printf` (compiled to `__printf_chk`) was moved to the very end of the function, after `puts` and the return to improve branch prediction as mentioned by other answers.

So it is basically the same as:

```c
int main() {
    int i = !time(NULL);
    if (i)
        goto printf;
puts:
    puts("a");
    return 0;
printf:
    printf("%d\n", i);
    goto puts;
}
```

This optimization was not done with `-O0`.

But good luck on writing an example that runs faster with `__builtin_expect` than without, [CPUs are really smart these days](https://stackoverflow.com/a/1851905/895245). My naive attempts [are here](https://github.com/cirosantilli/assembly-cheat/tree/ba3b76cd4530268d4c34e29c354d399c0d8552fc/compiler-generated/gcc/interactive).

**C++20 `[[likely]]` and `[[unlikely]]`**

C++20 has standardized those C++ built-ins: [How to use C++20's likely/unlikely attribute in if-else statement](https://stackoverflow.com/questions/51797959/how-to-use-c20s-likely-unlikely-attribute-in-if-else-statement) They will likely (a pun!) do the same thing.



## cnblogs [C/C++ 分支预测(likely unlikely)](https://www.cnblogs.com/xudong-bupt/p/7337240.html)



## TODO

stackoverflow [What is the advantage of GCC's __builtin_expect in if else statements?](https://stackoverflow.com/questions/7346929/what-is-the-advantage-of-gccs-builtin-expect-in-if-else-statements)
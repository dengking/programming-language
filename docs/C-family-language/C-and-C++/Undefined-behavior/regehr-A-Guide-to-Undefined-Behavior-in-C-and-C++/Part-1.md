# [A Guide to Undefined Behavior in C and C++, Part 1](https://blog.regehr.org/archives/213)

Also see [Part 2](http://blog.regehr.org/archives/226) and [Part 3](http://blog.regehr.org/archives/232).

Programming languages typically make a distinction between **normal program actions** and **erroneous actions**. For [Turing-complete languages](https://en.wikipedia.org/wiki/Turing_completeness) we cannot reliably decide offline whether a program has the potential to execute an error; we have to just run it and see.

In a *safe* programming language, errors are trapped(捕获) as they happen. Java, for example, is largely safe via its **exception system**. In an *unsafe* programming language, errors are not trapped. Rather, after executing an erroneous operation the program keeps going, but in a silently faulty way that may have **observable consequences** later on. [Luca Cardelli’s article on type systems](http://lucacardelli.name/Papers/TypeSystems.pdf) has a nice clear introduction to these issues. C and C++ are unsafe in a strong sense: executing an erroneous operation causes the entire program to be meaningless, as opposed to just the erroneous operation having an unpredictable result. In these languages **erroneous operations** are said to have *undefined behavior*.

***TRANSLATION*** : C和C ++在强烈意义上是不安全的：执行错误操作会导致整个程序无意义，而不仅仅是具有不可预测结果的错误操作。 在这些语言中，错误的操作被认为具有未定义的行为。

***summary***： JAVA和Python都是由interpreter来执行，因此，interpreter的设计与实现就考虑到了可能出现的各种常见错误，如数组越界，溢出等，一旦它发现这些错误，就通过exception的方式来提醒programmer。c和c++则不同，它们直接由OS来执行，OS只负责执行指令，而没有责任来判断该指令是否是正确的，所以当程序存在错误的时候，它也会继续执行下去。所以在这两门语言中，责任就落在了programmer的身上。

***SUMMARY*** : C and C++ are unsafe 的原因之一就是对个error的处理方式上；

**The C FAQ defines “undefined behavior” like this:

> Anything at all can happen; the Standard imposes no requirements. The program may fail to compile, or it may execute incorrectly (either crashing or silently generating incorrect results), or it may fortuitously do exactly what the programmer intended.

This is a good summary. Pretty much every C and C++ programmer understands that accessing a null pointer and dividing by zero are **erroneous actions**（这些在python和Java中都是exception）. On the other hand, the full implications of **undefined behavior** and its interactions with aggressive compilers are not well-appreciated. This post explores these topics.

***translation*** ： 另一方面，未定义行为及其与积极编译器的交互的全部含义并未得到充分认可。 这篇文章探讨了这些主题。

# A Model for Undefined Behavior

For now, we can ignore the existence of compilers. There is only the “C implementation” which — if the implementation conforms to the C standard — acts the same as the “**C abstract machine**” when executing a conforming program. The C abstract machine is a simple interpreter for C that is described in the C standard. We can use it to determine the meaning of any C program.

The execution of a program consists of simple steps such as adding two numbers or jumping to a label. If every step in the execution of a program has defined behavior, then the entire execution is well-defined. Note that even well-defined executions may not have a unique result due to unspecified and implementation-defined behavior; we’ll ignore both of these here.

If any step in a program’s execution has **undefined behavior**, then the entire execution is without meaning. This is important: it’s not that evaluating (`1<<32`) has an unpredictable result, but rather that the entire execution of a program that evaluates this expression is meaningless. Also, it’s not that the execution is meaningful up to the point where **undefined behavior** happens: the bad effects can actually precede the undefined operation.

As a quick example let’s take this program:

```c
#include <limits.h>
#include <stdio.h>

int main (void)
{
  printf ("%d\n", (INT_MAX+1) < 0);
  return 0;
}
```

***SUMMARY*** : 对于这种情况，python是会抛出*exception* `OverflowError`[¶](https://docs.python.org/3/library/exceptions.html#OverflowError)的，显然这也印证了上面的那段话：In a *safe* programming language, errors are trapped(捕获) as they happen. Java, for example, is largely safe via its **exception system**. 但是c和`c++`中，却允许这种情况的发生；

The program is asking the C implementation to answer a simple question: if we add one to the largest representable integer, is the result negative? This is perfectly legal behavior for a C implementation:

```
$ cc test.c -o test
$ ./test
1
```

So is this:

```
$ cc test.c -o test
$ ./test
0
```

And this:

```
$ cc test.c -o test
$ ./test
42
```

And this:

```
$ cc test.c -o test
$ ./test
Formatting root partition, chomp chomp
```

One might say: Some of these compilers are behaving improperly because the C standard says a **relational operator** must return 0 or 1. But since the program has no meaning at all, the implementation can do whatever it likes. **Undefined behavior** trumps(胜过) all other behaviors of **the C abstract machine**.

***SUMMARY*** : 上面这段话对于undefined behavior 总结非常到位

Will a real compiler emit code to chomp your disk? Of course not, but keep in mind that practically speaking, **undefined behavior** often does lead to Bad Things because many security vulnerabilities start out as memory or integer operations that have **undefined behavior**. For example, accessing an out of bounds array element is a key part of the canonical stack smashing attack. In summary: the compiler does not need to emit code to format your disk. Rather, following the OOB array access your computer will begin executing exploit code, and that code is what will format your disk.

# No Traveling

It is very common for people to say — or at least think — something like this:

> The x86 `ADD` instruction is used to implement C’s signed add operation, and it has two’s complement behavior when the result overflows. I’m developing for an x86 platform, so I should be able to **expect** two’s complement semantics when 32-bit signed integers overflow.

***SUMMARY*** : 只是主观的感觉好而已，实际上它已经违反了规定；

**THIS IS WRONG**. You are saying something like this:

> Somebody once told me that in basketball you can’t hold the ball and run. I got a basketball and tried it and it worked just fine. He obviously didn’t understand basketball.

([This explanation is due to Roger Miller via Steve Summit](http://www.eskimo.com/~scs/readings/undef.950311.html).)

Of course it is physically possible to pick up a basketball and run with it. It is also possible you will get away with it during a game.  However, it is against the rules; good players won’t do it and bad players won’t get away with it for long. Evaluating (`INT_MAX+1`) in C or C++ is exactly the same: it may work sometimes, but don’t expect to keep getting away with it. The situation is actually a bit subtle so let’s look in more detail.

First, are there C implementations that guarantee two’s complement behavior when a signed integer overflows? Of course there are. Many compilers will have this behavior when optimizations are turned off, for example, and GCC has an option (-fwrapv) for enforcing this behavior at all optimization levels. Other compilers will have this behavior at all optimization levels by default.

There are also, it should go without saying, compilers that do not have two’s complement behavior for signed overflows. Moreover, there are compilers (like GCC) where integer overflow behaved a certain way for many years and then at some point the optimizer got just a little bit smarter and integer overflows suddenly and silently stopped working as expected. This is perfectly OK as far as the standard goes. While it may be unfriendly to developers, it would be considered a win by the compiler team because it will increase benchmark scores.

In summary: There’s nothing inherently bad about running with a ball in your hands and also there’s nothing inherently bad about shifting a 32-bit number by 33 bit positions. But one is against the rules of basketball and the other is against the rules of C and C++. In both cases, the people designing the game have created arbitrary rules and we either have to play by them or else find a game we like better.

# Why Is Undefined Behavior Good?

The good thing — the only good thing! — about **undefined behavior** in C/C++ is that it simplifies the compiler’s job, making it possible to generate very efficient code in certain situations. Usually these situations involve **tight loops**. For example, high-performance array code doesn’t need to perform bounds checks, avoiding the need for tricky optimization passes to hoist these checks outside of loops. Similarly, when compiling a loop that increments a signed integer, the C compiler does not need to worry about the case where the variable overflows and becomes negative: this facilitates several loop optimizations. I’ve heard that certain tight loops speed up by 30%-50% when the compiler is permitted to take advantage of the undefined nature of signed overflow. Similarly, there have been C compilers that optionally give undefined semantics to unsigned overflow to speed up other loops.

***SUMMARY*** : 好事 - 唯一的好处！  - 关于C / C ++中未定义的行为是它简化了编译器的工作，使得在某些情况下生成非常高效的代码成为可能。 通常这些情况涉及紧密循环。 例如，高性能数组代码不需要执行边界检查，从而避免需要棘手的优化传递来在循环之外提升这些检查。 类似地，当编译一个递增有符号整数的循环时，C编译器不需要担心变量溢出并变为负数的情况：这有利于几个循环优化。 我听说当允许编译器利用已签名溢出的未定义特性时，某些紧密循环会加速30％-50％。 类似地，有一些C编译器可选择为未签名的溢出提供未定义的语义以加速其他循环。

# Why Is Undefined Behavior Bad?

When programmers cannot be trusted to reliably avoid **undefined behavior**, we end up with programs that silently misbehave. This has turned out to be a really bad problem for codes like web servers and web browsers that deal with hostile data because these programs end up being compromised and running code that arrived over the wire. In many cases, we don’t actually need the performance gained by exploitation of undefined behavior, but due to legacy code and legacy toolchains, we’re stuck with the nasty consequences.

***SUMMARY*** : 如果不能信任程序员可靠地避免未定义的行为，那么我们最终会得到无声行为的程序。 对于处理恶意数据的Web服务器和Web浏览器等代码来说，这已经成为一个非常糟糕的问题，因为这些程序最终会受到攻击并运行通过网络传输的代码。 在许多情况下，我们实际上并不需要通过利用未定义的行为获得的性能，但由于遗留代码和遗留工具链，我们陷入了令人讨厌的后果。

A less serious problem, more of an annoyance, is where behavior is undefined in cases where all it does is make the compiler writer’s job a bit easier, and no performance is gained. For example a C implementation has undefined behavior when:

> An unmatched ‘ or ” character is encountered on a logical source line during tokenization.

With all due respect to the C standard committee, this is just lazy. Would it really impose an undue burden on C implementors to require that they emit a compile-time error message when quote marks are unmatched? Even a 30 year-old (at the time C99 was standardized) systems programming language can do better than this. One suspects that the C standard body simply got used to throwing behaviors into the “undefined” bucket and got a little carried away. Actually, since the C99 standard lists 191 different kinds of undefined behavior, it’s fair to say they got a lot carried away.

***SUMMARY*** : 尽管对C标准委员会充满敬意，但这只是懒惰。 是否真的会给C实现者带来不必要的负担，要求它们在引号无法匹配时发出编译时错误消息？ 即使是30岁（在C99标准化时），系统编程语言也可以做得更好。 有人怀疑C标准机构只是习惯于将行为扔进“未定义”的桶中并且稍稍带走了。 实际上，由于C99标准列出了191种不同的未定义行为，可以说它们得到了很多让步。

# Understanding the Compiler’s View of Undefined Behavior

The key insight behind designing a programming language with undefined behavior is that the compiler is only obligated to consider cases where the behavior is defined. We’ll now explore the implications of this.

***SUMMARY*** : 这是compiler的实现者的立场，我们要从他们的立场出发来思考这个问题；

If we imagine a C program being executed by the C abstract machine, **undefined behavior** is very easy to understand: each operation performed by the program is either defined or undefined, and usually it’s pretty clear which is which. **Undefined behavior** becomes difficult to deal with when we start being concerned with all possible executions of a program. Application developers, who need code to be correct in every situation, care about this, and so do compiler developers, who need to emit **machine code** that is correct over all possible executions.

***SUMMARY*** : 考虑所有的可能性是非常难的，无论是对于普通的programmer还是对于compiler developer。

Talking about all possible executions of a program is a little tricky（棘手的）, so let’s make a few simplifying assumptions. First, we’ll discuss a single C/`C++` function instead of an entire program. Second, we’ll assume that the function terminates for every input. Third, we’ll assume the function’s execution is deterministic; for example, it’s not cooperating with other threads via shared memory. Finally, we’ll pretend that we have infinite computing resources, making it possible to exhaustively test the function. Exhaustive testing means that all possible inputs are tried, whether they come from arguments, global variables, file I/O, or whatever.

***SUMMARY*** : 谈论程序的所有可能执行都有点棘手，所以让我们做一些简化的假设。 首先，我们将讨论单个C / C ++函数而不是整个程序。 其次，我们假设函数终止于每个输入。 第三，我们假设函数的执行是确定性的; 例如，它不通过共享内存与其他线程协作。 最后，我们将假装我们拥有无限的计算资源，从而可以对功能进行详尽的测试。 穷举测试意味着尝试所有可能的输入，无论它们来自参数，全局变量，文件I / O还是其他任何东西。

The exhaustive testing algorithm is simple:

1. Compute next input, terminating if we’ve tried them all
2. Using this input, run the function in the C abstract machine, keeping track of whether any operation with undefined behavior was executed
3. Go to step 1

Enumerating all inputs is not too difficult. Starting with the smallest input (measured in bits) that the function accepts, try all possible bit patterns of that size. Then move to the next size. This process may or may not terminate but it doesn’t matter since we have infinite computing resources.

For programs that contain unspecified and implementation-defined behaviors, each input may result in several or many possible executions. This doesn’t fundamentally complicate the situation.

OK, what has our thought experiment accomplished? We now know, for our function, which of these categories it falls into:

- Type 1: Behavior is defined for all inputs
- Type 2: Behavior is defined for some inputs and undefined for others
- Type 3: Behavior is undefined for all inputs

# Type 1 Functions

These have no restrictions on their inputs: they behave well for all possible inputs (of course, “behaving well” may include returning an error code). Generally, API-level functions and functions that deal with unsanitized（未消过毒的） data should be Type 1. For example, here’s a utility function for performing integer division without executing undefined behaviors:

```c
int32_t safe_div_int32_t (int32_t a, int32_t b) {
  if ((b == 0) || ((a == INT32_MIN) && (b == -1))) {
    report_integer_math_error();
    return 0;
  } else {
    return a / b;
  }
}
```

Since Type 1 functions never execute operations with undefined behavior, the compiler is obligated to generate code that does something sensible regardless of the function’s inputs. We don’t need to consider these functions any further.

***SUMMARY*** : 有programmer来管理可能出现undefined behavior的场景；

# Type 3 Functions

These functions admit no well-defined executions. They are, strictly speaking, completely meaningless: the compiler is not even obligated to generate even a return instruction. Do Type 3 functions really exist? Yes, and in fact they are common. For example, a function that — regardless of input — uses a variable without initializing it is easy to unintentionally write. Compilers are getting smarter and smarter about recognizing and exploiting this kind of code. Here’s a great [example from the Google Native Client project](http://code.google.com/p/nativeclient/issues/detail?id=245):

这些函数不承认没有明确定义的执行。 严格来说，它们完全没有意义：编译器甚至没有义务生成返回指令。 Type 3的功能真的存在吗？ 是的，实际上它们很常见。 例如，一个函数 - 无论输入如何 - 使用变量而不初始化它很容易无意中写入。 编译器在识别和利用这种代码方面越来越聪明。 以下是Google Native Client项目的一个很好的示例：

> ```
> When returning from trusted to untrusted code, we must sanitize the return address before taking it. This ensures that untrusted code cannot use the syscall interface to vector execution to an arbitrary address. This role is entrusted to the function NaClSandboxAddr, in sel_ldr.h. Unfortunately, since r572, this function has been a no-op on x86.
> ```
>
> \```-- What happened?`
>
> ```
> During a routine refactoring, code that once read
> aligned_tramp_ret = tramp_ret & ~(nap->align_boundary - 1);
> was changed to read
> return addr & ~(uintptr_t)((1 << nap->align_boundary) - 1);
> Besides the variable renames (which were intentional and correct), a shift was introduced, treating nap->align_boundary as the log2 of bundle size.
> We didn't notice this because NaCl on x86 uses a 32-byte bundle size.  On x86 with gcc, (1 << 32) == 1. (I believe the standard leaves this behavior undefined, but I'm rusty.) Thus, the entire sandboxing sequence became a no-op.
> This change had four listed reviewers and was explicitly LGTM'd by two. Nobody appears to have noticed the change.
> -- Impact
> There is a potential for untrusted code on 32-bit x86 to unalign its instruction stream by constructing a return address and making a syscall. This could subvert the validator. A similar vulnerability may affect x86- 64.
> ARM is not affected for historical reasons: the ARM implementation masks the untrusted return address using a different method.
> ```

***SUMMARY*** : [NOP (code)](https://en.wikipedia.org/wiki/NOP_(code))

What happened? A simple refactoring put the function containing this code into Type 3. The person who sent this message believes that x86-gcc evaluates (`1<<32`) to 1, but there’s no reason to expect this behavior to be reliable (in fact it is not on a few versions of x86-gcc that I tried). This construct is definitely undefined and of course the compiler can do done anything it wants. As is typical for a C compiler, it chose to simply not emit the instructions corresponding to the undefined operation. (A C compiler’s #1 goal is to emit efficient code.) Once the Google programmers gave the compiler the license to kill, it went ahead and killed. One might ask: Wouldn’t it be great if the compiler provided a warning or something when it detected a Type 3 function? Sure! But that is not the compiler’s priority.

The Native Client example is a good one because it illustrates how competent programmers can be suckered in by an optimizing compiler’s underhanded way of exploiting undefined behavior. A compiler that is very smart at recognizing and silently destroying Type 3 functions becomes effectively evil, from the developer’s point of view.



# Type 2 Functions

These have behavior that is defined for some inputs and undefined for others. This is the most interesting case for our purposes. Signed integer divide makes a good example:

```c
int32_t unsafe_div_int32_t (int32_t a, int32_t b) {
  return a / b;
}
```

This function has a precondition; it should only be called with arguments that satisfy this predicate:

```c
(b != 0) && (!((a == INT32_MIN) && (b == -1)))
```

Of course it’s no coincidence that this predicate looks a lot like the test in the Type 1 version of this function. If you, the caller, violate this precondition, your program’s meaning will be destroyed. Is it OK to write functions like this, that have non-trivial preconditions? In general, for internal utility functions this is perfectly OK as long as the precondition is clearly documented.

Now let’s look at the compiler’s job when translating this function into object code. The compiler performs a case analysis:

- Case 1: `(b != 0) && (!((a == INT32_MIN) && (b == -1)))`
  Behavior of / operator is defined → Compiler is obligated to emit code computing a / b
- Case 2: `(b == 0) || ((a == INT32_MIN) && (b == -1))`
  Behavior of / operator is undefined → Compiler has no particular obligations

Now the compiler writers ask themselves the question: What is the most efficient implementation of these two cases? Since Case 2 incurs no obligations, the simplest thing is to simply not consider it. The compiler can emit code only for Case 1.

A Java compiler, in contrast, has obligations in Case 2 and must deal with it (though in this particular case, it is likely that there won’t be runtime overhead since processors can usually provide trapping behavior for integer divide by zero).

Let’s look at another Type 2 function:

```c
int stupid (int a) {
  return (a+1) > a;
}
```

The precondition for avoiding undefined behavior is:

```
(a != INT_MAX)
```

Here the case analysis done by an optimizing C or C++ compiler is:

- Case 1: **a != INT_MAX**
  Behavior of + is defined → Computer is obligated to return 1
- Case 2: **a == INT_MAX**
  Behavior of + is undefined → Compiler has no particular obligations

Again, Case 2 is degenerate and disappears from the compiler’s reasoning. Case 1 is all that matters. Thus, a good x86-64 compiler will emit:

```assembly
stupid:
  movl $1, %eax
  ret
```

If we use the `-fwrapv` flag to tell `GCC` that integer overflow has two’s complement behavior, we get a different case analysis:

- Case 1: **a != INT_MAX**
  Behavior is defined → Computer is obligated to return 1
- Case 2: **a == INT_MAX**
  Behavior is defined → Compiler is obligated to return 0

Here the cases cannot be collapsed and the compiler is obligated to actually perform the addition and check its result:

```assembly
stupid:
  leal 1(%rdi), %eax
  cmpl %edi, %eax
  setg %al
  movzbl %al, %eax
  ret
```

Similarly, an ahead-of-time Java compiler also has to perform the addition because Java mandates two’s complement behavior when a signed integer overflows (I’m using GCJ for x86-64):

```assembly
_ZN13HelloWorldApp6stupidEJbii:
  leal 1(%rsi), %eax
  cmpl %eax, %esi
  setl %al
  ret
```

This case-collapsing view of undefined behavior provides a powerful way to explain how compilers really work. Remember, their main goal is to give you fast code that obeys the letter of the law, so they will attempt to forget about undefined behavior as fast as possible, without telling you that this happened.

# A Fun Case Analysis

About a year ago, the Linux kernel started using a special GCC flag to tell the compiler to avoid optimizing away useless null-pointer checks. The code that caused developers to add this flag looks like this (I’ve cleaned up the example just a bit):

```c
static void __devexit agnx_pci_remove (struct pci_dev *pdev)
{
  struct ieee80211_hw *dev = pci_get_drvdata(pdev);
  struct agnx_priv *priv = dev->priv; 

  if (!dev) return;
  ... do stuff using dev ...
}
```

The idiom here is to get a pointer to a device `struct`, test it for null, and then use it. But there’s a problem! In this function, the pointer is dereferenced before the null check. This leads an optimizing compiler (for example, gcc at `-O2` or higher) to perform the following case analysis:

- Case 1: **dev == NULL**
  “dev->priv” has undefined behavior → Compiler has no particular obligations
- Case 2: **dev != NULL**
  Null pointer check won’t fail → Null pointer check is dead code and may be deleted

As we can now easily see, neither case necessitates a null pointer check. The check is removed, potentially creating an exploitable security vulnerability.

Of course the problem is the use-before-check of `pci_get_drvdata()`’s return value, and this has to be fixed by moving the use after the check. But until all such code can be inspected (manually or by a tool), it was deemed safer to just tell the compiler to be a bit conservative. The loss of efficiency due to a predictable branch like this is totally negligible. Similar code has been found in other parts of the kernel.

# Living with Undefined Behavior

In the long run, unsafe programming languages will not be used by mainstream developers, but rather reserved for situations where high performance and a low resource footprint are critical. In the meantime, dealing with **undefined behavior** is not totally straightforward and a patchwork approach seems to be best:

- Enable and heed compiler warnings, preferably using multiple compilers
- Use static analyzers (like Clang’s, `Coverity`, etc.) to get even more warnings
- Use compiler-supported dynamic checks; for example, gcc’s `-ftrapv` flag generates code to trap signed integer overflows
- Use tools like `Valgrind` to get additional dynamic checks
- When functions are “type 2” as categorized above, document their preconditions and postconditions
- Use **assertions** to verify that functions’ preconditions are postconditions actually hold
- Particularly in `C++`, use high-quality data structure libraries

﻿﻿Basically: be very careful, use good tools, and hope for the best.

Posted by[regehr](https://blog.regehr.org/archives/author/admin)[July 9, 2010](https://blog.regehr.org/archives/213)Posted in[Computer Science](https://blog.regehr.org/archives/category/cs), [Software Correctness](https://blog.regehr.org/archives/category/software-correctness)

## Post navigation

[Previous Post Previous post:
**Is There Anything Knol Could Have Done to Attract Plagiarists More Effectively?**](https://blog.regehr.org/archives/211)

[Next Post Next post:
**Ten Stupid Questions**](https://blog.regehr.org/archives/215)



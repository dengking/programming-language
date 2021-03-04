# regehr [Type Punning, Strict Aliasing, and Optimization](https://blog.regehr.org/archives/959)

One of the basic jobs of a low-level programming language like C or C++ is to make it easy to [peek and poke](https://en.wikipedia.org/wiki/PEEK_and_POKE) at **data representations**（或者叫做[Object representation](https://en.cppreference.com/w/c/language/object)）, as opposed to providing opaque high-level abstractions. Access to representations supports grungy tasks such as JIT compiling, setting up page tables, driving peripherals, and communicating with machines that use different representations.

As an example, we’ll look at taking an array of four 16-bit quantities and **viewing** it as a single 64-bit quantity. The basic observation is that this operation doesn’t require **moving** any data around, it only requires looking at some data in a different way. In `C/C++` it’s not possible to directly **type-cast** an array, but it is possible to cast between kinds of pointers, so we might write something like this:

```c
uint64_t c1 (const uint16_t *buf)  {
  return *(uint64_t *)buf;
}
```

This code is appealing because it is short and also it captures the idea that we are simply looking at data in a different way, as opposed to moving it around. And in fact, at `-O1` and higher, recent versions of `Clang` and `GCC` (on x86-64 Linux) compile `c1` into perfect **assembly**:

```assembly
c1:
        movq    (%rdi), %rax
        ret
```

Here the `movq` instruction moves 64 bits from where `%rdi`, the argument, points to, into `%rax`, which is where 64-bit return values live. On a 32-bit platform the output is also good, though not quite as pretty due to the **calling convention** and the lack of a 64-bit move instruction:

```assembly
c1:
        movl    4(%esp), %eax
        movl    4(%eax), %edx
        movl    (%eax), %eax
        ret
```

The problem with `c1` is that it violates C/C++’s “strict aliasing” rule which basically says that if you cast a pointer to a different kind of pointer and then dereference it, then your program has executed an [undefined behavior](http://blog.regehr.org/archives/213). The pernicious(险恶的) thing about **undefined behavior** is that it leads to problems only sometimes, and obviously, nothing bad happened here.

There do exist, however, simple programs where violations of **strict aliasing** lead to problems (this program is adapted from one I found on the net somewhere, I think on Stack Overflow):



```c
#include <stdio.h>
 
void check (int *h, long *k) {
  *h = 5;
  *k = 6;
  printf("%d\n", *h);
}
 
int main (void) {
  long k;
  check((int *)&k, &k);
  
  return 0;
}
```



The problem here is that `k` is **aliased** by a pointer to `long` and also a pointer to `int`. The result:

```
$ gcc -O1 cast3.c -o cast3 ; ./cast3
6
$ gcc -O2 cast3.c -o cast3 ; ./cast3
5
$ clang -O1 cast3.c -o cast3 ; ./cast3
6
$ clang -O2 cast3.c -o cast3 ; ./cast3
5
```

> NOTE: 当`h`和`k`相互alias的时候，它们应该具备相同的值；显然，上述输出中，当`-O1`的时候，输出是正确的；当`-O2`的时候，输出是错误的；至于错误的原因，在下面进行了介绍：

As you can imagine, a lot of code that previously worked broke when [**type-based alias analysis**](https://en.wikipedia.org/wiki/Alias_analysis) was turned on in GCC. A **type-based alias analysis** is one that assumes pointers to different types are not aliases. In a type-safe programming language, this is generally a solid assumption. In `C/C++`, the responsibility for avoiding problems is placed on developers, resulting in the **strict aliasing rules**. There is, by the way, a severe lack of tools which check for violations of **strict aliasing** (compilers warn about some violations, but these warnings are highly unreliable).

> NOTE: A **type-based alias analysis** is one that assumes pointers to different types are not aliases. 这句话在[Alias analysis](https://en.wikipedia.org/wiki/Alias_analysis)中也提及了；上面这一段并没有将问题的原因解释清楚，而是非常模糊的，在[What is the Strict Aliasing Rule and Why do we care?](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8)中也列举了和上述类似的例子，并且结合汇编语言进行了分析，以下是对在`-O3`开启后生成的可执行程序进行汇编的`check的汇编代码：

```assembly
0000000000400560 <check>:
check():
  400560:       c7 07 05 00 00 00       movl   $0x5,(%rdi)
  400566:       31 c0                   xor    %eax,%eax
  400568:       48 c7 06 06 00 00 00    movq   $0x6,(%rsi)
  40056f:       bf 10 06 40 00          mov    $0x400610,%edi
  400574:       be 05 00 00 00          mov    $0x5,%esi
  400579:       e9 92 fe ff ff          jmpq   400410 <printf@plt>
  40057e:       66 90                   xchg   %ax,%ax

```

> NOTE: 可以看到，该函数最终的返回值为`mov    $0x5,%esi`，compiler直接将常数返回了；至于这样做的具体原因，需要去阅读[What is the Strict Aliasing Rule and Why do we care?](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8) 。





Ok, back to the example: `c1` contains code that we must not use. The next most obvious way to change representations is to define a `union` like this:

```c
union u {
  uint16_t buf[4];
  uint64_t l;
};
```



Now we use the type punning feature of unions to read out the other representation:

```C
uint64_t c2 (const uint16_t *buf) {
  union u tp;
  tp.buf[0] = buf[0];
  tp.buf[1] = buf[1];
  tp.buf[2] = buf[2];
  tp.buf[3] = buf[3];
  return tp.l;
}
```

Unfortunately this code is also undefined by modern C/C++ dialects (or maybe just unspecified, I’m not totally sure). On the other hand, [GCC explicitly provides the intuitive semantics for this code](http://gcc.gnu.org/onlinedocs/gcc/Structures-unions-enumerations-and-bit_002dfields-implementation.html). The [Clang user’s manual](http://clang.llvm.org/docs/UsersManual.html) does not include the string “union” anywhere, but since Clang more or less emulates GCC, we might expect that it provides the same behavior. However, other compilers [such as Sun’s do not](http://blog.qt.digia.com/blog/2011/06/10/type-punning-and-strict-aliasing/) (and yes, [this compiler still exists](http://www.oracle.com/technetwork/server-storage/solarisstudio/overview/index.html)).

Also, compilers such as GCC 4.8.1 and Clang ~3.3 (both for x86-64 Linux) fail to generate good code for `c2`. Here is Clang’s output:

```C
c2: 
        movzwl  (%rdi), %ecx
        movzwl  2(%rdi), %eax
        shlq    $16, %rax
        orq     %rcx, %rax
        movzwl  4(%rdi), %ecx
        shlq    $32, %rcx
        orq     %rax, %rcx
        movzwl  6(%rdi), %eax
        shlq    $48, %rax
        orq     %rcx, %rax
        ret
```

Perhaps oddly, both GCC and Clang generate optimal object code from this variant:

```c
uint64_t c3 (const uint16_t *buf) {
  union u tp;
  memcpy (&tp, buf, 8);
  return tp.l;
}
```

Of course `c3` is still undefined/unspecified for other compilers.

Now let’s try to write some well-defined C code:

```c
uint64_t c4 (const uint16_t *buf) {
  return
      (((uint64_t)buf[0]) << (0*16))
    | (((uint64_t)buf[1]) << (1*16))
    | (((uint64_t)buf[2]) << (2*16))
    | (((uint64_t)buf[3]) << (3*16))
    ;
}
```

Finally, let’s try this:

```c
uint64_t c5 (const uint16_t *buf) {
  uint64_t num;
  memcpy(&num, buf, 8);
  return num;
}
```

Again, although we might expect that adding a function call would make the code harder to optimize, both compilers understand `memcpy` deeply enough that we get the desired object code:

```assembly
c5:
        movq    (%rdi), %rax
        ret
```

In my opinion `c5` is the easiest code to understand out of this little batch of functions because it doesn’t do the messy shifting and also it is totally, completely, obviously free of complications that might arise from the confusing rules for unions and strict aliasing. It became my preferred idiom for **type punning** a few years ago when I discovered that compilers could see through the `memcpy` and generate the right code.



## 15 replies on “Type Punning, Strict Aliasing, and Optimization”

[**Mans**says:](http://hardwarebug.org/)

[June 12, 2013 at 5:08 am](https://blog.regehr.org/archives/959#comment-10137)

C99 Techincal Corrigendum 3 clarifies that union-based type punning is valid by amending 6.5.2.3 with this footnote:

If the member used to access the contents of a union object is not the same as the member last used to store a value in the object, the appropriate part of the object representation of the value is reinterpreted as an object representation in the new type as described in 6.2.6 (a process sometimes called “type punning”). This might be a **trap representation**.



[**Eddie Kohler**says:](http://read.seas.harvard.edu/~kohler/)

[June 12, 2013 at 7:38 am](https://blog.regehr.org/archives/959#comment-10379)

If you’re willing to write GCC C, rather than standard C, `__attribute__((may_alias))` is super useful.

```c
typedef uint64_t aliasable_uint64_t attribute((may_alias));

uint64_t c6(const uint16_t *buf) {
return (aliasable_uint64_t) buf;
}

```



I prefer this to `memcpy` for RPC, for example, where you want to pun a character buffer with a large-ish message structure. But perhaps I shouldn’t; maybe GCC is now smart enough to optimize that too.



[**Joshua Cranmer**says:](http://quetzalcoatal.blogspot.com/)

[June 12, 2013 at 7:49 am](https://blog.regehr.org/archives/959#comment-10391)

The well-definedness of the union trick in c2 I have tried to track down in specifications before, since I recalled once reading that it was undefined. As far as I can make out, C99 and C11 appear to have roughly the same operative semantics:

[C99, 6.2.6.1p5]
Certain object representations need not represent a value of the object type. If the stored value of an object has such a representation and is read by an lvalue expression that does not have character type, the behavior is undefined. If such a representation is produced by a side effect that modifies all or any part of the object by an lvalue expression that
does not have character type, the behavior is undefined.41) Such a representation is called a trap representation.

C99 6.5.2.3 has a footnote which appears to reference this paragraph:
If the member used to access the contents of a union object is not the same as the member last used to store a value in the object, the appropriate part of the object representation of the value is reinterpreted as an object representation in the new type as described in 6.2.6 (a process sometimes called “type punning”). This might be a trap representation.

C99 lists this case as “unspecified behavior” in its Annex J.1, but that seems to contradict 6.2.6.1 (and since J is informative, it loses); C11 appears to correct the unspecified behavior to refer specifically to other bytes in the union.

For C++, I can’t find a definitive statement ruling either way. C++11 states that unions can have only one active member, but I suspect that’s largely for the unrestricted union support; it also states that it is not undefined to access an object via “an aggregate or union type that includes one of the aforementioned types among its elements or nonstatic data members” [an informative note somewhere would be very useful].

I don’t have C++03 or C89 on hand, but the gcc link implies that C89 makes it implementation-defined what you get if you use unions in this way.

While I’m being technical, c4 depends on the endianness of the machine to match everybody else and is the only well-defined program where the others are at best implementation-defined.

Personally, I dislike the strict aliasing rules of C/C++; it seems to me that simple pointer analyses (like Steensgard) could catch many of the optimization benefits without violating user intuition about what’s going on.



[**Jesse Ruderman**says:](http://www.squarefree.com/)

[June 12, 2013 at 7:53 am](https://blog.regehr.org/archives/959#comment-10392)

Whenever you use this trick, you should statically assert that the two types have the same size. With unions you might read uninitialized bits. With `memcpy` you can overflow the target pointer.



[**regehr**says:](http://www.cs.utah.edu/~regehr/)

[June 12, 2013 at 9:45 am](https://blog.regehr.org/archives/959#comment-10396)

Eddie, that is a good trick. I also wonder about the robustness of the `memcpy` optimizations, can you point me to some code that you’re talking about and I’ll try it out?



[**regehr**says:](http://www.cs.utah.edu/~regehr/)

[June 12, 2013 at 9:51 am](https://blog.regehr.org/archives/959#comment-10397)

Jesse, definitely. Static assert is one of my favorite parts of C11.

Joshua, I have read over the same text, and find it unfriendly. When people say that it is undefined I believe they are referring to “might be a trap representation” part. I agree about strict aliasing, it’s a pretty unfriendly thing to add to a low-level programming language, and the current lack of checkers for violations is absurd.



[**Jesse Ruderman**says:](http://www.squarefree.com/)

[June 12, 2013 at 4:27 pm](https://blog.regehr.org/archives/959#comment-10426)

I brought up the idea of using memcpy in https://bugzilla.mozilla.org/show_bug.cgi?id=860543#c12 and got some pushback. https://bugzilla.mozilla.org/show_bug.cgi?id=798179 is probably a better place, though.

If we were to use the memcpy technique in Firefox, we’d want to know:

\* Does it get optimized well even when the function containing the memcpy call is inlined? (Can we control whether certain optimizations happen “before” or “after” inlining?)

\* Do all the compilers we care about (MSVC, GCC, and Clang) optimize it well?

\* Does it make non-optimized builds significantly slower?



**pm215**says:

[June 13, 2013 at 11:35 am](https://blog.regehr.org/archives/959#comment-10444)

Another advantage of the memcpy() approach is that it will correctly handle the case where the incoming pointer isn’t as aligned as might be required for the type we want to convert it to (as can happen if doing RPC unmarshalling from a bytestream).



[**regehr**says:](http://www.cs.utah.edu/~regehr/)

[June 14, 2013 at 12:13 pm](https://blog.regehr.org/archives/959#comment-10488)

Jesse, thanks for the pointers. You’re right that there are a lot of issues and that I’m oversimplifying. Hopefully it’s not too much work to try out both alternatives in the Mozilla codebase?

pm215, good point! I never think about this since I do almost everything on architectures that support unaligned accesses.



**moo**says:

[June 15, 2013 at 12:31 pm](https://blog.regehr.org/archives/959#comment-10637)

What’s dismal is that type-punning by pointer casting is useful low-level functionality that (in the past) used to work reliably across pretty much any production compiler. But now we have to resort to horrific ugly idioms that the compiler _might_ understand how to optimize. Trying to write portable, safe code which can also do this kind of type-punning efficiently is an exercise in futility. Despite the obvious utility of being able to tell the compiler you want to do this, neither the standardization committee nor the compiler teams saw fit to provide a clean+reliable+efficient way of doing it. Having to rely on the compiler to optimize away the memcpy is kind of ridiculous (and it sucks in debug builds!)

I hate the strict-aliasing rule, because it makes it easy for code that _seems_ to work fine, to suddenly stop working a few months from now when someone changes compiler options or adds another include file or force-inlines some utility function or just sneezes too hard on a nearby compilation unit. It has just about the worst failure modes you could imagine for a language feature; spotty compiler diagnostics, code can silently be compiled in a way that doesn’t do what you think it does, but _only sometimes_ (depending on optimizer innards and what kind of code is compiled near/with it). Also, most programmers DON’T understand it correctly, and get confused about what types of pointer punning are legal (you can access the storage of any other type through a pointer to a char type, but not vice versa.. and you can’t rely on other char-sized types like uint8_t being safe if you use them in the same way…)

Anyway, for a programmer trying to maintain a legacy codebase, the strict-aliasing rule is horrible, and the sensible thing to do is use a compiler option (which all of the major compilers seem to provide, at least for now…) to disable any optimizations that take advantage of it. You lose a little bit of performance, but you gain some peace of mind.

I work on a multi-million line codebase that does contain various fishy type-punning optimizations most of which are not written in one of the “correct” styles listed above, and yet we don’t disable strict aliasing for our builds. I haven’t had any success convincing people that this is a problem. To me it seems to be only a matter of time until one of the functions with undefined behaviour (of which there are probably hundreds, after inlining) turns out to be responsible for an actual bug. But until they have an actual example of a bug caused by strict aliasing they aren’t interested in seeking out similar problems or disabling strict aliasing.



**moo**says:

[June 15, 2013 at 12:41 pm](https://blog.regehr.org/archives/959#comment-10639)

For example: how the heck do you write a high-speed decompression function that relies on unaligned access in standard C++ ? I don’t really think its possible. In practice find various non-standard ways that work on the compiler(s) they care about, and only care about strict aliasing when it causes their code to not execute the way the expected.

Example: LZ4 (which is meant for architectures with efficient unaligned integer access, mainly x86) uses the following totally-not-standard-compliant set of macros to do unaligned reads from its source data:

// http://code.google.com/p/lz4/source/browse/trunk/lz4.c
// around line 178

\#if !defined(LZ4_FORCE_UNALIGNED_ACCESS) && !defined(__GNUC__)
\# pragma pack(push, 1)
\#endif

typedef struct _U16_S { U16 v; } _PACKED U16_S;
typedef struct _U32_S { U32 v; } _PACKED U32_S;
typedef struct _U64_S { U64 v; } _PACKED U64_S;

\#if !defined(LZ4_FORCE_UNALIGNED_ACCESS) && !defined(__GNUC__)
\# pragma pack(pop)
\#endif

\#define A64(x) (((U64_S *)(x))->v)
\#define A32(x) (((U32_S *)(x))->v)
\#define A16(x) (((U16_S *)(x))->v)



[**regehr**says:](http://www.cs.utah.edu/~regehr/)

[June 15, 2013 at 12:52 pm](https://blog.regehr.org/archives/959#comment-10641)

moo, I agree with all of your points. C/C++ are caught in a bad place where they were intended to be low-level languages for writing OSes and similar code, but then seductive performance arguments and non-forward-thinking language in the standard (e.g. making things undefined when they would have better been implementation-defined) lead to all of these problems.

Regarding finding the bugs, is there a good fuzzer for your code base? That’s how I’d look for subtle problems.



[**Dan Moulding**says:](http://danm.net/)

[June 19, 2013 at 12:42 pm](https://blog.regehr.org/archives/959#comment-11171)

Excellent post, Professor Regehr. This is the best write-up of the issues surrounding type-punning and the C language’s aliasing rules that I have come across. The inescapable conclusion that results from a detailed analysis (such as yours) is that the language’s aliasing rules cause more problems than they solve. They should be changed, IMHO.

Any compiler that strictly adheres to the rules (such as GCC does when it’s in “strict aliasing rules” mode) is bound to do really stupid things with otherwise straightforward and totally reasonable code. According to the C language rationale document, the aliasing rules were originally made to avoid forcing compilers to always make worst-case aliasing assumptions. With the addition of the “restrict” keyword, we have a superior solution for this problem and the aliasing rules are no longer needed for that purpose.



[**Jakob Engblom**says:](http://jakob.engbloms.se/)

[June 24, 2013 at 8:36 am](https://blog.regehr.org/archives/959#comment-11273)

This is a particular angle on the problem, but I am always a bit dismayed by code that attempts to do endianness conversions via pointer addresses instead of via data value shift & mask in registers. My experience with devices and device drivers is that this is bound to fail at some point, and it often results in miserable code where data is stored to a local variable and then read back.

Most modern CPUs have various byte-swapping instructions in place, and simple within-the-value code can usually get such instructions emitted. Or at least that is what engineering working on this problem tells me.

For that reason, I actually like c4 a bit better than c5, since it does make the endianness of the conversion explicit. Moving code back and forth between LE and BE and 32-bit and 64-bit variants on those does create a certain paranoia 

If that thing you are accessing is a register in a device, you are probably better off using your OS macros for things like “device register read that is 16 bits wide and little-endian”. Which tend to map to the right kinds of assembly to safely read the device. Most devices tend to act up if accessed in the wrong way – they should either be separate reads to each 16-bit unit, or a single 64-bit read. Not both.
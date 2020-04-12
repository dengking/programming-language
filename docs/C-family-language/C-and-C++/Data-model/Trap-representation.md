[TOC]

# [trap representation](https://stackoverflow.com/questions/6725809/trap-representation)

1. What is a "trap representation" in C (some examples might help)? Does this apply to C++?

2. Given this code...

   ```C
   float f=3.5;
   int *pi = (int*)&f;
   ```

   ... and assuming that `sizeof(int) == sizeof(float)`, do `f` and `*pi` have the same binary representation/pattern?



## [A](https://stackoverflow.com/a/6725981)

1. A **trap representation** is a catch-all term used by C99 (IIRC not by C89) to describe bit patterns that fit into the space occupied by a type, but **trigger undefined behavior** if used as a **value** of that type. The definition is in section 6.2.6.1p5 (with tentacles into all of 6.2.6) and I'm not going to quote it here because it's long and confusing. A type for which such bit patterns exist is said to "have" **trap representations**. No type is required to have any **trap representations**, but the only type that the standard guarantees will *not* have **trap representations** is `unsigned char`(6.2.6.1p5, 6.2.6.2p1).

   ***SUMMARY*** : 有的type是have trap representation，有的type是not have trap representation的，唯一的是`unsigned char`。

   The standard gives two hypothetical examples of **trap representations**, neither of which correspond to anything that any real CPU has done for many years, so I'm not going to confuse you with them. A *good* example of a **trap representation** (also the *only* thing that qualifies as a hardware-level trap representation on any CPU you are likely to encounter) is a signaling `NaN` in a floating-point type. C99 Annex F (section 2.1) explicitly leaves the behavior of signaling `NaN`s undefined, even though IEC 60559 specifies their behavior in detail.

   It's worth mentioning that, while pointer types *are* allowed to have **trap representations**, **null pointers** are *not* **trap representations**. **Null pointers** only cause **undefined behavior** if they are dereferenced or offset; other operations on them (most importantly, comparisons and copies) are well-defined. **Trap representations** cause **undefined behavior** if you merely *read* them using the type that has the **trap representation**. (Whether *invalid* but non-null pointers are, or should be, considered **trap representations** is a subject of debate. The CPU doesn't treat them that way, but the compiler might.)

   ***SUMMARY*** :关于r *invalid* but non-null pointer是否是 **trap representations** 是一个debate；

2. The code you show has **undefined behavior**, but this is because of the pointer-aliasing rules, not because of trap representations. This is how to convert a `float` into the `int` with the same representation (assuming, as you say, `sizeof(float) == sizeof(int)`)

   ```c
   int extract_int(float f)
   {
       union { int i; float f; } u;
       u.f = f;
       return u.i;
   }
   ```

   This code has *unspecified* (not undefined) behavior in C99, which basically means the standard doesn't define *what integer value* is produced, but you do get *some* valid integer value, it's not a **trap representation**, and the compiler is not allowed to optimize on the assumption that you have not done this. (Section 6.2.6.1, para 7. My copy of C99 might include technical corrigenda — my recollection is that this *was* undefined in the original publication but was changed to unspecified in a TC.)





***COMMENTS*** : 

- 2

- indeed it's UB on C99 (see appendix J), which was probably an oversight (the wording elsewhere seems to suggest otherwise). In C1x it's no longer UB, and the wording has been made clearer. – [ninjalj](https://stackoverflow.com/users/371250/ninjalj) [Jul 17 '11 at 19:48](https://stackoverflow.com/questions/6725809/trap-representation#comment7967601_6725981)

- 

  @Zack Goz's answer to convert float to int is well defined(sizeof(int)==sizeof(float)): [stackoverflow.com/questions/4328342/…](http://stackoverflow.com/questions/4328342/float-bits-and-strict-aliasing) Ofcourse one can find the bit representation of float using an unsigned char[n]. – [Burt](https://stackoverflow.com/users/846468/burt) [Jul 18 '11 at 5:36](https://stackoverflow.com/questions/6725809/trap-representation#comment7971601_6725981)

- 

  PS: [ideone.com/zdbbr](http://ideone.com/zdbbr) (bit pattern not same). But the same code compiled on MSVC10 and codeblocks 10.05 (windows 7), netbeans 6.9.1 with Cygwin produces same bit representation for float and int. – [Burt](https://stackoverflow.com/users/846468/burt) [Jul 18 '11 at 5:43](https://stackoverflow.com/questions/6725809/trap-representation#comment7971654_6725981)

- 

  Very informative answer! I had never even heard of trap values. – [japreiss](https://stackoverflow.com/users/805659/japreiss) [Oct 11 '11 at 22:36](https://stackoverflow.com/questions/6725809/trap-representation#comment9409981_6725981)

- 2

  Defect report/TC for this issue in C99: www.open-std.org/jtc1/sc22/wg14/www/docs/dr_283.htmff – [u0b34a0f6ae](https://stackoverflow.com/users/137317/u0b34a0f6ae) [Oct 27 '11 at 21:36](https://stackoverflow.com/questions/6725809/trap-representation#comment9677733_6725981)

- 2

  IA64 has a trap representations for integers called "Not a Thing" (NaT). See [open-std.org/jtc1/sc22/wg14/www/docs/n1208.htm](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1208.htm) and [blogs.msdn.com/b/oldnewthing/archive/2004/01/19/60162.aspx](http://blogs.msdn.com/b/oldnewthing/archive/2004/01/19/60162.aspx) for more info. – [Adam Rosenfield](https://stackoverflow.com/users/9530/adam-rosenfield) [Nov 16 '12 at 20:52](https://stackoverflow.com/questions/6725809/trap-representation#comment18346846_6725981)

- 2

  If you read the defect report you cite carefully you will realize that ia64's NaT is *not* actually a C99-compliant trap representation (the DR asks for a change to make it be one, but AFAICT that never actually happened). A C99 trap representation for a type has to be a bit pattern that fits into the visible space allocated for that type; NaTs are out-of-band. This is one of the many ways in which NaTs are a bad design; the Old New Thing blog you cite illustrates another such way. – [zwol](https://stackoverflow.com/users/388520/zwol) [Nov 16 '12 at 21:08](https://stackoverflow.com/questions/6725809/trap-representation#comment18347233_6725981) 

- 

  @Zack: You're right, NaTs are tricky stuff. I've never had the (mis)fortune of having to write code that runs on ia64. – [Adam Rosenfield](https://stackoverflow.com/users/9530/adam-rosenfield) [Nov 16 '12 at 21:18](https://stackoverflow.com/questions/6725809/trap-representation#comment18347468_6725981)

- 

  I had a small hand in the GCC backend for ia64; some of the scars still hurt, eight years later ;-) – [zwol](https://stackoverflow.com/users/388520/zwol) [Nov 16 '12 at 21:34](https://stackoverflow.com/questions/6725809/trap-representation#comment18347794_6725981)

- 

  Would it be legal for an implementation to have trap representations of `unsigned char` which, when read, would not fault immediately but would become trap representations of `int` which would fault if one attempted to perform any arithmetic on them [the basic effect being that copying an area of memory containing such trap representations wouldn't cause a fault, but the destination would hold the same trap representations]? – [supercat](https://stackoverflow.com/users/363751/supercat) [Jun 30 '13 at 17:13](https://stackoverflow.com/questions/6725809/trap-representation#comment25251419_6725981)

- 1

  @supercat I think you have just described the behavior of a trap representation for `int`. `unsigned char`cannot itself have trap representations, and trap representations for any other type are specifically allowed to be read using "lvalue expressions that have character type". (C99 6.2.6.1p5) – [zwol](https://stackoverflow.com/users/388520/zwol) [Jun 30 '13 at 19:23](https://stackoverflow.com/questions/6725809/trap-representation#comment25253520_6725981)

- 

  @Zack: In what cases is reading uninitialized storage Undefined Behavior, and in what cases is uninitialized storage required to behave though it contains a sequence of `char` whose values will be indeterminate until they are read? – [supercat](https://stackoverflow.com/users/363751/supercat) [Jun 30 '13 at 20:46](https://stackoverflow.com/questions/6725809/trap-representation#comment25254915_6725981)

- 

  @supercat It is *always* undefined behavior to read uninitialized storage; for instance, the compiler is allowed to optimize upon the assumption that you never do that. If you read section 6.2.6 carefully, the only thing the standard means to let you get away with via the special rules for character types, is applying memory block operations (such as `memcpy`) to aggregates all of whose fields have been initialized, but whose *padding* has not been initialized. There are some slight variations on that theme that are also allowed, but it was that scenario they cared about. – [zwol](https://stackoverflow.com/users/388520/zwol) [Jul 1 '13 at 0:00](https://stackoverflow.com/questions/6725809/trap-representation#comment25257475_6725981) 

- 

  @Zack: That's what I would have expected, but I find the language quoted in [stackoverflow.com/a/17392467/363751](http://stackoverflow.com/a/17392467/363751) curious. That seems to suggest that taking the address of an automatic variable has some defined effect upon its behavior. I know that C requires that any "proper" operations on on `char` types must always yield non-trap representations, and a compiler is free to do anything whatsoever with Undefined Behavior, but I'm not sure what's required with things like uninitialized auto variables that are e.g. passed to `memcpy`. – [supercat](https://stackoverflow.com/users/363751/supercat) [Jul 1 '13 at 15:48](https://stackoverflow.com/questions/6725809/trap-representation#comment25278995_6725981) 

- 

  @supercat They're quoting C11; I haven't read that yet and don't know what might have changed. However, the phrase "If an object that has automatic storage duration is not initialized explicitly, its value is indeterminate." does appear in C99 (in a different place). An *indeterminate* value is defined as "either an *unspecified* value or a trap representation", and an *unspecified* value is a "valid value of the relevant type, where this International Standard imposes no requirements on which value is chosen in any instance." (C99 3.17.2, 3.17.3 respectively.) (cont'd...) – [zwol](https://stackoverflow.com/users/388520/zwol) [Jul 1 '13 at 18:57](https://stackoverflow.com/questions/6725809/trap-representation#comment25284764_6725981)

- 

  @supercat So, *by itself*, that sounds like if a particular type has no trap representations, then reading an uninitialized automatic object of that type provokes only *unspecified* behavior. In C99 I can find no language stating otherwise. It sounds like the sentence in 6.3.2.1p2 (quoted by Jens in the other thread) may have been added in C11 to override that implication. It certainly has always been the *intent* of the standard that reading an uninitialized automatic variable provokes full-fledged undefined behavior. – [zwol](https://stackoverflow.com/users/388520/zwol) [Jul 1 '13 at 19:02](https://stackoverflow.com/questions/6725809/trap-representation#comment25284940_6725981)

- 

  @supercat However, *taking the address of* an automatic variable is well-defined regardless of whether the variable has been initialized. It is, after all, perfectly valid to initialize the storage via the pointer (or pass the pointer to a function that does so). As I read the standard, though, `int a, b; memcpy(&b, &a, sizeof int)` triggers undefined behavior. – [zwol](https://stackoverflow.com/users/388520/zwol) [Jul 1 '13 at 19:07](https://stackoverflow.com/questions/6725809/trap-representation#comment25285112_6725981)

- 

  @Zack: The part of the standard I quoted makes it sound as though `unsigned char a,b,c,d; b=a; c=a; d=b^c;` would have behavior which would be defined as storing zero in `d` [`b` could be any value, but `c`must match it]. If a direct read of the variable would be legit, I'm not sure why passing it as a `memcpy` source operand wouldn't me? – [supercat](https://stackoverflow.com/users/363751/supercat) [Jul 1 '13 at 22:16](https://stackoverflow.com/questions/6725809/trap-representation#comment25290335_6725981)

- 

  @supercat The intent is and has always been that your XOR example also exhibits undefined behavior. It is unclear to me whether the text of C99 reflects that intent. It sounds like C2011 was modified to make it, at least, more obvious. – [zwol](https://stackoverflow.com/users/388520/zwol) [Jul 1 '13 at 22:59](https://stackoverflow.com/questions/6725809/trap-representation#comment25291092_6725981)

- 

  @Zack: In my xor example, I meant to have some other code take the address of the variables [e.g. `memcpy(&c,&a,1);`]. The spec makes it sound as though the act of taking the address of the variable could move the code from Undefined Behavior to unspecified value. If it can't, I'm not sure why the fact that the address isn't taken should matter unless the quoted section relates to something else (e.g. setjmp/longjmp behavior). – [supercat](https://stackoverflow.com/users/363751/supercat) [Jul 1 '13 at 23:31](https://stackoverflow.com/questions/6725809/trap-representation#comment25291549_6725981) 

- 

  @supercat Yeah I'm not sure why that's in there either. It's possible that there's some subtlety in C2011 I'm not aware of. I recommend you ask a new question specifically about this. – [zwol](https://stackoverflow.com/users/388520/zwol) [Jul 2 '13 at 0:42](https://stackoverflow.com/questions/6725809/trap-representation#comment25292457_6725981)

- 

  BTW, reading this I was just noticing another case I don't think was mentioned: NULL is not a trap representation, it's possible for pointers to have trap representations represented by other bit patterns. A simplistic compiler for the 80286, for example, might legitimately use an LES instruction to fetch a segment+offset pair any time code tries to read a pointer; that instruction would fail if the fetched segment part didn't represent a valid segment descriptor (to make "null" work, segment 0 would likely be set up as a segment with a valid descriptor whose upper bound was zero). – [supercat](https://stackoverflow.com/users/363751/supercat) [Dec 23 '14 at 18:50](https://stackoverflow.com/questions/6725809/trap-representation#comment43673087_6725981)

- 

  @supercat Is this not already covered by the second paragraph of my answer? – [zwol](https://stackoverflow.com/users/388520/zwol) [Dec 23 '14 at 19:16](https://stackoverflow.com/questions/6725809/trap-representation#comment43673765_6725981)

- 

  @Zack: I read the second sentence of that paragraph as saying that "because trap representations would cause a trap when read, null pointers aren't trap representations". I didn't see anything that suggested that trap-representations other than null could exist, unless I missed something. – [supercat](https://stackoverflow.com/users/363751/supercat) [Dec 23 '14 at 19:19](https://stackoverflow.com/questions/6725809/trap-representation#comment43673829_6725981)

- 

  @supercat I didn't explicitly say they *can* exist, but I don't believe I said anything that implies they *can't*, either? I'll fold in your example. – [zwol](https://stackoverflow.com/users/388520/zwol) [Dec 23 '14 at 23:39](https://stackoverflow.com/questions/6725809/trap-representation#comment43679748_6725981)

- 

  @Zack: It's true you didn't explicitly say that, but I think someone who read the earlier version paragraph might have inferred, wrongly, that it was intending to be a broader statement than it actually was. Your new version looks good. – [supercat](https://stackoverflow.com/users/363751/supercat) [Dec 24 '14 at 17:26](https://stackoverflow.com/questions/6725809/trap-representation#comment43699371_6725981)

- 

  @ninjalj Annex J is non-normative – [M.M](https://stackoverflow.com/users/1505939/m-m) [Jun 10 '15 at 1:44](https://stackoverflow.com/questions/6725809/trap-representation#comment49546761_6725981)

- 

  @zwol -- Can you provide a concrete example of CPU and a concrete C computation that produces a trap representation ? If you can, please update your answer, do not comment here. – [alinsoar](https://stackoverflow.com/users/1419272/alinsoar) [Jul 6 '16 at 11:29](https://stackoverflow.com/questions/6725809/trap-representation#comment63867873_6725981)

- 

  @zwol -- Also, can you say more about the reason that a char cannot have a trap representation ? – [alinsoar](https://stackoverflow.com/users/1419272/alinsoar)[Jul 6 '16 at 11:30](https://stackoverflow.com/questions/6725809/trap-representation#comment63867912_6725981) 

- 

  @alinsoar I did give a concrete example: signaling NaN. And the reason `unsigned char` can't have trap representations is that the standard specifically forbids it. I have attempted to make this clearer in the text. I do not think a code sample would add any additional clarity. – [zwol](https://stackoverflow.com/users/388520/zwol) [Jul 6 '16 at 12:39](https://stackoverflow.com/questions/6725809/trap-representation#comment63870377_6725981)

- 

  @zwol -- what is the reason the members of the Standard decided to forbit a char to generate traps ? Something misses me. Maybe because there was no exiting machine that generated traps when one reads a single byte ? I do not understand why . My intention was to read the tech manual of some processor and see details about what happens, because "trap repr" is a too general term, it is quite confusing, etc. – [alinsoar](https://stackoverflow.com/users/1419272/alinsoar) [Jul 7 '16 at 8:34](https://stackoverflow.com/questions/6725809/trap-representation#comment63902796_6725981)

- 

  @alinsoar There has to be *some* type that does not generate traps, otherwise it would be impossible to implement `memcpy`, and character types are what you use in C to access 'raw memory', so they are the obvious choice. (I think the standard may be internally contradictory when it comes to whether *signed* character types can have trap reps, but it would only be an issue on a non-twos-complement machine so it's moot.) – [zwol](https://stackoverflow.com/users/388520/zwol) [Jul 7 '16 at 14:24](https://stackoverflow.com/questions/6725809/trap-representation#comment63917327_6725981)

- 

  @alinsoar The problem with your idea of reading a tech manual of some processor is that there *are* no processors anymore that have trap reps other than IEEE signaling NaN. In this area, the C standard is bending over backward to allow for hypothetical architectures that don't exist. You're better off assuming that there is no such thing as a trap rep (again, except for signaling NaN, and I've never in my life written code that had to care). – [zwol](https://stackoverflow.com/users/388520/zwol) [Jul 7 '16 at 14:28](https://stackoverflow.com/questions/6725809/trap-representation#comment63917522_6725981)

- 

  @zwol `There has to be some type that does not generate traps, otherwise it would be impossible to implement memcpy...` --- would you like to update the answer and put this interesting idea from here to the answer ? – [alinsoar](https://stackoverflow.com/users/1419272/alinsoar) [Jul 7 '16 at 18:32](https://stackoverflow.com/questions/6725809/trap-representation#comment63926730_6725981)

- 

  @zwol: Neither gcc nor clang, last I checked, always processes `memcpy` nor `memmove` in a fashion consistent with reading and writing individual bytes of storage. Given `unsigned short a,b;`, they will effectively replace `memcpy(&a,&b,sizeof a);` with `a=b;` even though any 16-bit pattern would leave `a` holding some value 0-65535 after the `memcpy`, but processing the assignment on register-optimized variables might set `a` to a value beyond the range of `unsigned short`. – [supercat](https://stackoverflow.com/users/363751/supercat) [Dec 5 '17 at 23:32](https://stackoverflow.com/questions/6725809/trap-representation#comment82288033_6725981) 

- 

  @supercat I do not see how this relates to the OP's question or to any of the earlier discussion here. I would like to remind you that I am not interested in debating "what the C standard *ought* to say" or "how compilers *ought* to interpret the C standard" with you, or anyone, in the comment thread of any Stack Overflow question. – [zwol](https://stackoverflow.com/users/388520/zwol) [Dec 6 '17 at 13:43](https://stackoverflow.com/questions/6725809/trap-representation#comment82310325_6725981)

- 

  I believe your example does exhibit undefined behavior. 6.2.6.1p7 in both C99 and C11 states *When a value is stored in a member of an object of union type, the bytes of the object representation that do not correspond to that member but do correspond to other members take unspecified values.* This is talking about the extra bytes when assigning to a smaller member. For example, `union u { char c; int i; };` if you assign to `c` the extra bytes of `i` are unspecified but **not** the byte corresponding to `c`, so a trap representation can still exist for `i`. – [dbush](https://stackoverflow.com/users/1687119/dbush) [Jan 26 '18 at 16:41](https://stackoverflow.com/questions/6725809/trap-representation#comment83924787_6725981)

- 

  @dbush If `sizeof(int) == sizeof(float)`, which I specifically called out as a requirement of the code example, then all of the bytes of the "object" (the union) correspond to both members. – [zwol](https://stackoverflow.com/users/388520/zwol) [Jan 26 '18 at 19:51](https://stackoverflow.com/questions/6725809/trap-representation#comment83930782_6725981)







# [Anatomy of integer types in C](https://gustedt.wordpress.com/2010/09/21/anatomy-of-integer-types-in-c/)





# [N2091: Clarifying Trap Representations (Draft Defect Report or Proposal for C2x)](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2091.htm)





# [Trap representations and padding bits](https://trust-in-soft.com/trap-representations-and-padding-bits/)





# [Objects and alignment](https://en.cppreference.com/w/c/language/object)
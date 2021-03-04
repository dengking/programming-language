# Trap representation



## stackoverflow [trap representation](https://stackoverflow.com/questions/6725809/trap-representation)

1、What is a "trap representation" in C (some examples might help)? Does this apply to C++?

2、Given this code...

```C
float f=3.5;
int *pi = (int*)&f;
```

> NOTE: 
>
> 1、上述就是典型的aliasing写法

... and assuming that `sizeof(int) == sizeof(float)`, do `f` and `*pi` have the same binary representation/pattern?



### [A](https://stackoverflow.com/a/6725981)

1、A **trap representation** is a catch-all term used by C99 (IIRC not by C89) to describe **bit patterns** that fit into the space occupied by a type, but **trigger undefined behavior** if used as a **value** of that type. The definition is in section 6.2.6.1p5 (with tentacles(触手) into all of 6.2.6) and I'm not going to quote it here because it's long and confusing. A type for which such bit patterns exist is said to "have" **trap representations**. No type is required to have any **trap representations**, but the only type that the standard guarantees will *not* have **trap representations** is `unsigned char`(6.2.6.1p5, 6.2.6.2p1).

> NOTE: 
>
> 1、"bit patterns"就是object representation
>
> 2、通过上面这段话可知: trap representation是可能发生于aliasing的，如果不遵循strict aliasing，那么就可能发生trap representation？
>
> 通过下面的"2"中的内容来看，我的上面这段话中的观点是错误的
>
> 3、`unsigned char`是byte type，它是唯一能够保值"not have trap representation"的

The standard gives two hypothetical examples of **trap representations**, neither of which correspond to anything that any real CPU has done for many years, so I'm not going to confuse you with them. A *good* example of a **trap representation** (also the *only* thing that qualifies as a hardware-level trap representation on any CPU you are likely to encounter) is a signaling `NaN` in a floating-point type. C99 Annex F (section 2.1) explicitly leaves the behavior of signaling `NaN`s undefined, even though IEC 60559 specifies their behavior in detail.

It's worth mentioning that, while pointer types *are* allowed to have **trap representations**, **null pointers** are *not* **trap representations**. **Null pointers** only cause **undefined behavior** if they are dereferenced or offset; other operations on them (most importantly, comparisons and copies) are well-defined. **Trap representations** cause **undefined behavior** if you merely *read* them using the type that has the **trap representation**. (Whether *invalid* but non-null pointers are, or should be, considered **trap representations** is a subject of debate. The CPU doesn't treat them that way, but the compiler might.)



2、The code you show has **undefined behavior**, but this is because of the pointer-aliasing rules, not because of trap representations. This is how to convert a `float` into the `int` with the same representation (assuming, as you say, `sizeof(float) == sizeof(int)`)

```c
int extract_int(float f)
{
    union { int i; float f; } u;
    u.f = f;
    return u.i;
}
```

This code has *unspecified* (not undefined) behavior in C99, which basically means the standard doesn't define *what integer value* is produced, but you do get *some* valid integer value, it's not a **trap representation**, and the compiler is not allowed to optimize on the assumption that you have not done this. (Section 6.2.6.1, para 7. My copy of C99 might include technical corrigenda — my recollection is that this *was* undefined in the original publication but was changed to unspecified in a TC.)

> NOTE: 
>
> 1、通过上面的内容可知，trap representation 和 pointer-aliasing还不是一回事





## gustedt [Anatomy of integer types in C](https://gustedt.wordpress.com/2010/09/21/anatomy-of-integer-types-in-c/)

> NOTE: 
>
> 1、Anatomy 的意思是 剖析



## open-std [N2091: Clarifying Trap Representations (Draft Defect Report or Proposal for C2x)](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2091.htm)





## trust-in-soft [Trap representations and padding bits](https://trust-in-soft.com/trap-representations-and-padding-bits/)





## cppreference [Objects and alignment](https://en.cppreference.com/w/c/language/object)
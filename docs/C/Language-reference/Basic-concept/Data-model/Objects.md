# Object

## cppreference [Objects and alignment](https://en.cppreference.com/w/c/language/object)

C programs **create**, **destroy**, **access**, and **manipulate** objects.

> NOTE: 在[Basic concepts](https://en.cppreference.com/w/c/language/basic_concepts)中有这样的一段话：[Declarations](https://en.cppreference.com/w/c/language/declarations) and [expressions](https://en.cppreference.com/w/c/language/expressions) create, destroy, access, and manipulate [objects](https://en.cppreference.com/w/c/language/object). 

> NOTE: 到底哪些是object呢？比如**variable**是object（从这个角度来看，identifier更加偏向于是一个compile time概念，而variable则是一个run time概念），array是object，`malloc`出来的是object，正如下面这段话中所描述的：

An object, in C, is region of [data storage](https://en.cppreference.com/w/c/language/memory_model) in the execution environment, the contents of which can represent ***values*** (a value is the meaning of the contents of an object, when interpreted as having a specific [type](https://en.cppreference.com/w/c/language/type)).

> NOTE: 在[Memory model](https://en.cppreference.com/w/c/language/memory_model)中对data storage的解释为：The data storage (memory) available to a C program is one or more contiguous sequences of *bytes*. 其实所谓的 [data storage](https://en.cppreference.com/w/c/language/memory_model) 就是memory；

> NOTE: object是runtime概念，而identifier是compiler概念；[objects](https://en.cppreference.com/w/c/language/object)是runtime 概念，因此与object相关的概念都是runtime概念；它是c 语言的设计者对c程序运行时的描述；



Every object has

- size (can be determined with [sizeof](https://en.cppreference.com/w/c/language/sizeof))

- alignment requirement (can be determined by [alignof](https://en.cppreference.com/w/c/language/_Alignof)) (since C11)

  > NOTE: 在c中alignment requirement是由type决定的；

- [storage duration](https://en.cppreference.com/w/c/language/storage_duration) (automatic, static, allocated, thread-local)

  > NOTE : [storage duration](https://en.cppreference.com/w/c/language/storage_duration) 是一个runtime 概念，显然， [storage duration](https://en.cppreference.com/w/c/language/storage_duration) 和 lifetime密切相关；

- [lifetime](https://en.cppreference.com/w/c/language/lifetime) (equal to storage duration or temporary)

  > NOTE: lifetime即生命周期，显然这是一个动态的概念，是c设计者创走出来用以描述object的

- effective type (see below)

- **value** (which may be indeterminate)

- optionally, an [identifier](https://en.cppreference.com/w/c/language/identifier) that denotes this object

Objects are created by [declarations](https://en.cppreference.com/w/c/language/declarations), [allocation functions](https://en.cppreference.com/w/c/memory), [string literals](https://en.cppreference.com/w/c/language/string_literal), [compound literals](https://en.cppreference.com/w/c/language/compound_literal), and by non-lvalue expressions that return [structures or unions with array members](https://en.cppreference.com/w/c/language/lifetime).

> NOTE: 在c中，function不是object，显然function没有storage duration，并且function没有size，没有alignment；在[Type](https://en.cppreference.com/w/c/language/type)的Type groups章节中提及：object types: all types that aren't function types；显然我们是无法`malloc`一个function的，而其他的都是可以`malloc`出来的；function是由compiler编译生成的，并且我们在进行program的时候，所编写的大多数都是function，如`main()`等，如果允许用户来修改function，或者删除function，则显然一切都会乱了套；



> NOTE : 查看[lifetime](https://en.cppreference.com/w/c/language/lifetime) 章节可知，Objects are created by [declarations](https://en.cppreference.com/w/c/language/declarations) 和Objects are created by [allocation functions](https://en.cppreference.com/w/c/memory)两者的lifetime规则是不同的；查看下面的[Effective type](#Effective type)章节可知，这两种对象的effective type的规则也是不同的；



### Object representation

Except for [bit fields](https://en.cppreference.com/w/c/language/bit_field), objects are composed of **contiguous** sequences of one or more bytes, each consisting of [CHAR_BIT](https://en.cppreference.com/w/c/types/limits) bits, and can be copied with [memcpy](https://en.cppreference.com/w/c/string/byte/memcpy) into an object of type `unsigned char[n]`, where `n` is the **size** of the object. The contents of the resulting array are known as *object representation*.

> NOTE: 在[Memory model](https://en.cppreference.com/w/c/language/memory_model)中也提及了上面这一段中的观点；c中使用`unsigned char[n]`来表示object，在其他更加高级的语言中也都提供了类似的功能，如：python的Binary Sequence Types — [`bytes`](https://docs.python.org/3/library/stdtypes.html#bytes), [`bytearray`](https://docs.python.org/3/library/stdtypes.html#bytearray), [`memoryview`](https://docs.python.org/3/library/stdtypes.html#memoryview)[¶](https://docs.python.org/3/library/stdtypes.html#binary-sequence-types-bytes-bytearray-memoryview)

> NOTE: 这篇文章也描述了类似的问题[Data alignment: Straighten up and fly right](https://developer.ibm.com/articles/pa-dalign/)

> NOTE: 上面这一段中的`n`是否等于`sizeof`呢？是的

If two objects have the same **object representation**, they compare equal (except if they are floating-point `NaN`s). The opposite is not true: two objects that compare equal may have different **object representations** because not every bit of the **object representation** needs to participate in the **value**. Such bits may be used for padding to satisfy alignment requirement, for parity checks, to indicate trap representations, etc.

> NOTE: 上面这段话提及了object representation和value之间的关系，其实很明显，两者是关联的， 在c++的[Object](https://en.cppreference.com/w/cpp/language/object)的对应段落中，这一段的名称叫做[Object representation and value representation](https://en.cppreference.com/w/cpp/language/object)。

> NOTE : 关于padding to satisfy alignment requirement，参见[Data structure alignment](https://en.wikipedia.org/wiki/Data_structure_alignment)

> NOTE: 关于parity checks, 参见[Parity bit](https://en.wikipedia.org/wiki/Parity_bit)

> NOTE: 关于trap representation，参见[trap representation](https://stackoverflow.com/questions/6725809/trap-representation) 

If an **object representation** does not represent any value of  **the object type**, it is known as *trap representation*. Accessing a **trap representation** in any way other than reading it through an **lvalue expression** of character type is **undefined behavior**. The value of a structure or union is never a **trap representation** even if any particular member is one.

> NOTE: 关于**the object type**，在[Type](https://en.cppreference.com/w/c/language/type)的Type groups章节中有给出其定义；

For the objects of type `char`, `signed char`, and `unsigned char`, every bit of the **object representation** is required to participate in the **value representation** and each possible bit pattern represents a distinct value (no padding, trap bits, or multiple representations allowed).

When objects of [integer types](https://en.cppreference.com/w/c/language/arithmetic_types#Integer_types) (`short`, `int`, `long`, `long long`) occupy multiple bytes, the use of those bytes is implementation-defined, but the two dominant implementations are *big-endian* (POWER, Sparc, Itanium) and *little-endian* (x86, x86_64): a big-endian platform stores the most significant byte at the lowest address of the region of storage occupied by the integer, a little-endian platform stores the least significant byte at the lowest address. See [Endianness](https://en.wikipedia.org/wiki/Endianness) for detail. See also example below.

Although most implementations do not allow trap representations, padding bits, or multiple representations for integer types, there are exceptions; for example a value of an integer type on Itanium [may be a trap representation](https://blogs.msdn.microsoft.com/oldnewthing/20040119-00/?p=41003).

### Effective type

Every object has an *effective type*, which determines which [lvalue](https://en.cppreference.com/w/c/language/value_category) accesses are valid and which violate the **strict aliasing rules**.

If the object was created by a [declaration](https://en.cppreference.com/w/c/language/declarations), the **declared type** of that object is the object's *effective type*.

If the object was created by an [allocation function](https://en.cppreference.com/w/c/memory) (including [realloc](https://en.cppreference.com/w/c/memory/realloc)), it has no **declared type**. Such object acquires an **effective type** as follows:

- The first write to that object through an lvalue that has a type other than character type, at which time the type of that lvalue becomes this object's *effective type* for that write and all subsequent reads.

  

- [memcpy](https://en.cppreference.com/w/c/string/byte/memcpy) or [memmove](https://en.cppreference.com/w/c/string/byte/memmove) copy another object into that object, at which time the **effective type** of the source object (if it had one) becomes the effective type of this object for that write and all subsequent reads.

- Any other access to the object with no declared type, the effective type is the type of the lvalue used for the access.



> NOTE: 看到了effective type，我想到了在Unix中的effective user ID；

> NOTE: `c++`中并没有effective type的概念；其实主要原因在于c++中使用了`new`，而c中则是`malloc`，显然这是`c++`在type safety上的一些改善，这一点在[Type safety](https://en.wikipedia.org/wiki/Type_safety#C++)中有解释；



### Strict aliasing

Given an object with *effective type* `T1`, using an **lvalue expression** (typically, dereferencing a pointer) of a different type `T2` is **undefined behavior**, unless:

- `T2` and `T1` are [compatible types](https://en.cppreference.com/w/c/language/type#Compatible_types).

- `T2` is **cvr-qualified** version of a type that is [compatible](https://en.cppreference.com/w/c/language/type#Compatible_types) with `T1`.

  ***SUMMARY*** : 要想理解 **cvr-qualified** ，就需要首先知道[Type qualifier](https://en.wikipedia.org/wiki/Type_qualifier)；

- `T2` is a signed or unsigned version of a type that is [compatible](https://en.cppreference.com/w/c/language/type#Compatible_types) with `T1`.

- `T2` is an aggregate type or union type type that includes one of the aforementioned types among its members (including, recursively, a member of a subaggregate or contained union).

- `T2` is a character type (char, signed char, or unsigned char).

```c
int i = 7;
char* pc = (char*)(&i);
if(pc[0] == '\x7') // aliasing through char is ok
    puts("This system is little-endian");
else
    puts("This system is big-endian");
 
float* pf = (float*)(&i);
float d = *pf; // UB: float lvalue *p cannot be used to access int
```

These rules control whether a function that receives two pointers must re-read one after writing through another:

```c
// int* and double* cannot alias
void f1(int *pi, double *pd, double d)
{
  // the read from *pi can be done only once, before the loop
  for (int i = 0; i < *pi; i++) *pd++ = d;
}
```
***SUMMARY*** : 如果`pi`和`pd`相互alias，则它们指向同一memory，在代码中，`*pd++ = d` write了这个memory，`for (int i = 0; i < *pi; i++)`每次都re-read；

```c
struct S { int a, b; };
// int* and struct S* may alias because S is an aggregate type with a member of type int
void f2(int *pi, struct S *ps, struct S s)
{
  // read from *pi must take place after every write through *ps
  for (int i = 0; i < *pi; i++) *ps++ = s;
}
```

***SUMMARY*** : 上面的这些代码是function that receives two pointers 函数的典范；

Note that [restrict qualifier](https://en.cppreference.com/w/c/language/restrict) can be used to indicate that two pointers do not **alias** even if the rules above permit them to be.

Note that **type-punning** may also be performed through the inactive member of a [union](https://en.cppreference.com/w/c/language/union).



### Alignment

Every complete [object type](https://en.cppreference.com/w/c/language/types#Type_groups) has a property called *alignment requirement*, which is an integer value of type [size_t](https://en.cppreference.com/w/c/types/size_t) representing the number of bytes between successive addresses at which objects of this type can be allocated. **The valid alignment values are non-negative integral powers of two**.

The **alignment requirement** of a type can be queried with [alignof](https://en.cppreference.com/w/c/language/_Alignof). (since C11)

In order to satisfy alignment requirements of all members of a struct, **padding** may be inserted after some of its members.

```c
#include <stdio.h>
#include <stdalign.h>
 
// objects of struct S can be allocated at any address
// because both S.a and S.b can be allocated at any address
struct S {
  char a; // size: 1, alignment: 1
  char b; // size: 1, alignment: 1
}; // size: 2, alignment: 1
 
// objects of struct X must be allocated at 4-byte boundaries
// because X.n must be allocated at 4-byte boundaries
// because int's alignment requirement is (usually) 4
struct X {
  int n;  // size: 4, alignment: 4
  char c; // size: 1, alignment: 1
  // three bytes padding
}; // size: 8, alignment: 4
 
int main(void)
{
    printf("sizeof(struct S) = %zu\n", sizeof(struct S));
    printf("alignof(struct S) = %zu\n", alignof(struct S));
    printf("sizeof(struct X) = %zu\n", sizeof(struct X));
    printf("alignof(struct X) = %zu\n", alignof(struct X));
}
```

Possible output:

```c
sizeof(struct S) = 2
alignof(struct S) = 1
sizeof(struct X) = 8
alignof(struct X) = 4
```

Each object type imposes its **alignment requirement** on every object of that type. The strictest (largest) fundamental alignment of any type is the alignment of [max_align_t](https://en.cppreference.com/w/c/types/max_align_t). The weakest (smallest) alignment is the alignment of the types `char`, `signed char`, and `unsigned char`, and equals 1.

If an object's alignment is made stricter (larger) than [max_align_t](https://en.cppreference.com/w/c/types/max_align_t) using [alignas](https://en.cppreference.com/w/c/language/_Alignas), it has *extended alignment requirement*. A struct or union type whose member has extended alignment is an *over-aligned type*. It is implementation-defined if over-aligned types are supported, and their support may be different in each kind of [storage duration](https://en.cppreference.com/w/c/language/storage_duration).


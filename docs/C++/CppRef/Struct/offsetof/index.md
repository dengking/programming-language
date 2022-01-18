# `offsetof`

## wikipedia [offsetof](https://en.wikipedia.org/wiki/Offsetof)

C's **offsetof()** macro is an [ANSI C](https://en.wikipedia.org/wiki/ANSI_C) library feature found in `stddef.h`. It evaluates to the offset (in bytes) of a given member within a [struct](https://en.wikipedia.org/wiki/Struct) or [union](https://en.wikipedia.org/wiki/Union_(computer_science)) type, an expression of type `size_t`. The `offsetof`() macro takes two [parameters](https://en.wikipedia.org/wiki/Parameters), the first being a structure name, and the second being the name of a member within the structure. It cannot be described as a C prototype.[[1\]](https://en.wikipedia.org/wiki/Offsetof#cite_note-1)

### Implementation

The "traditional" implementation of the macro relied on the compiler obtaining the offset of a member by specifying a **hypothetical structure** that begins at address zero:

```c
#define offsetof(st, m) ((size_t)&(((st *)0)->m))
```

> NOTE: 关于上面这一段中的 **hypothetical structure** ，可以参见[A](https://stackoverflow.com/a/7898055)

This can be understood as taking a null pointer of type structure `st`, and then obtaining the address of member `m` within said structure. While this implementation works correctly in many compilers, it has generated some debate if this is [undefined behavior](https://en.wikipedia.org/wiki/Undefined_behavior) according to the C standard,[[2\]](https://en.wikipedia.org/wiki/Offsetof#cite_note-2) since it appears to involve a [dereference](https://en.wikipedia.org/wiki/Dereference) of a [null pointer](https://en.wikipedia.org/wiki/Null_pointer) (although, according to the standard, section 6.6 Constant Expressions, Paragraph 9, the value of the object is not accessed by the operation). It also tends to produce confusing compiler diagnostics if one of the arguments is misspelled.[*citation needed*]

An alternative is:

```c
#define offsetof(s,memb) \
    ((size_t)((char *)&((s *)0)->memb - (char *)0))
```

It may be specified this way because the standard does not specify that the internal representation of the null pointer is at address zero. Therefore the difference between the member address and the base address needs to be made. Again, since these are **constant expressions** it can be calculated at **compile time** and not necessarily at **run-time**.

Some modern compilers (such as [GCC](https://en.wikipedia.org/wiki/GNU_Compiler_Collection)) define the macro using a special form (as a language extension) instead, e.g.[[3\]](https://en.wikipedia.org/wiki/Offsetof#cite_note-3)

```c
#define offsetof(st, m) __builtin_offsetof(st, m)
```

This builtin is especially useful with C++ `class`es or `struct`s that declare a custom unary `operator &`.[[4\]](https://en.wikipedia.org/wiki/Offsetof#cite_note-4)

### Usage

It is useful when implementing **generic data structures** in C. For example, the [Linux kernel](https://en.wikipedia.org/wiki/Linux_kernel) uses `offsetof()` to implement `container_of()`, which allows something like a [mixin](https://en.wikipedia.org/wiki/Mixin) type to find the structure that contains it:[[5\]](https://en.wikipedia.org/wiki/Offsetof#cite_note-5)

```c
#define container_of(ptr, type, member) ({ \
                const typeof( ((type *)0)->member ) *__mptr = (ptr); \
                (type *)( (char *)__mptr - offsetof(type,member) );})
```

> NOTE: `__mptr`指向`member`，即它表示的是的`member`地址，将它`(type *)( (char *)__mptr - offsetof(type,member) );`表示将`__mptr`向后拨`member`的偏移量，所以就指向了container的地址了；

This macro is used to retrieve an enclosing structure from a pointer to a nested element, such as this iteration of a linked list of `my_struct` objects:

```c
struct my_struct {
    const char *name;
    struct list_node list;
};

extern struct list_node * list_next(struct list_node *);

struct list_node *current = /* ... */
while(current != NULL){
    struct my_struct *element = container_of(current, struct my_struct, list);
    printf("%s\n", element->name);
    current = list_next(&element->list);
}
```

The linux kernel implementation of `container_of` uses a GNU C extension called *statement expressions*.[[6\]](https://en.wikipedia.org/wiki/Offsetof#cite_note-6) It's possible a **statement expression** was used to ensure **type safety** and therefore eliminate potential accidental bugs. There is, however, a way to implement the same behaviour without using **statement expressions** while still ensuring **type safety**:

```c
#define container_of(ptr, type, member) ((type *)((char *)(1 ? (ptr) : &((type *)0)->member) - offsetof(type, member)))
```

At first glance, this implementation may seem more complex than necessary, and the unusual use of the conditional operator may seem out of place. A simpler implementation is possible:

```c
#define container_of(ptr, type, member) ((type *)((char *)(ptr) - offsetof(type, member)))
```

This implementation would also serve the same purpose, however, there's a fundamental omission in terms of the original linux kernel implementation. The type of `ptr` is never checked against the type of the member, this is something that the linux kernel implementation would catch.

In the aforementioned **type-checked** implementation, the check is performed by the unusual use of the **conditional operator**. The constraints of the **conditional operator** specify that if the operands to the **conditional operator** are both pointers to a type, they must both be pointers to **compatible types**. In this case, despite the fact that the value of the third operand of the conditional expression will never be used, the compiler must perform a check to ensure that `(ptr)` and `&((type *)0)->member` are both **compatible pointer types**.

### Limitations

Usage of `offsetof` is limited to [POD](https://en.wikipedia.org/wiki/Plain_old_data) types in [C++98](https://en.wikipedia.org/wiki/C%2B%2B98), and [standard-layout](https://en.wikipedia.org/wiki/C%2B%2B11#Modification_to_the_definition_of_plain_old_data) classes in more recent versions (such as [C++11](https://en.wikipedia.org/wiki/C%2B%2B11)),[[7\]](https://en.wikipedia.org/wiki/Offsetof#cite_note-7) otherwise it has an **undefined behavior**. While most compilers will generate a correct result even in cases that don't respect the standard, there are edge cases when `offsetof` will either yield an incorrect value, generate a compile-time warning or error, or outright crash the program. This is especially the case for virtual inheritance.[[8\]](https://en.wikipedia.org/wiki/Offsetof#cite_note-8) The following program will generate several warnings and print obviously suspicious results when compiled with gcc 4.7.3 on an amd64 architecture:

```c++
#include <stddef.h>
#include <stdio.h>

struct A
{
    int  a;
    virtual void dummy() {}
};

struct B: public virtual A
{
    int  b;
};

int main()
{
    printf("offsetof(A,a) : %zu\n", offsetof(A, a));
    printf("offsetof(B,b) : %zu\n", offsetof(B, b));
    return 0;
}
```

Output is:

```
offsetof(A,a) : 8
offsetof(B,b) : 8
```

## stackoverflow [How does the C offsetof macro work? [duplicate]](https://stackoverflow.com/questions/7897877/how-does-the-c-offsetof-macro-work)

> **Possible Duplicate:** 
> [Why does this C code work?](https://stackoverflow.com/questions/713963/why-does-this-c-code-work) 
> [How do you use offsetof() on a struct?](https://stackoverflow.com/questions/7180290/how-do-you-use-offsetof-on-a-struct)

I read about this offsetof macro on the Internet, but it doesn't explain what it is used for.

```c
#define offsetof(a,b) ((int)(&(((a*)(0))->b)))
```

What is it trying to do and what is the advantage of using it?



[A](https://stackoverflow.com/a/7897932)

It has no advantages and should not be used, since it invokes undefined behavior (and uses the wrong type - `int` instead of `size_t`).

The C standard defines an `offsetof` macro in `stddef.h` which actually works, for cases where you need the offset of an element in a structure, such as:

```c
#include <stddef.h>

struct foo {
    int a;
    int b;
    char *c;
};

struct struct_desc {
    const char *name;
    int type;
    size_t off;
};

static const struct struct_desc foo_desc[] = {
    { "a", INT, offsetof(struct foo, a) },
    { "b", INT, offsetof(struct foo, b) },
    { "c", CHARPTR, offsetof(struct foo, c) },
};
```

which would let you programmatically fill the fields of a `struct foo` by name, e.g. when reading a JSON file.



[A](https://stackoverflow.com/a/7898055)

R.. is correct in his answer to the second part of your question: this code is not advised when using a modern C compiler.

But to answer the first part of your question, what this is actually doing is:

```c
(
  (int)(         // 4.
    &( (         // 3.
      (a*)(0)    // 1.
     )->b )      // 2.
  )
)
```

Working from the inside out, this is ...

1. Casting the value zero to the `struct` pointer type `a*`
2. Getting the `struct` field `b` of this (illegally placed) `struct` object
3. Getting the address of this `b` field
4. Casting the address to an `int`

**Conceptually** this is placing a `struct` object at memory address zero and then finding out at what the address of a particular field is. This could allow you to figure out the offsets in memory of each field in a `struct` so you could write your own serializers and deserializers to convert `struct`s to and from byte arrays.

> NOTE:
>
> 1、这段解释是非常好的

Of course if you would actually dereference a zero pointer your program would crash, but actually everything happens in the **compiler** and no actual zero pointer is dereferenced at runtime.

In most of the original systems that C ran on the size of an `int` was 32 bits and was the same as a pointer, so this actually worked.



## stackoverflow [Why does this implementation of offsetof() work?](https://stackoverflow.com/questions/713963/why-does-this-implementation-of-offsetof-work)

In ANSI C, `offsetof` is defined as below.

```c
#define offsetof(st, m) \
    ((size_t) ( (char *)&((st *)(0))->m - (char *)0 ))
```

Why won't this throw a **segmentation fault** since we are dereferencing a NULL pointer? Or is this some sort of **compiler hack** where it sees that only address of the offset is taken out, so it statically calculates the address without actually dereferencing it? Also is this code portable?



[A](https://stackoverflow.com/a/713970)

At no point in the above code is anything dereferenced. A **dereference** occurs when the `*` or `->` is used on an address value to find referenced value. The only use of `*` above is in a type declaration for the purpose of casting.

The `->` operator is used above but it's not used to access the value. Instead it's used to **grab** the address of the value. Here is a non-macro code sample that should make it a bit clearer

```c
SomeType *pSomeType = GetTheValue();
int* pMember = &(pSomeType->SomeIntMember);
```

The second line does not actually cause a dereference (implementation dependent). It simply returns the address of `SomeIntMember` within the `pSomeType` value.

What you see is a lot of casting between arbitrary types and char pointers. The reason for char is that it's one of the only type (perhaps the only) type in the C89 standard which has an explicit size. The size is 1. By ensuring the size is one, the above code can do the evil magic of calculating the true offset of the value.



## stackoverflow [How do you use offsetof() on a struct?](https://stackoverflow.com/questions/7180290/how-do-you-use-offsetof-on-a-struct)



[A](https://stackoverflow.com/a/7181027)

A great article to read on this is:

[Learn a new trick with the offsetof() macro](http://www.embedded.com/design/prototyping-and-development/4024941/Learn-a-new-trick-with-the-offsetof--macro)

I use the `offsetof` macro frequently in my embedded code, together with the modified SIZEOF macro as discussed in the article.







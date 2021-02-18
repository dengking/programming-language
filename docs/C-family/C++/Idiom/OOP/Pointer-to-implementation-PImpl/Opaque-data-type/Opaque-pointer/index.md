# Opaque pointer

1、使用C/C++ programming language的术语来说明，"Opaque pointer"是"incomplete type"。

## wikipedia [Opaque pointer](https://en.wikipedia.org/wiki/Opaque_pointer)

In [computer programming](https://en.wikipedia.org/wiki/Computer_programming), an **opaque pointer** is a special case of an [opaque data type](https://en.wikipedia.org/wiki/Opaque_data_type), a [data type](https://en.wikipedia.org/wiki/Data_type) declared to be a [pointer](https://en.wikipedia.org/wiki/Pointer_(computer_science)) to a [record](https://en.wikipedia.org/wiki/Record_(computer_science)) or [data structure](https://en.wikipedia.org/wiki/Data_structure) of some unspecified type.

Opaque pointers are present in several [programming languages](https://en.wikipedia.org/wiki/Programming_language) including [Ada](https://en.wikipedia.org/wiki/Ada_(programming_language)), [C](https://en.wikipedia.org/wiki/C_(programming_language)), [C++](https://en.wikipedia.org/wiki/C%2B%2B), [D](https://en.wikipedia.org/wiki/D_(programming_language)) and [Modula-2](https://en.wikipedia.org/wiki/Modula-2).

> NOTE: 
>
> 1、使用C/C++ programming language的术语来说明，"Opaque pointer"是"incomplete type"。

If the language is [strongly typed](https://en.wikipedia.org/wiki/Strong_typing), [programs](https://en.wikipedia.org/wiki/Computer_program) and [procedures](https://en.wikipedia.org/wiki/Subroutine) that have no other information about an opaque pointer type *T* can still declare [variables](https://en.wikipedia.org/wiki/Variable_(programming)), [arrays](https://en.wikipedia.org/wiki/Array_data_structure), and record fields of type *T*, assign values of that type, and compare those values for equality. However, they will not be able to [de-reference](https://en.wikipedia.org/wiki/Reference_(computer_science)) such a pointer, and can only change the object's content by calling some procedure that has the missing information.

> NOTE: 
>
> 1、结合C/C++的内容来看，上面这段话是容易理解的

Opaque pointers are a way to hide the [implementation](https://en.wikipedia.org/wiki/Implementation_(computing)) details of an [interface](https://en.wikipedia.org/wiki/Interface_(computer_science)) from ordinary clients, so that the [implementation](https://en.wikipedia.org/wiki/Implementation_(computing)) may be changed without the need to recompile the [modules](https://en.wikipedia.org/wiki/Module_(programming)) using it. This benefits the programmer as well since a simple interface can be created, and most details can be hidden in another file.[[1\]](https://en.wikipedia.org/wiki/Opaque_pointer#cite_note-1) This is important for providing [binary code compatibility](https://en.wikipedia.org/wiki/Binary_code_compatibility) through different versions of a [shared library](https://en.wikipedia.org/wiki/Shared_library), for example.

> NOTE: 
>
> 1、information hiding
>
> 2、

This technique is described in *[Design Patterns](https://en.wikipedia.org/wiki/Design_Patterns)* as the [Bridge pattern](https://en.wikipedia.org/wiki/Bridge_pattern). It is sometimes referred to as "**[handle](https://en.wikipedia.org/wiki/Handle_(computing)) classes**",[[2\]](https://en.wikipedia.org/wiki/Opaque_pointer#cite_note-eckel20000-2) the "**Pimpl idiom**" (for "pointer to implementation idiom"),[[3\]](https://en.wikipedia.org/wiki/Opaque_pointer#cite_note-3) "**Compiler firewall idiom**",[[4\]](https://en.wikipedia.org/wiki/Opaque_pointer#cite_note-4) "**d-pointer"** or "**Cheshire Cat**", especially among the C++ community.[[2\]](https://en.wikipedia.org/wiki/Opaque_pointer#cite_note-eckel20000-2)

### Examples

#### C

`obj.h`

```C++
/* obj.h */
#include <stddef.h>// size_t

struct obj;

/*
 * The compiler considers struct obj an incomplete type. Incomplete types
 * can be used in declarations.
 */
struct obj* alloc_obj();

size_t obj_size(void);

void obj_setid(struct obj *, int);

int obj_getid(struct obj *);

void free_obj(struct obj *o);
```



`obj.c`

```c++
/* obj.c */

#include "obj.h"
#include <stdlib.h> // malloc
struct obj {
    int id;
};

struct obj* alloc_obj()
{
    return malloc(sizeof(struct obj));
}

void free_obj(struct obj *o)
{
    free(o);
}
/*
 * The caller will handle allocation.
 * Provide the required information only
 */

size_t obj_size(void) {
    return sizeof(struct obj);
}

void obj_setid(struct obj *o, int i) {
    o->id = i;
}

int obj_getid(struct obj *o) {
    return o->id;
}
```

`main.c`

```C++
#include "obj.h"
#include <stdio.h>
int main()
{
    struct obj* o = alloc_obj();
    obj_setid(o, 1);
    printf("size = %i,  id = %i\n", obj_size(), obj_getid(o));
    free_obj(o);
        
}

// gcc -g obj.c main.c -o test
```



This example demonstrates a way to achieve the [information hiding](https://en.wikipedia.org/wiki/Information_hiding) ([encapsulation](https://en.wikipedia.org/wiki/Encapsulation_(computer_science))) aspect of [object-oriented programming](https://en.wikipedia.org/wiki/Object-Oriented_Programming) using the C language. If someone wanted to change the definition of `struct obj`, it would be unnecessary to recompile any other modules in the program that use the `obj.h` header file unless the [API](https://en.wikipedia.org/wiki/API) was also changed. Note that it may be desirable for the functions to check that the passed pointer is not `NULL`, but such checks have been omitted above for brevity.
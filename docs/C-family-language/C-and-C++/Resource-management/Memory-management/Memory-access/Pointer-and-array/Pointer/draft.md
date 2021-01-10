

## pointer and its type

### [c-pointers-to-any-type](https://stackoverflow.com/questions/2935955/c-pointers-to-any-type)

Yes, you can use a `void*` to point to anything, and then cast it to the proper type when needed (that's how `malloc` and such can work).

`void*` is basically "pointer to an arbitrary block of memory".

### stackoverflow [Any type of pointer can point to anything?](https://stackoverflow.com/questions/19710601/any-type-of-pointer-can-point-to-anything)

Pointers *may* be interchangeable(通用的), but are not required to be(指针可以互换，但不是必需如此).

In particular, on some platforms, certain types need to be aligned to certain byte-boundaries. So while a `char` may be anywhere in memory, an `int` may need to be on a 4-byte boundary.

Another important potential difference is with function-pointers.
Pointers to functions may not be interchangeable with pointers to data-types on many platforms.

It bears repeating: **This is platform-specific**.

I believe Intel x86 architectures treat all pointers the same.
But you may well encounter other platforms where this is not true.

I strongly doubt that on any given platform, casting a pointer from one type to another will modify the **address value**. I understand that dereferencing an unaligned pointer may cause an **exception**, but I doubt the compiler would align the pointer when the cast is done. I can't see any benefits or any motive for this behavior. – [Spidey](https://stackoverflow.com/users/131326/spidey)

Every pointer is of some specific **type**. There's a special generic pointer type `void*` that can point to anything, but you have to convert a `void*` to some specific pointer type before you can **dereference** it.

You can convert a pointer value from one pointer type to another. In most cases, converting a pointer from `foo*` to `bar*` and back to `foo*` will yield the original value -- but that's not actually guaranteed in all cases.

You *can* cause a pointer of type `foo*` to point to an object of type `bar`, but (a) it's usually a bad idea, and (b) in some cases, it may not work (say, if the target types `foo` and `bar` have different sizes or **alignment requirements**).

You can get away with things like:

```cpp
int n = 42;
char *p = (char*)&n;
```

which causes `p` to point to `n` -- but then `*p` doesn't give you the value of `n`, it gives you the value of the first byte of `n` as a `char`.

The differing behavior of **pointer arithmetic** is only part of the reason for having different **pointer types**. It's mostly about *type safety*. If you have a pointer of type `int*`, you can be reasonably sure (unless you've done something unsafe) that it actually points to an `int` object. And if you try to treat it as an object of a different type, the compiler will likely complain about it.

Basically, we have distinct pointer types for the same reasons we have other distinct types: so we can keep track of what kind of value is stored in each object, with help from the compiler.

(There have been languages that only have untyped generic pointers. In such a language, it's more difficult to avoid type errors, such as storing a value of one type and accidentally accessing it as if it were of another type.)
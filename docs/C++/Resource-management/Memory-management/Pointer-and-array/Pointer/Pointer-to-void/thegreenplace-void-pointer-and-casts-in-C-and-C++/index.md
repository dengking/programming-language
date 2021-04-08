# thegreenplace [void* and casts, in C and C++](https://eli.thegreenplace.net/2009/11/16/void-and-casts-in-c-and-c)

One of the canonical examples of C code that won't compile as C++ is this idiom for dynamically allocating memory:

```C++
int* p = malloc(sizeof(*p));
```

While it compiles in C cleanly, trying to run it through a C++ compiler [[1\]](https://eli.thegreenplace.net/2009/11/16/void-and-casts-in-c-and-c#id3) will result in an error:

```c++
error: invalid conversion from void* to int*
```

The reason for this is simple: `malloc` returns `void*` and not `int*`. While in C it's legal to assign `void*` to `int*` without a cast, in C++ it isn't.

Why the difference?

Well, let us start with C. The official "bible" of C, *"The C Programming Language, 2nd edition"* by Kernighan and Ritchie states in section A.6.8:

> Any pointer to an object may be converted to type `void*` without loss of information. If the result is converted back to the original pointer type, the original pointer is recovered. Unlike the pointer-to-pointer conversions discussed in Par.A.6.6, which generally require an explicit cast, pointers may be assigned to and from pointers of type `void*`, and may be compared with them.

Note the *to and from* part of the above quote. We saw a conversion *from* a void pointer above. A conversion *to* a void pointer happens in the following code:

```c++
void foo(void* vptr)
{
}


int main()
{
    int* p = ... /* some initialization */;
    foo(p);
    return 0;
}
```

Note that `foo` expects a void pointer, but we pass it `int*`. This idiom is employed heavily by the C standard library functions. `memcpy`, `qsort` and many others.

The C99 standard says in 6.3.2.3:

> A pointer to void may be converted to or from a pointer to any incomplete or object type. A pointer to any incomplete or object type may be converted to a pointer to void and back again; the result shall compare equal to the original pointer.

Again, it's obvious that conversions to both directions are allowed.

Now, let us look at C++.

The C++ standard isn't very conclusive(确实的；确定性的) on this topic, IMHO. All I found on the subject is this line:

> An rvalue of type “pointer to cv T,” where T is an object type, can be converted to an rvalue of type “pointer to cv void.”

It basically means: an rvalue `T*` can be converted to an rvalue `void*`. But the conversion at lvalue isn't specified, so one has to assume it's forbidden. In other words, you can convert pointers to `void*` but not the other way around.

So the `foo` call as depicted above is valid C++ [[2\]](https://eli.thegreenplace.net/2009/11/16/void-and-casts-in-c-and-c#id4). But returning from `malloc` without case isn't.

To get some more "formal information" on the subject, I turned to *"The C++ programming language, 3rd edition"* by Stroustrup. Section 5.6 is named *Pointers to Void* and there Stroustrup writes:

> A pointer to any type of object can be assigned to a variable of type `void*`, a `void*` can be assigned to another `void*`, `void*` can be compared for equality and inequality, and a `void*` can be explicitly converted to another type. Other operations would be unsafe because the compiler cannot know what kind of object is really pointed to. Consequently, other operations result in compile-time errors.

Couldn't have said it any clearer. The idiomatic way of casting the returned `void*` in C++ is:

```C++
int* p = static_cast<int*>(malloc(sizeof(*p)));
```

Curiously, Stroustrup follows this snippet with the remark:

> [...] This form of explicit type conversion is inherently unsafe and ugly. Consequently, the notation used, `static_cast`, was designed to be ugly.

Naturally, you shouldn't use `malloc` in C++ anyway. The code above is much better written as:

```c++
int* p = new int;
```

Stroustrup says at the end of section 5.6:

> [...] Functions using `void*` pointers typically exist at the very lowest level of the system, where real hardware resources are manipulated. Occurrences of `void*` at higher levels of the system should be viewed with suspicion because they are likely indicators of design errors.

![img](https://eli.thegreenplace.net/images/hline.jpg)

| [[1\]](https://eli.thegreenplace.net/2009/11/16/void-and-casts-in-c-and-c#id1) | All compilations for this article were done with MinGW's gcc and g++ with these flags: `-Wall -ansi -pedantic` |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
|                                                              |                                                              |

| [[2\]](https://eli.thegreenplace.net/2009/11/16/void-and-casts-in-c-and-c#id2) | And indeed, the C library functions `memcpy` and `qsort` can be used just the same way in C++, without additional casts. |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
|                                                              |                                                              |
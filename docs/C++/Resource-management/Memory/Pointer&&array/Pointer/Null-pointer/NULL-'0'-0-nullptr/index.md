# NULL, '\0', 0 and `nullptr`

## stackoverflow [What is the difference between NULL, '\0' and 0?](https://stackoverflow.com/questions/1296843/what-is-the-difference-between-null-0-and-0)

In C, there appear to be differences between various values of zero -- `NULL`, `NUL` and `0`.

I know that the ASCII character `'0'` evaluates to `48` or `0x30`.

The `NULL` pointer is usually defined as:

```c
#define NULL 0
```

Or

```c
#define NULL (void *)0
```

In addition, there is the `NUL` character `'\0'` which seems to evaluate to `0` as well.

Are there times when these three values can not be equal?

Is this also true on 64 bit systems?

### [A](https://stackoverflow.com/a/1296865)

**Note:** This answer applies to the C language, not C++.

------

#### Null Pointers

The integer constant literal `0` has different meanings depending upon the context in which it's used. In all cases, it is still an integer constant with the value `0`, it is just described in different ways.

If a pointer is being compared to the constant literal `0`, then this is a check to see if the pointer is a null pointer. This `0` is then referred to as a null pointer constant. The C standard defines that `0` cast to the type `void *` is both a null pointer and a null pointer constant.

Additionally, to help readability, the macro `NULL` is provided in the header file `stddef.h`. Depending upon your compiler it might be possible to `#undef NULL` and redefine it to something wacky.

Therefore, here are some valid ways to check for a null pointer:

```c
if (pointer == NULL)
```

`NULL` is defined to compare equal to a null pointer. It is implementation defined what the actual definition of `NULL` is, as long as it is a valid null pointer constant.

```c
if (pointer == 0)
```

`0` is another representation of the null pointer constant.

```c
if (!pointer)
```

This `if` statement implicitly checks "is not 0", so we reverse that to mean "is 0".

The following are INVALID ways to check for a null pointer:

```c
int mynull = 0;
<some code>
if (pointer == mynull)
```

To the compiler this is not a check for a null pointer, but an equality check on two variables. This *might* work if mynull never changes in the code and the compiler optimizations constant fold the 0 into the if statement, but this is not guaranteed and the compiler has to produce at least one diagnostic message (warning or error) according to the C Standard.

Note that the value of a null pointer in the C language does not matter on the underlying architecture. If the underlying architecture has a null pointer value defined as address 0xDEADBEEF, then it is up to the compiler to sort this mess out.

As such, even on this funny architecture, the following ways are still valid ways to check for a null pointer:

```c
if (!pointer)
if (pointer == NULL)
if (pointer == 0)
```

The following are INVALID ways to check for a null pointer:

```c
#define MYNULL (void *) 0xDEADBEEF
if (pointer == MYNULL)
if (pointer == 0xDEADBEEF)
```

as these are seen by a compiler as normal comparisons.

#### Null Characters

`'\0'` is defined to be a null character - that is a character with all bits set to zero. `'\0'` is (like all character literals) an integer constant, in this case with the value zero. So `'\0'` is completely equivalent to an unadorned `0` integer constant - the only difference is in the *intent* that it conveys to a human reader ("I'm using this as a null character.").

`'\0'` has nothing to do with pointers. However, you may see something similar to this code:

```c
if (!*char_pointer)
```

checks if the char pointer is pointing at a null character.

```c
if (*char_pointer)
```

checks if the char pointer is pointing at a non-null character.

Don't get these confused with null pointers. Just because the bit representation is the same, and this allows for some convenient cross over cases, they are not really the same thing.

#### References

See [Question 5.3 of the comp.lang.c FAQ](http://c-faq.com/null/ptrtest.html) for more. See [this pdf](http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1124.pdf) for the C standard. Check out sections 6.3.2.3 Pointers, paragraph 3.



## stackoverflow [Do you use NULL or 0 (zero) for pointers in C++?](https://stackoverflow.com/questions/176989/do-you-use-null-or-0-zero-for-pointers-in-c)



### [A](https://stackoverflow.com/a/177007)

Here's Stroustrup's take on this: [C++ Style and Technique FAQ](http://www.stroustrup.com/bs_faq2.html#null)

> In C++, the definition of `NULL` is 0, so there is only an aesthetic difference. I prefer to avoid macros, so I use 0. Another problem with `NULL` is that people sometimes mistakenly believe that it is different from 0 and/or not an integer. In pre-standard code, `NULL` was/is sometimes defined to something unsuitable and therefore had/has to be avoided. That's less common these days.
>
> If you have to name the null pointer, call it `nullptr`; that's what it's called in C++11. Then, `nullptr` will be a keyword.

That said, don't sweat the small stuff.



### [A](https://stackoverflow.com/a/177716)

There are a few arguments (one of which is relatively recent) which I believe contradict Bjarne's position on this.

1、**Documentation of intent**

Using `NULL` allows for searches on its use and it also highlights that the developer **wanted** to use a `NULL` pointer, irrespective of whether it is being interpreted by the compiler as `NULL` or not.

2、**Overload of pointer and 'int' is relatively rare**

The example that everybody quotes is:

```cpp
 void foo(int*);
 void foo (int);

 void bar() {
   foo (NULL);  // Calls 'foo(int)'
 }
```

However, at least in my opinion, the problem with the above is not that we're using `NULL` for the null pointer constant: it's that we have overloads of `foo()` which take very different kinds of arguments. The parameter must be an `int` too, as any other type will result in an ambiguous call and so generate a helpful compiler warning.

3、**Analysis tools can help TODAY!**

Even in the absence of C++0x, there are tools available today that verify that `NULL` is being used for pointers, and that `0` is being used for integral types.

4、**[C++ 11](http://en.wikipedia.org/wiki/C++11) will have a new `std::nullptr_t` type.**

This is the newest argument to the table. The problem of `0` and `NULL` is being actively addressed for C++0x, and you can guarantee that for every implementation that provides `NULL`, the very first thing that they will do is:

```cpp
 #define NULL  nullptr
```

For those who use `NULL` rather than `0`, the change will be an improvement in type-safety with little or no effort - if anything it may also catch a few bugs where they've used `NULL` for `0`. For anybody using `0` today... well, hopefully they have a good knowledge of regular expressions...
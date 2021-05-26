# Union and type-punning



## stackoverflow [Is type-punning through a union unspecified in C99, and has it become specified in C11?](https://stackoverflow.com/questions/11639947/is-type-punning-through-a-union-unspecified-in-c99-and-has-it-become-specified)

A number of answers for the Stack Overflow question [Getting the IEEE Single-precision bits for a float](https://stackoverflow.com/questions/11638091/getting-the-ieee-single-precision-bits-for-a-float)suggest using a `union` structure for type punning (e.g.: turning the bits of a `float` into a `uint32_t`):

```c
union {
    float f;
    uint32_t u;
} un;
un.f = your_float;
uint32_t target = un.u;
```

However, the value of the `uint32_t` member of the union appears to be **unspecified** according to the C99 standard (at least draft n1124), where section 6.2.6.1.7 states:

> When a value is stored in a member of an object of union type, the bytes of the **object representation** that do not correspond to that member but do correspond to other members take **unspecified values**.

***SUMMARY*** : 其实这段话中所描述的情形就是对上面代码中的；

At least one footnote of the C11 n1570 draft seems to imply that this is no longer the case (see footnote 95 in 6.5.2.3):

> If the member used to read the contents of a union object is not the same as the member last used to store a value in the object, the appropriate part of the object representation of the value is reinterpreted as an object representation in the new type as described in 6.2.6 (a process sometimes called ‘‘type punning’’). This might be a **trap representation**.



However, the text to section 6.2.6.1.7 is the same in the C99 draft as in the C11 draft.

Is this behavior actually **unspecified** under C99? Has it become specified in C11? I realize that most compilers seem to support this, but it would be nice to know if it's specified in the standard, or just a very common extension.



***COMMENTS*** 

Technical note: Accessing a union member other than the last one stored does not cause a program to violate the C standard. Accessing such a union member results in an unspecified value (not undefined behavior), and, per C 1999 4 3, “shall be a correct program and act in accordance with 5.1.2.3.” Further, a compiler may provide additional guarantees about the value and remain a conforming implementation. – [Eric Postpischil](https://stackoverflow.com/users/298225/eric-postpischil) [Jul 24 '12 at 22:21](https://stackoverflow.com/questions/11639947/is-type-punning-through-a-union-unspecified-in-c99-and-has-it-become-specified#comment15419730_11639947)



### [A](https://stackoverflow.com/a/11640603)

The behavior of type punning with union changed from C89 to C99. The behavior in C99 is the same as C11.

As [Wug](https://stackoverflow.com/a/11639995/1119701) noted in his answer, **type punning** is allowed in C99 / C11. An unspecified value that could be a trap is read when the union members are of **different size**.

The footnote was added in C99 after Clive D.W. Feather [Defect Report #257](http://www.open-std.org/jtc1/sc22/wg14/www/docs/dr_257.htm):

> **Finally, one of the changes from C90 to C99 was to remove any restriction on accessing one member of a union when the last store was to a different one.** The rationale was that the behaviour would then depend on the representations of the values. Since this point is often misunderstood, it might well be worth making it clear in the Standard.
>
> [...]
>
> To address the issue about "type punning", attach a new footnote 78a to the words "named member" in 6.5.2.3#3: 78a If the member used to access the contents of a union object is not the same as the member last used to store a value in the object, the appropriate part of the object representation of the value is reinterpreted as an object representation in the new type as described in 6.2.6 (a process sometimes called "type punning"). This might be a trap representation.

The wording of Clive D.W. Feather was accepted for a Technical Corrigendum in the answer by the C Committee for [Defect Report #283](http://www.open-std.org/jtc1/sc22/wg14/www/docs/dr_283.htm).



### [A](https://stackoverflow.com/a/11640381)

The original C99 specification left this unspecified.

One of the technical corrigenda to C99 (TR2, I think) added footnote 82 to correct this oversight:

> If the member used to access the contents of a union object is not the same as the member last used to store a value in the object, the appropriate part of the object representation of the value is reinterpreted as an object representation in the new type as described in 6.2.6 (a process sometimes called "type punning"). This might be a trap representation.

That footnote is retained in the C11 standard (it's footnote 95 in C11).



## stackoverflow [Getting the IEEE Single-precision bits for a float](https://stackoverflow.com/questions/11638091/getting-the-ieee-single-precision-bits-for-a-float)

I need to write an IEEE single-precision floating point number to a 32-bit hardware register at a particular address. To do that, I need to convert a variable of type float to an unsigned integer. I can get the integer representation like this:

```c
float a = 2.39;
unsigned int *target;
printf("a = %f\n",a);
target = &a;
printf("target = %08X\n",*target);
```

which returns:

```c
a = 2.390000
target = 4018F5C3
```

All good. However this causes a compiler warning "cast.c:12: warning: assignment from incompatible pointer type"

Is there any other way to do this which doesn't generate the warning? This is for specific hardware, I don't need to handle different endianness etc and I don't want to loop through each char for performance reasons as some other questions tend to suggest. It seems like you might be able to use reinterpret_cast in C++ but I am using C.



### [A](https://stackoverflow.com/a/11638255)

You can use type punning with a union,

```c
union {
    float f;
    uint32_t u;
} un;
un.f = your_float;
uint32_t target = un.u;
```

to get the bits. Or you can use `memcpy`.



### [A](https://stackoverflow.com/a/11638276)

You could creat a `union` type that contains a float and an unsigned int, store a value into the float member, then read it out of the int, like so:

```c
union reg_val
{
  float f_val;
  unsigned int i_val;
} myRegister;
myRegister.f_val = 2.39;
printf("target = %08X", myRegister.i_val);
```





## stackoverflow [Type punning and Unions in C](https://stackoverflow.com/questions/11442708/type-punning-and-unions-in-c)

I'm currently working on a project to build a small compiler just for the heck of it.

I've decided to take the approach of building an extremely simple virtual machine to target so I don't have to worry about learning the ins and outs of elf, intel assembly, etc.

My question is about type punning in C using unions. I've decided to only support 32 bit integers and 32 bit float values in the vm's memory. To facilitate this, the "main memory" of the vm is set up like this:

```c
typedef union
{    
    int i;
    float f;
}word;


memory = (word *)malloc(mem_size * sizeof(word));
```

So I can then just treat the memory section as either an int or a float depending on the instruction.

Is this technically type punning? It certainly would be if I were to use ints as the words of memory and then use a `float*` to treat them like floats. My current approach, while syntactically different, I don't think is semantically different. In the end I'm still treating 32 bits in memory as either an int or a float.

The only information I could come up with `online` suggests that this is implementation dependent. Is there a more portable way to acheiv this without wasting a bunch of space?

I could do the following, but then I would be taking up more than 2 times as much memory and "reinventing the wheel" with respect to unions.

```c
typedef struct
{
    int i;
    float f;
    char is_int;
}
```

**Edit**

I perhaps didn't make my exact question clear. I am aware that I can use either a float or an int from a union without undefined behavior. What I'm after is specifically a way to have a 32 bit memory location that I can safely use as an int or float without knowing what the last value set was. I want to account for the situation where the other type is used.



### [A](https://stackoverflow.com/a/11443087)

Yes, storing one member of union and reading another is type punning (assuming the types are sufficiently different). Moreover, this is the only kind of universal (any type to any type) type punning that is officially supported by C language. It is supported in a sense that the language promises that in this case the type punning will actually occur, i.e. that a physical attempt to read an object of one type as an object of another type will take place. Among other things it means that writing one member of the union and reading another member implies a data dependency between the write and the read. **This, however, still leaves you with the burden of ensuring that the type punning does not produce a trap representation**.

When you use **casted pointers for type punning** (what is usually understood as "classic" type punning), the language explicitly states that in general case the behavior is **undefined** (aside from reinterpreting object's value as an array of `char`s and other restricted cases). Compilers like GCC implement so called "strict aliasing semantics", which basically means that the **pointer-based type punning** might not work as you expect it to work. For example, the compiler might (and will) ignore the **data dependency** between type-punned reads and writes and rearrange them arbitrarily, thus completely ruining your intent. This

```c
int i;
float f;

i = 5;
f = *(float *) &i;
```

can be easily rearranged into actual

```c
f = *(float *) &i;
i = 5;
```

specifically because a strict-aliased compiler deliberately ignores the possibility of data dependency between the write and the read in the example.

In a modern C compiler, when you really need to perform physical reinterpretation of one objects value as value of another type, you are restricted to either `memcpy`-ing bytes from one object to another or to union-based type punning. There are no other ways. Casting pointers is no longer a viable option.


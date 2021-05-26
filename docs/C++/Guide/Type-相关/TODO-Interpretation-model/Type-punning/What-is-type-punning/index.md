# Type punning

“type punning”的意思是“类型双关”，最最典型的例子就是C union。

## wikipedia [Type punning](https://en.wikipedia.org/wiki/Type_punning)


In [computer science](https://en.wikipedia.org/wiki/Computer_science), **type punning** is a common term for any programming technique that subverts（颠覆） or circumvents（绕过） the [type system](https://en.wikipedia.org/wiki/Type_system) of a [programming language](https://en.wikipedia.org/wiki/Programming_language) in order to achieve an effect that would be difficult or impossible to achieve within the bounds of the formal language.

In [C](https://en.wikipedia.org/wiki/C_programming_language) and [C++](https://en.wikipedia.org/wiki/C%2B%2B), constructs such as [pointer](https://en.wikipedia.org/wiki/Pointer_(computer_programming)) [type conversion](https://en.wikipedia.org/wiki/Type_conversion) and `union` — C++ adds [reference](https://en.wikipedia.org/wiki/Reference_(computer_science)) type conversion and `reinterpret_cast` to this list — are provided in order to permit many kinds of **type punning**, although some kinds are not actually supported by the standard language.

In the [Pascal](https://en.wikipedia.org/wiki/Pascal_(programming_language)) programming language, the use of [records](https://en.wikipedia.org/wiki/Record_(computer_science)) with [variants](https://en.wikipedia.org/wiki/Variant_type) may be used to treat a particular data type in more than one manner, or in a manner not normally permitted.

> NOTE: [type punning](https://en.wikipedia.org/wiki/Type_punning)则是希望使用一种新的type来解释重解释一个已经声明为另外一种类型的object，它不是[type conversion](https://en.cppreference.com/w/c/language/cast)（type conversion会读取原object，然后构造一个新的类型的object），type punning是就地进行重解释，或者说它是告诉compiler它：按照这种类型来进行解释，在c中，实现type punning可以使用不同的指针类型来进行aliasing，在creference [Objects and alignment](https://en.cppreference.com/w/c/language/object)中专门说明了这种用法：
>
> > Note that type-punning may also be performed through the inactive member of a [union](https://en.cppreference.com/w/c/language/union).
>
> 当然，在标准中，也对这此进行了说明；



### Sockets example

One classic example of **type punning** is found in the [Berkeley sockets](https://en.wikipedia.org/wiki/Berkeley_sockets) interface. The function to bind an opened but uninitialized socket to an [IP address](https://en.wikipedia.org/wiki/IP_address) is declared as follows:

```C
int bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen);
```

The `bind` function is usually called as follows:

```C
struct sockaddr_in sa = {0};
int sockfd = ...;
sa.sin_family = AF_INET;
sa.sin_port = htons(port);
bind(sockfd, (struct sockaddr *)&sa, sizeof sa);
```

The Berkeley sockets library fundamentally relies on the fact that in [C](https://en.wikipedia.org/wiki/C_programming_language), a pointer to `struct sockaddr_in` is freely convertible to a pointer to `struct sockaddr`; and, in addition, that the two structure types share **the same memory layout**. Therefore, a reference to the structure field `my_addr->sin_family` (where `my_addr` is of type `struct sockaddr*`) will actually refer to the field `sa.sin_family` (where `sa` is of type `struct sockaddr_in`). In other words, the sockets library uses **type punning** to implement a rudimentary(畸形的) form of [polymorphism](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)) or [inheritance](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)).

Often seen in the programming world is the use of "padded" data structures to allow for the storage of different kinds of values in what is effectively the same storage space. This is often seen when two structures are used in mutual exclusivity for optimization.

> NOTE: 
>
> 一、显然， "the two structure types share **the same memory layout**"是它们可以互换的前提；
>
> 二、 在[beej **struct sockaddr** and pals](https://beej.us/guide/bgnet/html/multi/sockaddr_inman.html)中对这个问题也进行了阐述：
>
> > In memory, the `struct sockaddr_in` and `struct sockaddr_in6` share the same beginning structure as `struct sockaddr`, and you can freely cast the pointer of one type to the other without any harm, except the possible end of the universe.

### Floating-point example

Not all examples of **type punning** involve structures, as the previous example did. Suppose we want to determine whether a [floating-point](https://en.wikipedia.org/wiki/Floating-point) number is negative. We could write:

```c
bool is_negative(float x) {
    return x < 0.0;
}
```

However, supposing that floating-point comparisons are expensive, and also supposing that `float` is represented according to the [IEEE floating-point standard](https://en.wikipedia.org/wiki/IEEE_floating-point_standard), and integers are 32 bits wide, we could engage in type punning to extract the [sign bit](https://en.wikipedia.org/wiki/Sign_bit) of the floating-point number using only integer operations:

```c
bool is_negative(float x) {
    unsigned int *ui = (unsigned int *)&x;
    return *ui & 0x80000000;
}
```

Note that the behaviour will not be exactly the same: in the special case of `x` being [negative zero](https://en.wikipedia.org/wiki/Signed_zero), the first implementation yields `false` while the second yields `true`.

This kind of **type punning** is more dangerous than most. Whereas the former example relied only on guarantees made by the C programming language about structure layout and **pointer convertibility**, the latter example relies on assumptions about a particular system's hardware. Some situations, such as [time-critical](https://en.wikipedia.org/wiki/Real-time_computing) code that the compiler otherwise fails to [optimize](https://en.wikipedia.org/wiki/Compiler_optimization), may require dangerous code. In these cases, documenting all such assumptions in [comments](https://en.wikipedia.org/wiki/Comment_(computer_programming)), and introducing [static assertions](https://en.wikipedia.org/wiki/Assertion_(computing)#Static_assertions) to verify portability expectations, helps to keep the code [maintainable](https://en.wikipedia.org/wiki/Maintainability).

For a practical example popularized by [Quake III](https://en.wikipedia.org/wiki/Quake_III), see [fast inverse square root](https://en.wikipedia.org/wiki/Fast_inverse_square_root).

### C and C++

> NOTE: 
>
> 在 C 和 C++中，有两种实现方式:
>
> 1、pointer conversion
>
> 2、union

In addition to the assumption about **bit-representation** of floating-point numbers, the previous floating-point type-punning example also violates the C language's constraints on **how objects are accessed**:[[1\]](https://en.wikipedia.org/wiki/Type_punning#cite_note-ISOs6.5/7-1) the declared type of `x` is `float` but it is read through an expression of type `unsigned int`. On many common platforms, this use of **pointer punning** can create problems if different pointers are [aligned in machine-specific ways](https://en.wikipedia.org/wiki/Data_structure_alignment). Furthermore, pointers of **different sizes** can [alias accesses to the same memory](https://en.wikipedia.org/wiki/Aliasing_(computing)), causing problems that are unchecked by the compiler.

> NOTE: 
>
> 一、"bit-representation of floating-point numbers" 即 "serialization序列化-object memory binary representation"
>
> 二、这一段关于**pointer punning**的分析非常细致；
>
> 其中关于pointer cast和alignment的问题，在[cppreference cast operator](https://en.cppreference.com/w/c/language/cast)中有描述；我们需要遵循"strict aliasing"原则，否则就是undefined behavior。
>
> 其中提及的"pointer of different size"的含义是: pointer to different type，different type may have different size；

#### Use of pointers

A naive attempt at type-punning can be achieved by using pointers:

```C++
float pi = 3.14159;
uint32_t piAsRawData = *(uint32_t*)&pi;
```

According to the C standard, this code should not (or rather, does not have to) compile, however, if it does, then `piAsRawData` typically contains the raw bits of `pi`.

> NOTE: 
>
> 上述code是违反strict aliasing的

#### Use of `union`

It is a common mistake to attempt to fix **type-punning** by the use of a `union`. (The following example additionally makes the assumption of IEEE-754 bit-representation for floating-point types).

This **aliasing problem** can be fixed by the use of a `union` (though, this example still makes the assumption about IEEE-754 bit-representation of floating-point types):

```c
bool is_negative(float x) {
    union {
        unsigned int ui;
        float d;
    } my_union = { .d = x };
    return my_union.ui & 0x80000000;
}
```

This is legal C99 and above (but [undefined behavior](https://en.wikipedia.org/wiki/Undefined_behavior) in C++ [[2\]](https://en.wikipedia.org/wiki/Type_punning#cite_note-2)) because the **stored value** of an object may be accessed through an **lvalue of aggregate or union type** that includes one of the aforementioned types among its members (including, recursively, a member of a subaggregate or contained union).[[1\]](https://en.wikipedia.org/wiki/Type_punning#cite_note-ISOs6.5/7-1) **Here the relevant declared type is `float` for the store and `unsigned` for the read in the union, thus complying with the s6.5 rules.**

> NOTE :在 s6.5 的`7`就描述了这一点；
>
> 最后一句话就表明了`union`在c中的价值；

Some compilers like [GCC](https://en.wikipedia.org/wiki/GNU_Compiler_Collection) support such constructs as a language extension.[[3\]](https://en.wikipedia.org/wiki/Type_punning#cite_note-3)

For another example of type punning, see [Stride of an array](https://en.wikipedia.org/wiki/Stride_of_an_array).


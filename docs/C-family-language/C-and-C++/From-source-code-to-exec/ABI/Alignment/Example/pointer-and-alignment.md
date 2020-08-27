# [Should I worry about the alignment during pointer casting?](https://stackoverflow.com/questions/13881487/should-i-worry-about-the-alignment-during-pointer-casting)



In my project we have a piece of code like this:

```C
// raw data consists of 4 ints
unsigned char data[16];
int i1, i2, i3, i4;
i1 = *((int*)data);
i2 = *((int*)(data + 4));
i3 = *((int*)(data + 8));
i4 = *((int*)(data + 12));
```

***SUMMARY*** : `char`类型的alignment是1，所以`data`的alignment也是1；

***SUMMARY*** : 上述代码中，`i1, i2, i3, i4`其实是在执行后面的赋值语句之前就已经被分配了，后面的`*((int*)data)`是dereference一个指针；`(int*)data`不一定满足`int`的alignment；



I talked to my tech lead that this code may not be portable since it's trying to cast a `unsigned char*`to a `int*` which usually has a more strict alignment requirement. But tech lead says that's all right, most compilers remains the same pointer value after casting, and I can just write the code like this.

***SUMMARY*** : 上面这段话中的cast a `unsigned char*`to a `int*` which usually has a more strict alignment requirement的意思是，`unsigned char*`和`int*` 都是指针类型，它们的alignment和它们所指向的数据的alignment是不同的，比如在64位系统中，`unsigned char*`的alignment是8，它所指向的是`char`，`char`的alignment是1；

To be frank, I'm not really convinced. After researching, I find some people against use of pointer castings like above, e.g., [here](https://www.securecoding.cert.org/confluence/display/cplusplus/EXP36-CPP.+Do+not+convert+pointers+into+more+strictly+aligned+pointer+types) and [here](http://www.informit.com/guides/content.aspx?g=cplusplus&seqNum=548).

So here are my questions:

1. Is it REALLY safe to dereference the pointer after casting in a real project?
2. Is there any difference between C-style casting and `reinterpret_cast`?
3. Is there any difference between C and `C++`?



Many CPUs will **trap** on **unaligned accesses**. Is that enough of a reason for you? – [Fanael](https://stackoverflow.com/users/784668/fanael) [Dec 14 '12 at 15:24](https://stackoverflow.com/questions/13881487/should-i-worry-about-the-alignment-during-pointer-casting#comment19120332_13881487)



## [A](https://stackoverflow.com/a/13881606)



> \1. Is it REALLY safe to dereference the pointer after casting in a real project?

If the pointer happens to not be aligned properly it really can cause problems. I've personally seen and fixed bus errors in real, production code caused by casting a `char*` to a **more strictly aligned type**. Even if you don't get an obvious error you can have less obvious issues like slower performance. Strictly following the standard to avoid UB is a good idea even if you don't immediately see any problems. (And one rule the code is breaking is the strict aliasing rule, § 3.10/10*)

A better alternative is to use `std::memcpy()` or `std::memmove` if the buffers overlap (or better yet [`bit_cast<>()`](https://gist.github.com/3472964))

```C
unsigned char data[16];
int i1, i2, i3, i4;
std::memcpy(&i1, data     , sizeof(int));
std::memcpy(&i2, data +  4, sizeof(int));
std::memcpy(&i3, data +  8, sizeof(int));
std::memcpy(&i4, data + 12, sizeof(int));
```

Some compilers work harder than others to make sure char arrays are aligned more strictly than necessary because programmers so often get this wrong though.

```C
#include <cstdint>
#include <typeinfo>
#include <iostream>

template<typename T> void check_aligned(void *p) {
    std::cout << p << " is " <<
      (0==(reinterpret_cast<std::intptr_t>(p) % alignof(T))?"":"NOT ") <<
      "aligned for the type " << typeid(T).name() << '\n';
}

void foo1() {
    char a;
    char b[sizeof (int)];
    check_aligned<int>(b); // unaligned in clang
}

struct S {
    char a;
    char b[sizeof(int)];
};

void foo2() {
    S s;
    check_aligned<int>(s.b); // unaligned in clang and msvc
}

S s;

void foo3() {
    check_aligned<int>(s.b); // unaligned in clang, msvc, and gcc
}

int main() {
    foo1();
    foo2();
    foo3();
}
```

http://ideone.com/FFWCjf

> \2. Is there any difference between C-style casting and reinterpret_cast?

It depends. C-style casts do different things depending on the types involved. C-style casting between pointer types will result in the same thing as a reinterpret_cast; See § 5.4 *Explicit type conversion (cast notation)* and § 5.2.9-11.

> \3. Is there any difference between C and C++?

There shouldn't be as long as you're dealing with types that are legal in C.

------

\* Another issue is that C++ does not specify the result of casting from one pointer type to a type with stricter alignment requirements. This is to support platforms where unaligned pointers cannot even be represented. However typical platforms today can represent unaligned pointers and compilers specify the results of such a cast to be what you would expect. As such, this issue is secondary to the aliasing violation. See [expr.reinterpret.cast]/7.



## [A](https://stackoverflow.com/a/13881632)





It's not alright, really. The alignment may be wrong, and the code may violate strict aliasing. You should unpack it explicitly.

```C
i1 = data[0] | data[1] << 8 | data[2] << 16 | data[3] << 24;
```

etc. This is definitely well-defined behaviour, and as a bonus, it's also endianness-independent, unlike your pointer cast.



## [A](https://stackoverflow.com/a/13881680)

The correct way to unpack `char` buffered data is to use `memcpy`:

```
unsigned char data[4 * sizeof(int)];
int i1, i2, i3, i4;
memcpy(&i1, data, sizeof(int));
memcpy(&i2, data + sizeof(int), sizeof(int));
memcpy(&i3, data + 2 * sizeof(int), sizeof(int));
memcpy(&i4, data + 3 * sizeof(int), sizeof(int));
```

Casting violates aliasing, which means that the compiler and optimiser are free to treat the source object as uninitialised.

Regarding your 3 questions:

1. No, dereferencing a cast pointer is in general unsafe, because of aliasing and alignment.
2. No, in C++, C-style casting is defined in terms of `reinterpret_cast`.
3. No, C and C++ agree on cast-based aliasing. There is a difference in the treatment of union-based aliasing (C allows it in some cases; C++ does not).







# [Output from arbitrary dereferenced pointer](https://stackoverflow.com/questions/12451230/output-from-arbitrary-dereferenced-pointer)

I fill the memory as follows:

```cpp
char buf[8] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
```

And than put the **unsigned long pointer** in turn on first 5 bytes and output result:

```cpp
char *c_ptr;
unsigned long *u_ptr;

c_ptr = buf;
for (int i=0;i<5;i++)
{
    u_ptr = (unsigned long *)c_ptr;
    printf("%X\n",*u_ptr);
    c_ptr++;
}
```

When I execute this code on my x64 plaform I get what I expected:

```cpp
44332211
55443322
66554433
77665544
88776655
```

***SUMMARY*** : 这个输出其实传达了非常多的信息：

- [Endianness](https://en.wikipedia.org/wiki/Endianness) 我们所看到的数据在内存中是如何存放的
- cast的底层含义：将一个`char *c_ptr`转换为`unsigned long *u_ptr`，然后访问`*u_ptr`，其实会read 4 个byte

But when I execute the same code on ARM platform I get following:

```cpp
44332211
11443322
22114433
33221144
88776655
```

I.e. it get bound every 4 byte and dereference only 4 bytes within this bounds.

So I want to ask, if this behavior (when `pointer_value%4 != 0`) erroneous or implementation-specific?

UPD: I known about endiannes, I want to know is this correct, that I am getting

```cpp
11443322
```

instead of

```cpp
55443322
```

I.e when I have pointer for example `0x10000001` It makes unsigned long from bytes with addresses `0x10000001`, `0x10000002`, `0x10000003` and than `0x10000000`, instead of `0x10000005`.



***COMMENTS*** : 

- [en.wikipedia.org/wiki/Endianness](http://en.wikipedia.org/wiki/Endianness) – [Nikolai Fetissov](https://stackoverflow.com/users/106671/nikolai-fetissov) [Sep 16 '12 at 22:24](https://stackoverflow.com/questions/12451230/output-from-arbitrary-dereferenced-pointer#comment16744142_12451230)

- 

  So far, every comment and answer has missed the point. The bytes aren't reversed, they're rotated. – [Marcelo Cantos](https://stackoverflow.com/users/9990/marcelo-cantos) [Sep 16 '12 at 22:29](https://stackoverflow.com/questions/12451230/output-from-arbitrary-dereferenced-pointer#comment16744192_12451230)

- 

  A memory alignment thing? – [paddy](https://stackoverflow.com/users/1553090/paddy) [Sep 16 '12 at 22:30](https://stackoverflow.com/questions/12451230/output-from-arbitrary-dereferenced-pointer#comment16744199_12451230)

- 

  The accepted answer explains what's happening. At the language level I think the problem is that your program is not a valid program according to the standard and you are in the *undefined behavior* arena: I think converting a `char*` to a `long*` is valid only if the pointer was originaly a pointer to `long*`, problem is that I cannot find the actual standard chapter(s) that make it so. – [Analog File](https://stackoverflow.com/users/574590/analog-file) [Sep 16 '12 at 22:48](https://stackoverflow.com/questions/12451230/output-from-arbitrary-dereferenced-pointer#comment16744347_12451230)

- 

  The above casting is only way to create a non-aligment pointer. I had problem in a program, where another non-aligment pointer was used, so want to know, is this problem because of this. – [Nosturion](https://stackoverflow.com/users/1676270/nosturion) [Sep 16 '12 at 22:50](https://stackoverflow.com/questions/12451230/output-from-arbitrary-dereferenced-pointer#comment16744369_12451230)



## [A](https://stackoverflow.com/a/12451301)

After suspecting memory alignment I did a quick google =)

http://awayitworks.blogspot.co.nz/2010/02/arm-memory-alignment.html

Stated in that article:

> Till ARMv4 architecture, it’s assumed that address given for fetching contents is memory aligned...a 32-bit data fetch should have address aligned to 32-bit and so on. As guessed correctly the problem is only for 32-bit and 16-bit data fetching. ARM ignores lower 2-bits of address if the data fetch is 32-bit, and ignores lower 1-bit if data fetch is 16-bit. So, in all if the address is not properly aligned then data fetch will be erroneous.

Note the last sentence =)

If you require the behaviour that you expected on x86, you'll have to explicitly build the integers from chars, *ie* (assuming little-endian):

```cpp
// Endian-specific
inline unsigned long ulong_at( const char *p ) {
    return ((unsigned long)p[0])
         | (((unsigned long)p[1]) << 8)
         | (((unsigned long)p[2]) << 16)
         | (((unsigned long)p[3]) << 24);
}
```

Or perhaps:

```cpp
// Architecture-specific
inline unsigned long ulong_at( const char *p ) {
    unsigned long val;
    char *v = (char*)&val;
    v[0] = p[0];
    v[1] = p[1];
    v[2] = p[2];
    v[3] = p[3];
    return val;
}
```



## [A](https://stackoverflow.com/a/12451306)

If you want to fetch a four-byte word from memory, the address should be a multiple of four.

Misaligned access is generally a bad idea on any architecture. Some will throw a SEGFAULT, others will transparently handle the fault and — very slowly — synthesise the correct value by fetching the two words containing the desired value and stitching them together. It appears (though I'm not an expert) that ARM is fetching the four-byte slot that the pointer occupies and rotating the result so that the LSB of the register matches the pointer.





# [How to Access Safely Unaligned Data](https://www.alfonsobeato.net/arm/how-to-access-safely-unaligned-data/)





# [EXP36-C. Do not cast pointers into more strictly aligned pointer types](https://wiki.sei.cmu.edu/confluence/display/c/EXP36-C.+Do+not+cast+pointers+into+more+strictly+aligned+pointer+types)
# Byte type

- `unsigned char`

- `char`

- `uint8_t`

- `int8_t`
- `std::byte` (since C++17)

它们都能够表示byte，都能够用于查看object representation（`C-family-language\C++\Language-reference\Basic-concept\Data-model\Object\Object.md`），但是如何来使用呢？

按照intentional programming原则来选择合适的类型：

## Arithmetic type

- `uint8_t`

- `int8_t`



当需要进行arithmetic operation的时候，需要使用`uint8_t`、`uint8_t`，在quarkslab [Unaligned accesses in C/C++: what, why and solutions to do it properly](https://blog.quarkslab.com/unaligned-accesses-in-cc-what-why-and-solutions-to-do-it-properly.html)中，给出了最最典型的例子：

```c++
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#define CST 11246805480246805480ULL
// 函数名中的le表示是是little endian
static uint64_t load64_le(uint8_t const* V)
{
#if !defined(__LITTLE_ENDIAN__)
#error This code only works with little endian systems
#endif
  uint64_t Ret = *((uint64_t const*)V); // 转换为uint64_t类型，它会从V开始，读取8个字节的数据
  return Ret;
}

static uint64_t hash(const uint8_t* Data, const size_t Len)
{
   uint64_t Ret = 0;
   // 每次取8字节
   const size_t NBlocks = Len/8;
   for (size_t I = 0; I < NBlocks; ++I) {
     const uint64_t V = load64_le(&Data[I*sizeof(uint64_t)]);
     Ret = (Ret ^ V)*CST;
   }
   uint64_t LastV = 0;
   for (size_t I = 0; I < (Len-NBlocks*8); ++I) {
     LastV |= ((uint64_t)Data[NBlocks*8+I]) << (I*8);
   }
   Ret = (Ret^LastV)*CST;
   return Ret;
}

int main(int argc, char** argv)
{
  if (argc < 2) {
    fprintf(stderr, "Usage: %s string\n", argv[0]);
    return 1;
  }
  const char* Str = argv[1];
  const uint64_t H = hash((const uint8_t*) Str, strlen(Str)); 
  printf("%016" PRIX64 "\n", H);
  return 0;
}
```

> The full source code with a convenient `main` function can be downloaded here: https://gist.github.com/aguinet/4b631959a2cb4ebb7e1ea20e679a81af.

在函数`hash`中要进行arithmetic operation，所以它的函数入参类型为`const uint8_t* Data`，在`main`函数中，调用它时（`hash((const uint8_t*) Str, strlen(Str))`），进行了type cast。



### stackoverflow [uint8_t vs unsigned char](https://stackoverflow.com/questions/1725855/uint8-t-vs-unsigned-char)

What is the advantage of using `uint8_t` over `unsigned char` in C?

I know that on almost every system `uint8_t` is just a typedef for `unsigned char`, so why use it?

#### [A](https://stackoverflow.com/a/1725867)

It documents your intent - you will be storing small numbers, rather than a character.

Also it looks nicer if you're using other typedefs such as `uint16_t` or `int32_t`.

#### [A](https://stackoverflow.com/a/1725901)

Just to be pedantic, some systems may not have an 8 bit type. According to [Wikipedia](http://en.wikipedia.org/wiki/Stdint.h):

> An implementation is required to define exact-width integer types for N = 8, 16, 32, or 64 if and only if it has any type that meets the requirements. It is not required to define them for any other N, even if it supports the appropriate types.

So `uint8_t` isn't guaranteed to exist, though it will for all platforms where 8 bits = 1 byte. Some embedded platforms may be different, but that's getting very rare. Some systems may define `char`types to be 16 bits, in which case there probably won't be an 8-bit type of any kind.

Other than that (minor) issue, [@Mark Ransom's answer](https://stackoverflow.com/questions/1725855/uint8-vs-unsigned-char/1725867#1725867) is the best in my opinion. Use the one that most clearly shows what you're using the data for.

Also, I'm assuming you meant `uint8_t` (the standard typedef from C99 provided in the `stdint.h`header) rather than `uint_8`(not part of any standard).

## `std::byte` (since C++17)

### [`std::byte`](https://en.cppreference.com/w/cpp/types/byte)  (since C++17)

```C++
enum class byte : unsigned char {} ;
```



Like char and unsigned char, it can be used to access raw memory occupied by other objects ([object representation](../language/objects.html)), but unlike those types, it is not a character type and is not an arithmetic type. A byte is only a collection of bits, and the only operators defined for it are the **bitwise ones**.



#### Notes

A numeric value `n` can be converted to a byte value using `std::byte{n}`, due to C++17 relaxed enum class initialization rules.

A byte can be converted to a numeric value (such as to produce an integer hash of an object) using **std::to_integer**.

### stackoverflow [How to use new std::byte type in places where old-style unsigned char is needed?](https://stackoverflow.com/questions/46150738/how-to-use-new-stdbyte-type-in-places-where-old-style-unsigned-char-is-needed)

```cpp
using Bytes = std::vector<std::byte>;
```

[A](https://stackoverflow.com/a/46151026)

Remember that C++ is a strongly typed language in the interest of safety (so implicit conversions are restricted in many cases). Meaning: If an implicit conversion from `byte` to `char` was possible, it would defeat the purpose.

So, to answer your question: To use it, you have to cast it whenever you want to make an assignment to it:

```cpp
std::byte x = (std::byte)10;
std::byte y = (std::byte)'a';
std::cout << (int)x << std::endl;
std::cout << (char)y << std::endl;
```

Anything else shall not work, by design! So that transform is ugly, agreed, but if you want to store chars, then use `char`. Don't use bytes unless you want to store raw memory *that should not be interpreted as `char` by default*.

### stackoverflow [How to use something like `std::basic_istream`](https://stackoverflow.com/questions/43735918/how-to-use-something-like-stdbasic-istreamstdbyte)



## [microsoft](https://github.com/microsoft)/**[GSL](https://github.com/microsoft/GSL)** # [`gsl::byte`](https://github.com/microsoft/GSL/blob/main/include/gsl/byte)

|      |      |                                               |
| ---- | ---- | --------------------------------------------- |
| byte | ☑    | either an alias to `std::byte` or a byte type |
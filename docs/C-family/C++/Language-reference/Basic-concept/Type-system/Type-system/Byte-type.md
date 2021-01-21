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

## Byte

- `std::byte`  (since C++17)

> Like char and unsigned char, it can be used to access raw memory occupied by other objects ([object representation](../language/objects.html)), but unlike those types, it is not a character type and is not an arithmetic type. A byte is only a collection of bits, and the only operators defined for it are the **bitwise ones**.
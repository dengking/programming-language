# Literals



## cppreference [Literals](https://en.cppreference.com/w/cpp/language/expressions#Literals)



## Example

char literal：

这个例子是源自creference `Objects and alignment#Strict aliasing`

```C++
#include <cstdio>

int main()
{
	int i = 7;//最低有效位是0x7
	char* pc = (char*) (&i);
	if (pc[0] == '\x7') // aliasing through char is ok
		puts("This system is little-endian");
	else
		puts("This system is big-endian");

}

```

integer literal

这个例子源自：wikipedia [Endianness](https://en.wikipedia.org/wiki/Endianness)

```c++
#include "stdint.h"
#include "stdio.h"

int main(void)
{
	union u_t
	{
		uint8_t u8;
		uint16_t u16;
		uint32_t u32;
		uint64_t u64;
	} u = { .u64 = 0x4A };
	puts(u.u8 == u.u16 && u.u8 == u.u32 && u.u8 == u.u64 ? "true" : "false");
}
// gcc test.c

```



pzemtsov [A bug story: data alignment on x86](http://pzemtsov.github.io/2016/11/06/bug-story-alignment-on-x86.html)

```c++
_Bool check_ip_header_sum (const char * p, size_t size)
{
    const uint32_t * q = (const uint32_t *) p; // 不符合strict aliasing，undefined behavior
    uint64_t sum = 0;

    sum += q[0];
    sum += q[1];
    sum += q[2];
    sum += q[3];
    sum += q[4];

    for (size_t i = 5; i < size / 4; i++) {
        sum += q[i];
    }

    do {
        sum = (sum & 0xFFFF) + (sum >> 16);
    } while (sum & ~0xFFFFL);

    return sum == 0xFFFF;
}
```


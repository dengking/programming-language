# pzemtsov [A bug story: data alignment on x86](http://pzemtsov.github.io/2016/11/06/bug-story-alignment-on-x86.html)

Tags: [C](http://pzemtsov.github.io/#tag:C) [optimisation](http://pzemtsov.github.io/#tag:optimisation) [alignment](http://pzemtsov.github.io/#tag:alignment)

I once had to calculate a sum of a vector of integers.

This sounds unusual. Who ever wants to do this in real life? Normally, we only see code like this as primary school assignments or compiler benchmarks. However, this time it happened in real life.

Well, the actual task was to verify the [IP header checksum](https://en.wikipedia.org/wiki/IPv4_header_checksum), which is a ones’ complement sum of two-byte words. Simply speaking, it means adding together all the words and all the carry bits produced in the process. This procedure has several nice features:

- it can be done very efficiently using the `ADC` processor instruction (unfortunately, this feature is not accessible from **C**)
- it can be done on any size words (you can add eight-byte values if you wish, as long as you reduce the result to two bytes and add all the overflow bits together)
- it is insensitive to the endianness of the processor (very surprising, but true).

There was one important requirement: the input data was not aligned (the IP frames as received by hardware or read from a file).

I didn’t have to bother about portability, as the only platform the code had to run was Intel x64 (Linux and GCC 4.8.3). Intel has no restrictions on the alignment of integer operands (an **unaligned access** used to be slow, but isn’t any more), and, since the endianness was not important, the little-endian case was also all right. So I quickly wrote:

```c
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

The source code and assembly output can be found in the [repository](https://github.com/pzemtsov/article-align-x86).

The most common size of an IP header is 20 bytes (5 double words, which I’ll call simply *words*) – that’s why the code looks like this. In addition, the size can never be less than that – this is checked before calling this function. Since IP header can never be longer than 15 words, the number of loop iterations is between 0 and 10.

This indeed isn’t portable – accessing arbitrary memory using pointers to 32-bit values is known to fail on some processors, such as most, if not all, RISC ones. However, as I said before, I never expected this to be an issue on x86.


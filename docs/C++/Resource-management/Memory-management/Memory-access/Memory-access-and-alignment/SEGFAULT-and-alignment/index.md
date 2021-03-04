# SEGFAULT and alignment



## groups.google [Segmentation fault at the alignment step](https://groups.google.com/forum/#!topic/rna-star/-3slzCzJGnk)





## stackoverflow [SIMD intrinsics - segmentation fault](https://stackoverflow.com/questions/25596379/simd-intrinsics-segmentation-fault)

I am running the following code:

```c
#include <emmintrin.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argv, char** argc)
{

        float a[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
        float b[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
        float c[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};

        __m128 *v_a = (__m128*)(a+1); // Trying to create c[i] = a[i=1] * b[i];
        __m128 *v_b = (__m128*)(b);
        __m128 *v_c = (__m128*)(c);

        for (int i=0; i < 1; i++)
        {
                *v_c = _mm_mul_ps(*v_a,*v_b);
                v_a++;
                v_b++;
                v_c++;
        }

        for (int i=0; i<= 9;i++)
        {
                printf("%f\n",c[i]);
        }
        return 0;
}
```

and getting a segmentation fault: 11 (on a Mac running OS X "Mavericks").

When removing the +1 from the a, and declaring a like that:

```c
__m128 *v_a = (__m128*)(a+1);
```

It works.

Now I wonder about few things:

1. Why does it happen? There shouldn't be any "memory alignment" issue that could result in accessing a memory that is not allocated. If I am wrong with my understanding - please let me know what I missed.
2. what conversion is happening with the `(__m128*)(a+1)`.

I am trying to understand how the SIMD works, so any information you can link - might help me understand why it reacts that way.



### [A](https://stackoverflow.com/a/25596779)

To expand on Cory Nelson's answer:

Every type has an alignment. An object of a given type "wants" an address that is a multiple of the alignment. For example, a variable of type float has an alignment of 4. This means very literally that when you take the address of a float, and cast it to an integer, you will get a multiple of 4, because the compiler will never assign an address that's not a multiple of 4 to a float.

On 32 bit x86, here are some example alignments: char=1, short=2, int=4, long long=4, float=4, double=4, void*=4, SSE vector=16. Alignments are always powers of 2.

We can get a misaligned address if we typecast a pointer to a different pointer type with a stricter (larger) alignment. That's what's happening in your code, when you cast a `float *` (alignment 4) to a `__m128 *` (alignment 16). The consequences of accessing (reading or writing) an object with a misaligned address can be nothing, a performance penalty, or a crash, depending on the processor architecture.

We can print out the addresses of your vectors:

```c
printf("%p %p %p\n", a, b, c);
```

or for more clarity, just their low 4 bits:

```c
printf("%ld %ld %ld\n", (intptr_t)a & 0xF, (intptr_t)b & 0xF,(intptr_t)c & 0xF);
```

On my machine, this outputs `12 4 12`, showing that the addresses are not multiples of 16, and therefore not 16-byte aligned. (But notice they are all multiples of 4, because they have type array-of-float, and floats must be 4 byte aligned.)

When you remove the +1, your code no longer crashes. This is because you are "getting lucky" with the addresses: the floats must be aligned to a multiple of 4, but they just happen to be aligned to a multiple of 16 as well. This is a time bomb! Tweak something in your code (say, introduce another variable), or change the optimization level, and it is likely to start crashing! You need to align the variables explicitly.

So how to align them? When you declare a variable, the compiler (not you) picks an address in memory where that variable will live. It tries to pack variables as close together as possible, so as to avoid wasting space, but it still has to ensure that the addresses have the correct alignment for their type.

One of the best ways to increase alignment is to use a union, that includes a type whose alignment is what you need:

```c
   union vec {
        float f[10];
        __m128 v;
    };
    union vec av = {.f = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0}};
    union vec bv = {.f = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0}};
    union vec cv = {.f = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0}};
    float *a = av.f;
    float *b = bv.f;
    float *c = cv.f;
    printf("%ld %ld %ld\n", (intptr_t)a & 0xF, (intptr_t)b & 0xF,(intptr_t)c & 0xF);
```

Now the printf outputs `0 0 0`, because the compiler has chosen 16-byte aligned addresses for each float[10].

gcc and clang also allow you to request an alignment directly:

```c
    float a[]  __attribute__ ((aligned (16))) = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
    float b[]  __attribute__ ((aligned (16))) = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
    float c[]  __attribute__ ((aligned (16))) = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
    printf("%ld %ld %ld\n", (intptr_t)a & 0xF, (intptr_t)b & 0xF,(intptr_t)c & 0xF);
```

This works too, but is less portable.

That said, what about your +1:

```c
__m128 *v_a = (__m128*)(a+1);
```

Assuming `a` is 16 byte aligned, and has type `float*`, then `a+1` adds `sizeof(float)` (which is 4) to the address, which results in an address that is merely 4 byte aligned. It's a hardware limitation that you cannot load/store from a merely 4-byte aligned address directly into an SSE register using the normal instructions. It will crash! You have to instead use different (slower) instructions, such as those generated by `_mm_loadu_ps`.

Ensuring proper alignment is one of the challenges of using SIMD instruction sets. You will often see SIMD algorithms process the first few elements using "normal" (scalar) code, so that it can reach the alignment required by the SIMD instructions.



## geeksforgeeks [Segmentation Fault (SIGSEGV) vs Bus Error (SIGBUS)](https://www.geeksforgeeks.org/segmentation-fault-sigsegv-vs-bus-error-sigbus/)


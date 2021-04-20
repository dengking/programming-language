# `ptrdiff_t`

## cppreference [ptrdiff_t](https://en.cppreference.com/w/c/types/ptrdiff_t)

Defined in header `<stddef.h>`

**`ptrdiff_t`** is the signed integer type of the result of [subtracting two pointers](https://en.cppreference.com/w/c/language/operator_arithmetic#Pointer_arithmetic).

### Notes

**`ptrdiff_t`** is used for **pointer arithmetic** and **array indexing**, if **negative values** are possible. Programs that use other types, such as int, may fail on, e.g. 64-bit systems when the index exceeds [INT_MAX](https://en.cppreference.com/w/c/types/limits) or if it relies on 32-bit modular arithmetic.

Only pointers to elements of the same array (including the pointer one past the end of the array) may be subtracted from each other.

If an array is so large (greater than [PTRDIFF_MAX](https://en.cppreference.com/w/c/types/limits) elements, but less than [SIZE_MAX](https://en.cppreference.com/w/c/types/limits) bytes), that the difference between two pointers may not be representable as **ptrdiff_t**, the result of subtracting two such pointers is undefined.

For **char arrays** shorter than [PTRDIFF_MAX](https://en.cppreference.com/w/c/types/limits), **ptrdiff_t** acts as the **signed counterpart** of [size_t](https://en.cppreference.com/w/c/types/size_t): it can store the size of the array of any type and is, on most platforms, synonymous with [intptr_t](https://en.cppreference.com/w/c/types/integer)).

> NOTE:`ptrdiff_t`是signed的，而`size_t`是unsigned；

### Example

```C
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
 
int main(void)
{
    const size_t N = 100;
    int numbers[N];
 
    printf("PTRDIFF_MAX = %ld\n", PTRDIFF_MAX);
    int *p1=&numbers[18], *p2=&numbers[23];
    ptrdiff_t diff = p2-p1;
    printf("p2-p1 = %td\n", diff);
 
    return 0;
}
```
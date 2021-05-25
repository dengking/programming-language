```C++
#include <sys/types.h>
#include <stdint.h>
#include <malloc.h>
#include <stddef.h>
#include <stdio.h>

// struct without flexible array member
struct package{
   char name[20];
   uint8_t len;
};



// struct with flexible array member
struct package_with_fam {
   char name[20];
   uint8_t len;
   uint64_t data[];
};

int main(){

printf("%d\n", sizeof(struct package));
printf("%d\n", sizeof(struct package_with_fam));

int l = offsetof(struct package_with_fam, data);
printf("%d\n", l);

struct package_with_fam *b = malloc(sizeof(*b) + 12 * sizeof(b->data[0]));
}

```


# inline and linkage



## ibm [Linkage of inline functions](https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_72/rzarg/inline_linkage.htm)

### Beginning of C only.

In C, `inline` functions are treated by default as having *static* linkage; that is, they are only visible within a single translation unit. Therefore, in the following example, even though function `foo` is defined in exactly the same way, `foo` in file A and `foo` in file B are treated as separate functions: two function bodies are generated, and assigned two different addresses in memory:

```C++
// test2.c
#include <stdio.h>

__inline__ int foo(){
   return 3;
}

void g() {
   printf("foo called from g: return value = %d, address = %#p\n", foo(), &foo);
}

```



```C
// test.c
#include <stdio.h>

__inline__ int foo(){
   return 3;
}

void g();

int main() {
   printf("foo called from main: return value = %d, address = %#p\n", foo(), &foo);
   g();
}


```

> NOTE: 编译:
>
> ```SHELL
> gcc test2.c test.c
> ```
>
> 报错如下:
>
> ```C++
> /tmp/cc05yZTX.o: In function `foo':
> test.c:(.text+0x0): multiple definition of `foo'
> /tmp/ccAQptbY.o:test2.c:(.text+0x0): first defined here
> collect2: error: ld returned 1 exit status
> ```
>
> 和原文给出的结果是不同的，从上述结果来看，inline function默认是external linkage。









## Example

下面程序是根据ibm [Linkage of inline functions](https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_72/rzarg/inline_linkage.htm)#Beginning of C only改编而来的: 

`test.h`

```C++
#ifndef TEST_H_
#define TEST_H_

inline int foo()
{
return 0;
}
#endif /* TEST_H_ */
```

`test2.c`

```C++
#include "test.h"

void g()
{
printf("foo called from g: return value = %d, address = %#p\n", foo(), &foo);
} // some function returning X


```

`test.c`

```C++
#include "test.h"

void g();

int main()
{
printf("foo called from main: return value = %d, address = %#p\n", foo(), &foo);
g();
}


```

编译:

```shell
gcc test2.c test.c
```



报错如下:

```C++
/tmp/ccLIVv9n.o: In function `foo':
test.c:(.text+0x0): multiple definition of `foo'
/tmp/ccbSk6RY.o:test2.c:(.text+0x0): first defined here
collect2: error: ld returned 1 exit status

```


# inline and linkage

结论是 inline 并不能够使function具有static linkage，对于在头文件中定义的function，需要使用static来进行修饰，使它具有static linkage，从而防止multiple definition。

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

Since inline functions are treated as having **internal linkage**, an inline function definition can co-exist with a regular, external definition of a function with the same name in another translation unit. However, when you call the function from the file containing the inline definition, the compiler may choose *either* the inline version defined in the same file *or* the external version defined in another file for the call; your program should not rely on the inline version being called. In the following example, the call to `foo` from function g could return either 6 or 3:

> NOTE: 关于拥有相同name的多个inline function这一点，在cppreference [`inline` specifier](https://en.cppreference.com/w/cpp/language/inline) 中有关于此的讨论。





### 测试程序

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



## TODO: stackoverflow [Does inline determine internal linkage?](https://stackoverflow.com/questions/56349383/does-inline-determine-internal-linkage)


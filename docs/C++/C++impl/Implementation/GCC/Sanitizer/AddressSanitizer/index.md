# Address Sanitizer



## stackoverflow [What are the useful GCC flags for C?](https://stackoverflow.com/questions/3375697/what-are-the-useful-gcc-flags-for-c) # [A](https://stackoverflow.com/a/3399776)

**-fmudflap** -- adds runtime checks to all risky pointer operations to catch UB. This effectively immunizes your program again buffer overflows and helps to catch all kinds of dangling pointers.

- http://gcc.gnu.org/wiki/Mudflap_Pointer_Debugging

Here's a demo:

```c
$ cat mf.c 
int main()
{
 int a[10];
 a[10]=1; // <-- o noes, line 4
}

$ gcc -fmudflap mf.c -lmudflap
$ ./a.out 
*******
mudflap violation 1 (check/write): time=1280862302.170759 ptr=0x7fff96eb3d00 size=44
pc=0x7f3a575503c1 location=`mf.c:4:2 (main)'
      /usr/lib/libmudflap.so.0(__mf_check+0x41) [0x7f3a575503c1]
      ./a.out(main+0x90) [0x400a54]
      /lib/libc.so.6(__libc_start_main+0xfd) [0x7f3a571e2c4d]
Nearby object 1: checked region begins 0B into and ends 4B after
mudflap object 0xf9c560: name=`mf.c:3:6 (main) a'
bounds=[0x7fff96eb3d00,0x7fff96eb3d27] size=40 area=stack check=0r/3w liveness=3
alloc time=1280862302.170749 pc=0x7f3a57550cb1
number of nearby objects: 1
```

> NOTE: 
>
> 典型的越界访问了

## gcc-10.3.0 [3.12 Program Instrumentation Options](https://gcc.gnu.org/onlinedocs/gcc-10.3.0/gcc/Instrumentation-Options.html#Instrumentation-Options)





## gcc-5.3.0 [3.9 Options for Debugging Your Program or GCC](https://gcc.gnu.org/onlinedocs/gcc-5.3.0/gcc/Debugging-Options.html)
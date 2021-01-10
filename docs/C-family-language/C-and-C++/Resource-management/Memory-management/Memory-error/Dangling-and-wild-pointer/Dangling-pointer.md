# Dangling pointer

dangling pointer和我们平时所说的wild pointer是同一个意思；

它主要是由于 "Access outside of object lifetime"而导致的，参见 `C++\Language-reference\Basic-concept\Object\Lifetime-and-storage-duration\Access-outside-of-object-lifetime` 章节。

## wikipedia [Dangling pointer](https://en.wikipedia.org/wiki/Dangling_pointer)	







## Dangling pointer素材

D:\github\dengking\programming-language-theory\programming-language\docs\C-family-language\C++\Library\Standard-library\Utility-library\Dynamic-memory-management\Smart-pointers\unique_ptr\unique_ptr.md

Lifetime:[Lifetime](https://en.cppreference.com/w/c/language/lifetime) 

```c



int* foo(void) {

   int a = 17; // a has automatic storage duration

​    return &a;

}  // lifetime of a ends

int main(void) {

​    int* p = foo(); // p points to an object past lifetime ("dangling pointer")

​    int n = *p; // undefined behavior

}

```





### geeksforgeeks [Dangling, Void , Null and Wild Pointers](https://www.geeksforgeeks.org/dangling-void-null-wild-pointers/)


### APUE 11.4 Thread Creation
The typeless pointer passed to pthread_create and pthread_exit can be used to pass more than a single value. The pointer can be used to pass the address of a structure containing more complex information. Be careful that the memory used for the structure is still valid when the caller has completed. If the structure was allocated on the caller’s stack, for example, the memory contents might have changed by the time the structure is used. If a thread allocates a structure on its stack and passes a pointer to this structure to pthread_exit, then the stack might be destroyed and its memory reused for something else by the time the caller of pthread_join tries to use it.
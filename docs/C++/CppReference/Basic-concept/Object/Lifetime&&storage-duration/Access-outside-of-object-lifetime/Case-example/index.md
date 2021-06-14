# Access outside of object lifetime example

本文总结我所遇到的access outside of object lifetime。

## Example: in multi-thread application

参见 `Multi-thread-and-observer-pattern` 章节。

## Example: in observer pattern

参见 `Multi-thread-and-observer-pattern` 章节。

## Example: Static Initialization Order Fiasco

参见`Static-Initialization-Order-Fiasco`章节。

4

## Example: iterator invalidation

1、iterator invalidate和object lifetime，Access outside  of lifetime of object 有关。

2、关于 Iterator invalidation，参见 `C++\Library\Standard-library\STL\Containers-library\Common-issue\Iterator-invalidation` 章节；

3、Pointer invalidation，参见:

a、stackoverflow [What exactly is invalidation of reference/pointer?](https://stackoverflow.com/questions/51748529/what-exactly-is-invalidation-of-reference-pointer)



## Example: realloc and move

dangling pointer and object lifetime and `vector<std::string>` and，realloc and move



## Example: Calling Virtual During Initialization

参见:

1、`Classes\Object-lifetime` 章节，其中对此进行了详细描述。

2、cppreference [virtual function specifier](https://en.cppreference.com/w/cpp/language/virtual) # During construction and destruction





## Example: access local object



源自: cppreference [Lifetime](https://en.cppreference.com/w/c/language/lifetime) 

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





### APUE 11.4 Thread Creation

The typeless pointer passed to `pthread_create` and `pthread_exit` can be used to pass more than a single value. The pointer can be used to pass the address of a structure containing more complex information. Be careful that the memory used for the structure is still valid when the caller has completed. If the structure was allocated on the caller’s stack, for example, the memory contents might have changed by the time the structure is used. If a thread allocates a structure on its stack and passes a pointer to this structure to `pthread_exit`, then the stack might be destroyed and its memory reused for something else by the time the caller of `pthread_join` tries to use it.


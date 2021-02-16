# Pointer to `void` 



## Generic interface

正如creference [Pointers to void](https://en.cppreference.com/w/c/language/pointer)中所言：

> Pointers to `void` are used to pass objects of unknown type, which is common in **generic interfaces**

显然pointer to void是C中实现**generic programming**的方式，这种generic是通过**type erasure**来实现的：

1、void是type-less、是incomplete type，programmer无法直接操作void pointer，必须要先将它[static_cast](static_cast.html) or [explicit cast](explicit_cast.html)为complete type才能够进行操作；

关于incomplete type，参见 `Incomplete-type` 章节。

2、Pointer to object of any type can be [implicitly converted](implicit_cast.html) to pointer to `void`，在这个过程中，就丢失了type，显然这就是type erasure，关于type erasure，参见 `Type-Erasure` 章节。

## cppreference Pointer to `void`

Pointer to object of any type can be [implicitly converted](implicit_cast.html) to pointer to `void` (optionally [cv-qualified](cv.html)); the pointer value is unchanged. The reverse conversion, which requires [static_cast](static_cast.html) or [explicit cast](explicit_cast.html), yields the original pointer value:

> NOTE: 上述的conversion，其实就是type erasure，是C中实现generic programming的重要手段。

> NOTE: 下面展示的就是`static_cast`的用法

```c++
#include <iostream>

int main()
{
	int n = 1;
	int* p1 = &n;
	void* pv = p1;
	int* p2 = static_cast<int*>(pv);
	std::cout << *p2 << '\n'; // prints 1
}

```

If the original pointer is pointing to a base class subobject within an object of some polymorphic type, [dynamic_cast](dynamic_cast.html) may be used to obtain a `void*` that is pointing at the complete object of the most derived type.

> NOTE: 这一点就是在cppreference [dynamic_cast](dynamic_cast.html) 中提及的`4)`

Pointers to void are used to pass objects of unknown type, which is common in C interfaces: [std::malloc](../memory/c/malloc.html) returns `void*`, [std::qsort](../algorithm/qsort.html) expects a user-provided callback that accepts two `const void*` arguments. [pthread_create](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_create.html) expects a user-provided callback that accepts and returns `void*`. In all cases, it is the **caller**'s responsibility to cast the pointer to the correct type before use.

TO READ:

stackoverflow [Why type cast a void pointer?](https://stackoverflow.com/questions/16986214/why-type-cast-a-void-pointer)



## void pointer and alignment

alignment如何保证？

错误观点:  `void*`应该具备最大的alignment，这样才能够保证它能够容纳下所有类型的pointer。

正确理解: `void*`类型的变量用于保存pointer的值，由于它是incomplete type，所以programmer无法直接对其进行操作（直接`+`、`*`一个`void*`，编译器会报错的，这样的program是无法编译通过的），programmer必须先将它转换为complete type pointer。由于无法直接使用`void*`，所以考虑考虑它的alignment是没有意义的。需要考虑的是：`void*`的值是否满足它的destination type的alignment，如果不满足，则是会发生undefined behavior的。

## Pointer to void VS pointer to `char` 

前面已经描述了pointer to void了。另外一种非常常见的pointer就是pointer to `char`，两者存在着相似点：

通用性：

| pointer type | 通用性的体现                                                 | 目的                     |
| ------------ | ------------------------------------------------------------ | ------------------------ |
| `void*`      | Pointer to object of any type can be [implicitly converted](implicit_cast.html) to pointer to `void` | 实现generic programming  |
| `char*`      | `char*`与value representation对应，并且它拥有最小的alignment requirement，所以任何类型的pointer也都可以转换为`char*`，从而获得所指向的object的value representation | 获得value representation |

最能够体现两者之间关联的例子是：`std::memcpy`、`std::memmove`，解释如下：

两者的入参类型都是`void*`，显然这样实现了generic，这些函数内部实现中，都需要执行:

> objects are reinterpreted as arrays of unsigned char.

然后按照byte( `unsigned char`)进行操作。

TO READ:

https://stackoverflow.com/questions/10058234/void-vs-char-pointer-arithmetic

https://bytes.com/topic/c/answers/618725-void-vs-char

https://github.com/RIOT-OS/RIOT/issues/5497

https://codeforwin.org/2017/11/c-void-pointer-generic-pointer-use-arithmetic.html

https://www.geeksforgeeks.org/dangling-void-null-wild-pointers/

https://groups.google.com/forum/#!topic/comp.lang.c/kz6ORGo6GD8

http://computer-programming-forum.com/47-c-language/6e45270da06116ac.htm

http://computer-programming-forum.com/47-c-language/6e45270da06116ac.htm





### stackoverflow [What does void mean in C, C++, and C#?](https://stackoverflow.com/questions/1043034/what-does-void-mean-in-c-c-and-c)



#### [A](https://stackoverflow.com/a/1043107)

Basically it means "nothing" or "no type"

There are 3 basic ways that void is used:

1. Function argument: `int myFunc(void)` -- the function takes nothing.

2. Function return value: `void myFunc(int)` -- the function returns nothing

3. Generic data pointer: `void* data` -- 'data' is a pointer to data of unknown type, and cannot be dereferenced

    > NOTE: 所以，it is the caller's responsibility to convert the pointer to the **correct type** before use.

Note: the `void` in a function argument is optional in C++, so `int myFunc()` is exactly the same as `int myFunc(void)`, and it is left out completely in C#. It is always required for a return value.



## Examples

### Plugin system

```c++
int FUNCTION_CALL_MODE HQInit(void* lpInstance, PFOnSetTimeOut pfOnSetTimeOut)
{
	int iRet = I_NONE;
	if (lpInstance)
		iRet = ((CHQImpl*) lpInstance)->OnInit();
	return iRet;
}
```



### redis linked list

```C
typedef struct listNode {
    struct listNode *prev;
    struct listNode *next;
    void *value;
} listNode;
```

节点的数据类型是pointer，因为redis中的所有的数据都是来自于网络，都是从接收到的数据new出一片空间的；

在`networking.c`中有如下函数：

```C
/* This function links the client to the global linked list of clients.
 * unlinkClient() does the opposite, among other things. */
void linkClient(client *c) {
    listAddNodeTail(server.clients,c);
    /* Note that we remember the linked list node where the client is stored,
     * this way removing the client in unlinkClient() will not require
     * a linear scan, but just a constant time operation. */
    c->client_list_node = listLast(server.clients);
    uint64_t id = htonu64(c->id);
    raxInsert(server.clients_index,(unsigned char*)&id,sizeof(id),c,NULL);
}
```

`listAddNodeTail`函数的原型如下：

```C
list *listAddNodeTail(list *list, void *value)
```

显然在`linkClient`函数中，涉及了从`client *`到`void *`类型的转换

参见[Implicit conversions](https://en.cppreference.com/w/c/language/conversion)，其中指出：

> A pointer to void can be implicitly converted to and from any pointer to object type with the following semantics:
>
> - If a pointer to object is converted to a pointer to void and back, its value compares equal to the original pointer.
> - No other guarantees are offered

### C interfaces

正如cppreference `Pointer declaration#Pointers to objects#Pointers to void`中所总结的：

> Pointers to void are used to pass objects of unknown type, which is common in C interfaces:

下面是例子:

| example                                                      | explanation |
| ------------------------------------------------------------ | ----------- |
| `std::malloc`                                                |             |
| [pthread_create](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_create.html) |             |
| `std::qsort`                                                 |             |






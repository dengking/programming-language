# Void pointer

## [Pointers to void](https://en.cppreference.com/w/c/language/pointer)

Pointer to object of any type can be [implicitly converted](https://en.cppreference.com/w/c/language/conversion) to pointer to `void` (optionally [const](https://en.cppreference.com/w/c/language/const) or [volatile](https://en.cppreference.com/w/c/language/volatile)-qualified), and vice versa:

```C
int n=1, *p=&n;
void* pv = p; // int* to void*
int* p2 = pv; // void* to int*
printf("%d\n", *p2); // prints 1
```

Pointers to `void` are used to pass objects of unknown type, which is common in **generic interfaces**: [malloc](https://en.cppreference.com/w/c/memory/malloc) returns `void`, [qsort](https://en.cppreference.com/w/c/algorithm/qsort) expects a user-provided callback that accepts two `const void*` arguments. [pthread_create](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_create.html) expects a user-provided callback that accepts and returns `void*`. In all cases, it is the caller's responsibility to convert the pointer to the **correct type** before use.





## [What does void mean in C, C++, and C#?](https://stackoverflow.com/questions/1043034/what-does-void-mean-in-c-c-and-c)



### [A](https://stackoverflow.com/a/1043107)

Basically it means "nothing" or "no type"

There are 3 basic ways that void is used:

1. Function argument: `int myFunc(void)` -- the function takes nothing.

2. Function return value: `void myFunc(int)` -- the function returns nothing

3. Generic data pointer: `void* data` -- 'data' is a pointer to data of unknown type, and cannot be dereferenced

   > NOTE: 所以，it is the caller's responsibility to convert the pointer to the **correct type** before use.

Note: the `void` in a function argument is optional in C++, so `int myFunc()` is exactly the same as `int myFunc(void)`, and it is left out completely in C#. It is always required for a return value.



## Example

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

## Q&A

alignment如何保证？`void*`应该具备最大的alignment。



## [Why type cast a void pointer?](https://stackoverflow.com/questions/16986214/why-type-cast-a-void-pointer)
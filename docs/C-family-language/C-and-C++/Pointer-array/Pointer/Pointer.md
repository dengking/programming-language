# Pointer 



## Pointer and the stored-program machine

[Stored-program computer](https://en.wikipedia.org/wiki/Stored-program_computer) and [Universal Turing machine § Stored-program computer](https://en.wikipedia.org/wiki/Universal_Turing_machine#Stored-program_computer) 告诉我们，将program（function）和data（object）保存到memory中，这样我们就可以使用pointer来引用这些内容。关于这一点，在`C++\Language-reference\Basic-concept\index.md#object 和 function`段中也进行了阐述。

在[Function-pointer#从stored-program computer来理解function pointer](./Function-pointer.md)中也对这个问题进行了探讨。

### 两大类pointer

简化，OOP只是一种programming paradigm，在运行时，我们可以认为process仅仅由如下两类构成: 

- function (包括free function、member function)
- data （）

| pointer             | 说明                                                         |
| ------------------- | ------------------------------------------------------------ |
| pointer to object   | 在`C++\Language-reference\Basic-concept\Data-model\Object\Object.md`中，将次描述为“Pointer can alias to an existing object” |
| pointer to function | 在`C-family-language\C-and-C++\Pointer-array\Pointer\Function-pointer.md`中，对此进行了描述 |

C++支持OOP，因此相对于C而言，它的很多概念都需要向OOP扩展。

## wikipedia [Pointer (computer programming)](https://en.wikipedia.org/wiki/Pointer_(computer_programming))



## cppreference [Pointer declaration](https://en.cppreference.com/w/cpp/language/pointer)

### Pointers to objects

```c++
#include <iostream>

int main()
{
	int n;
	int *np = &n; // pointer to int
	int * const *npp = &np; // non-const pointer to const pointer to non-const int

	int a[2];
	int (*ap)[2] = &a; // pointer to array of int

	struct S
	{
		int n;
	} s = { 1 };
	int* sp = &s.n; // pointer to the int that is a member of s
}
// g++ --std=c++11 test.cpp

```



### Pointers to void

> NOTE: 这部分内容放到了下面的Pointer to void章节

### Pointers to functions

> NOTE: 在[Function-pointer](./Function-pointer.md)中进行介绍。

### Pointers to members



#### Pointers to data members



#### Pointers to member functions



### Null pointers



### Constness

| Syntax               | meaning                             | 说明                                                         |
| -------------------- | ----------------------------------- | ------------------------------------------------------------ |
| `const T* ptr`       | pointer to constant object          |                                                              |
| `T const* ptr`       | pointer to constant object          |                                                              |
| `T* const ptr`       | constant pointer to object          | 按照[这篇文章](https://stackoverflow.com/a/596750)中的看法: <br>A *reference* can be thought of as a *constant pointer* |
| `const T* const ptr` | constant pointer to constant object |                                                              |
| `T const* const ptr` | constant pointer to constant object |                                                              |

> NOTE: 原文，并没有添加variable name，不添加variable name是不易理解的，上面是我添加了variable name的描述。
>
> 在下面文章中，对这个问题进行了深入分析:
>
> - [What is the difference between char * const and const char *?](https://stackoverflow.com/questions/890535/what-is-the-difference-between-char-const-and-const-char)
> - [Constant pointer vs Pointer to constant [duplicate]](https://stackoverflow.com/questions/21476869/constant-pointer-vs-pointer-to-constant)
> - [What is the difference between const int*, const int * const, and int const *?](https://stackoverflow.com/questions/1143262/what-is-the-difference-between-const-int-const-int-const-and-int-const)
>
> 其实，使用 [Clockwise/Spiral Rule](http://c-faq.com/decl/spiral.anderson.html)， 我们可以非常快速地理解其语义，参见`C-and-C++\Declaration\The-Clockwise-Spiral-Rule.md`。

## cppreference [Pointer comparison operators](https://en.cppreference.com/w/cpp/language/operator_comparison#Pointer_comparison_operators)





## Multiple-level pointer

“multiple-level pointer”即“多级指针”；在cppreference [const_cast conversion](https://en.cppreference.com/w/cpp/language/const_cast)中，使用multilevel pointer来表达“多级指针”。

### [Why use double indirection? or Why use pointers to pointers?](https://stackoverflow.com/questions/5580761/why-use-double-indirection-or-why-use-pointers-to-pointers)

#### [A](https://stackoverflow.com/a/5580952): characters-word-sentence-monologue类比

If you want to have a list of characters (a word), you can use `char *word`

> NOTE: a word consists of many characters. 

If you want a list of words (a sentence), you can use `char **sentence`

If you want a list of sentences (a monologue), you can use `char ***monologue`

> NOTE: “monologue”的意思“独白”

If you want a list of monologues (a biography), you can use `char ****biography`

If you want a list of biographies (a bio-library), you can use `char *****biolibrary`

If you want a list of bio-libraries (a ??lol), you can use `char ******lol`

Usage example with a very very very boring *lol*

```c++
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * 统计在一个sentence中的word的个数
 * @param x
 * @return
 */
int wordsinsentence(char **x)
{
	int w = 0;
	while (*x)
	{
		w += 1;
		x++;
	}
	return w;
}
/**
 * 统计在一个monologue中的word的个数
 * @param x
 * @return
 */
int wordsinmono(char ***x)
{
	int w = 0;
	while (*x)
	{
		w += wordsinsentence(*x);
		x++;
	}
	return w;
}

int wordsinbio(char ****x)
{
	int w = 0;
	while (*x)
	{
		w += wordsinmono(*x);
		x++;
	}
	return w;
}

int wordsinlib(char *****x)
{
	int w = 0;
	while (*x)
	{
		w += wordsinbio(*x);
		x++;
	}
	return w;
}

int wordsinlol(char ******x)
{
	int w = 0;
	while (*x)
	{
		w += wordsinlib(*x);
		x++;
	}
	return w;
}

int main(void)
{
	char *word; // a pointer to char
	char **sentence; // a pointer to char *
	char ***monologue; // a pointer to char **
	char ****biography; // a pointer to char ***
	char *****biolibrary; // a pointer to char ****
	char ******lol; // a pointer to char *****

	//fill data structure
	word = (char*) malloc(4 * sizeof *word); // assume it worked
	strcpy(word, "foo");

	sentence = (char**) malloc(4 * sizeof *sentence); // assume it worked
	sentence[0] = word;
	sentence[1] = word;
	sentence[2] = word;
	sentence[3] = NULL;

	monologue = (char***) malloc(4 * sizeof *monologue); // assume it worked
	monologue[0] = sentence;
	monologue[1] = sentence;
	monologue[2] = sentence;
	monologue[3] = NULL;

	biography = (char****) malloc(4 * sizeof *biography); // assume it worked
	biography[0] = monologue;
	biography[1] = monologue;
	biography[2] = monologue;
	biography[3] = NULL;

	biolibrary = (char*****) malloc(4 * sizeof *biolibrary); // assume it worked
	biolibrary[0] = biography;
	biolibrary[1] = biography;
	biolibrary[2] = biography;
	biolibrary[3] = NULL;

	lol = (char******) malloc(4 * sizeof *lol); // assume it worked
	lol[0] = biolibrary;
	lol[1] = biolibrary;
	lol[2] = biolibrary;
	lol[3] = NULL;

	printf("total words in my lol: %d\n", wordsinlol(lol));

	free(lol);
	free(biolibrary);
	free(biography);
	free(monologue);
	free(sentence);
	free(word);
}
// gcc test.cpp
```

Output:

```
total words in my lol: 243
```

#### [A](https://stackoverflow.com/a/5580790): change the value of the pointer passed to a function as the function argument

One reason is you want to change the value of the pointer passed to a function as the **function argument**, to do this you require **pointer to a pointer**.

In simple words, Use `**` when you want to preserve (OR retain change in) the Memory-Allocation or Assignment even outside of a function call. (So, Pass such function with double pointer arg.)

> NOTE: 这个回答和下面的回答表达的含义是相同的；

This may not be a very good example, but will show you the basic use:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void allocate(int** p)
{
	*p = (int*) malloc(sizeof(int));
}

int main()
{
	int* p = NULL;
	allocate(&p);
	*p = 42;
	free(p);
}
// gcc test.cpp
```

#### [A](https://stackoverflow.com/a/30717999)

- Let’s say you have a pointer. Its value is an address.
- but now you want to change that address.
- you could. by doing `pointer1 = pointer2`, you give `pointer1` the address of `pointer2`.
- but! if you do that within a function, and you want the result to persist after the function is done, you need do some extra work. you need a new `pointer3` just to point to `pointer1`. pass `pointer3` to the function.
- here is an example. look at the output below first, to understand.

```c++
#include <stdio.h>
void cant_change(int * x, int * z);
void change(int ** x, int * z);
int main()
{

	int c = 1;
	int d = 2;
	int e = 3;
	int * a = &c;
	int * b = &d;
	int * f = &e;
	int ** pp = &a;  // pointer to pointer 'a'

	printf("\n a's value: %x \n", a);
	printf("\n b's value: %x \n", b);
	printf("\n f's value: %x \n", f);
	printf("\n can we change a?, lets see \n");
	printf("\n a = b \n");
	a = b;
	printf("\n a's value is now: %x, same as 'b'... it seems we can, but can we do it in a function? lets see... \n", a);
	printf("\n cant_change(a, f); \n");
	cant_change(a, f);
	printf("\n a's value is now: %x, Doh! same as 'b'...  that function tricked us. \n", a);

	printf("\n NOW! lets see if a pointer to a pointer solution can help us... remember that 'pp' point to 'a' \n");
	printf("\n change(pp, f); \n");
	change(pp, f);
	printf("\n a's value is now: %x, YEAH! same as 'f'...  that function ROCKS!!!. \n", a);
	return 0;
}

void cant_change(int * x, int * z)
{
	x = z;
	printf("\n ----> value of 'a' is: %x inside function, same as 'f', BUT will it be the same outside of this function? lets see\n", x);
}

void change(int ** x, int * z)
{
	*x = z;
	printf("\n ----> value of 'a' is: %x inside function, same as 'f', BUT will it be the same outside of this function? lets see\n", *x);
}
// gcc test.cpp
```

### [How many levels of pointers can we have?](https://stackoverflow.com/questions/10087113/how-many-levels-of-pointers-can-we-have)

#### [A](https://stackoverflow.com/a/10087264)

The `C` standard specifies the lower limit:

> ### [5.2.4.1 Translation limits](http://c0x.coding-guidelines.com/5.2.4.1.html)
>
> 276 The implementation shall be able to translate and execute at least one program that contains at least one instance of every one of the following limits: [...]
>
> 279 — 12 pointer, array, and function declarators (in any combinations) modifying an arithmetic, structure, union, or void type in a declaration

The upper limit is implementation specific.



### Examples

#### [pthread_exit(3)](https://www.man7.org/linux/man-pages/man3/pthread_exit.3.html) and [pthread_join(3)](https://www.man7.org/linux/man-pages/man3/pthread_join.3.html)

关于上述两个函数，以及它们的用法，参见`Programming\Process\Thread\Thread-termination.md`。

#### `argv`

[What does int argc, char *argv[] mean?](https://stackoverflow.com/questions/3024197/what-does-int-argc-char-argv-mean)

`argv` and `argc` are how command line arguments are passed to `main()` in C and C++.

`argc` will be the number of strings pointed to by `argv`. This will (in practice) be 1 plus the number of arguments, as virtually all implementations will prepend the name of the program to the array.

The variables are named `argc` (*argument count*) and `argv` (*argument vector*) by convention, but they can be given any valid identifier: `int main(int num_args, char** arg_strings)` is equally valid.

```c++
#include <iostream>

int main(int argc, char** argv)
{
	std::cout << "Have " << argc << " arguments:" << std::endl;
	for (int i = 0; i < argc; ++i)
	{
		std::cout << argv[i] << std::endl;
	}
}
// g++ test.cpp
```



## Pointer to `void` 

### Generic interface

正如creference [Pointers to void](https://en.cppreference.com/w/c/language/pointer)中所言：

> Pointers to `void` are used to pass objects of unknown type, which is common in **generic interfaces**

显然pointer to void是C中实现**generic programming**的方式，这种generic是通过**type erasure**来实现的：

- void是type-less、是incomplete type，programmer无法直接操作void pointer，必须要先将它[static_cast](static_cast.html) or [explicit cast](explicit_cast.html)为complete type才能够进行操作
- Pointer to object of any type can be [implicitly converted](implicit_cast.html) to pointer to `void`，在这个过程中，就丢失了type

### cppreference Pointer to `void`

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

[Why type cast a void pointer?](https://stackoverflow.com/questions/16986214/why-type-cast-a-void-pointer)



### void pointer and alignment

alignment如何保证？

错误观点:  `void*`应该具备最大的alignment，这样才能够保证它能够容纳下所有类型的pointer。

正确理解: `void*`类型的变量用于保存pointer的值，由于它是incomplete type，所以programmer无法直接对其进行操作（直接`+`、`*`一个`void*`，编译器会报错的，这样的program是无法编译通过的），programmer必须先将它转换为complete type pointer。由于无法直接使用`void*`，所以考虑考虑它的alignment是没有意义的。需要考虑的是：`void*`的值是否满足它的destination type的alignment，如果不满足，则是会发生undefined behavior的。

### Pointer to void VS pointer to `char` 

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



### Examples

#### Plugin system

```c++
int FUNCTION_CALL_MODE HQInit(void* lpInstance, PFOnSetTimeOut pfOnSetTimeOut)
{
	int iRet = I_NONE;
	if (lpInstance)
		iRet = ((CHQImpl*) lpInstance)->OnInit();
	return iRet;
}
```



#### redis linked list

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

#### C interfaces

正如cppreference `Pointer declaration#Pointers to objects#Pointers to void`中所总结的：

> Pointers to void are used to pass objects of unknown type, which is common in C interfaces:

下面是例子:

- `std::malloc`
- [pthread_create](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_create.html)
- `std::qsort`

## TO READ

https://dev.to/noah11012/double-pointers-in-cc-2n96



https://beginnersbook.com/2014/01/c-pointer-to-pointer/



https://www.geeksforgeeks.org/double-pointer-pointer-pointer-c/



https://stackoverflow.com/questions/36689025/creating-double-pointer




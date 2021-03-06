# Multiple-level pointer

1、“multiple-level pointer”即“多级指针”；在cppreference [const_cast conversion](https://en.cppreference.com/w/cpp/language/const_cast)中，使用multilevel pointer来表达“多级指针”。

2、"Multiple-level pointer"可以看做是pointer of pointer，即使用recursive definition来进行描述。

## stackoverflow [Why use double indirection? or Why use pointers to pointers?](https://stackoverflow.com/questions/5580761/why-use-double-indirection-or-why-use-pointers-to-pointers)

### [A](https://stackoverflow.com/a/5580952): characters-word-sentence-monologue类比

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

### [A](https://stackoverflow.com/a/5580790): change the value of the pointer passed to a function as the function argument

One reason is you want to change the value of the pointer passed to a function as the **function argument**, to do this you require **pointer to a pointer**.

> NOTE: 
> 1、recursive definition

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

### [A](https://stackoverflow.com/a/30717999)

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

## stackoverflow [How many levels of pointers can we have?](https://stackoverflow.com/questions/10087113/how-many-levels-of-pointers-can-we-have)

### [A](https://stackoverflow.com/a/10087264)

The `C` standard specifies the lower limit:

> ### [5.2.4.1 Translation limits](http://c0x.coding-guidelines.com/5.2.4.1.html)
>
> 276 The implementation shall be able to translate and execute at least one program that contains at least one instance of every one of the following limits: [...]
>
> 279 — 12 pointer, array, and function declarators (in any combinations) modifying an arithmetic, structure, union, or void type in a declaration

The upper limit is implementation specific.



## Examples

### [pthread_exit(3)](https://www.man7.org/linux/man-pages/man3/pthread_exit.3.html) and [pthread_join(3)](https://www.man7.org/linux/man-pages/man3/pthread_join.3.html)

关于上述两个函数，以及它们的用法，参见`Programming\Process\Thread\Thread-termination.md`。

### `argv`

stackoverflow [What does int argc, char *argv[] mean?](https://stackoverflow.com/questions/3024197/what-does-int-argc-char-argv-mean)

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

### SQLite [Virtual Table Methods](https://sqlite.org/vtab.html)

这就是前面所述的"change the value of the pointer passed to a function as the function argument"。

在外层的声明是:

```C++
sqlite3_vtab* pVTab
```

在下面的函数`xCreate`中则进行类似的写法:

```C++
*ppVTab = new sqlite3_vtab;
```



2.1. The `xCreate` Method

```C++
int (*xCreate)(sqlite3 *db, void *pAux, int argc, char *const*argv, sqlite3_vtab **ppVTab, char **pzErr);
```

2.2. The `xConnect` Method

```C
int (*xConnect)(sqlite3*, void *pAux, int argc, char *const*argv, sqlite3_vtab **ppVTab, char **pzErr);
```





## TO READ

https://dev.to/noah11012/double-pointers-in-cc-2n96



https://beginnersbook.com/2014/01/c-pointer-to-pointer/



https://www.geeksforgeeks.org/double-pointer-pointer-pointer-c/



https://stackoverflow.com/questions/36689025/creating-double-pointer
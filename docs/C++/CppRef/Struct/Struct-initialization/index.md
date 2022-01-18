# struct initialization



```C++
#include <iostream>
struct Test
{
	int a;
	int b;
};

int main()
{
	Test t;
	if (t.a != 0 || t.b != 0)
	{
		std::cout << "[" << __LINE__ << "]" << "indeterminate value 中间值" << std::endl;
	}
	Test t2 { };
	if (t2.a != 0 || t2.b != 0)
	{
		std::cout << "[" << __LINE__ << "]" << "indeterminate value 中间值" << std::endl;
	}
}
// g++ test.cpp --std=c++11 -pedantic -Wall -Wextra

```

输出如下:

```C++
[13]indeterminate value 中间值
```



## stackoverflow [Initializing a struct to 0](https://stackoverflow.com/questions/11152160/initializing-a-struct-to-0)



If I have a struct like this:

```c
typedef struct
{
    unsigned char c1;
    unsigned char c2;
} myStruct;
```

What would be the easiest way to initialize this struct to 0? Would the following suffice?

```c
myStruct _m1 = {0};
```

or Would I need to explicitly init each member to 0?

```c
myStruct _m2 = {0,0};
```



[A](https://stackoverflow.com/a/11152199)

The first is easiest(*involves less typing*), and it is guaranteed to work, all members will be set to `0`[Ref 1].
The second is more readable.



[A](https://stackoverflow.com/a/11152208)

If the data is a static or global variable, it is zero-filled by default, so just declare it `myStruct _m;`

If the data is a local variable or a heap-allocated zone, clear it with `memset` like:

```c
memset(&m, 0, sizeof(myStruct));
```

Current compilers (e.g. recent versions of `gcc`) optimize that quite well in practice. This works only if all zero values (include null pointers and floating point zero) are represented as all zero bits, which is true on all platforms I know about (but the *C* standard permits implementations where this is false; I know no such implementation).

You could perhaps code `myStruct m = {};` or `myStruct m = {0};` (even if the first member of `myStruct` is not a scalar).

My feeling is that using `memset` for local structures is the best, and it conveys better the fact that at runtime, something has to be done (while usually, global and static data can be understood as initialized at compile time, without any cost at runtime).



## creference [Struct and union initialization](https://en.cppreference.com/w/c/language/struct_initialization)

When [initializing](https://en.cppreference.com/w/c/language/initialization) an object of [struct](https://en.cppreference.com/w/c/language/struct) or [union](https://en.cppreference.com/w/c/language/union) type, the **initializer** must be a non-empty, brace-enclosed, comma-separated list of initializers for the members:

```
= { expression , ... }		(until C99)
= { designator(optional) expression , ... }		(since C99)

```





## TODO

[C and C++ : Partial initialization of automatic structure](https://stackoverflow.com/questions/10828294/c-and-c-partial-initialization-of-automatic-structure)
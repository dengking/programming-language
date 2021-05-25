# C array length

## stackoverflow [How do I use arrays in C++?](https://stackoverflow.com/questions/4810664/how-do-i-use-arrays-in-c) # [common pitfalls when using arrays](https://stackoverflow.com/questions/4810664/how-do-i-use-arrays-in-c/7439261#7439261)

### 5.3 Pitfall: Using the C idiom to get number of elements.

With deep C experience it’s natural to write …

```cpp
#define N_ITEMS( array )   (sizeof( array )/sizeof( array[0] ))
```

Since an `array` decays to pointer to first element where needed, the expression `sizeof(a)/sizeof(a[0])` can also be written as `sizeof(a)/sizeof(*a)`. It means the same, and no matter how it’s written it is the **C idiom** for finding the number elements of array.

Main pitfall: the C idiom is not typesafe. For example, the code …

```c++
#include <stdio.h>

#define N_ITEMS( array ) (sizeof( array )/sizeof( *array ))

void display(int const a[7])
{
	int const n = N_ITEMS(a);          // Oops.
	printf("%d elements.\n", n);
}

int main()
{
	int const moohaha[] = { 1, 2, 3, 4, 5, 6, 7 };

	printf("%d elements, calling display...\n", N_ITEMS(moohaha));
	display(moohaha);
}
// gcc test.c
```

> NOTE: 上述程序，传递array的方式为：Pass by pointer

> NOTE: 上述程序输出如下：
>
> ```
> 7 elements, calling display...
> 2 elements.
> 
> ```

1、The compiler rewrites `int const a[7]` to just `int const a[]`.

2、The compiler rewrites `int const a[]` to `int const* a`.

3、`N_ITEMS` is therefore invoked with a pointer.

4、For a 32-bit executable `sizeof(array)` (size of a pointer) is then 4.

5、`sizeof(*array)` is equivalent to `sizeof(int)`, which for a 32-bit executable is also 4.

> NOTE: 
>
> 1、关于这一点，在creference [sizeof](https://en.cppreference.com/w/c/language/sizeof)中进行了详细说明。
>
> 2、上述程序典型的说明了:CppCoreGuidelines [Philosophy](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#p-philosophy)  #  P.4: Ideally, a program should be statically type safe 中提及的"array decay"
>
> 3、一旦array decay to pointer，那么上述程序就无法使用了，因此，下面的detection就是detect array是否是pointer，`typeid( &*array )`是pointer，如果`typeid( array )`和它不相等，则没有发生decay，否则发生decay了。

In order to detect this error at **run time** you can do …

```c++
#include "assert.h"
#include "stdio.h"
#include <iostream>
#include <typeinfo>

#define N_ITEMS( array )       (                               \
    assert((                                                    \
        "N_ITEMS requires an actual array as argument",        \
        typeid( array ) != typeid( &*array )                    \
        )),                                                     \
    sizeof( array )/sizeof( *array )                            \
    )

void display(int const a[7])
{
	int const n = N_ITEMS(a);          // Oops.
	std::cout << typeid( a ).name() << std::endl;
	std::cout << typeid( &*a ).name() << std::endl;
	printf("%d elements.\n", n);
}

int main()
{
	int const moohaha[] = { 1, 2, 3, 4, 5, 6, 7 };
	printf("%d elements, calling display...\n", N_ITEMS(moohaha));
	display(moohaha);
}

```

> NOTE: 
>
> 1、关于`typeid` ，参见cppreference [`typeid`](https://en.cppreference.com/w/cpp/language/typeid)
>
> 上述实现中，如果`array`的实际类型是pointer，则`typeid( array ) == typeid( &*array )`，显然是不符合assertion的。
>
> 运行结果:
>
> ```c++
> 7 elements, calling display...
> a.out: test.cpp:18: void display(const int*): Assertion `( "N_ITEMS requires an actual array as argument", typeid( a ) != typeid( &*a ) )' failed.
> 已放弃(吐核)
> ```
>
> 2、上述典型的说明了: CppCoreGuidelines [P.6: What cannot be checked at compile time should be checkable at run time](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#p6-what-cannot-be-checked-at-compile-time-should-be-checkable-at-run-time)

The **runtime error detection** is better than **no detection**, but it wastes a little processor time, and perhaps much more programmer time. Better with **detection at compile time**! And if you're happy to not support arrays of local types with C++98, then you can do that:

```cpp
#include "stddef.h"
#include "stdio.h"
#include <iostream>
#include <typeinfo>

typedef ptrdiff_t   Size;

template< class Type, Size n >
Size n_items( Type (&)[n] ) { return n; }

#define N_ITEMS( array )       n_items( array )
void display(int const a[7])
{
	int const n = N_ITEMS(a);          // Oops.
	std::cout << typeid( a ).name() << std::endl;
	std::cout << typeid( &*a ).name() << std::endl;
	printf("%d elements.\n", n);
}

int main()
{
	int const moohaha[] = { 1, 2, 3, 4, 5, 6, 7 };
	printf("%d elements, calling display...\n", N_ITEMS(moohaha));
	display(moohaha);
}
// g++ --std=c++11 test.cpp
```

> NOTE: 上述程序编译报错如下:
>
> ```c#
> test.cpp: 在函数‘void display(const int*)’中:
> test.cpp:11:47: 错误：对‘n_items(const int*&)’的调用没有匹配的函数
> #define N_ITEMS( array )       n_items( array )
> 
> ```
>
> 

How it works: the array is passed *by reference* to `n_items`, and so it does not decay to pointer to first element, and the function can just return the number of elements specified by the type.

With C++11 you can use this also for arrays of local type, and it's the type safe **C++ idiom** for finding the number of elements of an array.

### 5.4 C++11 & C++14 pitfall: Using a `constexpr` array size function.

With C++11 and later it's natural, but as you'll see dangerous!, to replace the C++03 function

```cpp
typedef ptrdiff_t   Size;

template< class Type, Size n >
Size n_items( Type (&)[n] ) { return n; }
```

with

```cpp
using Size = ptrdiff_t;

template< class Type, Size n >
constexpr auto n_items( Type (&)[n] ) -> Size { return n; }
```

where the significant change is the use of `constexpr`, which allows this function to produce a **compile time constant**.

For example, in contrast to the C++03 function, such a compile time constant can be used to declare an array of the same size as another:

```c++
#include "stddef.h"
#include "stdio.h"
#include <iostream>
#include <typeinfo>

typedef ptrdiff_t Size;

using Size = ptrdiff_t;

template<class Type, Size n>
constexpr auto n_items(Type (&)[n]) -> Size
{
	return n;
}

// Example 1
void foo()
{
	int const x[] = { 3, 1, 4, 1, 5, 9, 2, 6, 5, 4 };
	constexpr Size n = n_items(x);
	int y[n] = { 3, 1, 4, 1, 5, 9, 2, 6, 5, 4 };
	for (auto&& i : y)
	{
		std::cout << i << std::endl;
	}
	// Using y here.
}

int main()
{
	foo();
}
// g++ --std=c++11 test.cpp

```

> NOTE: 上述程序输出:
>
> ```
> 42
> ```



## Get length of array in C and C++

### Length in term of element in the array

关于此，在“5.3 Pitfall: Using the C idiom to get number of elements.”中进行了详细说明；

### Length in term of byte

当我们使用c api来操作array的时候，需要使用length in term of byte，主要使用`sizeof`。


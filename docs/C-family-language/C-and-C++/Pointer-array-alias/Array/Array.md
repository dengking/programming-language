# Array

## stackoverflow [How do I use arrays in C++?](https://stackoverflow.com/questions/4810664/how-do-i-use-arrays-in-c)

### Arrays on the type level

> NOTE: 从type system的角度来分析array

An array type is denoted as `T[n]` where `T` is the *element type* and `n` is a positive *size*, the number of elements in the array. The array type is a product type of the element type and the size. If one or both of those ingredients differ, you get a distinct type:

```cpp
#include <type_traits>
int main()
{
	static_assert(!std::is_same<int[8], float[8]>::value, "distinct element type");
	static_assert(!std::is_same<int[8], int[9]>::value, "distinct size");
}
// g++ --std=c++11 test.cpp
```

Note that the size is part of the type, that is, array types of different size are **incompatible types** that have absolutely nothing to do with each other. `sizeof(T[n])` is equivalent to `n * sizeof(T)`.

```c++
int main()
{
	static_assert(sizeof(int[8])==8*sizeof(int), "same size");
}
// g++ --std=c++11 test.cpp
```

#### Array-to-pointer decay

The only "connection" between `T[n]` and `T[m]` is that both types can implicitly be *converted* to `T*`, and the result of this conversion is a pointer to the first element of the array. That is, anywhere a `T*` is required, you can provide a `T[n]`, and the compiler will silently provide that pointer:

```cpp
                  +---+---+---+---+---+---+---+---+
the_actual_array: |   |   |   |   |   |   |   |   |   int[8]
                  +---+---+---+---+---+---+---+---+
                    ^
                    |
                    |
                    |
                    |  pointer_to_the_first_element   int*
```

This conversion is known as "array-to-pointer decay", and it is a major source of confusion. The size of the array is lost in this process, since it is no longer part of the type (`T*`). 

Pro: Forgetting the size of an array on the type level allows a pointer to point to the first element of an array of *any* size. 

Con: Given a pointer to the first (or any other) element of an array, there is no way to detect how large that array is or where exactly the pointer points to relative to the bounds of the array. [Pointers are extremely stupid](https://stackoverflow.com/questions/4261074/).

#### Arrays are not pointers

The compiler will silently generate a pointer to the first element of an array whenever it is deemed useful, that is, whenever an operation would fail on an array but succeed on a pointer. This conversion from array to pointer is **trivial**, since the resulting pointer *value* is simply the address of the array. Note that the pointer is *not* stored as part of the array itself (or anywhere else in memory). **An array is not a pointer.**

```c++
#include <type_traits>

int main()
{
	static_assert(!std::is_same<int[8], int*>::value, "an array is not a pointer");
}
// g++ --std=c++11 test.cpp
```

One important context in which an array does *not* decay into a pointer to its first element is when the `&` operator is applied to it. In that case, the `&` operator yields a pointer to the *entire* array, not just a pointer to its **first element**. Although in that case the *values* (the addresses) are the same, a pointer to the first element of an array and a pointer to the entire array are completely distinct types:

```cpp
static_assert(!std::is_same<int*, int(*)[8]>::value, "distinct element type");
```



The following ASCII art explains this distinction:

```cpp
      +-----------------------------------+
      | +---+---+---+---+---+---+---+---+ |
+---> | |   |   |   |   |   |   |   |   | | int[8]
|     | +---+---+---+---+---+---+---+---+ |
|     +---^-------------------------------+
|         |
|         |
|         |
|         |  pointer_to_the_first_element   int*
|
|  pointer_to_the_entire_array              int(*)[8]
```

Note how the pointer to the first element only points to a single integer (depicted as a small box), whereas the pointer to the entire array points to an array of 8 integers (depicted as a large box).

The same situation arises in classes and is maybe more obvious. A pointer to an object and a pointer to its first data member have the same *value* (the same address), yet they are completely distinct types.

> NOTE: 虽然value相同，但是type不同，这就决定了对两者的不同操作的结果的不同，比如`++`。

If you are unfamiliar with the C declarator syntax, the parenthesis in the type `int(*)[8]` are essential:

- `int(*)[8]` is a pointer to an array of 8 integers.

  > NOTE: `int(*ptr)[8]`

- `int*[8]` is an array of 8 pointers, each element of type `int*`.

  > NOTE: `int* array[8]`

### Accessing elements

C++ provides two syntactic variations to access individual elements of an array. Neither of them is superior to the other, and you should familiarize yourself with both.

#### Pointer arithmetic

Given a pointer `p` to the first element of an array, the expression `p+i` yields a pointer to the i-th element of the array. By dereferencing that pointer afterwards, one can access individual elements:

```cpp
#include <iostream>

int main()
{
	int x[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	std::cout << *(x + 3) << ", " << *(x + 7) << std::endl;
}
// g++ --std=c++11 test.cpp

```

If `x` denotes an *array*, then array-to-pointer decay will kick in, because adding an array and an integer is meaningless (there is no plus operation on arrays), but adding a pointer and an integer makes sense:

```cpp
   +---+---+---+---+---+---+---+---+
x: |   |   |   |   |   |   |   |   |   int[8]
   +---+---+---+---+---+---+---+---+
     ^           ^               ^
     |           |               |
     |           |               |
     |           |               |
x+0  |      x+3  |          x+7  |     int*
```

(Note that the implicitly generated pointer has no name, so I wrote `x+0` in order to identify it.)

If, on the other hand, `x` denotes a *pointer* to the first (or any other) element of an array, then **array-to-pointer decay** is not necessary, because the pointer on which `i` is going to be added already exists:

```cpp
   +---+---+---+---+---+---+---+---+
   |   |   |   |   |   |   |   |   |   int[8]
   +---+---+---+---+---+---+---+---+
     ^           ^               ^
     |           |               |
     |           |               |
   +-|-+         |               |
x: | | |    x+3  |          x+7  |     int*
   +---+
```

Note that in the depicted case, `x` is a pointer *variable* (discernible by the small box next to `x`), but it could just as well be the result of a function returning a pointer (or any other expression of type `T*`).

#### Indexing operator

Since the syntax `*(x+i)` is a bit clumsy, C++ provides the alternative syntax `x[i]`:

```cpp
std::cout << x[3] << ", " << x[7] << std::endl;
```

Due to the fact that addition is commutative, the following code does exactly the same:

```cpp
std::cout << 3[x] << ", " << 7[x] << std::endl;
```

The definition of the indexing operator leads to the following interesting equivalence:

```cpp
#include <iostream>

int main()
{
	int x[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	int i = 3;
	bool equal = &x[i] == &*(x + i) == (x + i);
	std::cout << std::boolalpha;
	std::cout << equal << std::endl;
}
// g++ --std=c++11 test.cpp

```

> NOTE: 上诉程序编译是会报错的：
>
> ```c++
> test.cpp: 在函数‘int main()’中:
> test.cpp:7:43: 错误：ISO C++ 不允许比较指针和整数的值 [-fpermissive]
>   bool equal = &x[i] == &*(x + i) == (x + i);
> ```
>
> 问题原因在于对`==`的错误使用，`&x[i] == &*(x + i) == (x + i)`等价于:
>
> ```c++
> bool equal = &x[i] == &*(x + i);
> equal = equal == (x + i);
> ```
>
> 显然，`equal == (x + i);`就触发了“比较指针和整数的值”，则导致上述编译报错。修改后的版本如下：
>
> ```c++
> #include <iostream>
> 
> int main()
> {
> 	int x[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
> 	int i = 3;
> 	bool equal = &x[i] == &*(x + i) && &*(x + i) == (x + i);
> 	std::cout << std::boolalpha;
> 	std::cout << equal << std::endl;
> }
> // g++ --std=c++11 test.cpp
> 
> ```
>
> 输出:
>
> ```c++
> true
> ```



However, `&x[0]` is generally *not* equivalent to `x`. The former is a pointer, the latter an array. Only when the context triggers array-to-pointer decay can `x` and `&x[0]` be used interchangeably. For example:

```cpp
T* p = &array[0];  // rewritten as &*(array+0), decay happens due to the addition
T* q = array;      // decay happens due to the assignment
```

On the first line, the compiler detects an assignment from a pointer to a pointer, which trivially succeeds. On the second line, it detects an assignment from an *array* to a pointer. Since this is meaningless (but *pointer* to pointer assignment makes sense), array-to-pointer decay kicks in as usual.

#### Ranges

An array of type `T[n]` has `n` elements, indexed from `0` to `n-1`; there is no element `n`. And yet, to support **half-open ranges** (where the beginning is *inclusive* and the end is *exclusive*), C++ allows the computation of a pointer to the (non-existent) n-th element, but it is illegal to **dereference** that pointer:

```cpp
   +---+---+---+---+---+---+---+---+....
x: |   |   |   |   |   |   |   |   |   .   int[8]
   +---+---+---+---+---+---+---+---+....
     ^                               ^
     |                               |
     |                               |
     |                               |
x+0  |                          x+8  |     int*
```

For example, if you want to sort an array, both of the following would work equally well:

```cpp
#include <algorithm>
#include <iostream>

int main()
{
	int x[] = { 8, 7, 6, 5, 4, 3, 2, 1 };
	int n = sizeof(x) / sizeof(int);
	std::cout << "length of array:" << n << std::endl;
	std::sort(x + 0, x + n);
	for (auto&& i : x)
	{
		std::cout << i << std::endl;
	}
	int y[] = { 8, 7, 6, 5, 4, 3, 2, 1 };
	std::sort(&y[0], &y[0] + n);
	for (auto&& i : x)
	{
		std::cout << i << std::endl;
	}
}
// g++ --std=c++11 test.cpp

```

Note that it is illegal to provide `&x[n]` as the second argument since this is equivalent to `&*(x+n)`, and the sub-expression `*(x+n)` technically invokes [undefined behavior](https://stackoverflow.com/questions/3144904/) in C++ (but not in C99).

Also note that you could simply provide `x` as the first argument. That is a little too terse（简洁） for my taste, and it also makes template argument deduction a bit harder for the compiler, because in that case the first argument is an array but the second argument is a pointer. (Again, array-to-pointer decay kicks in.)

### Multidimensional arrays

Most programmers are familiar with named multidimensional arrays, but many are unaware of the fact that **multidimensional array** can also be created anonymously. Multidimensional arrays are often referred to as "arrays of arrays" or "*true* multidimensional arrays".

#### Named multidimensional arrays

When using named multidimensional arrays, *all* dimensions must be known at compile time:

```c++
#include <algorithm>
#include <iostream>
int read_int()
{
	int i = 0;
	std::cin >> i;
	return i;
}
int main()
{
//	int H = read_int();
//	int W = read_int();

	int connect_four[6][7];   // okay

//	int connect_four[H][7];   // ISO C++ forbids variable length array
//	int connect_four[6][W];   // ISO C++ forbids variable length array
//	int connect_four[H][W];   // ISO C++ forbids variable length array
}
// g++ --std=c++11 test.cpp

```

This is how a named multidimensional array looks like in memory:

```cpp
              +---+---+---+---+---+---+---+
connect_four: |   |   |   |   |   |   |   |
              +---+---+---+---+---+---+---+
              |   |   |   |   |   |   |   |
              +---+---+---+---+---+---+---+
              |   |   |   |   |   |   |   |
              +---+---+---+---+---+---+---+
              |   |   |   |   |   |   |   |
              +---+---+---+---+---+---+---+
              |   |   |   |   |   |   |   |
              +---+---+---+---+---+---+---+
              |   |   |   |   |   |   |   |
              +---+---+---+---+---+---+---+
```

Note that 2D grids such as the above are merely helpful visualizations. From the point of view of C++, memory is a "flat" sequence of bytes. The elements of a multidimensional array are stored in row-major order. That is, `connect_four[0][6]` and `connect_four[1][0]` are neighbors in memory. In fact, `connect_four[0][7]` and `connect_four[1][0]` denote the same element! This means that you can take multi-dimensional arrays and treat them as large, one-dimensional arrays:

```cpp
int* p = &connect_four[0][0];
int* q = p + 42;
some_int_sequence_algorithm(p, q);
```



#### Anonymous multidimensional arrays

With anonymous multidimensional arrays, all dimensions *except the first* must be known at compile time:

```c++
#include <algorithm>
#include <iostream>
int main()
{
	int H = 6;
	int W = 7;

	int (*p)[7] = new int[6][7];   // okay
	int(*q)[7] = new int[H][7];   // okay

	// int(*p)[W] = new int[6][W];   // ISO C++ forbids variable length array
	// int(*p)[W] = new int[H][W];   // ISO C++ forbids variable length array
}
// g++ --std=c++11 test.cpp

```

> NOTE: 
>
> `int (*p)[7] = new int[6][7];` `p` is a pointer to array

This is how an anonymous multidimensional array looks like in memory:

```cpp
              +---+---+---+---+---+---+---+
        +---> |   |   |   |   |   |   |   |
        |     +---+---+---+---+---+---+---+
        |     |   |   |   |   |   |   |   |
        |     +---+---+---+---+---+---+---+
        |     |   |   |   |   |   |   |   |
        |     +---+---+---+---+---+---+---+
        |     |   |   |   |   |   |   |   |
        |     +---+---+---+---+---+---+---+
        |     |   |   |   |   |   |   |   |
        |     +---+---+---+---+---+---+---+
        |     |   |   |   |   |   |   |   |
        |     +---+---+---+---+---+---+---+
        |
      +-|-+
   p: | | |
      +---+
```

Note that the array itself is still allocated as a single block in memory.

### Named arrays of pointers

Here is a named array of five pointers which are initialized with anonymous arrays of different lengths:

```c++
#include <algorithm>
#include <iostream>
int main()
{
	int* triangle[5];
	for (int i = 0; i < 5; ++i)
	{
		triangle[i] = new int[5 - i];
	}

	// ...

	for (int i = 0; i < 5; ++i)
	{
		delete[] triangle[i];
	}
}
// g++ --std=c++11 test.cpp

```

And here is how it looks like in memory:

```cpp
          +---+---+---+---+---+
          |   |   |   |   |   |
          +---+---+---+---+---+
            ^
            | +---+---+---+---+
            | |   |   |   |   |
            | +---+---+---+---+
            |   ^
            |   | +---+---+---+
            |   | |   |   |   |
            |   | +---+---+---+
            |   |   ^
            |   |   | +---+---+
            |   |   | |   |   |
            |   |   | +---+---+
            |   |   |   ^
            |   |   |   | +---+
            |   |   |   | |   |
            |   |   |   | +---+
            |   |   |   |   ^
            |   |   |   |   |
            |   |   |   |   |
          +-|-+-|-+-|-+-|-+-|-+
triangle: | | | | | | | | | | |
          +---+---+---+---+---+
```

Since each line is allocated individually now, viewing 2D arrays as 1D arrays does not work anymore.

### Anonymous arrays of pointers

Here is an anonymous array of 5 (or any other number of) pointers which are initialized with anonymous arrays of different lengths:

```c++
#include <algorithm>
#include <iostream>
int calculate_five()
{
	int i = 5;
	return i;
}
int main()
{
	int n = calculate_five();   // or any other number
	int** p = new int*[n];
	for (int i = 0; i < n; ++i)
	{
		p[i] = new int[n - i];
	}

	// ...

	for (int i = 0; i < n; ++i)
	{
		delete[] p[i];
	}
	delete[] p;   // note the extra delete[] !
}
// g++ --std=c++11 test.cpp

```

And here is how it looks like in memory:

```cpp
          +---+---+---+---+---+
          |   |   |   |   |   |
          +---+---+---+---+---+
            ^
            | +---+---+---+---+
            | |   |   |   |   |
            | +---+---+---+---+
            |   ^
            |   | +---+---+---+
            |   | |   |   |   |
            |   | +---+---+---+
            |   |   ^
            |   |   | +---+---+
            |   |   | |   |   |
            |   |   | +---+---+
            |   |   |   ^
            |   |   |   | +---+
            |   |   |   | |   |
            |   |   |   | +---+
            |   |   |   |   ^
            |   |   |   |   |
            |   |   |   |   |
          +-|-+-|-+-|-+-|-+-|-+
          | | | | | | | | | | |
          +---+---+---+---+---+
            ^
            |
            |
          +-|-+
       p: | | |
          +---+
```

### Conversions

**Array-to-pointer decay** naturally extends to arrays of arrays and arrays of pointers:

```c++
#include <algorithm>
#include <iostream>
int main()
{
	int array_of_arrays[6][7];
	int (*pointer_to_array)[7] = array_of_arrays;

	int* array_of_pointers[6];
	int** pointer_to_pointer = array_of_pointers;
}
// g++ --std=c++11 test.cpp

```



## pointer and array

一维array会decay to 一级pointer，显然两者能够进行对应；static array vs dynamic array，其实dynamic array就是一个pointer；

因为array和memory的结构的相同：都是sequence，这就导致了两者的类似；

array和pointer有着相同的arithmetic；

上述两个相似点，导致了pointer和array的很多地方的相似；

Multidimensional array和multiple-level pointer有着对应关系的；可以使用一个multiple-level pointer来访问一个multidimensional array，典型的例子：

- `char **argv`和`char* argv[]`
- 



Multidimensional array和multiple-level pointer都可以使用containing关系来进行理解；





n-dimensional array、array of pointer、multiple-degree and n-dimensional array

https://stackoverflow.com/questions/4810664/how-do-i-use-arrays-in-c?noredirect=1&lq=1



## Multidimensional arrays

Multidimensional arrays是典型的containing关系，我们是可以使用recursion来对它进行计算的；

[n-dimensional c++ array. How`s that possible?](https://stackoverflow.com/questions/11583747/n-dimensional-c-array-hows-that-possible)

I find a library analogy very useful to visualizing arrays of multiple dimensions:

- 8-dimensional array is a library
- 7-dimensional array is a floor in a library
- 6-dimensional array is a room on a floor in a library
- 5-dimensional array is a bookcase in a room on a floor in a library
- 4-dimensional array is a shelf in a bookcase in a room on a floor in a library
- 3-dimensional array is a book on a shelf in a bookcase in a room on a floor in a library
- 2-dimensional array is a page in a book on a shelf in a bookcase in a room on a floor in a library
- 1-dimensional array is a line on a page in a book on a shelf in a bookcase in a room on a floor in a library
- 0-dimensional array is a character in a line on a page in a book on a shelf in a bookcase in a room on a floor in a library



## [Get length of array in C and C++](https://iq.opengenus.org/length-of-array-in-c/)



## [With arrays, why is it the case that a[5] == 5[a]?](https://stackoverflow.com/questions/381542/with-arrays-why-is-it-the-case-that-a5-5a)

As Joel points out in [Stack Overflow podcast #34](https://stackoverflow.blog/2008/12/18/podcast-34/), in [C Programming Language](https://rads.stackoverflow.com/amzn/click/com/0131103628) (aka: K & R), there is mention of this property of arrays in C: `a[5] == 5[a]`

Joel says that it's because of pointer arithmetic but I still don't understand. **Why does `a[5] == 5[a]`**?



### [A](https://stackoverflow.com/a/381549)

The C standard defines the `[]` operator as follows:

```
a[b] == *(a + b)
```

Therefore `a[5]` will evaluate to:

```c
*(a + 5)
```

and `5[a]` will evaluate to:

```c
*(5 + a)
```

`a` is a pointer to the first element of the array. `a[5]` is the value that's 5 **elements** further from `a`, which is the same as `*(a + 5)`, and from elementary school math we know those are equal (addition is [commutative](https://en.wikipedia.org/wiki/commutative)).

### [A](https://stackoverflow.com/a/381554)

```c++
#include <iostream>

int main(int argc, char** argv)
{
	std::cout << std::boolalpha << std::endl;
	bool Equal = "ABCD"[2] == 2["ABCD"];
	std::cout << Equal << std::endl;
	Equal = 2["ABCD"] == 'C';
	std::cout << Equal << std::endl;
	Equal = "ABCD"[2] == 'C';
	std::cout << Equal << std::endl;
}

```


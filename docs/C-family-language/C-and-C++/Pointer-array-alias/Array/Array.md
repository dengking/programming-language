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

#### Accessing elements

C++ provides two syntactic variations to access individual elements of an array. Neither of them is superior to the other, and you should familiarize yourself with both.

##### Pointer arithmetic

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

##### Indexing operator

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

##### Actual memory layout

Note that 2D grids such as the above are merely helpful visualizations. From the point of view of C++, memory is a "flat" sequence of bytes. The elements of a multidimensional array are stored in row-major order. That is, `connect_four[0][6]` and `connect_four[1][0]` are neighbors in memory. In fact, `connect_four[0][7]` and `connect_four[1][0]` denote the same element! This means that you can take multi-dimensional arrays and treat them as large, one-dimensional arrays:

```cpp
int* p = &connect_four[0][0];
int* q = p + 42;
some_int_sequence_algorithm(p, q);
```



```c++
#include <algorithm>
#include <iostream>
void print_array(int *arr, int length)
{
	for (int i = 0; i < length; ++i)
	{
		std::cout << arr[i] << " ";
	}
	std::cout << std::endl;
}
int main()
{
	const int row = 2;
	const int col = 2;
	int connect_four[row][col] = { { 4, 3 }, { 2, 1 } };
	int* p = &connect_four[0][0];
	int* q = p + row * col;
	std::sort(p, q);
	print_array(p, row * col);
}
// g++ --std=c++11 test.cpp

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

#### Named arrays of pointers

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

#### Anonymous arrays of pointers

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

#### Conversions

##### Array-to-pointer decay

**Array-to-pointer decay** naturally extends to **arrays of arrays** and **arrays of pointers**:

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

> NOTE: `pointer_to_array`是一个指向array的pointer；`array_of_pointers`是一个array，元素的类型是`int*`。
>
> 对于`array_of_arrays`，经过**array-to-pointer decay**，得到的是pointer to arrays，这个pointer的类型是`int (*)[7]`；
>
> 对于`array_of_pointers`，经过**array-to-pointer decay**，得到的是pointer to pointer，即double pointer；
>
> 上面的这个例子是非常经典的例子。
>
> ### Pointer to pointer to int array
>
> ```c++
> #include <algorithm>
> #include <iostream>
> void print_array(int **arr, int row, int col)
> {
> 	for (int i = 0; i < row; ++i)
> 	{
> 		for (int j = 0; j < col; ++i)
> 		{
> 			std::cout << arr[i][j] << std::endl;
> 		}
> 	}
> }
> int main()
> {
> 	const int row = 2;
> 	const int col = 2;
> 	int array_of_arrays[row][col] = { { 1, 2 }, { 3, 3 } };
> 	int (*pointer_to_array)[col] = array_of_arrays;
> 	int** pointer_to_pointer = &pointer_to_array;
> 	print_array(pointer_to_pointer, 2, 2);
> }
> // g++ --std=c++11 test.cpp
> ```
>
> 上述程序的编译报错如下：
>
> ```c++
> test.cpp: 在函数‘int main()’中:
> test.cpp:19:30: 错误：不能在初始化时将‘int (**)[2]’转换为‘int**’
>   int** pointer_to_pointer = &pointer_to_array;
> ```
>
> 上述编译报错给我们的提示是：`&pointer_to_array`的类型是`int (**)[2]`。

##### Two dimension array to `**pointer`

However, there is no **implicit conversion** from `T[h][w]` to `T**`. If such an **implicit conversion** did exist, the result would be a pointer to the first element of an array of `h` pointers to `T` (each pointing to the first element of a line in the original 2D array), but that pointer array does not exist anywhere in memory yet. If you want such a **conversion**, you must create and fill the required pointer array manually:

```c++
#include <algorithm>
#include <iostream>
void print_array(int **arr, int row, int col)
{
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			std::cout << arr[i][j] << " ";
		}
		std::cout << std::endl;
	}
}
int main()
{
	const int row = 2;
	const int col = 2;
	int connect_four[row][col] = { { 1, 2 }, { 3, 4 } };

	int** p = new int*[row];
	for (int i = 0; i < row; ++i)
	{
		p[i] = connect_four[i];
	}
	// ...

	print_array(p, row, col);
	delete[] p;
}
// g++ --std=c++11 test.cpp

```

> NOTE: 上述程序的输出如下:
>
> ```
> 1 2 
> 3 4 
> ```

Note that this generates a view of the original multidimensional array. If you need a copy instead, you must create extra arrays and copy the data yourself:

```c++
#include <algorithm>
#include <iostream>
void print_array(int **arr, int row, int col)
{
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			std::cout << arr[i][j] << " ";
		}
		std::cout << std::endl;
	}
}
int main()
{
	const int row = 2;
	const int col = 2;
	int connect_four[row][col] = { { 1, 2 }, { 3, 4 } };

	int** p = new int*[row];
	for (int i = 0; i < col; ++i)
	{
		p[i] = new int[7];
		std::copy(connect_four[i], connect_four[i + 1], p[i]);
	}

	// ...
	print_array(p, row, col);
	for (int i = 0; i < row; ++i)
	{
		delete[] p[i];
	}
	delete[] p;
}
// g++ --std=c++11 test.cpp

```



> NOTE: 下面是一些补充内容：
>
> [How to assign two dimension array to pointer](https://stackoverflow.com/questions/10165627/how-to-assign-two-dimensional-array-to-pointer)
>
> [conversion of 2D array to pointer-to-pointer](https://stackoverflow.com/questions/8203700/conversion-of-2d-array-to-pointer-to-pointer)



> ### NOTE: Passing multiple-dimensional array to function
>
> 如何passing multiple-dimensional array to function？有多种方式，下面以print multiple-dimensional array为例来进行说明
>
> #### Pass as pointer
>
> 正如在“Actual memory layout”中所述：
>
> “you can take multi-dimensional arrays and treat them as large, one-dimensional arrays”
>
> ```c++
> #include <algorithm>
> #include <iostream>
> void print_array(int *arr, int length)
> {
> 	for (int i = 0; i < length; ++i)
> 	{
> 		std::cout << arr[i] << " ";
> 	}
> 	std::cout << std::endl;
> }
> int main()
> {
> 	const int row = 2;
> 	const int col = 2;
> 	int connect_four[row][col] = { { 4, 3 }, { 2, 1 } };
> 	int* p = &connect_four[0][0];
> 	int* q = p + row * col;
> 	std::sort(p, q);
> 	print_array(p, row * col);
> }
> // g++ --std=c++11 test.cpp
> 
> ```
>
> 
>
> #### Pass as pointer to pointer
>
> 采用“Two dimension array to `**pointer`”中描述的方法。



### Assignment

For no particular reason, arrays cannot be assigned to one another. Use `std::copy` instead:

```c++
#include <algorithm>
#include <iostream>
void print_array(int *arr, int length)
{
	for (int i = 0; i < length; ++i)
	{
		std::cout << arr[i] << " ";
	}
	std::cout << std::endl;
}
int main()
{

	int a[8] = { 2, 3, 5, 7, 11, 13, 17, 19 };
	int b[8];
	std::copy(a + 0, a + 8, b);
	print_array(b, 8);
}
// g++ --std=c++11 test.cpp

```

This is more flexible than what true array assignment could provide because it is possible to copy slices of larger arrays into smaller arrays. `std::copy` is usually specialized for primitive types to give maximum performance. It is unlikely that `std::memcpy` performs better. If in doubt, measure.

Although you cannot assign arrays directly, you *can* assign structs and classes which *contain* array members. That is because [array members are copied memberwise](https://stackoverflow.com/questions/4164279/) by the assignment operator which is provided as a default by the compiler. If you define the assignment operator manually for your own struct or class types, you must fall back to manual copying for the array members.

#### Parameter passing

Arrays cannot be passed by value. You can either pass them by pointer or by reference.

##### Pass by pointer

Since arrays themselves cannot be passed by value, usually a pointer to their first element is passed by value instead. This is often called "**pass by pointer**". Since the size of the array is not retrievable via that pointer, you have to pass a second parameter indicating the size of the array (the classic C solution) or a second pointer pointing after the last element of the array (the C++ iterator solution):

```c++
#include <algorithm>
#include <iostream>
#include <numeric>
#include <cstddef>

void print_array(int *arr, int length)
{
	for (int i = 0; i < length; ++i)
	{
		std::cout << arr[i] << " ";
	}
	std::cout << std::endl;
}

int sum(const int* p, std::size_t n)
{
	return std::accumulate(p, p + n, 0);
}

int sum(const int* p, const int* q)
{
	return std::accumulate(p, q, 0);
}

int main()
{
	const int row = 2;
	const int col = 2;
	int connect_four[row][col] = { { 4, 3 }, { 2, 1 } };
	int* p = &connect_four[0][0];
	int* q = p + row * col;
	std::cout << sum(p, q) << std::endl;
	std::cout << sum(p, row * col) << std::endl;
}
// g++ --std=c++11 test.cpp

```

As a syntactic alternative, you can also declare parameters as `T p[]`, and it means the exact same thing as `T* p` **in the context of parameter lists only**:

```cpp
int sum(const int p[], std::size_t n)
{
    return std::accumulate(p, p + n, 0);
}
```

You can think of the compiler as rewriting `T p[]` to `T *p` **in the context of parameter lists only**. This special rule is partly responsible for the whole confusion about arrays and pointers. In every other context, declaring something as an array or as a pointer makes a *huge* difference.

Unfortunately, you can also provide a size in an array parameter which is silently ignored by the compiler. That is, the following three signatures are exactly equivalent, as indicated by the compiler errors:

```cpp
int sum(const int* p, std::size_t n)

// error: redefinition of 'int sum(const int*, size_t)'
int sum(const int p[], std::size_t n)

// error: redefinition of 'int sum(const int*, size_t)'
int sum(const int p[8], std::size_t n)   // the 8 has no meaning here
```



##### Pass by reference

Arrays can also be passed by reference:

```cpp
int sum(const int (&a)[8])
{
    return std::accumulate(a + 0, a + 8, 0);
}
```

In this case, the array size is significant. Since writing a function that only accepts arrays of exactly 8 elements is of little use, programmers usually write such functions as templates:

```cpp
#include <algorithm>
#include <iostream>
#include <numeric>
#include <cstddef>

void print_array(int *arr, int length)
{
	for (int i = 0; i < length; ++i)
	{
		std::cout << arr[i] << " ";
	}
	std::cout << std::endl;
}

template<std::size_t n>
int sum(const int (&a)[n])
{
	return std::accumulate(a + 0, a + n, 0);
}

int main()
{
	int connect_four[] = { 4, 3, 2, 1 };
	std::cout << sum(connect_four) << std::endl;
}
// g++ --std=c++11 test.cpp

```

Note that you can only call such a function template with an actual array of integers, not with a pointer to an integer. The size of the array is automatically inferred, and for every size `n`, a different function is instantiated from the template. You can also write [quite useful](https://stackoverflow.com/questions/4759078/) function templates that abstract from both the element type and from the size.



### 5. Common pitfalls when using arrays.

#### 5.1 Pitfall: Trusting type-unsafe linking.

OK, you’ve been told, or have found out yourself, that globals (namespace scope variables that can be accessed outside the translation unit) are Evil™. But did you know how truly Evil™ they are? Consider the program below, consisting of two files [main.cpp] and [numbers.cpp]:

```cpp
// [main.cpp]
#include <iostream>

extern int* numbers;

int main()
{
    using namespace std;
    for( int i = 0;  i < 42;  ++i )
    {
        cout << (i > 0? ", " : "") << numbers[i];
    }
    cout << endl;
}
```



```cpp
// [numbers.cpp]
int numbers[42] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
```

> NOTE: 编译: `g++ main.cpp numbers.cpp -g -o main`

In Windows 7 this compiles and links fine with both MinGW g++ 4.4.1 and Visual C++ 10.0.

Since the types don't match, the program crashes when you run it.

> NOTE: 这违背了strict aliasing。

In-the-formal explanation: the program has **Undefined Behavior** (UB), and instead of crashing it can therefore just hang, or perhaps do nothing, or it can send threating e-mails to the presidents of the USA, Russia, India, China and Switzerland, and make Nasal Daemons fly out of your nose.

In-practice explanation: in `main.cpp` the array is treated as a pointer, placed at the same address as the array. For 32-bit executable this means that the first `int` value in the array, is treated as a pointer. I.e., in `main.cpp` the `numbers` variable contains, or appears to contain, `(int*)1`. This causes the program to access memory down at very bottom of the address space, which is conventionally reserved and trap-causing. Result: you get a crash.

> NOTE: 在`main.cpp`中，`numbers`的类型是`int *`，那按理来说`numbers[i]`仅仅是访问成员，而并不会去dereference成员的值，所以应该不存在“access memory down at very bottom of the address space”。所以我觉得上述解释应该是错误的。下面是改编的程序，它将array和array中的内容打印出来：
>
> ```c++
> // [main.cpp]
> #include <iostream>
> 
> extern int* numbers;
> void print_array();
> 
> int main()
> {
> 	print_array();
> 	using namespace std;
> 	std::cout << numbers << std::endl;
> 
> 	for (int i = 0; i < 9; ++i)
> 	{
> 		cout << (i > 0 ? ", " : "") << (numbers + i * sizeof(int));
> 	}
> 	cout << endl;
> }
> 
> ```
>
> 
>
> ```c++
> // [numbers.cpp]
> #include <iostream>
> const int length = 9;
> int numbers[length] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
> 
> void print_array()
> {
> 	std::cout << numbers << std::endl;
> 	for (int i = 0; i < length; ++i)
> 	{
> 		std::cout << numbers + i << std::endl;
> 		std::cout << numbers[i] << std::endl;
> 	}
> }
> 
> ```
>
> 编译: `g++ main.cpp numbers.cpp -g -o main`
>
> 在linux下的运行结果如下：
>
> ```c++
> 0x6010a0
> 0x6010a0
> 1
> 0x6010a4
> 2
> 0x6010a8
> 3
> 0x6010ac
> 4
> 0x6010b0
> 5
> 0x6010b4
> 6
> 0x6010b8
> 7
> 0x6010bc
> 8
> 0x6010c0
> 9
> 0x200000001
> 0x200000001, 0x200000011, 0x200000021, 0x200000031, 0x200000041, 0x200000051, 0x200000061, 0x200000071, 0x200000081
> ```
>
> 通过上述运行结果可以看出，`main.cpp`中访问的`numbers`和`numbers.cpp`中的number的内存地址不一致，显然这是导致程序core dump的原因。

The compilers are fully within their rights to not diagnose this error, because C++11 §3.5/10 says, about the requirement of compatible types for the declarations,

> **[N3290 §3.5/10]**
> A violation of this rule on type identity does not require a diagnostic.

The same paragraph details the variation that is allowed:

> … declarations for an array object can specify array types that differ by the presence or absence of a major array bound (8.3.4).

This allowed variation does not include declaring a name as an array in one translation unit, and as a pointer in another **translation unit**.

#### 5.2 Pitfall: Doing premature optimization (`memset` & friends).



#### 5.3 Pitfall: Using the C idiom to get number of elements.

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

1. The compiler rewrites `int const a[7]` to just `int const a[]`.
2. The compiler rewrites `int const a[]` to `int const* a`.
3. `N_ITEMS` is therefore invoked with a pointer.
4. For a 32-bit executable `sizeof(array)` (size of a pointer) is then 4.
5. `sizeof(*array)` is equivalent to `sizeof(int)`, which for a 32-bit executable is also 4.



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

> NOTE: 关于`typeid` ，参见cppreference [`typeid`](https://en.cppreference.com/w/cpp/language/typeid)
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
>  #define N_ITEMS( array )       n_items( array )
> 
> ```
>
> 

How it works: the array is passed *by reference* to `n_items`, and so it does not decay to pointer to first element, and the function can just return the number of elements specified by the type.

With C++11 you can use this also for arrays of local type, and it's the type safe **C++ idiom** for finding the number of elements of an array.

#### 5.4 C++11 & C++14 pitfall: Using a `constexpr` array size function.

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
>



## cppreference [Array declaration](https://en.cppreference.com/w/cpp/language/array)



### initialize array

https://www.dummies.com/programming/cpp/initializing-an-array-in-c/

https://stackoverflow.com/questions/4057948/initializing-a-member-array-in-constructor-initializer

https://stackoverflow.com/questions/2983819/how-initialize-array-of-classes





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



## Get length of array in C and C++

### Length in term of element in the array

关于此，在“5.3 Pitfall: Using the C idiom to get number of elements.”中进行了详细说明；

### Length in term of byte

当我们使用c api来操作array的时候，需要使用length in term of byte，主要使用`sizeof`。

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





## TODO

[2 Dimensional Char Array C++](https://stackoverflow.com/questions/27697879/2-dimensional-char-array-c)

[Two-Dimensional Char Array](http://www.cplusplus.com/forum/beginner/53734/)

[valarray](http://www.cplusplus.com/reference/valarray/valarray/)

[Promoting a raw pointer to valarray](https://stackoverflow.com/questions/15077173/promoting-a-raw-pointer-to-valarray)


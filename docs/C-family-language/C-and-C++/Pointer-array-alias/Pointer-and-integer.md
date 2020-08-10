# Pointer and integer

理论上来说，在c family language中，使用pointer来保存address；理论上来说，address其实就是integer，所以我们自然而然地想到pointer和integer之间的关联，下面是一些样例。

## Conversion between pointer and integer

### cmu.edu [INT36-C. Converting a pointer to integer or integer to pointer](https://wiki.sei.cmu.edu/confluence/display/c/INT36-C.+Converting+a+pointer+to+integer+or+integer+to+pointer)

Although programmers often use **integers** and **pointers** interchangeably in C, **pointer-to-integer** and **integer-to-pointer** conversions are [implementation-defined](https://wiki.sei.cmu.edu/confluence/display/c/BB.+Definitions#BB.Definitions-implementation-definedbehavior). 

Conversions between integers and pointers can have undesired consequences depending on the [implementation](https://wiki.sei.cmu.edu/confluence/display/c/BB.+Definitions#BB.Definitions-implementation). According to the C Standard, subclause 6.3.2.3 [[ISO/IEC 9899:2011](https://wiki.sei.cmu.edu/confluence/display/c/AA.+Bibliography#AA.Bibliography-ISO-IEC9899-2011)],

> An integer may be converted to any pointer type. Except as previously specified, the result is implementation-defined, might not be correctly aligned, might not point to an entity of the referenced type, and might be a trap representation.
>
> NOTE: 上面描述了integer to pointer conversion的潜在问题：
>
> - alignment：“might not be correctly aligned”
> - dangling pointer：“might not point to an entity of the referenced type”
> - trap representation：“might be a trap representation”

> Any **pointer type** may be converted to an **integer type**. Except as previously specified, the result is implementation-defined. If the result cannot be represented in the integer type, the behavior is undefined. The result need not be in the range of values of any integer type.
>
> NOTE: 上面描述了pointer to integer conversion的潜在问题：
>
> - range

Do not convert an integer type to a pointer type if the resulting pointer is incorrectly aligned, does not point to an entity of the referenced type, or is a [trap representation](https://wiki.sei.cmu.edu/confluence/display/c/BB.+Definitions#BB.Definitions-traprepresentation).

Do not convert a pointer type to an integer type if the result cannot be represented in the integer type. (See [undefined behavior 24](https://wiki.sei.cmu.edu/confluence/display/c/CC.+Undefined+Behavior#CC.UndefinedBehavior-ub_24).)

The mapping between pointers and integers must be consistent with the addressing structure of the execution environment. Issues may arise, for example, on architectures that have a segmented memory model.

#### Noncompliant Code Example: pointer to integral type

The size of a **pointer** can be greater than the size of an **integer**, such as in an implementation where pointers are 64 bits and unsigned integers are 32 bits. This code example is noncompliant on such implementations because the result of converting the 64-bit `ptr` cannot be represented in the 32-bit integer type:

```c++
#include <stdio.h>
void f(void)
{
	char *ptr;
	printf("%d\n", ptr);
	/* ... */
	unsigned int number = (unsigned int) ptr;
	printf("%d\n", number);
	/* ... */
}
int main()
{
	f();
}
// gcc -g test.c

```

> NOTE: 上述程序在进行编译的时候，会产生如下告警：
>
> ```c++
> test.c: 在函数‘f’中:
> test.c:7:24: 警告：将一个指针转换为大小不同的整数 [-Wpointer-to-int-cast]
>   unsigned int number = (unsigned int) ptr;
> 
> ```
>
> 上述程序的输出：
>
> ```c++
> 4195392
> 4195392
> ```

#### Compliant Solution

Any valid pointer to `void` can be converted to `intptr_t` or `uintptr_t` and back with no change in value. (See **INT36-EX2**.) The C Standard guarantees that a pointer to `void` may be converted to or from a pointer to any object type and back again and that the result must compare equal to the original pointer. Consequently, converting directly from a `char *` pointer to a `uintptr_t`, as in this compliant solution, is allowed on implementations that support the `uintptr_t` type.

```c
#include <stdio.h>
#include <stdint.h>
void f(void)
{
	char *ptr;
	printf("%d\n", ptr);
	/* ... */
	uintptr_t number = (uintptr_t)ptr;
	printf("%d\n", number);
	/* ... */
}
int main()
{
	f();
}
// gcc -g test.c

```

#### Noncompliant Code Example: **literal integer** to **pointer** 

It is sometimes necessary to access memory at a specific location, requiring a **literal integer** to **pointer** conversion. In this noncompliant code, a pointer is set directly to an **integer constant**, where it is unknown whether the result will be as intended:

```c++
#include <stdio.h>
unsigned int *g(void)
{
	unsigned int *ptr = 0xdeadbeef;
	/* ... */
	return ptr;
}
int main()
{
	g();
}
// gcc -g test.c

```

The result of this assignment is [implementation-defined](https://wiki.sei.cmu.edu/confluence/display/c/BB.+Definitions#BB.Definitions-implementation-definedbehavior), might not be correctly aligned, might not point to an entity of the referenced type, and might be a [trap representation](https://wiki.sei.cmu.edu/confluence/display/c/BB.+Definitions#BB.Definitions-traprepresentation).

> NOTE: 上面这段话描述了integer to pointer conversion的危险

> NOTE: 上述程序在进行编译的时候，会产生如下告警：
>
> ```c
> test.c: 在函数‘g’中:
> test.c:4:22: 警告：初始化时将整数赋给指针，未作类型转换 [默认启用]
>   unsigned int *ptr = 0xdeadbeef;
> ```
>
> 修改为如下形式：
>
> ```c++
> #include <stdio.h>
> unsigned int *g(void)
> {
> 	unsigned int *ptr = (unsigned int *)0xdeadbeef;
> 	/* ... */
> 	return ptr;
> }
> int main()
> {
> 	g();
> }
> // gcc -g test.c
> ```

#### Compliant Solution

Unfortunately this code cannot be made safe while strictly conforming to ISO C.

A particular platform (that is, hardware, operating system, compiler, and Standard C library) might guarantee that a **memory address** is correctly **aligned** for the pointer type, and actually contains a value for that type. A common practice is to use addresses that are known to point to hardware that provides valid values.

#### Exceptions

**INT36-C-EX1:** The integer value 0 can be converted to a pointer; it becomes the **null pointer**.

**INT36-C-EX2:** Any valid pointer to `void` can be converted to `intptr_t` or `uintptr_t` or their underlying types and back again with no change in value. Use of underlying types instead of `intptr_t` or `uintptr_t` is discouraged, however, because it limits portability.

```c++
#include <assert.h>
#include <stdint.h>

void h(void)
{
	intptr_t i = (intptr_t) (void *) &i;
	uintptr_t j = (uintptr_t) (void *) &j;

	void *ip = (void *) i;
	void *jp = (void *) j;

	assert(ip == &i);
	assert(jp == &j);
}

int main()
{
	h();
}
// gcc -g test.c

```



## cppreference [reinterpret_cast conversion](https://en.cppreference.com/w/cpp/language/reinterpret_cast)

在cppreference [reinterpret_cast conversion](https://en.cppreference.com/w/cpp/language/reinterpret_cast)中，对integer to pointer、pointer to integer进行了详细说明。

### Integer to pointer

本节标题的含义是：“将integer转换为pointer”；正常来说，compiler是绝对不允许这样做的，因为这样做非常危险，下面举例进行说明。

```c++
#include <iostream>

int* numbers[42] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
int main()
{
	using namespace std;
	for (int i = 0; i < 42; ++i)
	{
		cout << (i > 0 ? ", " : "") << numbers[i];
	}
	cout << endl;
}
// g++ -g test.cpp
```

上述程序，编译报错如下：

```c++
test.cpp:4:48: 错误：从类型‘int’到类型‘int*’的转换无效 [-fpermissive]
 int* numbers[42] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
                                                ^
test.cpp:4:48: 错误：从类型‘int’到类型‘int*’的转换无效 [-fpermissive]
test.cpp:4:48: 错误：从类型‘int’到类型‘int*’的转换无效 [-fpermissive]
test.cpp:4:48: 错误：从类型‘int’到类型‘int*’的转换无效 [-fpermissive]
test.cpp:4:48: 错误：从类型‘int’到类型‘int*’的转换无效 [-fpermissive]
test.cpp:4:48: 错误：从类型‘int’到类型‘int*’的转换无效 [-fpermissive]
test.cpp:4:48: 错误：从类型‘int’到类型‘int*’的转换无效 [-fpermissive]
test.cpp:4:48: 错误：从类型‘int’到类型‘int*’的转换无效 [-fpermissive]
test.cpp:4:48: 错误：从类型‘int’到类型‘int*’的转换无效 [-fpermissive]
```

上述程序，就典型地“将integer用作pointer”；显然，compiler是不允许的；

但是在一些情况下，compiler会允许将integer用作pointer，

下面是这样的一个例子：

#### Example: `reinterpret_cast`

```c++
// [main.cpp]
#include <iostream>

int* numbers[42] = { reinterpret_cast<int*>(1), reinterpret_cast<int*>(2), reinterpret_cast<int*>(3), reinterpret_cast<int*>(4) };
int main()
{
	using namespace std;
	for (int i = 0; i < 42; ++i)
	{
		cout << (i > 0 ? ", " : "") << numbers[i];
	}
	cout << endl;
}
// g++ -g test.cpp

```

上述程序等价于：

#### Example: c-style cast

```c++
// [main.cpp]
#include <iostream>

int* numbers[42] = { (int*) 1, (int*) 2, (int*) 3, (int*) 4 };
int main()
{
	using namespace std;
	for (int i = 0; i < 42; ++i)
	{
		cout << (i > 0 ? ", " : "") << numbers[i];
	}
	cout << endl;
}
// g++ -g test.cpp

```

上述程序的输出：

```c++
0x1, 0x2, 0x3, 0x4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
```



#### Example: extern

在 stackoverflow [How do I use arrays in C++?](https://stackoverflow.com/questions/4810664/how-do-i-use-arrays-in-c)的[Common pitfalls when using arrays](https://stackoverflow.com/a/7439261) 中给出了样例程序：



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



> NOTE: 编译： `g++ main.cpp numbers.cpp -g -o main`



#### 0 and null pointer

compiler是允许将0赋值给pointer的，下面是一些例子：

```c++
// [main.cpp]
#include <iostream>
int i0 = 0;
int i1 = 1;
int i2 = 2;
int i3 = 3;
int* numbers[42] = { &i0, &i1, &i2, &i3 };
int main()
{
	using namespace std;
	for (int i = 0; i < 42; ++i)
	{
		cout << (i > 0 ? ", " : "") << *numbers[i];
	}
	cout << endl;
}
// g++ test.cpp
```

上述程序是会core dump的，因为除了前4个元素，`numbers`中的其他元素的值为`0`，这就导致了deference null pointer。



```c++
// [main.cpp]
#include <iostream>
int* numbers[42] = { 0, 0 };
int main()
{
	using namespace std;
	for (int i = 0; i < 42; ++i)
	{
		cout << (i > 0 ? ", " : "") << *numbers[i];
	}
	cout << endl;
}

```

上述程序是可以编译通过的。



### Pointer to integer

#### Example: stackoverflow [How do I cast a pointer to an int](https://stackoverflow.com/questions/14092754/how-do-i-cast-a-pointer-to-an-int)

错误程序如下：

```c++
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

vector<int> test;

int main()
{
	int *ip;
	int pointervalue = 50;
	ip = &pointervalue;

	int thatvalue = 1;
	thatvalue = ip; // compile error：pointer to integer

	cout << ip << endl;

	test.push_back(thatvalue);

	cout << test[0] << endl;
	return 0;
}
// g++ -g test.cpp

```

上述程序编译报错：

```c++
test.cpp: 在函数‘int main()’中:
test.cpp:16:12: 错误：从类型‘int*’到类型‘int’的转换无效 [-fpermissive]
  thatvalue = ip; // compile error：pointer to integer
```

按照 [A](https://stackoverflow.com/a/14093919) 中的回答，使用`reinterpret_cast<intptr_t>`：

```c++
#include "stdint.h"
#include <iostream>
#include <vector>

using namespace std;

vector<intptr_t> test;

int main()
{
	int *ip;
	int pointervalue = 50;
	ip = &pointervalue;

	intptr_t thatvalue = 1;
	/* Convert it as a bit pattern.
	 It is valid and converting it back to a pointer is also OK
	 But if you modify it all bets are off (you need to be very careful).*/
	thatvalue = reinterpret_cast<intptr_t>(ip);

	cout << ip << endl;

	test.push_back(thatvalue);

	cout << test[0] << endl;
	return 0;
}
// g++ -g test.cpp

```

按照 [A](https://stackoverflow.com/a/14092767) 中的回答，使用C-style cast：

```c++
#include "stdint.h"
#include <iostream>
#include <vector>

using namespace std;

vector<intptr_t> test;

int main()
{
	int *ip;
	int pointervalue = 50;
	ip = &pointervalue;

	intptr_t thatvalue = 1;
	/* Convert it as a bit pattern.
	 It is valid and converting it back to a pointer is also OK
	 But if you modify it all bets are off (you need to be very careful).*/
	thatvalue = (intptr_t) ip;

	cout << ip << endl;

	test.push_back(thatvalue);

	cout << test[0] << endl;
	return 0;
}
// g++ -g test.cpp

```





## iso c++ forbids comparison between pointer and integer fpermissive

这个编译错误的原因是非常清晰的，即进行了pointer和integer的比较，下面的是样例程序。

### Example 1: 错误的使用`==`



```c++
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

这个例子源自 https://stackoverflow.com/a/4810668 ，上述程序编译报错如下：

```c++
test.cpp: 在函数‘int main()’中:
test.cpp:7:43: 错误：ISO C++ 不允许比较指针和整数的值 [-fpermissive]
  bool equal = &x[i] == &*(x + i) == (x + i);
```

问题原因在于对`==`的错误使用：

`&x[i] == &*(x + i) == (x + i)`等价于:

```c++
bool equal = &x[i] == &*(x + i);
equal = equal == (x + i);
```

显然，`equal == (x + i)`就触发了“比较指针和整数的值”，则导致上述编译报错。修改后的版本如下：

```c++
#include <iostream>

int main()
{
	int x[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	int i = 3;
	bool equal = &x[i] == &*(x + i) && &*(x + i) == (x + i);
	std::cout << std::boolalpha;
	std::cout << equal << std::endl;
}
// g++ --std=c++11 test.cpp

```

输出:

```c++
true
```



### Example 2: compare `char` with `const char *`



```c++
#include <iostream>
#include <string>
using namespace std;
bool accept()
{
	cout << "Do you want to proceed (y or n)?\n";
	char answer;
	cin >> answer;
	if (answer == "y")
	{
		return true;
	}
	return false;
}

int main()
{
	if (accept())
	{

	}
}

```

上述例子源自：[c++ compile error: ISO C++ forbids comparison between pointer and integer](https://stackoverflow.com/questions/2263681/c-compile-error-iso-c-forbids-comparison-between-pointer-and-integer)，上述程序编译报错如下：

```c++
test.cpp: 在函数‘bool accept()’中:
test.cpp:9:16: 错误：ISO C++ 不允许比较指针和整数的值 [-fpermissive]
  if (answer == "y")
```




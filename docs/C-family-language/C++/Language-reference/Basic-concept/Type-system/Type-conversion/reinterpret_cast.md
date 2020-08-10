# `reinterpret_cast` 



## cppreference reinterpret_cast conversion

Converts between types by **reinterpreting** the underlying bit pattern.

> NOTE: 上面这段话所描述的就是alias；关于上面这段话中的**reinterpreting**，参见`C++\Language-reference\Basic-concept\Data-model\Object\Object.md`中”Interpretion of memory representation“

### Explanation

### Integral type and pointer

#### 1)

> NOTE: 原文描述的规则没有理解

### 2) pointer to integral type

A pointer can be converted to any integral type large enough to hold all values of its type (e.g. to `std::uintptr_t`)

A **pointer** converted to an integer of **sufficient size** and back to the same pointer type is guaranteed to have its original value, otherwise the resulting pointer cannot be dereferenced safely (the round-trip conversion in the opposite direction is not guaranteed; the same pointer may have multiple integer representations)

> NOTE: 为了保证“sufficient size”，最好使用C99中引入的[`intptr_t`、`uintptr_t`](https://en.cppreference.com/w/c/types/integer) ，关于此，可以参见：
>
> - stackoverflow [What is the use of intptr_t?](https://stackoverflow.com/questions/35071200/what-is-the-use-of-intptr-t)
> - stackoverflow [Why / when to use `intptr_t` for type-casting in C?](https://stackoverflow.com/questions/6326338/why-when-to-use-intptr-t-for-type-casting-in-c)

The null pointer constant NULL or integer zero is not guaranteed to yield the null pointer value of the target type; [static_cast](https://en.cppreference.com/w/cpp/language/static_cast) or [implicit conversion](https://en.cppreference.com/w/cpp/language/implicit_cast) should be used for this purpose.

> NOTE: 原文中的，这一段话没有理解。

##### Example: stackoverflow [How do I cast a pointer to an int](https://stackoverflow.com/questions/14092754/how-do-i-cast-a-pointer-to-an-int)

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



### 3) integral or enumeration type to pointer

### Type aliasing

#### 5) type aliasing through pointer



##### Examples

[Object](https://en.cppreference.com/w/cpp/language/object)`#`[Object representation and value representation](https://en.cppreference.com/w/cpp/language/object#Object_representation_and_value_representation)

```C++
#include <cassert>
struct S
{
	char c;  // 1 byte value
			 // 3 bytes padding (assuming alignof(float) == 4)
	float f; // 4 bytes value (assuming sizeof(float) == 4)
	bool operator==(const S& arg) const
	{ // value-based equality
		return c == arg.c && f == arg.f;
	}
};

void f()
{
	assert(sizeof(S) == 8);
	S s1 = { 'a', 3.14 };
	S s2 = s1;
	reinterpret_cast<unsigned char*>(&s1)[2] = 'b'; // change 2nd byte of padding
	assert(s1 == s2); // value did not change
}
int main()
{
	f();
}
// g++ test.cpp

```

stackoverflow [How to print (using cout) a number in binary form?](https://stackoverflow.com/questions/7349689/how-to-print-using-cout-a-number-in-binary-form) `#` [A](https://stackoverflow.com/a/7349839)

```c++
#include <iostream>
#include <bitset>
#include <climits>

template<typename T>
void show_binrep(const T& a)
{
	const char* beg = reinterpret_cast<const char*>(&a);
	const char* end = beg + sizeof(a);
	while (beg != end)
	{
		std::cout << std::bitset<CHAR_BIT>(*beg++) << ' ';
	}
	std::cout << '\n';
}
int main()
{
	int a, b;
	short c;
	a = 1;
	c = 2;
	b = a << 3;
	show_binrep(a);
	show_binrep(b);
	show_binrep(c);
	float f = 1;
	show_binrep(f);
}
// g++ test.cpp

```



#### 6) type aliasing through reference
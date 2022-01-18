# `reinterpret_cast` 

C++中`reinterpret_cast`的目的是是实现：alias to an existing object，这在`C++\Language-reference\Basic-concept\Data-model\Object\Object.md#Aliase to an existing object` 中进行了详细介绍。

pointer、reference的conversion都使用`reinterpret_cast` 。

参见:

- pointer: `C-family-language\C-and-C++\Pointer-array-alias\Pointer`
- reference: `C++\Language-reference\Reference`

## cppreference [reinterpret_cast conversion](https://en.cppreference.com/w/cpp/language/reinterpret_cast)

Converts between types by **reinterpreting** the underlying bit pattern.

> NOTE: 上面这段话所描述的就是alias，这在`C++\Language-reference\Basic-concept\Data-model\Object\Object.md#Aliase to an existing object` 中进行了详细介绍。

### Explanation

Only the following conversions can be done with `reinterpret_cast`, except when such conversions would cast away *constness* or *volatility*.

> NOTE: 上面这段话的意思是：`reinterpret_cast`是需要保证CV的，结合下面的例子来进行说明：
>
> ```c++
> #include <iostream>
> 
> void func(const void* input)
> {
> 	const int* i = reinterpret_cast<const int*>(input); // 保持CV
> 	std::cout << *i << std::endl;
> }
> int main()
> {
> 	const int i = 0;
> 	func(&i);
> }
> // g++ test.cpp
> ```
>
> 函数`func`的入参是`const`的，`reinterpret_cast`的时候，是需要保持CV的，否则会编译报错，如下是错误示例：
>
> ```c++
> #include <iostream>
> 
> void func(const void* input)
> {
> 	int* i = reinterpret_cast<int*>(input); // 丢失CV
> 	std::cout << *i << std::endl;
> }
> int main()
> {
> 	const int i = 0;
> 	func(&i);
> }
> 
> ```
>
> 上述程序报错如下：
>
> ```c++
> test.cpp: 在函数‘void func(const void*)’中:
> test.cpp:5:39: 错误：从类型‘const void*’到类型‘int*’的 reinterpret_cast 丢失了限定符
>   int* i = reinterpret_cast<int*>(input);
> ```
>
> 上述程序，可以通过`const_cast`来 drop CV，在“`reinterpret_cast` VS C-style cast”章节对它进行了详细描述。

### Integral type and pointer

> NOTE: 关于integral type 和 pointer之间的转换，在`C-family-language\C-and-C++\Pointer-array-alias\Pointer-and-integer.md`中对此进行了详细的说明。

#### 1)

> NOTE: 原文描述的规则没有理解

#### 2) pointer to integral type

A pointer can be converted to any integral type large enough to hold all values of its type (e.g. to `std::uintptr_t`)

A **pointer** converted to an integer of **sufficient size** and back to the same pointer type is guaranteed to have its original value, otherwise the resulting pointer cannot be dereferenced safely (the round-trip conversion in the opposite direction is not guaranteed; the same pointer may have multiple integer representations)

> NOTE: 为了保证“sufficient size”，最好使用C99中引入的[`intptr_t`、`uintptr_t`](https://en.cppreference.com/w/c/types/integer) ，关于此，在cmu.edu [INT36-C. Converting a pointer to integer or integer to pointer](https://wiki.sei.cmu.edu/confluence/display/c/INT36-C.+Converting+a+pointer+to+integer+or+integer+to+pointer)中进行了探讨，在`C-and-C++\Pointer-array-alias\Pointer-and-integer.md`中收录了这篇文章。
>
> 关于[`intptr_t`、`uintptr_t`](https://en.cppreference.com/w/c/types/integer) ，可以参见：
>
> - stackoverflow [What is the use of intptr_t?](https://stackoverflow.com/questions/35071200/what-is-the-use-of-intptr-t)
> - stackoverflow [Why / when to use `intptr_t` for type-casting in C?](https://stackoverflow.com/questions/6326338/why-when-to-use-intptr-t-for-type-casting-in-c)

The null pointer constant NULL or integer zero is not guaranteed to yield the null pointer value of the target type; [static_cast](https://en.cppreference.com/w/cpp/language/static_cast) or [implicit conversion](https://en.cppreference.com/w/cpp/language/implicit_cast) should be used for this purpose.

> NOTE: 原文中的，这一段话没有理解。



#### 3) integral or enumeration type to pointer



#### 4) `std::nullptr_t` to integral type



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



#### 11) OOP type aliasing: type aliasing through pointer



### Pointer to function

#### 7) pointer to function conversion



#### 10) OOP pointer to function conversion



## `reinterpret_cast` VS C-style cast

### CV

从CV角度来看：

- `reinterpret_cast`需要保证CV（关于此，参见cppreference [reinterpret_cast conversion](https://en.cppreference.com/w/cpp/language/reinterpret_cast)`#`Explanation段，其中给出了示例）
- C-style cast不需要保证CV

从这个角度来看：C++对C中的type-unsafe做的改善。`C++`的`reinterpret_cast`保证了type-safe，同时C++提供了[`const_cast`](https://en.cppreference.com/w/cpp/language/const_cast)来给予programmer escape from CV的能力，但是相比于C的implicit，C++的`const_cast`是explicit，因此相比而言，它更加safe。  

下面是cppreference [reinterpret_cast conversion](https://en.cppreference.com/w/cpp/language/reinterpret_cast)`#`Explanation段中给出了示例使用C-style cast重写的结果：

```c++
#include <iostream>

void func(const void* input)
{
	int* i = (int*) input;
	std::cout << *i << std::endl;
}
int main()
{
	const int i = 0;
	func(&i);
}
// gcc test.c

```

上述程序编译通过，但是可能引入undefined behavior，如果在`func`尝试对`input`进行修改，则会导致undefined behavior，这体现了了C++ 的type safety。

`reinterpret_cast` + `const_cast` 来模拟C-style cast：

```c++
#include <iostream>

void func(const void* input)
{
	int* i = const_cast<int*>(reinterpret_cast<const int*>(input)); // 丢失CV
	std::cout << *i << std::endl;
}
int main()
{
	const int i = 0;
	func(&i);
}
// g++ test.cpp

```





## TODO

[When to use reinterpret_cast?](https://stackoverflow.com/questions/573294/when-to-use-reinterpret-cast)

[Reinterpret_cast vs. C-style cast](https://stackoverflow.com/questions/7831696/reinterpret-cast-vs-c-style-cast)

[using reinterpret_cast to convert char* to vector](https://stackoverflow.com/questions/30287441/using-reinterpret-cast-to-convert-char-to-vectorbyte)


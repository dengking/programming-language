# Binary representation

与binary representation密切相关的一个概念是：[Endianness](https://en.wikipedia.org/wiki/Endianness)，在工程Hardware的`CPU\Endianess`对它进行了描述。

本文描述的binary representation其实就是在cppreference [Object](https://en.cppreference.com/w/cpp/language/object)`#`[Object representation and value representation](https://en.cppreference.com/w/cpp/language/object#Object_representation_and_value_representation)章节中描述的object representation。

## Print a number in binary form

下面程序都是在如下CPU中测试的：

```
Architecture:          x86_64
CPU op-mode(s):        32-bit, 64-bit
Byte Order:            Little Endian
```



### stackoverflow [How to print (using cout) a number in binary form?](https://stackoverflow.com/questions/7349689/how-to-print-using-cout-a-number-in-binary-form)

#### [A](https://stackoverflow.com/a/7349767): use `std::bitset` 

The easiest way is probably to create an [`std::bitset`](http://en.cppreference.com/w/cpp/utility/bitset) representing the value, then stream that to `cout`.

```C++
#include <bitset>
#include <iostream>
int main()
{
	char a = 1;
	std::bitset<8> x(a);
	std::cout << x << '\n';

	short c = 1;
	std::bitset<16> y(c);
	std::cout << y << '\n';
}
// g++ test.cpp

```

> NOTE: 上述程序的输出:
>
> ```
> 00000001
> 0000000000000001
> ```
>
> 通过上面的输出可以看出，通过`std::bitset`并不能够真实地或者CPU是big endian还是little endian，因为它已经将结果自动转换为big endian了。

#### [A](https://stackoverflow.com/a/7349839): use `reinterpret_cast` and`std::bitset` 

If you want to display the **bit representation** of any object, not just an integer, remember to reinterpret as a `char` array first, then you can print the contents of that array, as hex, or even as binary (via `bitset`):

```C++
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

> NOTE: 
>
> 上述程序能够真实地反映CPU的Endianness。
> 
> 上运行上述程序的输出如下：
>
> ```
>00000001 00000000 00000000 00000000 
> 00001000 00000000 00000000 00000000 
> 00000010 00000000 
> 00000000 00000000 10000000 00111111
> ```
> 
> 

Note that most common systems are little-endian, so the output of `show_binrep(c)` is **not** the `1111111 011000101` you expect, because that's not how it's stored in memory. If you're looking for *value* representation in binary, then a simple `cout << bitset<16>(c)` works.



#### [A](https://stackoverflow.com/a/7349925): C-style aliasing

```C++
#include<iostream>
#include <climits>

template<typename T>
void printBin(const T& t)
{
	size_t nBytes = sizeof(T);
	char* rawPtr((char*) (&t));
	for (size_t byte = 0; byte < nBytes; byte++)
	{
		for (size_t bit = 0; bit < CHAR_BIT; bit++)
		{
			std::cout << (((rawPtr[byte]) >> bit) & 1);
		}
	}
	std::cout << std::endl;
}
;

int main(void)
{
	for (int i = 0; i < 50; i++)
	{
		std::cout << i << ": ";
		printBin(i);
	}
}
// g++ test.cpp

```



#### [A](https://stackoverflow.com/a/50674064): `std::bitset`

```c++
#include<iostream>
#include<sstream>
#include <bitset>
#include <stdint.h>

template<typename T>
static std::string toBinaryString(const T& x)
{
	std::stringstream ss;
	ss << std::bitset<sizeof(T) * 8>(x);
	return ss.str();
}

int main()
{
	uint16_t x = 8;
	std::cout << toBinaryString(x);
}
// g++ test.cpp

```



#### [A](https://stackoverflow.com/a/61552933): `std::bitset`

```c++
#include <iostream>
#include <bitset>
#include <climits>
#include <stdint.h>
template<typename T>
struct BinaryForm {
    BinaryForm(const T& v) : _bs(v) {}
    const std::bitset<sizeof(T)*CHAR_BIT> _bs;
};

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const BinaryForm<T> bf) {
    return os << bf._bs;
}
int main()
{

char c = 'A';
std::cout << "c: " << c << " binary: " << BinaryForm<char>{c} << std::endl;
unsigned x = 1234;
std::cout << "x: " << x << " binary: " << BinaryForm<unsigned>{x} << std::endl;
int64_t z { -1024 };
std::cout << "z: " <<z  << " binary: " << BinaryForm<int64_t>{z} << std::endl;
}
```




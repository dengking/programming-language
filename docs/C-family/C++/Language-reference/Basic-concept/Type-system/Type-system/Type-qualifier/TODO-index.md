# 关于本章

本章对C++的type specifier进行说明，目前C++中有两个type specifier；

本章采用的在`C++\Language-reference\Basic-concept\index.md#Extend to OOP`中总结的思路:

> 首先描述Non-OOP，然后描述OOP

## const and non-const

const -> non const是危险的，丢失了CV，可能导致undefined behavior；
non-const -> const是正常的，不存在问题；

下面是一个典型non-const->const的例子：

```C++
#include <iostream>

void func(const void* input)
{
	const int* i = reinterpret_cast<const int*>(input); // 保持CV
	std::cout << *i << std::endl;
}
int main()
{
	const int i = 0;
	func(&i);
}
// g++ test.cpp
```



```c++
#include <iostream>

int main()
{
	int i = 3;                 // i is not declared const
	const int& rci = i;
	const_cast<int&>(rci) = 4; // OK: modifies i
	std::cout << "i = " << i << '\n';
}
// g++ test.cpp
```



## C and C++

C++的CV比C要严格。
# Constructor

“Constructor”即“构造函数”。我们已经知道了object是在runtime创建，根据Data-model章节的观点，我们可以将object简单地看做是一片内存区域，显然这片内存区域肯定会包含[non-static data members](https://en.cppreference.com/w/cpp/language/data_members)，那当程序运行时，如何来initialize这片内存区域呢？这对于c++ OOP是非常重要的，因为只有正确地初始化后，object才能够正确地运行。c++提供了丰富的constructor，程序员需要谨慎地实现以正确地initialize这些数据。需要注意的是，constructor是在[Initialization](https://en.cppreference.com/w/cpp/language/initialization)阶段被调用的。本文对c++中的constructor进行总结。



## cppreference [Constructors and member initializer lists](https://en.cppreference.com/w/cpp/language/initializer_list)



### Delegating constructor (since C++11)

> NOTE: 关于delegate，参见`Theory\Programming-paradigm\Object-oriented-programming\Assemble\Composition\Delegation`
>
> TODO:
>
> https://www.geeksforgeeks.org/constructor-delegation-c/

```c++
#include <iostream>
class Foo
{
public:
	Foo(char x, int y)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	Foo(int y)
			: Foo('a', y) // Foo(int) delegates to Foo(char,int)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

int main()
{
	Foo f(1);
}
// g++ --std=c++11 test.cpp

```

> NOTE: 上述程序的输出如下:
>
> ```c++
> Foo::Foo(char, int)
> Foo::Foo(int)
> ```



### Initialization order

> NOTE: 这部分内容移到了`Initialization-and-deinitialization-order`章节

## Summary

Classification of constructor：

- default constructor
- copy constructor
- move constructor

对于上述每种constructor，都涉及如下问题：

| 问题                                       | 说明                                                         |      |
| ------------------------------------------ | ------------------------------------------------------------ | ---- |
| implicitly-declared and Implicitly-defined |                                                              |      |
| default                                    | If some user-defined constructors are present, the user may still force the automatic generation of a default constructor by the compiler that would be implicitly-declared otherwise with the keyword `default`. (since C++11) |      |
| delete                                     | Deleted constructor.                                         |      |



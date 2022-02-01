# cppreference [Constructors and member initializer lists](https://en.cppreference.com/w/cpp/language/initializer_list)



## Delegating constructor (since C++11)

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



## Initialization order

> NOTE: 这部分内容移到了`Initialization&deinitialization-order`章节

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


# Constructor

“Constructor”即“构造函数”。我们已经知道了object是在runtime创建，根据Data-model章节的观点，我们可以将object简单地看做是一片内存区域，显然这片内存区域肯定会包含[non-static data members](https://en.cppreference.com/w/cpp/language/data_members)，那当程序运行时，如何来initialize这片内存区域呢？这对于c++ OOP是非常重要的，因为只有正确地初始化后，object才能够正确地运行。c++提供了丰富的constructor，程序员需要谨慎地实现以正确地initialize这些数据。需要注意的是，constructor是在[Initialization](https://en.cppreference.com/w/cpp/language/initialization)阶段被调用的。本文对c++中的constructor进行总结。



## cppreference [Constructors and member initializer lists](https://en.cppreference.com/w/cpp/language/initializer_list)



### Delegating constructor (since C++11)



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

The order of **member initializers** in the list is irrelevant: the actual order of initialization is as follows:

1) If the constructor is for the **most-derived class**, **virtual bases** are initialized in the order in which they appear in depth-first left-to-right traversal of the base class declarations (left-to-right refers to the appearance in **base-specifier lists**)

2) Then, **direct bases** are initialized in left-to-right order as they appear in this class's **base-specifier list**

3) Then, non-static data member are initialized in order of declaration in the class definition.

4) Finally, the body of the constructor is executed

(Note: if initialization order was controlled by the appearance in the member initializer lists of different constructors, then the [destructor](https://en.cppreference.com/w/cpp/language/destructor) wouldn't be able to ensure that the order of destruction is the reverse of the order of construction)

> NOTE: 沿着class hierarchy，自顶向下、自左至右进行初始化。

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











## cppreference [Default constructors](https://en.cppreference.com/w/cpp/language/default_constructor)

A default constructor is a [constructor](https://en.cppreference.com/w/cpp/language/constructor) which can be called with no arguments (either defined with an empty parameter list, or with default arguments provided for every parameter). A type with a public default constructor is [*DefaultConstructible*](https://en.cppreference.com/w/cpp/named_req/DefaultConstructible).

> NOTE: 在维基百科[C++11#Modification to the definition of plain old data](https://en.wikipedia.org/wiki/C++11#Modification_to_the_definition_of_plain_old_data) 中提及了**statically initialized**，这让我想起了这样的问题：
>
>  [*DefaultConstructible*](https://en.cppreference.com/w/cpp/named_req/DefaultConstructible)能够被  **statically initialized**？
>

### Syntax



### Implicitly-declared default constructor



### Implicitly-defined default constructor



### Deleted implicitly-declared default constructor



### Trivial default constructor



### Eligible default constructor



## cppreference [Copy constructors](https://en.cppreference.com/w/cpp/language/copy_constructor)



### Syntax



### Implicitly-declared copy constructor



### Implicitly-defined copy constructor



### Deleted implicitly-declared copy constructor



### Trivial copy constructor





## cppreference [Move constructors](https://en.cppreference.com/w/cpp/language/move_constructor)



### Syntax



### Implicitly-declared move constructor



### Implicitly-defined move constructor



### Trivial move constructor



## cppreference [Converting constructor](https://en.cppreference.com/w/cpp/language/converting_constructor)
# Polymorphic type

在上一节中，我们已经介绍了polymorphic type，本节对它进行专门介绍。

cppreference中并没有正式地定义polymorphic type，其中只有 [polymorphic object](https://en.cppreference.com/w/cpp/language/object) 的定义，仔细阅读 [polymorphic object](https://en.cppreference.com/w/cpp/language/object) 段，可以发现其中给出的例子中有“polymorphic type”，当我们进入到type system后，使用polymorphic type来进行描述会更加方便。

在文章panicsoftware [DYNAMIC_CAST AND TYPEID AS (NON) RTTI TOOLS.](https://blog.panicsoftware.com/dynamic_cast-and-typeid-as-non-rtti-tools/)中，给出了polymorphic type的定义:

> First thing you need to know about RTTI is, that it doesn’t work for the non-polymorphic types. If you wonder what polymorphic types are, then here I come with the explanation. The types are polymorphic, when either they have at least one virtual function or they inherit from another polymorphic type.



## Static type and dynamic type

`C++`中对**static type** 和 **dynamic type**的区分是源于`C++`对runtime polymorphism（参见 [polymorphic object](https://en.cppreference.com/w/cpp/language/object)）的支持，只有[polymorphic object](https://en.cppreference.com/w/cpp/language/object)才同时具备static type和dynamic type。

**static type**和**dynamic type**的概念对于`C++` 非常重要，关于两者的一个典型例子就是：`C++`中如何实现multiple dispatch，参见[A polyglot's guide to multiple dispatch](https://eli.thegreenplace.net/2016/a-polyglots-guide-to-multiple-dispatch/)，在`Theory\Programming-paradigm\Object-oriented-programming\Polymorphism\Subtyping-polymorphism\Multiple-dispatch.md`中收录了这篇文章。

## cast

`static_cast`、`dynamic_cast`

## 如何获得type？

既然`C++`中区分static type和dynamic type，那么就需要考虑如何来获得两者。

每个[Polymorphic object](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects)有[Static type](https://en.cppreference.com/w/cpp/language/type#Static_type)和[Dynamic type](https://en.cppreference.com/w/cpp/language/type#Dynamic_type)，那对于polymorphic object：

| 问题                       | code                          |
| -------------------------- | ----------------------------- |
| 如何获得**static type**？  | `typeid(decltype(b1)).name()` |
| 如何获得**dynamic type**？ | `typeid(b1).name()`           |

下面的例子源自cppreference [Object](https://en.cppreference.com/w/cpp/language/object)`#`[Polymorphic objects](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects) ：



```C++
#include <iostream>
#include <typeinfo>
struct Base1
{
	// polymorphic type: declares a virtual member
	virtual ~Base1()
	{
	}
};
struct Derived1: Base1
{
	// polymorphic type: inherits a virtual member
};

struct Base2
{
	// non-polymorphic type
};
struct Derived2: Base2
{
	// non-polymorphic type
};

int main()
{
	Derived1 obj1; // object1 created with type Derived1
	Derived2 obj2; // object2 created with type Derived2

	Base1& b1 = obj1; // b1 refers to the object obj1
	Base2& b2 = obj2; // b2 refers to the object obj2

	std::cout << "Expression type of b1: " << typeid(decltype(b1)).name() << '\n'
			<< "Expression type of b2: " << typeid(decltype(b2)).name() << '\n'
			<< "Object type of b1: " << typeid(b1).name() << '\n'
			<< "Object type of b2: " << typeid(b2).name() << '\n'
			<< "Size of b1: " << sizeof b1 << '\n'
			<< "Size of b2: " << sizeof b2 << '\n';
}
// g++ --std=c++11 test.cpp
```

上述程序的输出：

 ```c++
 Expression type of b1: 5Base1
 Expression type of b2: 5Base2
 Object type of b1: 8Derived1
 Object type of b2: 5Base2
 Size of b1: 8
 Size of b2: 1
 ```

对于上述输出，需要注意的是`typeid(b2).name()`的输出是`5Base2`，而不是`Derived2`，这是因为`Base2`和`Derived2`都不是polymorphic type。



### `typeid` and `decltype`

上面的例子已经展示了两者之间的关联了。



## Virtual

参见`C++\Language-reference\Classes\Subtype-polymorphism`。

## Implementation of dynamic type

这其实就是C++ dynamic polymorphism的实现，参见`C++\Language-reference\Classes\Subtype-polymorphism\Implementation`章节。


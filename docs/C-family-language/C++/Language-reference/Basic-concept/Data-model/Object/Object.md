# Object

需要注意的是，本文中的 object 并不是我们 object-oriented programming 中所指的 object；本文的 object是指“region of storage”，在C中，有同样的概念，参见[Objects and alignment](https://en.cppreference.com/w/c/language/object)。显然，它的含义与object-oriented programming 中所指的 object 完全不同，后面为了便于区分，将它称为memory object，将OOP中的object称为OOP object；

此处的 object 是一个 runtime 概念，因为只有当program运行的时候，object才会被创建。

> NOTE: 上面这段话的是不准确的，c++语言充分发挥了compile-time computation，所有有多object是在运行时就已经创建了

object概念对于理解后面的内容非常重要，可以说，后续的很多概念都是建立在object之上的。

## cppreference [Object](https://en.cppreference.com/w/cpp/language/object)

C++ programs create, destroy, refer to, access, and manipulate *objects*.

> NOTE: 在上一节我们描述了C++ program的组成（是静态的、compile-time的），上面这段话描述了C++ program在runtime所做的事情。上述描述和C的[Objects and alignment](https://en.cppreference.com/w/c/language/object)中的描述相同。

An object, in C++ , is a *region of storage* that (until C++14) has

- size (can be determined with [sizeof](https://en.cppreference.com/w/cpp/language/sizeof));

- alignment requirement (can be determined with [alignof](https://en.cppreference.com/w/cpp/language/alignof));

- [storage duration](https://en.cppreference.com/w/cpp/language/storage_duration) (automatic, static, dynamic, thread-local);

- [lifetime](https://en.cppreference.com/w/cpp/language/lifetime) (bounded by storage duration or temporary);

  > NOTE: 上面这段话中的“bounded ”的含义是“由...决定”

- [type](https://en.cppreference.com/w/cpp/language/type);

- value (which may be indeterminate, e.g. for [default-initialized](https://en.cppreference.com/w/cpp/language/default_initialization) non-class types);

- optionally, a [name](https://en.cppreference.com/w/cpp/language/name).

> NOTE: 上述都是对object的属性的描述。这些属性之间是存在着一定的关系的：
>
> ### Memory representation、value and type
>
> **type** 决定了**interpretation** of **memory representation**，进而决定了 **value**
>
> ### Type、size and alignment
>
> type决定了object的size、alignment
>
> ### [Storage duration](https://en.cppreference.com/w/cpp/language/storage_duration) and [lifetime](https://en.cppreference.com/w/cpp/language/lifetime) 
>
> 两者是相关的。

> NOTE: 
>
> ### Interpretion of memory representation
>
> 之所以在此专门添加这个说明，是为了强调“interpretion”这个词语，在cppreference中，这个词语多次出现：
>
> 

The following entities are not objects: value, reference, function, enumerator, type, non-static class member, template, class or function template specialization, namespace, parameter pack, and this.

> NOTE: 上面这段话有些多余，按照[Basic concepts](https://en.cppreference.com/w/cpp/language/basic_concepts)中的描述，object和这些entity之间是并列关系，所以显然它们都不是object。

A *variable* is an object or a reference that is not a non-static data member, that is introduced by a [declaration](https://en.cppreference.com/w/cpp/language/declarations).

> NOTE: “ a reference that is not a non-static data member”这段话是比较绕的，“non-static data member”指的是class的哪些没有使用`static`修饰的data member，“not a non-static data member”就相当于是否定的否定是肯定，即是static data member，则它的意思是：variable可以是static data member。

### Object representation and value representation

> NOTE: 
>
> 所谓的object representation，其实就memory representation。那：如何查看object representation？在工程`computer-arithmetic`的`Bitwise-operation\Binary-representation\Binary-representation.md`中对这个问题进行了说明；
>
> 与object representation密切相关的一个问题是：[endianess](https://en.wikipedia.org/wiki/Endianness)，这在工程hardware的`CPU\Endianess`章节对此进行了说明。

> NOTE: 需要留心的是，原文关于value representation和object representation之间关系的讨论对象是[*TriviallyCopyable*](https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable) types，而不是所有的类型，这一点和C中关于这个话题的讨论是不同的。关于[*TriviallyCopyable*](https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable)，在文章`C++\Language-reference\Basic-concept\Data-model\Object-layout\Object-layout.md`中进行了详细分析。
>
> 仅仅讨论[*TriviallyCopyable*](https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable) types是源于c++语言的复杂性，在文章`C++\Language-reference\Basic-concept\Data-model\Object-layout\Object-layout.md`中对这个问题进行了分析。



```c++
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

> NOTE: 编译: `g++ test.cpp`



#### [Subobjects](https://en.cppreference.com/w/cpp/language/object#Subobjects)

> NOTE: 原文定义了如下概念：
>
> *subobjects*
>
> *complete object*，显然*complete object*和*subobjects*是两个相反的概念
>
> *most derived objects*，所谓most derived，其实可以从class hierarchy来理解，最最顶端的是root class，most derived class，显然就是叶子节点了



#### [Polymorphic objects](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects)

> NOTE: 根据polymorphism，可以将`C++` object分为两类：
>
> - non-polymorphic objects
> - polymorphic objects

Objects of a class type that declares or inherits at least one **virtual function** are **polymorphic objects**. Within each polymorphic object, the implementation stores additional information (in every existing implementation, it is one pointer unless optimized out), which is used by [virtual function](https://en.cppreference.com/w/cpp/language/virtual) calls and by the RTTI features ([dynamic_cast](https://en.cppreference.com/w/cpp/language/dynamic_cast) and [typeid](https://en.cppreference.com/w/cpp/language/typeid)) to determine, at run time, the type with which the object was created, regardless of the expression it is used in.

> NOTE: [Polymorphic objects](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects)不是[*TriviallyCopyable*](https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable) 。
>
> 上面这段话还描述了`C++` runtime polymorphism的实现，这部分内容在ABI中会进行详细的介绍。

For non-polymorphic objects, the **interpretation** of the value is determined from the expression in which the object is used, and is decided at **compile time**.

> NOTE: 每个[Polymorphic object](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects)有[Static type](https://en.cppreference.com/w/cpp/language/type#Static_type)和[Dynamic type](https://en.cppreference.com/w/cpp/language/type#Dynamic_type)，那对于polymorphic object：
>
> 如何获得它的static type？
>
> `typeid(decltype(b1)).name()`
>
> 如何获得它的dynamic type？
>
> `typeid(b1).name()`
>
> 关于这个问题，下面的例子给出了回答：

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

> NOTE: 上述程序的输出：
>
> ```c++
> Expression type of b1: 5Base1
> Expression type of b2: 5Base2
> Object type of b1: 8Derived1
> Object type of b2: 5Base2
> Size of b1: 8
> Size of b2: 1
> ```
>
> 对于上述输出，需要注意的是`typeid(b2).name()`的输出是`5Base2`，而不是`Derived2`，这是因为`Base2`和`Derived2`都不是polymorphic type。

#### [Strict aliasing](https://en.cppreference.com/w/cpp/language/object#Strict_aliasing)



#### [Alignment](https://en.cppreference.com/w/cpp/language/object#Alignment)

> NOTE: 原文将alignment放到object章节，但是我觉得alignment应该是type的属性，所以将alignment放到了Type-system章节中。



## Variable and object

两种都是runtime概念，variable是一种object，但是不是所有的object都是variable。
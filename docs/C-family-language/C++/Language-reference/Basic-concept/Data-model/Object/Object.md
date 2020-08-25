# Object

需要注意的是，本文中的 object 并不是我们 object-oriented programming 中所指的 object；本文的 object是指“region of storage”，在C中，有同样的概念，参见[Objects and alignment](https://en.cppreference.com/w/c/language/object)。显然，它的含义与object-oriented programming 中所指的 object 完全不同，后面为了便于区分，将它称为memory object，将OOP中的object称为OOP object；

此处的 object 是一个 runtime 概念，因为只有当program运行的时候，object才会被创建。

> NOTE: 上面这段话的是不准确的，c++语言充分发挥了compile-time computation，所有有多object是在compile-time时就已经创建了

object概念对于理解后面的内容非常重要，可以说，后续的很多概念都是建立在object之上的。

## cppreference [Object](https://en.cppreference.com/w/cpp/language/object)

C++ programs create, destroy, refer to, access, and manipulate *objects*.

> NOTE: 在上一节我们描述了C++ program的组成（是静态的、compile-time的），上面这段话描述了C++ program在runtime所做的事情。上述描述和C的[Objects and alignment](https://en.cppreference.com/w/c/language/object)中的描述相同。
>
> 下面总结了对object的manipulation:
>
> - create
> - destroy
> - refer to
> - access
>
> - stored in arrays
> - copied
> - assigned
>
> 对于非object，无法执行全部上述这些manipulation。

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
> **type** 决定了**interpretation** of **memory representation**，进而决定了 **value**。关于这个观点，在cppreference [reinterpret_cast conversion](https://en.cppreference.com/w/cpp/language/reinterpret_cast)中的描述是可以佐证的：
>
> > Converts between **types** by **reinterpreting** the underlying **bit** pattern.
>
> 
>
> ### Type、size and alignment
>
> type决定了object的size、alignment；
>
> 通过上面的描述可以看出，type是核心概念。
>
> 
>
> ### [Storage duration](https://en.cppreference.com/w/cpp/language/storage_duration) and [lifetime](https://en.cppreference.com/w/cpp/language/lifetime) 
>
> 两者是密切相关的。



> NOTE: 
>
> ### Interpretion of memory representation
>
> 之所以在此专门添加这个说明，是为了强调“interpretion”这个词语，在cppreference中，这个词语多次出现：
>
> - 在[Polymorphic objects](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects)段：
>
> > For non-polymorphic objects, the **interpretation** of the **value** is determined from the expression in which the object is used, and is decided at **compile time**.
>
> - [reinterpret_cast](https://en.cppreference.com/w/cpp/language/reinterpret_cast) conversion
>
> > Converts between types by reinterpreting the underlying bit pattern.

The following entities are not objects: value, reference, function, enumerator, type, non-static class member, template, class or function template specialization, namespace, parameter pack, and this.

> NOTE: 上面这段话有些多余，按照[Basic concepts](https://en.cppreference.com/w/cpp/language/basic_concepts)中的描述，object和这些entity之间是并列关系，所以显然它们都不是object。
>
> 需要注意的是：reference不是object，这是它和pointer的重要差别，在文章`C++\Language-reference\Reference\Pointer-VS-reference.md`中引用了上述内容。
>
> 上面这些entity都不是object，所以它们不能够像object那样被manipulate（关于object的manipulation，参见第一段），关于这一段，在下面章节中进行了描述：
>
> cppreference [Pointer declaration#Pointer to function](https://en.cppreference.com/w/cpp/language/pointer#Pointers_to_functions):
>
> > Unlike functions or references to functions, pointers to **functions** are objects and thus can be stored in arrays, copied, assigned, etc.
>
> cpprefrence [Reference declaration](https://en.cppreference.com/w/cpp/language/reference)
>
> > References are not **objects**; they do not necessarily occupy storage, although the compiler may allocate storage if it is necessary to implement the desired semantics (e.g. a non-static data member of reference type usually increases the size of the class by the amount necessary to store a memory address).
> >
> > Because references are not objects, there are no arrays of references, no pointers to references, and no references to references:

A *variable* is an object or a reference that is not a non-static data member, that is introduced by a [declaration](https://en.cppreference.com/w/cpp/language/declarations).

> NOTE: “ a reference that is not a non-static data member”这段话是比较绕的，“non-static data member”指的是class的哪些没有使用`static`修饰的data member，“not a non-static data member”就相当于是否定的否定是肯定，即是static data member，则它的意思是：variable可以是static data member。

### Object representation and value representation

> NOTE: 
>
> 所谓的object representation，其实就memory representation。
>
> ### 如何查看object representation？
>
> 在工程`computer-arithmetic`的`Bitwise-operation\Binary-representation\Binary-representation.md`中对这个问题进行了说明；
>
> 按照“Serialization and deserialization”节的说法，这个过程叫做Serialization。
>
> ### Memory layout
>
> 前面说明了object representation的含义，现在我们思考这个问题：C++ compiler如何来编排object的memory layout（后面简称为**object layout**）？
>
> 其实这个问题涉及到了C++ ABI，下面是object layout需要考虑的：
>
> - [endianess](https://en.wikipedia.org/wiki/Endianness)，这在工程hardware的`CPU\Endianess`章节对此进行了说明
>
> - alignment
> - C++提供的很多高级特性的实现，比如下面两个：
>
>   - polymorphic type，polymorphic type有[virtual functions](https://en.cppreference.com/w/cpp/language/virtual)，需要RTTI、virtual method table
>   - [virtual base classes](https://en.cppreference.com/w/cpp/language/derived_class#Virtual_base_classes)
> - compiler optimization
> - subobject（在cppreference [Derived classes](https://en.cppreference.com/w/cpp/language/derived_class)中，有对它的讨论）
> - platform
> - ......
>
> 需要考虑的问题非常多，处于各种考虑，C++标准并没有对object layout的方方面面都进行统一规定，而是将一些留给了C++ implementation去自由地选择。关于这一点，在文章microsoft [Trivial, standard-layout, POD, and literal types](https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019)中进行了说明：
>
> > The term *layout* refers to how the members of an object of class, struct or union type are arranged in memory. In some cases, the layout is well-defined by the language specification. But when a class or struct contains certain C++ language features such as [virtual base classes](https://en.cppreference.com/w/cpp/language/derived_class#Virtual_base_classes), [virtual functions](https://en.cppreference.com/w/cpp/language/virtual), members with different access control, then the compiler is free to choose a **layout**. That **layout** may vary depending on what optimizations are being performed and in many cases the object might not even occupy a contiguous area of memory. 
>
> 关于C++标准对object layout的定义参见:
>
> - `C++\Language-reference\Basic-concept\Data-model\Object\Object-layout`中进行了描述。
>
> 关于implementation-defined的object layout参见:
>
> - `C-and-C++\From-source-code-to-exec\ABI\Itanium-Cpp-ABI`中进行了描述。
>
> 关于polymorphic type，参见：
>
> - `C++\Language-reference\Basic-concept\Type-system\Type-system\Type-system.md#Polymorphic type`



> NOTE: 原文关于value representation和object representation之间关系的讨论对象是[*TriviallyCopyable*](https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable) types，而不是所有的type，这一点和C中关于这个话题的讨论是不同的，C中讨论并没有区分type，也就是说C中所有的type都可以按照其中讨论的value representation和object representation；
>
> C++中关于value representation和object representation之间关系的讨论对象仅仅局限于[*TriviallyCopyable*](https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable) types，而不是所有的type这是源于C++语言的复杂性，C++标准貌似没有描述一些C++ feature的实现，所以标准无法统一地描述各种type的object representation和value representation之间的关系。
>
> 关于trivial type，参见：
>
> - `C++\Language-reference\Basic-concept\Data-model\Object-layout\Trivial.md`
>
> 



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

An object can have *subobjects*. These include

- member objects
- base class subobjects（在cppreference [Derived classes](https://en.cppreference.com/w/cpp/language/derived_class)中对这个进行了介绍）；
- array elements

> NOTE: subobject是典型的sub structure（参见工程discrete的`Guide\Relation-structure-computation\Computation\Induction-and-Recursion\Recursion\Recursive-definition.md`章节），它是containing关系。

#### [Polymorphic objects](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects)

> NOTE: 根据polymorphism，可以将`C++` object分为两类：
>
> - non-polymorphic objects
> - polymorphic objects

Objects of a class type that declares or inherits at least one **virtual function** are **polymorphic objects**. Within each polymorphic object, the implementation stores additional information (in every existing implementation, it is one pointer unless optimized out), which is used by [virtual function](https://en.cppreference.com/w/cpp/language/virtual) calls and by the RTTI features ([dynamic_cast](https://en.cppreference.com/w/cpp/language/dynamic_cast) and [typeid](https://en.cppreference.com/w/cpp/language/typeid)) to determine, at run time, the type with which the object was created, regardless of the expression it is used in.

> NOTE: [Polymorphic objects](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects)不是[*TriviallyCopyable*](https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable) ，其实这是因为polymorphic type不是trivial type。
>
> 上面这段话还描述了`C++` runtime polymorphism的实现，这部分内容在ABI中会进行详细的介绍。

For non-polymorphic objects, the **interpretation** of the value is determined from the expression in which the object is used, and is decided at **compile time**.

> NOTE: 原文中的例子移到了`C++\Language-reference\Basic-concept\Type-system\Type-system\Type-system.md#Polymorphic  type`中

#### [Strict aliasing](https://en.cppreference.com/w/cpp/language/object#Strict_aliasing)



#### [Alignment](https://en.cppreference.com/w/cpp/language/object#Alignment)

> NOTE: 将alignment放到了`C-and-C++\From-source-code-to-exec\ABI\Alignment`章节中。



## Variable and object

两种都是runtime概念，variable是一种object，但是不是所有的object都是variable。

c++中variable的概念和object的概念密切相关，在下面文章中描述了此：

learncpp [1.3 — Introduction to variables](https://www.learncpp.com/cpp-tutorial/introduction-to-variables/)



## Serialization and deserialization

序列化与反序列化。

| 概念            | 解释                                                | 需要考虑的问题                                               |
| --------------- | --------------------------------------------------- | ------------------------------------------------------------ |
| serialization   | 给定一个object，得到它的object representation       |                                                              |
| deserialization | 给定一个memory region，按照指定type进行interpretion | - memory region是否满足type的[Alignment](https://en.cppreference.com/w/cpp/language/object#Alignment) requirement<br>- [Strict aliasing](https://en.cppreference.com/w/cpp/language/object#Strict_aliasing) |

C++中，serialization and deserialization都是通过`reinterpret_cast`来实现的。

从上面可以看出，这些内容是密切相关的：

- deserialization
- `reinterpret_cast`
- [Alignment](https://en.cppreference.com/w/cpp/language/object#Alignment) 
- [Strict aliasing](https://en.cppreference.com/w/cpp/language/object#Strict_aliasing)

可以这样来总结它们之间的关联：在C++中，一般通过`reinterpret_cast`来进行deserialization，在进行deserialization的时候，需要[Strict aliasing](https://en.cppreference.com/w/cpp/language/object#Strict_aliasing)，其中非常重要的一点是：满足type的[Alignment](https://en.cppreference.com/w/cpp/language/object#Alignment) requirement。
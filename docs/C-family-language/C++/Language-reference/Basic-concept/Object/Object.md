# Object



## What is object?

cppreference中的"object"是指“**region of storage**”，在C中，有同样的概念，参见creference [Objects and alignment](https://en.cppreference.com/w/c/language/object)。

cppreference中的"object"是一个 runtime 概念，因为只有当program运行的时候，object才会被创建。

> NOTE: 上面这段话的是不准确的，c++语言充分发挥了compile-time computation，所有有多object是在compile-time时就已经创建了

object概念对于理解后面的内容非常重要，因为后续的很多概念都是建立在object之上的。

### Storage

cppreference中，习惯使用"storage"这个词语，它其实是对memory的一个更加抽象的描述，即它摒弃了实现细节，这是programming language的language reference中惯常的做法，因为对于想C++这样的general purpose programming language，它并不specific to某种具体的实现。所以读者需要对这种表达方式习惯。



### Object and OOP object

需要注意的是，cppreference中的 object 并不是我们 object-oriented programming 中所指的 object（参见`Theory\Programming-paradigm\Object-oriented-programming`）；后面为了便于区分，当两者同时出现的时候，将OOP中的object称为OOP object，将object称为memory object；默认情况下，object指的是memory object；

## Object and storage

本节概括object 和 storage的关系: 

1) An object is a region of storage: 一个object占据a region of  storage

2) Storage reuse: a region of storage can be reused

> 在下面章节中，也对storage reuse进行了讨论: 
>
> [Lifetime#Storage reuse](https://en.cppreference.com/w/cpp/language/lifetime#Storage_reuse)
>
> cppreference [Object#Implicit creation](https://en.cppreference.com/w/cpp/language/object)

3) cppreference [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime): Lifetime of an object is equal to or is nested within the lifetime of its storage, see [storage duration](https://en.cppreference.com/w/cpp/language/storage_duration).

4) cppreference [Object](https://en.cppreference.com/w/cpp/language/object): [lifetime](https://en.cppreference.com/w/cpp/language/lifetime) (bounded by storage duration or temporary);

> NOTE: 在lifetime中，对对3)、4)进行说明。

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

### Object property

> NOTE: 原文并没有本节的标题，这个标题是我添加的，目的是便于后续引用

An object, in C++ , is a *region of storage* that (until C++14) has

| property                                                     |         | 注解                          |
| ------------------------------------------------------------ | ------- | ----------------------------- |
| size (can be determined with [sizeof](https://en.cppreference.com/w/cpp/language/sizeof)) | static  |                               |
| alignment requirement (can be determined with [alignof](https://en.cppreference.com/w/cpp/language/alignof)) | static  |                               |
| [type](https://en.cppreference.com/w/cpp/language/type)      | static  |                               |
| value (which may be indeterminate, e.g. for [default-initialized](https://en.cppreference.com/w/cpp/language/default_initialization) non-class types); | static  |                               |
| optionally, a [name](https://en.cppreference.com/w/cpp/language/name). | static  | temporary object没有name      |
| [storage duration](https://en.cppreference.com/w/cpp/language/storage_duration) | dynamic |                               |
| [lifetime](https://en.cppreference.com/w/cpp/language/lifetime) (bounded by storage duration or temporary); | dynamic | “bounded ”的含义是“由...决定” |

> NOTE: 
>
>  [lifetime](https://en.cppreference.com/w/cpp/language/lifetime) and [Storage duration](https://en.cppreference.com/w/cpp/language/storage_duration) 是密切相关的，在`C++\Language-reference\Basic-concept\Data-model\Object\Object-lifetime-and-storage-duration`中对此进行了描述。
>
> 关于static property，在下面的"Type determines everything"节中进行了讨论。



The following entities are not objects: value, reference, function, enumerator, type, non-static class member, template, class or function template specialization, namespace, parameter pack, and `this`.

> NOTE: 上面这段话有些多余，按照[Basic concepts](https://en.cppreference.com/w/cpp/language/basic_concepts)中的描述，object和这些entity之间是并列关系，所以显然它们都不是object。
>
> 需要注意的是：reference不是object，这是它和pointer的重要差别，在文章`C++\Language-reference\Reference\Pointer-VS-reference`中引用了上述内容。
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

### Variable

A *variable* is an object or a reference that is not a non-static data member, that is introduced by a [declaration](https://en.cppreference.com/w/cpp/language/declarations).

> NOTE: “ a reference that is not a non-static data member”这段话是比较绕的，“non-static data member”指的是class的哪些没有使用`static`修饰的data member，“not a non-static data member”就相当于是否定的否定是肯定，即是static data member，则它的意思是：variable可以是static data member。



> #### Variable and object
>
> 两种都是runtime概念，variable是一种object，但是不是所有的object都是variable（variable还包括reference）。
>
> c++中variable的概念和object的概念密切相关，在下面文章中描述了此：
>
> learncpp [1.3 — Introduction to variables](https://www.learncpp.com/cpp-tutorial/introduction-to-variables/)
>



### Object creation

> NOTE: 原文中将object creation方式分为如下两大类:
>
> - Explicit creation
> - Implicit creation
>
> 另外一种分类标准是: 根据object是否由programmer主动(active)创建:
>
> |                     | 注解                                                         |
> | ------------------- | ------------------------------------------------------------ |
> | active creation     | 主动创建;<br>C++ language赋予programmer的创建object的所有的方式，包括了下面的Explicit creation、Implicit creation中描述的各种方式，temporary除外 |
> | non-active creation | 非主动创建;<br>object不是由programmer主动create的，而是在expression evaluation过程中自动创建的，C++将其称为temporary |
>
> 原文本节对C++ language赋予programmer主动地create object的所有方式进行了非常全面的总结。



#### Explicit creation



| classification                                               | 注解                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [definitions](https://en.cppreference.com/w/cpp/language/definition) | declaration不创建 object，这是它和definition的重要区别       |
| [new-expressions](https://en.cppreference.com/w/cpp/language/new) |                                                              |
| [throw-expressions](https://en.cppreference.com/w/cpp/language/throw) |                                                              |
| changing the active member of a [union](https://en.cppreference.com/w/cpp/language/union) |                                                              |
| evaluating expressions that require [temporary objects](https://en.cppreference.com/w/cpp/language/lifetime#Temporary_object_lifetime) | 意思是: 这类object是在expression evaluation过程中自动创建的，C++将其称为temporary |

> NOTE: [Temporary object](https://en.cppreference.com/w/cpp/language/lifetime#Temporary_object_lifetime) 是否属于 explicit creation？是的，上述表格的最后一行就是对这种情况的描述。
>
> 在[cppreference Lifetime#Temporary object lifetime](https://en.cppreference.com/w/cpp/language/lifetime#Temporary_object_lifetime)中介绍了creation of temporary objects。

#### Implicit creation

> NOTE: 
>
> object本质上是 *a region of storage* ，因此宽泛的说: 任何storage都可以用作object。Implicit creation本质上其实是给定 a region of storage，然后将它deserialization为指定type的object（关于deserialization，参见后面的"Serialization and deserialization"章节），这种做法是非常类似于C中的做法；
>
> 原文是根据storage来进行分类的，下面是我使用table的方式重新进行组织的。



> NOTE: 只有 [implicit-lifetime types](https://en.cppreference.com/w/cpp/language/lifetime#Implicit-lifetime_types) 才能够implicit creation。implicit creation和object layout是否有关联？我觉得是有关联的，原因如下:
>
> 1) `C++\Language-reference\Basic-concept\Data-model\Object\Object-layout\Object-layout.md`中描述的trivial type是可以使用`memcpy`的，并且其中还讨论了lifetime
>
> 2)  [implicit-lifetime types](https://en.cppreference.com/w/cpp/language/lifetime#Implicit-lifetime_types) 中提及了trivial
>
> 所以，我觉得 [implicit-lifetime types](https://en.cppreference.com/w/cpp/language/lifetime#Implicit-lifetime_types) 应该和  [TriviallyCopyable](https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable) 有关。



Objects of [implicit-lifetime types](https://en.cppreference.com/w/cpp/language/lifetime#Implicit-lifetime_types) can also be implicitly created by

| objects are created in                          | storage                                                      | 注解                   |
| ----------------------------------------------- | ------------------------------------------------------------ | ---------------------- |
| the array                                       | an array of type `char`, `unsigned char`, or [`std::byte`](https://en.cppreference.com/w/cpp/types/byte), (since C++17) |                        |
| the allocated storage                           | allocating functions: <br>- [operator new](https://en.cppreference.com/w/cpp/memory/new/operator_new)  <br/>- [`operator new[]`](https://en.cppreference.com/w/cpp/memory/new/operator_new) <br/>- [std::malloc](https://en.cppreference.com/w/cpp/memory/c/malloc) <br/>- [std::calloc](https://en.cppreference.com/w/cpp/memory/c/calloc) <br/>- [std::realloc](https://en.cppreference.com/w/cpp/memory/c/realloc) <br/>- [std::aligned_alloc](https://en.cppreference.com/w/cpp/memory/c/aligned_alloc) | 下面的例子就是这种情况 |
| the destination region of storage or the result | object representation copying functions: <br>- [std::memcpy](https://en.cppreference.com/w/cpp/string/byte/memcpy) <br>- [std::memmove](https://en.cppreference.com/w/cpp/string/byte/memmove) <br>- `std::bit_cast` | 其实是storage reuse    |

```C++
#include <cstdlib>
#include <iostream>
struct X
{
	int a, b;
};
X* MakeX()
{
	// One of possible defined behaviors:
	// the call to std::malloc implicitly creates an object of type X
	// and its subobjects a and b, and returns a pointer to that X object
	X *p = static_cast<X*>(std::malloc(sizeof(X)));
	p->a = 1;
	p->b = 2;
	return p;
}
int main(void)
{

	X *x = MakeX();
	std::cout << x->a << std::endl;
	std::cout << x->b << std::endl;
}
// g++ test.cpp
```

> NOTE: 是否需要考虑alignment问题？

### Object representation and value representation

For an object of type `T`, *object representation* is the sequence of `sizeof(T)` objects of type `unsigned char` (or, equivalently, [`std::byte`](https://en.cppreference.com/w/cpp/types/byte)) beginning at the same address as the `T` object.

> NOTE: 
>
> 前面我们已经知道了，object本质上是*a region of storage*，所谓的object representation，其实就是 *a region of storage*，后面我们将这个过程称为**Serialization** （关于**Serialization** ，参见后面的"Serialization and deserialization"章节））



> NOTE: 
>
> ### 如何查看object representation？
>
> 在工程`computer-arithmetic`的`Bitwise-operation\Binary-representation\Binary-representation`中对这个问题进行了说明；
>
> 按照“Serialization and deserialization”节的说法，这个过程叫做Serialization。



> NOTE: 原文关于value representation和object representation之间关系的讨论对象是[*TriviallyCopyable*](https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable) types，而不是所有的type，这一点和C中关于这个话题的讨论是不同的，C中讨论并没有区分type，也就是说C中所有的type都可以按照其中讨论的value representation和object representation；
>
> C++中关于value representation和object representation之间关系的讨论对象仅仅局限于[*TriviallyCopyable*](https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable) types，而不是所有的type这是源于C++语言的复杂性，C++标准没有描述一些C++ feature的实现，所以标准无法统一地描述各种type的object representation和value representation之间的关系。
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



### [Subobjects](https://en.cppreference.com/w/cpp/language/object#Subobjects)

> NOTE: 原文定义了如下概念：
>
> *subobjects*
>
> *complete object*，显然*complete object*和*subobjects*是两个相反的概念
>
> *most derived objects*，所谓most derived，其实可以从class hierarchy来理解，最最顶端的是root class，most derived class，显然就是叶子节点了

An object can have *subobjects*. These include

|                       | 注解                                                         |
| --------------------- | ------------------------------------------------------------ |
| member objects        | 这源于C++对OOP支持                                           |
| base class subobjects | 这源于C++对OOP支持; <br>在cppreference [Derived classes](https://en.cppreference.com/w/cpp/language/derived_class)中对这个进行了介绍 |
| array elements        |                                                              |



> NOTE: subobject是典型的sub structure（参见工程discrete的`Guide\Relation-structure-computation\Computation\Induction-and-Recursion\Recursion\Recursive-definition.md`章节），它是containing关系。

### [Polymorphic objects](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects)

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

### [Strict aliasing](https://en.cppreference.com/w/cpp/language/object#Strict_aliasing)

> NOTE: 将strict aliasing放到了`C-family-language\C-and-C++\Pointer-array-alias\Alias`章节中。

### [Alignment](https://en.cppreference.com/w/cpp/language/object#Alignment)

> NOTE: 将alignment放到了`C-and-C++\From-source-code-to-exec\ABI\Alignment`章节中。



## Type determines everything

在"Object property"节中描述了object的属性，很多都是由type决定的，这验证了“Type determines everything”（参见`Theory\Type-system\index.md`）。

这些属性之间是存在着一定的关系的，下面对此进行说明

### Type determines the interpretion of memory representation, and further determine value

**type** 决定了 **interpretation** of **memory representation**，进而决定了 **value**。关于这个观点，在cppreference [reinterpret_cast conversion](https://en.cppreference.com/w/cpp/language/reinterpret_cast)中的描述是可以佐证的：

Converts between **types** by **reinterpreting** the underlying **bit** pattern.

在工程hardware的，`CPU\Endianess\Endianness.md`中，我们将会进一步看到，“interpretion of memory representation”还涉及到endian，这在大多数情况下，programmer无需关注endian。

#### Aliase to an existing object

C++、C非常灵活，对于同一个object，允许

- 使用多种type进行interpret，即reinterpret（重解释），一般通过pointer + `rereinterpret_cast`来实现
- 使用多个name进行refer to，一般通过reference来实现

对于aliase to an existing object，C++、C标准都进行了严格的定义，这在strict aliasing中进行了详细介绍。如果programmer不遵循strict aliasing，则会导致undefined behavior。

C++中，reinterpret由`rereinterpret_cast`来实现，所它将strict aliasing的内容和`rereinterpret_cast`放到了一起。

需要注意的是：上述两种方式，都仅仅是alias，都不会重新创建一个新的object（原object的副本）。

参见：

- alias: `C++\Language-reference\Alias`
- `rereinterpret_cast`: `C++\Language-reference\Basic-concept\Type-system\Type-conversion\Cast-operator`
- reference: `C++\Language-reference\Reference`
- pointer: `C-family-language\C-and-C++\Pointer-array\Pointer`

### Type determines size and alignment

type决定了object的size、alignment；



## Interpretion of memory representation

之所以在此专门添加这个说明，是为了强调“interpretion”这个词语，在cppreference中，这个词语多次出现：

- 在[Polymorphic objects](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects)段：

  > For non-polymorphic objects, the **interpretation** of the **value** is determined from the expression in which the object is used, and is decided at **compile time**.

- [reinterpret_cast](https://en.cppreference.com/w/cpp/language/reinterpret_cast) conversion

  > Converts between types by reinterpreting the underlying bit pattern.



## Serialization and deserialization

序列化与反序列化。按照C++中的说法，deserialization也可以叫做reinterpret。

| 概念            | 解释                                                | 需要考虑的问题                                               |
| --------------- | --------------------------------------------------- | ------------------------------------------------------------ |
| serialization   | 给定一个object，得到它的object representation       | 一般使用**byte type**                                        |
| deserialization | 给定一个memory region，按照指定type进行interpretion | - memory address是否满足type的[Alignment](https://en.cppreference.com/w/cpp/language/object#Alignment) requirement<br>- [Strict aliasing](https://en.cppreference.com/w/cpp/language/object#Strict_aliasing) |

C++中，serialization and deserialization都是可以通过`reinterpret_cast`来实现的。

从上面可以看出，这些内容是密切相关的：

- deserialization
- `reinterpret_cast`
- [Strict aliasing](https://en.cppreference.com/w/cpp/language/object#Strict_aliasing)
- [Alignment](https://en.cppreference.com/w/cpp/language/object#Alignment) 

可以这样来总结它们之间的关联：在C++中，一般通过`reinterpret_cast`来进行deserialization，在进行deserialization的时候，需要考虑[Strict aliasing](https://en.cppreference.com/w/cpp/language/object#Strict_aliasing)，strict aliasing的目的是：满足type的[Alignment](https://en.cppreference.com/w/cpp/language/object#Alignment) requirement。



参见：

- byte type：`C-family-language\C++\Language-reference\Basic-concept\Type-system\Type-system\Byte-type.md`
- strict aliasing：`C-family-language\C-and-C++\Pointer-array-alias\Alias`
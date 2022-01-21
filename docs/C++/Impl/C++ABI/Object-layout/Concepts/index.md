# Object layout concepts

C++标准对object layout的定义是有变更的，下面首先反思变更历程，然后再来具体的思考各种object layout。

参考文章: 

1、wikipedia [C++11#Modification to the definition of plain old data](https://en.wikipedia.org/wiki/C++11#Modification_to_the_definition_of_plain_old_data)

2、microsoft [Trivial, standard-layout, POD, and literal types](https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019)

3、andreasfertig [C++20: Aggregate, POD, trivial type, standard layout class, what is what](https://andreasfertig.blog/2021/01/cpp20-aggregate-pod-trivial-type-standard-layout-class-what-is-what/)

## andreasfertig [C++20: Aggregate, POD, trivial type, standard layout class, what is what](https://andreasfertig.blog/2021/01/cpp20-aggregate-pod-trivial-type-standard-layout-class-what-is-what/) 

### A trivial type

In short, a trivial type is a class or struct for which the compiler provides all the special members, either implicitly or because they are **explicitly defaulted** by us. Once we provide our own **default constructor** for a class, such a type is no longer a trivial type. Another property of a trivial type is that such a type occupies a **contiguous memory area**, which makes it memcopy-able, and it supports `static` initialization.

> NOTE: 
>
> 一、上述总结非常好
>
> 二、需要注意的是: 上面所说的是  "**default constructor** "，trivial type是允许"user-provided constructor, as long as this is not the default constructor"，这在后面进行了强调。

We can copy a trivial type into a `char` or `unsigned char` array and back. Alignment and also the size of the type may be different from `char` due to alignment and padding rules.

> NOTE: 
>
> 这其实就是前面所说的 "memcopy-able"

#### Mixed **access specifiers** and compiler reorder

One important property of **trivial type** is that they can have mixed **access specifiers**. We all know the rule that classes and structs are have the same layout order in memory as the order of the defined data members. However, whenever we have different access specifiers in a class, the standard specifies that, in this case, the order is unspecified.

```C++
struct Unspecified {
public:
    int a;

private:
    int b;

public:
    int c;
};
```

This allows a compiler, at least in theory, because I don't know one compiler which takes the opportunity of this, to reorder the data members, of a struct. One way would be that the struct starts with all `public` members followed by all `private` data members:

```C++
struct Unspecified {
public:
    int a;
    int c; // A Compiler reordered c

private:
    int b;
};
```

As shown in `|#A]`, this reordering causes an incompatibility with C, which doesn't know about `private` and hence that reordering rule. The result is that a **trivial type** is not usable in C code.

#### User-provid non-default constructor

When you have read the above carefully, you noticed that I talked only about special members, which we must either default or let the compiler provide. However, a **trivial type** can still have a **user-provided constructor**, as long as this is not the **default constructor**.

```C++
#include <new>
#include <iostream>
#include <type_traits>

struct Trivial {
	int a;

	Trivial() = default;
	Trivial(int _a)// A user provided constructor
		: a{ _a + 1 }
	{}
};

static_assert( std::is_trivial_v<Trivial> );

int main()
{
}
// g++ test.cpp --std=c++17 -pedantic -Wall -Wextra -Werror

```

### A standard-layout class

> NOTE: 
>
> "standard-layout"所强调的是memory layout上能够和C保持一致

The term layout refers to the arrangement of members of classes, structs, or unions in memory. A standard-layout class defines a type, which does not use certain specific C++ features that are not available in C. Such a type is memcopy-able, and its layout is defined in a way that the same type can be used in a C program. So in more general speak, a standard-layout class (or type) is compatible with C and can be exchanged over a C-API.

With all that said, we have a standard-layout class if it does not contain any language elements which are not present in C. Here is a more complete definition. The numbers refer to the code example that follows:

- No `virtual` base classes;
- No `virtual` functions;
- No reference-members;
- The same access control for all non-static data members. Right this is different from the definition of a trivial type, we can have a `struct` with only `private` or `protected` members, and it is still standard-layout;
- Has a standard-layout base class or classes;
- All non-`static` data members are standard-layout as well;
- Meets one of these conditions:
- no non-`static` data member in the most-derived class and no more than one base class with non-`static` data members, or
- has no base classes with non-`static` data members

Please note that at this point, we talk about the layout in the memory and the interoperability with C. What you do not see in the definition above is that a standard-layout class could have special members. They do not change the memory layout. Special members only help to initialize an object. Despite that C has no special members, we can have them in C++ in a standard-layout type because it is just about the layout, nothing else.

> NOTE: 
>
> 上述强调了standard layout的含义

### POD

Before C++20, we had the definition of a POD type. The specification was that a POD is a type that is **trivial** and **standard-layout**. With C++20, the definition of POD, as well as the type-trait `std::is_pod`, is gone. No worries, your favorite STL vendor will for sure provide the **type-trait** for some time before it actually gets removed.

The idea of a POD was that it supports two distinct properties:

- we can compile a POD in C++ and still use it in a C program, as it has the same memory layout in both languages (meet by standard-layout);
- a POD supports static initialization (meet by trivial type).

While a **standard-layout** type has a C compatible memory layout, it can have a user-defined default constructor. This is something C doesn't have. Hence we need the second property, a trivial type. As we learned above, such a type is default constructible the same way as a C struct.

As far as the C++20 standard is concerned, the term POD no longer exists. POD is replaced by standard-layout and trivial type. As a consequence, the type-trait `std::is_pod` is deprecated in C++20, and you are encouraged to use the two type-traits `std::is_trivial` and `std::is_standard_layout`.

### Aggregate

> NOTE: 
>
> 暂未学习

### Properties Overview

| Type            | memcopy-able | C compatible memory layout |
| --------------- | ------------ | -------------------------- |
| Trivial type    | Yes          | No                         |
| Standard-layout | Yes          | Yes                        |
| Aggregate       | Yes          | maybe                      |

### Why should you care?

Why do you need to differentiate? One apparent reason is compatibility with C. Should that be the purpose, you must ensure that the type in question is trivial and standard-layout. For data exchange via network or file, you can use a type that satisfies only trivial or standard-layout. I would recommend either aiming for both or at least standard-layout. Because with only trivial, you risk that future compiler's or a different compiler on the other side, do rearrange data members, and then a receiver may have a different layout understanding than the sender.

> NOTE: 
>
> 一、在下面的 microsoft [Trivial, standard-layout, POD, and literal types](https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019) 的第一段中也说明了 Why should you care? 总的来说，有如下原因:
>
> 1、cross language
>
> 2、cross network: serialization:
>
> "For data exchange via network or file, you can use a type that satisfies only trivial or standard-layout. I would recommend either aiming for both or at least standard-layout. Because with only trivial, you risk that future compiler's or a different compiler on the other side, do rearrange data members, and then a receiver may have a different layout understanding than the sender"

## microsoft [Trivial, standard-layout, POD, and literal types](https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019)

> NOTE: 这篇文章，主要是对这些概念进行详细的分析

The term *layout* refers to how the members of an object of class, struct or union type are arranged in memory. In some cases, the layout is well-defined by the language specification. But when a class or struct contains certain C++ language features such as [virtual base classes](https://en.cppreference.com/w/cpp/language/derived_class#Virtual_base_classes), [virtual functions](https://en.cppreference.com/w/cpp/language/virtual), members with different access control, then the compiler is free to choose a **layout**. That **layout** may vary depending on what optimizations are being performed and in many cases the object might not even occupy a contiguous area of memory. 

> NOTE: 
>
> 一、第一段话对object memory layout进行了非常好的阐述
>
> 二、上诉“member”所指为data member
>
> 三、上面作者仅仅给出了结论，但是并没有给出例子，其实读者一时半会可能无法理解，在 andreasfertig [C++20: Aggregate, POD, trivial type, standard layout class, what is what](https://andreasfertig.blog/2021/01/cpp20-aggregate-pod-trivial-type-standard-layout-class-what-is-what/) 中给出了 "members with different access control" 时，导致object layout被optimization的情况，结合其中的例子是便于理解的。

For example, if a class has virtual functions, all the instances of that class might share a single virtual function table. Such types are very useful, but they also have limitations. Because the layout is undefined they cannot be passed to programs written in other languages, such as C, and because they might be non-contiguous they cannot be reliably copied with fast low-level functions such as `memcopy`, or serialized over a network.

> NOTE: 
>
> 一、上面这段话其实解释了 "Why should you care?"

To enable compilers as well as **C++ programs** and **metaprograms** to reason about the suitability of any given type for operations that depend on a particular memory layout, C++14 introduced three categories of simple classes and structs: *trivial*, *standard-layout*, and *POD* or Plain Old Data. The Standard Library has the function templates [`is_trivial`](https://en.cppreference.com/w/cpp/types/is_trivial) , [`is_standard_layout`](https://en.cppreference.com/w/cpp/types/is_standard_layout)  and [`is_pod`](https://en.cppreference.com/w/cpp/types/is_pod) that determine whether a given type belongs to a given category.

> NOTE: 
>
> 一、上述metaprogram所指的是c++的template metaprogramming，参见`C-family-language\C++\Language-reference\Template\Template-metaprogramming.md`

### [Trivial types](https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019#trivial-types)

When a class or struct in C++ has compiler-provided or explicitly defaulted **special member functions**, then it is a **trivial type**. 

> NOTE: 上面这段话中的“explicitly defaulted special member functions”，所指为类似如下的做法：
>
> `Trivial2() = default;`

Trivial types have a

1、[trivial default constructor](https://en.cppreference.com/w/cpp/language/default_constructor#Trivial_default_constructor), 

2、[trivial copy constructor](https://en.cppreference.com/w/cpp/language/copy_constructor#Trivial_copy_constructor),

3、 [trivial copy assignment operator](https://en.cppreference.com/w/cpp/language/copy_assignment#Trivial_copy_assignment_operator) 

4、[trivial destructor](https://en.cppreference.com/w/cpp/language/destructor#Trivial_destructor). 

In each case, *trivial* means the constructor/operator/destructor is not user-provided and belongs to a class that has

- no virtual functions or [virtual base classes](https://en.cppreference.com/w/cpp/language/derived_class#Virtual_base_classes),
- no base classes with a corresponding non-trivial constructor/operator/destructor
- no data members of class type with a corresponding non-trivial constructor/operator/destructor

> NOTE: 上面这段话解释了trivial的含义



The following examples show trivial types. In `Trivial2`, the presence of the `Trivial2(int a, int b)` constructor requires that you provide a **default constructor**. For the type to qualify as **trivial**, you must **explicitly default that constructor**.

> NOTE: 
>
> 一、trivial type是需要能够static initialization的

```c++
#include <iostream>
#include <type_traits>

struct Trivial
{
   int i;
private:
   int j;
};

struct Trivial2
{
   int i;
   Trivial2(int a, int b) : i(a), j(b) {}
   Trivial2() = default;
private:
   int j;   // Different access control
};

int main()
{
    std::cout << std::boolalpha;
    std::cout << std::is_trivial<Trivial>::value << '\n';
    std::cout << std::is_trivial<Trivial2>::value << '\n';
}
// g++ --std=c++11 test.cpp

```

> NOTE: 
>
> 输出如下:
>
> ```c++
> true
> true
> 
> ```
>
> 



### [Standard layout types](https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019#standard-layout-types)

> NOTE: 
>
> 原文的内容给出了很多的条件，其实并不需要特殊的记忆。

```c++
#include<type_traits>
#include<iostream>
struct SL
{
	// All members have same access:
	int i;
	int j;
	SL(int a, int b)
			: i(a), j(b)
	{
	} // User-defined constructor OK
};
int main()
{
	std::cout << std::boolalpha;
	std::cout << std::is_standard_layout<SL>::value << '\n';
}
// g++ --std=c++11 test.cpp

```

> NOTE: 
>
> 上述程序的输出如下:
>
> ```c++
> true
> ```
>
> 

The last two requirements can perhaps be better illustrated with code. In the next example, even though `Base` is standard-layout, `Derived` is not standard layout because both it (the most derived class) and `Base` have non-static data members:

> NOTE: 
>
> 下面的两个例子是非常特殊的

```c++
#include<type_traits>
#include<iostream>
struct Base
{
	int i;
	int j;
};

// std::is_standard_layout<<Derived> == false!
struct Derived: public Base
{
	int x;
	int y;
};

int main()
{
	std::cout << std::boolalpha;
	std::cout << std::is_standard_layout<Derived>::value << '\n';
}
// g++ --std=c++11 test.cpp

```

> NOTE: 
>
> 上述程序的输出为:
>
> ```c++
> false
> ```
>
> 

In this example `Derived` is standard-layout because `Base` has no non-static data members:

```c++
#include<type_traits>
#include<iostream>
struct Base
{
	void Foo()
	{
	}
};

// std::is_standard_layout<<Derived> == true
struct Derived: public Base
{
	int x;
	int y;
};
int main()
{
	std::cout << std::boolalpha;
	std::cout << std::is_standard_layout<Derived>::value << '\n';
}
// g++ --std=c++11 test.cpp

```

> NOTE:
>
> 上述程序的输出如下：
>
> ```c++
> true
> ```
>
> 

### [POD types](https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019#pod-types)



```c++
#include <type_traits>
#include <iostream>

using namespace std;

struct B
{
protected:
	virtual void Foo()
	{
	}
};

// Neither trivial nor standard-layout
struct A: B
{
	int a;
	int b;
	void Foo() override
	{
	} // Virtual function
};

// Trivial but not standard-layout
struct C
{
	int a;
	private:
	int b;   // Different access control
};

// Standard-layout but not trivial
struct D
{
	int a;
	int b;
	//User-defined constructor
	D()
	{
	}
};

struct POD
{
	int a;
	int b;
};

int main()
{
	cout << boolalpha;
	cout << "A is trivial is " << is_trivial<A>() << endl; // false
	cout << "A is standard-layout is " << is_standard_layout<A>() << endl;  // false

	cout << "C is trivial is " << is_trivial<C>() << endl; // true
	cout << "C is standard-layout is " << is_standard_layout<C>() << endl;  // false

	cout << "D is trivial is " << is_trivial<D>() << endl;  // false
	cout << "D is standard-layout is " << is_standard_layout<D>() << endl; // true

	cout << "POD is trivial is " << is_trivial<POD>() << endl; // true
	cout << "POD is standard-layout is " << is_standard_layout<POD>() << endl; // true

	return 0;
}
// g++ --std=c++11 test.cpp

```

### [Literal types](https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019#literal_types)

A **literal type** is one whose **layout** can be determined at compile time. The following are the literal types:

1、void

2、scalar types

3、references

4、Arrays of void, scalar types or references

5、A class that has a **trivial destructor**, and one or more `constexpr` constructors that are not move or copy constructors. Additionally, all its non-static data members and base classes must be literal types and not volatile.

> NOTE: 
>
> 一、上述对literal type的definition是非常好的、非常精准

## 标准变更历程

虽然C++标准并没有对object layout进行统一的定义，但是定义了几类object layout，下面描述C++标准定义的几类object layout，以及标准的变更: 

### wikipedia [C++11#Modification to the definition of plain old data](https://en.wikipedia.org/wiki/C++11#Modification_to_the_definition_of_plain_old_data)

In C++03, a class or struct must follow a number of rules for it to be considered a [plain old data](https://en.wikipedia.org/wiki/Plain_old_data) (POD) type. Types that fit this definition produce **object layouts** that are compatible with C, and they could also be initialized statically. The C++03 standard has restrictions on what types are compatible with C or can be statically initialized despite there being no technical reason a compiler couldn't accept the program; if someone were to create a C++03 POD type and add a non-virtual member function, this type would no longer be a POD type, could not be statically initialized, and would be incompatible with C despite no change to the memory layout.

> NOTE: 
>
> 一、C++03的POD type有如下特征：
>
> 1、compatible with C
>
> 2、initialized statically
>
> 3、不能有member function
>
> 二、"if someone were to create a C++03 POD type and add a non-virtual member function, this type would no longer be a POD type, could not be statically initialized, and would be incompatible with C despite no change to the memory layout"
>
> 这段话其实在突出强调C++03 POD type限制的太严格

C++11 relaxed several of the POD rules, by dividing the **POD concept** into two separate concepts: *trivial* and *standard-layout*.

> NOTE: trivial的含义是“普通的、平凡的”

#### Trivial

A type that is *trivial* can be **statically initialized**. It also means that it is valid to copy data around via `memcpy`, rather than having to use a **copy constructor**. The lifetime of a *trivial* type begins when its storage is defined, not when a constructor completes.

> NOTE: 
>
> 一、“A type that is *trivial* can be **statically initialized**"意味着它的constructor是default、implicitly defined，不能是user-defined，也就是下面所说的[trivial default constructor](https://en.cppreference.com/w/cpp/language/default_constructor#Trivial_default_constructor)，否则就无法实现**statically initialized**
>
> trivial type的核心特征：
>
> - can be **statically initialized**
>
> - copy data around via `memcpy` (是否意味着它是可以implicit creation？参见`C++\Language-reference\Basic-concept\Data-model\Object\Object.md`)
>
> 二、“not when a constructor completes”要如何理解？trivial type的destructor必须是trivial destructor
>
> 三、上面所描述的其实就是Named requirement [TrivialType](https://en.cppreference.com/w/cpp/named_req/TrivialType)、Named requirement [TriviallyCopyable](https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable)。



A trivial class or struct is defined as one that:

1、Has a trivial default constructor. This may use the [default constructor syntax](https://en.wikipedia.org/wiki/C++11#Explicitly_defaulted_and_deleted_special_member_functions) (`SomeConstructor() = default;`).

2、Has trivial copy and move constructors, which may use the default syntax.

3、Has trivial copy and move assignment operators, which may use the default syntax.

4、Has a trivial destructor, which must not be **virtual**.

> NOTE: Named requirement [TrivialType](https://en.cppreference.com/w/cpp/named_req/TrivialType)的描述是比上述描述要清晰易懂的，它将上述2、3、4，定义为Named requirement [TriviallyCopyable](https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable)。

**Constructors** are **trivial** only if there are no **virtual member functions** of the class and no **virtual base classes**. Copy/move operations also require all non-static data members to be trivial.

> NOTE: 这里对trivial的含义进行了描述，显然trivial是不允许virtual的(avoid virtual)。

#### Standard-layout

A type that is *standard-layout* means that it orders and packs its members in a way that is compatible with C. A class or struct is standard-layout, by definition, provided:

> NOTE: 
>
> 一、*standard-layout*的核心特征：compatible with C，意味着它是可以和C直接进行交互的，这就意味着，它也是不允许virtual的(avoid virtual)

1、It has no virtual functions

2、It has no [virtual base classes](https://en.cppreference.com/w/cpp/language/derived_class#Virtual_base_classes)

3、All its non-static data members have the same access control (public, private, protected)

4、All its non-static data members, including any in its base classes, are in the same one class in the hierarchy

> NOTE:所有的“non-static data members”都在同一个类中

5、The above rules also apply to all the base classes and to all non-static data members in the class hierarchy

6、It has no base classes of the same type as the first defined non-static data member

> NOTE: 显然，standard-layout type不是trivial type，它们是正交的概念。
>
> 上面所述的“compatible with C”的含义是通过`->`来access data member。
>
> 思考：standard-layout type能否像trivial type那样通过`memcpy`进行copy？结合最后一段的描述来看，这是不符合用途的：standard-layout type是有copy constructor的，所以我们应该使用它的copy constructor，而不是使用`memcpy`。

#### POD

A class/struct/union is considered POD if it is trivial, standard-layout, and all of its non-static data members and base classes are PODs.

By separating these concepts, it becomes possible to give up one without losing the other. A class with complex move and copy constructors may not be **trivial**, but it could be **standard-layout** and thus interoperate with C. Similarly, a class with public and private non-static data members would not be standard-layout, but it could be **trivial** and thus `memcpy`-able.



### stackoverflow [trivial vs. standard layout vs. POD](https://stackoverflow.com/questions/6496545/trivial-vs-standard-layout-vs-pod)

[回答](https://stackoverflow.com/a/6496703)中也对标准的便跟历程进行了说明。

```c++
struct N { // neither trivial nor standard-layout
   int i;
   int j;
    virtual ~N();
};

struct T { // trivial but not standard-layout，non-static data member的access right不同
    int i;
private:
    int j;
};

struct SL { // standard-layout but not trivial，因为它的destructor不是trivial的
    int i;
    int j;
    ~SL();
};

struct POD { // both trivial and standard-layout
    int i;
    int j;
};
```



## Default member initializers

在cppreference [Non-static data members#Member initialization](https://en.cppreference.com/w/cpp/language/data_members)中介绍了default member initializers，那需要思考一下：

如果使用default member initializer，那么这个类是否依然是trivial、standard layout、POD？

下面分别进行描述。

### Trivial type

在microsoft [Trivial, standard-layout, POD, and literal types](https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019) `#` [Trivial types](https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019#trivial-types)中给出的条件中，已经包含了对这种情况的说明：

> Trivial types have a [trivial default constructor](https://en.cppreference.com/w/cpp/language/default_constructor#Trivial_default_constructor), [trivial copy constructor](https://en.cppreference.com/w/cpp/language/copy_constructor#Trivial_copy_constructor), [trivial copy assignment operator](https://en.cppreference.com/w/cpp/language/copy_assignment#Trivial_copy_assignment_operator) and [trivial destructor](https://en.cppreference.com/w/cpp/language/destructor#Trivial_destructor). 

查看cppreference [trivial default constructor](https://en.cppreference.com/w/cpp/language/default_constructor#Trivial_default_constructor)，可以看到其中有这样的条件:

> - `T` has no non-static members with default initializers. (since C++11)

所以，一旦使用了default member initializer，则不再是**trivial type**了。

下面结合具体的例子来进行说明。

```c++
#include <iostream>
#include <type_traits>

struct NonTrivial
{
	int i { 0 }; // default member initializer
private:
	int j;
};

struct NonTrivial2
{
	int i { 0 }; // default member initializer
	NonTrivial2(int a, int b)
			: i(a), j(b)
	{
	}
	NonTrivial2() = default;
private:
	int j;   // Different access control
};

int main()
{
	std::cout << std::boolalpha;
	std::cout << std::is_trivial<NonTrivial>::value << '\n';
	std::cout << std::is_trivial<NonTrivial2>::value << '\n';
}
// g++ --std=c++11 test.cpp

```

上述程序的输出如下:

```c++
false
false
```



### Standard layout

```c++
#include<type_traits>
#include<iostream>
struct SL
{
	// All members have same access:
	int i { 1 }; // default member initializer
	int j;
	SL(int a, int b)
			: i(a), j(b)
	{
	} // User-defined constructor OK
};
int main()
{
	std::cout << std::boolalpha;
	std::cout << std::is_standard_layout<SL>::value << '\n';
}
// g++ --std=c++11 test.cpp

```

上述程序的输出如下：

```C++
true
```

所以，default member initializer并不影响**standard layout**了。

## Named requirements of layout 

> NOTE: 有了前面的基础，现在阅读cppreference [Named requirements](https://en.cppreference.com/w/cpp/named_req)中关于[layout](https://en.cppreference.com/w/cpp/named_req#Layout)的描述就容易了。

### cppreference [TrivialType](https://en.cppreference.com/w/cpp/named_req/TrivialType) (C++11)



### cppreference [StandardLayoutType](https://en.cppreference.com/w/cpp/named_req/StandardLayoutType) (C++11)



### cppreference [PODType](https://en.cppreference.com/w/cpp/named_req/PODType)



## 素材

1、cppreference [Non-static data members#Standard layout](https://en.cppreference.com/w/cpp/language/data_members#Standard_layout)

2、stackoverflow [What are POD types in C++?](https://stackoverflow.com/questions/146452/what-are-pod-types-in-c)


# Object layout

c++标准对object layout的定义是在有进行变更的，下面首先反思变更历程，然后再来具体的思考各种object layout。

## 标准变更历程

### 维基百科[C++11#Modification to the definition of plain old data](https://en.wikipedia.org/wiki/C++11#Modification_to_the_definition_of_plain_old_data)

In C++03, a class or struct must follow a number of rules for it to be considered a [plain old data](https://en.wikipedia.org/wiki/Plain_old_data) (POD) type. Types that fit this definition produce **object layouts** that are compatible with C, and they could also be initialized statically. 

> NOTE: POD有如下特征：
>
> - compatible with C
> - initialized statically

C++11 relaxed several of the POD rules, by dividing the **POD concept** into two separate concepts: *trivial* and *standard-layout*.

A type that is *trivial* can be **statically initialized**. It also means that it is valid to copy data around via `memcpy`, rather than having to use a **copy constructor**. The lifetime of a *trivial* type begins when its storage is defined, not when a constructor completes.

> NOTE: storage defined和constructor complete之间有什么关联？

> NOTE: 上面所描述的其实就是Named requirement [TrivialType](https://en.cppreference.com/w/cpp/named_req/TrivialType)、Named requirement [TriviallyCopyable](https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable)。

A trivial class or struct is defined as one that:

1. Has a trivial default constructor. This may use the [default constructor syntax](https://en.wikipedia.org/wiki/C++11#Explicitly_defaulted_and_deleted_special_member_functions) (`SomeConstructor() = default;`).
2. Has trivial copy and move constructors, which may use the default syntax.
3. Has trivial copy and move assignment operators, which may use the default syntax.
4. Has a trivial destructor, which must not be **virtual**.

> NOTE: Named requirement [TrivialType](https://en.cppreference.com/w/cpp/named_req/TrivialType)的描述是比上述描述要清晰易懂的，它将上述2、3、4，定义为Named requirement [TriviallyCopyable](https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable)。

Constructors are **trivial** only if there are no **virtual member functions** of the class and no **virtual base classes**. Copy/move operations also require all non-static data members to be trivial.

> NOTE: 这里对trivial的含义进行了描述，显然trivial是不允许virtual的。

A type that is *standard-layout* means that it orders and packs its members in a way that is compatible with C. A class or struct is standard-layout, by definition, provided:

1. It has no virtual functions
2. It has no virtual base classes
3. All its non-static data members have the same access control (public, private, protected)
4. All its non-static data members, including any in its base classes, are in the same one class in the hierarchy
5. The above rules also apply to all the base classes and to all non-static data members in the class hierarchy
6. It has no base classes of the same type as the first defined non-static data member

> NOTE: 显然，standard layout是不允许virtual的。
>
> 下面的文章有例子。

A class/struct/union is considered POD if it is trivial, standard-layout, and all of its non-static data members and base classes are PODs.

By separating these concepts, it becomes possible to give up one without losing the other. A class with complex move and copy constructors may not be trivial, but it could be standard-layout and thus interoperate with C. Similarly, a class with public and private non-static data members would not be standard-layout, but it could be trivial and thus `memcpy`-able.

### [trivial vs. standard layout vs. POD](https://stackoverflow.com/questions/6496545/trivial-vs-standard-layout-vs-pod)

[回答](https://stackoverflow.com/a/6496703)中也对标准的便跟历程进行了说明。

```c++
struct N { // neither trivial nor standard-layout
   int i;
   int j;
    virtual ~N();
};

struct T { // trivial but not standard-layout
    int i;
private:
    int j;
};

struct SL { // standard-layout but not trivial
    int i;
    int j;
    ~SL();
};

struct POD { // both trivial and standard-layout
    int i;
    int j;
};
```



## microsoft [Trivial, standard-layout, POD, and literal types](https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019)

> NOTE: 这篇文章，主要是对这些概念进行详细的分析

The term *layout* refers to how the members of an object of class, struct or union type are arranged in memory. In some cases, the layout is well-defined by the language specification. But when a class or struct contains certain C++ language features such as [virtual base classes](https://en.cppreference.com/w/cpp/language/derived_class#Virtual_base_classes), [virtual functions](https://en.cppreference.com/w/cpp/language/virtual), members with different access control, then the compiler is free to choose a **layout**. That **layout** may vary depending on what optimizations are being performed and in many cases the object might not even occupy a contiguous area of memory. 

> NOTE: 上诉“member”所指为data member

To enable compilers as well as **C++ programs** and **metaprograms** to reason about the suitability of any given type for operations that depend on a particular memory layout, C++14 introduced three categories of simple classes and structs: *trivial*, *standard-layout*, and *POD* or Plain Old Data. The Standard Library has the function templates [`is_trivial`](https://en.cppreference.com/w/cpp/types/is_trivial) , [`is_standard_layout`](https://en.cppreference.com/w/cpp/types/is_standard_layout)  and [`is_pod`](https://en.cppreference.com/w/cpp/types/is_pod) that determine whether a given type belongs to a given category.

> NOTE: 上述metaprogram所指的是c++的template metaprogramming，参见`C-family-language\C++\Language-reference\Template\Template-metaprogramming.md`

### [Trivial types](https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019#trivial-types)

When a class or struct in C++ has compiler-provided or explicitly defaulted special member functions, then it is a **trivial type**. 

> NOTE: 上面这段话中的“explicitly defaulted special member functions”，所指为类似如下的做法：
>
> `Trivial2() = default;`

Trivial types have a [trivial default constructor](https://en.cppreference.com/w/cpp/language/default_constructor#Trivial_default_constructor), [trivial copy constructor](https://en.cppreference.com/w/cpp/language/copy_constructor#Trivial_copy_constructor), [trivial copy assignment operator](https://en.cppreference.com/w/cpp/language/copy_assignment#Trivial_copy_assignment_operator) and [trivial destructor](https://en.cppreference.com/w/cpp/language/destructor#Trivial_destructor). In each case, *trivial* means the constructor/operator/destructor is not user-provided and belongs to a class that has

- no virtual functions or virtual base classes,
- no base classes with a corresponding non-trivial constructor/operator/destructor
- no data members of class type with a corresponding non-trivial constructor/operator/destructor

> NOTE: 上面这段话解释了trivial的含义



The following examples show trivial types. In `Trivial2`, the presence of the `Trivial2(int a, int b)` constructor requires that you provide a default constructor. For the type to qualify as trivial, you must explicitly default that constructor.

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
```

> NOTE: 编译`g++ --std=c++11 test.cpp`



### [Standard layout types](https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019#standard-layout-types)



Standard-layout types can have user-defined special member functions. In addition, standard layout types have these characteristics:

- no virtual functions or virtual base classes

- all non-static data members have the same access control

- all non-static members of class type are standard-layout

- any base classes are standard-layout

- has no base classes of the same type as the first non-static data member.

  > NOTE: 这段话要怎么理解？base class中和derived class中不允许使用相同的type来作为first non-static data member。

- meets one of these conditions:

  - no non-static data member in the most-derived class and no more than one base class with non-static data members, or
  - has no base classes with non-static data members



```c++
#include<type_traits>
#include<iostream>
struct SL
{
   // All members have same access:
   int i;
   int j;
   SL(int a, int b) : i(a), j(b) {} // User-defined constructor OK
};
int main()
{
    std::cout << std::boolalpha;
    std::cout << std::is_standard_layout<SL>::value << '\n';
}
```



The last two requirements can perhaps be better illustrated with code. In the next example, even though Base is standard-layout, `Derived` is not standard layout because both it (the most derived class) and `Base` have non-static data members:

```c++
#include<type_traits>
#include<iostream>
struct Base
{
   int i;
   int j;
};

// std::is_standard_layout<<Derived> == false!
struct Derived : public Base
{
   int x;
   int y;
};

int main()
{
    std::cout << std::boolalpha;
    std::cout << std::is_standard_layout<Derived>::value << '\n';
}
```

In this example `Derived` is standard-layout because `Base` has no non-static data members:

```c++
#include<type_traits>
#include<iostream>
struct Base
{
   void Foo() {}
};

// std::is_standard_layout<<Derived> == true
struct Derived : public Base
{
   int x;
   int y;
};
int main()
{
    std::cout << std::boolalpha;
    std::cout << std::is_standard_layout<Derived>::value << '\n';
}
```

### [POD types](https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019#pod-types)



```c++
#include <type_traits>
#include <iostream>

using namespace std;

struct B
{
protected:
   virtual void Foo() {}
};

// Neither trivial nor standard-layout
struct A : B
{
   int a;
   int b;
   void Foo() override {} // Virtual function
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
   D() {} //User-defined constructor
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
```

### [Literal types](https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=vs-2019#literal_types)

A literal type is one whose layout can be determined at compile time. The following are the literal types:

- void
- scalar types
- references
- Arrays of void, scalar types or references
- A class that has a trivial destructor, and one or more `constexpr` constructors that are not move or copy constructors. Additionally, all its non-static data members and base classes must be literal types and not volatile.

## Named requirements of layout 

> NOTE: 有了前面的基础，现在阅读cppreference [Named requirements](https://en.cppreference.com/w/cpp/named_req)中关于[layout](https://en.cppreference.com/w/cpp/named_req#Layout)的描述就容易了。

### [TriviallyCopyable](https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable) (C++11)



### [TrivialType](https://en.cppreference.com/w/cpp/named_req/TrivialType) (C++11)



### [StandardLayoutType](https://en.cppreference.com/w/cpp/named_req/StandardLayoutType)(C++11)



### [PODType](https://en.cppreference.com/w/cpp/named_req/PODType)



## SUMMARY

trivial type并没有限制access control

standard layout限制了必须相同的access control



## TO READ



[What are POD types in C++?](https://stackoverflow.com/questions/146452/what-are-pod-types-in-c)


# Type system



## cppreference [Type](https://en.cppreference.com/w/cpp/language/type)

### [Type classification](https://en.cppreference.com/w/cpp/language/type#Type_classification)

> NOTE: 对type的classification是有多种方法的，原文的这一段就提供了多种划分方法，下面是原文主要描述的一种划分方法

[fundamental types](https://en.cppreference.com/w/cpp/language/types) (see also [std::is_fundamental](https://en.cppreference.com/w/cpp/types/is_fundamental))

compound types (see also [std::is_compound](https://en.cppreference.com/w/cpp/types/is_compound))



> NOTE: 通过原文可以看到，从c++11开始，每种类型都通过了对应的trait，从而提供给programmer compile-time reflection。



### [Type naming](https://en.cppreference.com/w/cpp/language/type#Type_naming)

> NOTE: 在实际的program中，我们使用的type常常是没有name的，我们常常使用的是type-id。所以在c++中，有两种方式来name一个type：
>
> - type name
> - type id
>
> 对于type name我们是熟悉的；对于type id，其实我们已经熟悉了它的用法，只是对它的概念不熟悉。
>
> 在实际programming中， 为了便利，我们往往通过[type alias](type_alias.html) declaration、[typedef](typedef.html) declaration来为name一个type，这样后续直接通过name来使用它将会非常方便。

The syntax of the type-id that names type T is exactly the syntax of a [declaration](declarations.html) of a variable or function of type T, with the identifier omitted, except that *decl-specifier-seq* of the declaration grammar is constrained to *type-specifier-seq*

> NOTE: 上面这段话需要结合如下例子来理解:
>
> ```c++
> sizeof(static int); // error: storage class specifiers not part of type-specifier-seq
> ```
>
> 

*Type-id* may be used in the following situations:

to specify the target type in [cast expressions](expressions.html#Conversions);

Example:

```C++
int* p;               // declaration of a pointer to int
static_cast<int*>(p); // type-id is "int*"
```



as arguments to [sizeof](sizeof.html), [alignof](alignof.html), [alignas](alignas.html), [new](new.html), and [typeid](typeid.html);

Example:

```c++
int a[3];   // declaration of an array of 3 int
new int[3]; // type-id is "int[3]" (called new-type-id)
 
int (*(*x[2])())[3];      // declaration of an array of 2 pointers to functions
                          // returning pointer to array of 3 int
new (int (*(*[2])())[3]); // type-id is "int (*(*[2])())[3]"
```

on the right-hand side of a [type alias](type_alias.html) declaration;

Example:

```c++
struct { int x; } b;         // creates a new type and declares an object b of that type
sizeof(struct{ int x; });    // error: cannot define new types in a sizeof expression
using t = struct { int x; }; // creates a new type and declares t as an alias of that type
```



as the template argument for a [template type parameter](template_parameters.html#Template_type_arguments);

```C++
void f(int);                    // declaration of a function taking int and returning void
std::function<void(int)> x = f; // type template parameter is a type-id "void(int)"
std::function<auto(int) -> void> y = f; // same
```



### [Elaborated type specifier](https://en.cppreference.com/w/cpp/language/type#Elaborated_type_specifier)



### [Static type](https://en.cppreference.com/w/cpp/language/type#Static_type)

The type of an expression that results from the **compile-time** analysis of the program is known as the *static type* of the expression. The **static type** does not change while the program is executing.

​	

### [Dynamic type](https://en.cppreference.com/w/cpp/language/type#Dynamic_type)

If some [glvalue expression](https://en.cppreference.com/w/cpp/language/value_category) refers to a [polymorphic object](https://en.cppreference.com/w/cpp/language/object), the type of its **most derived object** is known as the **dynamic type**.

> NOTE: 上面这段话给出了dynamic type的定义，需要注意的是，dynamic type是run-time获得的。关于上面这段话中的**most derived object**，参见cppreference [Object#Subobjects](https://en.cppreference.com/w/cpp/language/object#Subobjects)



### [Incomplete type](https://en.cppreference.com/w/cpp/language/type#Incomplete_type)



## cppreference [Type support library](https://en.cppreference.com/w/cpp/types)

在`C++\Library\Standard-library\Utility-library\Language-support\Type_support`中，对此进行了分析。



## Static type and dynamic type

`C++`中对**static type** 和 **dynamic type**的区分是源于`C++`对runtime polymorphism（参见 [polymorphic object](https://en.cppreference.com/w/cpp/language/object)）的支持，只有[polymorphic object](https://en.cppreference.com/w/cpp/language/object)才同时具备static type和dynamic type。

**static type**和**dynamic type**的概念对于`C++` 非常重要，关于两者的一个典型例子就是：`C++`中如何实现multiple dispatch，参见[A polyglot's guide to multiple dispatch](https://eli.thegreenplace.net/2016/a-polyglots-guide-to-multiple-dispatch/)，在`Theory\Programming-paradigm\Object-oriented-programming\Polymorphism\Subtyping-polymorphism\Multiple-dispatch.md`中收录了这篇文章。

### cast

`static_cast`、`dynamic_cast`

### 如何获得type 

既然`C++`中区分static type和dynamic type，那么就需要考虑如何来获得两者。

每个[Polymorphic object](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects)有[Static type](https://en.cppreference.com/w/cpp/language/type#Static_type)和[Dynamic type](https://en.cppreference.com/w/cpp/language/type#Dynamic_type)，那对于polymorphic object：

如何获得它的**static type**？

`typeid(decltype(b1)).name()`

如何获得它的**dynamic type**？

`typeid(b1).name()`

关于这个问题，下面的例子（源自cppreference [Object](https://en.cppreference.com/w/cpp/language/object)`#`[Polymorphic objects](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects)）给出了回答：



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



#### `typeid` and `decltype`

上面的例子已经展示了两者之间的关联了。



### Implementation of dynamic type

在cppreference [Object](https://en.cppreference.com/w/cpp/language/object)`#`[Polymorphic objects](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects)中给出了这个问题的解答：

> Within each polymorphic object, the implementation stores additional information (in every existing implementation, it is one pointer unless optimized out), which is used by [virtual function](https://en.cppreference.com/w/cpp/language/virtual) calls and by the RTTI features ([dynamic_cast](https://en.cppreference.com/w/cpp/language/dynamic_cast) and [typeid](https://en.cppreference.com/w/cpp/language/typeid)) to determine, at run time, the type with which the object was created, regardless of the expression it is used in.







## cppreference [typeid](https://en.cppreference.com/w/cpp/language/typeid)

参见本章的`typeid`

## cppreference [decltype](https://en.cppreference.com/w/cpp/language/decltype) 

参见本章的`decltype`

## type cast and conversion

参见本章的`Type-conversion`


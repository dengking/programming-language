# Abstract class

## cppreference [abstract class](https://en.cppreference.com/w/cpp/language/abstract_class)	

> NOTE: 原文主要是说明pure virtual method

### Syntax

```
declarator virt-specifier(optional) = 0		
```

An *abstract class* is a class that either defines or inherits at least one function for which [the final overrider](virtual.html) is *pure virtual*.

> NOTE: 上面这段话是需要结合例子来进行说明的，它所描述的其实是两种情况：
>
> - pure virtual overrider，上面这段话中的“inherits at least one function for which [the final overrider](virtual.html) is *pure virtual*.”描述的就是这种情况，对应的是下面例子中的`struct Abstract2`
> - pure virtual，对应的是下面的例子中的`struct Abstract`
>
> 
>
> 原文的这个例子可以说明这两种情况：
>
> ```c++
> struct Abstract
> {
> 	virtual void f() = 0; // pure virtual
> };
> // "Abstract" is abstract
> 
> struct Concrete: Abstract
> {
> 	void f() override
> 	{
> 	} // non-pure virtual
> 	virtual void g();     // non-pure virtual
> };
> // "Concrete" is non-abstract
> 
> struct Abstract2: Concrete
> {
> 	void g() override = 0; // pure virtual overrider
> };
> // "Abstract2" is abstract
> 
> int main()
> {
> 	// Abstract a; // Error: abstract class
> 	Concrete b; // OK
> 	Abstract& a = b; // OK to reference abstract base
> 	a.f(); // virtual dispatch to Concrete::f()
> 	// Abstract2 a2; // Error: abstract class (final overrider of g() is pure)
> }
> 
> ```
>
> 

### Explanation

**Abstract classes** are used to represent **general concepts** (for example, **Shape**, **Animal**), which can be used as base classes for concrete classes (for example, Circle, Dog).

> NOTE: 上述general concept需要添加到文章concept中。

No objects of an abstract class can be created (except for base subobjects of a class derived from it) and no non-static data members of an abstract class can be declared.

> NOTE: 上面这段话的最后一句的意思是：在一个class中，不能够声明类型为abstract class的non-static data members ；它的意思并不是指abstract class不能够声明non-static data members 。



#### Definition of pure virtual method

> NOTE: C++是允许提供pure virtual method的definition的，这有些超出了我的认知；



#### Call to pure virtual method

Making a **virtual call** to a **pure virtual function** from a **constructor** or the **destructor** of the **abstract class** is **undefined behavior** (regardless of whether it has a definition or not).

> NOTE: 原文没有对此进行深入地解释，在`Pure-virtual-method-called`中对此进行了深入的分析
>
> 上面这段话中的virtual call要如何来理解？下面的例子对此进行了说明：在destructor `~Abstract()`中，`f()`这种unqualified的写法就是virtual call；
>
> 上面这段话没有说明的有：可以使用non-virtual call；

```c++
#include <iostream>

struct Abstract
{
	virtual void f() = 0; // pure virtual
	virtual void g()
	{
	}  // non-pure virtual
	~Abstract()
	{
		g(); // OK: calls Abstract::g()
		// f(); // undefined behavior
		Abstract::f(); // OK: non-virtual call
	}
};

// definition of the pure virtual function
void Abstract::f()
{
	std::cout << "A::f()\n";
}

struct Concrete: Abstract
{
	void f() override
	{
		Abstract::f(); // OK: calls pure virtual function
	}
	void g() override
	{
	}
	~Concrete()
	{
		g(); // OK: calls Concrete::g()
		f(); // OK: calls Concrete::f()
	}
};

int main()
{
	{
		Concrete c;
	}
}

```

> NOTE: 编译`g++ test.cpp`，输出:
>
> ```
> A::f()
> A::f()
> ```



## learncpp [Pure virtual functions, abstract base classes, and interface classes](https://www.learncpp.com/cpp-tutorial/126-pure-virtual-functions-abstract-base-classes-and-interface-classes/)





## zhihu [c++虚函数的作用是什么？](https://www.zhihu.com/question/23971699)



## cppreference [std::is_abstract](https://en.cppreference.com/w/cpp/types/is_abstract)
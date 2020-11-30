# Polymorphic type and non-polymorphic type 

本文对polymorphic type进行专门介绍。

cppreference中并没有正式地定义polymorphic type，其中只有 [polymorphic object](https://en.cppreference.com/w/cpp/language/object) 的定义，仔细阅读 [polymorphic object](https://en.cppreference.com/w/cpp/language/object) 段，可以发现其中给出的例子中有“**polymorphic type**”，当我们进入到type system后，使用polymorphic type来进行描述会更加方便。

在文章panicsoftware [DYNAMIC_CAST AND TYPEID AS (NON) RTTI TOOLS.](https://blog.panicsoftware.com/dynamic_cast-and-typeid-as-non-rtti-tools/)中，给出了polymorphic type的定义:

> First thing you need to know about RTTI is, that it doesn’t work for the non-polymorphic types. If you wonder what polymorphic types are, then here I come with the explanation. The types are polymorphic, when either they have at least one virtual function or they inherit from another polymorphic type.

在 cppreference [typeid operator](https://en.cppreference.com/w/cpp/language/typeid) 中，也有关于 polymorphic type的描述。

## Static type and dynamic type

`C++`中对**static type** 和 **dynamic type**的区分是源自`C++`对OOP subtyping polymorphism（参见 [polymorphic object](https://en.cppreference.com/w/cpp/language/object)）的支持，只有[polymorphic object](https://en.cppreference.com/w/cpp/language/object)才同时具备**static type**和**dynamic type**，对于 non-polymorphic object，它不具备dynamic type，只有static type。



## How to use static type and  dynamic type?

**static type**和**dynamic type**的概念对于`C++` 非常重要，它是理解很多C++问题的基础，我们需要把握如下原则:

> static type用于static context，dynamic type用于dynamic context

关于 static 和 dynamic，参见 `C-and-C++\Compile-time-and-run-time` 章节。

### Static type and dynamic type of `this` pointer

下面是一个非常典型的例子，用于说明上述观点，这个例子源自eli.thegreenplace [A polyglot's guide to multiple dispatch](https://eli.thegreenplace.net/2016/a-polyglots-guide-to-multiple-dispatch/) ，它给出了multiple dispatch的实现，这个例子非常能够体现: 灵活地运用 `this` pointer的**static type** 和 **dynamic type**，这篇文章收录在了 `C++\Pattern\Visitor-pattern` 章节。

```C++
#include <string>
#include <iostream>
#include <typeinfo>
#include <memory>
class Rectangle;
class Ellipse;

class Shape
{
public:
	virtual std::string name() const
	{
		return typeid(*this).name();
	}

	// Dispatcher that should be called by clients to intersect different shapes.
	virtual void Intersect(const Shape*) const = 0;

	// Specific interesection methods implemented by subclasses. If subclass A
	// has a special way to intersect with subclass B, it should implement
	// InteresectWith(const B*).
	virtual void IntersectWith(const Shape*) const
	{
	}
	virtual void IntersectWith(const Rectangle*) const
	{
	}
	virtual void IntersectWith(const Ellipse*) const
	{
	}
};

class Rectangle: public Shape
{
public:
	virtual void Intersect(const Shape *s) const
	{
		s->IntersectWith(this);
	}

	virtual void IntersectWith(const Shape *s) const
	{
		std::cout << "Rectangle x Shape [names this=" << this->name() << ", s=" << s->name() << "]\n";
	}

	virtual void IntersectWith(const Rectangle *r) const
	{
		std::cout << "Rectangle x Rectangle [names this=" << this->name() << ", r=" << r->name() << "]\n";
	}
};

class Ellipse: public Shape
{
public:
	virtual void Intersect(const Shape *s) const
	{
		s->IntersectWith(this);
	}

	virtual void IntersectWith(const Rectangle *r) const
	{
		std::cout << "Ellipse x Rectangle [names this=" << this->name() << ", r=" << r->name() << "]\n";
	}
};


int main()
{
	std::unique_ptr<Shape> pr1(new Rectangle);
	std::unique_ptr<Shape> pr2(new Rectangle);
	std::unique_ptr<Shape> pe(new Ellipse);

	std::cout << "Dynamic type dispatch\n";
	pr1->Intersect(pe.get());
	pr1->Intersect(pr2.get());
}
// g++ --std=c++11 test.cpp

```



#### `typeid(*this)`

在 `C++\Language-reference\Basic-concept\Type-system\Type-operation\Query-type\typeid` 中对上述例子中 `typeid(*this)` 的用法进行了详细的说明。







### Cast

`static_cast`、`dynamic_cast`



### Query type

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



#### `typeid` and `decltype`

上面的例子已经展示了两者之间的关联了，在 `C++\Language-reference\Basic-concept\Type-system\Type-operation\Query-type` 章节，对它们进行了专门介绍。



## Virtual

参见`C++\Language-reference\Classes\Subtype-polymorphism`。

## Implementation of dynamic type

这其实就是C++ dynamic polymorphism的实现，参见`C++\Language-reference\Classes\Subtype-polymorphism\Implementation`章节。


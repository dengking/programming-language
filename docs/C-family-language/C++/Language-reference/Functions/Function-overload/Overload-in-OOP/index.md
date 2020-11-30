# Function overload in OOP

我们已经知道，在C++中，virtual function对应的是dynamic polymorphism，而overload对应的是static polymorphism，那 "overload of virtual function" 该如何理解呢？C++中是否支持 "overload of virtual function" 呢？

本文结合具体例子对这个话题进行讨论，可以肯定的是C++是支持overload virtual function的，也就是mix static polymorphism 和 dynamic polymorphism，compiler是如何来进行处理的呢？下面结合具体例子来进行说明。

## Example

这是我在阅读 eli.thegreenplace [A polyglot's guide to multiple dispatch](https://eli.thegreenplace.net/2016/a-polyglots-guide-to-multiple-dispatch/) 时，其中给出的例子: 

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

上述代码是比较复杂的，因为它涉及到了如下两个方面的内容:



### Overload of virtual function

```C++
class Shape
{
public:
	
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
```

上述 `IntersectWith` 序列函数，就是 overload of virtual function。



```C++
class Rectangle: public Shape
{
public:
	virtual void Intersect(const Shape *s) const
	{
		s->IntersectWith(this);
	}
}
```

上述 `s->IntersectWith(this)` 就是 polymorphic object 对 virtual overloaded function的调用，compiler是如何对其进行处理的呢？下一节对此进行了描述。



### `typeid(*this)`

参见 `C++\Language-reference\Basic-concept\Type-system\Type-operation\Query-type\typeid` 章节。

## compiler实现分析

overload virtual function是mix static polymorphism 和 dynamic polymorphism，compiler对其的处理其实就是将两者结合起来，但是在一些方面还是需要特别注意。

### Name mangling

上述`IntersectWith`系列函数是overload，通过name mangling，compiler会为它们分配不同的name，因此虽然看上去它们的name相同，其实实现上它们的name是不同的，也就是 `Shape` 可以看做有三个不同的`***IntersectWith***`，此处添加`***`的目的是说明compiler的name mangling方案会修改overload function的name；

C++是支持overload virtual function的，因此在subclass中，是可以**override** parent class的overloaded virtual function的。由于C++的**override**要求function signature在subclass和parent class中保持一致(covariance例外)，因此这就要求C++ name mangling方案是不涉及class信息的，这样才能够保证overloaded virtual function在subclass、parent class中，保持name的一致，这样才能够实现override。

### Static polymorphism: overload resolution

既然是overload，因此compiler在编译的时候，就需要进行**overload resolution**，因此对于 `IntersectWith(this)` ，compiler是需要将其**dispatch**到**overload set**中的一个的，那到底**dispatch**到哪一个呢？显然这是由 `this` 的**static type**所决定的，需要注意的是，这里是 static type，而不是 dynamic type，因为



### Dynamic polymorphism



### `*this` and overload resolution







1 dynamic polymorphism

`IntersectWith`是virtual function，因此`s->IntersectWith`就涉及了dynamic polymorphism。

2 static polymorphism

通过上面的描述可知，`IntersectWith` 是overload函数，因此这就是涉及了overload resolution，它的入参类型是 `this` ，`this`的concrete type是已知的，因此compiler能够进行准确的overload resolution。



static type of this。



1 overload of virtual function

2 `*this` and overload resolution






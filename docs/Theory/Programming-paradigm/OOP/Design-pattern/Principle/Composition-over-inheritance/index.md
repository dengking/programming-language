# Composition over inheritance

本文主要参考下面文章，都对三者进行了比较分析:

| 文章                                                         | 说明                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| wikipedia [Composition over inheritance](https://en.wikipedia.org/wiki/Composition_over_inheritance) | 相比于其他的文章仅仅给出长篇的理论分析，这篇文章给出了一个非常好的对比例子，通过这两个例子，我们可以快速地领悟“composition over inheritance”的思想。因此将它置于首位。 |
| wikipedia [Design Patterns#Introduction, Chapter 1](https://en.wikipedia.org/wiki/Design_Patterns#Introduction,_Chapter_1) |                                                              |
|                                                              |                                                              |

## How to compare? 

本段对inheritance和composition进行对比，将从以下方面对两者进行对比。

### Relation

通过relation对两者进行对比。

| is-a        | has-a       |
| ----------- | ----------- |
| inheritance | composition |

### Code reuse、polymorphism、extendibility/flexibility、information hiding/encapsulation

从code reuse、polymorphism、extendibility、information hiding的角度对两者进行对比

| 标准                          | composition | inheritance |
| ----------------------------- | ----------- | ----------- |
| **code reuse**                | 能够实现    | 能够实现    |
| **dynamic polymorphism**      | 能够实现    | 能够实现    |
| **extendibility/flexibility** | 强          | 弱          |
| **information hiding**        | 强          | 弱          |

**information hiding**的分析，参见:

wikipedia [Design Patterns#Introduction, Chapter 1](https://en.wikipedia.org/wiki/Design_Patterns#Introduction,_Chapter_1)	

**extendibility/flexibility**的分析，参见:

1、wikipedia [Composition over inheritance](https://en.wikipedia.org/wiki/Composition_over_inheritance) # Benefits

2、jianshu [策略模式(Strategy Pattern)](https://www.jianshu.com/p/422acad380dd)

上述表格的绘制，还参考了:

1、stackoverflow [Prefer composition over inheritance?](https://stackoverflow.com/questions/49002/prefer-composition-over-inheritance)

2、wikipedia [Design Patterns#Introduction, Chapter 1](https://en.wikipedia.org/wiki/Design_Patterns#Introduction,_Chapter_1)	

3、isocpp faq: What are some disciplines for using multiple inheritance? [¶](https://isocpp.org/wiki/faq/multiple-inheritance#mi-disciplines) [Δ](https://isocpp.org/wiki/faq/multiple-inheritance#)





## Example 

建议首先阅读下面这些能够体现"Composition over inheritance"的具体案例，然后再来阅读下面的分析，因为这些具体的案例是非常详实的。

### Strategy pattern

参见 `Theory\Programming-paradigm\Object-oriented-programming\Design-pattern\Behavioral-pattern\Strategy-pattern` 章节



## wikipedia [Composition over inheritance](https://en.wikipedia.org/wiki/Composition_over_inheritance)

> NOTE: 

### Example

> NOTE: 本段给出了两个对比的例子，第一个例子使用inheritance，第二个例子使用Composition and interfaces。
>
> 下面对Inheritance example和Composition and interfaces example进行对比: 
>
> **Inheritance example**: 通过Inheritance的方式来获得behavior；
>
> **Composition and interfaces example**: 通过Composition的来获得behavior；
>
> 由子类来注入behavior的implementation；
>
> 非常类似于behavioral pattern（参见`Theory\Design-pattern\OOP-design-pattern\Behavioral-pattern`）的做法，尤其类似于strategy pattern（`Theory\Design-pattern\OOP-design-pattern\Behavioral-pattern\Strategy-pattern`）。
>
> 在下面的“Benefits”章节会对Composition and interfaces example的优势进行更加深入的分析。

#### Inheritance

> NOTE: 本段给出的是仅仅使用inheritance的例子

```C++
class Object
{
public:
	virtual void update()
	{
		// no-op
	}

	virtual void draw()
	{
		// no-op
	}

	virtual void collide(Object objects[])
	{
		// no-op
	}
};

class Visible: public Object
{
	Model *model;

public:
	virtual void draw() override
	{
		// code to draw a model at the position of this object
	}
};
/**
 * @brief 固体
 *
 */
class Solid: public Object
{
public:
    /**
     * @brief 碰撞
     * 固体会和其他对象碰撞
     */
	virtual void collide(Object objects[]) override
	{
		// code to check for and react to collisions with other objects
	}
};

class Movable: public Object
{
public:
	virtual void update() override
	{
		// code to update the position of this object
	}
};

```

Then, suppose we also have these concrete classes:

- class `Player` - which is `Solid`, `Movable` and `Visible`
- class `Cloud` - which is `Movable` and `Visible`, but not `Solid`
- class `Building` - which is `Solid` and `Visible`, but not `Movable`
- class `Trap` - which is `Solid`, but neither `Visible` nor `Movable`

Note that multiple inheritance is dangerous if not implemented carefully, as it can lead to the [diamond problem](https://en.wikipedia.org/wiki/Multiple_inheritance#The_diamond_problem). One solution to avoid this is to create classes such as `VisibleAndSolid`, `VisibleAndMovable`, `VisibleAndSolidAndMovable`, etc. for every needed combination, though this leads to a large amount of repetitive code. Keep in mind that C++ solves the diamond problem of multiple inheritance by allowing [virtual inheritance](https://en.wikipedia.org/wiki/Virtual_inheritance).



##### Complete program

> NOTE: 下面是使用C++ virtual inheritance的完整的例子
>
> 类图如下: 
>
> ```C++
>          Object
>       /    |    \
>      /     |     \
> Visible   Solid   Movable
>      \     |      /
>       \    |     /
>        \   |    /
>          Player
> ```
>
> 需要注意的是，在inheritance hierarchy的每一层都需要使用**virtual inheritance**。

```C++
#include <iostream>
class Object
{
public:
	virtual void update()
	{
		// no-op
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}

	virtual void draw()
	{
		// no-op
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}

	virtual void collide(Object objects[])
	{
		// no-op
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

class Visible: virtual public Object
{

public:
	virtual void draw() override
	{
		// code to draw a model at the position of this object
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};
/**
 * @brief 固体
 *
 */
class Solid: virtual public Object
{
public:
	/**
	 * @brief 碰撞
	 *  固体会和其他对象碰撞
	 */
	virtual void collide(Object objects[]) override
	{
		// code to check for and react to collisions with other objects
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

class Movable: virtual public Object
{
public:
	virtual void update() override
	{
		// code to update the position of this object
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

/**
 * @brief class Player - which is Solid, Movable and Visible
 *
 */
class Player: virtual public Visible, virtual public Solid, virtual public Movable
{

};

/**
 * @brief class Cloud - which is Solid, Movable and Visible
 *
 */
class Cloud: virtual public Visible, virtual public Movable
{

};

/**
 * @brief class Building - which is Solid, Movable and Visible
 *
 */
class Building: virtual public Visible, virtual public Solid, virtual public Movable
{

};

/**
 * @brief class Building - which is Solid, Movable and Visible
 *
 */
class Trap: virtual public Solid
{

};

void draw(Object &o)
{
	o.draw();
}
void update(Object &o)
{
	o.update();
}
int main()
{
	Player p;
	draw(p);
	update(p);
	Cloud c;
	draw(c);
	update(c);
	Building b;
	draw(b);
	update(b);
	Trap t;
	draw(t);
}
// g++ --std=c++11 test.cpp

```

> NOTE: 输出如下:
>
> ```C++
> virtual void Visible::draw()
> virtual void Movable::update()
> virtual void Visible::draw()
> virtual void Movable::update()
> virtual void Visible::draw()
> virtual void Movable::update()
> virtual void Visible::draw()
> virtual void Movable::update()
> virtual void Visible::draw()
> virtual void Movable::update()
> virtual void Object::draw()
> 
> ```
>
> 

#### Composition and interfaces

The C++ examples in this section demonstrate the principle of using composition and interfaces to achieve code reuse and polymorphism. Due to the C++ language not having a dedicated keyword to declare interfaces, the following C++ example uses "inheritance from a pure abstract base class". For most purposes, this is functionally equivalent to the interfaces provided in other languages, such as Java and C#.

##### Visibility

Introduce an abstract class named `VisibilityDelegate`, with the subclasses `NotVisible` and `Visible`, which provides a means of drawing an object:

```C++
class VisibilityDelegate
{
public:
	virtual void draw() = 0;
};

class NotVisible: public VisibilityDelegate
{
public:
	virtual void draw() override
	{
		// no-op
	}
};

class Visible: public VisibilityDelegate
{
public:
	virtual void draw() override
	{
		// code to draw a model at the position of this object
	}
};

```



##### Movability 

Introduce an abstract class named `UpdateDelegate`, with the subclasses `NotMovable` and `Movable`, which provides a means of moving an object:

```C++
class UpdateDelegate
{
public:
	virtual void update() = 0;
};

class NotMovable: public UpdateDelegate
{
public:
	virtual void update() override
	{
		// no-op
	}
};

class Movable: public UpdateDelegate
{
public:
	virtual void update() override
	{
		// code to update the position of this object
	}
};

```



##### Solidity 

Introduce an abstract class named `CollisionDelegate`, with the subclasses `NotSolid` and `Solid`, which provides a means of colliding with an object:

```C++
class CollisionDelegate
{
public:
	virtual void collide(Object objects[]) = 0;
};

class NotSolid: public CollisionDelegate
{
public:
	virtual void collide(Object objects[]) override
	{
		// no-op
	}
};

class Solid: public CollisionDelegate
{
public:
	virtual void collide(Object objects[]) override
	{
		// code to check for and react to collisions with other objects
	}
};

```

##### Object

Finally, introduce a class named `Object` with members to control its visibility (using a `VisibilityDelegate`), movability (using an `UpdateDelegate`), and solidity (using a `CollisionDelegate`). This class has methods which **delegate** to its members, e.g. `update()` simply calls a method on the `UpdateDelegate`:

```C++
class Object
{
	VisibilityDelegate *_v;
	UpdateDelegate *_u;
	CollisionDelegate *_c;

public:
	Object(VisibilityDelegate *v, UpdateDelegate *u, CollisionDelegate *c) :
			_v(v), _u(u), _c(c)
	{
	}

	void update()
	{
		_u->update();
	}

	void draw()
	{
		_v->draw();
	}

	void collide(Object objects[])
	{
		_c->collide(objects);
	}
};

```

##### Concrete classes

Then, concrete classes would look like:

```C++
class Player : public Object
{
public:
    Player()
        : Object(new Visible(), new Movable(), new Solid())
    {}

    // ...
};

class Smoke : public Object
{
public:
    Smoke()
        : Object(new Visible(), new Movable(), new NotSolid())
    {}

    // ...
};
```

##### Complete program

```C++
#include <iostream>

class Object;
class VisibilityDelegate
{
public:
	virtual void draw() = 0;
};

class NotVisible: public VisibilityDelegate
{
public:
	virtual void draw() override
	{
		// no-op
	}
};

class Visible: public VisibilityDelegate
{
public:
	virtual void draw() override
	{
		// code to draw a model at the position of this object
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

class UpdateDelegate
{
public:
	virtual void update() = 0;
};

class NotMovable: public UpdateDelegate
{
public:
	virtual void update() override
	{
		// no-op
	}
};

class Movable: public UpdateDelegate
{
public:
	virtual void update() override
	{
		// code to update the position of this object
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

class CollisionDelegate
{
public:
	virtual void collide(Object objects[]) = 0;
};

class NotSolid: public CollisionDelegate
{
public:
	virtual void collide(Object objects[]) override
	{
		// no-op
	}
};

class Solid: public CollisionDelegate
{
public:
	virtual void collide(Object objects[]) override
	{
		// code to check for and react to collisions with other objects
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

class Object
{
	VisibilityDelegate *_v;
	UpdateDelegate *_u;
	CollisionDelegate *_c;

public:
	Object(VisibilityDelegate *v, UpdateDelegate *u, CollisionDelegate *c) :
			_v(v), _u(u), _c(c)
	{
	}

	void update()
	{
		_u->update();
	}

	void draw()
	{
		_v->draw();
	}

	void collide(Object objects[])
	{
		_c->collide(objects);
	}
};
// 此处的基础是为了实现接口
class Player: public Object
{
public:
	Player() :
			Object(new Visible(), new Movable(), new Solid())
	{
	}

	// ...
};

class Smoke: public Object
{
public:
	Smoke() :
			Object(new Visible(), new Movable(), new NotSolid())
	{
	}

	// ...
};

void draw(Object &o)
{
	o.draw();
}
void update(Object &o)
{
	o.update();
}
int main()
{
	Player p;
	draw(p);
	update(p);
	Smoke c;
	draw(c);
	update(c);
}
// g++ --std=c++11 test.cpp

```

> NOTE: 上述程序的输出如下:
>
> ```C++
> virtual void Visible::draw()
> virtual void Movable::update()
> virtual void Visible::draw()
> virtual void Movable::update()
> 
> ```



### Benefits

To favor composition over inheritance is a design principle that gives the design higher **flexibility**. It is more natural to build **business-domain classes** out of various **components** than trying to find **commonality** between them and creating a family tree. 

> NOTE: 关于上述commonality，参见"Commonality-and-variability-analysis"章节

> NOTE: 结合“Example”节中给出的例子来分析上面这段话的中的一些概念:
>
> | 概念                    | example                           |
> | ----------------------- | --------------------------------- |
> | business-domain classes | `class Player`                    |
> | components              | `class Solid` <br>`class Movable` |

Composition also provides a more stable business domain in the long term as it is less prone to the quirks of the family members. In other words, it is better to compose what an object can do (*[HAS-A](https://en.wikipedia.org/wiki/HAS-A)*) than extend what it is (*[IS-A](https://en.wikipedia.org/wiki/IS-A)*).[[1\]](https://en.wikipedia.org/wiki/Composition_over_inheritance#cite_note-FHDPs-1)

> NOTE: 最后一段话总结得非常好，后面将它作为标准。结合前面的例子能够更好地理解它的内涵。
>
> 其实，在大多数情况下，我们是可以快速地判定 (*[HAS-A](https://en.wikipedia.org/wiki/HAS-A)*)、 (*[IS-A](https://en.wikipedia.org/wiki/IS-A)*)关系的。在下面的“is-a and has-a”章节会对它进行深入分析。



### Drawbacks

> NOTE: 原文的这一段，并没有理解

## wikipedia [Design Patterns#Introduction, Chapter 1](https://en.wikipedia.org/wiki/Design_Patterns#Introduction,_Chapter_1)	

### Composition over inheritance

The authors refer to [inheritance](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)) as *[white-box](https://en.wikipedia.org/wiki/White_box_(software_engineering)) reuse*, with white-box referring to visibility, because the internals of parent classes are often visible to [subclasses](https://en.wikipedia.org/wiki/Subclass_(computer_science)). In contrast, the authors refer to [object composition](https://en.wikipedia.org/wiki/Object_composition) (in which objects with well-defined interfaces are used dynamically at runtime by objects obtaining references to other objects) as *[black-box](https://en.wikipedia.org/wiki/Black_box) reuse* because no internal details of composed objects need be visible in the code using them.

> NOTE: white-box reuse VS black-box reuse。
>
> 根据下面章节的内容，我们可以总结出相比于composition，inheritance的劣势为:
>
> | 标准                          |                                                              |
> | ----------------------------- | ------------------------------------------------------------ |
> | **information hiding**        | "inheritance breaks encapsulation"                           |
> | **extendibility/flexibility** | “any change in the parent's implementation will force the subclass to change” |

#### Inheritance breaks encapsulation

The authors discuss the tension between inheritance and encapsulation at length and state that in their experience, designers overuse inheritance (Gang of Four 1995:20). The danger is stated as follows:

> "Because inheritance exposes a [subclass](https://en.wikipedia.org/wiki/Subclass_(computer_science)) to details of its parent's implementation, it's often said that '**inheritance breaks encapsulation**'". (Gang of Four 1995:19)

#### Any change in the parent's implementation will force the subclass to change

They warn that the implementation of a subclass can become so bound up with the implementation of its parent class that **any change in the parent's implementation will force the subclass to change**. Furthermore, they claim that a way to avoid this is to inherit only from **abstract classes**—but then, they point out that there is minimal code reuse.

> NOTE: 上面总结了相较于composition，inheritance的劣势: extendibility/flexibility太差；
>
> ABC无法实现code reuse



Using inheritance is recommended mainly when adding to the functionality of existing components, reusing most of the old code and adding relatively small amounts of new code.

#### Delegation

To the authors, 'delegation' is an extreme form of object composition that can always be used to replace inheritance. Delegation involves two objects: a 'sender' passes itself to a 'delegate' to let the delegate refer to the sender. Thus the link between two parts of a system are established only at runtime, not at compile-time. The [Callback](https://en.wikipedia.org/wiki/Callback_(computer_science)) article has more information about delegation.

> NOTE: 上面作者都是在对composition和inheritance进行讨论，composition是has-a关系、inheritance是is-a关系。
>

## stackoverflow [Prefer composition over inheritance?](https://stackoverflow.com/questions/49002/prefer-composition-over-inheritance)



## is-a and has-a

在进入inheritance 和 composition之前，我们先讨论 is-a 和 has-a，它们之间的对应关系为:

| is-a        | has-a       |
| ----------- | ----------- |
| inheritance | composition |



### w3resource [Inheritance (IS-A) vs. Composition (HAS-A) Relationship](https://www.w3resource.com/java-tutorial/inheritance-composition-relationship.php)



### wikipedia [Has-a](https://en.wikipedia.org/wiki/Has-a)



### wikipedia [Is-a](https://en.wikipedia.org/wiki/Is-a)



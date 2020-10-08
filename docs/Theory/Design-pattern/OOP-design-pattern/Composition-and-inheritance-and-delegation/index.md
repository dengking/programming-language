# Inheritance and composition and delegation

在下面文章中，都对三者进行了比较分析:

| 文章                                                         | 章节 |
| ------------------------------------------------------------ | ---- |
|                                                              |      |
| [Design Patterns#Introduction, Chapter 1](https://en.wikipedia.org/wiki/Design_Patterns#Introduction,_Chapter_1) |      |
|                                                              |      |



## wikipedia [Composition over inheritance](https://en.wikipedia.org/wiki/Composition_over_inheritance)

> NOTE: 相比于其他的文章仅仅给出长篇的理论分析，这篇文章给出了一个非常好的对比例子，通过这两个例子，我们可以快速地领悟“composition over inheritance”的思想。

### Example

> NOTE: 本段给出了两个对比的例子，第一个例子使用inheritance，第二个例子使用Composition and interfaces。

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
>         Object
>       /    |   \
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



#### Comparison

通过继承的方式来获得还是通过组合的方式来获得behavior；

由子类来注入implementation。

非常类似于strategy pattern。

`Object`的behavior

### Benefits

To favor over inheritance is a design principle that gives the design higher flexibility. It is more natural to build business-domain classes out of various components than trying to find commonality between them and creating a family tree. 

To favor **composition** over **inheritance** is a design principle that gives the design higher flexibility. It is more natural to build business-domain classes out of various components than trying to find **commonality** between them and creating a family tree. 





## stackoverflow [Prefer composition over inheritance?](https://stackoverflow.com/questions/49002/prefer-composition-over-inheritance)





## wikipedia [Design Patterns#Introduction, Chapter 1](https://en.wikipedia.org/wiki/Design_Patterns#Introduction,_Chapter_1)	





## is-a and has-a

在进入inheritance 和 composition之前，我们先讨论 is-a 和 has-a，它们之间的对应关系为:

| is-a        | has-a       |
| ----------- | ----------- |
| inheritance | composition |



### w3resource [Inheritance (IS-A) vs. Composition (HAS-A) Relationship](https://www.w3resource.com/java-tutorial/inheritance-composition-relationship.php)



### wikipedia [Has-a](https://en.wikipedia.org/wiki/Has-a)



### wikipedia [Is-a](https://en.wikipedia.org/wiki/Is-a)



## 对比

本段对inheritance和composition进行对比:



| is-a        | has-a       |
| ----------- | ----------- |
| inheritance | composition |



| 标准                     | composition | inheritance |
| ------------------------ | ----------- | ----------- |
| **code reuse**           | 能够实现    | 能够实现    |
| **dynamic polymorphism** | 能够实现    | 能够实现    |
| **extendibility**        | 强          | 弱          |

> NOTE: 上述表格的绘制，参考了:
>
> - stackoverflow [Prefer composition over inheritance?](https://stackoverflow.com/questions/49002/prefer-composition-over-inheritance)
> - wikipedia [Design Patterns#Introduction, Chapter 1](https://en.wikipedia.org/wiki/Design_Patterns#Introduction,_Chapter_1)	
> - isocpp faq: What are some disciplines for using multiple inheritance? [¶](https://isocpp.org/wiki/faq/multiple-inheritance#mi-disciplines) [Δ](https://isocpp.org/wiki/faq/multiple-inheritance#)


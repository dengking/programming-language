# Inheritance and composition and delegation

在下面文章中，都对三者进行了比较分析:

| 文章                                                         | 章节 |
| ------------------------------------------------------------ | ---- |
|                                                              |      |
| [Design Patterns#Introduction, Chapter 1](https://en.wikipedia.org/wiki/Design_Patterns#Introduction,_Chapter_1) |      |
|                                                              |      |



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

## stackoverflow [Prefer composition over inheritance?](https://stackoverflow.com/questions/49002/prefer-composition-over-inheritance)





## wikipedia [Design Patterns#Introduction, Chapter 1](https://en.wikipedia.org/wiki/Design_Patterns#Introduction,_Chapter_1)	



## wikipedia [Composition over inheritance](https://en.wikipedia.org/wiki/Composition_over_inheritance)



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

### Composition and interfaces



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

Finally, introduce a class named `Object` with members to control its visibility (using a `VisibilityDelegate`), movability (using an `UpdateDelegate`), and solidity (using a `CollisionDelegate`). This class has methods which delegate to its members, e.g. `update()` simply calls a method on the `UpdateDelegate`:

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

Then, concrete classes would look like:

```C++
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

```

### Benefits
# Virtual inheritance

C++引入的Virtual inheritance特性就是为了解决前面提到的 [the diamond problem](https://en.wikipedia.org/wiki/Multiple_inheritance#The_diamond_problem) 的。

virtual base class是virtual inheritance的base class。

## TODO cppreference [Virtual base classes](https://en.cppreference.com/w/cpp/language/derived_class#Virtual_base_classes)





## wikipedia [Virtual inheritance](https://en..org/wiki/Virtual_inheritance)

**Virtual inheritance** is a [C++](https://en.wikipedia.org/wiki/C%2B%2B) technique that ensures only one copy of a [base class](https://en.wikipedia.org/wiki/Base_class)'s **member variables** are [inherited](https://en.wikipedia.org/wiki/Inheritance_(computer_science)) by grandchild derived classes.

```
                X
               ^ ^
              /   \
     virtual /     \ virtual
            A       B
             ^     ^
              \   /
               \ /
                C
```

Without **virtual inheritance**, if classes *A* and *B* both inherit from class *X*, and class *C* inherits from classes *A* and *B*, then class *C* will contain two copies of *X'*s **member variables**: one via *A*, and one via *B*. These will be accessible independently, using [scope resolution](https://en.wikipedia.org/wiki/Scope_resolution_operator).

Instead, if classes *A* and *B* inherit virtually from class *X*, then objects of class *C* will contain only one set of the member variables from class *X*.

This feature is most useful for [multiple inheritance](https://en.wikipedia.org/wiki/Multiple_inheritance), as it makes the **virtual base** a common [subobject](https://en.wikipedia.org/wiki/Subobject) for the **deriving class** and all classes that are derived from it. This can be used to avoid the [diamond problem](https://en.wikipedia.org/wiki/Diamond_problem) by clarifying ambiguity over which ancestor class to use, as from the perspective of the deriving class (*C* in the example above) the virtual base (*X*) acts as though it were the direct base class of *C*, not a class derived indirectly through its base (*A*)

```cpp
struct Animal
{
	virtual ~Animal()
	{
	}
	virtual void eat()
	{
	}
};

struct Mammal: Animal
{
	virtual void breathe()
	{
	}
};

struct WingedAnimal: Animal
{
	virtual void flap()
	{
	}
};

// A bat is a winged mammal
struct Bat: Mammal, WingedAnimal
{
};
int main()
{
	Bat bat;
}
// g++ test.cpp
```

> NOTE: 
>
> 需要注意的是: 上述程序是能够编译通过的，并且也能够运行，下面的程序是无法编译通过的:
>
> ```C++
> struct Animal
> 
> {
> 
> 	virtual ~Animal()
> 
> 	{
> 
> 	}
> 
> 	virtual void eat()
> 
> 	{
> 
> 	}
> 
> };
> 
> 
> 
> struct Mammal: Animal
> 
> {
> 
> 	virtual void breathe()
> 
> 	{
> 
> 	}
> 
> };
> 
> 
> 
> struct WingedAnimal: Animal
> 
> {
> 
> 	virtual void flap()
> 
> 	{
> 
> 	}
> 
> };
> 
> 
> 
> // A bat is a winged mammal
> 
> struct Bat: Mammal, WingedAnimal
> 
> {
> 
> };
> 
> int main()
> 
> {
> 
> 	Bat bat;
> 
>     bat.eat();
> 
> }
> 
> // g++ test.cpp
> ```
>
> 编译报错如下:
>
> ```C++
> test.cpp: In function ‘int main()’:
> 
> test.cpp:32:13: error: request for member ‘eat’ is ambiguous
> 
>    32 |         bat.eat();
> 
>       |             ^~~
> 
> test.cpp:6:22: note: candidates are: ‘virtual void Animal::eat()’
> 
>     6 |         virtual void eat()
> 
>       |                      ^~~
> 
> test.cpp:6:22: note:                 ‘virtual void Animal::eat()’
> ```
>
> 

As declared above, a call to `bat.eat()` is ambiguous because there are two `Animal` (indirect) base classes in `Bat`, so any `Bat` object has two different `Animal` base class subobjects(子对象). So an attempt to directly bind a reference to the `Animal` subobject of a `Bat` object would fail, since the binding is inherently(本质上来说) ambiguous:

```cpp
Bat b;
Animal &a = b; // error: which Animal subobject should a Bat cast into, 
               // a Mammal::Animal or a WingedAnimal::Animal?
```

To disambiguate, one would have to explicitly convert `bat` to either base class subobject:

```cpp
Bat b;
Animal &mammal = static_cast<Mammal&> (b); 
Animal &winged = static_cast<WingedAnimal&> (b);
```

In order to call `eat()`, the same disambiguation, or **explicit qualification** is needed: `static_cast<Mammal&>(bat).eat()` or `static_cast<WingedAnimal&>(bat).eat()` or alternatively `bat.Mammal::eat()` and `bat.WingedAnimal::eat()`. **Explicit qualification** not only uses an easier, uniform syntax for both pointers and objects but also allows for static dispatch, so it would arguably be the preferable method.

> NOTE: 
>
> 一、可以看到，上述grammar是非常冗杂的；但是需要明白的是: C++是支持非virtual inheritance的multiple inheritance

In this case, the double inheritance of `Animal` is probably unwanted, as we want to model that the relation (`Bat` is an `Animal`) exists only once; that a `Bat` is a `Mammal` and is a `WingedAnimal` does not imply that it is an `Animal` twice: an `Animal` base class corresponds to a contract(合同) that `Bat` implements (the "is a" relationship above really means "*implements the requirements of*"), and a `Bat` only implements the `Animal` contract once. The real world meaning of "*is a* only once" is that `Bat` should have only one way of implementing `eat()`, not two different ways, depending on whether the `Mammal` view of the `Bat` is eating, or the `WingedAnimal` view of the `Bat`. (In the first code example we see that `eat()` is not overridden in either `Mammal` or `WingedAnimal`, so the two `Animal` subobjects will actually behave the same, but this is just a degenerate(退化) case, and that does not make a difference from the C++ point of view.)

This situation is sometimes referred to as **diamond inheritance** (see [Diamond problem](https://en.wikipedia.org/wiki/Diamond_problem)) because the inheritance diagram is in the shape of a diamond. Virtual inheritance can help to solve this problem.

### The solution

We can re-declare our classes as follows:

```cpp
struct Animal {
  virtual ~Animal() { }
  virtual void eat(){};
};

// Two classes virtually inheriting Animal:
struct Mammal : virtual Animal {
  virtual void breathe(){};
};

struct WingedAnimal : virtual Animal {
  virtual void flap(){};
};

// A bat is still a winged mammal
struct Bat : Mammal, WingedAnimal {
};
```

> NOTE: 
>
> 如果要支持multiple inheritance，那么在base class就需要为其加上virtual inheritance使其成为virtual base class

The `Animal` portion of `Bat::WingedAnimal` is now the *same* `Animal` instance as the one used by `Bat::Mammal`, which is to say that a `Bat` has only one, shared, `Animal` instance in its representation and so a call to `Bat::eat()` is unambiguous. Additionally, a direct cast from `Bat` to `Animal` is also unambiguous, now that there exists only one `Animal` instance which `Bat` could be converted to.

The ability to share a single instance of the `Animal` parent between `Mammal` and `WingedAnimal` is enabled by recording the **memory offset** between the `Mammal` or `WingedAnimal` members and those of the base `Animal` within the **derived class**. However this offset can in the general case only be known at runtime, thus `Bat` must become (`vpointer`, `Mammal`, `vpointer`, `WingedAnimal`, `Bat`, `Animal`). There are two [vtable](https://en.wikipedia.org/wiki/Vtable) pointers, one per **inheritance hierarchy** that virtually inherits `Animal`. In this example, one for `Mammal` and one for `WingedAnimal`. The object size has therefore increased by two pointers, but now there is only one `Animal` and no ambiguity. All objects of type `Bat` will use the same `vpointers`, but each `Bat` object will contain its own unique `Animal` object. If another class inherits from `Mammal`, such as `Squirrel`, then the `vpointer` in the `Mammal` part of `Squirrel` will generally be different to the `vpointer` in the `Mammal` part of `Bat` though they may happen to be the same should the `Squirrel` class be the same size as `Bat`.

### Additional Example

```C++
  A  
 / \  
B   C  
 \ /  
  D 
  |
  E
```





## stackoverflow [virtual inheritance](https://stackoverflow.com/questions/419943/virtual-inheritance)





## stackoverflow [In C++, what is a virtual base class?](https://stackoverflow.com/questions/21558/in-c-what-is-a-virtual-base-class)



## Example

来源: https://en.wikipedia.org/wiki/Composition_over_inheritance#Example

类图如下: 

```C++
        Object
      /    |   \
     /     |     \
Visible   Solid   Movable
     \     |      /
      \    |     /
       \   |    /
         Player
```

需要注意的是，在inheritance hierarchy的每一层都需要使用**virtual inheritance**。

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



输出如下:

```C++
virtual void Visible::draw()
virtual void Movable::update()
virtual void Visible::draw()
virtual void Movable::update()
virtual void Visible::draw()
virtual void Movable::update()
virtual void Visible::draw()
virtual void Movable::update()
virtual void Visible::draw()
virtual void Movable::update()
virtual void Object::draw()

```


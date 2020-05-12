# [Decorator pattern](https://en.wikipedia.org/wiki/Decorator_pattern)

In [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming), the **decorator pattern** is a [design pattern](https://en.wikipedia.org/wiki/Design_pattern_(computer_science)) that allows behavior to be added to an individual [object](https://en.wikipedia.org/wiki/Object_(computer_science)), dynamically , without affecting the behavior of other objects from the same [class](https://en.wikipedia.org/wiki/Class_(computer_science)).[[1\]](https://en.wikipedia.org/wiki/Decorator_pattern#cite_note-1) The decorator pattern is often useful for adhering to the [Single Responsibility Principle](https://en.wikipedia.org/wiki/Single_responsibility_principle), as it allows functionality to be divided between classes with unique areas of concern.[[2\]](https://en.wikipedia.org/wiki/Decorator_pattern#cite_note-2) The decorator pattern is structurally nearly identical to the [chain of responsibility pattern](https://en.wikipedia.org/wiki/Chain_of_responsibility_pattern), the difference being that in a chain of responsibility, exactly one of the classes handles the request, while for the decorator, all classes handle the request. 

## Overview 

The Decorator [[3\]](https://en.wikipedia.org/wiki/Decorator_pattern#cite_note-GoF-3) design pattern is one of the twenty-three well-known *GoF design patterns* that describe how to solve recurring design problems to design flexible and reusable object-oriented software, that is, objects that are easier to implement, change, test, and reuse.

总结：In the field of software engineering, design needs to consider the difficulty of implementation, but also needs to consider testing and maintenance. The latter is as important as the former.

What problems can the Decorator design pattern solve? [[4\]](https://en.wikipedia.org/wiki/Decorator_pattern#cite_note-4)

- Responsibilities should be added to (and removed from) an object dynamically at run-time.
- A flexible alternative to subclassing for extending functionality should be provided<!--这段话的意思是：在面向对象领域，我们最常用的extending functionality（扩展功能）的方法是subclassing（子类继承），实际上是存在比使用subclassing来实现此目的的更加灵活替代方案-->.

When using subclassing, different subclasses extend a class in different ways. But an extension is bound to the class at compile-time and can't be changed at run-time<!--这是使用子类继承的缺点所在，也就是说，当没有dynamically的要求的时候，我们也是可以使用subclassing来实现我们的extend functionality目的的，实际上，decorator pattern是也能够支持statically 的-->.

### What solution does the Decorator design pattern describe? 

Define `Decorator` objects that

- implement the interface of the extended (decorated) object (`Component`) transparently by **forwarding** all requests to it and
- perform additional functionality before/after **forwarding** a request.

总结：关于上面这段话中的forwarding的含义，参见[Forwarding (object-oriented programming)](https://en.wikipedia.org/wiki/Forwarding_(object-oriented_programming))

This enables to work through different `Decorator` objects to extend the functionality of an object dynamically at run-time.
See also the UML class and sequence diagram below.

## Intent

The decorator pattern can be used to **extend** (**decorate**) the functionality of a certain object statically, or in some cases at [run-time](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase)), independently of other instances of the same [class](https://en.wikipedia.org/wiki/Class_(computer_science)), provided some groundwork is done at design time<!--只要在设计时完成一些基础工作，装饰器模式可用于静态地（或在某些情况下）或在某些情况下在运行时扩展（装饰）某个对象的功能，而与同一类的其他实例无关。-->. This is achieved by designing a new *Decorator* class that [wraps](https://en.wikipedia.org/wiki/Wrapper_pattern) the original class. This wrapping could be achieved by the following sequence of steps: 

![img](https://upload.wikimedia.org/wikipedia/commons/thumb/e/e9/Decorator_UML_class_diagram.svg/400px-Decorator_UML_class_diagram.svg.png)

 

1. Subclass the original `Component` class into a `Decorator` class (see UML diagram);

2. In the `Decorator` class, add a `Component` pointer as a field;

3. In the `Decorator` class, pass a `Component` to the `Decorator` constructor to initialize the `Component` pointer;

   SUMMARY:The component is decorated by the decorator.

4. In the `Decorator` class, [forward](https://en.wikipedia.org/wiki/Forwarding_(object-oriented_programming)) all `Component` methods to the `Component` pointer; and

5. In the `ConcreteDecorator` class, **override** any `Component` method(s) whose behavior needs to be modified.

This pattern is designed so that multiple decorators can be stacked on top of each other, each time adding a new functionality to the overridden method(s)<!--此模式的设计使得多个装饰器可以堆叠在彼此之上，每次都为重写的方法添加新功能。-->.

思考：如何来进行实现？按照上面的UML中展示的方式进行实现即可；

Note that decorators and the original class object share a common set of **features**. In the previous diagram, the `operation()` method was available in both the decorated and undecorated versions<!--decorator类继承了component类并且还有一个component类型的成员变量-->. 

SUMMARY:This paragraph tell us that the decorator class share a common set of features with the original class object, which is decorated by the decorator.In fact this relationship is also shown in the above UML diagram because the decorator inherits the original class.In this way,decorated and undecorated versions share a common set of features;

The **decoration features**<!--装饰特性--> (e.g., methods, properties, or other members) are usually defined by an interface, [mixin](https://en.wikipedia.org/wiki/Mixin) (a.k.a. [trait](https://en.wikipedia.org/wiki/Trait_(computer_programming))) or class inheritance which is shared by the decorators and the decorated object. In the previous example the class `Component` is inherited by both the `ConcreteComponent` and the subclasses that descend from `Decorator`.

The decorator pattern is an alternative to [subclassing](https://en.wikipedia.org/wiki/Subclass_(computer_science)). Subclassing adds behavior at [compile time](https://en.wikipedia.org/wiki/Compile_time), and the change affects all instances of the original class; decorating can provide new behavior at [run-time](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase)) for selected objects.

总结：This paragraph compares inheritance and decorator patterns

This difference becomes most important when there are several *independent* ways of extending functionality. In some [object-oriented programming languages](https://en.wikipedia.org/wiki/Object-oriented_programming_language), classes cannot be created at runtime, and it is typically not possible to predict, at design time, what combinations of extensions will be needed. This would mean that a new class would have to be made for every possible combination. By contrast, decorators are objects, created at runtime, and can be combined on a per-use basis. The I/O Streams implementations of both [Java](https://en.wikipedia.org/wiki/Java_Platform,_Standard_Edition#java.io) and the [.NET Framework](https://en.wikipedia.org/wiki/.NET_Framework) incorporate（包括） the decorator pattern.

SUPPLEMENT:[here](https://en.wikipedia.org/wiki/Forwarding_(object-oriented_programming)#Complex) is good example to demonstrate the use of decorator pattern。

## Motivation

![img](https://upload.wikimedia.org/wikipedia/commons/thumb/c/c6/UML2_Decorator_Pattern.png/400px-UML2_Decorator_Pattern.png)

UML diagram for the window example 

As an example, consider a window in a [windowing system](https://en.wikipedia.org/wiki/Windowing_system). To allow [scrolling](https://en.wikipedia.org/wiki/Scrolling) of the window's contents, one may wish to add horizontal or vertical [scrollbars](https://en.wikipedia.org/wiki/Scrollbar) to it, as appropriate. Assume windows are represented by instances of the `Window` interface, and assume this class has no functionality for adding scrollbars. One could create a **subclass** `ScrollingWindow` that provides them, or create a `ScrollingWindowDecorator` that adds this functionality to existing `Window` objects. At this point, either solution would be fine.

Now, assume one also desires the ability to add borders to windows. Again, the original `Window` class has no support. The `ScrollingWindow` subclass now poses a problem, because it has effectively created a new kind of window(using inheritance will create a new kind). If one wishes to add border support to many but not *all* windows, one must create subclasses `WindowWithBorder` and `ScrollingWindowWithBorder` etc. This problem gets worse with every new feature or window subtype to be added. 

SUMMARY:it is obvious that using inheritance will spawn too many subclass

For the decorator solution, we simply create a new `BorderedWindowDecorator`—at runtime, we can decorate existing windows with the `ScrollingWindowDecorator` or the `BorderedWindowDecorator` or both, as we see fit(Obviously, using a decorator will be very flexible). Notice that if the functionality needs to be added to all Windows, you could modify the base class and that will do. On the other hand, sometimes (e.g., using external frameworks) it is not possible, legal, or convenient to modify the base class.

SUMMARY:The range of influence of decorator and inheritance is different

Note, in the previous example, that the `SimpleWindow` and `WindowDecorator` classes implement the "Window" interface, which defines the `draw()` method and the `getDescription()` method, that are required in this scenario, in order to decorate a window control.

## Usage[[edit](https://en.wikipedia.org/w/index.php?title=Decorator_pattern&action=edit&section=4)]

A decorator makes it possible to add or alter behavior of an interface at run-time. Alternatively, the [adapter](https://en.wikipedia.org/wiki/Adapter_pattern) can be used when the wrapper must respect a particular interface and must support [polymorphic](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)) behavior, and the [Facade](https://en.wikipedia.org/wiki/Facade_pattern) when an easier or simpler interface to an underlying object is desired.[[5\]](https://en.wikipedia.org/wiki/Decorator_pattern#cite_note-5)

| Pattern                                                  | Intent                                                       |
| -------------------------------------------------------- | ------------------------------------------------------------ |
| [Adapter](https://en.wikipedia.org/wiki/Adapter_pattern) | Converts one interface to another so that it matches what the client is expecting |
| Decorator                                                | Dynamically adds responsibility to the interface by wrapping the original code |
| [Facade](https://en.wikipedia.org/wiki/Facade_pattern)   | Provides a simplified interface                              |

## Structure[[edit](https://en.wikipedia.org/w/index.php?title=Decorator_pattern&action=edit&section=5)]

### UML class and sequence diagram[[edit](https://en.wikipedia.org/w/index.php?title=Decorator_pattern&action=edit&section=6)]

![img](https://upload.wikimedia.org/wikipedia/commons/8/83/W3sDesign_Decorator_Design_Pattern_UML.jpg)

A sample UML class and sequence diagram for the Decorator design pattern. [[6\]](https://en.wikipedia.org/wiki/Decorator_pattern#cite_note-6)

In the above [UML](https://en.wikipedia.org/wiki/Unified_Modeling_Language) [class diagram](https://en.wikipedia.org/wiki/Class_diagram), the abstract `Decorator` class maintains a reference (`component`) to the decorated object (`Component`) and forwards all requests to it (`component.operation()`). This makes `Decorator` transparent (invisible) to clients of `Component`.

Subclasses (`Decorator1`,`Decorator2`) implement additional behavior (`addBehavior()`) that should be added to the `Component` (before/after forwarding a request to it).

The sequence diagram shows the run-time interactions: The `Client` object works through `Decorator1` and `Decorator2` objects to extend the functionality of a `Component1` object.
The `Client` calls `operation()` on `Decorator1`, which forwards the request to `Decorator2`. `Decorator2` performs `addBehavior()` after forwarding the request to `Component1` and returns to `Decorator1`, which performs `addBehavior()` and returns to the `Client`.

## Examples

### C++

Two options are presented here, first a dynamic, runtime-composable decorator (has issues with calling decorated functions unless proxied explicitly) and a decorator that uses mixin inheritance.

#### Dynamic Decorator

```c++
#include <iostream>
#include <cstring>

using namespace std;

struct Shape
{
  virtual string str() = 0;
};

struct Circle : Shape
{
  float radius = 10.0f;
  void resize(float factor) { radius *= factor; }
  string str() override
  {
    return string("A circle of radius ") + to_string(radius);
  }
};

struct ColoredShape : Shape
{
  string color;
  Shape* shape;
  ColoredShape(string clr, Shape* shp)
  {
    color = clr;
    shape = shp;
  }
  string str() override
  {
    return shape->str() + string(" which is coloured ") + color;
  }
};

// usage:
int main()
{
  Circle c;
  ColoredShape cc("red", &c);
  cout << cc.str() << endl;
  // cannot call this:
  cc.resize(1.2); // not part of ColoredShape
}
```

#### Static Decorator (Mixin Inheritance)

This example demonstrates a static Decorator implementation, which is possible due to C++ ability to inherit from the template argument.

```c++
#include <iostream>
#include <cstring>

using namespace std;

struct Circle
{
  float radius = 10.0f;
  void resize(float factor) { radius *= factor; }
  string str()
  {
    return string("A circle of radius") + to_string(radius);
  }
};

template <typename T> struct ColoredShape : T
{
  string color;
  ColoredShape(string clr)
  {
    color = clr;
  }
  string str()
  {
    return T::str() + " which is colored " + color;
  }
};

// usage:
ColoredShape<Circle> red_circle("red");
cout << red_circle.str() << endl;
// and this is legal
red_circle.resize(1.2);
cout << red_circle.str() << endl;
```

 #### Python

The following Python example, taken from [Python Wiki - DecoratorPattern](https://wiki.python.org/moin/DecoratorPattern), shows us how to pipeline decorators to dynamically add many behaviors in an object:

```python
"""
Demonstrated decorators in a world of a 10x10 grid of values 0-255. 
"""

import random

def s32_to_u16( x ):
    if x < 0:
        sign = 0xf000
    else:
        sign = 0
    bottom = x & 0x00007fff
    return bottom | sign

def seed_from_xy( x,y ): return s32_to_u16( x ) | (s32_to_u16( y ) << 16 )

class RandomSquare:
    def __init__( s, seed_modifier ):
        s.seed_modifier = seed_modifier
    def get( s, x,y ):
        seed = seed_from_xy( x,y ) ^ s.seed_modifier
        random.seed( seed )
        return random.randint( 0,255 )

class DataSquare:
    def __init__( s, initial_value = None ):
        s.data = [initial_value]*10*10
    def get( s, x,y ):
        return s.data[ (y*10)+x ] # yes: these are all 10x10
    def set( s, x,y, u ):
        s.data[ (y*10)+x ] = u

class CacheDecorator:
    def __init__( s, decorated ):
        s.decorated = decorated
        s.cache = DataSquare()
    def get( s, x,y ):
        if s.cache.get( x,y ) == None:
            s.cache.set( x,y, s.decorated.get( x,y ) )
        return s.cache.get( x,y )

class MaxDecorator:
    def __init__( s, decorated, max ):
        s.decorated = decorated
        s.max = max
    def get( s, x,y ):
        if s.decorated.get( x,y ) > s.max:
            return s.max
        return s.decorated.get( x,y )

class MinDecorator:
    def __init__( s, decorated, min ):
        s.decorated = decorated
        s.min = min
    def get( s, x,y ):
        if s.decorated.get( x,y ) < s.min:
            return s.min
        return s.decorated.get( x,y )

class VisibilityDecorator:
    def __init__( s, decorated ):
        s.decorated = decorated
    def get( s,x,y ):
        return s.decorated.get( x,y )
    def draw(s ):
        for y in range( 10 ):
             for x in range( 10 ):
                 print "%3d" % s.get( x,y ),
             print

# Now, build up a pipeline of decorators:

random_square = RandomSquare( 635 )
random_cache = CacheDecorator( random_square )
max_filtered = MaxDecorator( random_cache, 200 )
min_filtered = MinDecorator( max_filtered, 100 )
final = VisibilityDecorator( min_filtered )

final.draw()
```

**Note:**

Please do not confuse the Decorator Pattern (or an implementation of this design pattern in Python - as the above example) with [Python Decorators](https://en.wikipedia.org/wiki/Python_syntax_and_semantics#Decorators), a Python language feature. They are different things.

Second to the Python Wiki:

> The Decorator Pattern is a pattern described in the Design Patterns Book. It is a way of apparently modifying an object's behavior, by enclosing it inside a decorating object with a similar interface. This is not to be confused with Python Decorators, which is a language feature for dynamically modifying a function or class.[[7\]](https://en.wikipedia.org/wiki/Decorator_pattern#cite_note-7)





## sourcemaking [Decorator Design Pattern](https://sourcemaking.com/design_patterns/decorator)



## refactoring [Decorator](https://refactoring.guru/design-patterns/decorator)
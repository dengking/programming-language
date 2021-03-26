# wikipedia [Decorator pattern](https://en.wikipedia.org/wiki/Decorator_pattern)

> NOTE: 
>
> 1、内容非常冗杂，不易阅读

In [object-oriented programming](https://en.wikipedia.org/wiki/Object-oriented_programming), the **decorator pattern** is a [design pattern](https://en.wikipedia.org/wiki/Design_pattern_(computer_science)) that allows behavior to be added to an individual [object](https://en.wikipedia.org/wiki/Object_(computer_science)), dynamically , without affecting the behavior of other objects from the same [class](https://en.wikipedia.org/wiki/Class_(computer_science)).[[1\]](https://en.wikipedia.org/wiki/Decorator_pattern#cite_note-1) 

> NOTE: 
>
> 1、仅仅为single object添加新的behavior
>
> 2、同时还需要遵循相同的interface，以支持dynamic polymorphism

The decorator pattern is often useful for adhering to the [Single Responsibility Principle](https://en.wikipedia.org/wiki/Single_responsibility_principle), as it allows functionality to be divided between classes with unique areas of concern.[[2\]](https://en.wikipedia.org/wiki/Decorator_pattern#cite_note-2) 

The decorator pattern is structurally nearly identical to the [chain of responsibility pattern](https://en.wikipedia.org/wiki/Chain_of_responsibility_pattern), the difference being that in a chain of responsibility, exactly one of the classes handles the request, while for the decorator, all classes handle the request. 

> NOTE: 
> 1、这段话要如何理解？关于这段话的理解，可以参考:
>
> a、refactoring.guru [Chain of Responsibility](https://refactoring.guru/design-patterns/chain-of-responsibility)
>
> > [Chain of Responsibility](https://refactoring.guru/design-patterns/chain-of-responsibility) and [Decorator](https://refactoring.guru/design-patterns/decorator) have very similar class structures. Both patterns rely on recursive composition to pass the execution through a series of objects. However, there are several crucial differences.
> >
> > The *CoR* handlers can execute arbitrary operations independently of each other. They can also stop passing the request further at any point. On the other hand, various *Decorators* can extend the object’s behavior while keeping it consistent with the base interface. In addition, decorators aren’t allowed to break the flow of the request.

## Overview 

### What problems can it solve?

1、**Responsibilities** should be added to (and removed from) an object dynamically at run-time.

2、A flexible alternative to subclassing for extending functionality should be provided

> NOTE: 这段话的意思是：在面向对象领域，我们最常用的extending functionality（扩展功能）的方法是subclassing（子类继承），实际上是存在比使用subclassing来实现此目的的更加灵活替代方案

When using subclassing, different subclasses extend a class in different ways. But an extension is bound to the class at compile-time and can't be changed at run-time.

> NOTE: 
>
> 这是使用子类继承的缺点所在，也就是说，当没有dynamically的要求的时候，我们也是可以使用subclassing来实现我们的extend functionality目的的，实际上，decorator pattern是也能够支持statically 的

### What solution does it describe?

Define `Decorator` objects that

1、implement the interface of the extended (decorated) object (`Component`) transparently by **forwarding** all requests to it and

2、perform additional functionality before/after **forwarding** a request.

> NOTE: 总结：关于上面这段话中的forwarding的含义，参见[Forwarding (object-oriented programming)](https://en.wikipedia.org/wiki/Forwarding_(object-oriented_programming))

This enables to work through different `Decorator` objects to extend the functionality of an object dynamically at run-time.

See also the UML class and sequence diagram below.

## Intent



![img](https://upload.wikimedia.org/wikipedia/commons/thumb/e/e9/Decorator_UML_class_diagram.svg/400px-Decorator_UML_class_diagram.svg.png)

 

1、Subclass the original `Component` class into a `Decorator` class (see UML diagram);

2、In the `Decorator` class, add a `Component` pointer as a field;

3、In the `Decorator` class, pass a `Component` to the `Decorator` constructor to initialize the `Component` pointer;

> NOTE:
>
> The component is decorated by the decorator.

4、In the `Decorator` class, [forward](https://en.wikipedia.org/wiki/Forwarding_(object-oriented_programming)) all `Component` methods to the `Component` pointer; and

5、In the `ConcreteDecorator` class, **override** any `Component` method(s) whose behavior needs to be modified.

This pattern is designed so that multiple decorators can be stacked on top of each other, each time adding a new functionality to the overridden method(s).

> NOTE: 
>
> 1、此模式的设计使得多个装饰器可以堆叠在彼此之上，每次都为重写的方法添加新功能。
>
> 2、思考：如何来进行实现？按照上面的UML中展示的方式进行实现即可；

Note that decorators and the original class object share a common set of **features**. In the previous diagram, the `operation()` method was available in both the decorated and undecorated versions. 

> NOTE: 
>
> 1、decorator类继承了component类并且还有一个component类型的成员变量
>
> 2、This paragraph tell us that the decorator class share a common set of features with the original class object, which is decorated by the decorator. In fact this relationship is also shown in the above UML diagram because the decorator inherits the original class. In this way,decorated and undecorated versions share a common set of features;

The **decoration features** (e.g., methods, properties, or other members) are usually defined by an interface, [mixin](https://en.wikipedia.org/wiki/Mixin) (a.k.a. [trait](https://en.wikipedia.org/wiki/Trait_(computer_programming))) or class inheritance which is shared by the decorators and the decorated object. In the previous example the class `Component` is inherited by both the `ConcreteComponent` and the subclasses that descend from `Decorator`.

The decorator pattern is an alternative to [subclassing](https://en.wikipedia.org/wiki/Subclass_(computer_science)). Subclassing adds behavior at [compile time](https://en.wikipedia.org/wiki/Compile_time), and the change affects all instances of the original class; decorating can provide new behavior at [run-time](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase)) for selected objects.

> NOTE: 
>
> 1、This paragraph compares inheritance and decorator patterns
>
> 2、这段总结得是非常好的，在 geeksforgeeks [Decorator Pattern | Set 1 (Background)](https://www.geeksforgeeks.org/decorator-pattern/)、geeksforgeeks [The Decorator Pattern | Set 2 (Introduction and Design)](https://www.geeksforgeeks.org/the-decorator-pattern-set-2-introduction-and-design/) 中的例子就说明了这一点

This difference becomes most important when there are several *independent* ways of extending functionality. In some [object-oriented programming languages](https://en.wikipedia.org/wiki/Object-oriented_programming_language), classes cannot be created at runtime, and it is typically not possible to predict, at design time, what combinations of extensions will be needed. This would mean that a new class would have to be made for every possible combination. By contrast, decorators are objects, created at runtime, and can be combined on a per-use basis. The I/O Streams implementations of both [Java](https://en.wikipedia.org/wiki/Java_Platform,_Standard_Edition#java.io) and the [.NET Framework](https://en.wikipedia.org/wiki/.NET_Framework) incorporate（包括） the decorator pattern.

> NOTE:
>
> 1、[here](https://en.wikipedia.org/wiki/Forwarding_(object-oriented_programming)#Complex) is good example to demonstrate the use of decorator pattern。

## Common Use Cases

> NOTE: 
> 1、原文的内容不易理解

## Alternatives to Decorators

A decorator makes it possible to add or alter behavior of an interface at run-time. Alternatively, the [adapter](https://en.wikipedia.org/wiki/Adapter_pattern) can be used when the wrapper must respect a particular interface and must support [polymorphic](https://en.wikipedia.org/wiki/Polymorphism_(computer_science)) behavior, and the [Facade](https://en.wikipedia.org/wiki/Facade_pattern) when an easier or simpler interface to an underlying object is desired.[[5\]](https://en.wikipedia.org/wiki/Decorator_pattern#cite_note-5)

| Pattern                                                  | Intent                                                       |
| -------------------------------------------------------- | ------------------------------------------------------------ |
| [Adapter](https://en.wikipedia.org/wiki/Adapter_pattern) | Converts one interface to another so that it matches what the client is expecting |
| Decorator                                                | Dynamically adds responsibility to the interface by wrapping the original code |
| [Facade](https://en.wikipedia.org/wiki/Facade_pattern)   | Provides a simplified interface                              |

## Structure 

### UML class and sequence diagram 

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

```C++
#include <iostream>
#include <string>

struct Shape
{
	virtual ~Shape() = default;

	virtual std::string GetName() const = 0;
};

struct Circle: Shape
{
	void Resize(float factor)
	{
		radius *= factor;
	}

	std::string GetName() const override
	{
		return std::string("A circle of radius ") + std::to_string(radius);
	}

	float radius = 10.0f;
};

struct ColoredShape: Shape
{
	ColoredShape(const std::string &color, Shape *shape) :
					color(color), shape(shape)
	{
	}

	std::string GetName() const override
	{
		return shape->GetName() + " which is colored " + color;
	}

	std::string color;
	Shape *shape;
};

int main()
{
	Circle circle;
	ColoredShape colored_shape("red", &circle);
	std::cout << colored_shape.GetName() << std::endl;

}
// g++ main.cpp -Wall -pedantic


```

> NOTE: 
>
> 1、输出如下:
>
> ```
> A circle of radius 10.000000 which is colored red
> ```



```c++
#include <memory>
#include <iostream>
#include <string>

struct WebPage
{
	virtual void display()=0;
	virtual ~WebPage() = default;
};

struct BasicWebPage: WebPage
{
	std::string html;
	void display() override
	{
		std::cout << "Basic WEB page" << std::endl;
	}
	~BasicWebPage() = default;
};

struct WebPageDecorator: WebPage
{
	WebPageDecorator(std::unique_ptr<WebPage> webPage) :
					_webPage(std::move(webPage))
	{
	}
	void display() override
	{
		_webPage->display();
	}
	~WebPageDecorator() = default;
private:
	std::unique_ptr<WebPage> _webPage;
};

struct AuthenticatedWebPage: WebPageDecorator
{
	AuthenticatedWebPage(std::unique_ptr<WebPage> webPage) :
					WebPageDecorator(std::move(webPage))
	{
	}

	void authenticateUser()
	{
		std::cout << "authentification done" << std::endl;
	}
	void display() override
	{
		authenticateUser();
		WebPageDecorator::display();
	}
	~AuthenticatedWebPage() = default;
};

struct AuthorizedWebPage: WebPageDecorator
{
	AuthorizedWebPage(std::unique_ptr<WebPage> webPage) :
					WebPageDecorator(std::move(webPage))
	{
	}

	void authorizedUser()
	{
		std::cout << "authorized done" << std::endl;
	}
	void display() override
	{
		authorizedUser();
		WebPageDecorator::display();
	}
	~AuthorizedWebPage() = default;
};

int main(int argc, char *argv[])
{
	std::unique_ptr<WebPage> myPage = std::make_unique<BasicWebPage>();

	myPage = std::make_unique<AuthorizedWebPage>(std::move(myPage));
	myPage = std::make_unique<AuthenticatedWebPage>(std::move(myPage));
	myPage->display();
	std::cout << std::endl;
	return 0;
}
// g++ main.cpp -Wall -pedantic --std=c++14


```

> NOTE: 
>
> 1、输出如下:
>
> ```
> authentification done
> authorized done
> Basic WEB page
> ```
>
> 2、上述例子展示了: pipeline multiple layer

#### Static Decorator (Mixin Inheritance)

This example demonstrates a static Decorator implementation, which is possible due to C++ ability to inherit from the template argument.

```c++
#include <iostream>
#include <cstring>

using namespace std;

struct Circle
{
	float radius = 10.0f;
	void resize(float factor)
	{
		radius *= factor;
	}
	string str()
	{
		return string("A circle of radius") + to_string(radius);
	}
};

template<typename T> struct ColoredShape: T
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
int main()
{
	// usage:
	ColoredShape<Circle> red_circle("red");
	cout << red_circle.str() << endl;
	// and this is legal
	red_circle.resize(1.2);
	cout << red_circle.str() << endl;
}
// g++ main.cpp -Wall -pedantic


```

> NOTE: 
>
> 1、上述例子展示了template inheritance base 、mixin from above
>
> 2、它和decorator有什么关联？

 ### Python

The following Python example, taken from [Python Wiki - DecoratorPattern](https://wiki.python.org/moin/DecoratorPattern), shows us how to pipeline decorators to dynamically add many behaviors in an object:

```python
"""
Demonstrated decorators in a world of a 10x10 grid of values 0-255.
"""

import random


def s32_to_u16(x):
    if x < 0:
        sign = 0xf000
    else:
        sign = 0
    bottom = x & 0x00007fff
    return bottom | sign


def seed_from_xy(x, y):
    return s32_to_u16(x) | (s32_to_u16(y) << 16)


class RandomSquare:
    def __init__(self, seed_modifier):
        self.seed_modifier = seed_modifier

    def get(self, x, y):
        seed = seed_from_xy(x, y) ^ self.seed_modifier
        random.seed(seed)
        return random.randint(0, 255)


class DataSquare:
    def __init__(self, initial_value=None):
        self.data = [initial_value] * 10 * 10

    def get(self, x, y):
        return self.data[(y * 10) + x]  # yes: these are all 10x10

    def set(self, x, y, u):
        self.data[(y * 10) + x] = u


class CacheDecorator:
    def __init__(self, decorated):
        self.decorated = decorated
        self.cache = DataSquare()

    def get(self, x, y):
        if self.cache.get(x, y) is None:
            self.cache.set(x, y, self.decorated.get(x, y))
        return self.cache.get(x, y)


class MaxDecorator:
    def __init__(self, decorated, max):
        self.decorated = decorated
        self.max = max

    def get(self, x, y):
        if self.decorated.get(x, y) > self.max:
            return self.max
        return self.decorated.get(x, y)


class MinDecorator:
    def __init__(self, decorated, min):
        self.decorated = decorated
        self.min = min

    def get(self, x, y):
        if self.decorated.get(x, y) < self.min:
            return self.min
        return self.decorated.get(x, y)


class VisibilityDecorator:
    def __init__(self, decorated):
        self.decorated = decorated

    def get(self, x, y):
        return self.decorated.get(x, y)

    def draw(self):
        for y in range(10):
            for x in range(10):
                print("%3d" % self.get(x, y))
            print()


# Now, build up a pipeline of decorators:
if __name__ == "__main__":
    random_square = RandomSquare(635)
    random_cache = CacheDecorator(random_square)
    max_filtered = MaxDecorator(random_cache, 200)
    min_filtered = MinDecorator(max_filtered, 100)
    final = VisibilityDecorator(min_filtered)

    final.draw()

```

> NOTE: 
> 1、上述例子是对原文中给出的例子的完善

**Note:**

Please do not confuse the Decorator Pattern (or an implementation of this design pattern in Python - as the above example) with [Python Decorators](https://en.wikipedia.org/wiki/Python_syntax_and_semantics#Decorators), a Python language feature. They are different things.

Second to the Python Wiki:

> The Decorator Pattern is a pattern described in the Design Patterns Book. It is a way of apparently modifying an object's behavior, by enclosing it inside a decorating object with a similar interface. This is not to be confused with Python Decorators, which is a language feature for dynamically modifying a function or class.[[7\]](https://en.wikipedia.org/wiki/Decorator_pattern#cite_note-7)




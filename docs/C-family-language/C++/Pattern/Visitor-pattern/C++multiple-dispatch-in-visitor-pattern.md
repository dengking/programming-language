# C++ multiple dispatch in visitor pattern

关于multiple dispatch，下面这篇文章的分析是非常到位的。

## thegreenplace [A polyglot's guide to multiple dispatch](https://eli.thegreenplace.net/2016/a-polyglots-guide-to-multiple-dispatch/)

> NOTE: 导读: 
>
> `Intersect`的实现依赖于它的两个parameter，按照program to abstraction principle，`Intersect`的两个parameter应该是pointer to interface，每个interface都有多个concrete/implementation，显然这是many-to-many关系，显然对于每一种可能的组合都有对应的implementation，因此需要使用multiple dispatch才能够实现。

### Polymorphism, single dispatch, multiple dispatch

A natural extension of this idea is *multiple dispatch*, wherein the decision which function to call is based on the **runtime types** of *multiple* objects. Why is this useful? It's not a tool programmers reach for very often, but when it *is* appropriate, alternatives（可选方案） tend to be cumbersome and repetitive（重复的）. A telling sign that **multiple dispatch** may be in order is when you have some operation that involves more than one class and there is no single obvious class where this operation belongs. Think of simulating a sound when a drumstick（鸡腿） hits a drum. There are many kinds of drumsticks, and many kinds of drums; their **combinations** produce different sounds. Say we want to write a function (or family of functions) that determines which sound is produced. Should this function be a method of the `Drum` class or the `DrumStick` class? Forcing this decision is one of the follies（罪恶） of classical OOP, and **multiple dispatch** helps us solve it naturally without adding a kludg（组装）e into our design.

> NOTE: 当出现组合的情况时，multiple dispatch就非常有效。

A simpler and more canonical example is computing intersections（交集） of shapes - maybe for computer graphics, or for simulation, or other use cases. A generic shape intersection computation can be complex to implement, but in many specific cases it's easy. For example, computing intersections of rectangles with rectangles is trivial; same for circles and ellipses; rectangles with triangles may be a tiny bit harder, but still much simpler than artibrary polygons, and so on [[2\]](https://eli.thegreenplace.net/2016/a-polyglots-guide-to-multiple-dispatch/#id7).

How do we write code to handle all these cases? All in all, we just need an `intersect` function that takes two shapes and computes an intersection. This function may have a whole bunch of special cases inside for different combinations of shapes it knows how to do easily, before it resorts to some heavy-handed generic polygon intersection approach. Such code, however, would be gross to develop and maintain. Wouldn't it be nice if we could have:

```c++
void Intersect(const Rectangle* r, const Ellipse* e) {
  // implement intersection of rectangle with ellipse
}

void Intersect(const Rectangle* r1, const Rectangle* r2) {
  // implement intersection of rectangle with another rectangle
}

void Intersect(const Shape* s1, const Shape* s2) {
  // implement interesction of two generic shapes
}
```

And then the call `Intersect(some_shape, other_shape)` would just magically dispatch to the right function? This capability is what's most often referred to by *multiple dispatch* in programming language parlance [[3\]](https://eli.thegreenplace.net/2016/a-polyglots-guide-to-multiple-dispatch/#id8).

### A failed attempt in C++

#### Overload

You may be tempted to come up with the following "trivial" solution in C++:

```c++
class Shape {
public:
  virtual std::string name() const {
    return typeid(*this).name();
  }
};

class Rectangle : public Shape {};

class Ellipse : public Shape {};

class Triangle : public Shape {};

// Overloaded Intersect methods.
void Intersect(const Rectangle* r, const Ellipse* e) {
  std::cout << "Rectangle x Ellipse [names r=" << r->name()
            << ", e=" << e->name() << "]\n";
}

void Intersect(const Rectangle* r1, const Rectangle* r2) {
  std::cout << "Rectangle x Rectangle [names r1=" << r1->name()
            << ", r2=" << r2->name() << "]\n";
}

// Fallback to shapes
void Intersect(const Shape* s1, const Shape* s2) {
  std::cout << "Shape x Shape [names s1=" << s1->name()
            << ", s2=" << s2->name() << "]\n";
}
```

Now in `main`:

```c++
Rectangle r1, r2;
Ellipse e;
Triangle t;

std::cout << "Static type dispatch\n";
Intersect(&r1, &e);
Intersect(&r1, &r2);
Intersect(&r1, &t);
```

> NOTE: 完整程序如下:
>
> ```C++
> #include <string>
> #include <iostream>
> #include <typeinfo>
> 
> class Shape
> {
> public:
>   virtual std::string name() const
>   {
>     return typeid(*this).name();
>   }
> };
> 
> class Rectangle: public Shape
> {
> };
> 
> class Ellipse: public Shape
> {
> };
> 
> class Triangle: public Shape
> {
> };
> 
> // Overloaded Intersect methods.
> void Intersect(const Rectangle *r, const Ellipse *e)
> {
> 	std::cout << "Rectangle x Ellipse [names r=" << r->name() << ", e=" << e->name() << "]\n";
> }
> 
> void Intersect(const Rectangle *r1, const Rectangle *r2)
> {
> 	std::cout << "Rectangle x Rectangle [names r1=" << r1->name() << ", r2=" << r2->name() << "]\n";
> }
> 
> // Fallback to shapes
> void Intersect(const Shape *s1, const Shape *s2)
> {
> 	std::cout << "Shape x Shape [names s1=" << s1->name() << ", s2=" << s2->name() << "]\n";
> }
> 
> int main()
> {
> 	Rectangle r1, r2;
> 	Ellipse e;
> 	Triangle t;
> 
> 	std::cout << "Static type dispatch\n";
> 	Intersect(&r1, &e);
> 	Intersect(&r1, &r2);
> 	Intersect(&r1, &t);
> }
> // g++ --std=c++11 test.cpp
> 
> ```



We'll see:

```c++
Static type dispatch
Rectangle x Ellipse [names r=9Rectangle, e=7Ellipse]
Rectangle x Rectangle [names r1=9Rectangle, r2=9Rectangle]
Shape x Shape [names s1=9Rectangle, s2=8Triangle]
```

Note how the intersections get dispatched to **specialized functions** when these exist and to a **generic** catch-all `Shape x Shape` handler when there is no specialized function.

> NOTE: specialized and generic catch-all
>
> 上述例子也可以看出，function overload是multiple dispatch的，关于此处的multiple dispatch，参见 `Theory\Programming-paradigm\Abstraction-and-polymorphism\Polymorphism\Implementation`

So that's it, multiple dispatch works out of the box? Not so fast... What we see here is just C++ **function overloading** in action. The compiler knows the **static, compile-time types** of the pointers passed to the `Intersect` calls, so it just emits the right call. **Function overloading** is great and useful, but this is not the general problem we're trying to solve. In a realistic code-base, you won't be passing pointers to concrete subclasses of `Shape` around. You are almost certainly going to be dealing with pointers to the `Shape` base class（[Design by contract](https://en.wikipedia.org/wiki/Design_by_contract)）. Let's try to see how the code in the previous sample works with **dynamic types**:

```c++
std::unique_ptr<Shape> pr1(new Rectangle);
std::unique_ptr<Shape> pr2(new Rectangle);
std::unique_ptr<Shape> pe(new Ellipse);
std::unique_ptr<Shape> pt(new Triangle);

std::cout << "Dynamic type dispatch\n";
Intersect(pr1.get(), pe.get());
Intersect(pr1.get(), pr2.get());
Intersect(pr1.get(), pt.get());
```

> NOTE: 完整程序如下:
>
> ```C++
> #include <string>
> #include <iostream>
> #include <typeinfo>
> #include <memory>
> 
> class Shape
> {
> public:
>   virtual std::string name() const
>   {
>     return typeid(*this).name();
>   }
> };
> 
> class Rectangle: public Shape
> {
> };
> 
> class Ellipse: public Shape
> {
> };
> 
> class Triangle: public Shape
> {
> };
> 
> // Overloaded Intersect methods.
> void Intersect(const Rectangle *r, const Ellipse *e)
> {
> 	std::cout << "Rectangle x Ellipse [names r=" << r->name() << ", e=" << e->name() << "]\n";
> }
> 
> void Intersect(const Rectangle *r1, const Rectangle *r2)
> {
> 	std::cout << "Rectangle x Rectangle [names r1=" << r1->name() << ", r2=" << r2->name() << "]\n";
> }
> 
> // Fallback to shapes
> void Intersect(const Shape *s1, const Shape *s2)
> {
> 	std::cout << "Shape x Shape [names s1=" << s1->name() << ", s2=" << s2->name() << "]\n";
> }
> 
> int main()
> {
> 	std::unique_ptr<Shape> pr1(new Rectangle);
> 	std::unique_ptr<Shape> pr2(new Rectangle);
> 	std::unique_ptr<Shape> pe(new Ellipse);
> 	std::unique_ptr<Shape> pt(new Triangle);
> 
> 	std::cout << "Dynamic type dispatch\n";
> 	Intersect(pr1.get(), pe.get());
> 	Intersect(pr1.get(), pr2.get());
> 	Intersect(pr1.get(), pt.get());
> }
> // g++ --std=c++11 test.cpp
> 
> ```
>
> 

Prints:

```
Dynamic type dispatch
Shape x Shape [names s1=9Rectangle, s2=7Ellipse]
Shape x Shape [names s1=9Rectangle, s2=9Rectangle]
Shape x Shape [names s1=9Rectangle, s2=8Triangle]
```

Yeah... that's not good. All calls were dispatched to the generic `Shape x Shape` handler, even though the **runtime types** of the objects are different (see the names gathered from `typeid`). This is hardly surprising, because when the compiler sees `Intersect(pr1.get(), pr2.get())`, the **static types** for the two arguments are `Shape*` and `Shape*`. You could be forgiven for thinking that the compiler may invoke **virtual dispatch** here, but **virtual dispatch** in C++ doesn't work this way. It only works when *a virtual method is called on a pointer to a base object*, which is not what's happening here.

> NOTE: 最后一句的意思是，使用一个static type为基类的指针来调用一个virtual method时，才会触发virtual dispatch。

> NOTE: static type和runtime type是c++ type system中的一个非常重要的概念，参见cppreference [Type](https://en.cppreference.com/w/cpp/language/type)。

> NOTE: virtual dispatch其实就是dynamic dispatch，参见 `Theory\Programming-paradigm\Abstraction-and-polymorphism\Polymorphism` 章节。

### Multiple dispatch in C++ with the visitor pattern

I'll admit I'm calling this approach "the visitor pattern" only because this is how it's called [elsewhere](https://en.wikipedia.org/wiki/Double_dispatch#Double_dispatch_in_C) and because I don't have a better name for it. In fact, it's probably closer to an "inverted"(反转) visitor pattern, and in general the pattern name may obscure(使晦涩) the code more than help. So forget about the name, and just study the code.

The last paragraph of the previous section ended with an important observation: **virtual dispatch** in C++ kicks in *only* when a virtual method is called on a pointer to a base object. Let's leverage this idea to simulate **double dispatch** on our hierarchy of shapes. The plan is to arrange `Intersect` to hop through virtual dispatches on both its arguments to get to the right method for their runtime types.

> NOTE: 最后一句的意思是：通过在两个arguments上都使用virtual dispatch来实现为runtime type调用正确的method，这种策略其实就是在 `Theory\Programming-paradigm\Abstraction-and-polymorphism\Polymorphism\Implementation` 中提出的 "Emulating(模拟) multiple dispatch using multiple single dispatch"。下面段落中的: 
>
>  `Intersect(A*, B*)` to a method call `A->Intersect(B)` 
>
> 形象的展示了这种思路。

We'll start by defining `Shape` like this:

```c++
class Shape {
public:
  virtual std::string name() const {
    return typeid(*this).name();
  }

  // Dispatcher that should be called by clients to intersect different shapes.
  virtual void Intersect(const Shape*) const = 0;

  // Specific interesection methods implemented by subclasses. If subclass A
  // has a special way to intersect with subclass B, it should implement
  // InteresectWith(const B*).
  virtual void IntersectWith(const Shape*) const {}
  virtual void IntersectWith(const Rectangle*) const {}
  virtual void IntersectWith(const Ellipse*) const {}
};
```

The `Intersect` method is what the users of the code will invoke. To be able to make use of **virtual dispatches**, we are forced to turn a two-argument call `Intersect(A*, B*)` to a method call `A->Intersect(B)`. The `IntersectWith` methods are **concrete** implementations of intersections the code will dispatch to and should be implemented by subclasses on a case-per-case basis.



```c++
class Rectangle : public Shape {
public:
  virtual void Intersect(const Shape* s) const {
    s->IntersectWith(this);
  }

  virtual void IntersectWith(const Shape* s) const {
    std::cout << "Rectangle x Shape [names this=" << this->name()
              << ", s=" << s->name() << "]\n";
  }

  virtual void IntersectWith(const Rectangle* r) const {
    std::cout << "Rectangle x Rectangle [names this=" << this->name()
              << ", r=" << r->name() << "]\n";
  }
};

class Ellipse : public Shape {
public:
  virtual void Intersect(const Shape* s) const {
    s->IntersectWith(this);
  }

  virtual void IntersectWith(const Rectangle* r) const {
    std::cout << "Ellipse x Rectangle [names this=" << this->name()
              << ", r=" << r->name() << "]\n";
  }
};
```

```c++
std::unique_ptr<Shape> pr1(new Rectangle);
std::unique_ptr<Shape> pr2(new Rectangle);
std::unique_ptr<Shape> pe(new Ellipse);

std::cout << "Dynamic type dispatch\n";
pr1->Intersect(pe.get());
pr1->Intersect(pr2.get());
```

> NOTE: 完整程序如下:
>
> ```C++
> #include <string>
> #include <iostream>
> #include <typeinfo>
> #include <memory>
> class Rectangle;
> class Ellipse;
> 
> class Shape
> {
> public:
> 	virtual std::string name() const
> 	{
> 		return typeid(*this).name();
> 	}
> 
> 	// Dispatcher that should be called by clients to intersect different shapes.
> 	virtual void Intersect(const Shape*) const = 0;
> 
> 	// Specific interesection methods implemented by subclasses. If subclass A
> 	// has a special way to intersect with subclass B, it should implement
> 	// InteresectWith(const B*).
> 	virtual void IntersectWith(const Shape*) const
> 	{
> 	}
> 	virtual void IntersectWith(const Rectangle*) const
> 	{
> 	}
> 	virtual void IntersectWith(const Ellipse*) const
> 	{
> 	}
> };
> 
> class Rectangle: public Shape
> {
> public:
> 	virtual void Intersect(const Shape *s) const
> 	{
> 		s->IntersectWith(this);
> 	}
> 
> 	virtual void IntersectWith(const Shape *s) const
> 	{
> 		std::cout << "Rectangle x Shape [names this=" << this->name() << ", s=" << s->name() << "]\n";
> 	}
> 
> 	virtual void IntersectWith(const Rectangle *r) const
> 	{
> 		std::cout << "Rectangle x Rectangle [names this=" << this->name() << ", r=" << r->name() << "]\n";
> 	}
> };
> 
> class Ellipse: public Shape
> {
> public:
> 	virtual void Intersect(const Shape *s) const
> 	{
> 		s->IntersectWith(this);
> 	}
> 
> 	virtual void IntersectWith(const Rectangle *r) const
> 	{
> 		std::cout << "Ellipse x Rectangle [names this=" << this->name() << ", r=" << r->name() << "]\n";
> 	}
> };
> 
> 
> int main()
> {
> 	std::unique_ptr<Shape> pr1(new Rectangle);
> 	std::unique_ptr<Shape> pr2(new Rectangle);
> 	std::unique_ptr<Shape> pe(new Ellipse);
> 
> 	std::cout << "Dynamic type dispatch\n";
> 	pr1->Intersect(pe.get());
> 	pr1->Intersect(pr2.get());
> }
> // g++ --std=c++11 test.cpp
> 
> ```
>
> 

Will now print:

```C++
Dynamic type dispatch
Ellipse x Rectangle [names this=7Ellipse, r=9Rectangle]
Rectangle x Rectangle [names this=9Rectangle, r=9Rectangle]
```

Success! Even though we're dealing solely in pointers to `Shape`, the right intersections are computed. Why does this work?

As I've mentioned before, the key here is use C++'s **virtual function dispatch capability**, **twice**. Let's trace through one execution to see what's going on. We have:

```c++
pr1->Intersect(pe.get());
```

#### 调用详情

`pr1` is a pointer to `Shape`, and `Intersect` is a **virtual method**. Therefore, the **runtime type**'s `Intersect` is called here, which is `Rectangle::Intersect`. The argument passed into the method is another pointer to `Shape` which at runtime points to an `Ellipse` (`pe`). `Rectangle::Intersect` calls `s->IntersectWith(this)`. The compiler sees that `s` is a `Shape*`, and `IntersectWith` is a **virtual method**, so this is *another* virtual dispatch. What gets called is `Ellipse::IntersectWith`. But which **overload** of this method is called?

> NOTE: 上面这段话详细介绍了两次dispatch发生的详细情况。

This is an extremely crucial point in the explanation, so please focus :-) Here is `Rectangle::Intersect` again:

```c++
virtual void Intersect(const Shape* s) const {
  s->IntersectWith(this);
}
```

`s->IntersectWith` is called with `this`, which the compiler knows is a pointer to `Rectangle`, *statically*. If you wondered why I define `Intersect` in each subclass rather than doing it once in `Shape`, even though its code is exactly the same for each subclass, this is the reason. Had I defined it in `Shape`, the compiler would think the type of `this` is `Shape*` and would always dispatch to the `IntersectWith(const Shape*)` overload. Defining this method in each **subclass** helps the compiler leverage **overloading** to call the right method.

> NOTE: 非常重要的一点

What happens eventually is that the call `pr1->Intersect(pe.get())` gets routed to `Ellipse::IntersectWith(const Rectangle*)`, thanks to two virtual dispatches and one use of method overloading. The end result is double dispatch! [[4\]](https://eli.thegreenplace.net/2016/a-polyglots-guide-to-multiple-dispatch/#id9)

But wait a second, how did we end up with `Ellipse::IntersectWith(Rectangle)`? Shouldn't `pr1->Intersect(pe.get())` go to `Rectangle::IntersectWith(Ellipse)` instead? Well, yes and no. Yes because this is what you'd expect from how the call is syntactically structured. No because you almost certainly want double dispatches to be symmetric. I'll discuss this and other related issues in the next section.

### Symmetry and base-class defaults

When we come up with ways to do **multiple dispatch**, whether in C++ or in other languages, there are two aspects of the solution we should always keep in mind:

1 Does it permit symmetry(对称性)? In other words, does the order of objects dispatched upon matters? And if it doesn't, how much extra code is needed to express this fact.

2 Does base-class default dispatch work as expected? Suppose we create a new subclass of `Rectangle`, called `Square` and we don't explicitly create an `IntersectWith` method for `Square` and `Ellipse`. Will the right thing happen and the intersection between a `Rectangle` and `Ellipse` be invoked when we ask for `Square x Ellipse`? This is the right thing because this is what we've come to expect from class hierarchies in object-oriented languages.

In the visitor-based solution presented above, both aspects will work, though symmetry needs a bit of extra code. The full code sample is [available here](https://github.com/eliben/code-for-blog/blob/master/2016/multiple-dispatch/c%2B%2B/visitor_full.h) (and the accompanying `.cpp` file). It's conceptually similar to the code shown above, but with a bit more details. In particular, it implements symmetry between rectangle and ellipse intersections as follows:

```c++
namespace {

// All intersections between rectangles and ellipses dispatch here.
void SymmetricIntersectRectangleEllipse(const Rectangle* r, const Ellipse* e) {
  std::cout << "IntersectRectangleEllipse [names r=" << r->name()
            << ", e=" << e->name() << "]\n";
}
}

void Rectangle::IntersectWith(const Ellipse* e) const {
  SymmetricIntersectRectangleEllipse(this, e);
}

void Ellipse::IntersectWith(const Rectangle* r) const {
  SymmetricIntersectRectangleEllipse(r, this);
}
```

This ensures that both `rectangle->Intersect(ellipse)` and `ellipse->Intersect(rectangle)` end up in the same function. As far as I know there's not way to do this automatically in the visitor approach, so a bit of extra coding is due when **symmetry** between subclasses is desired.

Note also that this method doesn't *force* symmetry either. If some form of dispatch is order-dependent, it's easy to express.

### The problem with the visitor-based approach

Although the visitor-based approach works, enables fairly clean client code and is efficient (constant time - two virtual calls), there's a glaring(刺目的) issue with it that's apparent with the most cursory look at the code: it's very **intrusive**（侵入的）, and hence hard to maintain.

Imagine we want to add a new kind of shape - a `HyperFrob`. Suppose also that there's an efficient algorithm for intersecting a `HyperFrob` with an `Ellipse`. Ideally, we'd only have to write code for the new functionality:

1 Define the new `HyperFrob` class deriving from `Shape`.

2 Implement the generic `HyperFrob x Shape` intersection algorithm.

3 Implement the specific `HyperFrom x Ellipse` algorithm.

But in reality, we're forced to modify the definition of the base class `Shape` to add an overload of `IntersectWith` for `HyperFrob`. Moreover, if we want intersections between `HyperFrob` and `Ellipse` to be symmetric (which we almost certainly do), we'll have to modify `Ellipse` as well to add the same overload.

If we don't control the `Shape` base class at all, we're in real trouble. This is an instance of the [expression problem](https://en.wikipedia.org/wiki/Expression_problem). I'll have more to say about the expression problem in a future post, but for now the Wikipedia link will have to do. It's not an easy problem to solve in C++, and the approaches to implement multiple dispatch should be judged by how flexible they are in this respect, along with the other considerations.


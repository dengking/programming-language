# Curiously recurring template pattern

模板递归模式，它在C++中有着非常非常广泛的用途。



## wikipedia [Curiously recurring template pattern](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern)

The **curiously recurring template pattern** (**CRTP**) is an idiom in [C++](https://en.wikipedia.org/wiki/C%2B%2B) in which a class `X` derives from a class [template](https://en.wikipedia.org/wiki/Template_(C%2B%2B)) instantiation using `X` itself as template argument.[[1\]](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern#cite_note-1) More generally it is known as **F-bound polymorphism**, and it is a form of [*F*-bounded quantification](https://en.wikipedia.org/wiki/F-bounded_quantification).

> NOTE: 关于 **F-bound polymorphism**，参见 `Theory\Programming-paradigm\Generic-programming\Implementation\Type-requirement\Generics` 章节。

### General form

```C++
// The Curiously Recurring Template Pattern (CRTP)
template <class T>
class Base
{
    // methods within Base can use template to access members of Derived
};
class Derived : public Base<Derived>
{
    // ...
};
```



### Static polymorphism

Typically, the base class template will take advantage of the fact that **member function bodies** (definitions) are not instantiated until long after their declarations, and will use members of the derived class within its own member functions, via the use of a [cast](https://en.wikipedia.org/wiki/Type_conversion); e.g.:

```c++
#include <iostream>

template<class T>
struct Base
{
	void interface()
	{
		// ...
		static_cast<T*>(this)->implementation();
		// ...
	}

	static void static_func()
	{
		// ...
		T::static_sub_func();
		// ...
	}
};

struct Derived: Base<Derived>
{
	void implementation()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	static void static_sub_func()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};
int main()
{
	Derived d;

	d.interface();
	d.static_sub_func();
}
// g++ test.cpp

```

> NOTE: 上述代码展示了两种static polymorphism：
>
> 1 method
>
> 2 static method
>
> 上述`static_cast<T*>(this)`对应的是cppreference [static_cast conversion](https://en.cppreference.com/w/cpp/language/static_cast)中的`2)`。

In the above example, note in particular that the function `Base<Derived>::interface()`, though *declared* before the existence of the `struct Derived` is known by the compiler (i.e., before `Derived` is declared), is not actually *instantiated* by the compiler until it is actually ***called*** by some later code which occurs ***after*** the declaration of `Derived` (not shown in the above example), so that at the time the function "`implementation`" is **instantiated**, the declaration of `Derived::implementation()` is known.

> NOTE: 
>
> CRTP是充分运用compiler的:
>
> 1) Lazyness of template instantiation特性: 直到在`main()`中`d.interface();`时，才instance `interface()` member method
>
> 关于"Lazyness of template instantiation"特性，参见 `C++\Language-reference\Template\Implementation` 章节
>
> 2) optimization原则: 当compiler instance `interface()` member method的时候，它已经知道了`Derived` 的完整的type info，所以compiler会选择`Derived`的implementation。
>
> 关于 optimization原则，参见 `C-and-C++\From-source-code-to-exec\Compile\Optimization` 章节。
>
> 原文的这一段所解释的其实是在`interface()`中，调用了`implementation()`方法，而这个方法在`struct Base`中是没有声明的。这段解释的含义是： `Base<Derived>::interface()`并不会被*instantiated*直到通过`struct Derived`的对象调用了`interface()`方法，这样就保证了`Derived::implementation()` is known。这是compiler编译template的机制，参见`C++\Language-reference\Template\Templates.md`的`Lazyness of template instantiation`段。具体的可执行的例子参见下面的文章thegreenplace [The Curiously Recurring Template Pattern in C++](https://eli.thegreenplace.net/2011/05/17/the-curiously-recurring-template-pattern-in-c)。
>
> 上述例子充分展现了compiler 编译 template的机制。

This technique achieves a similar effect to the use of [virtual functions](https://en.wikipedia.org/wiki/Virtual_function), without the costs (and some flexibility) of [dynamic polymorphism](https://en.wikipedia.org/wiki/Dynamic_polymorphism). This particular use of the CRTP has been called "simulated dynamic binding" by some.[[9\]](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern#cite_note-9) 

However, if base class member functions use CRTP for all member function calls, the overridden functions in the derived class will be selected at compile time. This effectively emulates the virtual function call system at compile time without the costs in size or function call overhead

> NOTE: 原文中的上面两段话解释了为什么在`interface()`中要`static_cast<T*>(this)`即进行`static_cast`的原因，这是为了能够调用到derived class的member method。

### Object counter

> NOTE: 
>
> 1、Object counter mixin

```c++
#include <iostream>

template<typename T>
struct counter
{
	static int objects_created;
	static int objects_alive;

	counter()
	{

		++objects_created;
		++objects_alive;
		std::cout << __PRETTY_FUNCTION__ << " " << objects_created << " " << objects_alive << std::endl;
	}

	counter(const counter&)
	{

		++objects_created;
		++objects_alive;
		std::cout << __PRETTY_FUNCTION__ << " " << objects_created << " " << objects_alive << std::endl;
	}
protected:
	~counter() // objects should never be removed through pointers of this type
	{
		--objects_alive;
		std::cout << __PRETTY_FUNCTION__ << " " << objects_created << " " << objects_alive << std::endl;
	}
};
template<typename T> int counter<T>::objects_created(0);
template<typename T> int counter<T>::objects_alive(0);

class X: counter<X>
{
	// ...
};

class Y: counter<Y>
{
	// ...
};

int main()
{
	{
		X x1, x2, x3, x4, x5;
	}
	{
		Y y1, y2, y3, y4, y5;
	}
}
// g++ test.cpp
```

> NOTE: 上述程序的输出:
>
> ```c++
> counter<T>::counter() [with T = X] 1 1
> counter<T>::counter() [with T = X] 2 2
> counter<T>::counter() [with T = X] 3 3
> counter<T>::counter() [with T = X] 4 4
> counter<T>::counter() [with T = X] 5 5
> counter<T>::~counter() [with T = X] 5 4
> counter<T>::~counter() [with T = X] 5 3
> counter<T>::~counter() [with T = X] 5 2
> counter<T>::~counter() [with T = X] 5 1
> counter<T>::~counter() [with T = X] 5 0
> counter<T>::counter() [with T = Y] 1 1
> counter<T>::counter() [with T = Y] 2 2
> counter<T>::counter() [with T = Y] 3 3
> counter<T>::counter() [with T = Y] 4 4
> counter<T>::counter() [with T = Y] 5 5
> counter<T>::~counter() [with T = Y] 5 4
> counter<T>::~counter() [with T = Y] 5 3
> counter<T>::~counter() [with T = Y] 5 2
> counter<T>::~counter() [with T = Y] 5 1
> counter<T>::~counter() [with T = Y] 5 0
> ```



> NOTE: 
>
> 1、按照文章thegreenplace [The Curiously Recurring Template Pattern in C++](https://eli.thegreenplace.net/2011/05/17/the-curiously-recurring-template-pattern-in-c)中的`struct Comparisons`例子，`struct counter`是一个mixin class。
>
> 2、`struct counter`将destructor声明为`protected`，这与我们平时的将destructor声明为public virtual是不同的，它这样做的原因是：
>
> 避免直接使用`counter`类对象，让`counter`类为abstract class，因为它是一个mixin class。关于这一点，在`C++\Language-reference\Classes\Special-member-functions\Destructor\Destructor.md`的“Make the base classes' destructor **`protected` and nonvirtual**”节中进行了详细介绍。



### Polymorphic chaining

[Method chaining](https://en.wikipedia.org/wiki/Method_chaining)

#### Example: without derived class

```c++
#include<iostream>
using namespace std;
class Printer
{
public:
    Printer(ostream& pstream) : m_stream(pstream) {}
 
    template <typename T>
    Printer& print(T&& t) { m_stream << t; return *this; }
 
    template <typename T>
    Printer& println(T&& t) { m_stream << t << endl; return *this; }
private:
    ostream& m_stream;
};
int main()
{
	Printer{cout}.println("hello").println(500);
}
// g++ --std=c++11 test.cpp

```

#### Example: with derived class

```c++
#include<iostream>
using namespace std;
class Printer
{
public:
    Printer(ostream& pstream) : m_stream(pstream) {}
 
    template <typename T>
    Printer& print(T&& t) { m_stream << t; return *this; }
 
    template <typename T>
    Printer& println(T&& t) { m_stream << t << endl; return *this; }
private:
    ostream& m_stream;
};
class CoutPrinter : public Printer
{
public:
    CoutPrinter() : Printer(cout) {}

    CoutPrinter& SetConsoleColor(Color c)
    {
        // ...
        return *this;
    }
};
int main()
{
	CoutPrinter().print("Hello ").SetConsoleColor(Color.red).println("Printer!"); 
}
```

we "lose" the concrete class as soon as we invoke a function of the base:

```c++
//                           v----- we have a 'Printer' here, not a 'CoutPrinter'
CoutPrinter().print("Hello ").SetConsoleColor(Color.red).println("Printer!"); // compile error
```

This happens because '`print`' is a function of the base - '`Printer`' - and then it returns a '`Printer`' instance.

#### Solution

The CRTP can be used to avoid such problem and to implement "**Polymorphic chaining**": 

```c++
#include <iostream>

using namespace std;
// Base class
template<typename ConcretePrinter>
class Printer
{
public:
	Printer(ostream& pstream)
			: m_stream(pstream)
	{
	}

	template<typename T>
	ConcretePrinter& print(T&& t)
	{
		m_stream << t;
		return static_cast<ConcretePrinter&>(*this);
	}

	template<typename T>
	ConcretePrinter& println(T&& t)
	{
		m_stream << t << endl;
		return static_cast<ConcretePrinter&>(*this);
	}
private:
	ostream& m_stream;
};
enum Color
{
	red,
	blue
};
// Derived class
class CoutPrinter: public Printer<CoutPrinter>
{
public:
	CoutPrinter()
			: Printer(cout)
	{
	}

	CoutPrinter& SetConsoleColor(Color c)
	{
		// ...
		return *this;
	}
};
int main()
{
	// usage
	CoutPrinter().print("Hello ").SetConsoleColor(red).println("Printer!");
}
// g++ --std=c++11 test.cpp

```

> NOTE: 输出如下:
>
> ```
> Hello Printer!
> ```
>
> 



### Polymorphic copy construction

> NOTE: 
>
> 一、virtual clone mixin
>
> 二、下面的实现使用了"OOP interface + template implementation"的做法，至于为什么这样做，在下面的"Pitfalls"章节进行了说明；它是比之前总结的"OOP interface + template implementation"要复杂一些的，因为它是三层的:
>
> 1、OOP interface
>
> 2、base class template
>
> 3、implementation class

When using polymorphism, one sometimes needs to create copies of objects by the base class pointer. A commonly used idiom for this is adding a virtual `clone` function that is defined in every derived class. The CRTP can be used to avoid having to duplicate that function or other similar functions in every derived class.

```c++
#include <iostream>
#include <memory>

#if __cplusplus >= 201402L // C++14 and beyond
using std::make_unique;
#else
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args &&... args)
{
	static_assert(!std::is_array<T>::value, "arrays not supported");
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif

// Base class has a pure virtual function for cloning
class AbstractShape
{
public:
	virtual ~AbstractShape() = default;
	virtual std::unique_ptr<AbstractShape> clone() const = 0;
};

// This CRTP class implements clone() for Derived
template<typename Derived>
class Shape: public AbstractShape
{
public:
	std::unique_ptr<AbstractShape> clone() const override
	{
		// 调用基类的copy constructor，所以必须要将this转换为子类的类型，这就是
		// static_cast<Derived const&>(*this) 的意图，否则代码是无法编译通过的，会报:
        // casts away qualifiers
        // 因为copyconstructor的原型是：Shape(const Shape&)，
		return make_unique<Derived>(static_cast<Derived const&>(*this));
	}

protected:
	// We make clear Shape class needs to be inherited
	Shape() = default;
	Shape(const Shape&) = default;
};

// Every derived class inherits from CRTP class instead of abstract class

class Square: public Shape<Square>
{
public:
	Square()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	virtual ~Square()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	Square(const Square&)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

class Circle: public Shape<Circle>
{
public:
	Circle()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	virtual ~Circle()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	Circle(const Circle&)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

int main()
{
	{
		std::unique_ptr<AbstractShape> Shape = std::unique_ptr<AbstractShape>(new Square());
		std::unique_ptr<AbstractShape> Shape2 = Shape->clone();
		std::unique_ptr<AbstractShape> Shape3 = Shape->clone();
	}
	{
		std::unique_ptr<AbstractShape> Shape = std::unique_ptr<AbstractShape>(new Circle());
		std::unique_ptr<AbstractShape> Shape2 = Shape->clone();
		std::unique_ptr<AbstractShape> Shape3 = Shape->clone();
	}
}

```

> NOTE: 输出如下：
>
> ```c++
> Square::Square()
> Square::Square(const Square&)
> Square::Square(const Square&)
> virtual Square::~Square()
> virtual Square::~Square()
> virtual Square::~Square()
> Circle::Circle()
> Circle::Circle(const Circle&)
> Circle::Circle(const Circle&)
> virtual Circle::~Circle()
> virtual Circle::~Circle()
> virtual Circle::~Circle()
> ```
>
> 

This allows obtaining copies of squares, circles or any other shapes by `shapePtr->clone()`.



> NOTE: 
>
> 1、这叫做Virtual Constructor Idiom，参见`C++\Idiom\Virtual-Constructor\Virtual-clone`
>
> 



### Pitfalls 

One issue with static polymorphism is that without using a general base class like `AbstractShape` from the above example, derived classes cannot be stored homogeneously(同样地)--that is, putting different types derived from the same base class in the same container. For example, a container defined as `std::vector<Shape*>` does not work because `Shape` is not a class, but a template needing specialization. A container defined as `std::vector<Shape<Circle>*>` can only store `Circle`s, not `Square`s. This is because each of the classes derived from the CRTP base class `Shape` is a unique type. A common solution to this problem is to inherit from a shared base class with a virtual destructor, like the `AbstractShape` example above, allowing for the creation of a `std::vector<AbstractShape*>`.

> NOTE: 
>
> 1、这一段的分析是非常好的





## Application

1、static polymorphism

2、mixin

exmaple:

a、object counter mixin

b、virtual clone mixin

c、具体类型由derived class来指定(我在AMUST core的代码重构中，大量使用了这种方式)

d、.....

3、polymorphic chaining



## 我的实践

### Factory method

```c++
template<typename DerivedClass>
class CUstApiMixin
{
public:
	/**
	 * 工厂方法
	 * @param WorkDir
	 * @return
	 */
	template<typename ...Args>
	static DerivedClass* Factory(Args &&...args)
	{
		return new DerivedClass(std::forward<Args>(args)...);
	}
};
```



## fluentcpp [The Curiously Recurring Template Pattern (CRTP)](https://www.fluentcpp.com/2017/05/12/curiously-recurring-template-pattern/)







## TO READ



这个回答中的例子非常好

https://stackoverflow.com/a/4173298

https://www.geeksforgeeks.org/curiously-recurring-template-pattern-crtp-2/

这个答案中进行了benchMark

https://www.fluentcpp.com/2017/05/12/curiously-recurring-template-pattern/
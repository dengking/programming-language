# Curiously recurring template pattern

模板递归模式，它在C++中有着广泛的用途。

## wikipedia [Curiously recurring template pattern](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern)

The **curiously recurring template pattern** (**CRTP**) is an idiom in [C++](https://en.wikipedia.org/wiki/C%2B%2B) in which a class `X` derives from a class [template](https://en.wikipedia.org/wiki/Template_(C%2B%2B)) instantiation using `X` itself as template argument.[[1\]](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern#cite_note-1) More generally it is known as **F-bound polymorphism**, and it is a form of [*F*-bounded quantification](https://en.wikipedia.org/wiki/F-bounded_quantification).

> NOTE: 关于 **F-bound polymorphism**，参见 `Theory\Programming-paradigm\Generic-programming\Generics` 章节。

### Static polymorphism

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

> NOTE: 上述代码展示了两种：
>
> - method
> - static method
>
> 上述`static_cast<T*>(this)`对应的是cppreference [static_cast conversion](https://en.cppreference.com/w/cpp/language/static_cast)中的`2)`。

In the above example, note in particular that the function `Base<Derived>::interface()`, though *declared* before the existence of the `struct Derived` is known by the compiler (i.e., before `Derived` is declared), is not actually *instantiated* by the compiler until it is actually *called* by some later code which occurs *after* the declaration of `Derived` (not shown in the above example), so that at the time the function "`implementation`" is instantiated, the declaration of `Derived::implementation()` is known.

> NOTE: 原文的这一段所解释的其实是在`interface()`中，调用了`implementation()`方法，而这个方法在`struct Base`中是没有声明的。这段解释的含义是： `Base<Derived>::interface()`并不会被*instantiated*直到通过`struct Derived`的对象调用了`interface()`方法，这样就保证了`Derived::implementation()` is known。这是compiler编译template的机制，参见`C++\Language-reference\Template\Templates.md`的`Lazyness of template instantiation`段。具体的可执行的例子参见下面的文章thegreenplace [The Curiously Recurring Template Pattern in C++](https://eli.thegreenplace.net/2011/05/17/the-curiously-recurring-template-pattern-in-c)。



To elaborate on the above example, consider a base class with **no virtual functions**. Whenever the base class calls another member function, it will always call its own base class functions. When we derive a class from this base class, we inherit all the member variables and member functions that weren't overridden (no constructors or destructors). If the derived class calls an inherited function which then calls another member function, that function will never call any derived or overridden member functions in the derived class.

However, if base class member functions use CRTP for all member function calls, the overridden functions in the derived class will be selected at compile time. This effectively emulates the virtual function call system at compile time without the costs in size or function call overhead

> NOTE: 原文中的上面两段话解释了为什么在`interface()`中要`static_cast<T*>(this)`即进行`static_cast`的原因，这是为了能够调用到derived class的member method。

### Object counter

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



> NOTE: 按照下面的文章thegreenplace [The Curiously Recurring Template Pattern in C++](https://eli.thegreenplace.net/2011/05/17/the-curiously-recurring-template-pattern-in-c)中的`struct Comparisons`例子，`struct counter`是一个mixin class。

> NOTE: `struct counter`将destructor声明为`protected`，这与我们平时的将destructor声明为public virtual是不同的，它这样做的原因是：
>
> 避免直接使用`counter`类对象，让`counter`类为abstract class，因为它是一个mixin class。关于这一点，在`C++\Language-reference\Classes\Special-member-functions\Destructor\Destructor.md`的“Make the base classes' destructor **`protected` and nonvirtual**”节中进行了详细介绍。

### Polymorphic chaining

[Method chaining](https://en.wikipedia.org/wiki/Method_chaining)

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



> NOTE: 这叫做Virtual Constructor Idiom，参见`C++\Idiom\Virtual-Constructor\Virtual-Constructor.md`
>
> 另外，与上述写法相关的有：
>
> [Polymorphic clones in modern C++](https://www.fluentcpp.com/2017/09/08/make-polymorphic-copy-modern-cpp/)，收录于`C++\Idiom\Virtual-Constructor\Virtual-Constructor.md`
>
> [How to Return a Smart Pointer AND Use Covariance](https://www.fluentcpp.com/2017/09/12/how-to-return-a-smart-pointer-and-use-covariance/)，收录于`C++\Idiom\Covariant-Return-Types\Return-a-Smart-Pointer-AND-Use-Covariance.md`
>
> 它们的实现依赖于static、dynamic的结合：
>
> static部分是`class Shape`是模板类
>
> dynamic部分是`clone()`是virtual method

## thegreenplace [The Curiously Recurring Template Pattern in C++](https://eli.thegreenplace.net/2011/05/17/the-curiously-recurring-template-pattern-in-c)

```c++
#include <iostream>
using namespace std;

template <typename Child>
struct Base
{
    void interface()
    {
        static_cast<Child*>(this)->implementation();
    }
};

struct Derived : Base<Derived>
{
    void implementation()
    {
        cerr << "Derived implementation\n";
    }
};

int main()
{
    Derived d;
    d.interface();  // Prints "Derived implementation"
}
```

The key to the technique is the strange template trickery that's being used: note that `Derived` inherits from `Base`. What gives? The idea is to "**inject**" the real type of the derived class into the base, *at compile time*, allowing the `static_cast` of `this` in the `interface` to produce the desired result. 

> NOTE: "**inject**"这个词是非常形象的。

The following example is much longer - although it is also a simplification. It presents a generic base class for visiting binary trees in various orders. This base class can be inherited to specify special handling of some types of nodes. Here is the tree node definition and the base class:

```c++
#include <iostream>
#include <assert.h> // header of assert
using namespace std;
struct TreeNode
{
    enum Kind {RED, BLUE};

    TreeNode(Kind kind_, TreeNode* left_ = NULL, TreeNode* right_ = NULL)
        : kind(kind_), left(left_), right(right_)
    {}

    Kind kind;
    TreeNode *left, *right;
};

template <typename Derived>
class GenericVisitor
{
public:
    // 先序遍历
    void visit_preorder(TreeNode* node)
    {
        if (node) {
            dispatch_node(node);
            visit_preorder(node->left);
            visit_preorder(node->right);
        }
    }
	// 中序遍历
    void visit_inorder(TreeNode* node)
    {
        if (node) {
            visit_inorder(node->left);
            dispatch_node(node);
            visit_inorder(node->right);
        }
    }
	// 后序遍历
    void visit_postorder(TreeNode* node)
    {
        if (node) {
            visit_postorder(node->left);
            visit_postorder(node->right);
            dispatch_node(node);
        }
    }

    void handle_RED(TreeNode* node)
    {
        cerr << "Generic handle RED\n";
    }

    void handle_BLUE(TreeNode* node)
    {
        cerr << "Generic handle BLUE\n";
    }

private:
    // Convenience method for CRTP
    //
    Derived& derived()
    {
        return *static_cast<Derived*>(this);
    }

    void dispatch_node(TreeNode* node)
    {
        switch (node->kind) {
            case TreeNode::RED:
                derived().handle_RED(node);
                break;
            case TreeNode::BLUE:
                derived().handle_BLUE(node);
                break;
            default:
                assert(0);
        }
    }
};
```

And a simple derived class:

```c++
class SpecialVisitor : public GenericVisitor<SpecialVisitor>
{
public:
    // 会override基类的方法
    void handle_RED(TreeNode* node)
    {
        cerr << "RED is special\n";
    }
};
```



```c++
int main()
{
	TreeNode N10(TreeNode::RED);
	TreeNode N11(TreeNode::RED);
	TreeNode N00(TreeNode::BLUE, &N10, &N11);
	SpecialVisitor Visitor;
	Visitor.visit_preorder(&N00);
	return 0;
}
```

> NOTE: 将上述三段代码进行合并，然后编译`g++ test.cpp`，运行程序，得到如下输出
>
> ```
> Generic handle BLUE
> RED is special
> RED is special
> ```

Now you can easily implement special handling of various kinds of nodes in subclasses, and use visiting services provided by the base class.

> NOTE: 使用[Inheritance](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)) 的目的是为了[Reusability](https://en.wikipedia.org/wiki/Reusability)。

To reiterate - this is a simplified example, as there are only two kinds of nodes, but in reality there can be many more. Such code would be quite useful inside compilers, where the source is usually parsed into a tree with many different kinds of nodes. Multiple passes in the compiler then process the trees by implementing their own visitors. As a matter of fact, the [Clang compiler frontend](http://clang.llvm.org/) has such a class, named `RecursiveASTVisitor`, which implements a much more complete version of the visitor displayed above.

Without CRTP, there's no way to implement such functionality except resorting to dynamic polymorphism and virtual functions [[2\]](https://eli.thegreenplace.net/2011/05/17/the-curiously-recurring-template-pattern-in-c#id4).

Another interesting example is the following:

```c++
template <typename Derived>
struct Comparisons
{
};


template <typename Derived>
bool operator==(const Comparisons<Derived>& o1, const Comparisons<Derived>& o2)
{
    const Derived& d1 = static_cast<const Derived&>(o1);
    const Derived& d2 = static_cast<const Derived&>(o2);

    return !(d1 < d2) && !(d2 < d1);
}


template <typename Derived>
bool operator!=(const Comparisons<Derived>& o1, const Comparisons<Derived>& o2)
{
    return !(o1 == o2);
}
```

This is a generic base class with some external comparison functions that act on it. What this makes possible is to create a derived class that only defines the `<` operator, making other comparison operators (`==` and `!=` here, but others are trivial to add) possible. Here's a sample derived class:

```c++
class Person : public Comparisons<Person>
{
public:
    Person(string name_, unsigned age_)
        : name(name_), age(age_)
    {}

    friend bool operator<(const Person& p1, const Person& p2);
private:
    string name;
    unsigned age;
};


bool operator<(const Person& p1, const Person& p2)
{
    return p1.age < p2.age;
}
```

Again, this is using CRTP to implement something that could only be possible with virtual functions had we wanted dynamic polymorphism. Sometimes a class like `Comparisons` above is called a *mixin class*:

> In object-oriented programming languages, a mixin is a class that provides a certain functionality to be inherited or just reused by a subclass, while not meant for instantiation (the generation of objects of that class). Inheriting from a mixin is not a form of specialization but is rather a means of collecting functionality. A class may inherit most or all of its functionality from one or more mixins through multiple inheritance.
>
> [[Wikipedia quote](http://en.wikipedia.org/wiki/Mixin)]

So how often is CRTP used in "real life"? I don't have any actual usage statistics, but it appears that this is a useful tool in a C++ programmer's toolbox. The `RecursiveASTVisitor` class from Clang I mentioned above is a very real use case. Clang's parent project LLVM uses CRTP in at least another place (the `HeuristicBase` class in the code generator module).

Boost also uses CRTP for its [Iterator Facade](http://www.boost.org/doc/libs/1_46_0/libs/iterator/doc/iterator_facade.html):

> `iterator_facade` is a base class template that implements the interface of standard iterators in terms of a few core functions and associated types, to be supplied by a derived iterator class.

And finally, Microsoft's Active Template Library (ATL) uses CRTP comprehensively. See, for example, the [CWindowImpl template](http://msdn.microsoft.com/en-us/library/h4616bh2(v=vs.80).aspx).





## fluentcpp [The Curiously Recurring Template Pattern (CRTP)](https://www.fluentcpp.com/2017/05/12/curiously-recurring-template-pattern/)







## TO READ



这个回答中的例子非常好

https://stackoverflow.com/a/4173298

https://www.geeksforgeeks.org/curiously-recurring-template-pattern-crtp-2/

这个答案中进行了benchMark

https://www.fluentcpp.com/2017/05/12/curiously-recurring-template-pattern/
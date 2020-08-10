# Virtual destructor

本文讨论virtual destructor，其实本质上是讨论polymorphism destructor即多态地调用destructor。

## When should my destructor be `virtual`?

### stackoverflow [When to use virtual destructors?](https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors)

Virtual destructors are useful when you can delete an instance of a derived class through a pointer to **base class**:

```cpp
class Base 
{
    // some virtual methods
};

class Derived : public Base
{
    ~Derived()
    {
        // Do some important cleanup
    }
};
```

Here, you'll notice that I didn't declare `Base`'s destructor to be `virtual`. Now, let's have a look at the following snippet:

```cpp
Base *b = new Derived();
// use b
delete b; // Here's the problem!
```

Since Base's destructor is not `virtual` and `b` is a `Base*` pointing to a `Derived` object, `delete b` has [undefined behaviour](https://stackoverflow.com/q/2397984/20984):

> [In `delete b`], if the **static type** of the object to be deleted is different from its **dynamic type**, the **static type** shall be a base class of the **dynamic type** of the object to be deleted and **the static type shall have a virtual destructor or the behavior is undefined**.

> NOTE:以下是我测试的代码，发现在这种情况下，确实如上所说，是不会触发dynamic dispatch，即不会执行`Derived`的析构函数，而只会执行基类的构造函数。

```cpp
#include <iostream>
class Base
{
public://必须要加上public，否则默认情况下，是private
    // some virtual methods
    ~Base()
    {
        // Do some important cleanup
        std::cout<<"Base destructor"<<std::endl;
    }
};

class Derived : public Base
{
    ~Derived()
    {
        // Do some important cleanup
        std::cout<<"derived destructor"<<std::endl;
    }
};

int main()
{
    Base *b = new Derived();
    // use b
    delete b; // Here's the problem!
}
```

In most implementations, the call to the **destructor** will be resolved like any **non-virtual** code, meaning that the destructor of the base class will be called but not the one of the derived class, resulting in a resources leak.

To sum up, always make base classes' destructors `virtual` when they're meant to be manipulated polymorphically.

If you want to prevent the deletion of an instance through a **base class pointer**, you can make the base class destructor **protected and nonvirtual**; by doing so, the compiler won't let you call `delete`on a base class pointer.

> NOTE：这种通过使用编译器来保证正确性的做法是非常值的借鉴的，但是这里为什么`protected`而不是`private`呢？因为如果将其声明为`private`，则derived class就无法调用。其实这里就涉及到了c++中的destructor，代码如下：

```cpp
#include <iostream>
class Base
{
	// some virtual methods
public:
	virtual ~Base()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

class Derived: public Base
{
	~Derived()
	{
		// Do some important cleanup
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

int main()
{
	Base *b = new Derived();
	// use b
	delete b;
}
// g++ test.cpp

```



You can learn more about virtuality and virtual base class destructor in [this article from Herb Sutter](http://www.gotw.ca/publications/mill18.htm).

### isocpp When should my destructor be `virtual`? [¶](https://isocpp.org/wiki/faq/virtual-functions#virtual-dtors) [Δ](https://isocpp.org/wiki/faq/virtual-functions#)



## Why are destructors not `virtual` by default?

我觉得这个问题，应该从C++语言的发展、设计思想来着手进行思考：

从C++语言的发展来分析：C++逐步发展，为了向后兼容。

从设计思想来分析：在`C++\Philosophy.md`中描述了C++的设计思想，其中一个重要的思想是给予programmer极大的自由，在[Destructor](./Destructor.md)的”Make the base classes' destructor **`protected` and nonvirtual**“小节中描述了不希望destructor polymorphism的场景，C++语言给予了programmer这样的自由。

### isocpp Why are destructors not `virtual` by default? [¶](https://isocpp.org/wiki/faq/virtual-functions#virtual-dtor-rationale) [Δ](https://isocpp.org/wiki/faq/virtual-functions#)


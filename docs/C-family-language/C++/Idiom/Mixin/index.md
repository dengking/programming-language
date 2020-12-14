# Mixin

本文讨论C++中的mixin。

## 为什么需要mixin?

对于一些常见功能，比如factor、singleto、clone等，它们的实现模式是非常类似的，为了促进code reuse，使用mixin来将这些概念给予给子类；

## 实现方式

下面是一些实现方式:

1) CRTR + inheritance



## Application

1) Singleton mixin: `C++\Pattern\Singleton\Singleton-mixin`

2) Factory mixin: `C++\Pattern\Factory`

## [Mixins For Cee Plus Plus](https://proxy.c2.com/cgi/fullSearch?search=MixinsForCeePlusPlus)

In the context of C++ [[CeePlusPlus](https://wiki.c2.com/?CeePlusPlus)] the term mixin is usually used to match [BjarneStroustrup](https://wiki.c2.com/?BjarneStroustrup)'s definition:

> *A class that provides some - but not all - of the implementation for a virtual base class is often called a "mixin".*

You can for example have a **virtual abstract base class** that defines the **interfaces** for the import and export of values and a flock of derived classes that implement different means to either export or import, but not both. Then you can combine these second-layer classes to form different concrete classes.

### Mix in from above and Mix in from below

I believe that it's possible to [MixIn](https://wiki.c2.com/?MixIn) from above (parent classes) or [MixIn](https://wiki.c2.com/?MixIn) from below ([AbstractSubclass](https://wiki.c2.com/?AbstractSubclass)es). The above example shows an example of [MixIn](https://wiki.c2.com/?MixIn) from the parent classes through [MultipleInheritance](https://wiki.c2.com/?MultipleInheritance). For [CeePlusPlus](https://wiki.c2.com/?CeePlusPlus), [AbstractSubclass](https://wiki.c2.com/?AbstractSubclass)es written by [UsingTemplates](https://wiki.c2.com/?UsingTemplates).

### Examples for [MixIn](https://wiki.c2.com/?MixIn) through [AbstractSubclass](https://wiki.c2.com/?AbstractSubclass)

The following code implements the [SingletonPattern](https://wiki.c2.com/?SingletonPattern)

```C++
template<class SingletonClass>
class Singleton: public SingletonClass
{
public:
	/** Singleton creation function */
	static Singleton& instance()
	{
		if (_instance.get() == NULL)
		{
			_instance = auto_ptr<Singleton<SingletonClass> >(new Singleton);
			assert(_instance.get() != NULL);
		}
		return *_instance;
	}

protected:
	/** Singleton instance holder */
	static auto_ptr<Singleton<SingletonClass> > _instance;
};

/** static instance definition */
template<class SingletonClass> auto_ptr<Singleton<SingletonClass> > Singleton<SingletonClass>::_instance;

```

If we have a class called `MyClass`:

```c++
class MyClass
{
public:
	virtual void bar();
	virtual ~MyClass();
protected:
	MyClass();
};

```

The **constructor** can either be public or protected depending on the need. A **protected constructor** can ensure that the class will only be possible to be constructed by subclasses in this case the Singleton [MixIn](https://wiki.c2.com/?MixIn).

The [MixIn](https://wiki.c2.com/?MixIn) can be used like:

```C++
void foo()
{
	Singleton<MyClass>::instance().bar();
}

```

or

```c++
class SingletonMyClass: public Singleton<MyClass>
{
};

void foo()
{
	SingletonMyClass::instance().bar();
}

```

The STL's `auto_ptr` is used to ensure proper **destruction**. In this example, the **abstract method** this [MixIn](https://wiki.c2.com/?MixIn) requires from the parent class is the default constructor. The disadvantage is that it is harder to pass parameters through constructor, but the advantage is [OnceAndOnlyOnce](https://wiki.c2.com/?OnceAndOnlyOnce) because any class can be a singleton by mixing in the [AbstractSubclass](https://wiki.c2.com/?AbstractSubclass) without rewriting the same code.
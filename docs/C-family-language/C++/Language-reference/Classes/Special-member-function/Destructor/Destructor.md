# Destructor

本章讨论destructor。

## cppreference [Destructors](https://en.cppreference.com/w/cpp/language/destructor)



## microsoft [Destructors (C++)](https://docs.microsoft.com/en-us/cpp/cpp/destructors-cpp?view=vs-2019)

> NOTE: microsoft 的这篇写的也不错



## Destructor and inheritance

对于OOP，思考destructor和inheritance之间的关系是非常重要的，下面是一些主题：

### Invokation order of  destructor in a class hierarchy

本节标题的含义是：在一个class hierarchy中，destructor的调用次序。

与本节相关的内容有：

- `C++\Language-reference\Classes\Subtype-polymorphism\Inheritance\Pure-virtual-function-and-abstract-class\Pure-virtual-method-called`

### Destructor and polymorphism

本节的内容基于stackoverflow [When to use virtual destructors?](https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors)整理的：

#### Make base classes' destructors `virtual` 

> To sum up, always make base classes' destructors `virtual` when they're meant to be manipulated polymorphically.

这部分内容主要在[Virtual destructor](./Virtual-destructor.md)中介绍。

简单地说就是：允许继承，并且允许runtime polymorphism。

#### Make the base classes' destructor **`protected` and nonvirtual**

> If you want to prevent the deletion of an instance through a **base class pointer**, you can make the base class destructor **protected and nonvirtual**; by doing so, the compiler won't let you call `delete`on a base class pointer.

简单地说就是：允许继承，但是不允许runtime polymorphism。

在维基百科[Curiously recurring template pattern#Object counter](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern#Object_counter)中列举了这样的例子，在`C++\Idiom\Templates-and-generic-programming\Curiously-recurring-template-pattern`中收录了这篇文章：

Object counter：

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

它这样做的原因是：

避免直接使用`counter`类对象，让`counter`类为abstract class，因为它是一个mixin class。

[What are practical uses of a protected constructor?](https://stackoverflow.com/questions/1057221/what-are-practical-uses-of-a-protected-constructor) `#` [A](https://stackoverflow.com/a/1057245)

> When a class is (intended as) an abstract class, a protected constructor is exactly right. In that situation you don't want objects to be instantiated from the class but only use it to inherit from.
>
> There are other uses cases, like when a certain set of construction parameters should be limited to derived classes.

最最典型的例子就是上面的mixin class `counter`

https://stackoverflow.com/questions/5150748/protected-constructor-and-accessibility/5150944





#### Make the destructor `public` and nonvirtual

典型的例子就是STL的container，它们这样的目的是避免STL的用户继承其中的class。在维基百科[Standard Template Library](https://en.wikipedia.org/wiki/Standard_Template_Library)中对此进行了说明：

> STL containers are not intended to be used as base classes (their destructors are deliberately non-virtual); deriving from a container is a common mistake.



#### Guideline: destructor of any base class must be [public and virtual or protected and non-virtual](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#discussion-make-base-class-destructors-public-and-virtual-or-protected-and-nonvirtual)

在cppreference `virtual function specifier#Virtual destructor`中提及了这个。
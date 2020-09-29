# Value and reference semantics

在阅读zhihu [如何评价 C++11 的右值引用（Rvalue reference）特性？ - zihuatanejo的回答 - 知乎](https://www.zhihu.com/question/22111546/answer/31929118) 时，发现了其中对value semantic的讨论，其中“[值语义](https://link.zhihu.com/?target=http%3A//www.parashift.com/c%2B%2B-faq/val-vs-ref-semantics.html)”的链接的文章在下文中收录了。



## isocpp faq Reference and Value Semantics [¶](https://isocpp.org/wiki/faq/value-vs-ref-semantics) [Δ](https://isocpp.org/wiki/faq/value-vs-ref-semantics#)

> NOTE: 原文的内容是比较好的， 但是没有较好地将它们串联起来，也就是读者不容易领悟作者重要表达的思路、思想，因此需要在此将作者的书写思路进行说明: 作者首先说明了value semantic 和 reference semantic，然后着重对两者进行对比，作者对两者的对比，着力点在于分析value semantic在speed上的优势以及它取得这种优势的原因，这个分析占用了原文的大部分内容。
>
> 有了上述认知，就能够更快的掌握原文的内容了。

### What is value and/or reference semantics, and which is best in C++? [¶](https://isocpp.org/wiki/faq/value-vs-ref-semantics#val-vs-ref-semantics) [Δ](https://isocpp.org/wiki/faq/value-vs-ref-semantics#)

With **reference semantics**, assignment is a pointer-copy (i.e., a *reference*). Value (or “copy”) semantics mean assignment copies the value, not just the pointer. C++ gives you the choice: use the assignment `operator` to copy the value (copy/value semantics), or use a pointer-copy to copy a pointer (reference semantics). C++ allows you to override the assignment `operator` to do anything your heart desires, however the default (and most common) choice is to copy the *value.*

> NOTE: 在java、python中，assignment的含义是bind；但是在c++中，assignment的含义是copy；使用本文的观点来看待这个问题就是：python、java都是reference semantic的，而c++是value semantic的。

> NOTE: C++默认是采用的value semantic，只有当我们显式地使用`*`来表示指针，使用`&`来表示reference。

Pros of reference semantics: flexibility and dynamic binding (you get **dynamic binding** in `C++` only when you pass by pointer or pass by reference, not when you pass by value).

> NOTE: C++中，只有当使用pointer、reference的时候，才能够实现dynamic binding。

Pros of value semantics: speed. “Speed” seems like an odd benefit for a feature that requires an object (vs. a pointer) to be copied, but the fact of the matter is that one usually accesses an object more than one copies the object, so the cost of the occasional copies is (usually) more than offset by the benefit of having an actual object rather than a pointer to an object.

> NOTE:value语义的优点:速度。
> “速度”似乎是一个奇怪的对一个功能需要一个对象(比一个指针)被复制,但事实是,通常一个对象访问对象的多个副本,所以偶尔的副本的成本(通常)超过抵消的好处有一个实际的对象而不是一个指向对象的指针。

There are three cases when you have an actual object as opposed to a pointer to an object: local objects, global/`static` objects, and fully contained member objects in a class. The most important of these is the last (“composition”).

> NOTE: composition 对应的是 [cppreference Subobjects](https://en.cppreference.com/w/cpp/language/object#Subobjects) 。

More info about copy-vs-reference semantics is given in the next FAQs. Please read them all to get a balanced perspective. The first few have intentionally been slanted toward value semantics, so if you only read the first few of the following FAQs, you’ll get a warped perspective.

Assignment has other issues (e.g., shallow vs. deep copy) which are not covered here.

### What is “`virtual` data,” and how-can / why-would I use it in C++? [¶](https://isocpp.org/wiki/faq/value-vs-ref-semantics#virt-data) [Δ](https://isocpp.org/wiki/faq/value-vs-ref-semantics#)

For example, `class` `Stack` might have an `Array` member object (using a pointer), and derived `class` `StretchableStack` might **override** the base class member data from `Array` to `StretchableArray`. For this to work, `StretchableArray` would have to inherit from `Array`, so `Stack` would have an `Array*`. `Stack`’s normal constructors would initialize this `Array*` with a `new Array`, but `Stack` would also have a (possibly `protected`) constructor that would accept an `Array*` from a derived class. `StretchableStack`’s constructor would provide a `new StretchableArray` to this special constructor.

```c++
#include <iostream>

class Array
{
public:
	Array()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	virtual ~Array()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

class StretchableArray: public Array
{
public:
	StretchableArray()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	virtual ~StretchableArray()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};

class Stack
{
public:
	Stack()
	{

	}
	virtual void InitArray()
	{
		m_A = new Array();
	}
	virtual ~Stack()
	{
		if (m_A)
		{
			delete m_A;
		}
	}
protected:
	Array* m_A { nullptr };
};

class StretchableStack: public Stack
{
public:
	StretchableStack()
	{

	}
	virtual void InitArray()
	{
		m_A = new StretchableArray();
	}
};

int main()
{
	{
		Stack s;
		s.InitArray();
	}
	std::cout << "----------------------" << std::endl;
	{
		StretchableStack ss;
		ss.InitArray();
	}
}
// g++ --std=c++11 test.cpp 
```

> NOTE: 上述程序的输出如下：
>
> ```c++
> Array::Array()
> virtual Array::~Array()
> ----------------------
> Array::Array()
> StretchableArray::StretchableArray()
> virtual StretchableArray::~StretchableArray()
> virtual Array::~Array()
> 
> ```
>
> 

Pros:

- Easier implementation of `StretchableStack` (most of the code is inherited)
- Users can pass a `StretchableStack` as a kind-of `Stack`

> NOTE: 这描述的是inheritance和dynamic polymorphism的优势

Cons:

- Adds an extra layer of indirection to access the `Array`
- Adds some extra freestore allocation overhead (both `new` and `delete`)
- Adds some extra dynamic binding overhead (reason given in next FAQ)

> NOTE: 这描述的是inheritance和dynamic polymorphism的劣势

In other words, we succeeded at making *our* job easier as the implementer of `StretchableStack`, but all our users [pay for it](https://isocpp.org/wiki/faq/value-vs-ref-semantics#costs-of-heap). Unfortunately the extra overhead was imposed on both users of `StretchableStack` *and* on users of `Stack`.

### What’s the difference between `virtual` data and dynamic data? [¶](https://isocpp.org/wiki/faq/value-vs-ref-semantics#virt-vs-dynam-data) [Δ](https://isocpp.org/wiki/faq/value-vs-ref-semantics#)

The easiest way to see the distinction is by an analogy with [virtual functions](https://isocpp.org/wiki/faq/virtual-functions): A `virtual` member function means the declaration (signature) must stay the same in derived classes, but the definition (body) can be overridden. The overriddenness of an inherited member function is a static property of the derived class; it doesn’t change dynamically throughout the life of any particular object, nor is it possible for distinct objects of the derived class to have distinct definitions of the member function.

Now go back and re-read the previous paragraph, but make these substitutions:

- “member function” → “member object”
- “signature” → “type”
- “body” → “exact class”

After this, you’ll have a working definition of `virtual` data.

> NOTE: 上述对比是非常巧妙的，初读可能无法理解其对照的工整。需要结合第一段话的内容来进行解释:
>
> | member function                                              | member object                                                |
> | ------------------------------------------------------------ | ------------------------------------------------------------ |
> | A `virtual` **member function** means the declaration (**signature**) must stay the same in derived classes | A `virtual` **member object** means the declaration (**type**) must stay the same in derived classes |
> | but the definition (**body**) can be overridden              | but the definition (**exact class**) can be overridden       |
>
> 

Another way to look at this is to distinguish “per-object” member functions from “dynamic” member functions. 

A “per-object” member function is a member function that is potentially different in any given instance of an object, and could be implemented by burying a function pointer in the object; this pointer could be `const`, since the pointer will never be changed throughout the object’s life. 

> NOTE: 感觉实现方式就是类有一个成员变量，这个成员变量的类型是function pointer，这个类的构造函数中接收一个function pointer

A “dynamic” member function is a member function that will change dynamically over time; this could also be implemented by a function pointer, but the function pointer would not be const.

> NOTE: 

Extending the analogy, this gives us three distinct concepts for data members:

| concept         | explanation                                                  | 说明                                                         |
| --------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| `virtual` data  | the definition (`class`) of the member object is overridable in derived classes provided its declaration (“type”) remains the same, and this overriddenness is a **static property** of the derived class | - static property<br>- 主要用在OOP inheritance中             |
| per-object-data | any given object of a class can instantiate a different conformal (same type) member object upon initialization (usually a “wrapper” object), and the exact class of the member object is a **static property** of the object that wraps it | - 没有inheritance，它强调的是每个对象可以有不同type的member object，这个member object在initialization时构造，但是它的准确类型是不变的 |
| dynamic-data    | the member object’s exact class can change dynamically over time | dynamic                                                      |



The reason they all look so much the same is that none of this is “supported” in C++. It’s all merely “allowed,” and in this case, the mechanism for faking each of these is the same: a pointer to a (probably abstract) base class. In a language that made these “first class” abstraction mechanisms, the difference would be more striking, since they’d each have a different syntactic variant.

> NOTE: 上述三者，C++都不直接支持，但是是允许的，它们的实现方式的一个共同点是: a pointer to a (probably abstract) base class

### Should I normally use pointers to freestore allocated objects for my data members, or should I use “composition”? [¶](https://isocpp.org/wiki/faq/value-vs-ref-semantics#compos-vs-heap) [Δ](https://isocpp.org/wiki/faq/value-vs-ref-semantics#)

Composition.

Your member objects should normally be “contained” in the composite object (but not always; “wrapper” objects are a good example of where you want a pointer/reference; also the N-to-1-uses-a relationship needs something like a pointer/reference).

There are three reasons why fully contained member objects (“composition”) has better performance than pointers to freestore-allocated member objects:

- Extra layer of indirection every time you need to access the member object
- Extra freestore allocations (`new` in constructor, `delete` in destructor)
- Extra dynamic binding (reason given below)

### What are relative costs of the 3 performance hits associated with allocating member objects from the freestore? [¶](https://isocpp.org/wiki/faq/value-vs-ref-semantics#costs-of-heap) [Δ](https://isocpp.org/wiki/faq/value-vs-ref-semantics#)

The three performance hits are enumerated in the previous FAQ:

- By itself, an extra layer of indirection is small potatoes
- Freestore allocations can be a performance issue (the performance of the typical implementation of `malloc()` degrades when there are many allocations; OO software can easily become “freestore bound” unless you’re careful)
- The extra dynamic binding comes from having a pointer rather than an object. Whenever the C++ compiler can know an object’s *exact* class, [`virtual`](https://isocpp.org/wiki/faq/virtual-functions) function calls can be *statically* bound, which allows inlining. Inlining allows zillions (would you believe half a dozen :-) optimization opportunities such as procedural integration, register lifetime issues, etc. The C++ compiler can know an object’s exact class in three circumstances: local variables, global/`static` variables, and fully-contained member objects

Thus fully-contained member objects allow significant optimizations that wouldn’t be possible under the “member objects-by-pointer” approach. This is the main reason that languages which enforce reference-semantics have “inherent” performance challenges.

*Note: Please read the next three FAQs to get a balanced perspective!*

### Are “`inline` `virtual`” member functions ever actually “inlined”? [¶](https://isocpp.org/wiki/faq/value-vs-ref-semantics#inline-virtuals) [Δ](https://isocpp.org/wiki/faq/value-vs-ref-semantics#)

> NOTE: `inline`发生在compile time，而virtual function一般是需要到run time才能够bind的，因此，从表面来看，两者是存在矛盾之处的，实际上，compiler在某些情况下是可以inline virtual function的，这是本节主要介绍的问题。从下面的内容来看，作者回答本节标题的提问是: Occasionally

Occasionally…

When the object is referenced via a **pointer** or a **reference**, a call to a [`virtual`](https://isocpp.org/wiki/faq/virtual-functions) function **generally** cannot be **inlined**, since the call must be resolved dynamically. Reason: the compiler can’t know which actual code to call until **run-time** (i.e., dynamically), since the code may be from a derived class that was created after the caller was compiled.

> NOTE: runtime polymorphism

Therefore the only time an `inline` `virtual` call can be inlined is when the compiler knows the “exact class” of the object which is the target of the `virtual` function call. This can happen only when the compiler has an actual object rather than a **pointer** or **reference** to an object. I.e., either with a local object, a global/`static` object, or a fully contained object inside a composite. This situation can sometimes happen even with a pointer or reference, for example when functions get inlined, access through a pointer or reference may become direct access on the object.

> NOTE: 下面的例子就是对最后一段话中描述的情况的说明

Note that the difference between inlining and non-inlining is normally *much* more significant than the difference between a regular function call and a `virtual` function call. For example, the difference between a regular function call and a `virtual` function call is often just two extra memory references, but the difference between an `inline` function and a non-`inline` function can be as much as an order of magnitude (for zillions of calls to insignificant member functions, loss of inlining `virtual` functions can result in 25X speed degradation! [Doug Lea, “Customization in C++,” proc Usenix C++ 1990]).

A practical consequence of this insight: don’t get bogged down（陷入） in the endless debates (or sales tactics!) of compiler/language vendors who compare the cost of a `virtual` function call on their language/compiler with the same on another language/compiler. Such comparisons are largely meaningless when compared with the ability of the language/compiler to “`inline` expand” member function calls. I.e., many language implementation vendors make a big stink about how good their dispatch strategy is, but if these implementations don’t *inline* member function calls, the overall system performance would be poor, since it is inlining —*not* dispatching— that has the greatest performance impact.

Here is an example of where virtual calls can be inlined even through a reference. The following code is all in the same translation unit, or otherwise organized such that the optimizer can see all of this code at once.

```c++
#include <stdio.h> // printf
class Calculable
{
public:
	virtual unsigned char calculate() = 0;
};
class X: public Calculable
{
public:
	virtual unsigned char calculate()
	{
		return 1;
	}
};
class Y: public Calculable
{
public:
	virtual unsigned char calculate()
	{
		return 2;
	}
};
static void print(Calculable &c)
{
	printf("%d\n", c.calculate());
	printf("+1: %d\n", c.calculate() + 1);
}
int main()
{
	X x;
	Y y;
	print(x);
	print(y);
}
// g++ test.cpp

```



### Sounds like I should never use reference semantics, right? [¶](https://isocpp.org/wiki/faq/value-vs-ref-semantics#ref-semantics-sometimes-good) [Δ](https://isocpp.org/wiki/faq/value-vs-ref-semantics#)





### Does the poor performance of reference semantics mean I should pass-by-value? [¶](https://isocpp.org/wiki/faq/value-vs-ref-semantics#pass-by-value) [Δ](https://isocpp.org/wiki/faq/value-vs-ref-semantics#)
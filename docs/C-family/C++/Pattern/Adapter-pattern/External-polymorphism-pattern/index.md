# External polymorphism pattern

在阅读 stackoverflow [Type erasure techniques](https://stackoverflow.com/questions/5450159/type-erasure-techniques) # Comments 时，其中提及了 external polymorphism pattern，搜索 了一下，发现它的实现是值得借鉴的。



## informit [Design Patterns with Signature-Based Polymorphism: A C++ Approach](https://www.informit.com/articles/article.aspx?p=700594)

> NOTE: 这篇文章给出的例子非常好，能够让reader非常快的掌握external polymorphism pattern

### External Polymorphism

Initially described in note [8], external polymorphism is a design pattern based on Adapter (see note [4]) that demonstrates almost all the features of a signature. Still based on ABCs, it allows decoupling between interface and implementation and signature-based polymorphism. There is a bonus, too: methods with different signatures can be adapted to respond to the same interface call.

The following example illustrates the technique ([refer to file 6.h](https://www.informit.com/content/images/art_braniste_sigbasedpoly/elementLinks/6.h)):

```C++
#include <iostream>

struct IPrintable
{
	virtual void print(const char*) = 0;
};
struct Printer
{
	void print()
	{
		std::cout << "print internal printJob" << std::endl;
	}
};
struct Fax
{
	void fax(const char *something)
	{
		std::cout << "fax & print " << something << std::endl;
	}
};
template<class T>
void adapt(T *t, const char *something)
{
	t->print(something);
}
template<>
void adapt<Fax>(Fax *t, const char *something)
{
	t->fax(something);
}
template<>
void adapt<Printer>(Printer *t, const char *something)
{
	t->print();
}

template<class T>
struct PrintableAdapter: IPrintable
{
	PrintableAdapter(T *t) :
					t_(t)
	{
	}
	virtual void print(const char *something)
	{
		adapt(t_, something);
	}
private:
	T *t_;
};

void test()
{
	Fax f;
	PrintableAdapter<Fax> paf(&f);
	IPrintable *p = &paf;
	p->print("something");
	Printer pr;
	PrintableAdapter<Printer> pap(&pr);
	p = &pap;
	p->print(0);
}
int main()
{
	test();
}
// g++ -Wall -pedantic test.cpp && ./a.out

```

> NOTE: 
>
> 一、上述code是典型的: OOP interface + template implementation
>
> 二、上述code实现了double dispatch:
>
> 1、`IPrintable`的`virtual void print(const char*) = 0;`构建了第一层dispatch，这是dynamic dispatch
>
> 2、`adapt` function template 和 它的full specialization构建了第二层dispatch，这是static dispatch
>
> 

`IPrintable` is the signature, implemented as an ABC. Instead of inheriting from `IPrintable`, as is customary(惯常的) in **subtype polymorphic design**, `Fax` and `Printer` are plain, independent classes with different implicit interfaces. There is another class, a `PrintableAdapter`, which publicly inherits `IPrintable` and keeps the address of the adaptee of a generic type. As a virtual function cannot be a point of specialization, a free function, `adapt()`, is used as the real adapter, being specialized for types having methods different from what `IPrintable` expects. For example, `Fax::fax()` has the right signature but the wrong name and `Printer::print()` is the other way around. On the other hand, any attempt to instantiate `PrintableAdapter` with non-conformant class interfaces fails at compile-time.

This method has the advantage of being simple and easy to be optimized by the compiler (all the calls, except the virtual one, are being inlined).

On the downside, compared with signature-based interfaces:

- The ABC interface is fixed and cannot expose a variable number of methods.
- The performance might be diminished due to the virtual call.
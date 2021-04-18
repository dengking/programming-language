# sodocumentation [C++Type Erasure](https://sodocumentation.net/cplusplus/topic/2872/type-erasure)

## Introduction

Type erasure is a set of techniques for creating a type that can provide a **uniform interface** to various underlying types, while hiding the underlying type information from the client. `std::function<R(A...)>`, which has the ability to hold callable objects of various types, is perhaps the best known example of type erasure in C++.

> NOTE: 
>
> 1、上述 "**uniform interface**"，其实就是consistent interface

## Basic mechanism

**Type erasure** is a way to hide the type of an object from code using it, even though it is not derived from a **common base class**. In doing so, it provides a bridge between the worlds of **static polymorphism** (templates; at the place of use, the exact type must be known at compile time, but it need not be declared to conform to an **interface** at definition) and **dynamic polymorphism** (inheritance and virtual functions; at the place of use, the exact type need not be known at compile time, but must be declared to conform to an interface at definition).

> NOTE:
>
> 1、glue between GP static polymorphism and OOP dynamic polymorphism

### Basic mechanism of type erasure

The following code shows the basic mechanism of type erasure.

```C++
#include <ostream>

class Printable
{
public:
  template <typename T>
  Printable(T value) : pValue(new Value<T>(value)) {}
  ~Printable() { delete pValue; }
  void print(std::ostream &os) const { pValue->print(os); }

private:
  Printable(Printable const &)        /* in C++1x: =delete */; // not implemented
  void operator = (Printable const &) /* in C++1x: =delete */; // not implemented
  struct ValueBase
  {
      virtual ~ValueBase() = default;
      virtual void print(std::ostream &) const = 0;
  };
  template <typename T>
  struct Value : ValueBase
  {
      Value(T const &t) : v(t) {}
      virtual void print(std::ostream &os) const { os << v; }
      T v;
  };
  ValueBase *pValue;
};
```

At the use site, only the above definition need to be visible, just as with base classes with virtual functions. For example:

```C++
#include <iostream>

void print_value(Printable const &p)
{
    p.print(std::cout);
}
```

Note that this is *not* a template, but a normal function that only needs to be declared in a header file, and can be defined in an implementation file (unlike templates, whose definition must be visible at the place of use).

At the definition of the concrete type, nothing needs to be known about `Printable`, it just needs to conform to an interface, as with templates:

```C++
struct MyType { int i; };
ostream& operator << (ostream &os, MyType const &mc)
{
  return os << "MyType {" << mc.i << "}";
}
```

We can now pass an object of this class to the function defined above:

```C++
MyType foo = { 42 };
print_value(foo);
```

### 完整测试程序

```C++
#include <iostream>

class Printable
{
public:
	template<typename T>
	Printable(T value) :
					pValue(new Value<T>(value))
	{
	}
	~Printable()
	{
		delete pValue;
	}
	void print(std::ostream &os) const
	{
		pValue->print(os);
	}

private:
	Printable(Printable const&) /* in C++1x: =delete */; // not implemented
	void operator =(Printable const&) /* in C++1x: =delete */; // not implemented
	struct ValueBase
	{
		virtual ~ValueBase() = default;
		virtual void print(std::ostream&) const = 0;
	};
	template<typename T>
	struct Value: ValueBase
	{
		Value(T const &t) :
						v(t)
		{
		}
		virtual void print(std::ostream &os) const
		{
			os << v;
		}
		T v;
	};
	ValueBase *pValue;
};

struct MyType
{
	int i;
};
std::ostream& operator <<(std::ostream &os, MyType const &mc)
{
	return os << "MyType {" << mc.i << "}";
}

void print_value(Printable const &p)
{
	p.print(std::cout);
}

int main()
{
	Printable p { 1 };
	print_value(p);
	print_value(1);
	MyType foo = { 42 };
	print_value(foo);
}
// g++ test.cpp
```



## Erasing down to a Regular type with manual vtable


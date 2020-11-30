# typeid



## 维基百科[Run-time type information#typeid](https://en.wikipedia.org/wiki/Run-time_type_information#typeid)



## cppreference [typeid operator](https://en.cppreference.com/w/cpp/language/typeid)

Queries information of a type.

Used where the [dynamic type](https://en.cppreference.com/w/cpp/language/type#Dynamic_type) of a [polymorphic object](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects) must be known and for static type identification.

### Notes

When applied to an expression of **polymorphic type**, evaluation of a typeid expression may involve **runtime overhead** (a **virtual table lookup**), otherwise `typeid` expression is resolved at **compile time**.

> NOTE: 上述是实现细节

There is no guarantee that the same [std::type_info](https://en.cppreference.com/w/cpp/types/type_info) instance will be referred to by all evaluations of the typeid expression on the same type, although [std::type_info::hash_code](https://en.cppreference.com/w/cpp/types/type_info/hash_code) of those `type_info` objects would be identical, as would be their [std::type_index](https://en.cppreference.com/w/cpp/types/type_index).

```C++
#include <string>
#include <iostream>
#include <typeinfo>
#include <typeindex>
#include <assert.h>     /* assert */
class A
{
};

int main()
{
	const std::type_info &ti1 = typeid(A);
	const std::type_info &ti2 = typeid(A);

	assert(&ti1 == &ti2); // not guaranteed
	assert(ti1.hash_code() == ti2.hash_code()); // guaranteed
	assert(std::type_index(ti1) == std::type_index(ti2)); // guaranteed
}
// g++ --std=c++11 test.cpp

```



## Example

thegreenplace [A polyglot's guide to multiple dispatch](https://eli.thegreenplace.net/2016/a-polyglots-guide-to-multiple-dispatch/)

```C++
#include <string>
#include <iostream>
#include <typeinfo>

class Shape
{
public:
  virtual std::string name() const
  {
    return typeid(*this).name();
  }
};

class Rectangle: public Shape
{
};

class Ellipse: public Shape
{
};

class Triangle: public Shape
{
};

// Overloaded Intersect methods.
void Intersect(const Rectangle *r, const Ellipse *e)
{
	std::cout << "Rectangle x Ellipse [names r=" << r->name() << ", e=" << e->name() << "]\n";
}

void Intersect(const Rectangle *r1, const Rectangle *r2)
{
	std::cout << "Rectangle x Rectangle [names r1=" << r1->name() << ", r2=" << r2->name() << "]\n";
}

// Fallback to shapes
void Intersect(const Shape *s1, const Shape *s2)
{
	std::cout << "Shape x Shape [names s1=" << s1->name() << ", s2=" << s2->name() << "]\n";
}

int main()
{
	Rectangle r1, r2;
	Ellipse e;
	Triangle t;

	std::cout << "Static type dispatch\n";
	Intersect(&r1, &e);
	Intersect(&r1, &r2);
	Intersect(&r1, &t);
}
// g++ --std=c++11 test.cpp

```

`typeid(*this)`的用法是值得借鉴的。
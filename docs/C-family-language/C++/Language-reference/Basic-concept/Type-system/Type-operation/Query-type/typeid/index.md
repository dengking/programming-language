# typeid



## 维基百科[Run-time type information#typeid](https://en.wikipedia.org/wiki/Run-time_type_information#typeid)



## cppreference [typeid operator](https://en.cppreference.com/w/cpp/language/typeid)

Queries information of a type.

Used where the [dynamic type](https://en.cppreference.com/w/cpp/language/type#Dynamic_type) of a [polymorphic object](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects) must be known and for **static type identification**.

### Notes

When applied to an expression of **polymorphic type**, evaluation of a `typeid` expression may involve **runtime overhead** (a **virtual table lookup**), otherwise `typeid` expression is resolved at **compile time**.

> NOTE: 
>
> 关于 **polymorphic type**，参见 `C++\Language-reference\Basic-concept\Type-system\Type-system\OOP-class-type` 章节；
>
> 对于 **polymorphic type**，`typeid` expression是evaluated at runtime的，因为它是有dynamic type的；
>
> 对于 **non-polymorphic type**，`typeid` expression是evaluated at compile time的，因为它没有dynamic type，只有static type；
>
> 上述是`typeid`的实现细节，可以看到compiler是会充分进行optimize的，显然它是compiler的optimize principle。
>
> 在下面的 "Example: `typeid(*this)`" 段中，列举的例子是对上述的说明。

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

## Example: `typeid(*this)`

下面是例子( 源自 thegreenplace [A polyglot's guide to multiple dispatch](https://eli.thegreenplace.net/2016/a-polyglots-guide-to-multiple-dispatch/) ):

### Polymorphic type

```C++
#include <string>
#include <iostream>
#include <typeinfo>
#include <memory>
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

int main()
{
	std::unique_ptr<Shape> pr1(new Rectangle);
	std::unique_ptr<Shape> pr2(new Rectangle);
	std::unique_ptr<Shape> pe(new Ellipse);

	std::cout << pr1->name() << std::endl;
	std::cout << pr2->name() << std::endl;
	std::cout << pe->name() << std::endl;
}
// g++ --std=c++11 test.cpp
```

输出如下:

```C++
9Rectangle
9Rectangle
7Ellipse
```

显然`typeid(*this)`是evaluated at runtime的，因此`Shape`是polymorphic type，它有dynamic type，所以`typeid(*this)`取到的是dynamic type；

### Non-polymorphic type

```C++
#include <string>
#include <iostream>
#include <typeinfo>
#include <memory>
class Shape
{
public:
	std::string name() const
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



int main()
{
	std::unique_ptr<Shape> pr1(new Rectangle);
	std::unique_ptr<Shape> pr2(new Rectangle);
	std::unique_ptr<Shape> pe(new Ellipse);

	std::cout << pr1->name() << std::endl;
	std::cout << pr2->name() << std::endl;
	std::cout << pe->name() << std::endl;
}
// g++ --std=c++11 test.cpp

```

输出如下:

```C++
5Shape
5Shape
5Shape
```

显然`typeid(*this)`是evaluated at compile time的，因此`Shape`是non-polymorphic type，它没有dynamic type，所以`typeid(*this)`取到的是static type；

### Polymorphic type

```C++
#include <string>
#include <iostream>
#include <typeinfo>
#include <memory>
class Shape
{
public:
	std::string name() const
	{
		return typeid(*this).name();
	}
	virtual void test()
	{

	}
};
class Rectangle: public Shape
{
};

class Ellipse: public Shape
{

};



int main()
{
	std::unique_ptr<Shape> pr1(new Rectangle);
	std::unique_ptr<Shape> pr2(new Rectangle);
	std::unique_ptr<Shape> pe(new Ellipse);

	std::cout << pr1->name() << std::endl;
	std::cout << pr2->name() << std::endl;
	std::cout << pe->name() << std::endl;
}
// g++ --std=c++11 test.cpp

```

输出如下:

```C++
9Rectangle
9Rectangle
7Ellipse
```

显然`typeid(*this)`是evaluated at runtime的，因此`Shape`是polymorphic type，它有dynamic type，所以`typeid(*this)`取到的是dynamic type；

`typeid(*this)`的用法是值得借鉴的。
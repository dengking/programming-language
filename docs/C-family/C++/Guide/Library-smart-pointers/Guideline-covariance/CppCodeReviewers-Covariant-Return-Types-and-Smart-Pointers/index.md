# [CppCodeReviewers](https://github.com/CppCodeReviewers)/**[Covariant-Return-Types-and-Smart-Pointers](https://github.com/CppCodeReviewers/Covariant-Return-Types-and-Smart-Pointers)**

> NOTE: 
>
> 一、本文提出的解决方案简单总结就是使用两次polymorphism(简称为double dispatch)，简单来说就是在原来的virtual clone基础上，在套一层generic `clone` function实现external polymorphism: 
>
> 1、使用一个generic `clone` function实现external polymorphism，来实现return 具体类型的smart pointer，后续统一使用这个generic `clone` function，参见"Clone function"、"Preventing use of `Figure::clone`"
>
> 2、virtual clone + covariant return type
>
> 二、在generic `clone` function 中，为什么要转换为 `base_type`？
>
> 这是因为generic `clone` function 是 `base_type` 的friend，而不是 derived class 的friend，因此需要转换为 `base_type` 才能够编译通过。



```C++
#include <iostream>
#include <typeinfo>

struct Figure
{
	virtual ~Figure() = default;
	virtual Figure* clone() const = 0;
	// ... other methods
};

struct Square: Figure
{
	Square* clone() const override     // return type is Square* instead of Figure* (Covariant Return Type)
	{
		return new Square(*this);
	}
	// ... other methods
};

int main()
{
	Square square;
	Figure &figure = square;

	auto f1 = figure.clone();	// f1 has type Figure*
	std::cout << typeid(f1).name() << '\n';
	auto f2 = square.clone();	// f2 has type Square*
	std::cout << typeid(f2).name() << '\n';
	auto f3 = Square { }.clone(); // f3 has type Square*
	std::cout << typeid(f3).name() << '\n';
}
// g++ --std=c++11 -Wall -pedantic test.cpp && ./a.out

```

> NOTE: 输出如下:
>
> ```
> P6Figure
> P6Square
> P6Square
> ```
>
> 

## What about Smart Pointers?

```C++
#include <iostream>
#include <typeinfo>
#include<memory>
struct Figure
{
	virtual ~Figure() = default;
	virtual std::unique_ptr<Figure> clone() const = 0;
	// ... other methods
};

struct Square: Figure
{
	std::unique_ptr<Square> clone() const override
	{
		return new Square(*this);
	}
	// ... other methods
};
int main()
{
	Square square;
	Figure &figure = square;

	auto f1 = figure.clone();	// f1 has type Figure*
	std::cout << typeid(f1).name() << '\n';
	auto f2 = square.clone();	// f2 has type Square*
	std::cout << typeid(f2).name() << '\n';
	auto f3 = Square { }.clone(); // f3 has type Square*
	std::cout << typeid(f3).name() << '\n';
}
// g++ --std=c++11 -Wall -pedantic test.cpp && ./a.out

```

Unfortunately smart pointers are not treated as covariant types so `virtual std::unique_ptr<Square> clone() override;` will not override `virtual std::unique_ptr<Figure> clone() = 0;`. My compiler (mingw with gcc-4.9) display errors:

```c++
Covariant-Return-Types-and-Smart-Pointers\FigureTests.cpp:34:29: error: invalid covariant return type for 'virtual std::unique_ptr<Square> Square::clone() const'
     std::unique_ptr<Square> clone() const override     // return type is Square* instead of Figure* (Covariant Return Type)
                             ^
Covariant-Return-Types-and-Smart-Pointers\FigureTests.cpp:24:37: error:   overriding 'virtual std::unique_ptr<Figure> Figure::clone() const'
     virtual std::unique_ptr<Figure> clone() const = 0;
```

## Clone function

To be able to use polymorphic magic we need to have `clone()` method inside base class interface but we can also use some helper function which will wrap raw pointers into smart pointers ie.

```c++
std::unique_ptr<Figure> clone(const Figure& object)
{
	return std::unique_ptr<Figure>(object.clone());
}
```





```C++
#include <iostream>
#include <typeinfo>
#include<memory>
struct Figure
{
	virtual ~Figure() = default;
	virtual Figure* clone() const = 0;
	// ... other methods
};

struct Square: Figure
{
	Square* clone() const override
	{
		return new Square(*this);
	}
	// ... other methods
};

template<typename T>
std::unique_ptr<T> clone(const T &object)
{
	return std::unique_ptr<T>(object.clone());
}
int main()
{
	Square square;
	Figure &figure = square;

	auto f1 = clone(figure);	
	std::cout << typeid(f1).name() << '\n';
	auto f2 = clone(square);	
	std::cout << typeid(f2).name() << '\n';
	auto f3 = clone(Square { });
	std::cout << typeid(f3).name() << '\n';
}
// g++ --std=c++11 -Wall -pedantic test.cpp && ./a.out

```

> NOTE: 输出如下:
>
> ```C++
> St10unique_ptrI6FigureSt14default_deleteIS0_EE
> St10unique_ptrI6SquareSt14default_deleteIS0_EE
> St10unique_ptrI6SquareSt14default_deleteIS0_EE
> ```
>
> 

## Preventing use of `Figure::clone`

Method `Figure::clone` is public so user is allowed to call it. We can make it protected and then to make function `clone` able to call `Figure::clone` we can make it friend.

```C++
#include <iostream>
#include <typeinfo>
#include<memory>
struct Figure
{
	virtual ~Figure() = default;

	// ... other methods

protected:
	virtual Figure* clone() const = 0;

	template<typename T>
	friend std::unique_ptr<T> clone(const T&);
};

struct Square: Figure
{
	// ... other methods

protected:
	Square* clone() const override
	{
		return new Square(*this);
	}
};

template<typename T>
std::unique_ptr<T> clone(const T &object)
{
	return std::unique_ptr<T>(object.clone());
}

int main()
{
	Square square;
	Figure &figure = square;

	auto f1 = clone(figure);	// f1 has type Figure*
	std::cout << typeid(f1).name() << '\n';
	auto f2 = clone(square);	// f2 has type Square*
	std::cout << typeid(f2).name() << '\n';
	auto f3 = clone(Square { }); // f3 has type Square*
	std::cout << typeid(f3).name() << '\n';
}
// g++ --std=c++11 -Wall -pedantic test.cpp && ./a.out

```

> NOTE: 编译报错如下:
>
> ```C++
> main.cpp: In instantiation of 'std::unique_ptr<T> clone(const T&) [with T = Square]':
> main.cpp:41:24:   required from here
> main.cpp:31:40: error: 'virtual Square* Square::clone() const' is protected within this context
>    31 |  return std::unique_ptr<T>(object.clone());
>       |                            ~~~~~~~~~~~~^~
> main.cpp:22:10: note: declared protected here
>    22 |  Square* clone() const override
>       |          ^~~~~
> 
> ```
>
> tag-friend-base-derived-inheritance
>
> 

When we call `clone` function on `Figure` reference it works as expected but when called on `Square` it fails to work. We can put friend declaration to every class which inherits from `Figure` but this is not the best what we can do. Lets try to use fact that `clone` function works on Figure reference/pointer. We can do something like that:

```C++
template <typename T>
std::unique_ptr<T> clone(const T& object)
{
	using base_type = Figure;
	auto ptr = static_cast<const base_type&>(object).clone();
	return std::unique_ptr<T>(static_cast<T*>(ptr));
}
```

And now everything works just like before. Unfortunately this code is not so generic - it can only work on classes which inherit Figure class. Lets make it more generic.

```C++
template <typename T>
std::unique_ptr<T> clone(const T& object)
{
	using base_type = typename T::base_type;
	auto ptr = static_cast<const base_type&>(object).clone();
	return std::unique_ptr<T>(static_cast<T*>(ptr));
}
```

and change definition of `Figure` class to:

```C++
struct Figure 
{
	using base_type = Figure;

	virtual ~Figure() = default;	
	
	// ... other methods
	
protected:
	virtual Figure* clone() const = 0;
	
    template <typename T>
    friend std::unique_ptr<T> object::clone(const T&);
};
```

Now `clone` function works on every polymorphic type which has defined `base_type` attribute and virtual `clone()` method. Type T has to inherits `T::base_type` - we can make this restriction explicit by putting `static_assert` into the code:

```C++
template <typename T>
std::unique_ptr<T> clone(const T& object)
{
	using base_type = typename T::base_type;
    static_assert(std::is_base_of<base_type, T>::value, "T object has to derived from T::base_type");	
	auto ptr = static_cast<const base_type&>(object).clone();
	return std::unique_ptr<T>(static_cast<T*>(ptr));
}
```

Can we do better? Yes, we can introduce class which can be inherited to make your type cloneable.

## Generic solution - cloneable type

Lets pack all requirements for `Figure` interface to separate class - lets call it `cloneable` - and lets put there everything which need to be defined to make class cloneable. Then `Figure` class can inherit it and rest should work the same.

```C++
struct Figure;

struct cloneable
{
	using base_type = Figure;

	virtual ~cloneable() = default;	
protected:
	virtual Figure* clone() const = 0;
	
    template <typename T>
    friend std::unique_ptr<T> object::clone(const T&);
}

struct Figure : cloneable
{	
	// ... other methods
};
```

Everything works! Again solution does not look generic - we can eliminate dependency to `Figure` class by using template and [Curiously recurring template pattern](http://en.wikipedia.org/wiki/Curiously_recurring_template_pattern)

```C++
template <typename Type>
struct cloneable
{
	using base_type = Type;

	virtual ~cloneable() = default;	
protected:
	virtual Type* clone() const = 0;
	
    template <typename T>
    friend std::unique_ptr<T> object::clone(const T&);
}

struct Figure : cloneable<Figure> // curiously recurring template pattern
{	
	// ... other methods
};
```

and that's it! We can add helper method to be able to clone object via pointer instead of reference:

```C++
template<typename T>
auto clone(T* object) -> decltype(clone(*object))
{
	return clone(*object);
}
```

and we can put everything in one namespace ie. `object` then entire solution will look like this:

```C++
#include <iostream>
#include <typeinfo>
#include<memory>
namespace object
{
template<typename T>
std::unique_ptr<T> clone(const T &object)
{
	using base_type = typename T::base_type;
	static_assert(std::is_base_of<base_type, T>::value, "T object has to derived from T::base_type");
	auto ptr = static_cast<const base_type&>(object).clone();
	return std::unique_ptr<T>(static_cast<T*>(ptr));
}

template<typename T>
auto clone(T *object) -> decltype(clone(*object))
{
	return clone(*object);
}

template<typename T>
struct cloneable
{
	using base_type = T;

	virtual ~cloneable() = default;
protected:
	virtual T* clone() const = 0;

	template<typename X>
	friend std::unique_ptr<X> object::clone(const X&);
};
}

struct Figure: object::cloneable<Figure>
{
	// ... other methods
};

struct Square: Figure
{
	// ... other methods

protected:
	Square* clone() const override
	{
		return new Square(*this);
	}
};

int main()
{
	Square square;
	Figure &figure = square;

	auto f1 = clone(figure);	// f1 has type Figure*
	std::cout << typeid(f1).name() << '\n';
	auto f2 = clone(square);	// f2 has type Square*
	std::cout << typeid(f2).name() << '\n';
	auto f3 = clone(Square { }); // f3 has type Square*
	std::cout << typeid(f3).name() << '\n';
}
// g++ --std=c++11 -Wall -pedantic test.cpp && ./a.out

```

> NOTE: 输出如下:
>
> ```C++
> St10unique_ptrI6FigureSt14default_deleteIS0_EE
> St10unique_ptrI6SquareSt14default_deleteIS0_EE
> St10unique_ptrI6SquareSt14default_deleteIS0_EE
> ```
>
> 

## Expert question

Can we adjust this solution to take advantage of move semantics when `clone` function is called on rvalue objects?
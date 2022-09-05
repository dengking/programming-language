# cppreference [PImpl](https://en.cppreference.com/w/cpp/language/pimpl) 

"Pointer to implementation" or "pImpl" is a C++ programming technique[[1\]](https://en.cppreference.com/w/cpp/language/pimpl#cite_note-1) that removes implementation details of a class from its object representation by placing them in a separate class, accessed through an opaque pointer:

> NOTE: 
>
> 1、"opaque pointer"是一个非常重要的概念，参见:
>
> a、wikipedia [Opaque pointer](https://en.wikipedia.org/wiki/Opaque_pointer)

```C++
// widget.h (interface)
class widget
{
	// public members
private:
	struct impl;  // forward declaration of the implementation class
	// One implementation example: see below for other design options and trade-offs
	std::experimental::propagate_const< // const-forwarding pointer wrapper
					std::unique_ptr< // unique-ownership opaque pointer
									impl>> pImpl; // to the forward-declared implementation class
};

// widget.cpp (implementation)
struct widget::impl
{
	// implementation details
};

```

This technique is used to construct C++ library interfaces with stable ABI and to reduce compile-time dependencies.

## Explanation

Because **private data members** of a class participate in its object representation, affecting size and layout, and because **private member functions** of a class participate in [overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution) (which takes place before member access checking), any change to those **implementation details** requires recompilation of all users of the class.

> NOTE: 
>
> 1、private data member会参见object representation
>
> 2、private member function会参见overload resolution
>
> 3、上述两个步骤都是implementation details、都发生于compile-time的
>
> 4、因此它们的修改都会导致需要recompile
>
> 

pImpl breaks this compilation dependency; changes to the implementation do not cause recompilation. Consequently, if a library uses pImpl in its ABI, newer versions of the library may change the implementation while remaining ABI-compatible with older versions.

> NOTE: interface、implementation
>
> 将implementation放到专门的 class中，外部类，depend on interface，这样就实现了break dependency，这让我想起来: OOP的"program to an interface,  not a implementation"

## Trade-offs

The alternatives to the pImpl idiom are

1、inline implementation: private members and public members are members of the same class

2、pure abstract class (OOP factory): users obtain a unique pointer to a lightweight or abstract base class, the implementation details are in the derived class that overrides its virtual member functions

## Compilation firewall

In simple cases, both pImpl and factory method break compile-time dependency between the implementation and the users of the class interface. 

Factory method creates a hidden dependency on the vtable, and so reordering, adding, or removing virtual member functions breaks the ABI. 

The pImpl approach has no hidden dependencies, however if the implementation class is a class template specialization, the compilation firewall benefit is lost: the users of the interface must observe(观察) the entire template definition in order to instantiate the correct specialization. A common design approach in this case is to refactor the implementation in a way that avoids parametrization, this is another use case for the C++ Core Guidelines [T.61 Do not over-parametrize members](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rt-scary) and [T.84 Use a non-template core implementation to provide an ABI-stable interface](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#t84-use-a-non-template-core-implementation-to-provide-an-abi-stable-interface)

For example, the following class template does not use the type T in its private members or in the body of push_back





## Example

demonstrates a pImpl with const propagation, with back-reference passed as a parameter, without allocator awareness, and move-enabled without runtime checks

```C++
#include <iostream>
#include <memory>
#include <experimental/propagate_const>

// interface (widget.h)
class widget
{
	class impl;
	std::experimental::propagate_const<std::unique_ptr<impl>> pImpl;
public:
	void draw() const; // public API that will be forwarded to the implementation
	void draw();
	bool shown() const
	{
		return true;
	} // public API that implementation has to call
	widget(int);
	~widget(); // defined in the implementation file, where impl is a complete type
	widget(widget&&); // defined in the implementation file
					  // Note: calling draw() on moved-from object is UB
	widget(const widget&) = delete;
	widget& operator=(widget&&); // defined in the implementation file
	widget& operator=(const widget&) = delete;
};

// implementation (widget.cpp)
class widget::impl
{
	int n; // private data
public:
	void draw(const widget &w) const
	{
		if (w.shown()) // this call to public member function requires the back-reference
			std::cout << "drawing a const widget " << n << '\n';
	}
	void draw(const widget &w)
	{
		if (w.shown())
			std::cout << "drawing a non-const widget " << n << '\n';
	}
	impl(int n) :
					n(n)
	{
	}
};
void widget::draw() const
{
	pImpl->draw(*this);
}
void widget::draw()
{
	pImpl->draw(*this);
}
widget::widget(int n) :
				pImpl { std::make_unique < impl > (n) }
{
}
widget::widget(widget&&) = default;
widget::~widget() = default;
widget& widget::operator=(widget&&) = default;

// user (main.cpp)
int main()
{
	widget w(7);
	const widget w2(8);
	w.draw();
	w2.draw();
}
// g++ -std=c++17  -Wall -pedantic -pthread main.cpp && ./a.out
```

https://coliru.stacked-crooked.com/
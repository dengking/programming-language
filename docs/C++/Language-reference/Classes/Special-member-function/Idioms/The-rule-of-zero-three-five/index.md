# The rule of three/five/zero



## drdobbs [C++ Made Easier: The Rule of Three](https://www.drdobbs.com/c-made-easier-the-rule-of-three/184401400)

### An Example and a Misconception

[example code](./Code/drdobbs/IntVec-Misconception.cpp)

> NOTE: 原文给出的这个例子是非常具有代表性的。
>
> 这个例子，导致错误是double free

## cppreference [The rule of three/five/zero](https://en.cppreference.com/w/cpp/language/rule_of_three)

### Rule of three

If a class requires a user-defined [destructor](https://en.cppreference.com/w/cpp/language/destructor), a user-defined [copy constructor](https://en.cppreference.com/w/cpp/language/copy_constructor), or a user-defined [copy assignment operator](https://en.cppreference.com/w/cpp/language/as_operator), it almost certainly requires all three.

> NOTE: 三者只要有其一，就需要有其三



```c++
#include <cstddef>
#include <cstring>
#include <iostream>
class rule_of_three
{
    char *cstring; // raw pointer used as a handle to a dynamically-allocated memory block

    void init(const char *s)
    {
        std::size_t n = std::strlen(s) + 1;
        cstring = new char[n];
        std::memcpy(cstring, s, n); // populate
    }

public:
    rule_of_three(const char *s = "")
    {
        std::cout << "construcotr" << std::endl;
        init(s);
    }

    ~rule_of_three()
    {
        delete[] cstring; // deallocate
    }

    rule_of_three(const rule_of_three &other) // copy constructor
    {
        std::cout << "copy constructor" << std::endl;
        init(other.cstring);
    }

    rule_of_three &operator=(const rule_of_three &other) // copy assignment
    {
        std::cout << "copy assignment" << std::endl;
        if (this != &other)
        {
            delete[] cstring; // deallocate
            init(other.cstring);
        }
        return *this;
    }
};

int main()
{

    rule_of_three a("rule of three"); // constructor
    rule_of_three a1();               // constructor
    rule_of_three b(a);               // copy constructor
    rule_of_three c = a;              // copy constructor
    c = b;                            // copy assignment
}
```

> NOTE: 上述代码中，copy assignment的实现是非常不好的，比它更好的实现方式是采用copy-and-swap idiom（在`C-family-language\C++\Idiom\OOP\Resource-management\Copy-and-swap`中对它进行了介绍）。

Classes that manage non-copyable resources through copyable handles may have to declare **copy assignment** and **copy constructor** private and not provide their definitions or define them as deleted. This is another application of the rule of three: deleting one and leaving the other to be implicitly-defined will most likely result in errors.

> NOTE: non-copyable resources 的典型例子包括：thread

### Rule of five

Because the presence of a user-defined destructor, copy-constructor, or copy-assignment operator prevents implicit definition of the [move constructor](https://en.cppreference.com/w/cpp/language/move_constructor) and the [move assignment operator](https://en.cppreference.com/w/cpp/language/move_operator), any class for which move semantics are desirable, has to declare all five special member functions:

```c++
class rule_of_five
{
    char* cstring; // raw pointer used as a handle to a dynamically-allocated memory block
 public:
    rule_of_five(const char* s = "")
    : cstring(nullptr)
    { 
        if (s) {
            std::size_t n = std::strlen(s) + 1;
            cstring = new char[n];      // allocate
            std::memcpy(cstring, s, n); // populate 
        } 
    }
 
    ~rule_of_five()
    {
        delete[] cstring;  // deallocate
    }
 
    rule_of_five(const rule_of_five& other) // copy constructor
    : rule_of_five(other.cstring)
    {}
 
    rule_of_five(rule_of_five&& other) noexcept // move constructor
    : cstring(std::exchange(other.cstring, nullptr))
    {}
 
    rule_of_five& operator=(const rule_of_five& other) // copy assignment
    {
         return *this = rule_of_five(other);
    }
 
    rule_of_five& operator=(rule_of_five&& other) noexcept // move assignment
    {
        std::swap(cstring, other.cstring);
        return *this;
    }
 
// alternatively, replace both assignment operators with 
//  rule_of_five& operator=(rule_of_five other) noexcept
//  {
//      std::swap(cstring, other.cstring);
//      return *this;
//  }
};
```

Unlike Rule of Three, failing to provide move constructor and move assignment is usually not an error, but a missed optimization opportunity.

### Rule of zero

Classes that have custom destructors, copy/move constructors or copy/move assignment operators should deal exclusively with ownership (which follows from the [Single Responsibility Principle](https://en.wikipedia.org/wiki/Single_responsibility_principle)). Other classes should not have custom destructors, copy/move constructors or copy/move assignment operators.[[1\]](https://en.cppreference.com/w/cpp/language/rule_of_three#cite_note-1)

This rule also appears in the C++ Core Guidelines as [C.20: If you can avoid defining default operations, do](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-zero).

```c++
class rule_of_zero
{
    std::string cppstring;
 public:
    rule_of_zero(const std::string& arg) : cppstring(arg) {}
};
```

> NOTE: 对于上面的这段胡，是需要进行分析的
>
> [Rule of Zero confusion?](https://stackoverflow.com/questions/44997955/rule-of-zero-confusion)
>
> **
> The Rule of Zero**
>
> The rule of zero is another rule of thumb about how to write classes that need to use some resources like memory or other objects. In the example the dynamically allocated memory containing the characters of the string is a resource that has to be managed.
>
> The recommendation is to let specialized classes manage resources, and do only that. In the example, std::string takes care of all the details of managing the allocated memory.
>
> 









When a base class is intended for polymorphic use, its destructor may have to be declared public and virtual. This blocks implicit moves (and deprecates implicit copies), and so the special member functions have to be declared as defaulted[[2\]](https://en.cppreference.com/w/cpp/language/rule_of_three#cite_note-2)

```c++
class base_of_five_defaults
{
 public:
    base_of_five_defaults(const base_of_five_defaults&) = default;
    base_of_five_defaults(base_of_five_defaults&&) = default;
    base_of_five_defaults& operator=(const base_of_five_defaults&) = default;
    base_of_five_defaults& operator=(base_of_five_defaults&&) = default;
    virtual ~base_of_five_defaults() = default;
};
```





## Classic example

### microsoft [Move Constructors and Move Assignment Operators (C++)](https://docs.microsoft.com/en-us/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=msvc-160)

1、没有使用copy-and-swap，无法实现strong  exception safety

```C++
// MemoryBlock.h
#pragma once
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

class MemoryBlock
{
public:

	// Simple constructor that initializes the resource.
	explicit MemoryBlock(size_t length) :
					_length(length), _data(new int[length])
	{
		std::cout << "In MemoryBlock(size_t). length = " << _length << "." << std::endl;
	}

	// Destructor.
	~MemoryBlock()
	{
		std::cout << "In ~MemoryBlock(). length = " << _length << ".";

		if (_data != nullptr)
		{
			std::cout << " Deleting resource.";
			// Delete the resource.
			delete[] _data;
		}

		std::cout << std::endl;
	}

	// Copy constructor.
	MemoryBlock(const MemoryBlock &other) :
					_length(other._length), _data(new int[other._length])
	{
		std::cout << "In MemoryBlock(const MemoryBlock&). length = " << other._length << ". Copying resource." << std::endl;

		std::copy(other._data, other._data + _length, _data);
	}

	// Copy assignment operator.
	MemoryBlock& operator=(const MemoryBlock &other)
	{
		std::cout << "In operator=(const MemoryBlock&). length = " << other._length << ". Copying resource." << std::endl;

		if (this != &other)
		{
			// Free the existing resource.
			delete[] _data;

			_length = other._length;
			_data = new int[_length];
			std::copy(other._data, other._data + _length, _data);
		}
		return *this;
	}

	// Retrieves the length of the data resource.
	size_t Length() const
	{
		return _length;
	}
	// Move constructor.
	MemoryBlock(MemoryBlock &&other) noexcept :
					_data(nullptr), _length(0)
	{
		std::cout << "In MemoryBlock(MemoryBlock&&). length = " << other._length << ". Moving resource." << std::endl;

		// Copy the data pointer and its length from the
		// source object.
		_data = other._data;
		_length = other._length;

		// Release the data pointer from the source object so that
		// the destructor does not free the memory multiple times.
		other._data = nullptr;
		other._length = 0;
	}

	// Move assignment operator.
	MemoryBlock& operator=(MemoryBlock &&other) noexcept
	{
		std::cout << "In operator=(MemoryBlock&&). length = " << other._length << "." << std::endl;

		if (this != &other)
		{
			// Free the existing resource.
			delete[] _data;

			// Copy the data pointer and its length from the
			// source object.
			_data = other._data;
			_length = other._length;

			// Release the data pointer from the source object so that
			// the destructor does not free the memory multiple times.
			other._data = nullptr;
			other._length = 0;
		}
		return *this;
	}
private:
	size_t _length; // The length of the resource.
	int *_data; // The resource.
};

int main()
{
	// Create a vector object and add a few elements to it.
	vector<MemoryBlock> v;
	v.push_back(MemoryBlock(25));
	v.push_back(MemoryBlock(75));

	// Insert a new element into the second position of the vector.
	v.insert(v.begin() + 1, MemoryBlock(50));
}
// g++ -Wall -pedantic test.cpp --std=c++11

```

### stackexchange [DeepPtr: a deep-copying unique_ptr wrapper in C++](https://codereview.stackexchange.com/questions/103744/deepptr-a-deep-copying-unique-ptr-wrapper-in-c)
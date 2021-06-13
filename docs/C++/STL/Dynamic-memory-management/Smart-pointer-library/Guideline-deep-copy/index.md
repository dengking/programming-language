# Deep copy smart pointer

本章讨论如何deep copy smart pointer 所指向的内容。

## 起因

在阅读 wikipedia [Opaque pointer](https://en.wikipedia.org/wiki/Opaque_pointer) 的 "C++ Example: **d-pointer pattern**" 时，其中给出的例子是典型的 "Copy constructor for a class with `unique_ptr`"，按照 stackoverflow [Copy constructor for a class with unique_ptr](https://stackoverflow.com/questions/16030081/copy-constructor-for-a-class-with-unique-ptr) 中的说法，显然这种情况下是需要deep copy的。

## 相关文章、章节

1、wikipedia [Opaque pointer](https://en.wikipedia.org/wiki/Opaque_pointer)

在这篇文章中，给出了非常好的样例，它收录于 `Opaque-data-type\Opaque-pointer` 章节。

2、stackoverflow [Copy constructor for a class with unique_ptr](https://stackoverflow.com/questions/16030081/copy-constructor-for-a-class-with-unique-ptr) 

在这篇文章中，给出了非常好的样例，它收录于 `Smart-pointers\unique_ptr\unique_ptr-as-member\Copy-constructor-for-a-class-with-unique_ptr` 章节。

3、`Guideline-strong-exception-safety\Prefer-use-object-generator\std-make_unique\Deep-copy` 

其中讨论了`std::make_unique` deep copy的内容。



## stackoverflow [How to implement deep copy feature in some smart pointer?](https://stackoverflow.com/questions/24334888/how-to-implement-deep-copy-feature-in-some-smart-pointer)

`unique_ptr` is quite useful. However, it is not copyable. If virutal clone (deep copy) methods are provided for its pointed class, I think it will become more useful. Is it necessary or any better way to implement it? Any similar smart pointer exist in some library? Here is a version

```cpp
template<class T>
class deep_ptr: private unique_ptr<T>
{
public:
    using unique_ptr<T>::operator *;
    using unique_ptr<T>::operator ->;
    using unique_ptr<T>::operator bool;
    using unique_ptr<T>::release;
    using unique_ptr<T>::reset;
    using unique_ptr<T>::get;

    // add (DEFAULT_CONSTRUCTOR)(MOVE_CONSTRUCTOR)(MOVE_ASSIGNMENT_METHOD) ...

    explicit deep_ptr(T* p) : unique_ptr(p) {}

    deep_ptr(deep_ptr const& r) : unique_ptr(r->clone()) {}

    deep_ptr& operator=(deep_ptrconst& r)
    { if (this != &r) reset(r->clone()); return *this; }
};
```

Juse feel it is very useful but never see similar things. ???

[A](https://stackoverflow.com/a/24335132)

Unless I am misunderstanding what you are looking for, if a class has a clone method, that should be sufficient to get what you are looking for.

Sample code:

```cpp
#include <iostream>
#include <memory>

struct A
{
	virtual ~A()
	{
	}
	virtual A* clone() = 0;
};

struct B: A
{
	B(int in = 0) :
					x(in)
	{
	}
	B(B const &copy) :
					x(copy.x)
	{
	}
	virtual ~B()
	{
		std::cout << "In B::~B()\n";
	}

	virtual A* clone()
	{
		return new B(*this);
	}
	int x;
};

int main()
{
	std::unique_ptr<A> p1(new B(10));
	std::unique_ptr<A> p2(p1->clone());
	return 0;
}
// g++ -std=c++11  -Wall -pedantic -pthread main.cpp && ./a.out

```

Output from running the above program:

```c++
In B::~B()
In B::~B()
```



## stackexchange [DeepPtr: a deep-copying unique_ptr wrapper in C++](https://codereview.stackexchange.com/questions/103744/deepptr-a-deep-copying-unique-ptr-wrapper-in-c)


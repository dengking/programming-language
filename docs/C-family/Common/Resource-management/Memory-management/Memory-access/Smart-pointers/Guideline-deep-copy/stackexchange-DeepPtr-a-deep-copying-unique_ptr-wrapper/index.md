# stackexchange [DeepPtr: a deep-copying unique_ptr wrapper in C++](https://codereview.stackexchange.com/questions/103744/deepptr-a-deep-copying-unique-ptr-wrapper-in-c)

When using the **pimple idiom** it is necessary to represent your "impl" as a forward declared pointer in the header file. This necessitates a lot of boilerplate code to implement the [rule of five](https://stackoverflow.com/q/4782757/2779792).

Instead, I want to wrap `std::unique_ptr` with a template class `DeepPtr` which automatically deep copies on copy construction, assignment, and "deep" swaps on move construction and move assignment. The resulting class could also be convenient for "value-like" objects that need to be passed by handle due to polymorphism.

> NOTE: 
>
> 1、这段描述的是作者的意图
>
> 2、不清楚的是: ""deep" swaps on move construction and move assignment"，它的含义是什么？显然需要结合下面的实现来看

Areas of concern are:

1、Correct use of the swap idiom (should the local swap function be `private`/`public` or done some other way)?

2、Const correctness of the dereferencing operator overloads

3、Signature of the assignment operator function (`const DeepPtr& other`) which uses this const ref only to copy to temp. This seems strange, but correct.

4、Any other pitfalls that could be caused by this class.

------

```cpp
#pragma once
#include <iostream>
#include <memory>

template<typename T>
class DeepPtr
{
public:
	DeepPtr() :
					myPtr(nullptr)
	{
	}
	/**
	 * @brief deep copy
	 *
	 * @param value
	 */
	DeepPtr(const T &value) :
					myPtr(new T { value })
	{
	}
	/**
	 * @brief deep copy
	 *
	 * @param other
	 */
	DeepPtr(const DeepPtr &other) :
					myPtr(nullptr)
	{
		if (other)
		{
			myPtr = std::unique_ptr<T> { new T { *other } };
		}
	}
	/**
	 * @brief 为什么move constructor还是使用的deep copy？
	 *
	 * @param other
	 */
	DeepPtr(DeepPtr &&other) :
					myPtr(nullptr)
	{
		if (other)
		{
			myPtr = std::unique_ptr<T> { new T { *other } };
		}
	}
	/**
	 * @brief 使用的copy and swap idiom；显然它还是deep copy
	 *
	 * @param other
	 * @return
	 */
	DeepPtr& operator=(const DeepPtr &other)
	{
		DeepPtr temp { other };
		swap(*this, temp);
		return *this;
	}
	/**
	 * @brief 没有deep copy
	 *
	 * @param other
	 * @return
	 */
	DeepPtr& operator=(DeepPtr &&other)
	{
		swap(*this, other);
		return *this;
	}

	static void swap(DeepPtr &left, DeepPtr &right)
	{
		std::swap(left.myPtr, right.myPtr);
	}

	T& operator*()
	{
		return *myPtr;
	}

	const T& operator*() const
	{
		return *myPtr;
	}

	T* const operator->()
	{
		return myPtr.operator->();
	}

	const T* const operator->() const
	{
		return myPtr.operator->();
	}

	const T* const get() const
	{
		return myPtr.get();
	}

	operator bool() const
	{
		return (bool) myPtr;
	}

private:
	std::unique_ptr<T> myPtr;
};

```



## [A](https://codereview.stackexchange.com/a/103792)

### Pragma(s)

Pragma once (like all pragmas) is not supported by all compilers.

```cpp
#pragma once
```

Prefer to use header guards.

You can generate a GUID or you can use the namespace and file name to make it unique.

### Namespace

You have not put your code into a namespace.

### Constructor nullptr

There are some situations where it is nice to be able to construct with an explicit `nullptr`.
[See here](http://lokiastari.com/blog/2015/01/23/c-plus-plus-by-example-smart-pointer-part-iii/)

```cpp
    DeepPtr() :myPtr( nullptr ) {}
```

So you could modify this constructor slightly.

```cpp
    DeepPtr(std::nullptr_t value = nullptr): myPtr(nullptr) {}
```

### Move Construction.

I though you said on move you wanted to move the object not copy it.

> NOTE: 上面这段话中的是"on move"的意思是"当move的时候"，因此上面这段话的意思是: 当move的时候，你想要move object而不是copy。
>
> 从下面的实现来看，作者是copy:

```cpp
    DeepPtr( DeepPtr&& other )
    :myPtr( nullptr )
    {
        if ( other )
        {
            myPtr = std::unique_ptr<T>{ new T{ *other } };
        }
    }
```

This does a copy on move construction. Also you should mark you move constructor as `noexcept`. This will give you several advantages when using the object with STL containers (the STL will default to copy even if move is available if the move is not not marked noexcept. This is to provide the strong exception guarantee).

Note: You should only mark it as noexcept if it will not throw. So you should not do that if you are going to copy only; if you change this to do an actual move then add `noexcept`.

> NOTE: 上面这两段话的核心观点是:
>
> 1、提问者的move constructor是采用的copy而不是move，因此它是会throw的
>
> 2、本回答的作者的建议是将move constructor改为move，如果改为move，则它是non-throw的，因此需要添加`noexcept` specifier

### Copy and Swap

This is technically correct.

```cpp
    DeepPtr& operator=( const DeepPtr& other )
    {
        DeepPtr temp{ other };
        swap( *this, temp );
        return *this;
    }
```

But you get a better optimization if you use passing by value to get your copy. Like this:

```cpp
    DeepPtr& operator=(DeepPtr other)  // Pass by value gets copy
    {
        swap( *this, other );
        return *this;
    }
```

See [What is the copy-and-swap idiom?](https://stackoverflow.com/q/3279543/14065)

### Move Assignment

Looks good. But like the move constructor, The move assignment should be marked as `noexcept`.

```cpp
    DeepPtr& operator=( DeepPtr&& other )
    {
        swap( *this, other );
        return *this;
    }
```

> NOTE: 
>
> 1、上述是典型的基于swap来实现move

### Swap

```cpp
    static void swap( DeepPtr& left, DeepPtr& right ) { std::swap( left.myPtr, right.myPtr ); }
```

Missed the `static` first time threw(第一次评审的时候遗漏了static). It is more normal to define a normal `swap` method. Then define a standalone `swap` function. Also like move operations the swap should be marked as `noexcept`.

```cpp
    void swap(DeepPtr& other) noexcept {
        using std::swap;
        swap(*this.myPtr, other.myPtr);

           // Adding `using std::swap`
           // Then calling swap (without std::)
           // Is the normal way of doing this. As it allows for Koenig
           // look up of a type specific swap first. If that does not
           // exist then the std::swap version will be used by the compiler.
    }
};
// outside the class
void swap(DeepPtr& lhs, DeepPtr& rhs) noexcept {
    lhs.swap(rhs);
}
```

Now that I have done swap. I would change the move operators too:

```cpp
    DeepPtr(DeepPtr&& other)    noexcept
        :myPtr( nullptr )
    {
        other.swap(*this);
    }
    DeepPtr& operator=( DeepPtr&& other ) noexcept
    {
        other.swap(*this);
        return *this;
    }
```

### Constness

```cpp
    const T* const get() const { return myPtr.get(); }
```

Sure but are you only ever going to want a const version of the pointer. Like the last two operations `operator*` and `operator->` you probably want a symmetric non const version.

### Bool cast

You should mark this `explicit`.

```cpp
    operator bool() const { return (bool)myPtr; }
```

See here https://codereview.stackexchange.com/a/93353/507

## [A](https://codereview.stackexchange.com/a/104023)

You already accepted Loki's answer (and he made good points) but there is one comment I wanted to add regarding this:

> Any other pitfalls that could be caused by this class.

Your class has no information on the concrete type held within, causing slicing in the presence of derived classes;

Consider this scenario:

```cpp
class Base { ... }; // not abstract (can be instantiated)
class Derived: public Base { ... };

auto ptr1 = DeepPtr<Base>(new Derived{});
auto ptr2 = ptr1;
```

The last line will populate `ptr2` with a `Base` instance that has all the sliced values from `ptr2`'s `Derived` instance (because `ptr2`'s constructor calls `new T`, which resolves to `new Base` - not `new Derived`).

> NOTE: 典型的object slicing: 将subclass object赋值给base class object

You have two possible solutions for this:

1、intrusive(入侵式的) cloning (your class hierarchy rooted in `Base` needs to implement a `virtual Base* clone() = 0` where each specialization of `Base` returns it's own instance); this cloning implementation would be then used for deep copy;

2、transparent cloning, based on template specialization of a factory function, by the derived type. I needed to implement something similar for the same reason (deep copy of polymorphic pointers). You can see [here](https://codereview.stackexchange.com/questions/54371/polymorphic-owned-reference-wrapper-for-class-hierarchies) my implementation.

See the use of `to_polymorphic` functions in my question, and how they instantiate the correct cloning function, transparently.
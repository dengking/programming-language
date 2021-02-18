# Copy constructor for a class with `unique_ptr`

对一个有`unique_ptr`类型成员变量的class，它的copy constructor要如何写？

## stackoverflow [Copy constructor for a class with unique_ptr](https://stackoverflow.com/questions/16030081/copy-constructor-for-a-class-with-unique-ptr)

How do I implement a copy constructor for a class that has a `unique_ptr` member variable? I am only considering C++11.

[A](https://stackoverflow.com/a/16030118)

Since the `unique_ptr` can not be shared, you need to either deep-copy its content or convert the `unique_ptr` to a `shared_ptr`.

> NOTE: 下面code，使用的是deep copy

```cpp
#include <memory>
class A
{
	std::unique_ptr<int> up_;

public:
	A(int i) :
					up_(new int(i))
	{
	}
	A(const A &a) :
					up_(new int(*a.up_))
	{
	}
};

int main()
{
	A a(42);
	A b = a;
}
// g++ --std=c++11 test.cpp

```

You can, as NPE mentioned, use a move-ctor instead of a copy-ctor but that would result in different semantics of your class. A move-ctor would need to make the member as moveable explicitly via `std::move`:

```cpp
A( A&& a ) : up_( std::move( a.up_ ) ) {}
```

Having a complete set of the necessary operators also leads to

```cpp
A& operator=( const A& a )
{
   up_.reset( new int( *a.up_ ) );
   return *this,
}

A& operator=( A&& a )
{
   up_ = std::move( a.up_ );
   return *this,
}
```

> NOTE: 完整如下:
>
> ```C++
> #include <memory>
> class A
> {
> 	std::unique_ptr<int> up_;
> 
> public:
> 	A(int i) :
> 					up_(new int(i))
> 	{
> 	}
> 	A(const A &a) :
> 					up_(new int(*a.up_))
> 	{
> 	}
> 	A& operator=(const A &a)
> 	{
> 		up_.reset(new int(*a.up_));
> 		return *this;
> 	}
> 
> 	A& operator=(A &&a)
> 	{
> 		up_ = std::move(a.up_);
> 		return *this;
> 	}
> };
> 
> int main()
> {
> 	A a(42);
> 	A b = a;
> }
> // g++ --std=c++11 test.cpp
> 
> ```
>
> 

If you want to use your class in a `std::vector`, you basically have to decide if the vector shall be the unique owner of an object, in which case it would be sufficient to make the class moveable, but not copyable. If you leave out the copy-ctor and copy-assignment, the compiler will guide your way on how to use a `std::vector` with move-only types.



## 使用`std::make_unique_ptr`

具体的例子参见 wikipedia [Opaque pointer](https://en.wikipedia.org/wiki/Opaque_pointer)，它他收录在 `C++\Idiom\OOP\Pointer-to-implementation-PImpl\Opaque-data-type\Opaque-pointer` 章节中。
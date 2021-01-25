# Copy constructor for a class with `unique_ptr`



## stackoverflow [Copy constructor for a class with unique_ptr](https://stackoverflow.com/questions/16030081/copy-constructor-for-a-class-with-unique-ptr)

How do I implement a copy constructor for a class that has a `unique_ptr` member variable? I am only considering C++11.

[A](https://stackoverflow.com/a/16030118)

Since the `unique_ptr` can not be shared, you need to either deep-copy its content or convert the `unique_ptr` to a `shared_ptr`.

```cpp
class A
{
   std::unique_ptr< int > up_;

public:
   A( int i ) : up_( new int( i ) ) {}
   A( const A& a ) : up_( new int( *a.up_ ) ) {}
};

int main()
{
   A a( 42 );
   A b = a;
}
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

If you want to use your class in a `std::vector`, you basically have to decide if the vector shall be the unique owner of an object, in which case it would be sufficient to make the class moveable, but not copyable. If you leave out the copy-ctor and copy-assignment, the compiler will guide your way on how to use a std::vector with move-only types.
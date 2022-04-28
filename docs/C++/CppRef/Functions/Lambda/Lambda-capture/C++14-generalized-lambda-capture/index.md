# C++14 generalized lambda capture

这是在bing "c++ move into lambda" 时，发现的

## stackoverflow [Move capture in lambda](https://stackoverflow.com/questions/8640393/move-capture-in-lambda)

How do I capture by move (also known as rvalue reference) in a C++11 lambda?

I am trying to write something like this:

```cpp
std::unique_ptr<int> myPointer(new int);

std::function<void(void)> example = [std::move(myPointer)]{
   *myPointer = 4;
};
```

### [A](https://stackoverflow.com/a/20669290)



#### Generalized lambda capture in C++14

In C++14 we will have the so called [generalized lambda capture](http://en.wikipedia.org/wiki/C%2B%2B14#Lambda_capture_expressions). This enables move capture. The following will be legal code in C++14:

```cpp
using namespace std;

// a unique_ptr is move-only
auto u = make_unique<some_type>( some, parameters );  

// move the unique_ptr into the lambda
go.run( [ u = move(u) ] { do_something_with( u ); } ); 
```

But it is much more general in the sense that captured variables can be initialized with anything like so:

```cpp
auto lambda = [value = 0] mutable { return ++value; };
```

In C++11 this is not possible yet, but with some tricks that involve helper types. Fortunately, the Clang 3.4 compiler already implements this awesome feature. The compiler will be released December 2013 or January 2014, if the [recent release pace](http://llvm.org/releases/) will be kept.

**UPDATE:** The [Clang 3.4 compiler](http://llvm.org/releases/3.4/tools/clang/docs/ReleaseNotes.html) was released on 6 Jan 2014 with the said feature.

#### A workaround for move capture

Here's an implementation of a helper function `make_rref` which helps with artificial move capture

```cpp
#include <cassert>
#include <memory>
#include <utility>

template <typename T>
struct rref_impl
{
    rref_impl() = delete;
    rref_impl( T && x ) : x{std::move(x)} {}
    rref_impl( rref_impl & other )
        : x{std::move(other.x)}, isCopied{true}
    {
        assert( other.isCopied == false );
    }
    rref_impl( rref_impl && other )
        : x{std::move(other.x)}, isCopied{std::move(other.isCopied)}
    {
    }
    rref_impl & operator=( rref_impl other ) = delete;
    T && move()
    {
        return std::move(x);
    }

private:
    T x;
    bool isCopied = false;
};

template<typename T> rref_impl<T> make_rref( T && x )
{
    return rref_impl<T>{ std::move(x) };
}
```

And here's a test case for that function that ran successfully on my gcc 4.7.3.

```cpp
int main()
{
    std::unique_ptr<int> p{new int(0)};
    auto rref = make_rref( std::move(p) );
    auto lambda =
        [rref]() mutable -> std::unique_ptr<int> { return rref.move(); };
    assert(  lambda() );
    assert( !lambda() );
}
```

The drawback here is that `lambda` is copyable and when copied the assertion in the copy constructor of `rref_impl` fails leading to a runtime bug. The following might be a better and even more generic solution because the compiler will catch the error.

#### Emulating generalized lambda capture in C++11

Here's one more idea, on how to implement generalized lambda capture. The use of the function `capture()` (whose implementation is found further down) is as follows:

```cpp
#include <cassert>
#include <memory>

int main()
{
    std::unique_ptr<int> p{new int(0)};
    auto lambda = capture( std::move(p),
        []( std::unique_ptr<int> & p ) { return std::move(p); } );
    assert(  lambda() );
    assert( !lambda() );
}
```

Here `lambda` is a functor object (almost a real lambda) which has captured `std::move(p)` as it is passed to `capture()`. The second argument of `capture` is a lambda which takes the captured variable as an argument. When `lambda` is used as a function object, then all arguments that are passed to it will be forwarded to the internal lambda as arguments after the captured variable. (In our case there are no further arguments to be forwarded). Essentially, the same as in the previous solution happens. Here's how `capture` is implemented:

```cpp
#include <utility>

template <typename T, typename F>
class capture_impl
{
    T x;
    F f;
public:
    capture_impl( T && x, F && f )
        : x{std::forward<T>(x)}, f{std::forward<F>(f)}
    {}

    template <typename ...Ts> auto operator()( Ts&&...args )
        -> decltype(f( x, std::forward<Ts>(args)... ))
    {
        return f( x, std::forward<Ts>(args)... );
    }

    template <typename ...Ts> auto operator()( Ts&&...args ) const
        -> decltype(f( x, std::forward<Ts>(args)... ))
    {
        return f( x, std::forward<Ts>(args)... );
    }
};

template <typename T, typename F>
capture_impl<T,F> capture( T && x, F && f )
{
    return capture_impl<T,F>(
        std::forward<T>(x), std::forward<F>(f) );
}
```

This second solution is also cleaner, because it disables copying the lambda, if the captured type is not copyable. In the first solution that can only be checked at runtime with an `assert()`.
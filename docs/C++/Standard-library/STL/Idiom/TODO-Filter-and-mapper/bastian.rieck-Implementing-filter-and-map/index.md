# bastian.rieck [Implementing filter and map with C++11](https://bastian.rieck.me/blog/posts/2016/filter_map_cxx11/)

I am a fan of [“higher-order functions”](https://en.wikipedia.org/wiki/Higher-order_function) such as `filter` and `map`. To me, they are much more elegant and concise than the corresponding constructions with `for`-loops and the like. Of course, I wanted to use them in my daily C++ grind as well. Until recently, this turned out to be rather clunky—C++98 and C++03 did not include `std::copy_if`, which can be used to implement `filter`. Furthermore, `std::transform`, which is the equivalent of `apply` or `map`, takes an output iterator as an additional argument. This does not yield concise code.

> NOTE:
>
> 1、典型的functional programming

Luckily, with C++11 the situation became better. First, let’s implement `filter`:

```cpp
#include <algorithm>
#include <iterator>
#include <vector>

template <
  class InputIterator,
  class Functor>
std::vector<typename std::iterator_traits<InputIterator>::value_type>
filter( InputIterator begin, InputIterator end, Functor f )
{
  using ValueType = typename std::iterator_traits<InputIterator>::value_type;

  std::vector<ValueType> result;
  result.reserve( std::distance( begin, end ) );

  std::copy_if( begin, end,
                std::back_inserter( result ),
                f );

  return result;
}
```

> NOTE: 
>
> 1、上述实现方式是不好的，一般应该按照STL的模式，输出应该使用output iterator

We can use the new implementation in the following manner:

```cpp
std::vector<unsigned> a = { 2,3,4,5,6 };
auto b = filter( a.begin(), a.end(),
                 [] ( unsigned i ) { return i % 2 == 0; } );
```

That is pretty concise in my opinion.

Let’s implement `map` next:

```cpp
#include <algorithm>
#include <iterator>
#include <type_traits>
#include <vector>

template <
  class InputIterator,
  class Functor>
auto map( InputIterator begin, InputIterator end, Functor f )
  -> std::vector<typename std::result_of< Functor( typename std::iterator_traits<InputIterator>::value_type ) >::type>
{
  using FunctorValueType = typename std::result_of< Functor( typename std::iterator_traits<InputIterator>::value_type ) >::type;

  std::vector<FunctorValueType> functorValues;
  functorValues.reserve( unsigned( std::distance( begin, end ) ) );

  std::transform( begin, end,
                  std::back_inserter( functorValues ), f );

  return functorValues;
}
```

That contains two awesome C++11 features: Trailing return types and `std::result_of`. We need this because the value type of the returned vector depends on the result of applying the functor to one of the values in the range! To be honest, the trailing return type is not really necessary, but I always wanted to use it somewhere. Again, I think that this implementation results in quite concise code:

```cpp
std::vector<unsigned> a = { 2,3,4,5,6 };
auto b = map( a.begin(), a.end(),
              [] ( unsigned i ) { return i * 2; } );
```
# [Boost range](https://www.boost.org/doc/libs/1_73_0/libs/range/doc/html/index.html)

Therefore we would like to raise the abstraction level for algorithms so they specify their interface in terms of [Ranges](https://www.boost.org/doc/libs/1_73_0/libs/range/doc/html/range/concepts.html) as much as possible.

> NOTE: range其实是一个abstraction layer，或者说是***layer of indirection***，或者说是adapter。



### [Overview](https://www.boost.org/doc/libs/1_73_0/libs/range/doc/html/range/concepts/overview.html)

A Range is a ***concept*** similar to the STL [Container](http://www.sgi.com/tech/stl/Container.html) concept. 

The motivation for the Range concept is that there are many useful Container-like types that do not meet the full requirements of Container, and many algorithms that can be written with this reduced set of requirements. In particular, a Range does not necessarily

- own the elements that can be accessed through it,
- have copy semantics,

> NOTE: `range`是对container概念的推广

The operations that can be performed on a Range is dependent on the [traversal category](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/new-iter-concepts.html#iterator-traversal-concepts-lib-iterator-traversal) of the underlying **iterator** type. Therefore the range concepts are named to reflect which traversal category its iterators support. See also terminology and style guidelines. for more information about naming of ranges.

> NOTE: 上面这段话揭示了`range`和`iterator`之间的关系，理解上面这段话是快速理解后面内容的关键所在。

### [Single Pass Range](https://www.boost.org/doc/libs/1_73_0/libs/range/doc/html/range/concepts/single_pass_range.html)

> NOTE: 对应的iterator是[Single Pass Iterator](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/new-iter-concepts.html#singls-pass-iterators-lib-single-pass-iterators).

### [Forward Range](https://www.boost.org/doc/libs/1_73_0/libs/range/doc/html/range/concepts/forward_range.html)

> NOTE: 对应的iterator是[Forward Traversal Iterator](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/new-iter-concepts.html#forward-traversal-iterators-lib-forward-traversal-iterators).

### [Bidirectional Range](https://www.boost.org/doc/libs/1_73_0/libs/range/doc/html/range/concepts/bidirectional_range.html)

> NOTE: 对应的iterator是[Bidirectional Range](https://www.boost.org/doc/libs/1_73_0/libs/range/doc/html/range/concepts/bidirectional_range.html).

### [Random Access Range](https://www.boost.org/doc/libs/1_73_0/libs/range/doc/html/range/concepts/random_access_range.html)

对应的iterator是[Random Access Traversal Iterator](https://www.boost.org/doc/libs/1_73_0/libs/iterator/doc/new-iter-concepts.html#random-access-traversal-iterators-lib-random-access-traversal-iterators).

### [Concept Checking](https://www.boost.org/doc/libs/1_73_0/libs/range/doc/html/range/concepts/concept_checking.html)

> NOTE: concept checking其实就是就是c++20的concept特性。

## [Reference](https://www.boost.org/doc/libs/1_73_0/libs/range/doc/html/range/reference.html)

### [Overview](https://www.boost.org/doc/libs/1_73_0/libs/range/doc/html/range/reference/overview.html)

#### [Synopsis](https://www.boost.org/doc/libs/1_73_0/libs/range/doc/html/range/reference/concept_implementation/synopsis.html)



```c++
namespace boost
{
    //
    // Single Pass Range metafunctions
    //

    template< class T, class Enabler=void >
    struct range_iterator;

    template< class T >
    struct range_value;

    template< class T >
    struct range_reference;

    template< class T >
    struct range_pointer;

    template< class T >
    struct range_category;

    //
    // Forward Range metafunctions
    //

    template< class T >
    struct range_difference;

    //
    // Bidirectional Range metafunctions
    //

    template< class T >
    struct range_reverse_iterator;

    //
    // Single Pass Range functions
    //

    template< class T >
    typename range_iterator<T>::type
    begin( T& r );

    template< class T >
    typename range_iterator<const T>::type
    begin( const T& r );

    template< class T >
    typename range_iterator<T>::type
    end( T& r );

    template< class T >
    typename range_iterator<const T>::type
    end( const T& r );

    template< class T >
    bool
    empty( const T& r );

    //
    // Forward Range functions
    //

    template< class T >
    typename range_difference<T>::type
    distance( const T& r );

    template< class T >
    typename range_size<T>::type
    size( const T& r );

    //
    // Bidirectional Range functions
    //

    template< class T >
    typename range_reverse_iterator<T>::type
    rbegin( T& r );

    template< class T >
    typename range_reverse_iterator<const T>::type
    rbegin( const T& r );

    template< class T >
    typename range_reverse_iterator<T>::type
    rend( T& r );

    template< class T >
    typename range_reverse_iterator<const T>::type
    rend( const T& r );

    //
    // Special const Range functions
    //

    template< class T >
    typename range_iterator<const T>::type
    const_begin( const T& r );

    template< class T >
    typename range_iterator<const T>::type
    const_end( const T& r );

    template< class T >
    typename range_reverse_iterator<const T>::type
    const_rbegin( const T& r );

    template< class T >
    typename range_reverse_iterator<const T>::type
    const_rend( const T& r );

    //
    // String utilities
    //

    template< class T >
    iterator_range< ... see below ... >
    as_literal( T& r );

    template< class T >
    iterator_range< ... see below ... >
    as_literal( const T& r );

    template< class T >
    iterator_range< typename range_iterator<T>::type >
    as_array( T& r );

    template< class T >
    iterator_range< typename range_iterator<const T>::type >
    as_array( const T& r );

} // namespace 'boost'
```

#### [Semantics](https://www.boost.org/doc/libs/1_73_0/libs/range/doc/html/range/reference/concept_implementation/semantics.html)



##### [Metafunctions](https://www.boost.org/doc/libs/1_73_0/libs/range/doc/html/range/reference/concept_implementation/semantics/metafunctions.html)
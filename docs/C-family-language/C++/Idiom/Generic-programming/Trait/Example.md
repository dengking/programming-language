# Example

## Trait type

所谓trait type，指的是获得type。

### boost [graph](https://github.com/boostorg/graph)/[include](https://github.com/boostorg/graph/tree/develop/include)/[boost](https://github.com/boostorg/graph/tree/develop/include/boost)/[graph](https://github.com/boostorg/graph/tree/develop/include/boost/graph)/`adjacency_list.hpp`

> //===========================================================================
> // Selectors for the VertexList and EdgeList template parameters of
> // adjacency_list, and the container_gen traits class which is used
> // to map the selectors to the container type used to implement the
> // graph.

selector and trait。

```c++
struct vecS
{
};
struct listS
{
};
struct setS
{
};

template < class Selector, class ValueType > struct container_gen
{
};

template < class ValueType > struct container_gen< listS, ValueType >
{
    typedef std::list< ValueType > type;
};

template < class ValueType > struct container_gen< vecS, ValueType >
{
    typedef std::vector< ValueType > type;
};

template < class ValueType > struct container_gen< mapS, ValueType >
{
    typedef std::set< ValueType > type;
};

```

显然，在primary template中，没有提供`type`，只在specialization中提供了`type`。



## Trait function



### [libstdc++-v3](https://code.woboq.org/gcc/libstdc++-v3/)/[include](https://code.woboq.org/gcc/libstdc++-v3/include/)/[std](https://code.woboq.org/gcc/libstdc++-v3/include/std/)/[limits](https://code.woboq.org/gcc/libstdc++-v3/include/std/limits.html)



## Trait variable

这种是最最简单的。

[What is a Type Trait?](https://blog.galowicz.de/2016/02/18/what_is_a_type_trait/)
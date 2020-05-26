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

```c++
  /**
   *  @brief Properties of fundamental types.
   *
   *  This class allows a program to obtain information about the
   *  representation of a fundamental type on a given platform.  For
   *  non-fundamental types, the functions will return 0 and the data
   *  members will all be @c false.
  */
  template<typename _Tp>
    struct numeric_limits : public __numeric_limits_base
```

注释中，对default value的说明。

## Trait variable

这种是最最简单的，即定义有成员变量来说明含义。

### [What is a Type Trait?](https://blog.galowicz.de/2016/02/18/what_is_a_type_trait/)

#### not

```c++
// (A)
template <bool X>
struct not
{
    static constexpr bool value {false};
};

// (B)
template <>
struct not<false>
{
    static constexpr bool value {true};
};
```

#### Comparing types

```c++
// (A)
template <typename T, typename U>
struct is_same_type
{
    static constexpr bool value {false};
};

// (B)
template <typename T>
struct is_same_type<T, T>
{
    static constexpr bool value {true};
};

template <typename T>
T myfunc(T x)
{
    if (is_same_type<T, FooType>::value) {
        /* do something which is completely FooType specific */
    } else {
        /* do the general thing */
    }
}
```

```c++
template <typename T>
T myfunc(T x)
{ /* do the general thing */ }

template <>
FooType myfunc(FooType x)
{ /* do something which is completely FooType specific */ }
```

#### Determining if `T` is a pointer type

```c++
template <typename T>
struct is_pointer
{
    static constexpr bool value {false};
};

template <typename T>
struct is_pointer<T*>
{
    static constexpr bool value {true};
};
```
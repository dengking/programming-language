# Iterator Facade

## abstract

`iterator_facade` is a base class template that implements the interface of standard iterators in terms of a few core functions and associated types, to be supplied by a derived iterator class.

> NOTE: 
>
> 1、iterator facade让我想起了[Facade pattern](https://en.wikipedia.org/wiki/Facade_pattern)，它其实就是采用的Facade pattern

## Overview

While the iterator interface is rich, there is a core subset of the interface that is necessary for all the
functionality. We have identified the following core behaviors for iterators:

- dereferencing

- incrementing

- decrementing

- equality comparison

- random-access motion

- distance measurement

> NOTE: 在`Theory\Programming-paradigm\Generic-programming\Templates-and-Duck-Typing\Templates-and-Duck-Typing.md`中同样使用了behavior这个词语，按照其中的总结的思想，不同类型的iterator的behavior是不同的

In addition to the behaviors listed above, the **core interface elements** include the associated types exposed through iterator traits: `value_type`, `reference`, `difference_type`, and `iterator_category`.

> NOTE: 上面这段话中的core interface element所指的是，Iterator facade的需要由子类进行override的element，显然，上述的behavior的实现是需要由子类来实现的，这种下面有描述



Iterator facade uses the Curiously Recurring Template Pattern (CRTP) [Cop95] so that the user can specify the behavior of `iterator_facade` in a derived class. Former designs used policy objects to specify the behavior, but that approach was discarded for several reasons:

1. the creation and eventual copying of the policy object may create overhead that can be avoided with the current approach.
2. The policy object approach does not allow for custom constructors on the created iterator types, an essential feature if `iterator_facade` should be used in other library implementations.
3. Without the use of CRTP, the standard requirement that an iterator’s `operator++` returns the **iterator type** itself would mean that all iterators built with the library would have to be specializations of `iterator_facade<...>`, rather than something more descriptive like `indirect_iterator<T*>`. Cumbersome **type generator metafunctions** would be needed to build new parameterized iterators, and a separate `iterator_adaptor` layer would be impossible.

> NOTE: 没有搞懂

## Usage



### Iterator Core Access

> NOTE: 本节标题所对应的是`iterator_core_access`



### `operator[]`

The indexing operator for a generalized iterator presents special challenges. A random access iterator’s `operator[]` is only required to return something convertible to its `value_type`. Requiring that it return an lvalue would rule out currently-legal random-access iterators which hold the referenced value in a data member (e.g. counting_iterator), because `*(p+n)` is a reference into the temporary iterator `p+n`, which is destroyed when `operator[]` returns.

> NOTE: 上面这段话所讨论的是`operator[]`返回值类型的问题

Writable iterators built with `iterator_facade` implement the semantics required by the preferred resolution to issue 299 and adopted by proposal n1550: the result of `p[n]` is an object convertible to the iterator’s `value_type`, and `p[n] = x` is equivalent to `*(p + n) = x` (Note: This result object may be implemented as a proxy containing a copy of `p+n`). This approach will work properly for any random-access iterator regardless of the other details of its implementation. A user who knows more about the implementation of her iterator is free to implement an `operator[]` that returns an lvalue in the derived iterator class; it will hide the one supplied by `iterator_facade` from clients of her iterator.



### `operator->`

The reference type of a **readable iterator** (and today’s input iterator) need not in fact be a reference, so long as it is convertible to the iterator’s `value_type`. When the value_type is a class, however, it must still be possible to access members through `operator->`. Therefore, an iterator whose reference



## Reference



```c++
// Comparison operators
template <class Dr1, class V1, class TC1, class R1, class D1,
class Dr2, class V2, class TC2, class R2, class D2>
typename enable_if_interoperable<Dr1,Dr2,bool>::type // exposition
operator ==(iterator_facade<Dr1,V1,TC1,R1,D1> const& lhs,
iterator_facade<Dr2,V2,TC2,R2,D2> const& rhs);
```



The `enable_if_interoperable` template used above is for exposition（展示） purposes. The member op-
erators should only be in an overload set provided the derived types `Dr1` and `Dr2` are interoperable,
meaning that at least one of the types is convertible to the other. The `enable_if_interoperable` approach uses SFINAE to take the operators out of the overload set when the types are not interoperable.
The operators should behave as-if `enable_if_interoperable` were defined to be:

```c++
template<bool, typename >
struct enable_if_interoperable_impl {
};
template<typename T> struct enable_if_interoperable_impl<true, T> {
	typedef T type;
};
template<typename Dr1, typename Dr2, typename T>
struct enable_if_interoperable: enable_if_interoperable_impl<
		std::is_convertible<Dr1, Dr2>::value
				|| std::is_convertible<Dr2, Dr1>::value, T> {
};
```

> NOTE: 上面是我根据原文的内容整理的

### `iterator_facade` Requirements

In the table below, `F` is `iterator_facade<X,V,C,R,D>`, `a` is an object of type `X`, `b` and `c` are objects of type `const X`, `n` is an object of `F::difference_type`, `y` is a constant object of a single pass iterator type
interoperable with `X`, and `z` is a constant object of a random access traversal iterator type interoperable
with `X`.



## Implementation

实现的源代码：`iterator_facade.hpp`

### `enable_if`

大量运用`enable_if`

`enable_if_interoperable`、`enable_if_interoperable_and_random_access_traversal`、

### 模板参数`CategoryOrTraversal`

category指的是标准库的iterator category，traversal指的是boost iterator library的，在`iterator_facade.html#categoryortraversal`中对此进行了说明。

### proxy

首先需要搞清楚proxy的功能是什么：proxy其实需要提供它所代理的object相同的behavior，它应该将这些behavior forward到它的底层的object。

下面是实现中，用到了proxy的

#### postfix increment proxy

这个proxy仅仅提供了`*`支持，所以它允许`(*r++)`。

iterators whose **dereference operators** reference the same value for all iterators into the same sequence (like many input iterators) need help with their postfix `++`: the **referenced value** must be read and stored away before the increment occurs so that `*a++` yields the originally referenced element and not the next one.

```C++
struct postfix_increment_proxy
{
    
};
```

#### `writable_postfix_increment_proxy`

顾名思义，它提供了write behavior。

```c++
struct writable_postfix_increment_proxy
{

};
```






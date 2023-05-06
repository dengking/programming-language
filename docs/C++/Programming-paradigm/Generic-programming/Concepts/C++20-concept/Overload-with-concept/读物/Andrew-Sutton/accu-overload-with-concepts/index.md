# accu [OVERLOADING WITH CONCEPTS](https://accu.org/journals/overload/24/136/sutton_2316/)

This is the third, and long overdue, article in my series on C++ concepts. The first two articles focused on how concepts are used to constrain generic algorithms [ [Sutton15 ](https://accu.org/journals/overload/24/136/sutton_2316/#[Sutton15])] and how concepts are defined [ [Sutton16 ](https://accu.org/journals/overload/24/136/sutton_2316/#[Sutton16])]. This article describes a sometimes overlooked, frequently misunderstood, and yet extraordinarily powerful feature of concepts: their role in function overloading. Concepts are useful for more than just improving error messages and precise specification of interfaces. They also increase expressiveness. Concepts can be used to shorten code, make it more generic, and increase performance.



Finally, since concepts was not accepted into C++17, I have seen an increase in online content promoting concept emulation techniques over language support and even claims that expression SFINAE, constexpr if, `static_assert `, and clever metaprogramming techniques are sufficient for our needs. That’s analogous to claiming that given `if `and `goto `, we don’t need `while `, `for `, and range- `for `. Yes, it’s logically correct, but in both cases we drag down the level of abstraction to specifying how things are to be done, rather than what should be done. The result is more work for the programmer, more bugs, and fewer optimization opportunities. C++ is not meant to be just an assembly language for template metaprogramming. Concepts allows us to raise the level of programming and simplify the code, without adding run-time overhead.



## Recap

In my previous articles [ [Sutton15 ](https://accu.org/journals/overload/24/136/sutton_2316/#[Sutton15]), [Sutton16 ](https://accu.org/journals/overload/24/136/sutton_2316/#[Sutton16])], I discussed a simple generic algorithm, `in() `, which determines whether an element can be found in a range of iterators. Here is an alternative version of the `in() `algorithm from the previous article. I’ve modified its constraints for the purpose of this article and also updated it to match some current naming trends in the C++ Standard Library (see Listing 1).

```c++
template<Sequence S, Equality_comparable T>
  requires Same_as<T, value_type_t<S>>
bool in(const S& seq, const T& value) {
  for (const auto& x : range)
    if (x == value)
      return true;
  return false;
}
```



This rendition of `in() `takes a sequence instead of a range as its first argument, and an equality comparable value for its second. The algorithm has three constraints:

1、the type of the `seq `must be a `Sequence `,

2、the type of `value `must be `Equality_comparable `, and

3、the `value `type must be the same as the element type of `seq `.

Here, `value_type_t `is a type alias that refers to the declared or deduced value type of `R `. The definitions of the `Sequence `and `Range `concepts needed for this algorithm look like Listing 2.

```c++
template<typename R>
concept bool Range() {
  return requires (R range) {
    typename value_type_t<R>;
    typename iterator_t<R>;
    { begin(range) } -> iterator_t<R>;
    { end(range) } -> iterator_t<R>;
    requires Input_iterator<iterator_t<R>>();
    requires Same_as<value_type_t<R>,
      value_type_t<iterator_t<R>>>();
  };
}

template<typename S>
concept bool Sequence() {
  return Range<R>() && requires (S seq) {
    { seq.front() } -> const value_type<S>&;
    { seq.back() } -> const value_type<S>&;
  };
}
	
```

> NOTE:
>
> 一、concept类似于一个函数

This specification requires all `Range `s to have:

1、two associated types named by `value_type_t `and `iterator_t`

2、two valid operations `begin() `and `end() `that return input iterators,

3、and that the value type of the range match that of the iterator.

Most `Sequence `s have the operations `front() `and `back() `, which return the first and last elements of the range. This isn’t a fully developed specification of a sequence, but it is sufficient for the discussion in this paper.

This seems reasonable. We can use the algorithm to determine if an element is contained within any sequence. Unfortunately, it no longer works for some collections:

```c++
  std::set<int> answers { ... };
  if (in(answers, 42)) // error: no front()
                       // or back()
  ...
```

This is unfortunate. We should clearly be able to determine if a key is contained within a set. But how do we do this?

## Extending algorithms

For someone who knows concepts and the standard library, the solution in this case is obvious: just add another overload that accepts associative containers.

```c++
  template<Associative_container A,
    Same_as<key_type_t<T>> T>
  bool in(const A& assoc, const T& value) {
    return assoc.find(value) != s.end();
  }
```

This version of `in() `has only two constraints: `A `must be an `Associative_container `, and `T `must be the same as key type of `A `( `key_type_t<A> `). For associative containers, we simply look up `value `using `find() `and then see if we found it by comparing to `end() `. That’s likely to be much faster than a sequential search.

Note that, unlike the `Sequence `version, `T `is not required to be equality comparable. This is because the precise requirements of `T `are determined by the associative container, and those requirements are usually determined by a separate comparator or hash function.

The concept `Associative_container `is defined like Listing 3.

```c++
template<typename S>
concept bool Associative_container() {
  return Regular<S> && Range<S>() && 
    requires {
      typename key_type_t<S>;
      requires Object_type<key_type_t<S>>;
    } &&
    requires (S s, key_type_t<S> k) {
      { s.empty() } -> bool;
      { s.size() } -> int;
      { s.find(k) } -> iterator_t<S>;
      { s.count(k) } -> int;
    };
}
```

That is, an associative container is `Regular `, defines a `Range `of elements, has a `key_type `(which may differ from the `value_type `), and a set of operations including `find() `, etc.



As with `Sequence `before, this is clearly not an exhaustive list of requirements for an associative container. It doesn’t address insertion and removal, and excludes specific requirements for `const `iterators. Moreover, we haven’t really described how we expect `size() `, `empty() `, `find() `and `count() `to behave. For now, we’ll just rely on our existing knowledge of the Standard Library.

This concept includes all of the associative containers in the C++ Standard Library ( `set `, `map `, `unordered_multiset `, etc.). It also includes non-standard-library associative containers, assuming that they expose this interface. For example, this overload would work for all of Qt’s associative containers ( `QSet<T> `, `QHash<T> `, etc.) [ [Qt ](https://accu.org/journals/overload/24/136/sutton_2316/#[Qt])].

To use concepts to extend algorithms, we need to understand how the compiler can tell a plain `Sequence `from an `Associative_container `. In other words, what happens when we call `in() `?

```c++
  std::vector<int> v { ... };
  std::set<int> s { ... };

  if (in(v, 42)) // Calls the `Sequence` overload
    std::cout << "found the answer...";
  if (in(s, 42)) // Calls the
                // `Associative_container` overload
  std::cout << "found the answer...";
```

For each call to `in() `, the compiler determines which function is called based on the arguments given. This is called *overload resolution* . This is an algorithm that attempts to find a single best function (amongst one or more candidates) to call based on the arguments given.

Both calls of `in() `refer to templates so the compiler performs template argument deduction and then form function declaration specializations based on the results. In both cases, deduction and substitution succeed in the usual and predictable way, so we have to choose amongst two specializations at each call site. This is where the constraints enter into the equation. Only functions whose constraints are satisfied can be selected by overload resolution.

In order to determine if a function’s constraints are satisfied, we substitute the deduced template arguments into the associated constraints of the function’s template declaration, and then we evaluate the resulting expression. The constraints are satisfied when substitution succeeds, and the expression evaluates to `true `.

In the first call to `in() `, the deduced template arguments are `std::vector<int> `and `int `. These arguments satisfy the constraints of `Sequence `but not those of the `Associative_container `because a `std::vector `does not have `find() `or `count() `. Therefore, the `Associative_container `candidate is rejected, leaving only the `Sequence `candidate.

In the second call to `in() `, the deduced arguments are `std::set<int> `and `int `. The resolution is the opposite of the one before: a `std::set `is never a `Sequence `because it lacks `front() `and `back() `, so that candidate is rejected, and overload resolution selects the `Associative_container `candidate.

In this cases the resolution is straightforward, and many readers will readily recognize the similarity to the `enable_if `technique used today. This works because the constraints on both overloads are sufficiently exclusive to ensure that a container satisfies the constraints of one template or the other, but not both.

> NOTE:
>
> 一、SFNIAE

The situation gets a bit more interesting if we want to add more overloads of this algorithm. We could extend the algorithm for specific types or templates like we might have done without concepts. Essentially, we could enumerate the valid definitions of `in() `for those types. For example, extending `in() `for WinRT’s `Map `class [ [WinRT ](https://accu.org/journals/overload/24/136/sutton_2316/#[WinRt])] might require a declaration like this:

```c++
  template<typename K, typename V, typename C>
  bool in(const Platform::Collections
                ::Map<K, V, C>& map, const K& k) {
    return map.HasKey(k);
  }
```

When there are many such viable definitions, this quickly becomes tedious and unmanageable. For any data structure that might represent a set of keys, we need a new overload. This simply does not scale.

If we’re lucky, many of those new enumerated overloads will have identical definitions. That would certainly be the case for WinRT’s `Map `and `MapView `classes; both would return `map.HasKey(k) `. In that case, we can unify their definitions into a single, more general template with appropriate constraints. For example:

```c++
  template<WinRtMap M>
  bool in(const M& map, const key_type_t<M>& k) {
    return map.HasKey(k);
  }
```

Here, `WinRtMap `would be a concept requiring members common to both `Map `and `MapView `. This would also accept any other map implementations that the library accrues over time, assuming they satisfied the `WinRtMap `constraints.

In general, we can continue extending the definition of a generic algorithm by adding overloads that differ only in their constraints. There are exactly three cases that we need to consider when overloading with concepts:

1、Extend a definition by providing an overload that works for a completely different set of types. The constraints of these new overloads would either be mutually exclusive or have some minimal amount of overlap with existing constraints.

2、Provide an optimized version of an existing overload by specializing it for a subset of its arguments. This entails creating a new overload that has stronger constraints than it’s more general form.

3、Provide a generalized version that is defined in terms of constraints shared by one or more existing overloads.
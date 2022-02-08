# `std::hash`

## cppreference [std::hash](https://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/utility/hash.html)



```C++
template< class Key >
struct hash; // not defined
```

> NOTE: 
>
> 可以看到，它是class template，扩展的方式是通过full specialization

The hash template defines a function object that implements a [hash function](https://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/utility/http%3A//en.wikipedia.com/wiki/Hash_function). Instances of this function object satisfy [`Hash`](https://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/concept/Hash.html). In particular, they define an operator() that:

> NOTE: 
>
> 下面其实就是 [`Hash`](https://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/concept/Hash.html) concept的requirement

3、 Does not throw exceptions when called

> NOTE: 
>
> 一、CppCoreGuidelines [C.89: Make a `hash` `noexcept`](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-hash)
>
> 

5、 For two different parameters `k1` and `k2` that are not equal, the probability that `std::hash<Key>()(k1) == std::hash<Key>()(k2)` should be very small, approaching 1.0/[std::numeric_limits](https://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/types/numeric_limits.html)<size_t>::max().

> NOTE: 
>
> 一、运行collision，说明并不要求是perfect hash function，关于collision，参见wikipedia [Hash table](https://en.wikipedia.org/wiki/Hash_table)

The unordered associative containers [std::unordered_set](https://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/container/unordered_set.html), [std::unordered_multiset](https://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/container/unordered_multiset.html), [std::unordered_map](https://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/container/unordered_map.html), [std::unordered_multimap](https://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/container/unordered_multimap.html) use specializations of the template **std::hash** as the default hash function.

> NOTE: 
>
> 如何实现呢？

### Notes

The actual hash functions are implementation-dependent and are not required to fulfill any other quality criteria except those specified above. Notably, some implementations use trivial (identity) hash functions which map an integer to itself. In other words, these hash functions are designed to work with unordered associative containers, but not as cryptographic hashes, for example.

> NOTE: 
>
> 一、上面这段话非常重要，首先它提出了两个概念:
>
> 1、"trivial (identity) hash function"
>
> 2、"cryptographic hash"
>
> 其次: 需要为不同的场景、不同的需求设计对应的hash function，对于 "unordered associative containers"，它不需要是 "cryptographic hashes"；由此可知: `std::hash` 主要是用于 "unordered associative containers" 的

####  (since C++14)

Hash functions are only required to produce the same result for the same input within a single execution of a program; this allows salted hashes that prevent collision DoS attacks.

> NOTE: 
>
> Python中也有类似的做法，参见 `object.__hash__`(*self*)[¶](https://docs.python.org/3/reference/datamodel.html#object.__hash__) 

#### `std::hash<const char*>`

There is no specialization for C strings. `std::hash<const char*>` produces a hash of the value of the pointer (the memory address), it does not examine the contents of any character array.

> NOTE: 
>
> 在思考使用`const char*`来作为`std::unordered_map`的时候，遇到了hash的问题，因此，我需要customize hash for `const char*`。

## `std::hash` in C++ standard library的概述

在 stackoverflow [std::hash value on char* value and not on memory address?](https://stackoverflow.com/questions/34597260/stdhash-value-on-char-value-and-not-on-memory-address) 的[回答](https://stackoverflow.com/a/34597785)中的一段话较好地概述了`std::hash` in C++ standard library: 

> Sadly enough, the current C++ standard library doesn't provide **general purpose hash algorithms** disentangled(拜托) from **object-specific hash solutions**. (But there is [some hope](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3980.html) this could change in the future.)

显然，这段话告诉了我们: 目前C++ standard library的`std::hash`是object-specific hash function，并没有提供general purpose hash algorithm。

### Change in the future: open-std [Types Don't Know](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3980.html) 

> NOTE: 在 stackoverflow [std::hash value on char* value and not on memory address?](https://stackoverflow.com/questions/34597260/stdhash-value-on-char-value-and-not-on-memory-address) 的[回答](https://stackoverflow.com/a/34597785)中"But there is [some hope](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3980.html) this could change in the future" [some hope](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3980.html) 所链接的是 open-std [Types Don't Know](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3980.html) 提案，本文就是基于这篇文章:

This paper proposes a new hashing infrastructure that completely decouples hashing algorithms from individual types that need to be hashed. This decoupling divides the hashing computation among 3 different programmers who need not coordinate with each other.


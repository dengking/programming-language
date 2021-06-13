

# hannes [A beginner's guide to C++ Ranges and Views.](https://hannes.hauswedell.net/post/2019/11/30/range_intro/)

## Preface

Since none of the large standard libraries ship C++ Ranges right now, you need to use the [range-v3 library](https://github.com/ericniebler/range-v3/) if you want to try any of this. If you do, you need to replace the `std::ranges::` prefixes with just `ranges::` and any `std::views::` prefixes with `ranges::views::`.

## Motivation

Traditionally most **generic algorithms** in the C++ standard library, like `std::sort`, take a pair of iterators (e.g. the object returned by `begin()`). If you want to sort a `std::vector v`, you have to call `std::sort(v.begin(), v.end())` and not `std::sort(v)`. Why was this design with iterators chosen? It is more **flexible**, because it allows e.g.:

- sorting only all elements after the fifth one:

```cpp
1std::sort(v.begin() + 5, v.end())
```

- using non-standard iterators like reverse iterators (sorts in reverse order):

```cpp
1std::sort(v.rbegin(), v.rend());
```

- combine both (sorts all elements except the last 5 in reverse order):

```cpp
1std::sort(v.rbegin() + 5, v.rend());
```

But this interface is less **intuitive** than just calling `std::sort` on the entity that you wish to sort and it allows for more **mistakes**, e.g. mixing two incompatible iterators. C++20 introduces the notion of *ranges* and provides algorithms that accept such in the namespace `std::ranges::`, e.g. `std::ranges::sort(v)` now works if `v` is range – and vectors are ranges!

> NOTE: 相比于range，iterator更容易出现错误



What about the examples that suggest superiority of the iterator-based approach? In C++20 you can do the following:

- sorting only all elements after the fifth one:

```cpp
1std::ranges::sort(std::views::drop(v, 5));
```

- sorting in reverse order:

```cpp
1std::ranges::sort(std::views::reverse(v));
```

- combine both:

```cpp
1std::ranges::sort(std::views::drop(std::views::reverse(v), 5));
```

We will discuss later what `std::views::reverse(v)` does, for now it is enough to understand that it returns something that appears like a container and that `std::ranges::sort` can sort it. Later you will see that this approach offers even more flexibility than working with iterators.

> NOTE: 都flexible

## Ranges

*Ranges* are an abstraction of “a collection of items”, or “something iterable”. The most basic definition requires only the existence of `begin()` and `end()` on the range.

### Range concepts

| Concept                            | Description                                              |
| ---------------------------------- | -------------------------------------------------------- |
| `std::ranges::input_range`         | can be iterated from beginning to end **at least once**  |
| `std::ranges::forward_range`       | can be iterated from beginning to end **multiple times** |
| `std::ranges::bidirectional_range` | iterator can also move backwards with `--`               |
| `std::ranges::random_access_range` | you can jump to elements **in constant-time** `[]`       |
| `std::ranges::contiguous_range`    | elements are always stored consecutively in memory       |



|                                    | `std::forward_list` | `std::list` | `std::deque` | `std::array` | `std::vector` |
| ---------------------------------- | ------------------- | ----------- | ------------ | ------------ | ------------- |
| `std::ranges::input_range`         | ✅                   | ✅           | ✅            | ✅            | ✅             |
| `std::ranges::forward_range`       | ✅                   | ✅           | ✅            | ✅            | ✅             |
| `std::ranges::bidirectional_range` |                     | ✅           | ✅            | ✅            | ✅             |
| `std::ranges::random_access_range` |                     |             | ✅            | ✅            | ✅             |
| `std::ranges::contiguous_range`    |                     |             |              | ✅            | ✅             |



### Storage behaviour

**Containers** are the ranges most well known, they own their elements. The standard library already provides many containers, see above.

**Views** are ranges that are usually defined on another range and transform the underlying range via some algorithm or operation. Views do not own any data beyond their algorithm and the time it takes to construct, destruct or copy them should not depend on the number of elements they represent. The algorithm is required to be lazy-evaluated so it is feasible to combine multiple views. More on this below.

The storage behaviour is orthogonal to the range concepts defined by the iterators mentioned above, i.e. you can have a container that satisfies `std::ranges::random_access_range` (e.g. `std::vector` does, but `std::list` does not) and you can have views that do so or don’t.

## Views

### Lazy-evaluation

A key feature of views is that whatever transformation they apply, they do so at the moment you request an element, not when the view is created.

```cpp
1std::vector vec{1, 2, 3, 4, 5, 6};
2auto v = std::views::reverse(vec);
```

Here `v` is a view; creating it neither changes `vec`, nor does `v` store any elements. The time it takes to construct `v` and its size in memory is independent of the size of `vec`.

```cpp
1std::vector vec{1, 2, 3, 4, 5, 6};
2auto v = std::views::reverse(vec);
3std::cout << *v.begin() << '\n';
```

This will print “6”, but the important thing is that resolving the first element of `v` to the last element of `vec` happens **on-demand**. This guarantees that views can be used as flexibly as iterators, but it also means that if the view performs an expensive transformation, it will have to do so repeatedly if the same element is requested multiple times.

### Combinability

You may have wondered why I wrote

```cpp
1auto v = std::views::reverse(vec);
```

and not

```cpp
1std::views::reverse v{vec};
```

That’s because `std::views::reverse` is not the view itself, it’s an *adaptor* that takes the underlying range (in our case the vector) and returns a view object over the vector. The exact type of this view is hidden behind the `auto` statement. This has the advantage, that we don’t need to worry about the template arguments of the view type, but more importantly the adaptor has an additional feature: it can be *chained* with other adaptors!

```cpp
1std::vector vec{1, 2, 3, 4, 5, 6};
2auto v = vec | std::views::reverse | std::views::drop(2);
3
4std::cout << *v.begin() << '\n';
```



# modernescpp [C++20: The Ranges Library](https://www.modernescpp.com/index.php/c-20-the-ranges-library)

```C++
// rangesFilterTransform.cpp

#include <iostream>
#include <ranges>
#include <vector>

int main() {

    std::vector<int> numbers = {1, 2, 3, 4, 5, 6};
  
    auto results = numbers | std::views::filter([](int n){ return n % 2 == 0; })
                           | std::views::transform([](int n){ return n * 2; });
                           
    for (auto v: results) std::cout << v << " ";     // 4 8 12

}
```


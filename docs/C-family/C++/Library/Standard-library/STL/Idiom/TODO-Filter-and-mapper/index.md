# Filter

## STL `_if`

在STL中，带filter的algorithm一般以`_if`结尾，比如`std::copy_if`

## 使用filter iterator实现

第二种实现filter的方式是filter iterator，实现有:
a、boost [Filter Iterator](https://www.boost.org/doc/libs/1_67_0/libs/iterator/doc/html/iterator/specialized/filter.html)

## 使用 `std::stable_partition()`实现

参见 [iterators of a vector filtered?](https://stackoverflow.com/questions/44350760/iterators-of-a-vector-filtered) # [A](https://stackoverflow.com/a/44351167)

> I suggest it would be easier to use standard algorithm `std::stable_partition()` to separate the container into two ranges.
>
> ```cpp
>  auto false_partition = std::stable_partition(your_vector.begin(), your_vector.end(), your_filter);
> ```
>
> The `begin()` and `end()` iterators of the vector do not change (i.e. are not invalidated), but the elements between them are reorganised into two ranges, such that the elements for which `your_filter` returns `true` precedes the set of elements for which `your_filter` returns `false`. `false_partition` is therefore simultaneously the "past the end" iterator for the first range, and the beginning of the second range. The order of elements in each range is the same as in the original vector.
>
> These can be used as follows
>
> ```cpp
>  //   a loop to operates on the elements for which your_filter returned true
> 
>  for (auto i = your_vector.begin(); i != false_partition; ++i)
>  {
>       // do whatever
>  }
> 
>  //   a loop to operates on the elements for which your_filter returned false
> 
>  for (auto i = false_partition; i != your_vector.end(); ++i)
>  {
>       // do whatever
>  }
> ```
>
> Before C++11, the `auto` keyword can be replaced with appropriate iterator types (e.g. `std::vector<int>::iterator` or `std::vector<int>::const_iterator`, depending on whether you want the elements to be changed using the iterators).

## 仿照SLT algorithm自定义实现

参见 bastian.rieck [Implementing filter and map with C++11](https://bastian.rieck.me/blog/posts/2016/filter_map_cxx11/)。
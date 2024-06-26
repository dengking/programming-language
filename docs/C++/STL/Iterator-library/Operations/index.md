# Iterator operation

对应cppreference iterator operation。在阅读akrzemi1 [Value semantics](https://akrzemi1.wordpress.com/2012/02/03/value-semantics/)的时候，其中对比了[std::next](https://en.cppreference.com/w/cpp/iterator/next)、[std::advance](https://en.cppreference.com/w/cpp/iterator/advance)。

## cppreference [std::distance](https://en.cppreference.com/w/cpp/iterator/distance)



### C++17 implementation

对比Second version和First version，可以发现两种实现，其实都是tag dispatch，即根据iterator的`iterator_category`来选择不同的实现。

```c++
// implementation via constexpr if, available in C++17
template<class It>
constexpr typename std::iterator_traits<It>::difference_type
    distance(It first, It last)
{
    using category = typename std::iterator_traits<It>::iterator_category;
    static_assert(std::is_base_of_v<std::input_iterator_tag, category>);
 
    if constexpr (std::is_base_of_v<std::random_access_iterator_tag, category>) // random access iterator
        return last - first;
    else { // 其他的iterator
        typename std::iterator_traits<It>::difference_type result = 0;
        while (first != last) {
            ++first;
            ++result;
        }
        return result;
    }
}
```



### 使用 `std::distance` 的优势

在 artima [On the Tension Between Object-Oriented and Generic Programming in C++](https://www.artima.com/cppsource/type_erasure.html) 中给出的例子说明了使用 `std::distance` 的优势，概括起来是:

1、program to abstraction and polymorphism



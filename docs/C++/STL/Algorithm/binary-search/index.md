# binary search



## cppreference [std::binary_search](https://en.cppreference.com/w/cpp/algorithm/binary_search)



### Possible implementation

See also the implementations in [libstdc++](https://github.com/gcc-mirror/gcc/blob/d9375e490072d1aae73a93949aa158fcd2a27018/libstdc%2B%2B-v3/include/bits/stl_algo.h#L2236) and [libc++](https://github.com/llvm-mirror/libcxx/blob/a12cb9d211019d99b5875b6d8034617cbc24c2cc/include/algorithm#L4320).

```C++
template<class ForwardIt, class T>
bool binary_search(ForwardIt first, ForwardIt last, const T& value)
{
    first = std::lower_bound(first, last, value);
    return (!(first == last) && !(value < *first));
}
```



## cppreference [std::lower_bound](https://en.cppreference.com/w/cpp/algorithm/lower_bound)



## cppreference [std::upper_bound](https://en.cppreference.com/w/cpp/algorithm/upper_bound)



## stackoverflow [lower_bound to perform binary search](https://stackoverflow.com/questions/30352231/lower-bound-to-perform-binary-search)
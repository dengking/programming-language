# [std::multimap](https://en.cppreference.com/w/cpp/container/multimap)、 [std::unordered_multimap](https://en.cppreference.com/w/cpp/container/unordered_multimap)

一、 [std::multimap](https://en.cppreference.com/w/cpp/container/multimap)`<int, int>`、 [std::unordered_multimap](https://en.cppreference.com/w/cpp/container/unordered_multimap)`<int, int>`  VS `std::map<int, std::vector<int>>`、`std::unordered_map<int, std::vector<int>>`

使用 [std::multimap](https://en.cppreference.com/w/cpp/container/multimap)、 [std::unordered_multimap](https://en.cppreference.com/w/cpp/container/unordered_multimap) 可以实现value类型为 `std::vector` 的效果，但是，前者无法保证插入value时的次序，而后者时可以的。

这是我在做 [LeetCode-1. Two Sum](https://leetcode.cn/problems/two-sum/) 时想到的。



## cppreference [std::multimap](https://en.cppreference.com/w/cpp/container/multimap)



## cppreference [std::unordered_multimap](https://en.cppreference.com/w/cpp/container/unordered_multimap)



> draft:
>
> 一、range API
>
> [insert_range](https://en.cppreference.com/mwiki/index.php?title=cpp/container/unordered_multimap/insert_range&action=edit&redlink=1) (C++23)
>
> [equal_range](https://en.cppreference.com/w/cpp/container/unordered_multimap/equal_range) (C++11)



## `equals_range`

cppreference [std::multimap<Key,T,Compare,Allocator>::equal_range](https://en.cppreference.com/w/cpp/container/multimap/equal_range) 



素材:

1、thispointer [Finding all values for a key in multimap using equals_range – Example](https://thispointer.com/finding-all-values-for-a-key-in-multimap-using-equals_range-example/)

```c++
#include <iostream>
#include <map>
#include <iterator>
#include <algorithm>

int main()
{
    // MultiMap of char and int
    std::multimap<char, int> mmapOfPos = {
        {'a', 1},
        {'b', 2},
        {'c', 3},
        {'a', 4},
        {'c', 5},
        {'c', 6},
        {'d', 7},
    };

    typedef std::multimap<char, int>::iterator MMAPIterator;

    // It returns a pair representing the range of elements with key equal to 'c'
    std::pair<MMAPIterator, MMAPIterator> result = mmapOfPos.equal_range('c');

    std::cout << "All values for key 'c' are," << std::endl;

    // Iterate over the range
    for (MMAPIterator it = result.first; it != result.second; it++)
        std::cout << it->second << std::endl;

    // Total Elements in the range
    int count = std::distance(result.first, result.second);
    std::cout << "Total values for key 'c' are : " << count << std::endl;

    return 0;
}
```



```c++
#include <iostream>
#include <map>
#include <iterator>
#include <algorithm>

int main()
{
    // MultiMap of char and int
    std::multimap<char, int> mmapOfPos = {
        {'a', 1},
        {'b', 2},
        {'c', 3},
        {'a', 4},
        {'c', 5},
        {'c', 6},
        {'d', 7},
    };

    typedef std::multimap<char, int>::iterator MMAPIterator;

    // It returns a pair representing the range of elements with key equal to 'f'
    std::pair<MMAPIterator, MMAPIterator> result = mmapOfPos.equal_range('f');

    // Total Elements in the range
    int count = std::distance(result.first, result.second);

    std::cout << "Total values for key 'f' are : " << count << std::endl;

    if (count == 0)
        std::cout << "key 'f' don't exist in multimap" << std::endl;
    return 0;
}
```



## [lower_bound](https://en.cppreference.com/w/cpp/container/multimap/lower_bound)、[upper_bound](https://en.cppreference.com/w/cpp/container/multimap/upper_bound)

素材:

1、geeksforgeeks [How to traverse through all values for a given key in multimap?](https://www.geeksforgeeks.org/traverse-values-given-key-multimap/)
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

## `std::lower_bound` vs `std::upper_bound`

### stackoverflow [lower_bound == upper_bound](https://stackoverflow.com/questions/12158948/lower-bound-upper-bound)

A

1、lower bound: first element that is greater-or-equal.

2、Upper bound: first element that is strictly greater.

**Example:**

```
+- lb(2) == ub(2)       +- lb(6)        +- lb(8)
|        == begin()     |  == ub(6)     |   +- ub(8) == end()
V                       V               V   V
+---+---+---+---+---+---+---+---+---+---+---+
| 3 | 4 | 4 | 4 | 4 | 5 | 7 | 7 | 7 | 7 | 8 |
+---+---+---+---+---+---+---+---+---+---+---+
    ^               ^                       ^
    |               |                       |
    +- lb(4)        +- ub(4)                +- lb(9) == ub(9) == end()

    |- eq-range(4) -|
```

As you can see, the half-open equal-range for *n* is [lb(*n*), ub(*n*)).

> NOTE: 
>
> 上面这段总结非常好: 
>
> 所谓 `lower_bound`，即下限； `>=`
>
> 所谓 `upper_bound`，即上限； `<`
>
> 符合STL的左边右开惯例
>
> STL的sorted container都支持这个function，see also:
>
> cppreference [std::map<Key,T,Compare,Allocator>::upper_bound](https://en.cppreference.com/w/cpp/container/map/upper_bound)

Note that both bounds give you meaningful insertion locations for an element of the desired value so that the ordering is maintained, but `lower_bound` has the distinguishing feature that *if* the element already exists, then you get an iterator which actually points to that element. Thus you can use `lower_bound` on an ordered range to implement your own unique-membership *or* multiple-membership container.

```c++
void insert(Container & c, T const & t)
{
    auto it = std::lower_bound(c.begin(), c.end(), t);

    // if unique container:
    if (it != c.end() && *it == t) { /* error, element exists! */ return; }

    c.insert(it, t);
}
```



## cppreference [std::lower_bound](https://en.cppreference.com/w/cpp/algorithm/lower_bound)

Returns an iterator pointing to the first element in the range `[first, last)` that is *not less* than (i.e. greater or equal to) `value`, or `last` if no such element is found.

### example

```c++
#include <iostream>
#include <vector>
#include <utility>   // std::pair
#include <algorithm> // std::lower_bound
#include <iterator>  // std::distance
#include <unordered_map>
#include <string>
#include <map>
#include <climits> // INT_MAX
using namespace std;

int main()
{
  const std::vector<int> data = {1, 3, 5, 7, 9};
  auto lower = std::lower_bound(data.begin(), data.end(), 2);
  lower != data.end()
      ? std::cout << *lower << " at index " << std::distance(data.begin(), lower)
      : std::cout << "[not found]";
  std::cout << '\n';
}
```

输出如下：

```
3 at index 1
```



## cppreference [std::upper_bound](https://en.cppreference.com/w/cpp/algorithm/upper_bound)

Returns an iterator pointing to the first element in the range `[first, last)` that is *greater* than `value`, or `last` if no such element is found.

### example

```c++
#include <iostream>
#include <vector>
#include <utility>   // std::pair
#include <algorithm> // std::lower_bound
#include <iterator>  // std::distance
#include <unordered_map>
#include <string>
#include <map>
#include <climits> // INT_MAX
using namespace std;

int main()
{
  const std::vector<int> data = {1, 3, 5, 7, 9};
  auto lower = std::upper_bound(data.begin(), data.end(), 2);
  lower != data.end()
      ? std::cout << *lower << " at index " << std::distance(data.begin(), lower)
      : std::cout << "[not found]";
  std::cout << '\n';
}
```

输出如下：

```
3 at index 1
```



## stackoverflow [lower_bound to perform binary search](https://stackoverflow.com/questions/30352231/lower-bound-to-perform-binary-search)
# contiguous_container concept

## cppreference [C++ named requirements: *ContiguousContainer*](https://en.cppreference.com/w/cpp/named_req/ContiguousContainer)

## stackoverflow [Checking if a sequence container is contiguous in memory](https://stackoverflow.com/questions/35004633/checking-if-a-sequence-container-is-contiguous-in-memory)

```C++
Is there a way to check if a sequence container is contiguous in memory? Something like:

#include <iostream>
#include <vector>
#include <deque>
#include <array>

int main()
{
    std::cout << std::boolalpha;
    std::cout << is_contiguous<std::vector<int>>::value   << '\n'  // true
    std::cout << is_contiguous<std::deque<int>>::value    << '\n'; // false
    std::cout << is_contiguous<std::array<int, 3>>::value << '\n'; // true
}
```



## Implementation

### `boost.beast.type_traits.hpp`

https://www.boost.org/doc/libs/master/boost/beast/core/detail/type_traits.hpp
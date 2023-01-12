# Primer



hannes [A beginner's guide to C++ Ranges and Views.](https://hannes.hauswedell.net/post/2019/11/30/range_intro/)

modernescpp [C++20: The Ranges Library](https://www.modernescpp.com/index.php/c-20-the-ranges-library)

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


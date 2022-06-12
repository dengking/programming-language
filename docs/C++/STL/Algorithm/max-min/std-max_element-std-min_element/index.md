# `std::max_element`、`std::min_element`、`std::minmax_element`



## stackoverflow [How can I get the max (or min) value in a vector?](https://stackoverflow.com/questions/9874802/how-can-i-get-the-max-or-min-value-in-a-vector)

### [A](https://stackoverflow.com/a/9874912)

Using c++11/c++0x compile flags, you can

```cpp
auto it = max_element(std::begin(cloud), std::end(cloud)); // c++11
```

Otherwise, write your own:

```cpp
template <typename T, size_t N> const T* mybegin(const T (&a)[N]) { return a; }    
template <typename T, size_t N> const T* myend  (const T (&a)[N]) { return a+N; }
```

See it live at **http://ideone.com/aDkhW**:

```cpp
#include <iostream>
#include <algorithm>

template <typename T, size_t N> const T* mybegin(const T (&a)[N]) { return a; }    
template <typename T, size_t N> const T* myend  (const T (&a)[N]) { return a+N; }

int main()
{
    const int cloud[] = { 1,2,3,4,-7,999,5,6 };

    std::cout << *std::max_element(mybegin(cloud), myend(cloud)) << '\n';
    std::cout << *std::min_element(mybegin(cloud), myend(cloud)) << '\n';
}
```

Oh, and **use [`std::minmax_element(...)`](http://en.cppreference.com/w/cpp/algorithm/minmax_element)** if you need both at once :/

## cppreference [std::min_element](https://en.cppreference.com/w/cpp/algorithm/min_element)



## cppreference [std::max_element](https://en.cppreference.com/w/cpp/algorithm/max_element)



## cppreference [std::minmax_element](https://en.cppreference.com/w/cpp/algorithm/minmax_element)



## max value of `std::map`

stackoverflow [Finding the max value in a map](https://stackoverflow.com/questions/9370945/finding-the-max-value-in-a-map)

techiedelight [Find element with the maximum value in a map in C++](https://www.techiedelight.com/find-element-with-the-maximum-value-in-a-map-in-cpp/)



### example code

这是LeetCode [424. 替换后的最长重复字符](https://leetcode.cn/problems/longest-repeating-character-replacement/) 的源代码:

```C++
#include <iostream>
#include <string>
#include <unordered_map>
#include <array>
#include <vector>
#include <string>
#include <iostream>
#include <variant>
#include <typeinfo>
#include <limits>

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

class Solution
{
public:
    int characterReplacement(string s, int k)
    {
        std::unordered_map<char, int> cnt;
        using pair_type = decltype(cnt)::value_type;
        // https://stackoverflow.com/a/9371137/10173843
        auto max_of_cnt_func = [&]() -> int
        {
            return std::max_element(cnt.begin(), cnt.end(), [](const pair_type &left, const pair_type &right)
                                    { return left.second < right.second; })
                ->second;
        };
        int ret = 0;
        int len = s.size();
        for (int left = 0, right = 0; right < len; ++right)
        {
            cnt[s[right]]++;
            // int window_size = right - left + 1;
            while (right - left + 1 - max_of_cnt_func() > k)
            {
                cnt[s[left++]]--;
            }
            ret = max(ret, right - left + 1);
        }
        return ret;
    }
};

int main()
{
}
// g++ test.cpp --std=c++11 -pedantic -Wall -Wextra

```


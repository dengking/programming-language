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





## `std::initializer_list<T>`参数

这种入参，兼职就是语法糖。

leetcode [【中规中矩】Bellman Ford 动态规划两种写法](https://leetcode-cn.com/problems/cheapest-flights-within-k-stops/solution/zhong-gui-zhong-ju-bellman-ford-dong-tai-gui-hua-l/)

```C++
dp[v][k] = min({dp[v][k - 1], dp[v][k], dp[u][k - 1] + w});
```


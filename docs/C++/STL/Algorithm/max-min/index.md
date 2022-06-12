# max、min

## zhihu [C++的std::max({a, b, c})，这是什么用法？](https://www.zhihu.com/question/338294495/answer/772000909)



[裴浩的回答]( https://www.zhihu.com/question/338294495/answer/772000909)

C++11对`max`的新重载：

```c++
template <class T> T max (initializer_list<T> il);
template <class T, class Compare>
T max (initializer_list<T> il, Compare comp);
```

 而在C++11中`{1,2,3}`会被推导成`std::initializer_list<int>`类型。





## `std::max`、`std::min`

### `std::initializer_list<T>`参数

这种入参，兼职就是语法糖。

leetcode [【中规中矩】Bellman Ford 动态规划两种写法](https://leetcode-cn.com/problems/cheapest-flights-within-k-stops/solution/zhong-gui-zhong-ju-bellman-ford-dong-tai-gui-hua-l/)

```C++
dp[v][k] = min({dp[v][k - 1], dp[v][k], dp[u][k - 1] + w});
```



#### `std::max(vector)`?

结论：不支持，应该使用`max_element(v.begin(), v.end())`。

```c++
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <utility> // std::pair
#include <unordered_map>
#include <string>
#include <set>
#include <map>
#include <climits> // INT_MAX
using namespace std;

int main()
{
  Solution s;
  vector<int> v {1,2,3};
  max(v);
}

```

编译报错如下：

```c++
example.cc:37:3: error: no matching function for call to 'max'
  max(v);
  ^~~
/Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/__algorithm/max.h:59:1: note: candidate template ignored: could not match 'initializer_list' against 'vector'
max(initializer_list<_Tp> __t)
^
/Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/__algorithm/max.h:39:1: note: candidate function template not viable: requires 2 arguments, but 1 was provided
max(const _Tp& __a, const _Tp& __b)
^
/Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/__algorithm/max.h:50:1: note: candidate function template not viable: requires 2 arguments, but 1 was provided
max(initializer_list<_Tp> __t, _Compare __comp)
^
/Applications/Xcode-beta.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1/__algorithm/max.h:30:1: note: candidate function template not viable: requires 3 arguments, but 1 was provided
max(const _Tp& __a, const _Tp& __b, _Compare __comp)
^
1 error generated.
```


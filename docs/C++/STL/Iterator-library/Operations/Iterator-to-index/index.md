# iterator to index



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
  vector<int> colors{2, 1, 2, 2, 1};
  cout << std::distance(colors.begin(), colors.begin() + 1) << endl;
  cout << std::distance(colors.begin(), colors.begin() + 2) << endl;
  cout << std::distance(colors.begin(), colors.end() - 1) << endl;
  cout << std::distance(colors.begin(), colors.end()) << endl;
}

```

输出如下：

```
1
2
4
5
```


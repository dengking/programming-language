# Custom `emplace`

对于custom  container，如何实现`emplace`呢？在前面的章节中，我们已经知道`emplace`的实现所依赖于语言特性。

下面分别对我们custom container的两种方式中，如何custom `emplace`来进行说明。



## Wrapper of STL container 

对于基于STL container 进行封装实现的custom container，由于STL container 一般都实现了`emplace`，因此，programmer可以直接进行调用，下面是一些例子。

### stackoverflow [Custom Container emplace with variadic templates](https://stackoverflow.com/questions/28628484/custom-container-emplace-with-variadic-templates)



[A](https://stackoverflow.com/a/28672159)

In case anyone stumbles upon the same issue, this is how I achieved this:

```cpp
void emplace(Args&&... args) {
    ++Count;
    ++_indexWrite;
    if (_indexWrite > _size - 1) _indexWrite = 0;
    _v.emplace(_v.begin() + _indexWrite, std::forward<Args>(args)...);
}
```

Although what I really wanted was to construct an element using the reserved memory in that index, and not inserting a new element at that specific position.

### stackexchange [Perfect-forwarding while implementing emplace](https://codereview.stackexchange.com/questions/57494/perfect-forwarding-while-implementing-emplace)



```C++
#include <set>
using namespace std;

template<typename T>
class myset {
  std::set<T> m_s;
  typedef typename std::set<T>::iterator iterator;
  int cnt;
public:
  template <class... Args>
  pair <iterator,bool> emplace ( Args&&... args ) {
    pair<iterator,bool> p = m_s.emplace(std::forward<Args>(args)...);
    if (p.second) //true if a new element was inserted
      cnt ++; //house keeping
    return p;
  }
};

int main() {myset<int> s;}
```


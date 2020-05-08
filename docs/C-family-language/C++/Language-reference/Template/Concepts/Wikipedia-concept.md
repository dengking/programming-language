# 维基百科[Concepts (C++)](https://en.wikipedia.org/wiki/Concepts_(C%2B%2B))



## [Main uses](https://en.wikipedia.org/wiki/Concepts_(C%2B%2B)#Main_uses)

> NOTE: 原文的main use章节的总结是非常好的



## [Compiler diagnostics](https://en.wikipedia.org/wiki/Concepts_(C%2B%2B)#Compiler_diagnostics)

```c++
#include <iostream>
#include <algorithm>
#include <list>


int main() {
    std::list<int> l = {2, 1, 3};
    std::sort(l.begin(), l.end());
}
```

`g++ --std=c++11 test.cpp`

编译错误如下：

```c++
In file included from /usr/include/c++/4.8.2/algorithm:62:0,
                 from test.cpp:2:
/usr/include/c++/4.8.2/bits/stl_algo.h: In instantiation of ‘void std::sort(_RAIter, _RAIter) [with _RAIter = std::_List_iterator<int>]’:
test.cpp:8:33:   required from here
/usr/include/c++/4.8.2/bits/stl_algo.h:5461:22: error: no match for ‘operator-’ (operand types are ‘std::_List_iterator<int>’ and ‘std::_List_iterator<int>’)
     std::__lg(__last - __first) * 2);

```

If concepts are used, the error can be detected and reported in the context of the call:

```
error: cannot call function 'void std::sort(_RAIter, _RAIter) [with _RAIter = std::_List_iterator<int>]'
note:   concept 'RandomAccessIterator()' was not satisfied
```

> NOTE: 在cppreference [Constraints and concepts](https://en.cppreference.com/w/cpp/language/constraints) 中对此有说明，因为
>
> Violations of **constraints** are detected at compile time, early in the template instantiation process, which leads to easy to follow error messages.

## [Overload resolution](https://en.wikipedia.org/wiki/Concepts_(C%2B%2B)#Overload_resolution)

> NOTE: concept可以作为 [SFINAE](https://en.wikipedia.org/wiki/SFINAE) and [tag dispatching](https://en.wikipedia.org/w/index.php?title=Tag_dispatching&action=edit&redlink=1) 的替代方案

## [Type deduction](https://en.wikipedia.org/wiki/Concepts_(C%2B%2B)#Type_deduction)


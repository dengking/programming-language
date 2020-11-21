# wikipedia [Concepts (C++)](https://en.wikipedia.org/wiki/Concepts_(C%2B%2B))
> 维基百科的[Concepts (C++)](https://en.wikipedia.org/wiki/Concepts_(C%2B%2B))对concept的介绍是比较简洁清晰的，可以作为prime来进行阅读。
>
> C++ concept是对GP concept的一种实现，同时结合C++的实现进行了很多的扩展。


## [Main uses](https://en.wikipedia.org/wiki/Concepts_(C%2B%2B)#Main_uses)

> NOTE: 原文的main use章节的总结是非常好的

The main uses of concepts are:

1) Introducing type-checking to template programming

> NOTE: type requirement是GP的核心内容

2) Simplified compiler diagnostics for failed template instantiations



3) Selecting **function template overloads** and **class template specializations** based on type properties

> NOTE: static polymorphism，能够替代SFINAE

4) Constraining automatic type deduction

> NOTE: 这个用法还不太了解，需要进行扩展

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



## TO READ

https://www.modernescpp.com/index.php/c-20-concepts-the-details

https://www.drdobbs.com/cpp/the-c0x-remove-concepts-decision/218600111

https://www.modernescpp.com/index.php/c-20-define-the-concept-regular-and-semiregular
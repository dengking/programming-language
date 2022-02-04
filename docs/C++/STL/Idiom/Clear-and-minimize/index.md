# Clear-and-minimize



## [More C++ Idioms Clear-and-minimize](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Clear-and-minimize)



### 11.0.2 Also Known As

This is sometimes called the swap with temporary idiom.

> NOTE: 
>
> 1、显然，它依赖于temporary、swap
>
> 2、显然，"swap with temporary idiom"是更加形象的

### 11.0.4 Solution and Sample Code

```c++
#include <vector>
#include <iostream>
int main()
{
    std::vector <int> v;
    for(int i=0;i<100;i++)
    {
        v.push_back(i);
    }
    v.clear();
    std::cout<<"size of v:"<<v.size()<<std::endl;
    std::cout<<"capacity of v:"<<v.capacity()<<std::endl;
    std::vector<int>().swap (v);
    std::cout<<"size of v:"<<v.size()<<std::endl;
    std::cout<<"capacity of v:"<<v.capacity()<<std::endl;
    return 0;
}


```

> NOTE: 
>
> 1、输出如下：
>
> ```
> size of v:0
> capacity of v:128
> size of v:0
> capacity of v:0
> ```
>
> 2、下面是精髓所在:
>
> ```C++
> std::vector<int>().swap (v);
> ```
>
> default constructor + swap

### 11.0.5 Solution in C++14

Since C++11, some containers declare the function [shrink_to_fit()](http://www.cplusplus.com/reference/vector/vector/shrink_to_fit/), e.g. [`vector`](http://www.cplusplus.com/reference/vector/vector/) , `deque`, `basic_string`. `shrink_to_fit()` which is a non-binding request to reduce `capacity()` to `size()`. Thus, using clear() and `shrink_to_fit()` is a non-binding request to clear-and-minimize.

> NOTE: `vector`在C++11中已经支持[shrink_to_fit()](http://www.cplusplus.com/reference/vector/vector/shrink_to_fit/)



## stackoverflow [How do I clear the std::queue efficiently?](https://stackoverflow.com/questions/709146/how-do-i-clear-the-stdqueue-efficiently)



### [A](https://stackoverflow.com/a/709161)

A common idiom for clearing standard containers is swapping with an empty version of the container:

```c++
void clear( std::queue<int> &q )
{
   std::queue<int> empty;
   std::swap( q, empty );
}
```

It is also the only way of actually clearing the memory held inside some containers (`std::vector`)
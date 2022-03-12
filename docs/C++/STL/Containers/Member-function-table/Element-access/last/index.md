# last element of container

### zoukankan [C++ 获取vector容器最后一个元素](http://t.zoukankan.com/guolixiucai-p-4878295.html)

声明：`vector<T> vec`;

方法一： `return vec.at(vec.size()-1);`

方法二： `return vec.back();`

方法三： `return vec.end()-`1; 注意：end指向末尾元素的下一个元素。

方法四： `return vec.rbegin();`



## iterator to last element

### stackoverflow [Iterator to last element of std::vector using end()--](https://stackoverflow.com/questions/37017302/iterator-to-last-element-of-stdvector-using-end)

A

```C++
std::vector<int>::iterator it = --container.end();
std::vector<int>::iterator it = container.end() - 1;
std::vector<int>::iterator it = std::prev(container.end());
```


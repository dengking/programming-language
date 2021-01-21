# [martinmoene](https://github.com/martinmoene)/**[string-view-lite](https://github.com/martinmoene/string-view-lite)**





## 实现分析

它的hash的实现，其实是复用的`std::string`，它通过构建一个`std::string` temporary来实现hash。


# Data dependency

## Classic example

1、stackoverflow [How to understand acquire and release semantics?](https://stackoverflow.com/questions/24565540/how-to-understand-acquire-and-release-semantics) # [A](https://stackoverflow.com/a/24565699)

收录于 `Release-Acquire-ordering` 章节

2、cppreference [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order) # Explanation # Relaxed ordering

没有data dependency的，则compiler是能够进行reordering的

3、preshing [Weak vs. Strong Memory Models](https://preshing.com/20120930/weak-vs-strong-memory-models/)



## cppreference 中关于data dependency的

1、

cppreference [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order) # Formal description # Carries dependency

cppreference [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order) # Formal description # Dependency-ordered before

### dependency chain control

1、`[[carries_dependency]]`

2、[std::kill_dependency](https://en.cppreference.com/w/cpp/atomic/kill_dependency)
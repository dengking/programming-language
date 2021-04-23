# `std::make_array`



## stackoverflow [Current status of std::make_array](https://stackoverflow.com/questions/50942556/current-status-of-stdmake-array)



[A](https://stackoverflow.com/a/50943029)

As @DeiDei writes, C++17 includes [template argument deduction for classes](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0091r3.html), so you can now write:

```cpp
std::pair p (foo, bar);
std::array arr = { 1, 2, 3, 4, 5 };
```

and so on. But there are some (somewhat subtle) remaining use cases where `make_pair` or `make_array` can be useful, and you can read about them in: [Usefulness of std::make_pair and std::make_tuple in C++1z](https://stackoverflow.com/questions/43875420/usefulness-of-stdmake-pair-and-stdmake-tuple-in-c1)





## cppreference [std::experimental::make_array](https://en.cppreference.com/w/cpp/experimental/make_array)
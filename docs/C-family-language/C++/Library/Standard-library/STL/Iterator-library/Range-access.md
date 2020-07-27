# Range access

C++11为了支持[Range-based for loop](https://en.cppreference.com/w/cpp/language/range-for)，iterator library 中引入了**Range access**。





## cppreference [std::begin, std::cbegin](https://en.cppreference.com/w/cpp/iterator/begin)





## cppreference [std::end, std::cend](https://en.cppreference.com/w/cpp/iterator/end)





## User-defined overloads and ADL

关于这一点，在cppreference [std::begin, std::cbegin](https://en.cppreference.com/w/cpp/iterator/begin)、cppreference [std::end, std::cend](https://en.cppreference.com/w/cpp/iterator/end)中，都有描述，下面是截取自cppreference [std::begin, std::cbegin](https://en.cppreference.com/w/cpp/iterator/begin)：

> Similar to the use of `swap` (described in [*Swappable*](../named_req/Swappable.html)), typical use of the `begin` function in generic context is an equivalent of `using std::begin; begin(arg);`, which allows both the [ADL](../language/adl.html)-selected overloads for user-defined types and the standard library function templates to appear in the same overload set.
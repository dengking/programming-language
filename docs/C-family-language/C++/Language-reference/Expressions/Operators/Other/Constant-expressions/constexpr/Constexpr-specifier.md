# `constexpr` specifier

`constexpr`应该是constant expression的缩写，显然它是为了支持c++的constant expression特性而添加的。

## cppreference [constexpr specifier](https://en.cppreference.com/w/cpp/language/constexpr)

### Explanation

> NOTE: 因为本节第一段解释地非常清楚



A **constexpr variable** must satisfy the following requirements:

> NOTE: 原文给出了**constexpr variable**的若干requirements，总的来说，这些requirements是为了保证这个variable的值是可以在compile time evaluate得到的，所以它需要排除所有的run time feature。



## microsoft [constexpr (C++)](https://docs.microsoft.com/en-us/cpp/cpp/constexpr-cpp?view=vs-2019)



## [Difference between `constexpr` and `const`](https://stackoverflow.com/questions/14116003/difference-between-constexpr-and-const)
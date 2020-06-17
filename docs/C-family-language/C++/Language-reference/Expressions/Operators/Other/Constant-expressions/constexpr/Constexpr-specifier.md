# `constexpr` specifier

`constexpr`应该是constant expression的缩写，显然它是为了支持c++的constant expression特性而添加的。

## cppreference [constexpr specifier](https://en.cppreference.com/w/cpp/language/constexpr)

### Explanation

> NOTE: 因为本节第一段解释地非常清楚



A **constexpr variable** must satisfy the following requirements:

> NOTE: 原文给出了**constexpr variable**的若干requirements，总的来说，这些requirements是为了保证这个variable的值是可以在compile time evaluate得到的，所以它需要排除所有的run time feature。



## microsoft [constexpr (C++)](https://docs.microsoft.com/en-us/cpp/cpp/constexpr-cpp?view=vs-2019)



## [Difference between `constexpr` and `const`](https://stackoverflow.com/questions/14116003/difference-between-constexpr-and-const)



## Example

### 维基百科[Compile time function execution#Examples](https://en.wikipedia.org/wiki/Compile_time_function_execution)

C++11之前：

```c++
template <int N>
struct Factorial {
  enum { value = N * Factorial<N - 1>::value };
};

template <>
struct Factorial<0> {
  enum { value = 1 };
};

// Factorial<4>::value == 24
// Factorial<0>::value == 1
void Foo() {
  int x = Factorial<0>::value;  // == 1
  int y = Factorial<4>::value;  // == 24
}
```

C++11

```c++
#include <cstdio>

constexpr int Factorial(int n) { return n ? (n * Factorial(n - 1)) : 1; }

constexpr int f10 = Factorial(10);

int main() {
  printf("%d\n", f10);
  return 0;
}
```

C++14

```c++
// Iterative factorial at compile time.
constexpr int Factorial(int n) {
  int result = 1;
  while (n > 1) {
    result *= n--;
  }
  return result;
}

int main() {
  constexpr int f4 = Factorial(4);  // f4 == 24
}
```

### galowicz [What is a Type Trait?](https://blog.galowicz.de/2016/02/18/what_is_a_type_trait/)

```c++
template <bool X>
struct not
{
    static constexpr bool value {!X};
};
```



```c++
static constexpr not(bool x) 
{
    return !x;
}
```
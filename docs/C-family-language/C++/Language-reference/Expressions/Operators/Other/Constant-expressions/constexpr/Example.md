# Example

## 维基百科[Compile time function execution#Examples](https://en.wikipedia.org/wiki/Compile_time_function_execution)



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

```c++
#include <cstdio>

constexpr int Factorial(int n) { return n ? (n * Factorial(n - 1)) : 1; }

constexpr int f10 = Factorial(10);

int main() {
  printf("%d\n", f10);
  return 0;
}
```

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

## galowicz [What is a Type Trait?](https://blog.galowicz.de/2016/02/18/what_is_a_type_trait/)

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
# SFINAE

“SFINAE”是“Substitution failure is not an error”的缩写。

SFINAE和c++的特性`template`相关，"substitution"所表示是使用template argument来**substitute** template parameter。对于a set of  candidate，compiler会逐个尝试，直到有一个成功，candidate可以是：

- [function overloads](https://en.wikipedia.org/wiki/Overload_resolution)
- template specializations



## 维基百科[Substitution failure is not an error](https://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error)



### Example

```c++
struct Test {
  typedef int foo;
};

template <typename T>
void f(typename T::foo) {}  // Definition #1

template <typename T>
void f(T) {}  // Definition #2

int main() {
  f<Test>(10);  // Call #1.
  f<int>(10);   // Call #2. Without error (even though there is no int::foo)
                // thanks to SFINAE.
}
```

Although SFINAE was initially introduced to avoid creating ill-formed programs when unrelated template declarations were visible (e.g., through the inclusion of a header file), many developers later found the behavior useful for **compile-time introspection**. Specifically, it allows a template to determine certain properties of its **template arguments** at instantiation time.

> NOTE: **compile-time introspection**是c++20的[concept](https://en.cppreference.com/w/cpp/language/constraints)所要解决的。

```c++
#include <iostream>

template <typename T>
struct has_typedef_foobar {
  // Types "yes" and "no" are guaranteed to have different sizes,
  // specifically sizeof(yes) == 1 and sizeof(no) == 2.
  typedef char yes[1];
  typedef char no[2];

  template <typename C>
  static yes& test(typename C::foobar*);

  template <typename>
  static no& test(...);

  // If the "sizeof" of the result of calling test<T>(nullptr) is equal to
  // sizeof(yes), the first overload worked and T has a nested type named
  // foobar.
  static const bool value = sizeof(test<T>(nullptr)) == sizeof(yes);
};

struct foo {
  typedef float foobar;
};

int main() {
  std::cout << std::boolalpha;
  std::cout << has_typedef_foobar<int>::value << std::endl;  // Prints false
  std::cout << has_typedef_foobar<foo>::value << std::endl;  // Prints true
}
```

> NOTE: 编译`g++ --std=c++11 test.cpp`

When `T` has the nested type `foobar` defined, the instantiation of the first `test` works and the null pointer constant is successfully passed. (And the resulting type of the expression is `yes`.) If it does not work, the only available function is the second `test`, and the resulting type of the expression is `no`. An ellipsis is used not only because it will accept any argument, but also because its conversion rank is lowest, so a call to the first function will be preferred if it is possible; this removes ambiguity.

> NOTE: 这个技巧需要好好学习

### C++11 simplification

In [C++11](https://en.wikipedia.org/wiki/C%2B%2B11), the above code could be simplified to:

```c++
#include <iostream>
#include <type_traits>

template <typename... Ts>
using void_t = void;

template <typename T, typename = void>
struct has_typedef_foobar : std::false_type {};

template <typename T>
struct has_typedef_foobar<T, void_t<typename T::foobar>> : std::true_type {};

struct foo {
  using foobar = float;
};

int main() {
  std::cout << std::boolalpha;
  std::cout << has_typedef_foobar<int>::value << std::endl;
  std::cout << has_typedef_foobar<foo>::value << std::endl;
}
```

> NOTE: 编译`g++ --std=c++11 test.cpp`，这段代码和上一段代码的输出是不同的，这段代码都是输出的`true`。



## cppreference [sfinae](https://en.cppreference.com/w/cpp/language/sfinae)



## SFINAE and `enable_if`



## SFINAE and constructor



## SFINAE and member function



## SFINAE and type trait



## sfinae and `static_assert`



## TO READ



https://www.bfilipek.com/2016/02/notes-on-c-sfinae.html



https://www.fluentcpp.com/2018/05/18/make-sfinae-pretty-2-hidden-beauty-sfinae/


https://www.modernescpp.com/index.php/c-20-concepts-the-details


### concept and sfinae
https://stackoverflow.com/questions/28133118/will-concepts-replace-sfinae
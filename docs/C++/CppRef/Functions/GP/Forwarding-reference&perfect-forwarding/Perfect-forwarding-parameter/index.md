# Perfect forwarding

参考素材:

thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c) 

stackoverflow [Advantages of using forward](https://stackoverflow.com/questions/3582001/advantages-of-using-forward) # [A](https://stackoverflow.com/a/3582313) 

## TODO

写一个综合展示perfect forwarding的例子，需要注释清楚哪里涉及forwarding reference、哪里涉及reference collapsing



## 内容梳理

### What is perfect forwarding

- 在文章thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c)中结合具体的例子对perfect forwarding问题进行了较好的介绍，可以作为入门读物；

- stackoverflow [Advantages of using forward](https://stackoverflow.com/questions/3582001/advantages-of-using-forward) # [A](https://stackoverflow.com/a/3582313): 

  > Why is this useful? Because combined we maintain the ability to keep track of the value category of a type: if it was an **lvalue**, we have an **lvalue-reference parameter**, otherwise we have an **rvalue-reference parameter**.

- open-std jtc1 [The Forwarding Problem: Arguments](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2002/n1385.htm)

### Pre C++11 perfect forwarding的困难

Perfect forwarding是一直以来困扰C++ programmer的问题，在如下文章中进行了概括:

- thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c) 的“The perfect forwarding problem”段中对它进行了说明
- stackoverflow [Advantages of using forward](https://stackoverflow.com/questions/3582001/advantages-of-using-forward) # [A](https://stackoverflow.com/a/3582313) 

下面是基于这两篇文章的简短概括:

1 pass-by-value:

```c++
template <typename T1, typename T2>
void wrapper(T1 e1, T2 e2) {
    func(e1, e2);
}
```

存在的问题是: 参数`e1`、`e2` 是pass-by-value，因此函数 `func` 接收到的参数不是原本的object。

2 pass-by-reference:

```c++
template <typename T1, typename T2>
void wrapper(T1& e1, T2& e2) {
    func(e1, e2);
}
```

存在的问题是: 参数`e1`、`e2` 是pass-by-reference，它们无法bind到rvalue，比如下面的程序无法编译通过:

```c++
wrapper(42, 3.14f);                  // error: invalid initialization of
                                     //        non-const reference from
                                     //        an rvalue

wrapper(i, foo_returning_float());   // same error
```



3 brute-force

穷举 `const` 和 non-`const` references的所有可能组合的overload:

```c++
template <typename T1, typename T2>
void wrapper(T1& e1, T2& e2)                { func(e1, e2); }

template <typename T1, typename T2>
void wrapper(const T1& e1, T2& e2)          { func(e1, e2); }

template <typename T1, typename T2>
void wrapper(T1& e1, const T2& e2)          { func(e1, e2); }

template <typename T1, typename T2>
void wrapper(const T1& e1, const T2& e2)    { func(e1, e2); }
```

存在的问题: 指数爆炸导致可能无法穷举。

显然，对于这种困难，需要在语言层面进行解决，这是下部分的内容。

### C++11 perfect forwarding的实现

C++11在语言层面对perfect forwarding进行实现，它主要依赖于如下两个方面：

| C++11新特性                                        | application                      |
| -------------------------------------------------- | -------------------------------- |
| Reference collapsing(折叠) rule                    | 主要用于实现`std::forward`       |
| Special type deduction rules for rvalue references | 主要用于实现forwarding reference |

> NOTE: 关于"special rules for [template argument deduction](https://en.cppreference.com/w/cpp/language/template_argument_deduction) when `T&&` is used in a function template"，在cppreference [Template argument deduction](https://en.cppreference.com/w/cpp/language/template_argument_deduction) 中进行了介绍

上述两者，在文章thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c) 的"Reference collapsing and special type deduction for rvalues"中进行了总结；

实现perfect forwarding: perfect forwarding = forwarding reference + `std::forward`；



## Examples

- `emplace_back`，在thegreenplace [Perfect forwarding and universal references in C++](https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c/)中，将它作为引言
- [`std::make_unique`](http://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique)
- [`std::make_shared`](http://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared) 

### 一般形式

[What are the main purposes of using std::forward and which problems it solves?](https://stackoverflow.com/questions/3582001/what-are-the-main-purposes-of-using-stdforward-and-which-problems-it-solves)

```cpp
template <typename T1, typename T2>
void outer(T1&& t1, T2&& t2) 
{
    inner(std::forward<T1>(t1), std::forward<T2>(t2));
}
```



### [variadic templates](http://eli.thegreenplace.net/2014/variadic-templates-in-c/) and perfect forwarding 

TODO

### cpppatterns [Perfect forwarding](https://cpppatterns.com/patterns/perfect-forwarding.html)



```c++
#include <iostream>
#include <utility>

template<typename T, typename U>
std::pair<T, U> make_pair_wrapper(T&& t, U&& u)
{
	return std::make_pair(std::forward<T>(t),
			std::forward<U>(u));
}

int main()
{
	auto p = make_pair_wrapper(1, 2);
	std::cout << p.first << " " << p.second << std::endl;
}
// g++ --std=c++11  test.cpp
```



## draft

下面是非常好的例子:

### stackoverflow [How to bind function to an object by reference?](https://stackoverflow.com/questions/26187192/how-to-bind-function-to-an-object-by-reference) # A

**Note on using `std::forward`**

First of all, `std::forward` is meant to be used for [perfect forwarding](https://stackoverflow.com/q/3582001/873025), i.e. to forward the reference type (l-value or r-value).

If you pass an *l-value* reference to `std::forward` that is what is returned, and likewise if an *r-value* reference is passed then an r-value is returned. This works as opposed to `std::move` that will always return an r-value reference. Also remember that *named* r-value references are l-value references.

```c++
/* Function that takes r-value reference. */
void func(my_type&& t) {
    // 't' is named and thus is an l-value reference here.

    // Pass 't' as lvalue reference.
    some_other_func(t);
    // Pass 't' as rvalue reference (forwards rvalueness).
    some_other_func(std::forward<my_type>(t));
    // 'std::move' should be used instead as we know 't' is always an rvalue.
    // e.g. some_other_func(std::move(t));
}
```
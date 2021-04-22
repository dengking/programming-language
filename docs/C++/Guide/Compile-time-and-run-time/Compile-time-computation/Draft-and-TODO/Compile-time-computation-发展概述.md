# compile time computation发展概述

## compile time computation发展概述



## `constexpr`

### `constexpr` and `const` and compile time computation

### `constexpr` VS `const`

stackoverflow [Difference between `constexpr` and `const`](https://stackoverflow.com/questions/14116003/difference-between-constexpr-and-const)

> NOTE: 解答的非常详细



### stackoverflow [When should you use constexpr capability in C++11?](https://stackoverflow.com/questions/4748083/when-should-you-use-constexpr-capability-in-c11)

### 我的理解

const 是 常量

constexpr compile time computation、constant expression、充分发挥compile time computation

### `constexpr`发展史

#### C++14

1、stackoverflow [Difference between `constexpr` and `const`](https://stackoverflow.com/questions/14116003/difference-between-constexpr-and-const) # [A](https://stackoverflow.com/a/14117121)

> As of C++14, the rules are more relaxed, what is allowed since then inside a constexpr function: `asm` declaration, a `goto` statement, a statement with a label other than `case` and `default`, try-block, the definition of a variable of non-literal type, definition of a variable of static or thread storage duration, the definition of a variable for which no initialization is performed.
2、wikipedia [C++14](https://en.wikipedia.org/wiki/C%2B%2B14) # [Relaxed constexpr restrictions](https://en.wikipedia.org/wiki/C%2B%2B14#Relaxed_constexpr_restrictions)





#### C++20

cppreference [C++20](https://en.cppreference.com/w/cpp/20)

> further relaxed constexpr

具体参见原文。

## C++20 [`consteval`](https://en.cppreference.com/w/cpp/language/consteval), [`constinit`](https://en.cppreference.com/w/cpp/language/constinit)




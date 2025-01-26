# `auto&&`

`auto&&`解决的是函数的返回值问题。

## cppreference [Reference declaration](https://en.cppreference.com/w/cpp/language/reference) # Forwarding references (since C++11)

参见 `C++11-forwarding-reference` 章节。

**Forwarding references** are a special kind of references that preserve the **value category** of a function argument, making it possible to *forward* it by means of [std::forward](https://en.cppreference.com/w/cpp/utility/forward). 

`auto&&` except when deduced from a **brace-enclosed initializer list** or, when representing a template parameter of a class template during [class template argument deduction](https://en.cppreference.com/w/cpp/language/class_template_argument_deduction)(since C++17):

```c++
#include <vector>  // std::vector

std::vector<int> foo() { return {1, 2, 3, 4}; }
std::vector<int> f() { return {1, 2, 3, 4}; }

template <class T>
int g(T&& x) {  // x is a forwarding reference
  return 0;
}

int main() {
  auto&& vec = foo();                   // foo() may be lvalue or rvalue, vec is a forwarding reference
  auto i = std::begin(vec);             // works either way
  (*i)++;                               // works either way
  g(std::forward<decltype(vec)>(vec));  // forwards, preserving value category

  for (auto&& x : f()) {
    // x is a forwarding reference; this is the safest way to use range for loops
  }

  auto&& z = {1, 2, 3};  // *not* a forwarding reference (special case for initializer lists)
}

// g++ --std=c++11 test.cpp
```

## Q: 使用 `auto&&` 的优势?

A: 在 stackoverflow [What does auto&& tell us?](https://stackoverflow.com/questions/13230480/what-does-auto-tell-us) # [A](https://stackoverflow.com/a/13242177) 中对这个问题进行了很好的回答

By using `auto&& var = <initializer>` you are saying: 

**I will accept any initializer regardless of whether it is an lvalue or rvalue expression and I will preserve its constness**. 

This is typically used for *forwarding* (usually with `T&&`). The reason this works is because a "universal reference", `auto&&` or `T&&`, will bind to *anything*.

### `const auto&` 

> NOTE: 这段话主要在讨论使用 `const auto&` 的劣势

You might say, well why not just use a `const auto&` because that will *also* bind to anything? The problem with using a `const` reference is that it's `const`! You won't be able to later bind it to any **non-const references** or invoke any member functions that are not marked `const`.

As an example, imagine that you want to get a `std::vector`, take an iterator to its first element and modify the value pointed to by that iterator in some way:

```cpp
auto&& vec = some_expression_that_may_be_rvalue_or_lvalue;
auto i = std::begin(vec);
(*i)++;
```

This code will compile just fine regardless of the initializer expression. 

### Alternatives to `auto&&` 

> NOTE: 这些alternatives各有劣势，相比之下 `auto&&` 最优

The alternatives to `auto&&` fail in the following ways:

```cpp
auto         => will copy the vector, but we wanted a reference
auto&        => will only bind to modifiable lvalues
const auto&  => will bind to anything but make it const, giving us const_iterator
const auto&& => will bind only to rvalues
```

So for this, `auto&&` works perfectly! An example of using `auto&&` like this is in a range-based `for` loop. See [my other question](https://stackoverflow.com/q/13241108/150634) for more details.

> NOTE: 上述  [my other question](https://stackoverflow.com/q/13241108/150634) 所链接的是 "stackoverflow [Why does a range-based for statement take the range by auto&&?](https://stackoverflow.com/questions/13241108/why-does-a-range-based-for-statement-take-the-range-by-auto)"

### use `std::forward` on your `auto&&` 

If you then use `std::forward` on your `auto&&` reference to preserve the fact that it was originally either an lvalue or an rvalue, your code says: **Now that I've got your object from either an lvalue or rvalue expression, I want to preserve whichever valueness it originally had so I can use it most efficiently - this might invalidate it.** As in:

```cpp
auto&& var = some_expression_that_may_be_rvalue_or_lvalue;
// var was initialized with either an lvalue or rvalue, but var itself
// is an lvalue because named rvalues are lvalues
use_it_elsewhere(std::forward<decltype(var)>(var));
```

This allows `use_it_elsewhere` to rip its guts out for the sake of performance (avoiding copies) when the original initializer was a modifiable rvalue.

> NOTE:  "rip its guts out"的字面意思是 "把它的内脏掏出来"，引申义为: move
>

What does this mean as to whether we can or when we can steal(偷窃) resources from `var`? Well since the `auto&&` will bind to anything, we cannot possibly try to rip out `var`s guts ourselves - it may very well be an lvalue or even const. We can however `std::forward` it to other functions that may totally ravage(毁坏) its insides. As soon as we do this, we should consider `var` to be in an invalid state.

###  `auto&& var = foo();`

Now let's apply this to the case of `auto&& var = foo();`, as given in your question, where `foo` returns a `T` by value. In this case we know for sure that the type of `var` will be deduced(推导) as `T&&`. Since we know for certain(肯定) that it's an **rvalue**, we don't need `std::forward`'s permission to steal its resources. In this specific case, *knowing that `foo` returns by value*, the reader should just read it as: **I'm taking an rvalue reference to the temporary returned from `foo`, so I can happily move from it.**

> NOTE: temporary是可以被move的

------

### 一个验证`auto&&`的例子

As an addendum(补充), I think it's worth mentioning when an expression like `some_expression_that_may_be_rvalue_or_lvalue` might turn up(出现), other than a "well your code might change" situation. So here's a contrived(人为的) example:

```cpp
#include <iostream>
#include <vector>

std::vector<int> global_vec{1, 2, 3, 4};

template <typename T>
T get_vector() {
  return global_vec;
}

template <typename T>
void foo() {
  auto &&vec = get_vector<T>();
  auto i = std::begin(vec);
  ++(*i);
  std::cout << vec[0] << std::endl;
}

int main() {
  foo<std::vector<int>>();
  std::cout << global_vec[0] << std::endl;
  foo<std::vector<int> &>();
  std::cout << global_vec[0] << std::endl;
}

// g++ test.cpp --std=c++11 -pedantic -Wall -Wextra -g

/**
输出如下:
2
1
2
2
**/
```

> NOTE: 
>
> 上述例子通过抽象函数 `get_vector` 的返回值类型 `T` 来控制function call expression `get_vector<T>()` 是**lvalue expression**还是**rvalue expression**，无论如何，`auto&& vec` 都能够bind。
>
> 对于 `foo<std::vector<int>>()`， `auto&&` 是rvalue reference，原因是这种情况下 `get_vector<std::vector<int>>` 是return-by-value，它相当于复制了一份 `global_vec`，此时它是"**rvalue expression**"，下面对此进行了分析。

Here, `get_vector<T>()` is that lovely expression that could be either an **lvalue** or **rvalue** depending on the generic type `T`. We essentially change the **return type** of `get_vector` through the template parameter of `foo`.

When we call `foo<std::vector<int>>`, `get_vector` will return `global_vec` by value, which gives an **rvalue expression**. Alternatively, when we call `foo<std::vector<int>&>`, `get_vector` will return `global_vec` by reference, resulting in an **lvalue expression**.

If you were to change the `auto&&` in the code to any of `auto`, `auto&`, `const auto&`, or `const auto&&` then we won't get the result we want.

------

An alternative way to change program logic based on whether your `auto&&` reference is initialised with an lvalue or rvalue expression is to use type traits:

```cpp
if (std::is_lvalue_reference<decltype(var)>::value) {
  // var was initialised with an lvalue expression
} else if (std::is_rvalue_reference<decltype(var)>::value) {
  // var was initialised with an rvalue expression
}
```
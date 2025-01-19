# `auto&&`

`auto&&`解决的是函数的返回值。

## cppreference [Reference declaration](https://en.cppreference.com/w/cpp/language/reference) # Forwarding references (since C++11)

参见 `C++11-forwarding-reference` 章节。

## stackoverflow [What does auto&& tell us?](https://stackoverflow.com/questions/13230480/what-does-auto-tell-us)



### [A](https://stackoverflow.com/a/13242177)

By using `auto&& var = <initializer>` you are saying: 

**I will accept any initializer regardless of whether it is an lvalue or rvalue expression and I will preserve its constness**. 

This is typically used for *forwarding* (usually with `T&&`). The reason this works is because a "universal reference", `auto&&` or `T&&`, will bind to *anything*.

#### `const auto&` 

You might say, well why not just use a `const auto&` because that will *also* bind to anything? The problem with using a `const` reference is that it's `const`! You won't be able to later bind it to any non-const references or invoke any member functions that are not marked `const`.

As an example, imagine that you want to get a `std::vector`, take an iterator to its first element and modify the value pointed to by that iterator in some way:

```cpp
auto&& vec = some_expression_that_may_be_rvalue_or_lvalue;
auto i = std::begin(vec);
(*i)++;
```

This code will compile just fine regardless of the initializer expression. The alternatives to `auto&&` fail in the following ways:

```cpp
auto         => will copy the vector, but we wanted a reference
auto&        => will only bind to modifiable lvalues
const auto&  => will bind to anything but make it const, giving us const_iterator
const auto&& => will bind only to rvalues
```

So for this, `auto&&` works perfectly! An example of using `auto&&` like this is in a range-based `for` loop. See [my other question](https://stackoverflow.com/q/13241108/150634) for more details.

#### use `std::forward` on your `auto&&` 

If you then use `std::forward` on your `auto&&` reference to preserve the fact that it was originally either an lvalue or an rvalue, your code says: **Now that I've got your object from either an lvalue or rvalue expression, I want to preserve whichever valueness it originally had so I can use it most efficiently - this might invalidate it.** As in:

```cpp
auto&& var = some_expression_that_may_be_rvalue_or_lvalue;
// var was initialized with either an lvalue or rvalue, but var itself
// is an lvalue because named rvalues are lvalues
use_it_elsewhere(std::forward<decltype(var)>(var));
```

This allows `use_it_elsewhere` to rip its guts out for the sake of performance (avoiding copies) when the original initializer was a modifiable rvalue.

> NOTE: 
>
> "rip its guts out"的字面意思是 "把它的内脏掏出来"，引申义为: move

What does this mean as to whether we can or when we can steal resources from `var`? Well since the `auto&&` will bind to anything, we cannot possibly try to rip out `var`s guts ourselves - it may very well be an lvalue or even const. We can however `std::forward` it to other functions that may totally ravage(毁坏) its insides. As soon as we do this, we should consider `var` to be in an invalid state.

####  `auto&& var = foo();`

Now let's apply this to the case of `auto&& var = foo();`, as given in your question, where `foo` returns a `T` by value. In this case we know for sure that the type of `var` will be deduced as `T&&`. Since we know for certain that it's an **rvalue**, we don't need `std::forward`'s permission to steal its resources. In this specific case, *knowing that `foo` returns by value*, the reader should just read it as: **I'm taking an rvalue reference to the temporary returned from `foo`, so I can happily move from it.**

> NOTE: temporary是可以被move的

------

As an addendum(补充), I think it's worth mentioning when an expression like `some_expression_that_may_be_rvalue_or_lvalue` might turn up, other than a "well your code might change" situation. So here's a contrived example:

```cpp
std::vector<int> global_vec{1, 2, 3, 4};

template <typename T>
T get_vector()
{
  return global_vec;
}

template <typename T>
void foo()
{
  auto&& vec = get_vector<T>();
  auto i = std::begin(vec);
  (*i)++;
  std::cout << vec[0] << std::endl;
}
```

Here, `get_vector<T>()` is that lovely expression that could be either an lvalue or rvalue depending on the generic type `T`. We essentially change the return type of `get_vector` through the template parameter of `foo`.

When we call `foo<std::vector<int>>`, `get_vector` will return `global_vec` by value, which gives an rvalue expression. Alternatively, when we call `foo<std::vector<int>&>`, `get_vector` will return `global_vec` by reference, resulting in an lvalue expression.

If we do:

```cpp
foo<std::vector<int>>();
std::cout << global_vec[0] << std::endl;
foo<std::vector<int>&>();
std::cout << global_vec[0] << std::endl;
```

We get the following output, as expected:

```cpp
2
1
2
2
```

> NOTE: 
>
> 完整测试程序如下:
>
> ```C++
> #include<vector>
> #include<iostream>
> 
> std::vector<int> global_vec { 1, 2, 3, 4 };
> 
> template<typename T>
> T get_vector()
> {
> 	return global_vec;
> }
> 
> template<typename T>
> void foo()
> {
> 	auto &&vec = get_vector<T>();
> 	auto i = std::begin(vec);
> 	(*i)++;
> 	std::cout << vec[0] << std::endl;
> }
> int main()
> {
> 	foo<std::vector<int>>();
> 	std::cout << global_vec[0] << std::endl;
> 	foo<std::vector<int>&>();
> 	std::cout << global_vec[0] << std::endl;
> }
> // g++ test.cpp --std=c++11 -pedantic -Wall -Wextra -g
> 
> ```
>
> 

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
# 引言
今日在阅读LLVM相关源码时（如下所示），遇到了enable_if<>这个概念，以前从没有遇到过，这里做个小记。

```
/*----------llvm/include/llvm/ADT/Hashing.h------------*/
/// \brief Compute a hash_code for any integer value.
///
/// Note that this function is intended to compute the same hash_code for
/// a particular value without regard to the pre-promotion type. This is in
/// contrast to hash_combine which may produce different hash_codes for
/// differing argument types even if they would implicit promote to a common
/// type without changing the value.

template <typename T>
typename std::enable_if<is_integral_or_enum<T>::value, hash_code>::type
hash_value(T value);//此函数的返回值是typename std::enable_if<is_integral_or_enum<T>::value, hash_code>::type
```
enable_if 的主要作用就是当某个 condition 成立时，enable_if可以提供某种类型。enable_if在标准库中通过结构体模板实现的，声明如下：

```
template<bool Cond, class T = void> struct enable_if;
```
参考解释：

[std::enable_if](http://www.cplusplus.com/reference/type_traits/enable_if/)

但是当 condition 不满足的时候，enable_if<>::type 就是未定义的，当用到模板相关的场景时，只会 instantiate fail，并不会编译错误，这时很重要的一点。为了理解 std::enable_if<> 的实现，我们先来了解一下 SFINAE。

# SFINAE
SFINAE 是C++ 的一种语言属性，具体内容就是”从一组重载函数中删除模板实例化无效的函数”。
> Prune functions that do not yield valid template instantiations from a set of overload functions.

SFINAE 的的全称是 Substitution Failure Is Not An Error。

SFINAE 应用最为广泛的场景是C++中的 std::enable_if，这里有完整的[英文描述](https://en.wikibooks.org/wiki/More_C++_Idioms/SFINAE)：
> In the process of template argument deduction, a C++ compiler attempts to instantiate signatures of a number of candidate overloaded functions to make sure that exactly one overloaded function is available as a perfect match for a given function call.

从上面的描述中我们可以看到，在对一个函数调用进行模板推导时，编译器会尝试推导所有的候选函数（重载函数，模板，但是普通函数的优先级要高），以确保得到一个最完美的匹配。

> If an invalid argument or return type is formed during the instantiation of a function template, the instantiation is removed from the overload resolution set instead of causing a compilation error. 
As long as there is one and only one function to which the call can be dispatched, the compiler issues no errors.

也就是说在推导的过程中，如果出现了无效的模板参数，则会将该候选函数从重载决议集合中删除，只要最终得到了一个 perfect match ，编如下代码所示：译就不会报错。(这段话非常重要)

如下代码所示：

```
long multiply(int i, int j) { return i * j; }

template <class T>
typename T::multiplication_result multiply(T t1, T t2)
{
    return t1 * t2;
}

int main(void)
{
    multiply(4, 5);
}
```

main 函数调用 multiply 会使编译器会尽可能去匹配所有候选函数，虽然第一个 multiply 函数明显是较优的匹配，但是为了得到一个最精确的匹配，编译器依然会尝试去匹配剩下的候选函数，此时就会去推导 第二个multiply函数，中间在参数推导的过程中出现了一个无效的类型 int::multiplication_result ，但是因为 SFINAE 原则并不会报错。

# std::enable_if<> 的实现
前面我们在介绍 std::enable_if 的时候提到，如果 condition 不满足的话，会生成一个无效的类型，此处由于SFINAE 机制的存在，只要 call 存在一个匹配的话，就不会报错（只是简单的丢弃该函数）。

std::enable_if<>的实现机制如下代码所示：

```
template<bool Cond, typename T = void> struct enable_if {};

template<typename T> struct enable_if<true, T> { typedef T type; };
```

从上面的代码可以看到：
在 condition 为真的时候，由于偏特化机制，第二个结构体模板明显是一个更好的匹配，所以 std::enable_if<>::type 就是有效的。

当 condition 为假的时候，只有第一个结构体模板能够匹配，所以std::enable_if<>::type 是无效的，会被丢弃，编译器会报错：error: no type named ‘type’ in ‘struct std::enable_if<false, bool>。

# std::enable_if<> 使用
该例子来自于[这里](http://www.cplusplus.com/reference/type_traits/enable_if/)：

```
// enable_if example: two ways of using enable_if
#include <iostream>
#include <type_traits>

// 1. the return type (bool) is only valid if T is an integral type:
template <class T>
typename std::enable_if<std::is_integral<T>::value,bool>::type
  is_odd (T i) {return bool(i%2);}

// 2. the second template argument is only valid if T is an integral type:
template < class T,
           class = typename std::enable_if<std::is_integral<T>::value>::type>
bool is_even (T i) {return !bool(i%2);}

int main() {

  short int i = 1;    // code does not compile if type of i is not integral

  std::cout << std::boolalpha;
  std::cout << "i is odd: " << is_odd(i) << std::endl;
  std::cout << "i is even: " << is_even(i) << std::endl;

  return 0;
}
```




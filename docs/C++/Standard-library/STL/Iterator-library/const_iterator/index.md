# `const_iterator`

在学习`cbegin`和`cend`的时候，发现了`const_iterator`。

## What's new in C++?

从下面的文章来看，C++11引入`const_iterator`的目的是:

保证type safety: const-correctness。

### 以pointer来进行类比

参见 stackoverflow [What is the difference between const_iterator and non-const iterator in the C++ STL?](https://stackoverflow.com/questions/309581/what-is-the-difference-between-const-iterator-and-non-const-iterator-in-the-c) # [A](https://stackoverflow.com/a/309610)，下面收录了，简而言之:

| pointer    | iterator                         | explanation                             |
| ---------- | -------------------------------- | --------------------------------------- |
| `const T*` | `std::vector<T>::const_iterator` | A non-const iterator to a const element |

 即: 等价于 pointer to  const。

### stackoverflow [What is the reason behind cbegin/cend?](https://stackoverflow.com/questions/12001410/what-is-the-reason-behind-cbegin-cend)

I wonder why `cbegin` and `cend` were introduced in C++11?

What are cases when calling these methods makes a difference from const overloads of `begin` and `end`?

[A](https://stackoverflow.com/a/12001519)

It's quite simple. Say I have a vector:

```cpp
std::vector<int> vec;
```

I fill it with some data. Then I want to get some iterators to it. Maybe pass them around. Maybe to [`std::for_each`](https://en.cppreference.com/w/cpp/algorithm/for_each) :

```cpp
std::for_each(vec.begin(), vec.end(), SomeFunctor());
```

In C++03, `SomeFunctor` was free to be able to *modify* the parameter it gets. Sure, `SomeFunctor` could take its parameter by value or by `const&`, but there's no way to *ensure* that it does. Not without doing something silly like this:

> NOTE: 让compiler保证能够检查出违反type safety的情况，这是C++的发展发现之一，其实这是在type safety方向的演进。

```cpp
const std::vector<int> &vec_ref = vec;
std::for_each(vec_ref.begin(), vec_ref.end(), SomeFunctor());
```

Now, we introduce `cbegin/cend`:

```cpp
std::for_each(vec.cbegin(), vec.cend(), SomeFunctor());
```

Now, we have syntactic assurances that `SomeFunctor` cannot modify the elements of the vector (without a const-cast, of course). We explicitly get `const_iterator`s, and therefore `SomeFunctor::operator()` will be called with `const int &`. If it takes it's parameters as `int &`, C++ will issue a compiler error.

> NOTE: 
>
> ```C++
> #include <iostream>
> #include <vector>
> #include <algorithm>
> void SomeFunctor(int& i)
> {
> 	std::cout << i << std::endl;
> }
> int main()
> {
> 	std::vector<int> vec { 1, 2, 3 };
> 	const std::vector<int> &vec_ref = vec;
> 	std::for_each(vec_ref.begin(), vec_ref.end(), SomeFunctor);
> }
> // g++ --std=c++11 test.cpp
> 
> ```
>
> 上述代码编译报错如下:
>
> ```C++
> In file included from /usr/include/c++/4.8.2/algorithm:62:0,
>                  from test.cpp:3:
> /usr/include/c++/4.8.2/bits/stl_algo.h: In instantiation of ‘_Funct std::for_each(_IIter, _IIter, _Funct) [with _IIter = __gnu_cxx::__normal_iterator<const int*, std::vector<int> >; _Funct = void (*)(int&)]’:
> test.cpp:12:59:   required from here
> /usr/include/c++/4.8.2/bits/stl_algo.h:4417:14: error: invalid initialization of reference of type ‘int&’ from expression of type ‘const int’
>   __f(*__first);
> 
> ```
>
> 从上述代码可以看出，函数调用所执行的是:
>
> ```C++
> __f(*__first);
> ```
>
> 由于`*first`的type是`const int`，而函数 `SomeFunctor(int& i)` 的入参是 `int&`，显然这是违背 const-correctness 的，这就导致了compiler error。
>
> 需要注意的是，pass-by-value是不违背的:
>
> ```C++
> #include <iostream>
> #include <vector>
> #include <algorithm>
> void SomeFunctor(int i)
> {
> 	std::cout << i << std::endl;
> }
> int main()
> {
> 	std::vector<int> vec { 1, 2, 3 };
> 	const std::vector<int> &vec_ref = vec;
> 	std::for_each(vec_ref.begin(), vec_ref.end(), SomeFunctor);
> }
> // g++ --std=c++11 test.cpp
> 
> ```
>
> 上述代码是编译通过的。

------

C++17 has a more elegant solution to this problem: [`std::as_const`](http://en.cppreference.com/w/cpp/utility/as_const). Well, at least it's elegant when using range-based `for`:

```cpp
for(auto &item : std::as_const(vec))
```

This simply returns a `const&` to the object it is provided.

@NicolBolas is `for(auto &item : std::as_const(vec))` equivalent to `for(const auto &item : vec)`? – [luizfls](https://stackoverflow.com/users/4942129/luizfls) [Oct 18 '17 at 20:34](https://stackoverflow.com/questions/12001410/what-is-the-reason-behind-cbegin-cend#comment80584910_12001519)

@luizfls Yes. Your code says the item will not be modified by putting the `const` on the reference. Nicol's views the container as const, so `auto` deduces a `const` reference. IMO `auto const& item` is easier and clearer. It's unclear why `std::as_const()` is good here; I can see it'd be useful when passing something non-`const` to generic code where we can't control the type that gets used, but with range-`for`, we can, so it just seems like added noise to me there. – [underscore_d](https://stackoverflow.com/users/2757035/underscore-d) [Sep 24 '18 at 21:00](https://stackoverflow.com/questions/12001410/what-is-the-reason-behind-cbegin-cend#comment91915829_12001519) 

### stackoverflow [What is the difference between const_iterator and non-const iterator in the C++ STL?](https://stackoverflow.com/questions/309581/what-is-the-difference-between-const-iterator-and-non-const-iterator-in-the-c)



[A](https://stackoverflow.com/a/309589)

`const_iterator`s don't allow you to change the values that they point to, regular `iterator`s do.

As with all things in C++, always prefer `const`, unless there's a good reason to use regular iterators (i.e. you want to use the fact that they're not `const` to change the pointed-to value).



[A](https://stackoverflow.com/a/309610)

> NOTE: 使用point来进行类比，非常容易理解。

They should pretty much be self-explanatory. If iterator points to an element of type `T`, then const_iterator points to an element of type '`const T`'.

It's basically equivalent to the pointer types:

```c++
T* // A non-const iterator to a non-const element. Corresponds to std::vector<T>::iterator
T* const // A const iterator to a non-const element. Corresponds to const std::vector<T>::iterator
const T* // A non-const iterator to a const element. Corresponds to std::vector<T>::const_iterator
```

| pointer    | iterator                         | explanation                                 |
| ---------- | -------------------------------- | ------------------------------------------- |
| `T*`       | `std::vector<T>::iterator`       | A non-const iterator to a non-const element |
| `T* const` | `const std::vector<T>::iterator` | A const iterator to a non-const element     |
| `const T*` | `std::vector<T>::const_iterator` | A non-const iterator to a const element     |

A **const iterator** always points to the same element, so the iterator *itself* is const. But the element it points to does not have to be const, so the element it points to can be changed. 

A `const_iterator` is an iterator that points to a **const element**, so while the iterator itself can be updated (incremented or decremented, for example), the element it points to can not be changed.



### stackoverflow [What is the difference between cbegin and begin for vector?](https://stackoverflow.com/questions/31208640/what-is-the-difference-between-cbegin-and-begin-for-vector)

The member [begin](http://www.cplusplus.com/reference/vector/vector/begin/) has two overloadings one of them is `const_iterator begin() const;`. There's also the [cbegin](http://www.cplusplus.com/reference/vector/vector/cbegin/) `const_iterator cbegin() const noexcept;`. Both of them returns `const_iterator` to the begin of a list. What's the difference?

[A](https://stackoverflow.com/a/31208681)

`begin` will return an `iterator` or a `const_iterator` depending on the const-qualification of the object it is called on.

`cbegin` will return a `const_iterator` unconditionally.

```C++
std::vector<int> vec;
const std::vector<int> const_vec;

vec.begin(); //iterator
vec.cbegin(); //const_iterator

const_vec.begin(); //const_iterator
const_vec.cbegin(); //const_iterator
```



## 如何获得？

通过 `cbegin`、`cend`。
# Iterate and apply to tuple



## stackoverflow [How can you iterate over the elements of an std::tuple?](https://stackoverflow.com/questions/1198260/how-can-you-iterate-over-the-elements-of-an-stdtuple)



### [A](https://stackoverflow.com/a/6894436)

I have an answer based on [Iterating over a Tuple](https://stackoverflow.com/questions/4832949/c-iterating-over-a-tuple-resolution-of-type-vs-constant-parameters):

```cpp
#include <tuple>
#include <utility>
#include <iostream>

template<std::size_t I = 0, typename ... Tp>
inline typename std::enable_if<I == sizeof...(Tp), void>::type
print(std::tuple<Tp...> &t)
{
}

template<std::size_t I = 0, typename ... Tp>
inline typename std::enable_if<I < sizeof...(Tp), void>::type 
print(std::tuple<Tp...> &t)
{
	std::cout << std::get<I>(t) << std::endl;
	print<I + 1, Tp...>(t);
}

int main()
{
	typedef std::tuple<int, float, double> T;
	T t = std::make_tuple(2, 3.14159F, 2345.678);

	print(t);
}
// g++ test.cpp --std=c++11 -pedantic -Wall -Wextra

```

> NOTE: 
>
> 1、输出如下:
>
> ```C++
> 2
> 3.14159
> 2345.68
> ```
>
> 

The usual idea is to use compile time recursion. In fact, this idea is used to make a printf that is type safe as noted in the original tuple papers.

This can be easily generalized into a `for_each` for tuples:

```cpp
#include <tuple>
#include <utility>
#include <iostream>
#include <tuple>
#include <utility>

template<std::size_t I = 0, typename FuncT, typename ... Tp>
inline typename std::enable_if<I == sizeof...(Tp), void>::type
for_each(std::tuple<Tp...>&, FuncT) // Unused arguments are given no names.
{
}

template<std::size_t I = 0, typename FuncT, typename ... Tp>
inline typename std::enable_if<I < sizeof...(Tp), void>::type
for_each(std::tuple<Tp...> &t, FuncT f)
{
	f(std::get<I>(t));
	for_each<I + 1, FuncT, Tp...>(t, f);
}

int main()
{
	typedef std::tuple<int, float, double> T;
	T t = std::make_tuple(2, 3.14159F, 2345.678);
	for_each(t, [](auto& i){std::cout<<i<<std::endl;});
}
// g++ test.cpp --std=c++14 -pedantic -Wall -Wextra

```

Though this then requires some effort to have `FuncT` represent something with the appropriate overloads for every type the tuple might contain. This works best if you know all the tuple elements will share a common base class or something similar.

> NOTE: 
>
> 1、这可能需要template lambda



### [A](https://stackoverflow.com/a/54053084)

In C++17, you can use [`std::apply`](https://en.cppreference.com/w/cpp/utility/apply) with [fold expression](https://en.cppreference.com/w/cpp/language/fold):

```cpp
std::apply([](auto&&... args) {((/* args.dosomething() */), ...);}, the_tuple);
```

A complete example for printing a tuple:

```cpp
#include <tuple>
#include <iostream>

int main()
{
    std::tuple t{42, 'a', 4.2}; // Another C++17 feature: class template argument deduction
    std::apply([](auto&&... args) {((std::cout << args << '\n'), ...);}, t);
}
```

[[Online Example on Coliru\]](http://coliru.stacked-crooked.com/a/0f98350719456d66)

This solution solves the issue of evaluation order in [M. Alaggan's answer](https://stackoverflow.com/a/37100178/5376789).
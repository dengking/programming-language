# csdn [C++14 make_index_sequence原理](https://blog.csdn.net/dboyguan/article/details/51706357)

## 功能

实现编译期的整数序列，如下例`make_index_sequence<3>()`会使`fun`函数的模板参数: `int... N` 推演为：`0，1，2`序列 ：


```C++
#include <iostream>
#include <tuple>

using namespace std;

template<int ... N>
decltype(auto) fun(index_sequence<N...> is)
{
	return make_tuple(N...);
}

int main()
{
	auto t = fun(make_index_sequence<3>());
	cout << std::get<0>(t) << endl;
	cout << std::get<1>(t) << endl;
	cout << std::get<2>(t) << endl;
	return 0;
}
// g++ --std=c++17 test.cpp -Wall -pedantic

```
> NOTE: 
>
> 1、上述程序编译报错如下:
>
> ```C++
> test.cpp: In function ‘int main()’:
> test.cpp:14:39: error: no matching function for call to ‘fun(std::make_index_sequence<3>)’
>   auto t = fun(make_index_sequence<3>());
>                                        ^
> test.cpp:7:16: note: candidate: ‘template<int ...N> decltype(auto) fun(std::index_sequence<N ...>)’
>  decltype(auto) fun(index_sequence<N...> is)
>                 ^~~
> test.cpp:7:16: note:   template argument deduction/substitution failed:
> test.cpp:14:39: note:   mismatched types ‘int’ and ‘long unsigned int’
>   auto t = fun(make_index_sequence<3>());
> ```
>
> 上述‘long unsigned int’其实对应的是`std:size_t`
>
> 2、`std::index_sequence`的type如下:
>
> ```C++
> template<std::size_t... Ints>
> using index_sequence = std::integer_sequence<std::size_t, Ints...>;
> ```
>
> 3、下面是正确的版本:
>
> ```C++
> #include <iostream>
> #include <tuple>
> 
> using namespace std;
> 
> template<size_t ... N>
> decltype(auto) fun(index_sequence<N...> is)
> {
> 	return make_tuple(N...);
> }
> 
> int main()
> {
> 	auto t = fun(make_index_sequence<3>());
> 	cout << std::get<0>(t) << endl;
> 	cout << std::get<1>(t) << endl;
> 	cout << std::get<2>(t) << endl;
> 	return 0;
> }
> // g++ --std=c++17 test.cpp -Wall -pedantic
> 
> ```
>
> 输出如下:
>
> ```C++
> 0
> 1
> 2
> ```
>
> 

## 实现分析

### 傻瓜式实现

即是生成一个可变的编译期整数序列，实际上目的就是要生成如下类的对象：

```C++
template<int...N>
struct index_sequence {};
```

定义出上面的`index_sequence`后，其实就可以使用了，仍利用`fun`函数，调用方式为：

```C++
auto t = fun(index_sequence<0,1,2>());
```

实际上就是一个**空类**，因为我们的目的只是模板的**可变参数**（英文直译应叫模板参数包），并生成一个递增的整数序列，这种实现实际什么都不需要做。

> NOTE: 
>
> 1、这段话是理解`index_sequence`用法的关键: 我们要使用的是class template的**可变参数**，而不是class template本身。
>
> 3、`make_index_sequence`的template parameter指定的是sequence的长度，`make_index_sequence`相当于一个sequence generator。

### 更好的实现

分析，c++14的实现的`make_index_sequence`模板只需指定一个模板参数 “N”，就会自动生成整数序列：`0 - N-1`，从接口来看：

`make_index_sequence<4>` 可以实现为`index_sequence`的子类或，一个函数，并返回`index_sequence<0,1,2...>`对象。

> NOTE: 
>
> 1、下面就按照上述的两种思路来展开

### 1、继承方式实现

因c++14已定义`index_sequence`，为避免符号冲突，使用`index_seq`代替。

```C++
template<int ... N>
struct index_seq
{
};

/**
 * @brief 递归函数
 *
 * @tparam N
 * @tparam M
 */
template<int N, int ...M>
struct make_index_seq: public make_index_seq<N - 1, N - 1, M...>
{

};
/**
 * @brief 递归终止条件
 *
 * @tparam M
 */
template<int ...M>
struct make_index_seq<0, M...> : public index_seq<M...>
{

};

```

> NOTE: 
> 1、上述实现方式是典型的recursive variadic class template
>
> 2、递归函数如下:
>
> seq(0) = 0
>
> seq(N) =  N-1 + seq(N-1) (N > 1)

实现的原理是，当给定一个整数`N`，如3，定义`make_index_seq<3>()` 对象时，模板可变参数`M`，由空逐渐推导为序列`0，1，2`。

即`make_index_seq<3>` 时，M 为空。

1、`make_index_seq<3-1,3-1, M...>`时，M 为3-1 = 2

2、`make_index_seq<2-1,2-1,M...>`时，M为2-1=1，2 即序列（1，2）

3、`make_index_seq<1-1,1-1,M...>`时，M为1-1 = 0， 1，2即序列（0，1，2）

4、`make_index_seq<0,M...>`时，M为（0，1，2）此时，`make_index_seq<3>`实际继承自`index_seq<0,1,2>`

这样就生成了编译期的整数序列。


#### 测试

```C++
#include <iostream>
#include <tuple>
using namespace std;

template<int ... N>
struct index_seq
{
};

/**
 * @brief 递归函数
 *
 * @tparam N
 * @tparam M
 */
template<int N, int ...M>
struct make_index_seq: public make_index_seq<N - 1, N - 1, M...>
{

};
/**
 * @brief 递归终止条件
 *
 * @tparam M
 */
template<int ...M>
struct make_index_seq<0, M...> : public index_seq<M...>
{

};

template<int ... N>
decltype(auto) fun(index_seq<N...> is)
{
	return make_tuple(N...);
}

int main()
{
	auto t = fun(make_index_seq<3>());
	cout << std::get<0>(t) << endl;
	cout << std::get<1>(t) << endl;
	cout << std::get<2>(t) << endl;
	return 0;
}
// g++ --std=c++14 test.cpp -Wall -pedantic


```

输出如下:

```C++
0
1
2
```

符合预期。

### 2、函数方式实现

对于使用函数方式还不知道是否可行，整体思路的代码如下：

```C++
#include <type_traits>

template<int ... M>
void make_index_seq_impl(std::true_type t)
{

}

template<int N, int ... M>
void make_index_seq_impl(std::false_type t)
{

	(N < 1) ? (make_index_seq_impl<M...>(std::true_type())) : (make_index_seq_impl<N - 1, N - 1, M...>(std::false_type()));
}

```

此种写法是不可以的，会导编译器编译时，模板的无穷递归。

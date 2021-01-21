# `std::tuple`



## cppreference [std::tuple](https://en.cppreference.com/w/cpp/utility/tuple)

```C++
template< class... Types >
class tuple;
```



## Implementation

### `gcc/libstdc++-v3`

https://code.woboq.org/gcc/libstdc++-v3/include/std/tuple.html



## Named tuple

http://vitiy.info/named-tuple-for-cplusplus/







# [Converting Tuple to string](https://stackoverflow.com/questions/23436406/converting-tuple-to-string)



## [A](https://stackoverflow.com/a/23436849)

Since this question is tagged C++11, here's my take at it:

```cpp
#include <iostream>
#include <string>
#include <tuple>

template<typename T, T...>
struct integer_sequence { };

template<std::size_t N, std::size_t... I>
struct gen_indices : gen_indices<(N - 1), (N - 1), I...> { };
template<std::size_t... I>
struct gen_indices<0, I...> : integer_sequence<std::size_t, I...> { };

template<typename H>
std::string& to_string_impl(std::string& s, H&& h)
{
  using std::to_string;
  s += to_string(std::forward<H>(h));
  return s;
}

template<typename H, typename... T>
std::string& to_string_impl(std::string& s, H&& h, T&&... t)
{
  using std::to_string;
  s += to_string(std::forward<H>(h));
  return to_string_impl(s, std::forward<T>(t)...);
}

template<typename... T, std::size_t... I>
std::string to_string(const std::tuple<T...>& tup, integer_sequence<std::size_t, I...>)
{
  std::string result;
  int ctx[] = { (to_string_impl(result, std::get<I>(tup)...), 0), 0 };
  (void)ctx;
  return result;
}

template<typename... T>
std::string to_string(const std::tuple<T...>& tup)
{
  return to_string(tup, gen_indices<sizeof...(T)>{});
}

int main(int argc, char** argv)
{
  std::tuple<int, double, float> tup(1, 2.1, 3.2);
  std::cout << to_string(tup) << std::endl;
}
```

If you want to stick with boost::lexical_cast, replace to_string with lexical_cast.

Live output on [ideone](http://ideone.com/ffws3P)

***20190313*** : 经过测试，上述代码是存在缺陷的，比如`std::tuple<std::string, std::string, float> tup("hello ", "world", 3.2)`，使用上述代码就会编译报错。

# [How can you iterate over the elements of an std::tuple?](https://stackoverflow.com/questions/1198260/how-can-you-iterate-over-the-elements-of-an-stdtuple)
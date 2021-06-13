# cppreference [std::data](https://en.cppreference.com/w/cpp/iterator/data)

## Possible implementation

### First version

```
template<class C>
constexpr auto data(C &c) -> decltype(c.data())
{
	return c.data();
}


```

> NOTE: 
>
> 测试程序:
>
> ```C++
> #include <vector>
> #include <cassert>
> template<class C>
> constexpr auto data(C &c) -> decltype(c.data())
> {
> 	return c.data();
> }
> 
> int main()
> {
> 	std::vector<int> v { 1, 2, 3 };
> 	int *begin = data(v);
> 	assert(begin == v.data());
> }
> // g++ test.cpp --std=c++11 -pedantic -Wall -Wextra
> 
> ```
>
> 

### Second version

```C++
template <class C> 
constexpr auto data(const C& c) -> decltype(c.data())
{
    return c.data();
}

```



### Third version



> NOTE: 
>
> 测试程序:
>
> ```C++
> #include <vector>
> #include <cassert>
> template<class T, std::size_t N>
> constexpr T* data(T (&array)[N]) noexcept
> {
> 	return array;
> }
> 
> int main()
> {
> 	int v[] { 1, 2, 3 };
> 	int *begin = data(v);
> 	assert(begin == v);
> }
> // g++ test.cpp --std=c++11 -pedantic -Wall -Wextra
> 
> ```
>
> 

### Fourth version

```c++
template <class E> 
constexpr const E* data(std::initializer_list<E> il) noexcept
{
    return il.begin();
}
```


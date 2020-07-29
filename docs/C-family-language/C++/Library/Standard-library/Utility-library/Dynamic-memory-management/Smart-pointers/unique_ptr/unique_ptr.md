# `unique_ptr`



## cppreference [`std::unique_ptr`](https://en.cppreference.com/w/cpp/memory/unique_ptr) 



### [std::unique_ptr<T,Deleter>::release](https://en.cppreference.com/w/cpp/memory/unique_ptr/release) and [std::unique_ptr<T,Deleter>::reset](https://en.cppreference.com/w/cpp/memory/unique_ptr/reset)

> NOTE: 
>
> release: returns a pointer to the managed object and **releases the ownership**
>
> reset: Replaces the managed object.

> NOTE:  `unique_ptr` release and delete the owned object：
>
> ```c++
> u_ptr.reset(nullptr);
> ```
>
> 



## Implementation

> NOTE: 很多问题，当阅读它的实现的时候，就非常任意理解了。

### [gcc](https://github.com/gcc-mirror/gcc)/[libstdc++-v3](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3)/[include](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include)/[bits](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include/bits)/[unique_ptr.h](https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/unique_ptr.h)



## `std::make_unique`

### [gcc](https://github.com/gcc-mirror/gcc)/[libstdc++-v3](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3)/[include](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include)/[bits](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include/bits)/[unique_ptr.h](https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/unique_ptr.h) 

```c++
#if __cplusplus >= 201402L
  /// @relates unique_ptr @{
#define __cpp_lib_make_unique 201304

  /// @cond undocumented

  template<typename _Tp>
    struct _MakeUniq
    { typedef unique_ptr<_Tp> __single_object; };

  template<typename _Tp>
    struct _MakeUniq<_Tp[]>
    { typedef unique_ptr<_Tp[]> __array; };

  template<typename _Tp, size_t _Bound>
    struct _MakeUniq<_Tp[_Bound]>
    { struct __invalid_type { }; };

  /// @endcond

  /// std::make_unique for single objects
  template<typename _Tp, typename... _Args>
    inline typename _MakeUniq<_Tp>::__single_object
    make_unique(_Args&&... __args)
    { return unique_ptr<_Tp>(new _Tp(std::forward<_Args>(__args)...)); }

  /// std::make_unique for arrays of unknown bound
  template<typename _Tp>
    inline typename _MakeUniq<_Tp>::__array
    make_unique(size_t __num)
    { return unique_ptr<_Tp>(new remove_extent_t<_Tp>[__num]()); }

  /// Disable std::make_unique for arrays of known bound
  template<typename _Tp, typename... _Args>
    inline typename _MakeUniq<_Tp>::__invalid_type
    make_unique(_Args&&...) = delete;
  // @} relates unique_ptr
#endif // C++14
```



### `make_unique` support for pre c++14

https://github.com/gabime/spdlog/blob/v1.x/include/spdlog/common.h

```c++
#include <type_traits> // std::is_array
#include <memory> // std::unique_ptr
// make_unique support for pre c++14

#if __cplusplus >= 201402L // C++14 and beyond
using std::make_unique;
#else
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args &&... args)
{
    static_assert(!std::is_array<T>::value, "arrays not supported");
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif
```







## `std::unique_ptr<void>`

[Should std::unique_ptr be permitted](https://stackoverflow.com/questions/19840937/should-stdunique-ptrvoid-be-permitted)

https://stackoverflow.com/questions/39288891/why-is-shared-ptrvoid-legal-while-unique-ptrvoid-is-ill-formed





## std::unique_ptr return from function and test for null

[C++ std::unique_ptr return from function and test for null](https://stackoverflow.com/questions/30293338/c-stdunique-ptr-return-from-function-and-test-for-null)

[`std::unique_ptr<T,Deleter>::operator bool`](https://en.cppreference.com/w/cpp/memory/unique_ptr/operator_bool) 



## Exmaple

```c++
#include <iostream>
#include <memory> // for std::shared_ptr

class Resource
{
public:
	Resource()
	{
		std::cout << "Resource acquired\n";
	}
	~Resource()
	{
		std::cout << "Resource destroyed\n";
	}
};

class Test
{
public:
	Test()
	{
		InitR();
	}
	void InitR()
	{
		Resource* R = new Resource();
		m_R = std::unique_ptr<Resource>(R);
	}
private:
	std::unique_ptr<Resource> m_R;
};

int main()
{
	Test t;
}
```
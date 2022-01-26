# [gcc](https://github.com/gcc-mirror/gcc)/[libstdc++-v3](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3)/[include](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include)/[std](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include/std)/[any](https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/std/any)

```C
  [[gnu::noreturn]] inline void __throw_bad_any_cast()
  {
#if __cpp_exceptions
    throw bad_any_cast{};
#else
    __builtin_abort();
#endif
  }
```

## storage



```C++
// Holds either pointer to a heap object or the contained object itself.
union _Storage
{
    constexpr _Storage() : _M_ptr{nullptr} {}

    // Prevent trivial copies of this type, buffer might hold a non-POD.
    _Storage(const _Storage&) = delete;
    _Storage& operator=(const _Storage&) = delete;

    void* _M_ptr; // pointer to the heap object
    aligned_storage<sizeof(_M_ptr), alignof(void*)>::type _M_buffer; // 这是small buffer optimization
};
```

上述写法其实是可以作为一个通用的small object optimization。

```C++
#include <type_traits>
// Holds either pointer to a heap object or the contained object itself.
union Storage
{
	constexpr Storage() : ptr_{ nullptr } {}

	// Prevent trivial copies of this type, buffer might hold a non-POD.
	Storage(const Storage&) = delete;
	Storage& operator=(const Storage&) = delete;

	void* ptr_; // pointer to the heap object
	std::aligned_storage<sizeof(ptr_), alignof( void* )>::type buffer_; // 这是small buffer optimization
};
template<
	typename _Tp, // contained object的类型
	// 为了执行small object optimization，_Tp 必须是nothrow move constructible的
	typename _Safe = std::is_nothrow_move_constructible<_Tp>,
	// 为了执行small object optimization，_Tp 的size、align等都必须要满足要求
	bool _Fits = ( sizeof(_Tp) <= sizeof(_Storage) ) && ( alignof( _Tp ) <= alignof( _Storage ) )
>
using Internal = std::integral_constant<bool, _Safe::value&& _Fits>; // metafunction: 判断能否使用small object optimization
int main()
{

}
```



## `Manager`



```C++
template<
	typename _Tp, // contained object的类型
	// 为了执行small object optimization，_Tp 必须是nothrow move constructible的
	typename _Safe = is_nothrow_move_constructible<_Tp>,
	// 为了执行small object optimization，_Tp 的size、align等都必须要满足要求
	bool _Fits = ( sizeof(_Tp) <= sizeof(_Storage) ) && ( alignof( _Tp ) <= alignof( _Storage ) )
>
using _Internal = std::integral_constant<bool, _Safe::value&& _Fits>; // metafunction: 判断能否使用small object optimization

template<typename _Tp>
struct _Manager_internal; // uses small-object optimization

template<typename _Tp>
struct _Manager_external; // creates contained object on the heap

// 由 _Internal<_Tp>::value 来决定使用internal还是external
template<typename _Tp>
using _Manager = __conditional_t<_Internal<_Tp>::value,
	_Manager_internal<_Tp>,
	_Manager_external<_Tp>
>;
```

一、由`_Manager`对`_Storage`进行管理

二、可以看到，下面的两种manager有如下共性:

1、模板参数都是 `_Tp`

2、成员函数都是 static

### `struct _Manager_internal`

```C++
// Manage in-place contained object.
template<typename _Tp>
struct _Manager_internal
{
	static void _S_manage(_Op __which, const any* __anyp, _Arg* __arg);

	/// 在 __storage 中构造contained object
	template<typename _Up>
	static void
		_S_create(_Storage& __storage, _Up&& __value)
	{
		void* __addr = &__storage._M_buffer;
		::new ( __addr ) _Tp(std::forward<_Up>(__value)); // 典型的placement new
	}

	/// 在 __storage 中构造contained object
	template<typename... _Args>
	static void
		_S_create(_Storage& __storage, _Args&&... __args)
	{
		void* __addr = &__storage._M_buffer;
		::new ( __addr ) _Tp(std::forward<_Args>(__args)...); // 典型的placement new
	}

	static _Tp*
		_S_access(const _Storage& __storage)
	{
		// The contained object is in __storage._M_buffer
		const void* __addr = &__storage._M_buffer;
		return static_cast<_Tp*>( const_cast<void*>( __addr ) );
	}
};

```



### `struct _Manager_external`

```C++
// Manage external contained object.
template<typename _Tp>
struct _Manager_external
{
	static void _S_manage(_Op __which, const any* __anyp, _Arg* __arg);

	template<typename _Up>
	static void
		_S_create(_Storage& __storage, _Up&& __value)
	{
		__storage._M_ptr = new _Tp(std::forward<_Up>(__value));
	}
	template<typename... _Args>
	static void
		_S_create(_Storage& __storage, _Args&&... __args)
	{
		__storage._M_ptr = new _Tp(std::forward<_Args>(__args)...);
	}
	static _Tp*
		_S_access(const _Storage& __storage)
	{
		// The contained object is in *__storage._M_ptr
		return static_cast<_Tp*>( __storage._M_ptr );
	}
};
```



## 成员变量

1、`_Storage _M_storage;`

2、`void (*_M_manager)(_Op, const any*, _Arg*);`

`_M_manager` 是pointer to function



## `emplace`


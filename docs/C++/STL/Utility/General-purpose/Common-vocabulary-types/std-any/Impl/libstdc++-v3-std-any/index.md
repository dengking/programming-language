# [gcc](https://github.com/gcc-mirror/gcc)/[libstdc++-v3](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3)/[include](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include)/[std](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include/std)/[any](https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/std/any)



## stackoverflow [std::any without RTTI, how does it work?](https://stackoverflow.com/questions/51361606/stdany-without-rtti-how-does-it-work)

```cpp
#include <iostream>
#include <any>

int main()
{
	std::any x;
	x = 9.9;
	std::cout << std::any_cast<double>( x ) << std::endl;
}
// g++ -std=c++17 -O2 -Wall -pedantic  test.cpp && ./a.out

```

But how `std::any` stores the type information? As I see, if I call `std::any_cast` with the "wrong" type I got `std::bad_any_cast` exception as expected.

> NOTE: 
>
> 非常神奇，相当于将类型存储起来了，然后运行时在使用。但是它的implementation可以不使用RTTI

### comment

Boost has its own typeinfo that replaces RTTI, that's why `boost::any` does not need it. Generally I do not see other possibility than implementing one's own typeinfo that does not depend on RTTI – [bartop](https://stackoverflow.com/users/3414900/bartop)

> NOTE: 
>
> boost type_info source here: [github.com/boostorg/core/blob/develop/include/boost/core/…](https://github.com/boostorg/core/blob/develop/include/boost/core/typeinfo.hpp) enjoy :)

### [A](https://stackoverflow.com/a/51362647/10173843)

`std::any` holds a pointer to a static member function of a templated class. This function can perform many operations and is specific to a given type since the actual instance of the function depends on the template arguments of the class.

> NOTE: 
>
> compile-time constant

Basically, `std::any` holds two things:

1、A pointer to a (dynamically) allocated storage;

2、A pointer to a "storage manager function":

```C++
void (*_M_manager)(_Op, const any*, _Arg*);
```

When you construct or assign a new `std::any` with an object of type `T`, `_M_manager` points to a function specific to the type `T` (which is actually a **static member function** of class specific to `T`):

```C++
template <typename _ValueType, 
          typename _Tp = _Decay<_ValueType>,
          typename _Mgr = _Manager<_Tp>, // <-- Class specific to T.
          __any_constructible_t<_Tp, _ValueType&&> = true,
          enable_if_t<!__is_in_place_type<_Tp>::value, bool> = true>
any(_ValueType&& __value)
  : _M_manager(&_Mgr::_S_manage) { /* ... */ }
```

Since this function is specific to a given type, you don't need RTTI to perform the operations required by `std::any`.

Furthermore, it is easy to check that you are casting to the right type within `std::any_cast`. Here is the core of the gcc implementation of `std::any_cast`:

```cpp
template<typename _Tp>
void* __any_caster(const any* __any) {
    if constexpr (is_copy_constructible_v<decay_t<_Tp>>) {
        if (__any->_M_manager == &any::_Manager<decay_t<_Tp>>::_S_manage) {
            any::_Arg __arg;
            __any->_M_manager(any::_Op_access, __any, &__arg);
            return __arg._M_obj;
        }
    }
    return nullptr;
}
```

You can see that it is simply an equality check between the stored function inside the object you are trying to cast (`_any->_M_manager`) and the manager function of the type you want to cast to (`&any::_Manager<decay_t<_Tp>>::_S_manage`).

> NOTE: 
>
> 一、实际上就是pointer comparision。

------

The class `_Manager<_Tp>` is actually an alias to either `_Manager_internal<_Tp>` or `_Manager_external<_Tp>` depending on `_Tp`. This class is also used for allocation / construction of object for the `std::any` class.



## 成员变量

1、`_Storage _M_storage;`

2、`void (*_M_manager)(_Op, const any*, _Arg*);`

`_M_manager` 就是在 stackoverflow [std::any without RTTI, how does it work?](https://stackoverflow.com/questions/51361606/stdany-without-rtti-how-does-it-work) # [A](https://stackoverflow.com/a/51362647/10173843) 中提及的"A pointer to a "storage manager function""



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

上述写法其实是可以作为一个通用的small object optimization，下面是测试程序:

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

一、由`_Manager`对`_Storage`进行管理

二、可以看到，下面的两种manager有如下共性:

1、模板参数都是 `_Tp`

2、成员函数都是 static

3、它典型的C-style OOP: 第一个参数的类型都是`_Storage`

三、它采用的是典型的pointer to instantiation of function template，"instantiation of function template"记住了具体的类型信息，这种做法是非常常见的

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

`__conditional_t` 可以实现static polymorphism: 根据 `_Tp` 来选择具体的implementation。

```C++
enum _Op {
	_Op_access, 
    _Op_get_type_info,
    _Op_clone, 
    _Op_destroy, 
    _Op_xfer // transfer，主要用于move
};

union _Arg
{
	void* _M_obj;
	const std::type_info* _M_typeinfo;
	any* _M_any;
};
```

主要是对"contained object"进行管理。

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

## `_Decay_if_not_any`

为什么要使用 `_Decay_if_not_any`？

`std::any` 是要能够存储任何类型的object的，它通过template constructor、template assignment 来实现；在这些它支持的类型中还包括它自己`std::any`，但是这两种情况的处理逻辑是不同的，因此需要通过overload进行区分。简而言之: 便于区分overload、template method:

1、template constructor、template assignment 

2、constructor with argument `std::any`、assignment with argument `std::any`



## Constructors

### Default constructor

```C++
/// Default constructor, creates an empty object.
constexpr any() noexcept : _M_manager(nullptr) { }
```

### Copy constructor

```C++
/// Copy constructor, copies the state of @p __other
any(const any& __other)
{
    if (!__other.has_value())
        _M_manager = nullptr;
    else
    {
        _Arg __arg;
        __arg._M_any = this;
        __other._M_manager(_Op_clone, &__other, &__arg);
    }
}
```

在 `_M_manager(_Op_clone, &__other, &__arg)` 中，会调用copy constructor。



### Move constructor

```C++
/**
     * @brief Move constructor, transfer the state from @p __other
     *
     * @post @c !__other.has_value() (this postcondition is a GNU extension)
     */
any(any&& __other) noexcept
{
    if (!__other.has_value())
        _M_manager = nullptr;
    else
    {
        _Arg __arg;
        __arg._M_any = this;
        __other._M_manager(_Op_xfer, &__other, &__arg);
    }
}
```



### Constructor: copy contained object

```C++
template<typename _Tp, typename _VTp = decay_t<_Tp>>
using _Decay_if_not_any = enable_if_t<!is_same_v<_VTp, any>, _VTp>;

/// Construct with a copy of @p __value as the contained object.
template <
    typename _Tp, 
    typename _VTp = _Decay_if_not_any<_Tp>,
    typename _Mgr = _Manager<_VTp>,
    typename = _Require<
                    __not_<__is_in_place_type<_VTp>>,
                    is_copy_constructible<_VTp>
                >
    >
    any(_Tp&& __value)
    : _M_manager(&_Mgr::_S_manage)
{
    _Mgr::_S_create(_M_storage, std::forward<_Tp>(__value));
}
```

一、典型的SFIANE

二、`_Require` 是 concept

### Constructor: construct contained object in-place

```C++
template <typename _Res, typename _Tp, typename... _Args>
using __any_constructible
= enable_if<
	__and_<
	    is_copy_constructible<_Tp>,
	    is_constructible<_Tp, _Args...>
	>::value,
	_Res
>;

template <typename _Tp, typename... _Args>
using __any_constructible_t = typename __any_constructible<bool, _Tp, _Args...>::type;

/// Construct with an object created from @p __args as the contained object.
template <
	typename _Tp, 
	typename... _Args, 
	typename _VTp = decay_t<_Tp>,
	typename _Mgr = _Manager<_VTp>,
	__any_constructible_t<_VTp, _Args&&...> = false
>
explicit any(in_place_type_t<_Tp>, _Args&&... __args)
	: _M_manager(&_Mgr::_S_manage)
{
	_Mgr::_S_create(_M_storage, std::forward<_Args>(__args)...);
}

```

### Constructor: construct contained object in-place、`std::initializer_list`

```C++
/// Construct with an object created from @p __il and @p __args as
/// the contained object.
template <
    typename _Tp, 
    typename _Up, 
    typename... _Args,
    typename _VTp = decay_t<_Tp>, 
    typename _Mgr = _Manager<_VTp>,
    __any_constructible_t<
        _VTp, 
        initializer_list<_Up>&,
        _Args&&...
    > = false>
explicit any(in_place_type_t<_Tp>, initializer_list<_Up> __il, _Args&&... __args)
    : _M_manager(&_Mgr::_S_manage)
{
    _Mgr::_S_create(_M_storage, __il, std::forward<_Args>(__args)...);
}

```

## Assignments



## `std::any_cast`



```C++
template<typename _Tp>
void* __any_caster(const any* __any) {
    if constexpr (is_copy_constructible_v<decay_t<_Tp>>) {
        if (__any->_M_manager == &any::_Manager<decay_t<_Tp>>::_S_manage) { // 函数指针比较
            any::_Arg __arg;
            __any->_M_manager(any::_Op_access, __any, &__arg);
            return __arg._M_obj;
        }
    }
    return nullptr;
}
```





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

### dynamic type safety



## `emplace`


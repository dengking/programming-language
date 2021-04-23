# `std::swap`



## cppreference [std::swap](https://en.cppreference.com/w/cpp/algorithm/swap)



### Swap value 

#### until C++11

```C++
template< class T >
void swap( T& a, T& b );
```



#### since C++11 until C++20

```C++
template< class T >
void swap( T& a, T& b ) noexcept(/* see below */);
```



#### since C++20

```C++
template< class T >
constexpr void swap( T& a, T& b ) noexcept(/* see below */);
```





### Swap array

#### since C++11 until C++20

```C++
template< class T2, std::size_t N >
void swap( T2 (&a)[N], T2 (&b)[N]) noexcept(/* see below */);
```



#### since C++20

```c++
template< class T2, std::size_t N >
constexpr void swap( T2 (&a)[N], T2 (&b)[N]) noexcept(/* see below */);
```



### Type requirements

-`T` must meet the requirements of [*MoveAssignable*](https://en.cppreference.com/w/cpp/named_req/MoveAssignable) and [*MoveConstructible*](https://en.cppreference.com/w/cpp/named_req/MoveConstructible).

-`T2` must meet the requirements of [*Swappable*](https://en.cppreference.com/w/cpp/named_req/Swappable).

> NOTE: 下面有程序进行验证



### Specializations

> NOTE:
>
> 1、`std` 中，提供了`std::swap`的各种overload，因此`std::swap` 的参数类型如果是 `std` type 的话，是能够调用到这些overload
>
> 2、对于user defined type，采用`Non-throwing-swap-idiom` 和 `Swappable-concept-and-swap-value-idiom`
>
> 通过ADL来找到user-defined swap。

#### 不推荐的方式

`std::swap` may be [specialized in namespace std](https://en.cppreference.com/w/cpp/language/extending_std) for program-defined types, but such specializations are not found by [ADL](https://en.cppreference.com/w/cpp/language/adl) (the namespace std is not the associated namespace for the program-defined type). (until C++20)

> NOTE: 
>
> 1、在 `More-C++Idioms-Non-throwing-swap` 中演示了这种写法，即添加一个function template的full specialization，这种方式是不好的，需要些更多的code。

#### 推荐的方式

The expected way to make a program-defined type swappable is to provide a non-member function `swap` in the same namespace as the type: see [*Swappable*](https://en.cppreference.com/w/cpp/named_req/Swappable) for details.

> NOTE: 
>
> 1、这就是 `Non-throwing-swap-idiom` 和 `Swappable-concept-and-swap-value-idiom`

#### Overloads in STL

The following overloads are already provided by the standard library:

> NOTE: 
>
> 1、STL是采用的`Non-throwing-swap-idiom`的，下面的这些overload的function parameter type提供了各自高效的member `swap` 和 non-member function `swap` overload





## `std::swap` 要求  [*MoveAssignable*](https://en.cppreference.com/w/cpp/named_req/MoveAssignable) and [*MoveConstructible*](https://en.cppreference.com/w/cpp/named_req/MoveConstructible).



### 测试程序

```C++
#include <iostream>
#include <vector>
#include <utility>

class IntVector
{
	std::vector<int> v;
public:
	void swap(IntVector &other)
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		v.swap(other.v);
	}
};
void swap(IntVector &v1, IntVector &v2)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	v1.swap(v2);
}

int main()
{
	using std::swap;
	IntVector v1, v2;
	swap(v1, v2);
}
// g++ --std=c++11 test.cpp -Wall -pedantic

```

上述程序能够正常编译通过，输出如下:

```C++
void swap(IntVector&, IntVector&)
void IntVector::swap(IntVector&)

```





```C++
#include <iostream>
#include <vector>

class IntVector
{
	std::vector<int> v;
	
public:
	void swap(IntVector &other)
	{
		v.swap(other.v);
	}
	IntVector& operator=(const IntVector& v) = default; // not assignable
	IntVector& operator=(IntVector&& v) = delete; // not assignable
};
void swap(IntVector &v1, IntVector &v2)
{
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	v1.swap(v2);
}

int main()
{
	IntVector v1, v2;
    std::swap(v1, v2); // compiler error! std::swap requires MoveAssignable
	// std::iter_swap(&v1, &v2); // OK: library calls unqualified swap()
}
// g++ --std=c++11 test.cpp -Wall -pedantic

```

编译报错如下:

```C++
main.cpp: In function 'int main()':
main.cpp:25:21: error: no matching function for call to 'swap(IntVector&, IntVector&)'
   25 |     std::swap(v1, v2); // compiler error! std::swap requires MoveAssignable

/usr/local/include/c++/10.2.0/bits/move.h:189:5: note: candidate: 'template<class _Tp> std::_Require<std::__not_<std::__is_tuple_like<_Tp> >, std::is_move_constructible<_Tp>, std::is_move_assignable<_Tp> > std::swap(_Tp&, _Tp&)'
  189 |     swap(_Tp& __a, _Tp& __b)
```





## Implementation

### [microsoft](https://github.com/microsoft)/[STL](https://github.com/microsoft/STL)/[stl](https://github.com/microsoft/STL/tree/main/stl)/[inc](https://github.com/microsoft/STL/tree/main/stl/inc)/[utility](https://github.com/microsoft/STL/blob/main/stl/inc/utility)



```C++
// FUNCTION TEMPLATE iter_swap (from <algorithm>)
template <class _FwdIt1, class _FwdIt2>
_CONSTEXPR20 void iter_swap(_FwdIt1 _Left, _FwdIt2 _Right) { // swap *_Left and *_Right
    swap(*_Left, *_Right);
}

// FUNCTION TEMPLATE swap
template <class _Ty, size_t _Size, enable_if_t<_Is_swappable<_Ty>::value, int> _Enabled>
_CONSTEXPR20 void swap(_Ty (&_Left)[_Size], _Ty (&_Right)[_Size]) noexcept(_Is_nothrow_swappable<_Ty>::value) {
    if (&_Left != &_Right) {
        _Ty* _First1 = _Left;
        _Ty* _Last1  = _First1 + _Size;
        _Ty* _First2 = _Right;
        for (; _First1 != _Last1; ++_First1, ++_First2) {
            _STD iter_swap(_First1, _First2);
        }
    }
}

#if _HAS_CXX17
template <class _Ty, enable_if_t<is_move_constructible_v<_Ty> && is_move_assignable_v<_Ty>, int> _Enabled>
#else // ^^^ _HAS_CXX17 / !_HAS_CXX17 vvv
template <class _Ty, int _Enabled>
#endif // _HAS_CXX17
_CONSTEXPR20 void swap(_Ty& _Left, _Ty& _Right) noexcept(
    is_nothrow_move_constructible_v<_Ty>&& is_nothrow_move_assignable_v<_Ty>) {
    _Ty _Tmp = _STD move(_Left);
    _Left    = _STD move(_Right);
    _Right   = _STD move(_Tmp);
}

```



`std::pair` overload

```c++
template <class _Ty1, class _Ty2, enable_if_t<_Is_swappable<_Ty1>::value && _Is_swappable<_Ty2>::value, int> = 0>
_CONSTEXPR20 void swap(pair<_Ty1, _Ty2>& _Left, pair<_Ty1, _Ty2>& _Right) noexcept(noexcept(_Left.swap(_Right))) {
    _Left.swap(_Right);
}

```



### [gcc](https://github.com/gcc-mirror/gcc)/[libstdc++-v3](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3)/[include](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include)/[bits](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include/bits)/[move.h](https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/move.h)



```C++
/**
 *  @brief Swaps two values.
 *  @param  __a  A thing of arbitrary type.
 *  @param  __b  Another thing of arbitrary type.
 *  @return   Nothing.
 */
template<typename _Tp>
_GLIBCXX20_CONSTEXPR
inline
#if __cplusplus >= 201103L
typename enable_if<__and_<__not_<__is_tuple_like<_Tp>>,
is_move_constructible<_Tp>,
is_move_assignable<_Tp>>::value>::type
#else
    void
#endif
swap(_Tp& __a, _Tp& __b)
_GLIBCXX_NOEXCEPT_IF(__and_<is_nothrow_move_constructible<_Tp>,
				is_nothrow_move_assignable<_Tp>>::value)
{
#if __cplusplus < 201103L
      // concept requirements
      __glibcxx_function_requires(_SGIAssignableConcept<_Tp>)
#endif
	_Tp __tmp = _GLIBCXX_MOVE(__a);
	__a = _GLIBCXX_MOVE(__b);
	__b = _GLIBCXX_MOVE(__tmp);
}

// _GLIBCXX_RESOLVE_LIB_DEFECTS
// DR 809. std::swap should be overloaded for array types.
/// Swap the contents of two arrays.
template<typename _Tp, size_t _Nm>
_GLIBCXX20_CONSTEXPR
inline
#if __cplusplus >= 201103L
typename enable_if<__is_swappable<_Tp>::value>::type
#else
    void
#endif
swap(_Tp (&__a)[_Nm], _Tp (&__b)[_Nm])
_GLIBCXX_NOEXCEPT_IF(__is_nothrow_swappable<_Tp>::value)
{
	for (size_t __n = 0; __n < _Nm; ++__n)
	swap(__a[__n], __b[__n]);
}

```



### [llvm-project](https://github.com/llvm/llvm-project)/[libcxx](https://github.com/llvm/llvm-project/tree/main/libcxx)/[include](https://github.com/llvm/llvm-project/tree/main/libcxx/include)/[utility](https://github.com/llvm/llvm-project/blob/main/libcxx/include/utility)



### Summary

stackoverflow [How does the standard library implement std::swap?](https://stackoverflow.com/questions/25286544/how-does-the-standard-library-implement-stdswap) # [A](https://stackoverflow.com/a/25286610)

A more modern (C++11) implementation of `std::swap` looks like this:

```cpp
template<typename T> void swap(T& t1, T& t2) {
    T temp = std::move(t1); // or T temp(std::move(t1));
    t1 = std::move(t2);
    t2 = std::move(temp);
}
```
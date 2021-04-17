# [microsoft](https://github.com/microsoft)/**[GSL](https://github.com/microsoft/GSL)**/[pointers](https://github.com/microsoft/GSL/blob/main/include/gsl/pointers)

`is_comparable_to_nullptr`

```C++
namespace details
{
template<typename T, typename = void>
struct is_comparable_to_nullptr : std::false_type {};

template <typename T>
struct is_comparable_to_nullptr<T, std::enable_if_t<std::is_convertible<decltype(std::declval<T>() != nullptr), bool>::value>> : std::true_type {};
}   // namespace details
```

典型的使用`stdd::declval` + decltype来实现concept，使用`stdd::declval`来描述valid expression。





## `class not_null`

```C++
//
// not_null
//
// Restricts a pointer or smart pointer to only hold non-null values.
//
// Has zero size overhead over T.
//
// If T is a pointer (i.e. T == U*) then
// - allow construction from U*
// - disallow construction from nullptr_t
// - disallow default construction
// - ensure construction from null U* fails
// - allow implicit conversion to U*
//
template<class T>
class not_null
{
public:
	static_assert(details::is_comparable_to_nullptr<T>::value, "T cannot be compared to nullptr.");

	template<typename U, typename = std::enable_if_t<std::is_convertible<U, T>::value>>
	constexpr not_null(U &&u) :
					ptr_(std::forward < U > (u))
	{
		Expects(ptr_ != nullptr);
	}

	template<typename = std::enable_if_t<!std::is_same<std::nullptr_t, T>::value>>
	constexpr not_null(T u) :
					ptr_(std::move(u))
	{
		Expects(ptr_ != nullptr);
	}

	template<typename U, typename = std::enable_if_t<std::is_convertible<U, T>::value>>
	constexpr not_null(const not_null<U> &other) :
					not_null(other.get())
	{
	}

	not_null(const not_null &other) = default;
	not_null& operator=(const not_null &other) = default;
	constexpr std::conditional_t<std::is_copy_constructible<T>::value, T, const T&> get() const
	{
		Ensures(ptr_ != nullptr);
		return ptr_;
	}

	constexpr operator T() const
	{
		return get();
	}
	constexpr decltype(auto) operator->() const
	{
		return get();
	}
	constexpr decltype(auto) operator*() const
	{
		return *get();
	}

	// prevents compilation when someone attempts to assign a null pointer constant
	not_null(std::nullptr_t) = delete;
	not_null& operator=(std::nullptr_t) = delete;

	// unwanted operators...pointers only point to single objects!
	not_null& operator++() = delete;
	not_null& operator--() = delete;
	not_null operator++(int) = delete;
	not_null operator--(int) = delete;
	not_null& operator+=(std::ptrdiff_t) = delete;
	not_null& operator-=(std::ptrdiff_t) = delete;
	void operator[](std::ptrdiff_t) const = delete;

private:
	T ptr_;
};

```

### 一、Coercion by Member Template idiom

```C++
template<typename U, typename = std::enable_if_t<std::is_convertible<U, T>::value>>
constexpr not_null(U &&u) :
ptr_(std::forward < U > (u))
{
	Expects(ptr_ != nullptr);
}

template<typename = std::enable_if_t<!std::is_same<std::nullptr_t, T>::value>>
constexpr not_null(T u) :
ptr_(std::move(u))
{
	Expects(ptr_ != nullptr);
}

template<typename U, typename = std::enable_if_t<std::is_convertible<U, T>::value>>
constexpr not_null(const not_null<U> &other) :
not_null(other.get())
{
}

not_null(const not_null &other) = default;
not_null& operator=(const not_null &other) = default;

```

闲人，它同时提供了两个版本: 一个是非模板版本，一个是模板版本。



### 二 `get()`

下面是对它的implementation的分析:

```C++
	constexpr std::conditional_t<std::is_copy_constructible<T>::value, T, const T&> get() const
	{
		Ensures(ptr_ != nullptr);
		return ptr_;
	}
```

1、它使用的是conditional return type:

如果`T`是copy constructible，则它return-by-value；

如果`T`不是copy constructible，则它return-by-reference；

比如，对于`std::unique_ptr<T>`，由于它不是copy constructible，因此，上述函数的返回值是lvalue reference。

#### stackoverflow [Why gsl::not_null ensures ptr is not null on get()?](https://stackoverflow.com/questions/51157916/why-gslnot-null-ensures-ptr-is-not-null-on-get)

All the constructors of `gsl::not_null` which take possibly pointers check these pointers are not null, but we still check stored value of pointer (`ptr_`) against null on **each** dereference. Why do we have this check, given that in C++ we typically don't pay for what we don't need?

> NOTE: 
>
> 1、"Zero-overhead principle-You don't pay for what you don't use"

[A](https://stackoverflow.com/a/51163107)

The comments are already giving the idea why removing null check from `not_null::get()` is not desirable. The main problem is that the change allows dereferencing a smart pointer after move.

> NOTE:
>
> 1、避免发生dereference null pointer错误

For examples, see the following discussion on a PR that enables usage of `not_null<unique_ptr>` and how the change is incompatible with removing the null check from `not_null::get()`

https://github.com/Microsoft/GSL/pull/675

> NOTE: 
>
> ```C++
> #include<gsl>
> #include<memory>
> 
> int main()
> {
>     gsl::not_null<std::unique_ptr<int>> y{ std::make_unique<int>() };
>     auto z = std::move(y);
>     return *y; // AV here
> }
> // clang --std=c++14 test.cpp -Iinclude/gsl -Iinclude
> 
> ```
>
> 上述程序编译报错如下:
>
> ```C++
> include/gsl/pointers:106:43: error: call to deleted constructor of 'std::unique_ptr<int, std::default_delete<int> >'
>     constexpr operator T() const { return get(); }
>                                           ^~~~~
> include/gsl/pointers:83:38: note: in instantiation of member function 'gsl::not_null<std::unique_ptr<int, std::default_delete<int> > >::operator unique_ptr' requested here
>     constexpr not_null(U&& u) : ptr_(std::forward<U>(u))
>                                      ^
> test.cpp:11:14: note: in instantiation of function template specialization 'gsl::not_null<std::unique_ptr<int, std::default_delete<int> > >::not_null<gsl::not_null<std::unique_ptr<int, std::default_delete<int> > >, void>' requested here
>     auto z = std::move(y);
>              ^
> /usr/bin/../lib/gcc/x86_64-redhat-linux/8/../../../../include/c++/8/bits/unique_ptr.h:389:7: note: 'unique_ptr' has been explicitly marked deleted here
>       unique_ptr(const unique_ptr&) = delete;
> ```
>
> 通过后面的"`gsl::not_null<std::unique_ptr<T>>`"章节的分析可知，目前并不支持`gsl::not_null<std::unique_ptr<int>>`，后面会进行深入的分析。

As for performance concerns, compiler optimizer should be able to remove many of the null checks, but not all, of course. If some checks are not removed but seem to be removable, we should fix compiler optimizations.

> NOTE: 
>
> 1、从我的实践来看，目前并不支持`gsl::not_null<std::unique_ptr<T>>`，因此这有些多余，所以这个作者的回答是有误的
>
> 

### 三、user define conversion method `operator T()`

1、这个user define conversion operator能够支持使用一个`not_null` object来构造它的`T`对象。

2、下面是对它的implementation进行分析



```C++
	constexpr operator T() const
	{
		return get();
	}
```



对于上述函数:

如果`T`是copy constructible，则`get()`会return-by-value，其实就是copy返回一个`T` object；

如果`T`不是copy constructible，则`get()`会return-by-reference，在`operator T()`中，会调用`T`的copy constructor。

因此，从上述来看 `operator T()` 总是copy construct。

因此，如果`T`是moveable-only，那么`class not_null`将不支持它。

3、关于此，参见 cppreference [user-defined conversion function](https://en.cppreference.com/w/cpp/language/cast_operator)。

### 四、copyable but not movable

1、它是典型的copyable but not movable。`operator T()`总是copy construct。

2、从更高的角度来进行分析:

a、github [remove move constructor for not_null. #842](https://github.com/microsoft/GSL/pull/842) 

> Moving not_null will replace whatever pointer is stored within with null, which goes against the purpose of `not_null`.
> This change removes the move constructor for not_null.

**[hsutter](https://github.com/hsutter)** commented [on 5 Feb 2020](https://github.com/microsoft/GSL/pull/842#issuecomment-582074941) 

> I agree that `[strict_]not_null` move construction and move assignment should be `=delete`. Thanks for pointing this out!
>
> Yes, an intended effect of this change is that a `not_null<unique_ptr<T>>` can only sit there, it can't be moved anywhere. But this is already inherently true, moving one of those is impossible today without breaking the `not_null` invariant. The correct long-term answer for this would be if C++ gets something along the lines of the **relocation / destructive move semantics proposals**, where roughly "relocation/destructive-move leaves an object that is guaranteed to be no longer used" or similar (in those proposals, including even its dtor won't be called), then that would naturally enable cases like returning a local `not_null<unique_ptr<T>>` by value.

关于 "**relocation / destructive move semantics proposals**"，参见 

open-std [P1144R2 Object relocation in terms of move plus destroy](http://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1144r2.html)



#### stackoverflow [Is there a legal way to move from gsl::not_null?](https://stackoverflow.com/questions/35420704/is-there-a-legal-way-to-move-from-gslnot-nullt)

The [Guidelines Support Library](https://github.com/Microsoft/GSL) introduced `not_null<T>` who's purpose is to enforce an invariant on pointer-like types, advertently on smart pointers. However [it's a known issue](https://github.com/Microsoft/GSL/issues/89) that `not_null<unique_ptr<T>>` doesn't work.

As far as I see the reason is that `unique_ptr<T>` is not copy-constructible and `not_null<T>` doesn't have a constructor that would move from its `T`. 

> NOTE: 
>
> 1、上面这段话，需要结合`gsl::not_null<std::unique_ptr<T>>`中的例子和它的source code来进行理解: 
>
> "`not_null<T>` doesn't have a constructor that would move from its `T`"，结合前面对source code分析是可以理解这段话的。
>
> 

`not_null<T>` is not default-constructible either because it would break it's **invariant**. Even if we could construct `not_null<unique_ptr<T>>`, it would be impossible to meaningfully reach the `unique_ptr` inside because we couldn't copy `unique_ptr` and moving it would leave `not_null<T>` with a nullptr. It looks like a perfect trap.

> NOTE: 
>
> 1、这在 github [remove move constructor for not_null. #842](https://github.com/microsoft/GSL/pull/842)  中，进行了讨论
>
> 2、需要始终维持class invariant

I argued that we could legally move from a `not_null<T>` object in a specific context: just before it goes out of scope. In other words, moving from it should be the last access before destruction. That way the object with broken invariant wouldn't be observable to the rest of the program. (It would be observable for `not_null`'s own code only.)

> NOTE: 
>
> 1、目前C++在语言层面还不支持这种方式。作者的这段讨论其实github [remove move constructor for not_null. #842](https://github.com/microsoft/GSL/pull/842)  # **[hsutter](https://github.com/hsutter)** commented [on 5 Feb 2020](https://github.com/microsoft/GSL/pull/842#issuecomment-582074941) 中，有着更加深入的分析，上面已经收录了这部分内容
>
> 

In the following examples let's assume that we can move from `not_null<T>`.

```C++
not_null<unique_ptr<int>> f()
{
    return make_unique<int>(1);
}

void g(not_null<unique_ptr<int>> p)
{
    ...
}

void h()
{
    auto p = f();
    g(make_unique<int>(2));
}
```

1、Is my assumption correct that state of the `not_null<unique_ptr<int>>` returned from f() couldn't leak after moving from it (just for the example)?

2、Is my assumption correct that state of the `not_null<unique_ptr<int>>` passed to g() couldn't leak after moving from it (just for the example)?

3、Is it possible to allow this special kind of moving while prohibiting the common case of moving in C++14/17?

**comments**:

Regarding 3: see [github.com/microsoft/GSL/pull/842](https://github.com/microsoft/GSL/pull/842), especially the comment from hsutter. For a general solution move construction in combination with `std::move` is a problem, compare `auto p=f(); auto q=std::move(p);` where after the move `p` cannot hold any meaningful value that does not violate the class invariant "not null" (there is no valid default value to set `p` to). – [Werner Henze](https://stackoverflow.com/users/1023911/werner-henze) Feb 28 '20 at 15:30 

> NOTE: 

## `gsl::not_null<std::unique_ptr<int>>`

### Move

```C++
#include<gsl>
#include<memory>

void test(gsl::not_null<std::unique_ptr<int>> i)
{

}
int main()
{
	gsl::not_null<std::unique_ptr<int>> i { std::unique_ptr<int>(new int(3)) };
	test(std::move(i));
}
// clang --std=c++14 test.cpp -Iinclude/gsl -Iinclude

```

编译报错如下:



```C++
In file included from test.cpp:1:
In file included from include/gsl/gsl:23:
include/gsl/pointers:106:43: error: call to deleted constructor of 'std::unique_ptr<int, std::default_delete<int> >'
    constexpr operator T() const { return get(); }
                                          ^~~~~
include/gsl/pointers:83:38: note: in instantiation of member function 'gsl::not_null<std::unique_ptr<int, std::default_delete<int> > >::operator unique_ptr' requested here
    constexpr not_null(U&& u) : ptr_(std::forward<U>(u))
                                     ^
test.cpp:11:7: note: in instantiation of function template specialization 'gsl::not_null<std::unique_ptr<int, std::default_delete<int> > >::not_null<gsl::not_null<std::unique_ptr<int, std::default_delete<int> > >, void>' requested here
        test(std::move(i));
             ^
/usr/bin/../lib/gcc/x86_64-redhat-linux/8/../../../../include/c++/8/bits/unique_ptr.h:389:7: note: 'unique_ptr' has been explicitly marked deleted here
      unique_ptr(const unique_ptr&) = delete;
      ^
1 error generated.
```

#### 问题: 为什么 `constexpr operator T() const { return get(); }` 会被调用？

`class not_null`提供了如下constructor:

1、`constexpr not_null(U &&u)`

2、`constexpr not_null(T u)`

构造函数 `constexpr not_null(T u)` 的入参是lvalue，且类型需要是class template parameter `T`

3、`constexpr not_null(const not_null<U> &other)`

4、`not_null(const not_null &other)` 

而`test(std::move(i))`的入参类型是rvalue，，且它的入参类型是`gsl::not_null<std::unique_ptr<int>>`，并不是`T`，因此compiler会调用如下constructor:

```C++
template<typename U, typename = std::enable_if_t<std::is_convertible<U, T>::value>>
constexpr not_null(U &&u) :
ptr_(std::forward < U > (u))
{
	Expects(ptr_ != nullptr);
}
```

由于提供了user define conversion method `operator T()`，因此`std::is_convertible<U, T>::value`是true，因此，compiler是会调用user define conversion method`operator T()`来将`gsl::not_null<std::unique_ptr<int>>` 转换为class template parameter `T`，即  `std::unique_ptr<int>`。

在前面的分析中，我们知道: user define conversion method `operator T()` 中，始终调用的是copy constructor，而`std::unique_ptr<int>` 的copy constructor是`delete`的，因此最终会报入手编译报错。



### TODO: Copy

```C++
 
#include<gsl>
#include<memory>

int main()
{
    gsl::not_null<std::unique_ptr<int>> y{ std::make_unique<int>() };
    auto z = y;
    return 0;
}
// clang --std=c++14 test.cpp -Iinclude/gsl -Iinclude

```

编译报错如下:

```C++
In file included from test.cpp:1:
In file included from include/gsl/gsl:23:
include/gsl/pointers:106:43: error: call to deleted constructor of 'std::unique_ptr<int, std::default_delete<int> >'
    constexpr operator T() const { return get(); }
                                          ^~~~~
include/gsl/pointers:83:38: note: in instantiation of member function 'gsl::not_null<std::unique_ptr<int, std::default_delete<int> > >::operator unique_ptr' requested here
    constexpr not_null(U&& u) : ptr_(std::forward<U>(u))
                                     ^
test.cpp:7:14: note: in instantiation of function template specialization 'gsl::not_null<std::unique_ptr<int, std::default_delete<int> > >::not_null<gsl::not_null<std::unique_ptr<int, std::default_delete<int> > > &, void>' requested here
    auto z = y;
             ^
/usr/bin/../lib/gcc/x86_64-redhat-linux/8/../../../../include/c++/8/bits/unique_ptr.h:389:7: note: 'unique_ptr' has been explicitly marked deleted here
      unique_ptr(const unique_ptr&) = delete;
      ^
1 error generated.
```

#### 问题: 为什么没有调用`not_null(const not_null &other) = default;`

从上述编译报错来看，它最终调用的是`constexpr not_null(U &&u)`，这说明在overload resolution中它的排名是在`not_null(const not_null &other) = default;`之前的。
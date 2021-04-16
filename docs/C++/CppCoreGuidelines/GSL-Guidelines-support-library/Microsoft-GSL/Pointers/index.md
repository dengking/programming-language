# [microsoft](https://github.com/microsoft)/**[GSL](https://github.com/microsoft/GSL)**/pointers

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





### stackoverflow [Why gsl::not_null ensures ptr is not null on get()?](https://stackoverflow.com/questions/51157916/why-gslnot-null-ensures-ptr-is-not-null-on-get)





## `gsl::not_null<std::unique_ptr<T>>`

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

#### 问题

为什么 `constexpr operator T() const { return get(); }` 会被调用？
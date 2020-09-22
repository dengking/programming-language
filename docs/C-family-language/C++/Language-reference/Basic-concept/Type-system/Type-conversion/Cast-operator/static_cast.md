# static_cast



## cppreference [static_cast conversion](https://en.cppreference.com/w/cpp/language/static_cast)

Only the following conversions can be done with static_cast, except when such conversions would cast away *constness* or *volatility*.

> NOTE: `static_cast`是需要维护CV的，否则会编译报错

### 1) 

If there is an [implicit conversion sequence](https://en.cppreference.com/w/cpp/language/implicit_cast) from *expression* to *`new_type`*, or if **overload resolution** for a [direct initialization](https://en.cppreference.com/w/cpp/language/direct_initialization) of an object or reference of type *`new_type`* from *`expression`* would find at least one viable（可行的） function, then `static_cast<new_type>(expression)` returns the imaginary（虚构的） variable `Temp` initialized as if by `new_type Temp(expression);`, which may involve [implicit conversions](https://en.cppreference.com/w/cpp/language/implicit_cast), a call to the [constructor](https://en.cppreference.com/w/cpp/language/constructor) of *`new_type`* or a call to a [user-defined conversion operator](https://en.cppreference.com/w/cpp/language/cast_operator). 

For non-reference *`new_type`*, the result object of the `static_cast` prvalue expression is what's direct-initialized. (since C++17)

> NOTE: 上面这段话，`if`、`or if`描述了两种情况下，`static_cast`的行为：`new_type Temp(expression);`
>
> 在Example中，有例子对此进行说明。

### 2) static downcast

> NOTE: 在[Why use static_cast(x) instead of (int)x?](https://stackoverflow.com/questions/103512/why-use-static-castintx-instead-of-intx)的[A](https://stackoverflow.com/a/103868)中对downcast进行了说明：
>
> The only time it's a bit risky is when you cast down to an **inherited class**; you must make sure that the **object** is actually the **descendant** that you claim it is, by means external to the language (like a flag in the object).

#### Example: CRTP

CRTP是典型的cast to derived class，下面是截取自维基百科[Curiously recurring template pattern](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern)中的例子：

```c++
#include <iostream>

template<class T>
struct Base
{
	void interface()
	{
		// ...
		static_cast<T*>(this)->implementation();
		// ...
	}

	static void static_func()
	{
		// ...
		T::static_sub_func();
		// ...
	}
};

struct Derived: Base<Derived>
{
	void implementation()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
	static void static_sub_func()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
	}
};
int main()
{
	Derived d;

	d.interface();
	d.static_sub_func();
}
// g++ test.cpp

```

上述程序的输出如下：

```c++
void Derived::implementation()
static void Derived::static_sub_func()

```



### 3) lvalue to xvalue (since C++11)

If *`new_type`* is an **rvalue reference type**, `static_cast` converts the value of glvalue, class prvalue, or array prvalue (until C++17)any lvalue (since C++17) *expression* to  ***xvalue*** referring to the same object as the expression, or to its **base sub-object** (depending on *`new_type`*). If the target type is an inaccessible or ambiguous base of the type of the expression, the program is ill-formed. If the expression is a [bit field](https://en.cppreference.com/w/cpp/language/bit_field) lvalue, it is first converted to prvalue of the underlying type. This type of `static_cast` is used to implement move semantics in `std::move`.

> [gcc](https://github.com/gcc-mirror/gcc)/[libstdc++-v3](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3)/[include](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include)/[bits](https://github.com/gcc-mirror/gcc/tree/master/libstdc%2B%2B-v3/include/bits)/[move.h](https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/move.h)
>
> ```c++
> /**
>  *  @brief  Convert a value to an rvalue.
>  *  @param  __t  A thing of arbitrary type.
>  *  @return The parameter cast to an rvalue-reference to allow moving it.
>  */
> template<typename _Tp>
> constexpr typename std::remove_reference<_Tp>::type&&
> move(_Tp&& __t) noexcept
> {	return static_cast<typename std::remove_reference<_Tp>::type&&>(__t);}
> ```
>

### 4) discard value expression

> NOTE: 关于discard value expression，参见`C++\Language-reference\Expressions\Expressions.md`

### 5) inverse of that implicit conversion



### 7) enum to int or float



### 8) int to enum



### 9) pointer to member upcast



### 10) pointer to `void` to pointer to complete type



### Example

```c++
#include <vector>
#include <iostream>

struct B
{
	int m = 0;
	void hello() const
	{
		std::cout << "Hello world, this is B!\n";
	}
};
struct D: B
{
	void hello() const
	{
		std::cout << "Hello world, this is D!\n";
	}
};

enum class E
{
	ONE = 1, TWO, THREE
};
enum EU
{
	ONE = 1, TWO, THREE
};

int main()
{
	// 1: initializing conversion
	int n = static_cast<int>(3.14);
	std::cout << "n = " << n << '\n';
	std::vector<int> v = static_cast<std::vector<int>>(10);
	std::cout << "v.size() = " << v.size() << '\n';

	// 2: static downcast
	D d;
	B& br = d; // upcast via implicit conversion
	br.hello();
	D& another_d = static_cast<D&>(br); // downcast
	another_d.hello();

	// 3: lvalue to xvalue
	std::vector<int> v2 = static_cast<std::vector<int>&&>(v);
	std::cout << "after move, v.size() = " << v.size() << '\n';

	// 4: discarded-value expression
	static_cast<void>(v2.size());

	// 5. inverse of implicit conversion
	void* nv = &n;
	int* ni = static_cast<int*>(nv);
	std::cout << "*ni = " << *ni << '\n';

	// 6. array-to-pointer followed by upcast
	D a[10];
	B* dp = static_cast<B*>(a);

	// 7. scoped enum to int or float
	E e = E::ONE;
	int one = static_cast<int>(e);
	std::cout << one << '\n';

	// 8. int to enum, enum to another enum
	E e2 = static_cast<E>(one);
	EU eu = static_cast<EU>(e2);

	// 9. pointer to member upcast
	int D::*pm = &D::m;
	std::cout << br.*static_cast<int B::*>(pm) << '\n';

	// 10. void* to any type
	void* voidp = &e;
	std::vector<int>* p = static_cast<std::vector<int>*>(voidp);
}
// g++ --std=c++11 test.cpp
```

输出如下:

```c++
n = 3
v.size() = 10
Hello world, this is B!
Hello world, this is D!
after move, v.size() = 0
*ni = 3
1
0
```



## 维基百科[static_cast](https://en.wikipedia.org/wiki/Static_cast)



## Examples



[Question of using static_cast on “this” pointer in a derived object to base class](https://stackoverflow.com/questions/4543670/question-of-using-static-cast-on-this-pointer-in-a-derived-object-to-base-clas)



### `static_cast` and user-define conversion

```c++
#include <iostream>

struct X
{
	//implicit conversion
	operator int() const
	{
		std::cout << __LINE__<< " " << __PRETTY_FUNCTION__ << std::endl;
		return 7;
	}

	// explicit conversion
	explicit operator int*() const
	{
		std::cout << __LINE__<< " " << __PRETTY_FUNCTION__ << std::endl;
		return nullptr;
	}

//   Error: array operator not allowed in conversion-type-id
//   operator int(*)[3]() const { return nullptr; }
	using arr_t = int[3];
	operator arr_t*() const
	{
		std::cout << __LINE__<< " " << __PRETTY_FUNCTION__ << std::endl;
		return nullptr;
	} // OK if done through typedef
//  operator arr_t () const; // Error: conversion to array not allowed in any case
};

int main()
{
	X x;

	int n = static_cast<int>(x);   // OK: sets n to 7
	int m = x;                     // OK: sets m to 7

	int *p = static_cast<int*>(x);  // OK: sets p to null
//  int* q = x; // Error: no implicit conversion

	int (*pa)[3] = x;  // OK
}
// g++ --std=c++11 test.cpp

```

上述程序的输出如下:

```c++
8 X::operator int() const
8 X::operator int() const
15 X::operator int*() const
24 X::operator int (*)[3]() const
```


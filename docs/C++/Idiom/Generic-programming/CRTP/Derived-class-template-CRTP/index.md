# Derived class template CRTP

CRTP是基于inheritance的，一般我们见到的CRTP的用法中，derived class都是class，那derived class template呢？它能否用于CRTP呢？在使用它的过程中，是否会存在一些问题？本章对这个topic进行讨论。

## Derived class template CRTP

C++是允许的，下面是example。

### Example 我的实践

```C++
#include <iostream>

template<typename UstTag, typename SpiType>
class CUstApi
{
protected:
	template<typename ServiceImpl, typename ServiceTrait>
	struct CServiceRspBase
	{
	public:
		CServiceRspBase()
		{

		}
		int Run()
		{
			std::cout << __PRETTY_FUNCTION__ << std::endl;
			// 回调
			return static_cast<ServiceImpl*>(this)->CallBack();
		}
	};
};

template<typename UstTag, typename SpiType>
class CUstApiImpl: public CUstApi<UstTag, SpiType>
{
protected:
	template<typename ServiceTrait>
	struct CServiceMultiSpanRsp: CUstApi<UstTag, SpiType>::template CServiceRspBase<CServiceMultiSpanRsp<ServiceTrait>, ServiceTrait>
	{
		friend typename CUstApi<UstTag, SpiType>:: template CServiceRspBase<CServiceMultiSpanRsp<ServiceTrait>, ServiceTrait>;
	protected:
		int CallBack()
		{
			std::cout << __PRETTY_FUNCTION__ << std::endl;
			return 0;
		}

	};
public:
	class CTestServiceTrait
	{
	};
	void Test()
	{
		CServiceMultiSpanRsp<CTestServiceTrait> s;
		s.Run();
	}
};

struct TestUstTag
{
};
struct TestSpiType
{
};

int main()
{
	CUstApiImpl<TestUstTag, TestSpiType> Api;
	Api.Test();
}
// g++ test.cpp -Wall -pedantic

```

输出如下:

```C++
int CUstApi<UstTag, SpiType>::CServiceRspBase<ServiceImpl, ServiceTrait>::Run() [with ServiceImpl = CUstApiImpl<TestUstTag, TestSpiType>::CServiceMultiSpanRsp<CUstApiImpl<TestUstTag, TestSpiType>::CTestServiceTrait>; ServiceTrait = CUstApiImpl<TestUstTag, TestSpiType>::CTestServiceTrait; UstTag = TestUstTag; SpiType = TestSpiType]
    
int CUstApiImpl<UstTag, SpiType>::CServiceMultiSpanRsp<ServiceTrait>::CallBack() [with ServiceTrait = CUstApiImpl<TestUstTag, TestSpiType>::CTestServiceTrait; UstTag = TestUstTag; SpiType = TestSpiType]
```



## Base access member in derived



### stackoverflow [C++ static polymorphism (CRTP) and using typedefs from derived classes](https://stackoverflow.com/questions/6006614/c-static-polymorphism-crtp-and-using-typedefs-from-derived-classes)

I read the [Wikipedia article](http://en.wikipedia.org/wiki/Curiously_Recurring_Template_Pattern) about the curiously recurring template pattern in C++ for doing static (read: compile-time) polymorphism. I wanted to generalize it so that I could change the return types of the functions based on the derived type. 

```C++
#include <iostream>

template<typename derived_t>
class base
{
public:
	typedef typename derived_t::value_type value_type;
	value_type foo()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return static_cast<derived_t*>(this)->foo();
	}
};

template<typename T>
class derived: public base<derived<T> >
{
public:
	typedef T value_type;
	value_type foo()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return T(); //return some T object (assumes T is default constructable)
	}
};

int main()
{
	derived<int> a;
}
// g++ test.cpp -Wall -pedantic

```

> NOTE: 
>
> 1、编译报错如下:
>
> ```C++
> test.cpp: In instantiation of ‘class base<derived<int> >’:
> test.cpp:16:7:   required from ‘class derived<int>’
> test.cpp:29:15:   required from here
> test.cpp:7:41: 错误：no type named ‘value_type’ in ‘class derived<int>’
>   typedef typename derived_t::value_type value_type;
>                                          ^
> test.cpp: 在函数‘int main()’中:
> test.cpp:29:15: 警告：未使用的变量‘a’ [-Wunused-variable]
>   derived<int> a;
> 
> ```
>
> 2、上述程序是想实现CRTP static polymorphism，但是它的写法是有问题的，参见下面的"完整测试程序"获得正确的写法
>
> 

BTW, I have a work-around using extra template parameters, but I don't like it---it will get very verbose when passing many types up the inheritance chain.

```cpp
template <typename derived_t, typename value_type>
class base { ... };

template <typename T>
class derived : public base<derived<T>,T> { ... };
```

> NOTE: 
>
> 1、完整测试程序如下:
>
> ```C++
> #include <iostream>
> 
> template<typename derived_t, typename value_type>
> class base
> {
> public:
> 	value_type foo()
> 	{
> 		std::cout << __PRETTY_FUNCTION__ << std::endl;
> 		return static_cast<derived_t*>(this)->foo_impl();
> 	}
> };
> 
> template<typename T>
> class derived: public base<derived<T>, T>
> {
> 	friend class base<derived<T>, T> ;
> protected:
> 	typedef T value_type;
> 	value_type foo_impl()
> 	{
> 		std::cout << __PRETTY_FUNCTION__ << std::endl;
> 		return T(); //return some T object (assumes T is default constructable)
> 	}
> };
> 
> int main()
> {
> 	derived<int> a;
> 	a.foo();
> }
> // g++ test.cpp -Wall -pedantic
> 
> ```
>
> 输出如下:
>
> ```C++
> value_type base<derived_t, value_type>::foo() [with derived_t = derived<int>; value_type = int]
> derived<T>::value_type derived<T>::foo_impl() [with T = int; derived<T>::value_type = int]
> 
> ```
>
> 

#### [A](https://stackoverflow.com/a/6006629)

`derived` is incomplete when you use it as a template argument to `base` in its base classes list.

A common workaround is to use a traits class template. Here's your example, traitsified. This shows how you can use both types and functions from the derived class through the traits.

```C++
#include <iostream>

// Declare a base_traits traits class template:
template<typename derived_t>
struct base_traits;

// Define the base class that uses the traits:
template<typename derived_t>
struct base
{
	typedef typename base_traits<derived_t>::value_type value_type;
	value_type base_foo()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return base_traits<derived_t>::call_foo(static_cast<derived_t*>(this));
	}
};

// Define the derived class; it can use the traits too:
template<typename T>
struct derived: base<derived<T> >
{
	typedef typename base_traits<derived>::value_type value_type;

	value_type derived_foo()
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return value_type();
	}
};

// Declare and define a base_traits specialization for derived:
template<typename T>
struct base_traits<derived<T> >
{
	typedef T value_type;

	static value_type call_foo(derived<T> *x)
	{
		return x->derived_foo();
	}
};

int main()
{
	derived<int> d;
	d.base_foo();
}
// g++ test.cpp -Wall -pedantic

```

> NOTE: 
>
> 1、这种实现方式是非常冗杂的
>
> 2、输出如下:
>
> ```C++
> base<derived_t>::value_type base<derived_t>::base_foo() [with derived_t = derived<int>; base<derived_t>::value_type = int]
> derived<T>::value_type derived<T>::derived_foo() [with T = int; derived<T>::value_type = int]
> 
> ```
>
> 

You just need to specialize `base_traits` for any types that you use for the template argument `derived_t` of `base` and make sure that each specialization provides all of the members that `base` requires.

#### [A](https://stackoverflow.com/a/38919227)

In C++14 you could remove the `typedef` and use function `auto` return type deduction:

```cpp
template <typename derived_t>
class base {
public:
    auto foo() {
        return static_cast<derived_t*>(this)->foo();
    }
};
```

This works because the deduction of the return type of `base::foo` is delayed until `derived_t` is complete.

> NOTE: 
>
> 1、完整的测试程序如下:
>
> ```C++
> #include <iostream>
> 
> template<typename derived_t>
> class base
> {
> public:
> 	auto foo()
> 	{
> 		std::cout << __PRETTY_FUNCTION__ << std::endl;
> 		return static_cast<derived_t*>(this)->foo_impl();
> 	}
> };
> 
> template<typename T>
> class derived: public base<derived<T>>
> {
> 	friend class base<derived<T>> ;
> protected:
> 	typedef T value_type;
> 	value_type foo_impl()
> 	{
> 		std::cout << __PRETTY_FUNCTION__ << std::endl;
> 		return T(); //return some T object (assumes T is default constructable)
> 	}
> };
> 
> int main()
> {
> 	derived<int> a;
> 	a.foo();
> }
> // g++ --std=c++14 test.cpp -Wall -pedantic
> 
> ```
>
> 输出如下:
>
> ```C++
> auto base<derived_t>::foo() [with derived_t = derived<int>]
> derived<T>::value_type derived<T>::foo_impl() [with T = int; derived<T>::value_type = int]
> ```
>
> 



#### [A](https://stackoverflow.com/a/27862695)

One small drawback of using traits is that you have to declare one for each derived class. You can write a less verbose and redondant workaround like this :

```cpp
template<template<typename > class Derived, typename T>
class base
{
public:
	typedef T value_type;
	value_type foo()
	{
		return static_cast<Derived<T>*>(this)->foo();
	}
};

template<typename T>
class Derived: public base<Derived, T>
{
public:
	typedef T value_type;
	value_type foo()
	{
		return T(); //return some T object (assumes T is default constructable)
	}
};

int main()
{
	Derived<int> a;
}

```
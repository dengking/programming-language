# SFINAE-based custom polymorphism的前提条件是: 有substitution

SFINAE的前提是必要有substitution，如果没有substitution，那么就无法使用SFINAE，这是在使用SFINAE-base custom static polymorphism的时候，非常容易犯下的错误。我之前就犯过这样的错误，下面是我写的错误的程序: 



```C++
#include <type_traits>
#include <utility>
#include <iostream>

template<typename ServiceTrait>
struct CGenericPackerFactory
{
	template<typename ...Args>
	static auto New(Args &&...args) -> typename std::enable_if<ServiceTrait::has_member, bool>::type
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return true;
	}
	template<typename ...Args>
	static auto New(Args &&...args) -> typename std::enable_if<!ServiceTrait::has_member,bool>::type
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return false;
	}
};
struct TestServiceTrait
{
	static constexpr bool has_member = true;
};

struct TestServiceTrait2
{
	static constexpr bool has_member = false;
};
int main()
{

	std::cout << CGenericPackerFactory<TestServiceTrait>::New(1, 2, 3) << std::endl;
	std::cout << CGenericPackerFactory<TestServiceTrait2>::New(1, 2, 3) << std::endl;
}
// g++ test.cpp --std=c++11

```

编译报错如下:

```C++
test.cpp: In instantiation of ‘struct CGenericPackerFactory<TestServiceTrait>’:
test.cpp:33:54:   required from here
test.cpp:15:14: 错误：no type named ‘type’ in ‘struct std::enable_if<false, bool>’
  static auto New(Args &&...args) -> typename std::enable_if<!ServiceTrait::has_member,bool>::type
              ^
test.cpp: In instantiation of ‘struct CGenericPackerFactory<TestServiceTrait2>’:
test.cpp:34:55:   required from here
test.cpp:9:14: 错误：no type named ‘type’ in ‘struct std::enable_if<false, bool>’
  static auto New(Args &&...args) -> typename std::enable_if<ServiceTrait::has_member, bool>::type
```

通过Google: no type named ‘type’ in ‘`struct std::enable_if<false, bool>`’，下面是有价值的内容: 

## stackoverflow [Selecting a member function using different enable_if conditions](https://stackoverflow.com/questions/13401716/selecting-a-member-function-using-different-enable-if-conditions)

I am trying to determine which version of a member function gets called based on the class template parameter. I have tried this:

```cpp
#include <iostream>
#include <type_traits>

template<typename T>
struct Point
{
	void MyFunction(typename std::enable_if<std::is_same<T, int>::value, T>::type* = 0)
	{
		std::cout << "T is int." << std::endl;
	}

	void MyFunction(typename std::enable_if<!std::is_same<T, int>::value, float>::type* = 0)
	{
		std::cout << "T is not int." << std::endl;
	}
};

int main()
{
	Point<int> intPoint;
	intPoint.MyFunction();

	Point<float> floatPoint;
	floatPoint.MyFunction();
}
// g++ test.cpp --std=c++11

```

> NOTE: 
>
> 1、编译报错如下:
>
> ```
> test.cpp: In instantiation of ‘struct Point<int>’:
> test.cpp:20:13:   required from here
> test.cpp:12:7: 错误：no type named ‘type’ in ‘struct std::enable_if<false, float>’
>   void MyFunction(typename std::enable_if<!std::is_same<T, int>::value, float>::type* = 0)
>        ^
> test.cpp: In instantiation of ‘struct Point<float>’:
> test.cpp:23:15:   required from here
> test.cpp:7:7: 错误：no type named ‘type’ in ‘struct std::enable_if<false, float>’
>   void MyFunction(typename std::enable_if<std::is_same<T, int>::value, T>::type* = 0)
> 
> ```
>
> 

which I thought is saying "use the first MyFunction if T is int, and use the second MyFunction if T is not int, but I get compiler errors saying "error: no type named ‘type’ in ‘struct std::enable_if’". Can anyone point out what I am doing wrong here?

### [A](https://stackoverflow.com/a/13401982)

> NOTE: 非常好的解答

`enable_if` works because the [substitution of a template argument resulted in an error](https://en.wikipedia.org/wiki/SFINAE), and so that substitution is dropped from the **overload resolution set** and only other viable overloads are considered by the compiler.

In your example, there is no substitution occurring when instantiating the **member functions** because the template argument `T` is already known at that time. The simplest way to achieve what you're attempting is to create a dummy template argument that is defaulted to `T` and use that to perform SFINAE.

> NOTE: 
>
> 1、在 "instantiating the **member functions** " 的时候，没有substitution发生，因此无法使用SFINAE-base custom static polymorphism
>
> 2、作者提供的解决方案是: dummy template argument，通过它引入substitution

```cpp
template<typename T>
struct Point
{
  template<typename U = T>
  typename std::enable_if<std::is_same<U, int>::value>::type
    MyFunction()
  {
    std::cout << "T is int." << std::endl;
  }

  template<typename U = T>
  typename std::enable_if<std::is_same<U, float>::value>::type
    MyFunction()
  {
    std::cout << "T is not int." << std::endl;
  }
};
```

------

**Edit:**

As HostileFork mentions in the comments, the original example leaves the possibility of the user explicitly specifying template arguments for the member functions and getting an incorrect result. The following should prevent explicit specializations of the member functions from compiling.

```cpp
template<typename T>
struct Point
{
  template<typename... Dummy, typename U = T>
  typename std::enable_if<std::is_same<U, int>::value>::type
    MyFunction()
  {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments!");
    std::cout << "T is int." << std::endl;
  }

  template<typename... Dummy, typename U = T>
  typename std::enable_if<std::is_same<U, float>::value>::type
    MyFunction()
  {
    static_assert(sizeof...(Dummy)==0, "Do not specify template arguments!");
    std::cout << "T is not int." << std::endl;
  }
};
```

## 解决方案: dummy template parameter

通过dummy template parameter，引入substitution，从而引入SFINAE。下面根据stackoverflow [Selecting a member function using different enable_if conditions](https://stackoverflow.com/questions/13401716/selecting-a-member-function-using-different-enable-if-conditions) # [A](https://stackoverflow.com/a/13401982) 的内容，总结了两种实现方式:

### 使用class template parameter作为method template dummy parameter

正确的程序:

```C++
#include <type_traits>
#include <utility>
#include <iostream>

template<typename ServiceTrait>
struct CGenericPackerFactory
{
	template<typename Dummy = ServiceTrait, typename ...Args>
	static auto New(Args &&...args) -> typename std::enable_if<Dummy::has_member, bool>::type
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return true;
	}
	template<typename Dummy = ServiceTrait, typename ...Args>
	static auto New(Args &&...args) -> typename std::enable_if<!Dummy::has_member,bool>::type
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return false;
	}
};
struct TestServiceTrait
{
	static constexpr bool has_member = true;
};

struct TestServiceTrait2
{
	static constexpr bool has_member = false;
};

struct TestUstTag
{
};
int main()
{
	std::cout << CGenericPackerFactory<TestServiceTrait>::New(1, 2, 3) << std::endl;
	std::cout << CGenericPackerFactory<TestServiceTrait2>::New(1, 2, 3) << std::endl;
}
// g++ test.cpp --std=c++11

```



### 使用parameter pack作为method template dummy parameter

```C++
#include <type_traits>
#include <utility>
#include <iostream>

template<typename T>
struct Point
{
	template<typename ... Dummy, typename U = T>
	typename std::enable_if<std::is_same<U, int>::value>::type MyFunction()
	{
		static_assert(sizeof...(Dummy)==0, "Do not specify template arguments!");
		std::cout << "T is int." << std::endl;
	}

	template<typename ... Dummy, typename U = T>
	typename std::enable_if<std::is_same<U, float>::value>::type MyFunction()
	{
		static_assert(sizeof...(Dummy)==0, "Do not specify template arguments!");
		std::cout << "T is not int." << std::endl;
	}
};
int main()
{
	Point<int> intPoint;
	intPoint.MyFunction();

	Point<float> floatPoint;
	floatPoint.MyFunction();
}
// g++ test.cpp --std=c++11

```


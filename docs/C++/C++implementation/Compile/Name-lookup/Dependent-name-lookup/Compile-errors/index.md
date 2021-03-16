# Compile errors

本章总结与dependent name lookup相关的compile error。

## error: expected primary-expression before ‘>’ token

这是一种非常常见的compile错误，在 "thegreenplace [Dependent name lookup for C++ templates](https://eli.thegreenplace.net/2012/02/06/dependent-name-lookup-for-c-templates) `#` "Disambiguating dependent template names"" 段中介绍了这种错误，下面再补充一些例子: 

1) stackoverflow [C++ template compilation error: expected primary-expression before ‘>’ token](https://stackoverflow.com/questions/3505713/c-template-compilation-error-expected-primary-expression-before-token)

[A](https://stackoverflow.com/a/3505738)

You need to do:

```c++
std::cout << pt.template get<std::string>("path");
```

Use `template` in the same situation as `typename`, except for template members instead of types.

(That is, since `pt::get` is a template member *dependent* on a template parameter, you need to tell the compiler it's a template.)

## error: there are no arguments to ‘`***`’ that depend on a template parameter, so a declaration of ‘`***`’ must be available

这是一种常见的compiler错误，在"thegreenplace [Dependent name lookup for C++ templates](https://eli.thegreenplace.net/2012/02/06/dependent-name-lookup-for-c-templates) `#` "A simple problem and a solution""中介绍了这种错误，下面再补充一些例子:

stackoverflow [Inheritance and templates in C++ - why are inherited members invisible?](https://stackoverflow.com/questions/1567730/inheritance-and-templates-in-c-why-are-inherited-members-invisible)

```C++
template <int a>
class Test {
public:
    Test() {}
    int MyMethod1() { return a; }
};

template <int b>
class Another : public Test<b>
{
public:
    Another() {}
    void MyMethod2() {
        MyMethod1();
    }
};

int main()
{
    Another<5> a;
    a.MyMethod1();
    a.MyMethod2();
}
// g++ test.cpp

```

编译报错如下: 

```c++
test.cpp: In member function ‘void Another<b>::MyMethod2()’:
test.cpp:14:19: error: there are no arguments to ‘MyMethod1’ that depend on a template parameter, so a declaration of ‘MyMethod1’ must be available [-fpermissive]
         MyMethod1();
                   ^
test.cpp:14:19: note: (if you use ‘-fpermissive’, G++ will accept your code, but allowing the use of an undeclared name is deprecated)
```

[A](https://stackoverflow.com/a/1567781)

```C++
template <int a>
class Test {
public:
    Test() {}
    int MyMethod1() { return a; }
};

template <int b>
class Another : public Test<b>
{
public:
    Another() {}
    void MyMethod2() {
        Test<b>::MyMethod1();
    }
};

int main()
{
    Another<5> a;
    a.MyMethod1();
    a.MyMethod2();
}
// g++ test.cpp

```



## error: ‘`***`’ does not name a type

这是一种常见的compiler错误，在thegreenplace [Dependent name lookup for C++ templates](https://eli.thegreenplace.net/2012/02/06/dependent-name-lookup-for-c-templates) `#` "Disambiguating dependent type names"中介绍了这种错误，下面再补充一些例子:

stackoverflow [Propagating 'typedef' from based to derived class for 'template'](https://stackoverflow.com/questions/1643035/propagating-typedef-from-based-to-derived-class-for-template)



```C++
#include <iostream>
#include <vector>

template<typename T>
class A
{
public:
	typedef std::vector<T> Vec_t;
};

template<typename T>
class B: public A<T>
{
private:
	Vec_t v;  // fails - Vec_t is not recognized
};
int main()
{
	B<int> b;
}
// g++ test.cpp

```

编译报错如下:

```C++
test.cpp:15:2: error: ‘Vec_t’ does not name a type
  Vec_t v;  // fails - Vec_t is not recognized
  ^
test.cpp:15:2: note: (perhaps ‘typename A<T>::Vec_t’ was intended)
```

正确写法:

```C++
#include <iostream>
#include <vector>

template<typename T>
class A
{
public:
	typedef std::vector<T> Vec_t;
};

template<typename T>
class B: public A<T>
{
private:
	typename A<T>::Vec_t v; 
};
int main()
{
	B<int> b;
}
// g++ test.cpp

```

## error: qualified name refers into a specialization of function template '`***`'

## error: expected ‘;’ before ‘::’ token

下面是我写的code，在进行编译的时候，报了如下错误:

```C++
struct GenericPackerFactory  
{};

template<typename ServiceTrait, typename ...Args>
  auto GenericPackerFactory(int UserRequestID, Args &&...args) -> decltype(GenericPackerFactory<ServiceTrait>::New(std::forward<Args>(args)...))
  {
    static constexpr FuncNoType FuncNo = ServiceTrait::FuncNo; // 功能号
    auto Packer = GenericPackerFactory<ServiceTrait>::New(std::forward<Args>(args)...);
    int ApiRequestID = RequestIdConverter.ToApiRequestId(UserRequestID);
    Packer.SetFuncNo(FuncNo).SetSenderID(ApiRequestID);
    // 打印通讯日志
    {
      LOG_MSG_INFO("{}的请求:", FuncNo);
      LOG_PACKER_INFO(&Packer);
    }
    Packer.SetFuncNo(FuncNo).SetSenderID(ApiRequestID);
    PackHead(Packer, ApiRequestID);
    Packer.Finish();
    return Packer;
  }
```

`clang`编译报错:

```C++
./../common/api_framework/ldp_api.h:519:51: error: qualified name refers into a specialization of function template 'GenericPackerFactory'
                auto Packer = GenericPackerFactory<ServiceTrait>::template New(std::forward<Args>(args)...);
```

`g++`编译报错:

```C++
错误：expected ‘;’ before ‘::’ token
```



### 模拟程序

下面是简化的模拟程序:

```C++
#include <type_traits>
#include <utility>
#include <iostream>

template<typename ServiceTrait>
struct GenericPackerFactory
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
template<typename UstTag>
class UstApi
{
public:
	template<typename ServiceTrait, typename ...Args>
	auto GenericPackerFactory(int UserRequestID, Args &&...args) -> decltype(GenericPackerFactory<ServiceTrait>::New(std::forward<Args>(args)...))
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return GenericPackerFactory<ServiceTrait>::New(std::forward<Args>(args)...);
	}
};

struct TestUstTag
{
};
int main()
{
	UstApi<TestUstTag> Api;
	Api.GenericPackerFactory<TestServiceTrait>(1, 1, 2, 3);
	Api.GenericPackerFactory<TestServiceTrait2>(1, 1, 2, 3);
}

```

`g++ test.cpp --std=c++11`编译报错如下:

```C++
test.cpp: 在成员函数‘decltype (GenericPackerFactory<ServiceTrait>::New((forward<Args>)(args)...)) UstApi<UstTag>::GenericPackerFactory(int, Args&& ...)’中:
test.cpp:38:44: 错误：expected ‘;’ before ‘::’ token
   return GenericPackerFactory<ServiceTrait>::New(std::forward<Args>(args)...);
                                            ^
test.cpp:38:44: 错误：‘::New’未被声明
test.cpp: In instantiation of ‘decltype (GenericPackerFactory<ServiceTrait>::New((forward<Args>)(UstApi::GenericPackerFactory::args)...)) UstApi<UstTag>::GenericPackerFactory(int, Args&& ...) [with ServiceTrait = TestServiceTrait; Args = {int, int, int}; UstTag = TestUstTag; decltype (GenericPackerFactory<ServiceTrait>::New((forward<Args>)(UstApi::GenericPackerFactory::args)...)) = bool]’:
test.cpp:48:55:   required from here
test.cpp:38:10: 错误：无法解析重载函数‘GenericPackerFactory’，基于向类型‘std::enable_if<true, bool>::type {aka bool}’的转换
   return GenericPackerFactory<ServiceTrait>::New(std::forward<Args>(args)...);
          ^
test.cpp: In instantiation of ‘decltype (GenericPackerFactory<ServiceTrait>::New((forward<Args>)(UstApi::GenericPackerFactory::args)...)) UstApi<UstTag>::GenericPackerFactory(int, Args&& ...) [with ServiceTrait = TestServiceTrait2; Args = {int, int, int}; UstTag = TestUstTag; decltype (GenericPackerFactory<ServiceTrait>::New((forward<Args>)(UstApi::GenericPackerFactory::args)...)) = bool]’:
test.cpp:49:56:   required from here
test.cpp:38:10: 错误：无法解析重载函数‘GenericPackerFactory’，基于向类型‘std::enable_if<true, bool>::type {aka bool}’的转换
```

`clang test.cpp --std=c++11`编译报错如下:

```C++
test.cpp: In member function ‘decltype (GenericPackerFactory<ServiceTrait>::New((forward<Args>)(args)...)) UstApi<UstTag>::GenericPackerFactory(int, Args&& ...)’:
test.cpp:38:44: error: expected ‘;’ before ‘::’ token
   return GenericPackerFactory<ServiceTrait>::New(std::forward<Args>(args)...);
```

### Google: qualified name refers into a specialization of function template 

下面是有价值的文章: 

#### code.i-harness [c++ - namespace - qualified name refers into a specialization of variable template](https://code.i-harness.com/en/q/2a5bfba)

> Is `this->` mandatory to access `Base<T>` identifiers from derived classes? (2)
>
> This code compiles with MSVC 2015, but doesn't compile with Clang 5.0.0 (trunk 304874):
>
> ```C++
> template <typename T>
> struct Base
> {
>   T data;
> };
> 
> template <typename T>
> struct Derived : Base<T>
> {
>   auto getData() const
>   {
>     return data;
>   }
> };
> ```



粗略的浏览了一下这篇文章的内容，我想起了this dependent name、然后惊奇的发现我错误地将 `UstApi::GenericPackerFactory` 和 `struct GenericPackerFactory` 命名为相同的名字，这就导致了compiler将  `GenericPackerFactory<ServiceTrait>::New(std::forward<Args>(args)...);` 中的`GenericPackerFactory`理解为`UstApi::GenericPackerFactory`函数，而不是  `struct GenericPackerFactory` 。

显然改法就是将它们的名称区分开来，下面是修改后的，可以通过的code:



### 正确程序

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
template<typename UstTag>
class UstApi
{
public:
	template<typename ServiceTrait, typename ...Args>
	auto GenericPackerFactory(int UserRequestID, Args &&...args) -> decltype(CGenericPackerFactory<ServiceTrait>::New(std::forward<Args>(args)...))
	{
		std::cout << __PRETTY_FUNCTION__ << std::endl;
		return CGenericPackerFactory<ServiceTrait>::New(std::forward<Args>(args)...);
	}
};

struct TestUstTag
{
};
int main()
{
	UstApi<TestUstTag> Api;
	Api.GenericPackerFactory<TestServiceTrait>(1, 1, 2, 3);
	Api.GenericPackerFactory<TestServiceTrait2>(1, 1, 2, 3);
}
// g++ test.cpp --std=c++11

```

#### 输出如下:

```C++
decltype (CGenericPackerFactory<ServiceTrait>::New((forward<Args>)(UstApi::GenericPackerFactory::args)...)) UstApi<UstTag>::GenericPackerFactory(int, Args&& ...) [with ServiceTrait = TestServiceTrait; Args = {int, int, int}; UstTag = TestUstTag; decltype (CGenericPackerFactory<ServiceTrait>::New((forward<Args>)(UstApi::GenericPackerFactory::args)...)) = bool]
static typename std::enable_if<Dummy:: has_member, bool>::type CGenericPackerFactory<ServiceTrait>::New(Args&& ...) [with Dummy = TestServiceTrait; Args = {int, int, int}; ServiceTrait = TestServiceTrait; typename std::enable_if<Dummy:: has_member, bool>::type = bool]
1
decltype (CGenericPackerFactory<ServiceTrait>::New((forward<Args>)(UstApi::GenericPackerFactory::args)...)) UstApi<UstTag>::GenericPackerFactory(int, Args&& ...) [with ServiceTrait = TestServiceTrait2; Args = {int, int, int}; UstTag = TestUstTag; decltype (CGenericPackerFactory<ServiceTrait>::New((forward<Args>)(UstApi::GenericPackerFactory::args)...)) = bool]
static typename std::enable_if<(! Dummy:: has_member), bool>::type CGenericPackerFactory<ServiceTrait>::New(Args&& ...) [with Dummy = TestServiceTrait2; Args = {int, int, int}; ServiceTrait = TestServiceTrait2; typename std::enable_if<(! Dummy:: has_member), bool>::type = bool]
0

```


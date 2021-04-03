# Detection idiom

## 涉及Detection idiom的章节

1、`SFINAE`

2、`SFINAE\Expression-SFINAE`



## Detection idiom的本质

1、detection 一般以 trait（metafunction）的方式呈现，但是，我们应该从concept的角度来理解、运用它。在使用的使用，一般是使用SFINAE-based custom static polymorphism

2、static/compile-time reflection



## Detect对象以及implementation



### 一、Detect member(method、variable)

最最简单的: 判断class是否具有某个member(variable、method)

#### maddouri gist [has_member.hpp](https://gist.github.com/maddouri/0da889b331d910f35e05ba3b7b9d869b#file-has_member-hpp)



```C++
// A compile-time method for checking the existence of a class member
// @see https://general-purpose.io/2017/03/10/checking-the-existence-of-a-cpp-class-member-at-compile-time/

// This code uses "decltype" which, according to http://en.cppreference.com/w/cpp/compiler_support
// should be supported by Clang 2.9+, GCC 4.3+ and MSVC 2010+ (if you have an older compiler, please upgrade :)
// As of "constexpr", if not supported by your compiler, you could try "const"
// or use the value as an inner enum value e.g. enum { value = ... }

// check "test_has_member.cpp" for a usage example

/// Defines a "has_member_member_name" class template
///
/// This template can be used to check if its "T" argument
/// has a data or function member called "member_name"
#define define_has_member(member_name)                                         \
    template <typename T>                                                      \
    class has_member_##member_name                                             \
    {                                                                          \
        typedef char yes_type;                                                 \
        typedef long no_type;                                                  \
        template <typename U> static yes_type test(decltype(&U::member_name)); \
        template <typename U> static no_type  test(...);                       \
    public:                                                                    \
        static constexpr bool value = sizeof(test<T>(0)) == sizeof(yes_type);  \
    }

/// Shorthand for testing if "class_" has a member called "member_name"
///
/// @note "define_has_member(member_name)" must be used
///       before calling "has_member(class_, member_name)"
#define has_member(class_, member_name)  has_member_##member_name<class_>::value
```

[**test_has_member.cpp**](https://gist.github.com/maddouri/0da889b331d910f35e05ba3b7b9d869b#file-test_has_member-cpp)

```c++
// usage example: check for the existence of a "sayHi" member //////////////////////////////////////

// clang++ -std=c++11 -pedantic -Wall -Wextra test_has_member.cpp -o test_has_member && ./test_has_member
// g++     -std=c++11 -pedantic -Wall -Wextra test_has_member.cpp -o test_has_member && ./test_has_member

#include <iostream> // cout, endl
#include <iomanip>  // std::boolalpha

#include "has_member.hpp"

struct A  // has a "sayHi" member
{
    void sayHi() { std::cout << "Hi there!" << std::endl;  }
};

struct B  // doesn't have a "sayHi" member
{
    void sayBye() { std::cout << "Bye bye!" << std::endl; }
};

// define a "sayHi" "member checker" class
define_has_member(sayHi);

int main()
{
    using std::cout;
    using std::endl;
    cout << std::boolalpha;  // display "true" or "false" for booleans

    A a;
    B b;

    // check the existence of "sayHi"
    cout << "has_member(A, sayHi) " << has_member(A, sayHi) << endl;
    cout << "has_member(B, sayHi) " << has_member(B, sayHi) << endl;

    cout << endl;

    // same thing, using decltype on instances
    cout << "has_member(decltype(a), sayHi) " << has_member(decltype(a), sayHi) << endl;
    cout << "has_member(decltype(b), sayHi) " << has_member(decltype(b), sayHi) << endl;

    return 0;
}
```

> NOTE:
>
> 1、上述是可行的



#### stackoverflow [Templated check for the existence of a class member function?](https://stackoverflow.com/questions/257288/templated-check-for-the-existence-of-a-class-member-function)

[A](https://stackoverflow.com/a/257382): SFINA+`decltype`

```c++
#include <iostream>

struct Hello
{
	int helloworld()
	{
		return 0;
	}
};

struct Generic
{
};

// SFINAE test
template<typename T>
class has_helloworld
{
	typedef char one;
	struct two
	{
		char x[2];
	};

	template<typename C> static one test(typeof(&C::helloworld));
	template<typename C> static two test(...);

public:
	enum
	{
		value = sizeof(test<T>(0)) == sizeof(char)
	};
};

int main(int argc, char *argv[])
{
	std::cout << has_helloworld<Hello>::value << std::endl;
	std::cout << has_helloworld<Generic>::value << std::endl;
	return 0;
}
// g++ test.cppm

```



[A](https://stackoverflow.com/a/9154394): expression SFINAE 

This question is old, but with `C++11` we got a new way to check for a functions existence (or existence of any non-type member, really), relying on SFINAE again:

```cpp
template<class T>
auto serialize_imp(std::ostream& os, T const& obj, int)
    -> decltype(os << obj, void())
{
  os << obj;
}

template<class T>
auto serialize_imp(std::ostream& os, T const& obj, long)
    -> decltype(obj.stream(os), void())
{
  obj.stream(os);
}

template<class T>
auto serialize(std::ostream& os, T const& obj)
    -> decltype(serialize_imp(os, obj, 0), void())
{
  serialize_imp(os, obj, 0);
}
```

Now onto some explanations. First thing, I use [expression SFINAE](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2634.html) to exclude the `serialize(_imp)` functions from **overload resolution**, if the first expression inside `decltype` isn't valid (aka, the function doesn't exist).

The `void()` is used to make the return type of all those functions `void`.

The `0` argument is used to prefer the `os << obj` overload if both are available (literal `0` is of type `int` and as such the first overload is a better match).



#### gist [sfinae_tostring_ex.cpp](https://gist.github.com/fenbf/d2cd670704b82e2ce7fd#file-sfinae_tostring_ex-cpp)

https://gist.github.com/fenbf/d2cd670704b82e2ce7fd





### 二、Detect member method

#### stackoverflow [What is “Expression SFINAE”?](https://stackoverflow.com/questions/12654067/what-is-expression-sfinae)

> NOTE: 
>
> 收录在了`SFINAE\Expression-SFINAE`章节。

[A](https://stackoverflow.com/a/12654277)

> NOTE: 
>
> 1、下面的这种实现方式是比较符合C++11的

```C++
#include <iostream>
#include <type_traits>
#include <vector>

struct has_member_begin_test
{
	template<class U>
	static auto test(U* p) -> decltype(p->begin(), std::true_type());
	template<class >
	static auto test(...) -> std::false_type;
};

template<class T>
struct has_member_begin
: decltype(has_member_begin_test::test<T>(0))
{
};

int main()
{
	std::cout << std::boolalpha;
	std::cout << has_member_begin<int>::value << std::endl;
}
// g++ --std=c++11 test.cpp
```

#### [macmade](https://github.com/macmade)/**[CPPTraits](https://github.com/macmade/CPPTraits)**

参见 `Library-CPPTraits` 章节。





### 三、Detecting member types

判断class是否具备member type。

#### wikipedia [Substitution failure is not an error](https://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error)



```c++
#include <iostream>
#include <type_traits>

template <typename... Ts>
using void_t = void;

template <typename T, typename = void>
struct has_typedef_foobar : std::false_type {};

template <typename T>
struct has_typedef_foobar<T, void_t<typename T::foobar>> : std::true_type {};

struct foo {
  using foobar = float;
};

int main() {
  std::cout << std::boolalpha;
  std::cout << has_typedef_foobar<int>::value << std::endl;
  std::cout << has_typedef_foobar<foo>::value << std::endl;
}
// g++ --std=c++11 test.cpp

```



正确版本如下:

```C++
#include <iostream>
#include <type_traits>

template<typename ...Ts> struct make_void
{
	using type = void;
};
template<typename ...Ts> using void_t = typename make_void<Ts...>::type;

template<typename T, typename = void>
struct has_typedef_foobar: std::false_type
{
};

template<typename T>
struct has_typedef_foobar<T, void_t<typename T::foobar>> : std::true_type
{
};

struct foo
{
	using foobar = float;
};

int main()
{
	std::cout << std::boolalpha;
	std::cout << has_typedef_foobar<int>::value << std::endl;
	std::cout << has_typedef_foobar<foo>::value << std::endl;
}
// g++ --std=c++11 test.cpp

```



实现总结: Primary class template 和 specialization 继承不同的 parent class；使用`void_t` 来控制SFINAE；

#### More C++ Idioms/[Member Detector](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Member_Detector) # Detecting member types

参见 `More-C++Idioms-Member-Detector` 章节。

### 四、判断某个member type是否defined

参见 `Detect-whether-a-type-is-defined` 章节



### 五、Detect member method with specific signature

判断class是否具有指定类型的member(variable、method)。

#### stackoverflow [Check if a class has a member function of a given signature](https://stackoverflow.com/questions/87372/check-if-a-class-has-a-member-function-of-a-given-signature)

I'm asking for a template trick to detect if a class has a specific member function of a given signature.

[A](https://stackoverflow.com/a/16824239)

Here's a possible implementation relying on C++11 features. It correctly detects the function even if it's inherited (unlike the solution in the accepted answer, as Mike Kinghan observes in [his answer](https://stackoverflow.com/a/10707822/947836)).

The function this snippet tests for is called `serialize`:

```cpp
#include <type_traits>

// Primary template with a static assertion
// for a meaningful error message
// if it ever gets instantiated.
// We could leave it undefined if we didn't care.

template<typename, typename T>
struct has_serialize {
    static_assert(
        std::integral_constant<T, false>::value,
        "Second template parameter needs to be of function type.");
};

// specialization that does the checking

template<typename C, typename Ret, typename... Args>
struct has_serialize<C, Ret(Args...)> {
private:
    template<typename T>
    static constexpr auto check(T*)
    -> typename
        std::is_same<
            decltype( std::declval<T>().serialize( std::declval<Args>()... ) ),
            Ret    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        >::type;  // attempt to call it and see if the return type is correct

    template<typename>
    static constexpr std::false_type check(...);

    typedef decltype(check<C>(0)) type;

public:
    static constexpr bool value = type::value;
};
```

Usage:

```cpp
struct X {
     int serialize(const std::string&) { return 42; } 
};

struct Y : X {};

std::cout << has_serialize<Y, int(const std::string&)>::value; // will print 1
```

> NOTE: 
>
> 一、上述程序的实现是非常巧妙的:
>
> `has_serialize<Y, int(const std::string&)>` 对应的是如下template:
>
> ```C++
> template<typename C, typename Ret, typename... Args>
> struct has_serialize<C, Ret(Args...)> 
> ```
>
> 我第一次看的时候，惊讶于 `has_serialize<Y, int(const std::string&)>` 并没有指定 template parameter `Ret`、`Args`，难道是compiler自己deduce的？要理解这个，就需要对compiler编译template的完整流程有一个清楚的认识，在`Compile-template`章节中，对上述程序进行了非常好的剖析。
>
> 上述程序的巧妙之处在于，它利用了compiler自动推到出template specialization的template argument机制，得到了function signature的如下两个部分:
>
> 1、return type: `Ret`
>
> 2、function parameter list: `Args`
>
> 这样，在template specialization中，就可以写出valid expression了
>
> 需要注意的是 `int(const std::string&)` 是一个function signature，它并不是pointer to member function。
>
> 二、完整测试程序如下:
>
> ```C++
> #include <type_traits>
> #include <iostream>
> // Primary template with a static assertion
> // for a meaningful error message
> // if it ever gets instantiated.
> // We could leave it undefined if we didn't care.
> 
> template<typename, typename T>
> struct has_serialize
> {
> 	static_assert(std::integral_constant<T, false>::value, "Second template parameter needs to be of function type.");
> };
> 
> // specialization that does the checking
> template<typename C, typename Ret, typename ... Args>
> struct has_serialize<C, Ret(Args...)>
> {
> private:
> 	template<typename T>
> 	static constexpr auto check(T*) -> typename
> 	std::is_same<
> 	decltype( std::declval<T>().serialize( std::declval<Args>()... ) ),
> 	Ret    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
> 					>::type;// attempt to call it and see if the return type is correct
> 
> 	template<typename >
> 	static constexpr std::false_type check(...);
> 
> 	typedef decltype(check<C>(0)) type;
> 
> public:
> 	static constexpr bool value = type::value;
> };
> 
> struct X
> {
> 	int serialize(const std::string&)
> 	{
> 		return 42;
> 	}
> };
> 
> struct Y: X
> {
> };
> 
> struct Z
> {
> };
> int main()
> {
> 	std::cout << has_serialize<Y, int(const std::string&)>::value << std::endl; // will print 1
> 	std::cout << has_serialize<Z, int(const std::string&)>::value << std::endl; // will print 1
> }
> // g++ --std=c++11 test.cpp
> 
> ```
>
> 输出如下:
>
> ```C++
> 1
> 0
> ```
>
> 

#### More C++ Idioms/[Member Detector](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Member_Detector) # Detecting overloaded member functions



## Implementation总结

1、detection idiom的实现是基于SFINAE-based custom static polymorphism，因此，而SFINAE-based custom static polymorphism的实现technique是不断在演进的，因此可以看到，实现detection idiom的方式是非常多、非常灵活的。

> NOTE: 
>
> 1、上述内容是我在阅读 `void_t`章节、tartanllama [Detection Idiom - A Stopgap for Concepts](https://blog.tartanllama.xyz/detection-idiom/) 时，联想到的。

2、STL的支持: [`std::experimental::is_detected, std::experimental::detected_t, std::experimental::detected_or`](https://en.cppreference.com/w/cpp/experimental/is_detected)，参见 `Standard-library\Extensions\Version-2\Detection-idiom` 章节。

3、看了诸多实现，我觉得`std::is_detected`是最最像C++ concept的，关于此，参见 `Standard-library\Extensions\Version-2\Detection-idiom` 章节。

4、不同的detect对象，则它们的valid expression就不同

## TODO

https://www.codeplay.com/portal/09-15-17-detection-idiom-a-stopgap-for-concepts

https://stackoverflow.com/questions/45249985/how-to-require-an-exact-function-signature-in-the-detection-idiom

https://humanreadablemag.com/issues/1/articles/cpp-evolution-via-detection-idiom-lens

https://blog.tartanllama.xyz/detection-idiom/

https://stackoverflow.com/questions/257288/templated-check-for-the-existence-of-a-class-member-function/9154394#9154394

https://stackoverflow.com/questions/87372/check-if-a-class-has-a-member-function-of-a-given-signature

https://stackoverflow.com/questions/257288/templated-check-for-the-existence-of-a-class-member-function

https://stackoverflow.com/questions/18570285/using-sfinae-to-detect-a-member-function
# Member Detector



## More C++ Idioms/[Member Detector](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Member_Detector)

### Motivation

**Compile-time reflection** capabilities are the cornerstone of C++ template meta-programming. **Type traits** libraries such as `Boost.TypeTraits` and `TR1 <type_traits>` header provide powerful ways of extracting information about types and their relationships. Detecting the presence of a **data member** in a class is also an example of compile-time reflection.

### Solution and Sample Code

#### Before C++11

**Member detector idiom** is implemented using the Substitution Failure Is Not An Error (SFINAE)  idiom. The following class `template DetectX<T>` is a meta-function that determines whether type `T` has a data or function member named `X` in it or not. Note that the type of the data member `X` does not matter nor does the return value and arguments of the member function (if it is one).

```c++
template<typename T>
class DetectX
{
	struct Fallback
	{
		int X;
	}; // add member name "X"
	struct Derived: T, Fallback
	{
	};
	template<typename U, U> struct Check;
	typedef char ArrayOfOne[1]; // typedef for an array of size one.
	typedef char ArrayOfTwo[2]; // typedef for an array of size two.
	// Check<int Fallback::*, &U::X> 是入参类型
	template<typename U>
	static ArrayOfOne & func(Check<int Fallback::*, &U::X> *);

	template<typename U>
	static ArrayOfTwo & func(...);
public:
	typedef DetectX type;
	enum
	{
		value = sizeof(func<Derived>(0)) == 2
	};
};

```



This idiom works by creating a **controlled ambiguity** during compilation and recovering from that using the **SFINAE idiom**. First proxy class, `Fallback`, has a data member of the same name that we want to detect the presence of. Class `Derived` inherits from both `T` and `Fallback`. As a result, `Derived` class will have at least one data member named `X`. Potentially, `Derived` class may have two `X` data members if `T` also has one.

The `Check` template is used to create **controlled ambiguity**. `Check` template takes two parameters. First is a type parameter and the second is an instance of that type. For example, `Check<int, 5>` is be a valid instantiation. Two overloaded functions named `func` create an **overload-set** as often done in the **SFINAE idiom**. The first `func` function can be instantiated only if the address of data member `U::X` can be taken **unambiguously**. Address of `U::X` can be taken if there is exactly one `X` data member in the `Derived` class; i.e., `T` does not have data member `X`. If `T` has `X` in it, the address of `U::X` can’t be taken without
further disambiguation and therefore the instantiation of the first `func` fails and the other function is chosen, all without an error. Note the difference between the return types of the two `func` functions. The first function returns a reference to array of size one whereas the second one returns a reference to an array of size two. This difference in the sizes allows us to identify which function was instantiated.

Finally, a boolean value is exposed, which is true only if the `sizeof` the return type of the function is two. That is, when the second `func` is instantiated only because `T` has `X` data member.

For every different member that is to be detected, the above class template needs to change. A macro would be preferable in such cases. The following sample code demonstrates the use of a macro.

```c++
#include <iostream>
#include <utility>

#define CREATE_MEMBER_DETECTOR(X)\
template<typename T>\
class Detect_##X\
{\
	struct Fallback\
	{\
		int X;\
	};\
	struct Derived: T, Fallback\
	{\
	};\
\
	template<typename U, U> struct Check;\
\
	typedef char ArrayOfOne[1];\
	typedef char ArrayOfTwo[2];\
\
	template<typename U> static ArrayOfOne & func(Check<int Fallback::*, &U::X> *);\
	template<typename U> static ArrayOfTwo & func(...);\
\
public:\
	typedef Detect_##X type; \
	enum\
	{\
		value = sizeof(func<Derived>(0)) == 2\
	};\
};

CREATE_MEMBER_DETECTOR(first);
CREATE_MEMBER_DETECTOR(second);
int main(void)
{
	typedef std::pair<int, double> Pair;
	std::cout << ((Detect_first<Pair>::value && Detect_second<Pair>::value) ? "Pair" : "Not Pair") << std::endl;
}
// g++ test.cpp
```

#### C++11

Using `C++11` features, this example can be rewritten so that the **controlled ambiguity** is created using the `decltype` specifier instead of a `Check` template and a pointer to member. The result can then be wrapped inside a class inheriting from `integral_constant` to provide an interface identical to the type predicates present in the standard header `<type_traits>`.



```c++
#include <type_traits> // To use 'std::integral_constant'.
#include <iostream> // To use 'std::cout'.
#include <iomanip> // To use 'std::boolalpha'.

#define GENERATE_HAS_MEMBER(member)\
template<class T>\
class HasMember_ ##member\
{\
private:\
	using Yes = char[2];\
	using No = char[1];\
\
	struct Fallback\
	{\
		int member;\
	};\
\
	struct Derived: T, Fallback\
	{\
	};\
\
	template<class U>\
	static No& test(decltype(U::member)*);\
\
	template<typename U>\
	static Yes& test(U*);\
\
public:\
\
	static constexpr bool RESULT = sizeof(test<Derived>(nullptr)) == sizeof(Yes);\
};\
\
template < class T >\
struct has_member_ ##member\
: public std::integral_constant<bool, HasMember_##member<T>::RESULT>\
{\
\
};

// Creates 'has_member_att'.
GENERATE_HAS_MEMBER(att)
// Creates 'has_member_func'.
GENERATE_HAS_MEMBER(func)

struct A
{
	int att;
	void func(double);
};
struct B
{
	char att[3];
	double func(const char*);
};
struct C: A, B
{
};
// It will also work with ambiguous members.
int main()
{
	std::cout << std::boolalpha << std::endl;
	std::cout << "'att' in 'C' : " << has_member_att<C>::value << std::endl; // <type_traits>-like interface.
	std::cout << "'func' in 'C' : " << has_member_func<C>() << std::endl; // Implicitly convertible to 'bool'.
}
// g++ --std=c++11 test.cpp
```

> NOTE: 上述程序的输出如下：
>
> ```
> 'att' in 'C' : true
> 'func' in 'C' : true
> ```



#### Detecting member types

The above example can be adapted to detect existence of a **member type**, be it a nested class or a `typedef`, even if it is incomplete. This time, the **ambiguity** would be created by adding to `Fallback` a nested type with the same name as the macro argument.

```c++
#include <type_traits> // To use 'std::integral_constant'.
#include <iostream> // To use 'std::cout'.
#include <iomanip> // To use 'std::boolalpha'

#define GENERATE_HAS_MEMBER_TYPE(Type)\
template<class T>\
class HasMemberType_ ##Type\
{\
private:\
	using Yes = char[2];\
	using No = char[1];\
	struct Fallback\
	{\
		struct Type\
		{\
		};\
	};\
	struct Derived: T, Fallback\
	{\
	};\
\
	template<class U>\
	static No& test(typename U::Type*);\
\
	template<typename U>\
	static Yes& test(U*);\
\
public:\
	static constexpr bool RESULT = sizeof(test<Derived>(nullptr)) == sizeof(Yes);\
};\
\
template<class T>\
struct has_member_type_ ##Type\
: public std::integral_constant<bool, HasMemberType_##Type<T>::RESULT>\
{\
};

// Creates 'has_member_type_Foo'.
GENERATE_HAS_MEMBER_TYPE(Foo)
struct A
{
	struct Foo;
};
struct B
{
	using Foo = int;
};
struct C: A, B
{
};

// Will also work on incomplete or ambiguous types.
int main()
{
	std::cout << std::boolalpha;
	std::cout << "'Foo' in 'C' : " << has_member_type_Foo<C>::value << "\n";
}

```

This is much like the **member detection** macro that declares a data member of the same name as the macro argument. An overload set of two test functions is then created, just like the other examples. The first version can only be instantiated if the type of `U::Type` can be used unambiguously. This type can be used only if there is exactly one instance of Type in `Derived`, i.e. there is no `Type` in `T`. If `T` has a member type `Type`, it is garanteed to be different than `Fallback::Type`, since the latter is a unique type, hence creating the ambiguity. This leads to the second version of test being instantiated in case the substitution
fails, meaning that `T` has indeed a member type `Type`. Since no objects are ever created (this is entirely resolved by compile-time type checking), `Type` can be an incomplete type or be ambiguous inside `T`; only the name matters. We then wrap the result in a class inheriting from `integral_constant`, just like before to provide the same interface as the standard library.

#### Detecting overloaded member functions

A variation of the **member detector idiom** can be used to detect existence of a specific member function in a class even if it is overloaded.

```c++
template<typename T, typename RESULT, typename ARG1, typename ARG2>
class HasPolicy
{
	template<typename U, RESULT (U::*)(ARG1, ARG2)>
	struct Check;
	template<typename U> static char func(Check<U, &U::policy> *);
	template<typename U> static int func(...);
public:
	typedef HasPolicy type;
	enum
	{
		value = sizeof(func<T>(0)) == sizeof(char)
	};
};

```

The `HasPolicy` template above checks if `T` has a member function called `policy` that takes two parameters `ARG1`, `ARG2` and returns `RESULT`. Instantiation of Check template succeeds only if `U` has a `U::policy` member function that takes two parameters and returns `RESULT`. Note that the first type parameter of `Check` template is a type whereas the second parameter is a pointer to a member function in the same type. If `Check` template cannot be instantiated, the only remaining `func` that returns an `int` is instantiated. The size of the return value of `func` eventually determines the answer of the type-trait: `true` or `false`.

### Known Issues

Will not work if the class checked for member is declared `final` (C++11 keyword).

Will not work to check for a member of a union (unions cannot be base classes).

## Detect member function

- stackoverflow [Templated check for the existence of a class member function?](https://stackoverflow.com/questions/257288/templated-check-for-the-existence-of-a-class-member-function)

- stackoverflow [Check if a class has a member function of a given signature](https://stackoverflow.com/questions/87372/check-if-a-class-has-a-member-function-of-a-given-signature)

### [A](https://stackoverflow.com/a/257382): SFINA+`decltype`

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



### [A](https://stackoverflow.com/a/9154394): expression SFINAE 

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





## cppreference [std::is_member_function_pointer](https://en.cppreference.com/w/cpp/types/is_member_function_pointer)



## [sfinae_tostring_ex.cpp](https://gist.github.com/fenbf/d2cd670704b82e2ce7fd#file-sfinae_tostring_ex-cpp)



https://gist.github.com/fenbf/d2cd670704b82e2ce7fd

## [has_member.hpp](https://gist.github.com/maddouri/0da889b331d910f35e05ba3b7b9d869b#file-has_member-hpp)

https://gist.github.com/fenbf/d2cd670704b82e2ce7fd



## reddit SFINAE Checking for Existence



## Detection Idiom TO READ

https://www.codeplay.com/portal/09-15-17-detection-idiom-a-stopgap-for-concepts

https://stackoverflow.com/questions/45249985/how-to-require-an-exact-function-signature-in-the-detection-idiom

https://humanreadablemag.com/issues/1/articles/cpp-evolution-via-detection-idiom-lens

https://blog.tartanllama.xyz/detection-idiom/

https://stackoverflow.com/questions/257288/templated-check-for-the-existence-of-a-class-member-function/9154394#9154394

https://stackoverflow.com/questions/87372/check-if-a-class-has-a-member-function-of-a-given-signature

https://stackoverflow.com/questions/257288/templated-check-for-the-existence-of-a-class-member-function

https://stackoverflow.com/questions/18570285/using-sfinae-to-detect-a-member-function
# Template(模板化) function signature

## 实现思路: primary template + specialization

在下面文章中，对它的实现进行了分析:

1、`C++\Programming-paradigm\Generic-programming\Pre-C++20-concept\Detection-idiom`

2、`Compile-template`章节

## Example code

1、sean-parent [Polymorphic Task Template in Ten](https://sean-parent.stlab.cc/presentations/2017-11-11-lightning-talk/lightning-talk.pdf)

```C++
#include<memory>
#include<iostream>
#include<utility>
#include<type_traits>

using namespace std;

template<class >
class task;

//...

template<class R, class ... Args>
class task<R(Args...)>
{
	struct concept;
	template<class F>
	struct model;
	unique_ptr<concept> _p;
public:
	template<class F>
	task(F &&f) :
					_p(make_unique < model<decay_t<F>> > (forward<F>(f)))
	{
	}
	R operator()(Args ... args)
	{
		return _p->_invoke(forward<Args>(args)...);
	}
};

template<class R, class ... Args>
struct task<R(Args...)>::concept
{
	virtual ~concept() = default;
	virtual R _invoke(Args&&...) = 0;
};

template<class R, class ... Args>
template<class F>
struct task<R(Args...)>::model final : concept
{
	template<class G>
	model(G &&f) :
					_f(forward<G>(f))
	{
	}
	R _invoke(Args &&... args) override
	{
		return invoke(_f, forward<Args>(args)…);
	}
	F _f;
};

```

2、stackoverflow [Check if a class has a member function of a given signature](https://stackoverflow.com/questions/87372/check-if-a-class-has-a-member-function-of-a-given-signature) # [A](https://stackoverflow.com/a/16824239)

```C++
#include <type_traits>
#include <iostream>
// Primary template with a static assertion
// for a meaningful error message
// if it ever gets instantiated.
// We could leave it undefined if we didn't care.

template<typename, typename T>
struct has_serialize
{
	static_assert(std::integral_constant<T, false>::value, "Second template parameter needs to be of function type.");
};

// specialization that does the checking
template<typename C, typename Ret, typename ... Args>
struct has_serialize<C, Ret(Args...)>
{
private:
	template<typename T>
	static constexpr auto check(T*) -> typename
	std::is_same<
	decltype( std::declval<T>().serialize( std::declval<Args>()... ) ),
	Ret    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
					>::type;// attempt to call it and see if the return type is correct

	template<typename >
	static constexpr std::false_type check(...);

	typedef decltype(check<C>(0)) type;

public:
	static constexpr bool value = type::value;
};

struct X
{
	int serialize(const std::string&)
	{
		return 42;
	}
};

struct Y: X
{
};

struct Z
{
};
int main()
{
	std::cout << has_serialize<Y, int(const std::string&)>::value << std::endl; // will print 1
	std::cout << has_serialize<Z, int(const std::string&)>::value << std::endl; // will print 1
}
// g++ --std=c++11 test.cpp

```


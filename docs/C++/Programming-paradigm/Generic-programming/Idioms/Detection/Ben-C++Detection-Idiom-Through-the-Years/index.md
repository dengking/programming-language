# **Ben's Blog** [C++ Detection Idiom Through the Years](https://people.eecs.berkeley.edu/~brock/blog/detection_idiom.php)

This is a record, mostly for my own benefit, of different ways to write the detection idiom in C++. If you're looking for a tutorial-style guide to the detection idiom, I highly recommend Sy Brand's [blog post](https://blog.tartanllama.xyz/detection-idiom/) about the detection idiom as a stand-in(替代) for concepts.

## What can detection idiom do?

The **detection idiom** is what allows **compile-time type introspection** in C++. Using the detection idiom, we can check if a type has methods, type aliases, or members that fit a particular interface. This allows library writers to create metaprogramming facilities that are much more expressive. The user can plug(插入) any type she likes into a metafunction as long as it fits the required interface.

## C++ 20 concepts

C++ 20 will have a first-class facility for compile-type type introspection called concepts. Concepts are more concise and explicit than current iterations of the detection idiom and will result in better compiler errors. For now, though, we emulate concepts using the detection idiom. All current iterations of the detection idiom depend on SFINAE, which is the property that certain errors that occur during overload resolution are not fatal errors, but just result in substitution failure, with the compiler going on to try the next candidate. We can use this property to check whether a type has a particular interface, returning true if it does and false if it does not.

> NOTE: 
>
> 1、上面这段话中关于SFINAE的描述是有些狭隘的，"which is the property that certain errors that occur during overload resolution are not fatal errors"，SFINAE不仅仅发生于overload resolution，相反，它是发生于所有的template instantiation

## Detection idiom implementation 

### Modern Detection Idiom: Use Fundamentals TS v2

C++ Fundamentals TS v2 provides a super easy way to use the detection idiom in C++ through [`is_detected`](https://en.cppreference.com/w/cpp/experimental/is_detected). All the user needs to do is write a template struct that checks for the attributes the user wants, then hand that off to `is_detected`. Here's an example, where we want to ensure that a type has a method `get()`

```C++
template<typename T>
using get_t = decltype(std::declval<T>().get());

template<typename >
using supports_get = std::experimental::is_detected<get_t, T>;

main()
{

	std::cout << supports_get<foo>::value << std::endl;

	...
}

```

You can use your new `supports_get` template metafunction to create partial specializations, SFINAE out, etc. You can use `conjunction` to check for multiple traits and `is_same` to check for the return value. Both of these are in [`type_traits`](https://en.cppreference.com/w/cpp/header/type_traits). The downside of this is that either your compiler needs to include `<experimental/type_traits>`, or you need to include the the implementation of `is_detected` from cppreference.

### C++11 Detection Idiom: Use Expression SFINAE

In an older style of the detection idiom, you create a single class and use **expression SFINAE** on the return type of methods in that class to return true if the method is detected and false if the method does not exist. In the example below, we're calling `test_get` with an `int` parameter. This causes the first `test_get` method, which takes an `int` as an argument, to be preferred over the second `test_get` method, which takes anything (`...`) as an argument. The first method will only be enabled, however, if the expression inside the `decltype` can resolve without errors. The return type for both functions is bool, since `decltype` will return the type of the last expression (technically, this is due to the comma operator).

> NOTE: 
>
> 1、理解上面这段话的一个前提是理解C++编译流程，参见`Compile-flow`章节
>
> 2、关于comma operator，参见 `Built-in-comma-operator` 章节

```C++
template<typename T>
struct has_get
{

	template<typename U>
	static constexpr
	decltype(std::declval<U>().get(), bool()) test_get(int)
	{
		return true;
	}

	template<typename U>
	static constexpr bool test_get(...)
	{
		return false;
	}

	static constexpr bool value = test_get<T>(int());
};

```


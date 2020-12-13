# Callable concept/named requirement

Callable named requirement是一个比function object/function更加宽泛的概念。

C++对callable named requirement的实现、描述是符合named requirement的原则的: 

1 它是behavior-based(在cppreference [C++ named requirements: *Callable*](https://en.cppreference.com/w/cpp/named_req/Callable)中有所描述)

2 C++ standard library提供了: 

- 操作callable的uniform interface/magic function
- trait

> NOTE: 
>
> 1 uniform interface在`./interface`章节中进行了描述
>
> 2 关于 named requirement，参见 `C++\Language-reference\Template\Programming-paradigm\Generic-programming\Named-requirements` 章节

## What's new in C++?

首先搞清楚C++提出callable concept/named requirement目的、它要解决的问题。由于它的uniform interface/magic function是`std::invoke`，所以我们搞清楚`std::invoke`所解决的问题和搞清楚callable concept/named requirement所要解决的问题是相同的:

在`C++\Language-reference\Functions\Function`的"Function call expression"章节中，我们知道了C++中有多种类型都能够被用于function call expression，但是它的syntax并不统一，这就导致了generic programming需要由programmer处理多种情况: 需要由programmer进行abstraction和implementation，造成programmer的负担；`std::invoke`就是提供了对所有这些callable的uniform invokation interface，显然这简化了generic programming。

### bfilipek [C++20 Ranges, Projections, std::invoke and if constexpr#`std::invoke`, C++17](https://www.bfilipek.com/2020/10/understanding-invoke.html#stdinvoke-c17)

The primary motivation for this helper function is the issue with a **non-uniform syntax** for various **callable objects**.

For example, if you have a regular function object, you can just call:

```C++
func(args...)
```

But if you have a pointer to a member function then the syntax is different:

```C++
(obj.*funcPtr)(args...)
```

This might be an issue when you write a function template like:

```cpp
template <typename T, typename F>
void CallOnRange(T& container, F f) {
    for (auto&& elem : container)
        f(elem);
}

std::vector v { 1, 2, 3, 4 };
CallOnRange(v, [](int i) { std::cout << i << '\n'; });
```

`CallOnRange` works nicely for a regular function object type (like a lambda or a function pointer), but won’t work on pointers to member functions. In that case, we need to make additional overload:

```cpp
template <typename TCont, typename Type, typename U>
void CallOnRange(TCont& container, Type U::* f)
{
    for (auto&& elem : container)
        (elem.*f)();
}
```

See the experiments at [@Wandbox](https://wandbox.org/permlink/tQYV7YjWln5IrNZ9)

That’s why, for those special cases, we can use `std::invoke` which gives us **uniform syntax call**:

```cpp
template <typename T, typename F>
void CallOnRangeInvoke(T& container, F f)
{
    for (auto&& elem : container)
        std::invoke(f, elem);
}
```

In short `invoke(f, t1, t2, ..., tN)` (proposed in [N4169](https://wg21.link/N4169) and accepted for C++17) can handle the following cases::

- function objects: like `func(arguments...)`
- pointers to member functions `(obj.*funcPtr)(arguments...)` + pointers and references
- pointer to member data `obj.*pdata` + pointers and references

As you can see, this expression creates a nice **abstraction** over several options that you can “call” something. No matter if that’s a pointer to a member function, a regular callable object, or even a data member.

#### How it works?

In principle, the function has to check the type of the **callable object** and then make a right call to and forward the arguments.

Fortunately, since C++17 all of those checks can be done with a relatively easy way! There’s no need for complicated **SFINAE tricks**, and in most of the cases the code can leverage `if constexpr`.

To understand the code, we can look at the sample implementation [@cppreference](https://en.cppreference.com/w/cpp/utility/functional/invoke).

The main function `std::invoke` wraps the call to the `INVOKE` template function that has two overloads:

Here’s one for a regular function:

```cpp
template <class F, class... Args>
constexpr decltype(auto) INVOKE(F&& f, Args&&... args) {
      return std::forward<F>(f)(std::forward<Args>(args)...);
}
```

And then the overload for pointers to member functions or for data members:

```cpp
template <class T, class Type, class T1, class... Args>
constexpr decltype(auto) INVOKE(Type T::* f, T1&& t1, Args&&... args) {
    if constexpr (std::is_member_function_pointer_v<decltype(f)>) {
        if constexpr (std::is_base_of_v<T, std::decay_t<T1>>)
            return (std::forward<T1>(t1).*f)(std::forward<Args>(args)...);
        else if constexpr (is_reference_wrapper_v<std::decay_t<T1>>)
            return (t1.get().*f)(std::forward<Args>(args)...);
        else
            return ((*std::forward<T1>(t1)).*f)(std::forward<Args>(args)...);
    } else {
        static_assert(std::is_member_object_pointer_v<decltype(f)>);
        static_assert(sizeof...(args) == 0);
        if constexpr (std::is_base_of_v<T, std::decay_t<T1>>)
            return std::forward<T1>(t1).*f;
        else if constexpr (is_reference_wrapper_v<std::decay_t<T1>>)
            return t1.get().*f;
        else
            return (*std::forward<T1>(t1)).*f;
    }
}
```

One note: in C++17 `std::invoke` wasn’t specified with `constexpr`, it was added in C++20.

Thanks to `if constexpr` (added in C++17) we can read this function in a “normal” way. As we can see the function checks

- if the callable is a `is_member_function_pointer` - this is a type trait available in the standard library, see [here](https://en.cppreference.com/w/cpp/types/is_member_function_pointer)
- otherwise we can assume that it’s a pointer to a non-static data member. For this case, there can be no arguments passed, only the

Here’s a simple code that demonstrates pointers to non static data members:

```cpp
struct GameActor {
    std::string name;
    std::string desc;
};

int main(){
    std::string GameActor::* pNameMember = &GameActor::name;

    GameActor actor { "enemy", "super evil" };
    std::cout << actor.name << " is " << actor.desc << '\n';
    actor.*pNameMember = "friend";
    pNameMember = &GameActor::desc;
    actor.*pNameMember = "very friendly";
    std::cout << actor.name << " is " << actor.desc << '\n';
}
```

See the code [@Wandbox](https://wandbox.org/permlink/uerGvfhidX9LzhOq)

If we look closer in the function implementation, you can also spot that `std::invoke` then have three more cases:

- regular call - no dereferencing needed
- via reference wrapper - so we have to call `.get()` to get the object
- in other cases we assume it’s a pointer and then we need to dereference it. This supports, for example, smart pointers.

```cpp
struct GameActor {
    std::string name;
    std::string desc;
};

int main(){
    GameActor actor { "robot", "a friendly type" };
    std::cout << "actor is: " << std::invoke(&GameActor::name, actor) << '\n';

    auto ptr = std::make_unique<GameActor>("space ship", "slow");
    std::cout << "actor is: " << std::invoke(&GameActor::name, ptr) << '\n';
}
```

See code [@Wandbox](https://wandbox.org/permlink/1n2G8lrKs3QpKXQz)

We can also look at more sophisticated, production-ready implementation at MSVC/STL code here [@Github](https://github.com/microsoft/STL/blob/master/stl/inc/type_traits#L1588). Surprisingly the code for `invoke` is located in the `type_traits` header and not in `<functional>`.

## (Bonus) Compilation Speed And Costs

If you wonder about the cost of extra compilation time and runtime performance of code with `std::invoke`, you can see this early preview text available for Patrons:
[Understanding std::invoke, CppCon 2020 Presentation notes, Compilation Speed of std::invoke](https://www.patreon.com/posts/43262190)
Or see all benefits at this summary site: [Exlusive C++ Content](https://www.bfilipek.com/p/extra-content.html).



## open-std [N4169](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4169.html)

The aim of this proposal is to introduce the function template `invoke` that provide uniform semantics for invoking all C++ callable types which includes: 

1 function pointers, 

2 member pointers and 

3 functors. 

The behaviour of the function is defined in terms of `*INVOKE*` expression.

### Implementability

Proposed `invoke` function template may be implemented in terms of existing C++11 standard library components:

```C++
#include <type_traits> // std::enable_if
#include <functional> // std::mem_fn
template<typename Functor, typename ... Args>
typename std::enable_if<std::is_member_pointer<typename std::decay<Functor>::type>::value, typename std::result_of<Functor&& (Args&&...)>::type>::type invoke(Functor &&f, Args &&... args)
{
	return std::mem_fn(f)(std::forward<Args>(args)...);
}

template<typename Functor, typename ... Args>
typename std::enable_if<!std::is_member_pointer<typename std::decay<Functor>::type>::value, typename std::result_of<Functor&& (Args&&...)>::type>::type invoke(Functor &&f, Args &&... args)
{
	return std::forward<Functor>(f)(std::forward<Args>(args)...);
}
// g++ --std=c++11 test.cpp
```

An `constexpr` implemenatation may be found at: https://github.com/tomaszkam/proposals/blob/master/invoke/invoke_cpp11.hpp.

> NOTE: 上述链接已经失效，我找到正确的实现，fork到了我的GitHub中，下面是实现: 
>
> https://github.com/dengking/proposals/tree/master/implementation/invoke

## cppreference [C++ named requirements: *Callable*](https://en.cppreference.com/w/cpp/named_req/Callable)

A ***Callable*** type is a type for which the **INVOKE operation** (used by, e.g., [std::function](https://en.cppreference.com/w/cpp/utility/functional/function), [std::bind](https://en.cppreference.com/w/cpp/utility/functional/bind), and [std::thread::thread](https://en.cppreference.com/w/cpp/thread/thread/thread)) is applicable. This operation may be performed explicitly using the library function [std::invoke](https://en.cppreference.com/w/cpp/utility/functional/invoke). (since C++17)

> NOTE: 
>
> **INVOKE operation** 说明是 behavior-based。
>
> 关于[std::invoke](https://en.cppreference.com/w/cpp/utility/functional/invoke)，参见 `./Interface` 章节

### Requirements

The type `T` satisfies *Callable* if

Given

- `f`, an object of type `T`
- `ArgTypes`, suitable list of argument types
- `R`, suitable return type

The following expressions must be valid:

|                          Expression                          |                     Requirements                     |
| :----------------------------------------------------------: | :--------------------------------------------------: |
| `INVOKE<R>(f,` [std::declval](http://en.cppreference.com/w/cpp/utility/declval)`<ArgTypes>()...)` | the expression is well-formed in unevaluated context |

where *INVOKE(f, t1, t2, ..., tN)* is defined as follows:

> NOTE: 从下面的描述可以看出，callable concept包含了C++中所有的可以进行invoke的情况，它是对它们的统一描述，显然这是uniform。

1 if `f` is a [pointer to member function](https://en.cppreference.com/w/cpp/language/pointer#Pointers_to_member_functions) of class `T`:

- If [std::is_base_of](http://en.cppreference.com/w/cpp/types/is_base_of)`<T,` [std::remove_reference_t](http://en.cppreference.com/w/cpp/types/remove_reference)`<decltype(t1)>>::value` is true, then `INVOKE(f, t1, t2, ..., tN)` is equivalent to `(t1.*f)(t2, ..., tN)`
- otherwise, if [std::remove_cvref_t](http://en.cppreference.com/w/cpp/types/remove_cvref)`<decltype(t1)>` is a specialization of [std::reference_wrapper](https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper), then `INVOKE(f, t1, t2, ..., tN)` is equivalent to `(t1.get().*f)(t2, ..., tN)` (since C++17)
- otherwise, if `t1` does not satisfy the previous items, then `INVOKE(f, t1, t2, ..., tN)` is equivalent to `((*t1).*f)(t2, ..., tN)`.

2 otherwise, if N == 1 and `f` is a [pointer to data member](https://en.cppreference.com/w/cpp/language/pointer#Pointers_to_data_members) of class `T`:

- If [std::is_base_of](http://en.cppreference.com/w/cpp/types/is_base_of)`<T,` [std::remove_reference_t](http://en.cppreference.com/w/cpp/types/remove_reference)`<decltype(t1)>>::value` is true, then `INVOKE(f, t1)` is equivalent to `t1.*f`
- otherwise, if [std::remove_cvref_t](http://en.cppreference.com/w/cpp/types/remove_cvref)`<decltype(t1)>` is a specialization of [std::reference_wrapper](https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper), then `INVOKE(f, t1)` is equivalent to `t1.get().*f` (since C++17)
- otherwise, if `t1` does not satisfy the previous items, then `INVOKE(f, t1)` is equivalent to `(*t1).*f`

3 otherwise, `INVOKE(f, t1, t2, ..., tN)` is equivalent to `f(t1, t2, ..., tN)` (that is, `f` is a [*FunctionObject*](https://en.cppreference.com/w/cpp/named_req/FunctionObject))

### Notes

For **pointers to member functions** and **pointers to data members**, `t1` may be a regular pointer or an object of class type that overloads `operator*`, such as [std::unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr) or [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr).

[Pointers to data members](https://en.cppreference.com/w/cpp/language/pointer#Pointers_to_data_members) are *Callable*, even though no function calls take place.



### Standard library

In addition, the following standard library facilities accept any *Callable* type (not just [*FunctionObject*](https://en.cppreference.com/w/cpp/named_req/FunctionObject))

| API                                                          |
| ------------------------------------------------------------ |
| [std::function](https://en.cppreference.com/w/cpp/utility/functional/function) |
| [std::bind](https://en.cppreference.com/w/cpp/utility/functional/bind) |
| [std::result_of](https://en.cppreference.com/w/cpp/types/result_of) |
| [std::thread::thread](https://en.cppreference.com/w/cpp/thread/thread/thread) |
| [std::call_once](https://en.cppreference.com/w/cpp/thread/call_once) |
| [std::async](https://en.cppreference.com/w/cpp/thread/async) |
| [std::packaged_task](https://en.cppreference.com/w/cpp/thread/packaged_task) |
| [std::reference_wrapper](https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper) |


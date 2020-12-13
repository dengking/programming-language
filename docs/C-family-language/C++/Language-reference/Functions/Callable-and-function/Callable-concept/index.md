# Callable concept

callable是一个比function更加宽泛的概念，它是behavior-based(在cppreference [C++ named requirements: *Callable*](https://en.cppreference.com/w/cpp/named_req/Callable)中有所描述)，C++提供了操作callable的uniform interface。



## cppreference [C++ named requirements: *Callable*](https://en.cppreference.com/w/cpp/named_req/Callable)

A ***Callable*** type is a type for which the **INVOKE operation** (used by, e.g., [std::function](https://en.cppreference.com/w/cpp/utility/functional/function), [std::bind](https://en.cppreference.com/w/cpp/utility/functional/bind), and [std::thread::thread](https://en.cppreference.com/w/cpp/thread/thread/thread)) is applicable. This operation may be performed explicitly using the library function [std::invoke](https://en.cppreference.com/w/cpp/utility/functional/invoke). (since C++17)

> NOTE: **INVOKE operation** 说明是 behavior-based。
>
> 关于[std::invoke](https://en.cppreference.com/w/cpp/utility/functional/invoke)，参见 `C++\Language-reference\Functions\Generic-programming\Invokation\std-invoke` 章节

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

| [std::function](https://en.cppreference.com/w/cpp/utility/functional/function) |
| ------------------------------------------------------------ |
| [std::bind](https://en.cppreference.com/w/cpp/utility/functional/bind) |
| [std::result_of](https://en.cppreference.com/w/cpp/types/result_of) |
| [std::thread::thread](https://en.cppreference.com/w/cpp/thread/thread/thread) |
| [std::call_once](https://en.cppreference.com/w/cpp/thread/call_once) |
| [std::async](https://en.cppreference.com/w/cpp/thread/async) |
| [std::packaged_task](https://en.cppreference.com/w/cpp/thread/packaged_task) |
| [std::reference_wrapper](https://en.cppreference.com/w/cpp/utility/functional/reference_wrapper) |
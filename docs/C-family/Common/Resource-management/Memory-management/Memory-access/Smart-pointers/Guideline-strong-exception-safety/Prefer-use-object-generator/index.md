# Smart pointer object generator

都触及到了相同的问题:

1、Exception safety 

为了实现strong exception safety。

2、CppCoreGuidelines [R: Resource management # Allocation and deallocation rule summary](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#r-resource-management):

[R.10: Avoid `malloc()` and `free()`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-mallocfree)



[R.11: Avoid calling `new` and `delete` explicitly](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-newdelete)



[R.12: Immediately give the result of an explicit resource allocation to a manager object](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-immediate-alloc)



[R.13: Perform at most one explicit resource allocation in a single expression statement](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-single-alloc)



## Prefer use object generator

CppCoreGuidelines 中的两个主要rule: 

1、[R.11: Avoid calling `new` and `delete` explicitly](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-newdelete)

2、[R.13: Perform at most one explicit resource allocation in a single expression statement](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rr-single-alloc)



## stackoverflow [Exception safety and make_unique](https://stackoverflow.com/questions/19472550/exception-safety-and-make-unique)



Just to clarify, using `make_unique` only adds exception safety when you have multiple allocations in an expression, not just one, correct? For example

```cpp
void f(T*);

f(new T);
```

is perfectly exception safe (as far as allocations and stuff), while

```cpp
void f(T*, T*);

f(new T, new T);
```

is not, correct?

> NOTE: 提问者所问的是: code1是exception safe而code2不是exception safe。

### **Comments**

`multiple allocations in an expression` but each of those allocations via `new T` *is* a separate expression, so you have the opposite: one allocation per each of multiple expressions. That the function call is *itself* an expression containing the other 2 doesn't change that. But aside from the wording, you were on the right track. By my reading, the rule, as formulated by Sutter, is to perform each allocation within its own **statement** or **sequence** them by returning from a separate function call, thereby avoiding weird ordering & leaks: [gotw.ca/gotw/056.htm](http://www.gotw.ca/gotw/056.htm) (oldie-but-goodie) – [underscore_d](https://stackoverflow.com/users/2757035/underscore-d) [Aug 13 '16 at 11:25](https://stackoverflow.com/questions/19472550/exception-safety-and-make-unique/19472607#comment65220154_19472550) 

> NOTE: 这个comment是非常好的，[gotw.ca/gotw/056.htm](http://www.gotw.ca/gotw/056.htm) (oldie-but-goodie) 是非常好的文章，已经收录了。

### [A](https://stackoverflow.com/a/19472607)

Not only when you have multiple allocations, but whenever you can throw at different places. Consider this:

```cpp
f(make_unique<T>(), function_that_can_throw());
```

Versus:

```cpp
f(unique_ptr<T>(new T), function_that_can_throw());
```

In the second case, the compiler is allowed to call (in order):

- `new T`
- `function_that_can_throw()`
- `unique_ptr<T>(...)`

Obviously if `function_that_can_throw` actually throws then you leak. `make_unique` prevents this case.

And of course, a second allocation (as in your question) is just a special case of `function_that_can_throw()`.

> NOTE: 这段话其实所说的是这种情况: `g(std::unique_ptr<T>(new T), std::unique_ptr<T>(new T));`
>
> 一旦第二个`std::unique_ptr<T>(new T)` throw，则可能leak。

As a general rule of thumb, just use `make_unique` so that your code is consistent. It is always correct (read: exception-safe) when you need a `unique_ptr`, and it doesn't have any impact on performance, so there is no reason not to use it (while actually *not* using it introduces a lot of gotchas).

#### **Comments**

Exception safety is not (just) about "no exceptions" but rather about "guaranteeing the right behaviour when you get an exception". The `make_unique` documentation is clear: no effect if there is an exception, this is a strong guarantee (see [en.wikipedia.org/wiki/Exception_safety](https://en.wikipedia.org/wiki/Exception_safety)). And the `unique_ptr` is not yet constructed so, again, no effect; the exception is simply propagated. – [syam](https://stackoverflow.com/users/2070725/syam) [Sep 24 '20 at 19:18](https://stackoverflow.com/questions/19472550/exception-safety-and-make-unique/19472607#comment113266965_19472607) 

> NOTE: 实现了strong exception safety

### [A](https://stackoverflow.com/a/44932594)

As of C++17, the exception safety issue is fixed by a rewording of [[expr.call\]](http://eel.is/c++draft/expr.call#5)

> The initialization of a parameter, including every associated value computation and side effect, is indeterminately sequenced with respect to that of any other parameter.

Here *indeterminately sequenced* means that one is sequenced before another, but it is not specified which.

```cpp
f(unique_ptr<T>(new T), function_that_can_throw());
```

Can have only two possible order of execution

1. `new T` `unique_ptr<T>::unique_ptr` `function_that_can_throw`
2. `function_that_can_throw` `new T` `unique_ptr<T>::unique_ptr`

Which means it is now exception safe.

### [A](https://stackoverflow.com/a/19472593)

> NOTE: 非常好的总结

I'd think you'd be better off comparing things actually using `std::unique_ptr<T>`:

```cpp
void f(std::unique_ptr<T>);

f(std::unique_ptr<T>(new T));
f(std::make_unique<T>());
```

Neither of these calls can leak if there is an exception being thrown. However

```cpp
void f(std::unique_ptr<T>, std::unique_ptr<T>);

g(std::unique_ptr<T>(new T), std::unique_ptr<T>(new T));
g(std::make_unique<T>(), std::make_unique<T>());
```

In this case, the version using `std::unique_ptr<T>` explicitly can leak if an exception is thrown (because the compiler might start evaluating the `new`-expressions before constructing either of the temporaries).

## stackoverflow [Differences between std::make_unique and std::unique_ptr with new](https://stackoverflow.com/questions/22571202/differences-between-stdmake-unique-and-stdunique-ptr-with-new)

Does `std::make_unique` have any efficiency benefits like `std::make_shared`?

Compared to manually constructing `std::unique_ptr`:

```cpp
std::make_unique<int>(1);         // vs
std::unique_ptr<int>(new int(1));
```

### [A](https://stackoverflow.com/a/22571331)

The motivation behind `make_unique` is primarily two-fold:

1、`make_unique` is safe for creating temporaries, whereas with explicit use of `new` you have to remember the rule about not using unnamed temporaries.

> NOTE: 上述"rule about not using unnamed temporaries"要如何理解？

```cpp
foo(make_unique<T>(), make_unique<U>()); // exception safe

foo(unique_ptr<T>(new T()), unique_ptr<U>(new U())); // unsafe*
```

2、The addition of `make_unique` finally means we can tell people to 'never' use `new` rather than the previous rule to "'never' use `new` except when you make a `unique_ptr`".

There's also a third reason:

- `make_unique` does not require redundant type usage. `unique_ptr<T>(new T())` -> `make_unique<T>()`

None of the reasons involve improving runtime efficiency the way using `make_shared` does (due to avoiding a second allocation, at the cost of potentially higher peak memory usage).

`*` It is expected that C++17 will include a rule change that means that this is no longer unsafe. See C++ committee papers [P0400R0](http://wg21.link/p0400r0) and [P0145R3](http://wg21.link/p0145r3).

> NOTE: 在前面的 stackoverflow [Exception safety and make_unique](https://stackoverflow.com/questions/19472550/exception-safety-and-make-unique) # [A](https://stackoverflow.com/a/44932594) 中已经对此进行了解释。
>
> 

### [A](https://stackoverflow.com/a/22571261)

`std::make_unique` and `std::make_shared` are there for two reasons:

1. So that you don't have to explicitly list the template type arguments.
2. Additional exception safety over using `std::unique_ptr` or `std::shared_ptr` constructors. (See the Notes section [here](http://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared).)

It's not really about runtime efficiency. There is the bit about the control block and the `T` being allocated all at once, but I think that's more a bonus and less a motivation for these functions to exist.

## stackoverflow [Advantages of using std::make_unique over new operator [duplicate]](https://stackoverflow.com/questions/37514509/advantages-of-using-stdmake-unique-over-new-operator)

### [A](https://stackoverflow.com/a/37514601)

#### Advantages

1、`make_unique` teaches users "never say `new`/`delete` and `new[]`/`delete[]`" without disclaimers.

> NOTE: 翻译如下: 
>
> “make_unique”教给用户“在没有免责声明的情况下绝不说‘new’/‘delete’和‘new[]’/‘delete[]’”。

2、`make_unique` shares two advantages with `make_shared` (excluding(除了) the third advantage, increased efficiency). 

First, `unique_ptr<LongTypeName> up(new LongTypeName(args))` must mention `LongTypeName` twice, while `auto up = make_unique<LongTypeName>(args)` mentions it once.

3、`make_unique` prevents the unspecified-evaluation-order leak triggered by expressions like `foo(unique_ptr<X>(new X)`, `unique_ptr<Y>(new Y))`. (Following the advice "never say `new`" is simpler than "never say `new`, unless you immediately give it to a named `unique_ptr`".)

4、`make_unique` is carefully implemented for exception safety and is recommended over directly calling `unique_ptr` constructors.

> NOTE: 提高exception safety

#### When not to use `make_unique`

- Don't use `make_unique` if you need a custom deleter or are adopting a raw pointer from elsewhere.

#### Sources

1、[Proposal of `std::make_unique`](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3588.txt).

2、[Herb Sutter's GotW #89 Solution: Smart Pointers](https://herbsutter.com/2013/05/29/gotw-89-solution-smart-pointers/)

## When use raw new？

### stackoverflow [Differences between std::make_unique and std::unique_ptr with new](https://stackoverflow.com/questions/22571202/differences-between-stdmake-unique-and-stdunique-ptr-with-new) # [A](https://stackoverflow.com/a/45210410)

A reason why you would have to use `std::unique_ptr(new A())` or `std::shared_ptr(new A())` directly instead of `std::make_*()` is being unable to access the constructor of class `A` outside of current scope.



### stackoverflow  [Differences between std::make_unique and std::unique_ptr with new](https://stackoverflow.com/questions/22571202/differences-between-stdmake-unique-and-stdunique-ptr-with-new)  # [A](https://stackoverflow.com/a/22571331) # comment

One reason I once had to use `std::unique_ptr<T>(new T())` was because the constructor of T was private. Even if the call to std::make_unique was in a public factory method of class T, it didn't compile because one of the underlying methods of `std::make_unique` could not access the private constructor. I didn't want to make that method friend because I didn't want to rely on the implementation of std::make_unique. So the only solution was, calling new in my factory method of class T, and then wrap it in an `std::unique_ptr<T>`. – [Patrick](https://stackoverflow.com/users/163551/patrick) [Jun 5 '19 at 7:09](https://stackoverflow.com/questions/22571202/differences-between-stdmake-unique-and-stdunique-ptr-with-new#comment99503613_22571331) 
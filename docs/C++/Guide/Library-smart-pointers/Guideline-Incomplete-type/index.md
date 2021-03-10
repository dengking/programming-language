# Incomplete types and `shared_ptr` / `unique_ptr`

之前在阅读`unique_tr` 和 `shared_ptr` 的文档中，其中提及了incomplete type 和 complete type，本文对此进行总结。

## 为什么smart pointer需要支持incomplete-type？
这是因为C++支持Opaque pointer，而C++ smart pointer是需要能够替代pointer的，因此它需要支持incomplete-type。

## herbsutter [GotW #100: Compilation Firewalls (Difficulty: 6/10)](https://herbsutter.com/gotw/_100/)

You still need to write the visible class’ destructor yourself and define it out of line in the implementation file, even if normally it’s the same as what the compiler would generate. This is because although both *unique_ptr* and *shared_ptr* can be instantiated with an **incomplete type**, *unique_ptr*’s destructor requires a **complete type** in order to invoke *delete* (unlike *shared_ptr* which captures more information when it’s constructed). By writing it yourself in the implementation file, you force it to be defined in a place where *impl* is already defined, and this successfully prevents the compiler from trying to automatically generate the destructor on demand in the caller’s code where *impl* is not defined.

> NOTE: 
>
> 看了上面这段话，我想到了: 
>
> 1、why "*unique_ptr* and *shared_ptr* can be instantiated with an **incomplete type**"？

## stackoverflow [Is std::unique_ptr required to know the full definition of T?](https://stackoverflow.com/questions/6012157/is-stdunique-ptrt-required-to-know-the-full-definition-of-t)

### [A](https://stackoverflow.com/a/6089065)

Adopted from [here](http://howardhinnant.github.io/incomplete.html).

Most templates in the C++ standard library require that they be instantiated with complete types. However `shared_ptr` and `unique_ptr` are *partial* exceptions. Some, but not all of their members can be instantiated with incomplete types. The motivation for this is to support idioms such as [pimpl](http://en.wikipedia.org/wiki/Opaque_pointer#C.2B.2B) using smart pointers, and without risking undefined behavior.

Undefined behavior can occur when you have an incomplete type and you call `delete` on it:

```
class A;
A* a = ...;
delete a;
```

The above is legal code. It will compile. Your compiler may or may not emit a warning for above code like the above. When it executes, bad things will probably happen. If you're very lucky your program will crash. However a more probable outcome is that your program will silently leak memory as `~A()` won't be called.

Using `auto_ptr<A>` in the above example doesn't help. You still get the same undefined behavior as if you had used a raw pointer.

Nevertheless, using incomplete classes in certain places is very useful! This is where `shared_ptr` and `unique_ptr` help. Use of one of these smart pointers will let you get away with an incomplete type, except where it is necessary to have a complete type. And most importantly, when it is necessary to have a complete type, you get a compile-time error if you try to use the smart pointer with an incomplete type at that point.

**No more undefined behavior:**

If your code compiles, then you've used a complete type everywhere you need to.

```
class A
{
    class impl;
    std::unique_ptr<impl> ptr_;  // ok!

public:
    A();
    ~A();
    // ...
};
```

`shared_ptr` and `unique_ptr` require a complete type in different places. The reasons are obscure, having to do with a dynamic deleter vs a static deleter. The precise reasons aren't important. In fact, in most code it isn't really important for you to know exactly where a complete type is required. Just code, and if you get it wrong, the compiler will tell you.

> NOTE: 
> 1、上面这段话中的"dynamic deleter vs a static deleter"是什么含义？
>
> 这段话让我想起了

However, in case it is helpful to you, here is a table which documents several members of `shared_ptr` and `unique_ptr` with respect to completeness requirements. If the member requires a complete type, then entry has a "C", otherwise the table entry is filled with "I".

```
Complete type requirements for unique_ptr and shared_ptr

                            unique_ptr       shared_ptr
+------------------------+---------------+---------------+
|          P()           |      I        |      I        |
|  default constructor   |               |               |
+------------------------+---------------+---------------+
|      P(const P&)       |     N/A       |      I        |
|    copy constructor    |               |               |
+------------------------+---------------+---------------+
|         P(P&&)         |      I        |      I        |
|    move constructor    |               |               |
+------------------------+---------------+---------------+
|         ~P()           |      C        |      I        |
|       destructor       |               |               |
+------------------------+---------------+---------------+
|         P(A*)          |      I        |      C        |
+------------------------+---------------+---------------+
|  operator=(const P&)   |     N/A       |      I        |
|    copy assignment     |               |               |
+------------------------+---------------+---------------+
|    operator=(P&&)      |      C        |      I        |
|    move assignment     |               |               |
+------------------------+---------------+---------------+
|        reset()         |      C        |      I        |
+------------------------+---------------+---------------+
|       reset(A*)        |      C        |      C        |
+------------------------+---------------+---------------+
```

Any operations requiring pointer conversions require complete types for both `unique_ptr` and `shared_ptr`.

The `unique_ptr<A>{A*}` constructor can get away with an incomplete `A` only if the compiler is not required to set up a call to `~unique_ptr<A>()`. For example if you put the `unique_ptr` on the heap, you can get away with an incomplete `A`. More details on this point can be found in [BarryTheHatchet's](https://stackoverflow.com/users/560648/barrythehatchet) answer [here](https://stackoverflow.com/a/27624369/576911).



## stackoverflow [std::unique_ptr with an incomplete type won't compile](https://stackoverflow.com/questions/9954518/stdunique-ptr-with-an-incomplete-type-wont-compile)

I'm using the pimpl-idiom with `std::unique_ptr`:

```cpp
class window {
  window(const rectangle& rect);

private:
  class window_impl; // defined elsewhere
  std::unique_ptr<window_impl> impl_; // won't compile
};
```

However, I get a compile error regarding the use of an incomplete type, on line 304 in `<memory>`:

> Invalid application of '`sizeof`' to an incomplete type '`uixx::window::window_impl`'

For as far as I know, `std::unique_ptr` should be able to be used with an incomplete type. Is this a bug in libc++ or am I doing something wrong here?



> NOTE: 下面是我的测试程序，它能够复现上述问题:
>
> `test.h`
>
> ```C++
> #include <memory>
> class foo
> {
> 	class impl;
> 	std::unique_ptr<impl> impl_;
> 
> public:
> 	foo(); // You may need a def. constructor to be defined elsewhere
> 
> 	// ~foo(); // Implement (with {}, or with = default;) where impl is complete
> };
> ```
>
> `test.cpp`
>
> ```C++
> #include "test.h"
> class foo::impl
> {
> };
> foo::foo() :
> 				impl_(new foo::impl)
> {
> 
> }
> 
> ```
>
> `main.cpp`
>
> ```C++
> #include "test.h"
> // user (main.cpp)
> int main()
> {
>     {
>         foo f;
>     }
> }
> // g++ test.cpp main.cpp --std=c++11
> ```
>
> 
>
> 上述程序编译报错如下:
>
> ```C++
> In file included from /usr/include/c++/4.8.2/memory:81:0,
>                  from test.h:1,
>                  from main.cpp:1:
> /usr/include/c++/4.8.2/bits/unique_ptr.h: In instantiation of ‘void std::default_delete<_Tp>::operator()(_Tp*) const [with _Tp = foo::impl]’:
> /usr/include/c++/4.8.2/bits/unique_ptr.h:184:16:   required from ‘std::unique_ptr<_Tp, _Dp>::~unique_ptr() [with _Tp = foo::impl; _Dp = std::default_delete<foo::impl>]’
> test.h:2:7:   required from here
> /usr/include/c++/4.8.2/bits/unique_ptr.h:65:22: 错误：invalid application of ‘sizeof’ to incomplete type ‘foo::impl’
>   static_assert(sizeof(_Tp)>0,
> ```
>
> 上述报错信息提示了我们如何判断是否是incomplete type:
>
> 1、`static_assert(sizeof(_Tp)>0)`
>
> 按照下面的各种修改方案，下面是可以编译通过的版本:
>
> `test.h`
>
> ```C++
> #include <memory>
> class foo
> {
> 	class impl;
> 	std::unique_ptr<impl> impl_;
> 
> public:
> 	foo(); // You may need a def. constructor to be defined elsewhere
> 
> 	~foo(); // Implement (with {}, or with = default;) where impl is complete
> };
> ```
>
> `test.cpp`
>
> ```C++
> #include "test.h"
> class foo::impl
> {
> };
> foo::foo() :
> 				impl_(new foo::impl)
> {
> 
> }
> foo::~foo()=default;
> ```
>
> `main.cpp`
>
> ```C++
> #include "test.h"
> // user (main.cpp)
> int main()
> {
>     {
>         foo f;
>     }
> }
> // g++ test.cpp main.cpp --std=c++11
> ```
>
> 上述程序是可以编译通过的。
>
> 

[A](https://stackoverflow.com/a/9954553)

Here are some examples of `std::unique_ptr` with incomplete types. The problem lies in destruction.

If you use pimpl with `unique_ptr`, you need to declare a destructor:

```cpp
class foo
{ 
    class impl;
    std::unique_ptr<impl> impl_;

public:
    foo(); // You may need a def. constructor to be defined elsewhere

    ~foo(); // Implement (with {}, or with = default;) where impl is complete
};
```

because otherwise the compiler generates a default one, and it needs a complete declaration of `foo::impl` for this.

If you have **template constructors**, then you're screwed(厄运临头), even if you don't construct the `impl_` member:

> NOTE: 
>
> 1、没有读懂

```cpp
template <typename T>
foo::foo(T bar) 
{
    // Here the compiler needs to know how to
    // destroy impl_ in case an exception is
    // thrown !
}
```

At namespace scope, using `unique_ptr` will not work either:

```cpp
class impl;
std::unique_ptr<impl> impl_;
```

> NOTE: `impl_` 是 "At namespace scope"，它的storage duration是"static duration"。

since the compiler must know here how to destroy this static duration object. A workaround is:

```cpp
class impl;
struct ptr_impl : std::unique_ptr<impl>
{
    ~ptr_impl(); // Implement (empty body) elsewhere
} impl_;
```



## TODO

1、stackoverflow [Unique_ptr and forward declaration](https://stackoverflow.com/questions/27336779/unique-ptr-and-forward-declaration)


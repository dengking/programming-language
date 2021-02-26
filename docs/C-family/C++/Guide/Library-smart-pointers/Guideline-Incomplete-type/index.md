# Incomplete types and `shared_ptr` / `unique_ptr`

之前在阅读`unique_tr` 和 `shared_ptr` 的文档中，其中提及了incomplete type 和 complete type，这个点，我目前还是不怎么了解的，下面是关于此的一些素材。



## [GotW #100: Compilation Firewalls (Difficulty: 6/10)](https://herbsutter.com/gotw/_100/)

You still need to write the visible class’ destructor yourself and define it out of line in the implementation file, even if normally it’s the same as what the compiler would generate. This is because although both *unique_ptr* and *shared_ptr* can be instantiated with an **incomplete type**, *unique_ptr*’s destructor requires a **complete type** in order to invoke *delete* (unlike *shared_ptr* which captures more information when it’s constructed). By writing it yourself in the implementation file, you force it to be defined in a place where *impl* is already defined, and this successfully prevents the compiler from trying to automatically generate the destructor on demand in the caller’s code where *impl* is not defined.

> NOTE: 
>
> 看了上面这段话，我想到了: 
>
> 1、why "*unique_ptr* and *shared_ptr* can be instantiated with an **incomplete type**"？



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

If you have template constructors, then you're screwed(厄运临头), even if you don't construct the `impl_` member:

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



## Incomplete type and type erasure


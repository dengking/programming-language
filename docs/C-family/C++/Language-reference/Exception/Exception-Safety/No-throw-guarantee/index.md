# No throw guarantee

## Why need non throw function？

首先需要搞清楚为什么需要non throw function。

### stackoverflow [confusion about non-throw functions](https://stackoverflow.com/questions/11841963/confusion-about-non-throw-functions)

I have 2 questions about non-throwing functions:

1、Why make a function non-throwing?

2、How to make a function non-throwing? If the code inside the function actually may `throw`, then should I still make it non-throwing?

Here is an example:

```
void swap(Type t1, Type t2) throw()
{
    //swap
}
```

If the code in `swap` won't throw at all, should I still append `throw()`? Why?

[A](https://stackoverflow.com/a/11842328)

`throw()` (or `noexcept` in C++11) is useful for two reasons:

1、It allows the compiler to be more aggressive in its optimisations.

> NOTE: 需要遵循optimization principle

2、It tells function users that they can use this function in their own non-throwing functions.

Non-throwing functions are very important for writing exception safe code. For instance, the usual way of writing an exception safe `operator=` is via a non-throwing `swap()` function.

> NOTE: 第二个目标是: 实现strong exception safe

On the other hand, other exception specifications are useless and have been justly(应该的、应得的) deprecated in the current standard. They don't mix well at all with templates and are too expensive to enforce.

> NOTE:
>
> 1、这一段没有读懂

Now, if you use a `noexcept` specification in a function that might actually throw, all bets are off. Even if your compiler does not terminate the program when an exception leaves the function (VS does not do it for runtime efficiency reasons, for instance), your code might not do what you thought because of optimisations. For example:

```
void f() noexcept
{
  a();
  b();
}
```

If `a()` actually throws and `b()` has side effects, the function behaviour will be unpredictable, because your compiler may decide to execute `b()` before `a()`, since you have told it that no exceptions will be thrown.

> NOTE:
>
> 1、memory reordering
>
> 2、前面的这段话告诉我们:
>
> a、一定要谨慎地使用 `noexcept`
>
> b、一定要保证程序的一致: 程序的实际行为应该和你的specifier保持一致

EDIT: Now for the second part of your question: how to make a function non-throwing?

First you need to ask yourself whether your function should really be non-throwing. For instance:

```
class C
{
  C* CreateInstance()
  {
    return new C();
  }
}
```

Since operator `new` can throw a `std::bad_alloc`, `CreateInstance()` can throw. You could try to avoid it with a try-catch block, handling or swallowing any exceptions that may be thrown inside the `try` block, but would that really be sensible? For instance:

```
C* CreateInstance()
{
  try
  {
     return new C();
  }
  catch (...)
  {
     return null;
  }
}
```

Problem solved, it seems, but would your callers be prepared for `CreateInstance()` returning `null`? If not, a crash will ensue(跟着发生) when they try to use that pointer. Besides, a `std::bad_alloc` usually means that you have run out of memory and you would just be postponing the problem.

So be careful: some functions can be made non-throwing, but others should be allowed to throw. Exception safety is not a trivial matter.

> NOTE: "some functions can be made non-throwing" 在后面对这些functions进行了总结

### CppCoreGuidelines [F.6: If your function must not throw, declare it `noexcept`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#f6-if-your-function-must-not-throw-declare-it-noexcept)



### Implementation of strong guarantee may depend on stronger guarantee

参见 `Exception-Safety # Implementation of strong guarantee may depend on stronger guarantee` 段: 

> 它是可靠的、它是最最强的guarantee、可以依靠它来实现strong exception safety

## How to implement no throw guarantee?

> NOTE: tag-how-to-implement-no-throw-guarantee

我们需要考虑:

1、如何实现"no throw guarantee"？

2、哪些情况下能够满足"no throw guarantee"？

在下面文章中涉及到了这个内容:

1、stackoverflow [How can an implementation guarantee that copy constructor of an iterator is no throw?](https://stackoverflow.com/questions/13428851/how-can-an-implementation-guarantee-that-copy-constructor-of-an-iterator-is-no-t) # [A](https://stackoverflow.com/a/13430036)

> none of them have to use any dynamically allocated memory

一般，如果不dynamically allocated memory，基本上能够保证no throw guarantee。

2、一般，释放resource是能够保证no throw guarantee的。





## 哪些需要no throw guarantee/`noexcept`?

CppCoreGuidelines [C.37: Make destructors `noexcept`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-dtor-noexcept)

CppCoreGuidelines [C.44: Prefer default constructors to be simple and non-throwing](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c44-prefer-default-constructors-to-be-simple-and-non-throwing)

CppCoreGuidelines [C.66: Make move operations `noexcept`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-move-noexcept)

CppCoreGuidelines [C.83: For value-like types, consider providing a `noexcept` swap function](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-swap)

CppCoreGuidelines [C.85: Make `swap` `noexcept`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-swap-noexcept)

CppCoreGuidelines [C.86: Make `==` symmetric with respect of operand types and `noexcept`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-eq)

CppCoreGuidelines [C.89: Make a `hash` `noexcept`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-hash)



CppCoreGuidelines [E.16: Destructors, deallocation, and `swap` must never fail](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#e16-destructors-deallocation-and-swap-must-never-fail)

Deallocation functions, including `operator delete`, must be `noexcept`. `swap` functions must be `noexcept`. Most destructors are implicitly `noexcept` by default. Also, [make move operations `noexcept`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-move-noexcept).
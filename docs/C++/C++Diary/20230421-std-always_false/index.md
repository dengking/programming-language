# `std::always_false`

如果使用 `static_assert(false, "error message!")`，那么肯定会导致编译失败，从实践来看，即使这个template没有被使用，编译器仍然会触发这个`static_assert(false)` 进而导致编译失败，在 stackoverflow [Forbids functions with `static_assert`](https://stackoverflow.com/questions/34745581/forbids-functions-with-static-assert) 中对此进行了讨论，下面是例子:

```c++
#include <string>
#include <cstddef>
#include <concepts>

template<class T>
struct UsingThisShouldBeAnError {
    static_assert(false, "You should not use this!");
};

int main() {
}
```



```c++
/Users/kai/test/cpp/test.cc:7:5: error: static_assert failed "You should not use this!"
    static_assert(false, "You should not use this!");
```



## stackoverflow [Forbids functions with `static_assert`](https://stackoverflow.com/questions/34745581/forbids-functions-with-static-assert)

I want to prevent certain functions from being called. Let's ignore the case of calling the function via a function pointer or something, and just concentrate on the case of direct function call. I can do this with `= delete`. However, the diagnostic issued is not quite informative. I considered using `static_assert`, with which you can supply a custom **diagnostic message**. I placed a `static_assert(false, ...)` statement within the function body, hoping that it fires when the function is called. However, it turns out that the `static_assert` fails even if the function is not called. Any suggestions?

**Additional Note:** The function is forbidden unconditionally. So, `std::enable_if` does not apply here. The motivation for such a function is that I want to prevent certain use, which would otherwise compile fine with **overload resolution**. So I can't just remove the function. `deprecated` is not what I want. I want a compilation error, not a warning.

> NOTE:
>
> 一、作者的意图是想要提示信息完善的编译报错



### [A](https://stackoverflow.com/a/34746077)

I agree with others that you shouldn't use `static_assert` for this at all and mark the function as deprecated instead.

`static_assert`ions fire at the time they are compiled. For an ordinary function, this is the time it is parsed, not the time it is called. For a `template`, however, it is the time of instantiation. So you can make your function a `template` like this.

> NOTE:
>
> 一、作者的上面这段话其实不怎么准确，准确的是: 
>
> 编译器是会parse template的，如果它碰到 `static_assert(false)` 它是会报不易错误的，但是如果 `static_assert(condition)` condition是一个template，那么在parse阶段它是不会去evaluate这个assertion，而是在instantiation阶段，所以如果这个template没有被使用，那么这个assertion也就不会被evaluate。

```cpp
template <typename...>
struct always_false { static constexpr bool value = false; };

template <typename... Ts>
void
never_call_me(Ts&&...)
{
  static_assert(always_false<Ts...>::value,
                "You should have never called this function!");
}
```

If `typename...` is not right for you (because the function is overloaded), try narrowing it down to only match what you want to make an error.

The trick used here is that `always_false<Ts...>::value` depends on the type parameters `Ts...` so it cannot be evaluated until the `template` is instantiated. (Even though we can clearly see that it will always be `false`.)



### stackoverflow [Is it possible to implement always_false in the C++ standard library?](https://stackoverflow.com/questions/57787666/is-it-possible-to-implement-always-false-in-the-c-standard-library) 

There are cases where one uses an `always_false` helper to e.g. cause unconditional `static_assert` failure if instantiation of some template is attempted:

```c++
#include <string>
#include <cstddef>
#include <concepts>


template<class... T>
struct always_false : std::false_type {
};

template<class T>
struct UsingThisShouldBeAnError {
    static_assert(always_false<T>::value, "You should not use this!");
};

int main() {
    UsingThisShouldBeAnError<int>();
}
```

This helper is necessary because a template definition must (at least theoretically) have at least one set of template parameters for which a valid specialization can be produced in order for the program to be well-formed:

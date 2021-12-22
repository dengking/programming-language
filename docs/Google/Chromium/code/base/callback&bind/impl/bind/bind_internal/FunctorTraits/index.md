# [`FunctorTraits`](https://github.com/chromium/chromium/blob/main/base/bind_internal.h)

## doc

`FunctorTraits<>` -- Type traits used to determine the correct `RunType` and invocation manner for a `Functor`.  This is where function signature adapters are applied.

> NOTE: 
>
> "invocation manner for a `Functor`" 对应的是 `Invoke` 函数，后面会进行详细的介绍。

trait是meta-function、定义了static interface、通过specialization、SFINAE来进行实现static polymorphism，`FunctorTrait` 用来区分所有不同的 `Functor` concept 类别，`FunctorTraits`是典型的trait玩法，`FunctorTraits`提供了如下consistent API:

1、static method: `Invoke`

2、static variable:

`is_method`: 是否是member method

`is_nullable`: 是否可以为空，pointer是可以为空的

`is_callback`: 是否是chromium的`Callback` 类型


```C++
template <typename Functor, typename SFINAE = void>
struct FunctorTraits;
```

### static method: `Invoke`

`Invoke` 函数 描述了 "invocation manner for a `Functor`"，更加准确地说: **原始函数**被调用的地方是 `FunctorTraits::Invoke` 函数；无论如何bind、如何partial application，chromium的bind-callback需要保证: 原始的函数被调用的时候的入参是和它的声明是一致的，最终这个函数被调用的时候，它的入参有如下两个部分组成:

1、bound args

2、unbound args



## Functor类别(需要处理的情况)

1、captureless lambda 

2、function pointer 

3、method、const method 

4、`IgnoreResult`s 

5、`OnceCallback`s

6、`RepeatingCallback`s





### For empty callable types

注释中说lambda是`IsCallableObject`，但是在`IsCallableObject`中给出的一个例子中，lambda是不可以的。

所谓empty，是指它不带参数。

### For functions

```C++
template <typename R, typename... Args>
struct FunctorTraits<R (*)(Args...)>
```
`FunctorTraits` 只有两个模板参数，但是这里的第二个竟然是 parameter pack，并且它没有模板参数`SFINAE`

#### `OS_WIN` For functions

### For methods.

```C++
template <typename R, typename Receiver, typename... Args>
struct FunctorTraits<R (Receiver::*)(Args...)>
```
它的目标参数更多，显然和primary template是不同的，但是compiler会认为它是一个specialization。

### For const methods.

```C++
template <typename R, typename Receiver, typename... Args>
struct FunctorTraits<R (Receiver::*)(Args...) const>
```

#### `__cpp_noexcept_function_type`

```C++
#ifdef __cpp_noexcept_function_type
// noexcept makes a distinct function type in C++17.
// I.e. `void(*)()` and `void(*)() noexcept` are same in pre-C++17, and
// different in C++17.
template <typename R, typename... Args>
struct FunctorTraits<R (*)(Args...) noexcept> : FunctorTraits<R (*)(Args...)> {
};

template <typename R, typename Receiver, typename... Args>
struct FunctorTraits<R (Receiver::*)(Args...) noexcept>
    : FunctorTraits<R (Receiver::*)(Args...)> {};

template <typename R, typename Receiver, typename... Args>
struct FunctorTraits<R (Receiver::*)(Args...) const noexcept>
    : FunctorTraits<R (Receiver::*)(Args...) const> {};
#endif
```

### For `IgnoreResults`

```C++
// For IgnoreResults.
template <typename T>
struct FunctorTraits<IgnoreResultHelper<T>> : FunctorTraits<T> {
  using RunType =
      typename ForceVoidReturn<typename FunctorTraits<T>::RunType>::RunType;

  template <typename IgnoreResultType, typename... RunArgs>
  static void Invoke(IgnoreResultType&& ignore_result_helper,
                     RunArgs&&... args) {
    FunctorTraits<T>::Invoke(
        std::forward<IgnoreResultType>(ignore_result_helper).functor_,
        std::forward<RunArgs>(args)...);
  }
};
```




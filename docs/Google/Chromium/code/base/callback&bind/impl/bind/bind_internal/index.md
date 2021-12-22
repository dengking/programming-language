# [chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/main/base)/**[bind_internal.h](https://github.com/chromium/chromium/blob/main/base/bind_internal.h)**



## doc

这部分内容源自源代码中的注释。

### concepts

从注释可知：Functor concept包括`Callback<>`。

`Runtype` 
### types

`ForceVoidReturn` helper class，将函数signature转换为返回值为void



`FunctorTraits` type trait，

Type traits used to determine the correct `RunType` and      invocation manner for a Functor.  This is where function signature adapters are applied.


`StorageTraits<>` -- Type traits that determine how a bound argument is stored in `BindState`.


`InvokeHelper<>` -- Take a Functor + arguments and actually invokes it. Handle the differing syntaxes needed for `WeakPtr<>` support.  This is separate from `Invoker` to avoid creating multiple version of `Invoker<>`.


`Invoker<>` -- Unwraps the curried parameters and executes the `Functor`.

`BindState<>` -- Stores the curried parameters, and is the main entry point into the `Bind()` system.



## `struct BindState final : BindStateBase` 

主要描述`BindStateBase`的implementation `BindState` 。

```C++
template <typename Functor, typename... BoundArgs>
struct BindState final : BindStateBase 
```

### 成员变量

```C++
  Functor functor_;
  std::tuple<BoundArgs...> bound_args_;
```

可以看到，它使用 `std::tuple` 来存储bound args。

## `BindImpl`

## `BindTypeHelper`

一、显然，它是unwrap function signature

二、顾名思义: 它提供了各种 type





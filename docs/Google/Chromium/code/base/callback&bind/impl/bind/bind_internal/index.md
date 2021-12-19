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





## `Invoker`

```
Invoker<> -- Unwraps the curried parameters and executes the Functor.
```

```C++
// Invoker<>
//
// See description at the top of the file.
template <typename StorageType, typename UnboundRunType>
struct Invoker;

template <typename StorageType, typename R, typename... UnboundArgs>
struct Invoker<StorageType, R(UnboundArgs...)> {
  static R RunOnce(BindStateBase* base,
                   PassingType<UnboundArgs>... unbound_args) {
    // Local references to make debugger stepping easier. If in a debugger,
    // you really want to warp ahead and step through the
    // InvokeHelper<>::MakeItSo() call below.
    StorageType* storage = static_cast<StorageType*>(base);
    static constexpr size_t num_bound_args =
        std::tuple_size<decltype(storage->bound_args_)>::value;
    return RunImpl(std::move(storage->functor_),
                   std::move(storage->bound_args_),
                   std::make_index_sequence<num_bound_args>(),
                   std::forward<UnboundArgs>(unbound_args)...);
  }

  static R Run(BindStateBase* base, PassingType<UnboundArgs>... unbound_args) {
    // Local references to make debugger stepping easier. If in a debugger,
    // you really want to warp ahead and step through the
    // InvokeHelper<>::MakeItSo() call below.
    const StorageType* storage = static_cast<StorageType*>(base);
    static constexpr size_t num_bound_args =
        std::tuple_size<decltype(storage->bound_args_)>::value;
    return RunImpl(storage->functor_, storage->bound_args_,
                   std::make_index_sequence<num_bound_args>(),
                   std::forward<UnboundArgs>(unbound_args)...);
  }

 private:
  template <typename Functor, typename BoundArgsTuple, size_t... indices>
  static inline R RunImpl(Functor&& functor,
                          BoundArgsTuple&& bound,
                          std::index_sequence<indices...>,
                          UnboundArgs&&... unbound_args) {
    static constexpr bool is_method = MakeFunctorTraits<Functor>::is_method;

    using DecayedArgsTuple = std::decay_t<BoundArgsTuple>;
    static constexpr bool is_weak_call =
        IsWeakMethod<is_method,
                     std::tuple_element_t<indices, DecayedArgsTuple>...>();

    return InvokeHelper<is_weak_call, R>::MakeItSo(
        std::forward<Functor>(functor),
        Unwrap(std::get<indices>(std::forward<BoundArgsTuple>(bound)))...,
        std::forward<UnboundArgs>(unbound_args)...);
  }
};
```



### `InvokeHelper`

```
//  InvokeHelper<> -- Take a Functor + arguments and actually invokes it.
//                    Handle the differing syntaxes needed for WeakPtr<>
//                    support.  This is separate from Invoker to avoid creating
//                    multiple version of Invoker<>.
```




```c++
// InvokeHelper<>
//
// There are 2 logical InvokeHelper<> specializations: normal, WeakCalls.
//
// The normal type just calls the underlying runnable.
//
// WeakCalls need special syntax that is applied to the first argument to check
// if they should no-op themselves.
template <bool is_weak_call, typename ReturnType>
struct InvokeHelper;
```


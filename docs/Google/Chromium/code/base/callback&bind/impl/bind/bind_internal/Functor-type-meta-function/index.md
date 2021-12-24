# `Functor` type meta-function

为了描述便利，我使用函数的语言来进行描述。

采用自顶向下的方式来进行分析: 

`BindTypeHelper` 是总入口，在 `BindTypeHelper` 中，会调用 `FunctorTraits` ，`FunctorTraits` 是核心的 meta-function，它通过template specialization的方式能够拆解chromium中的所有的`Functor` concept。

## [`BindTypeHelper`](https://github.com/chromium/chromium/blob/master/base/bind_internal.h#L791) 

这是最最顶层的 `Functor` type meta-function，它是下面的很多`Functor` type meta-function的调用者。



### doc 1

> Extracts necessary type info from `Functor` and `BoundArgs`. Used to implement `MakeUnboundRunType`, `BindOnce` and `BindRepeating`.



### doc 2

> Example:
>
> When `Functor` is `double (Foo::*)(int, const std::string&)`, and `BoundArgs` is a template pack of `Foo*` and `int16_t`:
>
> - `RunType` is `double(Foo*, int, const std::string&)`,
> - `ReturnType` is `double`,
> - `RunParamsList` is `TypeList<Foo*, int, const std::string&>`,
> - `BoundParamsList` is `TypeList<Foo*, int>`,
> - `UnboundParamsList` is `TypeList<const std::string&>`,
> - `BoundArgsList` is `TypeList<Foo*, int16_t>`,
> - `UnboundRunType` is `double(const std::string&)`.



## [`FunctorTraits`](https://github.com/chromium/chromium/blob/main/base/bind_internal.h)

它通过template specialization的方式，识别chromium中的所有的`Functor` concept。



### Factory meta-function: `MakeFunctorTraits` 

一、`MakeFunctorTraits` 是 `FunctorTraits` 的入口，需要提供它来调用 `FunctorTraits`

二、实现如下: 

```C++
template <typename Functor>
using MakeFunctorTraits = FunctorTraits<std::decay_t<Functor>>;
```

可以看到，`MakeFunctorTraits` 只有一个模板参数 `Functor`，这说明: 

`FunctorTraits`通过template specialization的方式能够拆解chromium中的所有的`Functor` concept，从而获得:

1、args type list

2、return type



### doc

`FunctorTraits<>` -- Type traits used to determine the correct `RunType` and invocation manner for a `Functor`.  This is where function signature adapters are applied.

> NOTE: 
>
> "invocation manner for a `Functor`" 对应的是 `Invoke` 函数，后面会进行详细的介绍。

trait是meta-function、定义了static interface、通过specialization、SFINAE来进行实现static polymorphism，`FunctorTrait` 用来区分所有不同的 `Functor` concept 类别，`FunctorTraits`是典型的trait玩法，提供了consistent API to trait。

### traits: `Functor`的类别

`is_method`: 是否是member method

`is_nullable`: 是否可以为空，pointer是可以为空的

`is_callback`: 是否是chromium的`Callback` 类型


```C++
template <typename Functor, typename SFINAE = void>
struct FunctorTraits;
```



### trait: `RunType` 

后面会进行详细介绍。



### trait: static method `Invoke`

`Invoke` 函数 描述了 "invocation manner for a `Functor`"，更加准确地说: **原始函数**被调用的地方是 `FunctorTraits::Invoke` 函数；无论如何bind、如何partial application，chromium的bind-callback需要保证: 原始的函数被调用的时候的入参是和它的声明是一致的，最终这个函数被调用的时候，它的入参有如下两个部分组成:

1、bound args

2、unbound args



### 模板参数 `Args`

所有的参数，包括: 

一、bound args

二、unbound args



## `FunctorTraits` trait: `RunType` concept

### doc

> CONCEPTS:
>
> `RunType` -- A function type (as opposed to `function_pointer_type`) for a `Callback<>::Run()`.  Usually just a convenience typedef.

一、它是一个concept；

二、需要注意的是: `RunType` 是由 `FunctorTraits` 生成的。在 `BindTypeHelper` 会使用 。

三、需要注意的是: `RunType` 不是 pointer to function，它是function type (它描述函数的返回值、参数类型)；这样做的目的是:

1、为了static  type safety吗？

2、后面可以根据 `RunType` 而获得 `Functor` 的 return type ( 由meta-function `ExtractReturnType` 实现  )、完整的args type list ( 由meta-function `ExtractArgs` ) 实现

四、需要注意的是: `RunType` 描述的是原始函数的运行类型，因此它的`Args`包含**bound args**和**unbound args**。这一点可以通过如下代码验证:

1、

```C++
// For functions.
template <typename R, typename... Args>
struct FunctorTraits<R (*)(Args...)> {
  using RunType = R(Args...);
};   
```

2、

```C++
// For methods.
template <typename R, typename Receiver, typename... Args>
struct FunctorTraits<R (Receiver::*)(Args...)> {
  using RunType = R(Receiver*, Args...);
};
```

### example

在 [`BindTypeHelper`](https://github.com/chromium/chromium/blob/master/base/bind_internal.h#L791) 中，给出了example。

### meta-function `ExtractArgs`

```C++
// Used for ExtractArgs and ExtractReturnType.
template <typename Signature>
struct ExtractArgsImpl;

template <typename R, typename... Args>
struct ExtractArgsImpl<R(Args...)> {
  using ReturnType = R;
  using ArgsList = TypeList<Args...>;
};

// A type-level function that extracts function arguments into a TypeList.
// E.g. ExtractArgs<R(A, B, C)> is evaluated to TypeList<A, B, C>.
template <typename Signature>
using ExtractArgs = typename ExtractArgsImpl<Signature>::ArgsList;
```



### meta-function `ExtractReturnType` 

```C++
// A type-level function that extracts the return type of a function.
// E.g. ExtractReturnType<R(A, B, C)> is evaluated to R.
template <typename Signature>
using ExtractReturnType = typename ExtractArgsImpl<Signature>::ReturnType;
```



### meta-function: `ExtractCallableRunType`

```C++
// For empty callable types.
// This specialization is intended to allow binding captureless lambdas, based
// on the fact that captureless lambdas are empty while capturing lambdas are
// not. This also allows any functors as far as it's an empty class.
// Example:
//
//   // Captureless lambdas are allowed.
//   []() {return 42;};
//
//   // Capturing lambdas are *not* allowed.
//   int x;
//   [x]() {return x;};
//
//   // Any empty class with operator() is allowed.
//   struct Foo {
//     void operator()() const {}
//     // No non-static member variable and no virtual functions.
//   };
template <typename Functor>
struct FunctorTraits<Functor,
                     std::enable_if_t<IsCallableObject<Functor>::value &&
                                      std::is_empty<Functor>::value>> {
  using RunType = ExtractCallableRunType<Functor>;
  static constexpr bool is_method = false;
  static constexpr bool is_nullable = false;
  static constexpr bool is_callback = false;

  template <typename RunFunctor, typename... RunArgs>
  static ExtractReturnType<RunType> Invoke(RunFunctor&& functor,
                                           RunArgs&&... args) {
    return std::forward<RunFunctor>(functor)(std::forward<RunArgs>(args)...);
  }
};
```



```C++
template <typename Callable,
          typename Signature = decltype(&Callable::operator())>
struct ExtractCallableRunTypeImpl;

template <typename Callable, typename R, typename... Args>
struct ExtractCallableRunTypeImpl<Callable, R (Callable::*)(Args...)> {
  using Type = R(Args...);
};

template <typename Callable, typename R, typename... Args>
struct ExtractCallableRunTypeImpl<Callable, R (Callable::*)(Args...) const> {
  using Type = R(Args...);
};

// Evaluated to RunType of the given callable type.
// Example:
//   auto f = [](int, char*) { return 0.1; };
//   ExtractCallableRunType<decltype(f)>
//   is evaluated to
//   double(int, char*);
template <typename Callable>
using ExtractCallableRunType =
    typename ExtractCallableRunTypeImpl<Callable>::Type;


```

### meta-function: `ForceVoidReturn`



```C++
// ForceVoidReturn<>
//
// Set of templates that support forcing the function return type to void.
template <typename Sig>
struct ForceVoidReturn;

template <typename R, typename... Args>
struct ForceVoidReturn<R(Args...)> {
  using RunType = void(Args...);
};
```



## `BindTypeHelper`: `ParamsList`

```C++
template <typename Functor, typename... BoundArgs>
struct BindTypeHelper {
  static constexpr size_t num_bounds = sizeof...(BoundArgs);
  using FunctorTraits = MakeFunctorTraits<Functor>;
  using RunType = typename FunctorTraits::RunType;
  using RunParamsList = ExtractArgs<RunType>;
  using BoundParamsList = TakeTypeListItem<num_bounds, RunParamsList>;
  using UnboundParamsList = DropTypeListItem<num_bounds, RunParamsList>;
  using BoundArgsList = TypeList<BoundArgs...>;
};
```

1、`RunParamsList` 

2、`BoundParamsList`

3、`UnboundParamsList`

4、`BoundArgsList`



### bound args VS unbound args

为了便于对比。





## `RunType` VS `UnboundRunType` 

为便于对比，将 `RunType` & `UnboundRunType` 放到一起。



## `BindTypeHelper`: `UnboundRunType` 

```c++
using UnboundRunType = MakeFunctionType<ReturnType, UnboundParamsList>;
```



## meta-function: `MakeFunctionType`

```C++
// Used for MakeFunctionType implementation.
template <typename R, typename ArgList>
struct MakeFunctionTypeImpl;

template <typename R, typename... Args>
struct MakeFunctionTypeImpl<R, TypeList<Args...>> {
  // MSVC 2013 doesn't support Type Alias of function types.
  // Revisit this after we update it to newer version.
  typedef R Type(Args...);
};

// A type-level function that constructs a function type that has |R| as its
// return type and has TypeLists items as its arguments.
template <typename R, typename ArgList>
using MakeFunctionType = typename MakeFunctionTypeImpl<R, ArgList>::Type;
```





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




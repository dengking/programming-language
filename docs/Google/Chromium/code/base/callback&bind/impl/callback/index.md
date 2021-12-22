# Callback

## [chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/main/base)/**[callback.h](https://github.com/chromium/chromium/blob/main/base/callback.h)** 

Callbacks also support cancellation. A common use is binding the receiver object as a `WeakPtr<T>`. If that weak pointer is invalidated, calling `Run()`
will be a no-op. Note that |`IsCancelled()`| and |`is_null()`| are distinct: simply cancelling a callback will not also make it null.

> NOTE: 
>
> 一、"A common use is binding the receiver object as a `WeakPtr<T>`" 这是非常重要的，是能够避免access outside of object lifetime，后面将它称为 weak receiver pattern，在 [chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/main/base)/**[bind_internal.h](https://github.com/chromium/chromium/blob/main/base/bind_internal.h)** 中有如下定义:
>
> ```C++
> template <typename T>
> struct IsWeakReceiver;
> ```
>
> 



## [chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/master/base)/**[callback_internal.h](https://github.com/chromium/chromium/blob/master/base/callback_internal.h)**

这个文件中，主要定义abstraction:

1、`BindStateBase`

2、`CallbackBase`

这两个类都不是template。

### `class BASE_EXPORT BindStateBase`



`BindStateBase` is used to provide an **opaque handle** that the `Callback` class can use to represent a function object with bound arguments.  It behaves as an existential type that is used by a corresponding `DoInvoke` function to perform the function execution.  This allows us to shield(屏蔽、保护) the `Callback` class from the types of the bound argument via "type erasure."

> NOTE: 
>
> 一、使用了type erasure技术
>
> 二、"This allows us to shield(屏蔽、保护) the `Callback` class from the types of the bound argument via "type erasure.""
>
> 这段话的意思是: 让  `Callback` class 不受  "types of the bound argument " 的影响
>
> 三、需要注意的是: 是由 `BindState` 来存储 function pointer 和 bound arguments。

At the base level, the only task is to add reference counting data. Avoid using or inheriting any virtual functions. Creating a vtable for every `BindState` template instantiation results in a lot of bloat. Its only task is to call the destructor which can be done with a function pointer.

> NOTE: 
>
> 一、上述base，指的是 base class；"At the base level, the only task is to add reference counting data"的含义是: 在 `BindStateBase` 中，唯一做的是 "add reference counting data"；这段话是需要结合如下点来进行理解:
>
> 1、`class BASE_EXPORT BindStateBase` 继承自 `RefCountedThreadSafe`，这就是  "add reference counting data"
>
> 2、 `class BASE_EXPORT CallbackBase` 来进行理解的: 它的唯一的成员变量是: `scoped_refptr<BindStateBase> bind_state_;`
>
> 二、"Avoid using or inheriting any virtual functions. Creating a vtable for every `BindState` template instantiation results in a lot of bloat" 这段话要如何理解呢？它的意思是在基类 `BindStateBase` 中，并没有声明任何的virtual class，因为一旦声明了virtual class，则C++ implementation中，就会"Creating a vtable for every `BindState` template instantiation results in a lot of bloat"。但是我们知道一般base class是需要virtual destructor的，那它是如何做到的呢？为了实现这一点，它使用了 function pointer 来替代 virtual destructor: "Its only task is to call the destructor which can be done with a function pointer"，结合代码来看:
>
> ```C++
> // Pointer to a function that will properly destroy |this|.
> void (*destructor_)(const BindStateBase*);
> ```
>
> 总的来说，这是一种avoid code bloat的奇技淫巧。

#### member variable

```C++
  using InvokeFuncStorage = void(*)();
  // In C++, it is safe to cast function pointers to function pointers of
  // another type. It is not okay to use void*. We create a InvokeFuncStorage
  // that that can store our function pointer, and then cast it back to
  // the original type on usage.
  InvokeFuncStorage polymorphic_invoke_;
```

注释的意思是: 在C++中，function pointer之间的cast是可以的，但是void pointer 和 function pointer之间的转换是不可行的。后面会对它进行专门的描述。

```C++
  // Pointer to a function that will properly destroy |this|.
  void (*destructor_)(const BindStateBase*);
```

有了上面的描述，这段话是容易理解的，它其实就是pointer to destructor。





#### implementation

`BindStateBase`的implementation是在 [chromium](https://gitee.com/mirrors/chromium/tree/master) / **[base](https://gitee.com/mirrors/chromium/tree/master/base)** / **[bind_internal.h](https://gitee.com/mirrors/chromium/blob/master/base/bind_internal.h)** 中定义的。

#### constructor

```C++
  BindStateBase(InvokeFuncStorage polymorphic_invoke,
                void (*destructor)(const BindStateBase*));
  BindStateBase(InvokeFuncStorage polymorphic_invoke,
                void (*destructor)(const BindStateBase*),
                bool (*query_cancellation_traits)(const BindStateBase*,
                                                  CancellationQueryMode mode));
```

可以看到，constructor主要是对

### `class BASE_EXPORT CallbackBase`

#### 成员变量

```C++
scoped_refptr<BindStateBase> bind_state_;
```

可以看到，它的成员变量的类型是 `BindStateBase` ，这是典型的design-to-an-abstraction。



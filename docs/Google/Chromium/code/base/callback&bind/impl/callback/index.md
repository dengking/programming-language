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



### `class BASE_EXPORT CallbackBase`

#### 成员变量

```C++
scoped_refptr<BindStateBase> bind_state_;
```

可以看到，它的成员变量的类型是 `BindStateBase` ，这是典型的design-to-an-abstraction。

### `class BASE_EXPORT BindStateBase`

参见 `BindState` 章节。


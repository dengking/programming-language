# Implementation

## 参见: 

1、docs [OnceCallback<> and BindOnce(), RepeatingCallback<> and BindRepeating()](https://github.com/chromium/chromium/blob/master/docs/callback.md) # [Implementation notes](https://gitee.com/mirrors/chromium/blob/master/docs/callback.md#implementation-notes)



## 我的总结

1、遵循design to abstraction

2、回答这些问题:

### 一、如何存储bind state？

abstract class: `BindStateBase` ( [chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/master/base)/**[callback_internal.h](https://github.com/chromium/chromium/blob/master/base/callback_internal.h)** )

implementation class: `BindState` ( [chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/main/base)/**[bind_internal.h](https://github.com/chromium/chromium/blob/main/base/bind_internal.h)** )，这是class template 

显然，它采用的是: OOP interface + template implementation idiom

### 二、如何表示callback？

abstract class: `CallbackBase` ( [chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/master/base)/**[callback_internal.h](https://github.com/chromium/chromium/blob/master/base/callback_internal.h)** )

implementation class: ，

1、`OnceCallback` ( [chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/main/base)/**[callback.h](https://github.com/chromium/chromium/blob/main/base/callback.h)** )，这是class template 

2、`RepeatingCallback` ( [chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/main/base)/**[callback.h](https://github.com/chromium/chromium/blob/main/base/callback.h)** )，这是class template 

### 三、如何关联 callback 和 bind state？

1、成员变量: 

abstract class: `CallbackBase` 有成员变量 `bind_state_`，它的类型是 `scoped_refptr<BindStateBase> bind_state_`，显然它使用的是 abstract class: `BindStateBase` 。

2、factory method

## [chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/main/base)/**[callback_forward.h](https://github.com/chromium/chromium/blob/main/base/callback_forward.h)**

主要声明最最顶级的、最被外部使用的class:

1、`OnceCallback`、`OnceClosure` 

2、`RepeatingCallback`、`RepeatingClosure` 



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
> 二、上面这段话中提及了avoid code bloat
>
> 



`BindStateBase`的implementation是在 [chromium](https://gitee.com/mirrors/chromium/tree/master) / **[base](https://gitee.com/mirrors/chromium/tree/master/base)** / **[bind_internal.h](https://gitee.com/mirrors/chromium/blob/master/base/bind_internal.h)** 中定义的。



### `class BASE_EXPORT CallbackBase`

#### 成员变量

```C++
scoped_refptr<BindStateBase> bind_state_;
```

可以看到，它的成员变量的类型是 `BindStateBase` ，这是典型的design-to-an-abstraction。



## TODO

CSDN Luoshengyang [Chromium多线程通信的Closure机制分析](https://blog.csdn.net/Luoshengyang/article/details/46747797)


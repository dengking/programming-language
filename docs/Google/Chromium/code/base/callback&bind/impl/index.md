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



## TODO

CSDN Luoshengyang [Chromium多线程通信的Closure机制分析](https://blog.csdn.net/Luoshengyang/article/details/46747797)




# docs [OnceCallback<> and BindOnce(), RepeatingCallback<> and BindRepeating()](https://github.com/chromium/chromium/blob/master/docs/callback.md)



## partial application of function

Together with the `base::Bind{Once, Repeating}()` functions in [`base/bind.h`](https://github.com/chromium/chromium/blob/main/base/bind.h), they provide a type-safe method for performing partial application of functions

> NOTE: 
>
> 如果没有踩过类似的坑，则很难体会到上面这段话中 "type-safe method for performing partial application of functions" 的含义以及它的重要意义了，下面是一些相关内容:
>
> 一、qt [Signals & Slots](https://doc.qt.io/qt-5/signalsandslots.html)
>
> > A callback is a pointer to a function,
> >
> > callbacks can be unintuitive and may suffer from problems in ensuring the type-correctness of callback arguments.
>
> 二、我踩过的坑
>
> 使用dart ffigen依据C header file生成dart binding，它的内部实现是采用的function pointer，因此就存在 "problems in ensuring the type-correctness of callback arguments": 
>
> dll 是使用的旧版本
>
> dart binding是使用的新版本
>
> 两个版本恰好有一个函数新加了一个非常重要的参数
>
> dll中使用的是旧版本的，因此没有这个非常重要的参数；但是dart binding使用的是新的版本的，因此有这个非常重要的函数入参；
>
> 但是就是这样mismatch的dart binding 和 dll，竟然能够正常启动、运行，但是程序的输出是错误的，并且会偶然地core dump。导致这个非常重要的参数没有传入到dll中，从而导致了非常奇怪的问题。排查起来的难道是比较大的。
>
> 三、如何保证static type safety

Partial application is the process of binding a subset of a function's arguments to produce another function that takes fewer arguments. This can be used to pass around a unit of delayed execution, much like **lexical closures** are used in other languages. For example, it is used in Chromium code to schedule tasks on different `MessageLoop`s.


## partial application of function VS closure


"partial application of function" 其实 非常类似于 closure。

> Note that this is NOT the same as what other languages refer to as a closure -- it does not retain a reference to its enclosing environment.



## 类型、factory method


| 类型                         | no unbound parameter     | factory                 |          |
| ---------------------------- | ------------------------ | ----------------------- | -------- |
| `base::OnceCallback<void()>` | `base::OnceClosure`      | `base::BindOnce()`      | movable  |
| `base::RepeatingCallback`    | `base::RepeatingClosure` | `base::BindRepeating()` | copyable |




## [OnceCallback<> And RepeatingCallback<>](https://github.com/chromium/chromium/blob/master/docs/callback.md#oncecallback-and-repeatingcallback)





## [Quick reference for basic stuff](https://github.com/chromium/chromium/blob/master/docs/callback.md#quick-reference-for-basic-stuff)



## Running A Callback

Callbacks can be run with their `Run` method, which has the same signature as the template argument to the callback. Note that `base::OnceCallback::Run` consumes the callback object and can only be invoked on a callback rvalue.

> NOTE: 
>
> 上面这段话中的 "`base::OnceCallback::Run` consumes the callback object and can only be invoked on a callback rvalue" 要如何理解？
>
> 它对应的应该是:
>
> ```C++
> void DoSomethingOther(base::OnceCallback<void(int, std::string)> callback) {
> std::move(callback).Run(5, "hello");
> }
> ```
>
> 

## [Implementation notes](https://gitee.com/mirrors/chromium/blob/master/docs/callback.md#implementation-notes)



### [How The Implementation Works:](https://gitee.com/mirrors/chromium/blob/master/docs/callback.md#how-the-implementation-works)



# docs [OnceCallback<> and BindOnce(), RepeatingCallback<> and BindRepeating()](https://github.com/chromium/chromium/blob/master/docs/callback.md)



## 类型、factory method


| 类型                         | no unbound parameter     | factory                 |          |
| ---------------------------- | ------------------------ | ----------------------- | -------- |
| `base::OnceCallback<void()>` | `base::OnceClosure`      | `base::BindOnce()`      | movable  |
| `base::RepeatingCallback`    | `base::RepeatingClosure` | `base::BindRepeating()` | copyable |



## Introduction

### partial application of function

Together with the `base::Bind{Once, Repeating}()` functions in [`base/bind.h`](https://github.com/chromium/chromium/blob/main/base/bind.h), they provide a type-safe method for performing partial application of functions

> NOTE: 
>
> 如果没有踩过类似的坑，则很难体会到上面这段话中 "**type-safe** method for performing partial application of functions"  "**type-safe**"的含义以及它的重要意义了，下面是一些相关内容:
>
> 一、qt [Signals & Slots](https://doc.qt.io/qt-5/signalsandslots.html)
>
> > A callback is a pointer to a function,
> >
> > callbacks can be unintuitive(违反直觉的) and may suffer from problems in ensuring the type-correctness of callback arguments.
>
> 二、我踩过的坑
>
> 使用dart ffigen依据C header file生成dart binding，它的内部实现是采用的function pointer，因此就存在 "problems in ensuring the type-correctness of callback arguments"，下面的大致的情况: 
>
> dll 是使用的旧版本
>
> dart binding是使用的新版本
>
> 两个版本恰好有一个函数新加了一个非常重要的参数
>
> dll中使用的是旧版本的，因此没有这个非常重要的参数；但是dart binding使用的是新的版本的，因此有这个非常重要的函数入参；
>
> 但是就是这样mismatch的dart binding 和 dll，竟然能够正常启动、运行，但是程序的输出是错误的，并且会偶然地core dump。由于这个非常重要的参数没有传入到dll中，从而导致了非常奇怪的问题。排查起来的难道是比较大的。
>
> 三、如何保证static type safety？

Partial application is the process of binding a subset of a function's arguments to produce another function that takes fewer arguments. This can be used to pass around a unit of delayed execution, much like **lexical closures** are used in other languages. For example, it is used in Chromium code to schedule tasks on different `MessageLoop`s.




### `Callback` VS closure

A callback with no unbound input parameters (`base::OnceCallback<void()>`) is called a `base::OnceClosure`. The same pattern exists for `base::RepeatingCallback`, as `base::RepeatingClosure`. Note that this is NOT the same as what other languages refer to as a closure -- it does not retain a reference to its enclosing environment.

> NOTE: 这段话其实解释了chromium中的closure和我们平时所说的closure之间的差异: 
>
> chromium中的closure不会 "retain a reference to its enclosing environment"，但是标准的closure是会 "retain a reference to its enclosing environment"，并且标准的closure是具备capture功能的。
>

### `OnceCallback<>` And `RepeatingCallback<>`

> NOTE: 
>
> unique VS shared

`base::RepeatingCallback<>` is convertible to `base::OnceCallback<>` by the implicit conversion.

> NOTE: 如何实现implicit conversion呢？



### Memory Management And Passing

> NOTE: 这一段其实是在描述parameter passing和ownership，它其实和`std::unique_ptr` 、`std::shared_ptr` 的规则非常类似

Pass `base::{Once,Repeating}Callback` objects by value if ownership is transferred; otherwise, pass it by const-reference.

> NOTE: 
>
> 这段话总结的非常好的，它其实对应的是move ownership

```C++
// |Foo| just refers to |cb| but doesn't store it nor consume it.
bool Foo(const base::OnceCallback<void(int)>& cb) {
  return cb.is_null();
}

// |Bar| takes the ownership of |cb| and stores |cb| into |g_cb|.
base::RepeatingCallback<void(int)> g_cb;
void Bar(base::RepeatingCallback<void(int)> cb) {
  g_cb = std::move(cb);
}

// |Baz| takes the ownership of |cb| and consumes |cb| by Run().
void Baz(base::OnceCallback<void(int)> cb) {
  std::move(cb).Run(42);
}

// |Qux| takes the ownership of |cb| and transfers ownership to PostTask(),
// which also takes the ownership of |cb|.
void Qux(base::RepeatingCallback<void(int)> cb) {
  PostTask(FROM_HERE, base::BindOnce(cb, 42));
  PostTask(FROM_HERE, base::BindOnce(std::move(cb), 43));
}
```



When you pass a `base::{Once,Repeating}Callback` object to a function parameter, use `std::move()` if you don't need to keep a reference to it, otherwise, pass the object directly.



### Chaining callbacks

> NOTE:
>
> 这是典型的fluent API，这让我想起了folly的`future.then`

More concretely, calling `a.Then(b)` produces a new `base::OnceCallback` that will run `b(a());`, returning the result from `b`.

This example uses `Then()` to join 2 `base::OnceCallback`s together:

```C++
int Floor(float f) { return std::floor(f); }
std::string IntToString(int i) { return base::NumberToString(i); }

base::OnceCallback<int(float)> first = base::BindOnce(&Floor);
base::OnceCallback<std::string(int)> second = base::BindOnce(&IntToString);

// This will run |first|, run and pass the result to |second|, then return
// the result from |second|.
std::string r = std::move(first).Then(std::move(second)).Run(3.5f);
// |r| will be "3". |first| and |second| are now both null, as they were
// consumed to perform the join operation.
```

> NOTE: 
>
> 创建两个`Callback`，然后通过`Then`将它们串联起来；
>
> 需要注意的是，Then方法会创建一个新的`Callback`对象，上述`Run(3.5f)`是调用的这个新创建的`Callback`对象的。



This example uses `Then()` to join 2 `base::RepeatingCallback`s together:

```C++
int Floor(float f) { return std::floor(f); }
std::string IntToString(int i) { return base::NumberToString(i); }

base::RepeatingCallback<int(float)> first = base::BindRepeating(&Floor);
base::RepeatingCallback<std::string(int)> second = base::BindRepeating(&IntToString);

// This creates a RepeatingCallback that will run |first|, run and pass the
// result to |second|, then return the result from |second|.
base::RepeatingCallback<std::string(float)> joined =
    std::move(first).Then(std::move(second));
// |first| and |second| are now both null, as they were consumed to perform
// the join operation.

// This runs the functor that was originally bound to |first|, then |second|.
std::string r = joined.Run(3.5);
// |r| will be "3".

// It's valid to call it multiple times since all callbacks involved are
// base::RepeatingCallbacks.
r = joined.Run(2.5);
// |r| is set to "2".
```

In the above example, casting the `base::RepeatingCallback` to an r-value with `std::move()` causes `Then()` to destroy the original callback, in the same way that occurs for joining `base::OnceCallback`s.

> NOTE: `Then()` 如何实现分辨出它是由r-value 调用的呢？关于此，参见:
>
> 1、foonathan [Guidelines For Rvalue References In APIs](https://www.foonathan.net/2018/03/rvalue-references-api-guidelines/)
>
> 2、stackoverflow [LValue ref qualified member function being called on an RValue object](https://stackoverflow.com/questions/60427227/lvalue-ref-qualified-member-function-being-called-on-an-rvalue-object)
>
> 3、wiggling-bits [Introducing ref-qualifiers for member functions](https://wiggling-bits.net/introducing-ref-qualifiers-for-member-functions/)
>
> 简而言之: 通过"ref-qualifier"

However since a `base::RepeatingCallback` can be run multiple times, it can be joined non-destructively as well.

```C++
int Floor(float f) { return std::floor(f); }
std::string IntToString(int i) { return base::NumberToString(i); }

base::RepeatingCallback<int(float)> first = base::BindRepeating(&Floor);
base::RepeatingCallback<std::string(int)> second = base::BindRepeating(&IntToString);

// This creates a RepeatingCallback that will run |first|, run and pass the
// result to |second|, then return the result from |second|.
std::string r = first.Then(second).Run(3.5f);
// |r| will be 3, and |first| and |second| are still valid to use.

// Runs Floor().
int i = first.Run(5.5);
// Runs IntToString().
std::string s = second.Run(9);
```

#### `base::IgnoreResult`

If the second callback does not want to receive a value from the first callback, you may use `base::IgnoreResult` to drop the return value in between running the two.

> NOTE: 
>
> 非常的强大和灵活

```C++
// Returns an integer.
base::RepeatingCallback<int()> first = base::BindRepeating([](){ return 5; });
// Does not want to receive an integer.
base::RepeatingClosure second = base::BindRepeating([](){});

// This will not compile, because |second| can not receive the return value from
// |first|.
//   first.Then(second).Run();

// We can drop the result from |first| before running second.
base::BindRepeating(base::IgnoreResult(first)).Then(second).Run();
// This will effectively create a callback that when Run() will call
// `first(); second();` instead of `second(first());`.
```

### Splitting a `OnceCallback` in two

If a callback is only run once, but two references need to be held to the callback, using a `base::OnceCallback` can be clearer than a `base::RepeatingCallback`, from an intent and semantics point of view. `base::SplitOnceCallback()` takes a `base::OnceCallback` and returns a pair of callbacks with the same signature. When either of the returned callback is run, the original callback is invoked. Running the leftover callback will result in a crash. This can be useful when passing a `base::OnceCallback` to a function that may or may not take ownership of the callback. E.g, when an object creation could fail:

> NOTE: 
>
> 一、"When either of the returned callback is run, the original callback is invoked. Running the leftover callback will result in a crash. "
>
> 上面这段话的意思是: 两者中的任意一个给执行，则另外一个就是null了
>
> 二、"This can be useful when passing a `base::OnceCallback` to a function that may or may not take ownership of the callback"
>
> 需要结合下面的例子来理解这段话，它的意思是: 将`base::OnceCallback` 传入到一个函数中，这个函数是可能不执行或者不"take ownership of the callback"的，这种情况是可能出现在这个函数内部创建的object失败时，显然对于这种情况，如果不进行处理的话，是可能导致leakage的。
>
> 结合下面的例子来看，它提出的方案是: `SplitOnceCallback`，为原callback创建一个split，两个之一能够被绑定就能够保证不被泄露，下面的例子就展示了这种方案:

```C++
std::unique_ptr<FooTask> CreateFooTask(base::OnceClosure task) {
  std::pair<base::OnceClosure,base::OnceClosure> split
                                    = base::SplitOnceCallback(std::move(task));

  std::unique_ptr<FooTask> foo = TryCreateFooTask(std::move(split.first));
  if (foo)
    return foo;

  return CreateFallbackFooTask(std::move(split.second));
}
```

> NOTE: 
>
> 如果 `foo` 是true，则表示`TryCreateFooTask`已经获得了`task`的ownership；否则它没有，在这种情况下， 如果不对`task`绑定，那么就会导致leakage。

While it is best to use a single callback to report success/failure, some APIs already take multiple callbacks. `base::SplitOnceCallback()` can be used to split a **completion callback** and help in such a case:

```C++
using StatusCallback = base::OnceCallback<void(FooStatus)>;
void DoOperation(StatusCallback done_cb) {
  std::pair<StatusCallback, StatusCallback> split
                                 = base::SplitOnceCallback(std::move(done_cb));

  InnerWork(BindOnce(std::move(split.first), STATUS_OK),
            BindOnce(std::move(split.second), STATUS_ABORTED));
}

void InnerWork(base::OnceClosure work_done_cb,
               base::OnceClosure work_aborted_cb);
```

> NOTE: 
>
> 如果有多个分支怎么办？



## [Quick reference for basic stuff](https://github.com/chromium/chromium/blob/master/docs/callback.md#quick-reference-for-basic-stuff)

### Binding A Class Method

The first argument to bind is the member function to call, the second is the object on which to call it.

```C++
class Ref : public base::RefCountedThreadSafe<Ref> {
 public:
  int Foo() { return 3; }
};
scoped_refptr<Ref> ref = new Ref();
base::RepeatingCallback<void()> ref_cb = base::BindRepeating(&Ref::Foo, ref);
LOG(INFO) << ref_cb.Run();  // Prints out 3.
```

By default the object must support `RefCounted` or you will get a compiler error. If you're passing between threads, be sure it's `RefCountedThreadSafe`! See "Advanced binding of member functions" below if you don't want to use reference counting.

> NOTE: 
>
> 为什么一定要支持`RefCounted` ？

### Running A Callback

Callbacks can be run with their `Run` method, which has the same signature as the template argument to the callback. Note that `base::OnceCallback::Run` consumes the callback object and can only be invoked on a **callback rvalue**.

> NOTE: 
>
> 一、"Note that `base::OnceCallback::Run` consumes the callback object and can only be invoked on a **callback rvalue**." 
>
> 这是通过ref-qualified member function来实现的
>
> 难道在`Run`方法中，会将对象给析构掉吗?
>
> 

```C++
void DoSomethingOther(base::OnceCallback<void(int, std::string)> callback) {
  std::move(callback).Run(5, "hello");
}
```

`RepeatingCallbacks` can be run more than once (they don't get deleted or marked when run). However, this precludes using `base::Passed` (see below).

> NOTE: 
>
> "preclude"的含义是"不包括"

If running a callback could result in its own destruction (e.g., if the callback recipient(接受者) deletes the object the callback is a member of), the callback should be moved or copied onto the stack before it can be safely invoked. (Note that this is only an issue for `RepeatingCallbacks`, because a `OnceCallback` always has to be moved for execution.)

> NOTE: 
>
> 这段话是什么意思？没有理解



### Creating a Callback That Does Nothing

> NOTE: 
>
> empty `Callback`，它让我想到了empty lambda

Default-constructed callbacks are null, and thus cannot be Run(). Instead, use `base::DoNothing()`:

```
...
MyFunction(base::DoNothing());  // Can be Run(), will no-op
```

> NOTE: 
>
> 这是典型的named factory pattern

`base::DoNothing()` can be passed for any `OnceCallback` or `RepeatingCallback` that returns void.

> NOTE: 
>
> 这是如何实现的？

### Passing Unbound Input Parameters

> NOTE: 
>
> 当调用`Run`的时候指定

### Passing Bound Input Parameters

> NOTE: 

They will be passed to the function and the `Run()`ner of the callback doesn't see those values or even know that the function it's calling.

> NOTE: 
>
> 一、"`Run()`ner of the callback doesn't see those values or even know that the function it's calling"
>
> 这段话的含义是: `Run()`ner of the callback (callback 的运行者) 是无法看到这些bound input parameters 和 the function的

### Avoiding Copies With Callback Parameters

A parameter of `base::BindRepeating()` or `base::BindOnce()` is moved into its internal storage if it is passed as a **rvalue**.



Avoid using `base::Passed()` with `base::BindOnce()`, as `std::move()` does the same thing and is more familiar.

> NOTE: 
>
> `base::Passed()` 所做的其实就是 `std::move()` 

## Quick reference for advanced binding

### Binding A Class Method With Weak Pointers

If `MyClass` has a `base::WeakPtr<MyClass> weak_this_` member (see below) then a class method can be bound with:

```c++
base::BindOnce(&MyClass::Foo, weak_this_);
```

The callback will not be run if the object has already been destroyed.

To use `base::WeakPtr` with `base::Bind{Once, Repeating}()` as the `this` pointer to a method bound in a callback, `MyClass` will typically look like:

```C++
class MyClass {
public:
  MyClass() {
    weak_this_ = weak_factory_.GetWeakPtr();
  }
private:
  base::WeakPtr<MyClass> weak_this_;
  // MyClass member variables go here.
  base::WeakPtrFactory<MyClass> weak_factory_{this};
};
```

`weak_factory_` is the last member variable in `MyClass` so that it is destroyed first. This ensures that if any class methods bound to `weak_this_` are `Run()` during teardown, then they will not actually be executed.

> NOTE: 
>
> initialization 和 deinitialization order

### Binding A Class Method With Manual Lifetime Management

```C++
base::BindOnce(&MyClass::Foo, base::Unretained(this));
```

> NOTE: 
>
> 上面的`base::Unretained(this)`让我想起了前面的"Note that this is NOT the same as what other languages refer to as a closure -- it does not retain a reference to its enclosing environment"

This disables all lifetime management on the object. You're responsible for making sure the object is alive at the time of the call. You break it, you own it!

### Binding A Class Method And Having The Callback Own The Class

```C++
MyClass* myclass = new MyClass;
base::BindOnce(&MyClass::Foo, base::Owned(myclass));
```

The object will be deleted when the callback is destroyed, even if it's not run (like if you post a task during shutdown). Potentially useful for "fire and forget" cases.

> NOTE: 非常巧妙

Smart pointers (e.g. `std::unique_ptr<>`) are also supported as the receiver.

```C++
std::unique_ptr<MyClass> myclass(new MyClass);
base::BindOnce(&MyClass::Foo, std::move(myclass));
```

> NOTE: 
>
> 一、`std::move(std::unique_ptr<MyClass>)`这是典型的表达transfer ownership语义
>
> 二、`base::owned` 的含义是让 "`Callback`  Own The Class"，即transfer ownership。
>
> 



### Ignoring Return Values

> NOTE: 
>
> ```C++
> base::IgnoreResult
> ```





## Quick reference for binding parameters to `BindOnce()` and `BindRepeating()`

### Passing Parameters Owned By The Callback

> NOTE: 
>
> 这段话中描述的内容其实在前面已经论述了

```C++
void Foo(int* arg) { cout << *arg << endl; }
int* pn = new int(1);
base::RepeatingClosure foo_callback = base::BindRepeating(&foo, base::Owned(pn));
```

The parameter will be deleted when the callback is destroyed, even if it's not run (like if you post a task during shutdown).

> NOTE: 
>
> 一、"you post a task during shutdown" 对于这种场景，即 "during shutdown"，`Callback`能否被执行是不确定的，但是resource肯定是需要被release的，显然它将resource ownership转换给了`Callback` object，让它来负责release

### Passing Parameters As A `unique_ptr`

> NOTE: 
>
> 它其实和上面是相同的情况

```C++
void TakesOwnership(std::unique_ptr<Foo> arg) {}
auto f = std::make_unique<Foo>();
// f becomes null during the following call.
base::OnceClosure cb = base::BindOnce(&TakesOwnership, std::move(f));
```

### Passing Parameters As A `scoped_refptr`

```C++
void TakesOneRef(scoped_refptr<Foo> arg) {}
scoped_refptr<Foo> f(new Foo);
base::RepeatingClosure cb = base::BindRepeating(&TakesOneRef, f);
```

> NOTE: 
>
> 这是典型的 `std::shared_ptr` 的用法

This should "just work." The closure will take a reference as long as it is alive, and another reference will be taken for the called function.

> NOTE: 
>
> 一、"This should "just work." "是什么含义呢？
>
> 二、"The closure will take a reference as long as it is alive, and another reference will be taken for the called function."
>
> 对于上面这段话的理解是需要结合前面的代码的: `f` 是一个 `scoped_refptr`，将它传入到 `base::BindRepeating` 中时，则"The closure will take a reference as long as it is alive"；
>
> 由于函数 `TakesOneRef(scoped_refptr<Foo> arg)` 的入参类型是 `scoped_refptr<Foo>` ，因此 "another reference will be taken for the called function"

```C++
void DontTakeRef(Foo* arg) {}
scoped_refptr<Foo> f(new Foo);
base::RepeatingClosure cb = base::BindRepeating(&DontTakeRef, base::RetainedRef(f));
```

`base::RetainedRef` holds a reference to the object and passes a raw pointer to the object when the Callback is run.

> NOTE: 
>
> 一、对于上面这段话的理解是需要对比上一段代码和这段代码的: 
>
> 1、上面: `TakesOneRef(scoped_refptr<Foo> arg)`  的入参类型是 `scoped_refptr<Foo>` ，显然它是会 "take a reference" 的；
>
> 2、本段: `DontTakeRef(Foo* arg)` 的入参类型是pointer，显然它是不会 "take a reference" 的；对于这种情况，是需要使用 `base::RetainedRef(f)` 的

### Binding Const Reference Parameters

If the callback function takes a const reference parameter then the value is *copied* when bound unless `std::ref` or `std::cref` is used. 

> NOTE: 
>
> 这段话的意思是: "const reference parameter"的默认行为是会被copy的，如果想改变这种行为，可以使用`std::ref` or `std::cref` 

Example:

```C++
void foo(const int& arg) { printf("%d %p\n", arg, &arg); }
int n = 1;
base::OnceClosure has_copy = base::BindOnce(&foo, n);
base::OnceClosure has_ref = base::BindOnce(&foo, std::cref(n));
n = 2;
foo(n);                                   // Prints "2 0xaaaaaaaaaaaa"
std::move(has_copy).Run();                // Prints "1 0xbbbbbbbbbbbb"
std::move(has_ref).Run();                 // Prints "2 0xaaaaaaaaaaaa"
```

Normally parameters are copied in the closure. **DANGER**: `std::ref` and `std::cref` store a (const) reference instead, referencing the original parameter. This means that you must ensure the object outlives the callback!



### Binding Non-Const Reference Parameters

> NOTE:
>
> prefer explicit over implicit

If the callback function takes a **non-const reference** then the bind statement must specify what behavior is desired. If a reference that can mutate the original value is desired then `std::ref` is used. If the callback should take ownership of the value, either by making a copy or moving an existing value, then `base::OwnedRef` is used. If neither is used the bind statement will fail to compile.

```C++
void foo(int& arg) {
  printf("%d\n", arg);
  ++arg;
}

int n = 0;
base::RepeatingClosure has_ref = base::BindRepeating(&foo, std::ref(n));
base::RepeatingClosure has_copy = base::BindRepeating(&foo, base::OwnedRef(n));

foo(n);                        // Prints "0"
has_ref.Run();                 // Prints "1"
has_ref.Run();                 // Prints "2"
foo(n);                        // Prints "3"

has_copy.Run();                // Prints "0"
has_copy.Run();                // Prints "1"

// This will fail to compile.
base::RepeatingClosure cb = base::BindRepeating(&foo, n);
```





## [Implementation notes](https://gitee.com/mirrors/chromium/blob/main/docs/callback.md#implementation-notes)

### Customizing the behavior

```C++
namespace base {

template <typename Receiver>
struct IsWeakReceiver {
  static constexpr bool value = false;
};

template <typename Obj>
struct UnwrapTraits {
  template <typename T>
  T&& Unwrap(T&& obj) {
    return std::forward<T>(obj);
  }
};

}  // namespace base
```

`base::UnwrapTraits<BoundObject>::Unwrap()` is called for each **bound argument** right before the callback calls the target function. You can specialize this to define an **argument wrapper** such as `base::Unretained`, `base::Owned`, `base::RetainedRef` and `base::Passed`.

> NOTE: 在"callback"调用"target function"之前会对每个"bound argument"调用 `base::UnwrapTraits<BoundObject>::Unwrap()`

> NOTE: 其中也提出了**argument wrapper**的概念；显然如果对argument进行了wrap，那么就需要在调用之前进行unwrap。后面的内容中会对**argument wrapper**进行非常详细的说明



### [How The Implementation Works:](https://gitee.com/mirrors/chromium/blob/main/docs/callback.md)


There are three main components to the system:


1、The `base::{Once, Repeating}Callback<>` classes.

2、The `base::BindOnce() and base::BindRepeating()` functions.

3、The arguments wrappers (e.g., `base::Unretained()` and `base::Owned()`).

> NOTE: 需要思考: 为什么需要argument wrapper。我觉得使用argument wrapper，显然是引入了一层控制，这样这个system就更加灵活了，能够让programmer "Customizing the behavior"


The `base::{Once, Repeating}Callback` constructor takes a `base::BindStateBase*`, which is **upcasted** from a `base::BindState<>`. `base::BindOnce()` or `base::BindRepeating()` creates the `base::BindState<>` that has the full static type, and erases the target function type as well as the types of the bound parameters. It does this by storing a pointer to the specific `Run()` function, and upcasting the state of `base::BindState<>*` to a `base::BindStateBase*`. This is safe as long as this `BindStateBase` pointer is only used with the stored `Run()` pointer.

> NOTE: 
> `base::BindStateBase*` 是base class
> `upcast`的意思是: 从derived class到base class

> NOTE: 
>
> 一、`base::BindState<>`  是derived class，它是implementation，因此 "`base::BindState<>` that has the full static type"
>
> 二、上面这段话是需要结合下面的具体的代码来进行理解的
>
> 三、使用到了type erasure技术，那它是如何进行type erasure的呢？简而言之是: abstraction class + template implementation。在上面这段话中也有描述: "It does this by storing a pointer to the specific `Run()` function, and upcasting the state of `base::BindState<>*` to a `base::BindStateBase*`. This is safe as long as this `BindStateBase` pointer is only used with the stored `Run()` pointer."
> 上面这段话中提及了'type safe'，让我联想到的是: 什么情况下是非type safe的呢？

These bind functions, along with a set of internal templates, are responsible for

1、Unwrapping the function signature into return type, and parameters

> NOTE: 上面这段话中的"Unwrapping"的含义是 "拆解"，即将 "function signature" 拆解为 "return type"、"parameters"

2、Determining the number of parameters that are bound

> NOTE: 确定已经绑定的parameter的数量；那这如何来进行实现呢？可以肯定的是: "the number of parameters that are bound"在编译时就能够确定

3、Creating the `BindState` storing the bound parameters

> NOTE: 

4、Performing compile-time asserts to avoid error-prone behavior

> NOTE: 保证static safety

5、Returning a `Callback<>` with an arity matching the number of unbound parameters and that knows the correct refcounting semantics for the **target object** if we are binding a method.

> NOTE: `bind`后，其实创建了一个新的`Callback<>` (function)，显然这个`Callback<>` (function)的 "arity" 等于 "the number of unbound parameters"

> NOTE: "arity"的含义是 "参数数量"、"元数"

> NOTE: "the correct refcounting semantics for the target object if we are binding a method" 这段话要如何理解？其中提出了一个概念: "target object"

The `base::Bind` functions do the above using type-inference and variadic templates.

#### bind parameter and argument wrapper and `Unwrap()`

By default `base::Bind{Once, Repeating}()` will store copies of all bound parameters, and attempt to refcount a **target object** if the function being bound is a **class method**. These copies are created even if the function takes parameters as **const references**. (Binding to **non-const references** is forbidden, see `bind.h`.)

> NOTE: 上面描述的是default behavior

> NOTE: "Binding to non-const references is forbidden, see `bind.h`" ，为什么是这样？ 能否bind to **non-const pointer**。

To change this behavior, we introduce a set of **argument wrappers** (e.g., `base::Unretained()`). These are simple container templates that are passed by value, and wrap a pointer to argument. Each helper has a comment describing it in `base/bind.h`.

These types are passed to the `Unwrap()` functions to modify the behavior of `base::Bind{Once, Repeating}()`. The `Unwrap()` functions change behavior by doing partial specialization based on whether or not a parameter is a wrapper type.

`base::Unretained()` is specific to Chromium.

### Missing Functionality

1、Binding arrays to functions that take a non-const pointer. Example:

```
void Foo(const char* ptr);
void Bar(char* ptr);
base::BindOnce(&Foo, "test");
base::BindOnce(&Bar, "test");  // This fails because ptr is not const.
```

2、In case of partial binding of parameters a possibility of having unbound parameters before bound parameters. Example:

```
void Foo(int x, bool y);
base::BindOnce(&Foo, _1, false); // _1 is a placeholder.
```


# chromium `weakptr`

[chromium/base/memory/weak_ptr.h](https://github.com/chromium/chromium/blob/master/base/memory/weak_ptr.h)

[chromium](https://github.com/chromium/chromium/tree/master)/[base](https://github.com/chromium/chromium/tree/master/base)/[memory](https://github.com/chromium/chromium/tree/master/base/memory)/[**weak_ptr.cc**](https://github.com/chromium/chromium/blob/master/base/memory/weak_ptr.cc)

极好的避免dangling。

## CSDN [浅谈chromium中的指针管理](https://blog.csdn.net/dzhj11/article/details/10550299) # `weakptr`
> NOTE: 这篇文章重点描述了`weakptr`，基本上讲清楚了weakptr的

在chromium的多线程模型里，对象通常被不同的线程访问，这就牵涉到**线程同步**问题；传统方法采用锁机制，关于锁机制的优劣参见Chrome源码剖析。 chromium的多线程模型实现的一个基本原则是：尽量规避锁；采取的策略则是task机制。

> NOTE: 
>
> 一、chromium采用的是task model，因此就涉及了在task被执行的时候，它需要访问的object的lifetime的问题，即下面这段话中的: 
>
> "在该模型中一个最核心的问题就是**task传递、运行过程中所涉及的对象、数据的安全性问题**"

假设如下场景，A线程需要B线程做一些事情，然后回到A线程继续做一些事情；在chromium下你可以这样来做：生成一个Task，放到B线程的队列中，在该Task的Run方法最后，会生成另一个Task，这个Task会放回到A的线程队列，由A来执行。在该模型中一个最核心的问题就是**task传递、运行过程中所涉及的对象、数据的安全性问题**。在考虑上述的场景，线程A委托给线程B task1 ，task1运行需要访问对象object1；同样，线程B委托给线程A task2，task2 需要访问对象object2. 由于线程都采用消息队列机制，即轮询task队列方式实现。这就可能存在如下问题：

1、线程B 运行 task1时 object1已经被其他线程销毁；

2、线程A运行task2是 object2已经被其他线程销毁；

这里你可以说如果我使用**引用计数**机制，确保object1和object2都不会被销毁当然可以解决问题。但更多的情况下我们可能需要的另一种情况，即取消task1、task2的运行。例如UI线程委托IO线程下载某个资源时，用户刷新了当前网页，显然这时IO线程的下载task应取消。这个问题的核心就是：**如何构建一种机制，确保能正确监测指针所指向对象是否有效**；也就是如果其实线程B检测到Object1已经无效了，则自动取消task1的运行。这实质也是weakptr 弱指针的所需要解决的问题。

> NOTE: 
>
> 一、track object lifetime


在谈论weakptr之前，我们先来探讨现实中一般如何解决这类问题。如果这个时候，存在一个**第三方认证机构C**，A和B能够通过向**机构C**来查询获取某个object的状态来确定是否执行某个task，这样问题就既然就得以解决了。实质上，这也是weakptr所采取的方法。先看weakptr的定义：

```C++
Template<class T>    
Class  Weakptr : public internal::WeakPtrBase {  
    // …….  
    T* ptr；  
}；  
Class WeakPtrBase {  
Protected：  
        WeakReference ref_;  
}
```

显然`WeakPtr` 弱指针除了包括所指对象的指针外，还包括一个额外的**弱引用** `WeakReference ref_`。`ref_`可以看作是对**第三方机构C**的引用，即通过判断`ref_`是否有效来判定`WeakPtr`所保存的指针`ptr`是否有效。下面是`WeakReference`的定义：


```C++
class WeakReference {
public:
	class Flag : public RefCounted, public NonThreadSafe {
	public:
		Flag(Flag** handle);
		~Flag();

		void AddRef() const;
		void Release() const;
		void Invalidate() { handle_ = NULL; }
		bool is_valid() const { return handle_ != NULL; }

		void DetachFromThread() { NonThreadSafe::DetachFromThread(); }

	private:
		Flag** handle_;
	};
	WeakReference();
	WeakReference(Flag* flag);
	~WeakReference();

	bool is_valid() const;

private:
	scoped_refptr flag_;
}

```

由上面定义可知有效性的检测实质由 `WeakReference` 内的 `Flag` 对象提供。接下来问题就是**第三方机构C**如何建立？ `WeakReference ref_` 又如何使用它呢？这牵涉到类`WeakReferenceOwner`和`WeakPtrFactory`。 这里有一个事实应首先清楚：在上面的场景中 Object1可能被task1所使用、也可能被task2、… taskN所使用，而且这些task可能还在不同的线程中运行。因此为确保所有线程运行这些task时都能检测到Object1是否有效，显然这里要求`WeakReference ref_` 所实质引用的对象是同一个，而且要确保该对象的生命周期可能长于`Object1`，即`Object1`被销毁了它还存在。 类`WeakReferenceOwner` 顾名思义，是`WeakReference`的`owner`，即它用于保证所有相关`WeakPtr` 中`WeakReference ref_`引用的是同一个对象，具体到Chromium中即为同一个`Flag`对象。见 `WeakReferenceOwner::GetRef()` 的实现：

```C++
WeakReference WeakReferenceOwner::GetRef() const {  
    if (!flag_)  
        flag_ = new WeakReference::Flag(&flag_);  
    return WeakReference(flag_);  
} 
```
接下来的问题就是如何创建`WeakPtr`对象，显然这里应有如下两个需求：

1、创建出来的`WeakPtr`跟实质所包含的对象是相关的，即设置`ptr`指针；

2、`WeakReference ref_` 最终所引用的`flag` 是同一个。

这通过`WeakPtrFactory` 实现，`WeakPtrFactory`用于创建某个Object的`Weakptr`，定义如下：

```C++
template <typename T>  
class WeakPtrFactory {  
public:  
    explicit WeakPtrFactory(T* ptr) : ptr_(ptr) {}  
   
    WeakPtr GetWeakPtr() {  
        return WeakPtr(weak_reference_owner_.GetRef(), ptr_);  
    }  
   
    // Call this method to invalidate all existing weak pointers.  
    void InvalidateWeakPtrs() {  
        weak_reference_owner_.Invalidate();  
    }  
   
    // Call this method to determine if any weak pointers exist.  
    bool HasWeakPtrs() const {  
        return weak_reference_owner_.HasRefs();  
    }  
   
private:  
    internal::WeakReferenceOwner weak_reference_owner_;  
    T* ptr_;  
    DISALLOW_IMPLICIT_CONSTRUCTORS(WeakPtrFactory);  
};  

```

`WeakPtrFactory` 聚合了 `WeakReferenceOwner`，用于保证指向同一个 `Flag`。对于`WeakPtrFactory`的使用，可以在 相关Object中聚合一个该`WeakPtrFactory`， 这样可以直接通过Object的某个方法来获取其`WeakPtr`对象，例如：

```c++
Object1->weakptrfactory()->GetWeakPtr();
```

同时在Object的**析构函数**中调用`WeakPtrFactory`的`InvalidateWeakPtrs`函数，来使其`Flag`无效，最终供`WeakPtr`使用者检测其有效性。
> NOTE: 这是关键所在

对于另一个问题，如何确保所有`WeakReference ref_` 所引用的`Flag`对象的生命周期长于Object本身，即Object销毁时它仍然存在，直到所有的`WeakReference ref_`都销毁时。显然，从`Flag`的定义可以看出，采用引用计数方式即可。
> NOTE: 所有的都指向同一个`Flag`，这样当"Object的**析构函数**中调用`WeakPtrFactory`的`InvalidateWeakPtrs`函数，来使其`Flag`无效"时，所有的Weakptr object都能够同时读到这个value

关于`WeakPtr`的关系见下图：



## doc

```C++
// Weak pointers are pointers to an object that do not affect its lifetime,
// and which may be invalidated (i.e. reset to nullptr) by the object, or its
// owner, at any time, most commonly when the object is about to be deleted.
```

一、`weak_ptr` 被 "invalidated"的原因: 

1、"reset to nullptr"

2、"be deleted"



```C++
// Weak pointers are useful when an object needs to be accessed safely by one
// or more objects other than its owner, and those callers can cope with the
// object vanishing and e.g. tasks posted to it being silently dropped.
// Reference-counting such an object would complicate the ownership graph and
// make it harder to reason about the object's lifetime.
```

一、"object vanishing " 的含义是 "对象消失"

二、chromium中"Reference-counting"和`weak_ptr`如何一起使用？

### EXAMPLE



```C++
class Controller {
public:
	void SpawnWorker() {
		Worker::StartNew(weak_factory_.GetWeakPtr());
	}
	void WorkComplete(const Result& result) {
		...
	}
private:
	// Member variables should appear before the WeakPtrFactory, to ensure
	// that any WeakPtrs to Controller are invalidated before its members
	// variable's destructors are executed, rendering them invalid.
	WeakPtrFactory<Controller> weak_factory_{ this };
};

class Worker {
public:
	static void StartNew(WeakPtr<Controller> controller) {
		Worker* worker = new Worker(std::move(controller));
		// Kick off asynchronous processing...
	}
private:
	Worker(WeakPtr<Controller> controller)
		: controller_(std::move(controller)) {}
	void DidCompleteAsynchronousProcessing(const Result& result) {
		if (controller_) // 此处就是使用weak_ptr来track object lifetime
			controller_->WorkComplete(result);
	}
	WeakPtr<Controller> controller_;
};

```

一、`Controller::WorkComplete` 会被 `Worker` 回调。

二、`Worker` 的 `StartNew` 中，会调用 `DidCompleteAsynchronousProcessing`，从而将 `result` 告诉 `Controller`。

### Thread-safety
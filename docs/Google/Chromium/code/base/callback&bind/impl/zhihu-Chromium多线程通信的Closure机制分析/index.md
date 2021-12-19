# zhihu [Chromium多线程通信的Closure机制分析](https://zhuanlan.zhihu.com/p/23501067?from_voters_page=true)

> NOTE: 
>
> 实现方式已经改变，这篇文章的参考意义并不大了

为了充分利用CPU多核特性，Chromium在启动时会创建很多线程，来负责执行不同的操作。这样就涉及到了多线程通信问题。Chromium为每一个线程都创建了一个[消息队列](https://www.zhihu.com/search?q=消息队列&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra={"sourceType"%3A"article"%2C"sourceId"%3A23501067})。当一个线程需要另一个线程执行某一操作时，就向该线程的消息队列发送一个Closure。这个Closure最终在目标线程中得到执行。这种基于Closure的多线程通信方式在Chromium中使用得很普通，因此本文就对它的实现进行分析。

> NOTE: 
>
> closure-based concurrency

## Callback从创建到调用

> NOTE: 
>
> `base::Callback` 的模板参数描述的是 `Callback` 的函数类型，而不是原函数的函数类型，关于这一点，可以从下面的例子看出

一个Callback从创建到调用的代码示例如下所示：

![img](https://pic2.zhimg.com/80/v2-0e9bb96d3d50ca10b68e3889050e740d_720w.png)

![img](https://pic1.zhimg.com/80/v2-e011f122619a3a5abd0c7105aa305d34_720w.png)

## 避免access outside of object lifetime

这里绑定的类对象就为myclass，并且是以裸指针的形式指定的。这时候我们就需要保证Callback对象cb的成员函数Run被调用时，对象myclass还没有被销毁。

> NOTE: 
>
> 这是问题所在，chromium提供了smart pointer来解决这个问题:
>
> 1、`scoped_refptr`
>
> 2、`WeakPtr`



## chromium closure(`Callback`)的实现

在分析`Callback`对象的创建和调用过程之前，我们首先看它的类关系图，如图1所示：

![img](https://pic3.zhimg.com/80/v2-02ce225e353011c2ae23f36bc68e0746_720w.jpg)



图1 `Callback`类关系图

> NOTE: 
>
> 1、[chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/main/base)/**[callback.h](https://github.com/chromium/chromium/blob/main/base/callback.h)**
>
> 2、[chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/main/base)/**[callback_internal.h](https://github.com/chromium/chromium/blob/main/base/callback_internal.h)**
>
> 3、[chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/main/base)/**[callback_internal.cc](https://github.com/chromium/chromium/blob/main/base/callback_internal.cc)**
>
> 

从图1可以看到，`Callback`类是从`CallbackBase`类继承下来的。`CallbackBase`类有两个成员变量`bind_state_`和`polymorphic_invoke_`。其中，成员变量`bind_state_`指向的是一个`BindState`对象，该`BindState`对象是保存了其宿主`Callback`对象创建时**绑定的函数以及参数**。另外一个成员变量`polymorphic_invoke_`指向的是一个`InvokeFuncStorage`对象，该`InvokeFuncStorage`对象描述的是`Invoker`类的静态成员函数`Run`。

> NOTE: 
>
> 当前的implementation已经不是这样了

`BindState`类是从`BindStateBase`类继承下来的，`BindStateBase`类又是从`RefCountedThreadSafe`类继承下来的。从前面[Chromium和WebKit的智能指针实现原理分析](https://link.zhihu.com/?target=http%3A//blog.csdn.net/luoshengyang/article/details/46598223)一文可以知道，从`RefCountedThreadSafe`继承下来的类可以配合**智能指针**来使用，因此我们就可以看到`CallbackBase`类的成员变量`bind_state_`是一个引用了`BindStateBase`对象的`scoped_refptr`智能指针。

> NOTE: 
>
> Source codd: [chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/main/base)/**[callback_internal.h](https://github.com/chromium/chromium/blob/main/base/callback_internal.h)**
>
> ```C++
> class BASE_EXPORT CallbackBase {  
>   scoped_refptr<BindStateBase> bind_state_;
> };
> ```
>
> 



`BindState`类将宿主`Callback`对象创建时绑定的参数保存在成员变量`p1_`、`p2_`等中，绑定的函数则保存在成员变量`runnable_`指向的一个`RunnableAdapter`对象的成员变量`function_`中。

> NOTE:
>
> 现在的实现也就不是这样了。
>
> 
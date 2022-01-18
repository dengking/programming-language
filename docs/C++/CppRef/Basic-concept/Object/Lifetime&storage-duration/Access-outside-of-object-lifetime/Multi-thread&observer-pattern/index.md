# Access outside of object lifetime in multi thread application

在multi thread application是非常容易出现access outside of object lifetime的，下面分情况讨论，对于每种情况，给出具体的案例、解决方案。

## 问题梳理

### Multiple-thread share a pointer and [Dangling pointer](https://en.wikipedia.org/wiki/Dangling_pointer)

当一个pointer由多个thread访问，那么对于这个pointer的管理就变得棘手了：

1、谁来负责pointer的释放

2、如果某个thread释放了pointer所指向的object，那么其他thread如果继续使用该pointer的话，则就会导致[Dangling pointer](https://en.wikipedia.org/wiki/Dangling_pointer)；

显然在multithread中，是非常容易出现dangling pointer的。

### Access uninitialized object

1、下面的"在constructor中register callback/listener/observer"段中，就是描述的这种情况

2、`aristeia-C++and-the-Perils-of-Double-Checked-Locking` 章节中，也描述了这种情况了

## Example: observer pattern

Event-driven/network library中，一般采用observer pattern，传入一个object来作为callback。将一个object作为callback传入到另外一个event driven library中的做法是非常不好的，这种情况下，一般涉及multithread，这个object同时被多个thread access，这样的做法有如下劣势就是非常容易access outside of object lifetime。

> NOTE: 这种情况的最最典型的例子是: amust api。



### Asynchronous event导致调用dangling pointer 

在TCP SDK（网络库，可以认为是一个[Event-driven architecture](https://en.wikipedia.org/wiki/Event-driven_architecture)），一般采 [Observer pattern](https://en.wikipedia.org/wiki/Observer_pattern) ；这种模式的一个特点就是: 将**Listener、observer对象**传入到TCP SDK（网络库），一般是通过pointer、reference的方式进行注册；TCP SDK会network的各种event通过回调**Listener对象**的成员方法通知到应用层，比如将**连接断开**的事件通过调用**Listener对象**的`OnDisconnected`通知到应用层。下面描述了在这种模式下非常容易出现的一个问题:

如果TCP SDK的网络断开是**异步**的，并且不对**Listener对象**的lifetime的进行控制；那么就存在这样的一种可能: **Listener对象**已经被释放了，而TCP SDK并不知晓，依然调用`OnDisconnected`，则这样就发生了本节标题中描述的dangling pointer错误，导致程序core dump。

#### 修正方法: synchronization

> NOTE: 其实这也是 "take control and make it computational and ordering"

对网络连接的断开、object的lifetime不进行**控制**而导致的问题，修正方法是：首先将网络连接关闭（使用阻塞的方法），待关闭完成后（`OnDisconnected`被调用了），再来析构`Listener`对象。

#### 修正方法:  automatic memory management

参见下面的`Automatic memory management`段



### 在constructor中register callback/listener/observer

这个例子体现的是: Access uninitialized object错误。

#### 问题分析

一个zookeeper HA library(event driven library)，它提供了`IARBEventCallBack` interface，user需要implement其中的interface。

我第一次的写法如下: 

```C++
class CMasterMonitor: public IARBEventCallBack
{
public:
	CMasterMonitor()

	{
		m_lpArb = lpArb;
		int nRet = m_lpArb->CreateArbitrate(this);
		if (nRet == 0)
		{
			LOG_INFO("创建仲裁对象成功");
		}
		else
		{
			LOG_ERROR("创建仲裁对象失败,nRet:{}", nRet);
			throw std::runtime_error("创建仲裁对象失败");
		}
	}
};

```

即在constructor中，调用`m_lpArb->CreateArbitrate(this)`。上述程序，在运行后，很快就core dump了。

原因如下:

由于在zookeeper HA library(event driven library)中，会有一个单独的thread调用`IARBEventCallBack` object的member method，而我是在constructor中，进行的register，这就可能导致，object还没有完成initialization，另外一个thread就开始access它了，这就是典型的access outside of object lifetime。

> NOTE:
>
> 如果在这个object的constructor中，使用`this`传入到另外一个library中，则这就导致了access outside of object lifetime；
>
> 在这个object的constructor的执行过程中，可能library中的thread就开始access它了，那么这就发生了access outside of object lifetime，这个问题，我之前碰到过，就是在是使用研发中心的仲裁插件的时候。



#### 修正方法: 完成construction后再register

```C++
class CMasterMonitor: public IARBEventCallBack
{
public:
	CMasterMonitor()

	{

	}
	bool Init(IArbServer *lpArb)
	{
		m_lpArb = lpArb;
		int nRet = m_lpArb->CreateArbitrate(this);
		if (nRet == 0)
		{
			LOG_INFO("创建仲裁对象成功");
		}
		else
		{
			LOG_ERROR("创建仲裁对象失败,nRet:{}", nRet);
			throw std::runtime_error("创建仲裁对象失败");
		}

		nRet = m_lpArb->Start();
	}
};

```



## Example: 在开发行情插件时遇到的一个错误

这个例子， 体现的是dangling pointer错误。

### 问题分析

在开发行情插件时遇到的一个错误，其中涉及到了multiple thread中，分析如下: 

原来的程序将对行情插件的重订阅线程、行情消息处理线程的停止放到了行情类的析构函数中，由于对析构函数的调用不受programmer的控制，这就可能出现: 重订阅线程、行情消息处理线程没有被及时地停止并且使用已经被析构的对象而导致segment fault；修改方法是：显示地调用线程停止函数将重订阅线程、行情消息处理线程停止，从而确保不会出现前面描述的情况

### 修正方法 Synchronization

不可依赖于未知的、无序的、不可靠的，而是要进行显式的、可靠的控制(take control)，这在对object的lifetime的控制中尤为重要:

process exit的 时候，对destructor的调用顺序是比较复杂的，并且当object非常多的时候，programmer压根就无法进行准确的把控，对它们之间的依赖关系是未知的，尤其是当使用multiple thread、`shared_ptr`等，就会使情况更加复杂。

过去我总是习惯于将一些资源的释放放到destructor中，正如前面所述，destructor有时也是不可靠的，因此在一些情况下，我们要尽可能地选择更加准确的、可靠的控制方式，比如在`OnStop`中显式地将资源释放，这样的程序是更加健壮的。

这个例子，其实与"Process exit and destructor"有关，关于这个话题，参见 "Destructor" 章节。





## How to resolve总结

### Automatic memory management

显然，从 [Inversion of control principle](https://en.wikipedia.org/wiki/Inversion_of_control) 的角度来思考的话，解决思路是: 当没有thread使用这个pointer的时候，由 [framework](https://en.wikipedia.org/wiki/Software_framework)来将它释放。

虽然C++没有提供GC，但是C++ 11`std::shard_ptr`就正好适用于这种场景，它使用reference counting的技术，来保证pointee的自动释放，从而避免了上诉问题；



> draft: 这是一个非常重要的主题，在control theory中已经涉及这个topic了，只是没有提供具体的实现案例

#### Example: spdlog `async_logger`

参见 `Spdlog\Asynchronous-logging` 章节。

#### nextptr [Using weak_ptr for circular references](https://www.nextptr.com/tutorial/ta1382183122/using-weak_ptr-for-circular-references) 

One of the biggest concerns dealing with the raw pointers is that sometimes it is hard to ensure that a raw pointer is not dangling or valid. **Automatic memory management** by *shared_ptr* leads to a safer and easier to maintain code. As long as a component or function holds a *`shared_ptr`*, the object managed by the *`shared_ptr`* stays in memory.

> NOTE: 
>
> 1、这篇文章总结的非常好

#### Memory reclamation

参见

### Synchronization

1、体现了"Take control and make it computational and ordering"思想。

2、它需要programmer进行显式的控制，比如添加`release`、`close`接口，并且在这些接口中阻塞直至完成。



### 对比: explicit control VS implicit/automatic control

1、使用类似于reference counting的automatic memory management所实现的control，是一种隐式的control(implicit control)、自动的control，这种control方式是更加高级的，在很多情况下，我们都是 需要这种更加高级的control方式的，因为它使得用户不用关注、调用很多的接口。

2、关于此的典型例子就是spdlog asynchronous logging，对于像spdlog这样的library，让programmer显式地调用类似于`close`、`stop`这样的函数是不可接受的，那么在这种情况下，就需要使用atomic、implicit control。

spdlog使用`std::shared_ptr`来实现的automatic control。
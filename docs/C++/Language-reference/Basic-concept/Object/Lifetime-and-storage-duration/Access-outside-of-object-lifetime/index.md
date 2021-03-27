# Access outside of lifetime

1、要在object lifetime内使用它，如果不遵循这个原则，在object lifetime外使用它，就会导致memory error，因为C++ object对应的是memory，关于 memory error，参见`Resource-management\Memory-management\Memory-error`章节。

2、我将这类错误统一称为"Access outside of lifetime"，这是一种常见的memory error，它和 object lifetime相关的。它的另外一个说法是: "dangling"，比如dangling pointer、dangling reference。

3、以"Access outside of object lifetime"这个角度出发，可以组织很多内容。



## 原因

### Indirection

C++支持iterator、pointer、reference等indirection，pointee object(被执行的object)的释放，pointer的如果仍然去访问，则就导致了access out of object lifetime的发生，它对应的memory error中的"Dangling-and-wild-pointer"，下面对上述几种example中的原因进行总结。



### 由于out of order导致 access outside of object lifetime 

关于C++ object lifetime，参见如下章节:

1、`Object\Lifetime-and-storage-duration`

2、`Classes\Object-lifetime`

3、`Object\Object-operation`

通过其中的内容可知，C++ object lifetime是按照ordering发生的。很多导致"Access outside of lifetime"的情况，其实都是由于out of order而导致的

典型的例子包括: 

1、object还没有构造好

2、object已经被释放了，仍然去访问



#### How to solve?

对于这种由于out of order而导致的access outside of object lifetime，我的解决思路自然而然就是: take control，make it computational and ordering。

### TODO: local object

local object也是比较如意导致access outside of object lifetime的。

## cppreference [Lifetime](https://en.cppreference.com/w/cpp/language/lifetime) # [Access outside of lifetime](https://en.cppreference.com/w/cpp/language/lifetime#Access_outside_of_lifetime)

> NOTE: 这是一种常见的error，这种error是和lifetime相关的

Before the lifetime of an object has started but after the storage which the object will occupy has been allocated

> NOTE: "after the storage which the object will occupy has been allocated"的意思是: storage已经被分配了; "before the lifetime of an object has started"说明: initialization还没有完成

or,

after the lifetime of an object has ended and before the storage which the object occupied is reused or released, 

> NOTE: "before the storage which the object occupied is reused or released"说明: storage还没有给回收; "after the lifetime of an object has ended"说明: deinitialization已经完成了

the following uses of the glvalue expression that identifies that object are undefined:

> NOTE: 需要补充

During construction and destruction, other restrictions apply, see [virtual function calls during construction and destruction](https://en.cppreference.com/w/cpp/language/virtual#During_construction_and_destruction).

> NOTE: 
>
> 1、与此相关的一个idiom是: `OOP\Calling-Virtual-During-Initialization`
>
> 2、[dangling references](https://en.cppreference.com/w/cpp/language/reference#Dangling_references) 也是一种access outside of lifetime error，参见`C++\Language-reference\Reference\Reference`章节。



## Example: 在开发行情插件时遇到的一个错误

### 问题分析

在开发行情插件时遇到的一个错误，其中涉及到了multiple thread中，分析如下: 

原来的程序将对行情插件的重订阅线程、行情消息处理线程的停止放到了行情 类的析构函数中，由于对析构函数的调用不受programmer的控制，这就可能出现: 重订阅线程、行情消息处理线程没有被及时地停止并且使用已经被析构的对象而导致segment fault；修改方法是：显示地调用线程停止函数将重订阅线程、行情消息处理线程停止，从而确保不会出现前面描述的情况

### 经验总结: take control and make it computational and ordering

不可依赖于未知的、无序的、不可靠的，而是要进行显式的、可靠的控制(take control)，这在对object的lifetime的控制中尤为重要:

process exit的 时候，对destructor的调用顺序是比较复杂的，并且当object非常多的时候，programmer压根就无法进行准确的把控，对它们之间的依赖关系是未知的，尤其是当使用multiple thread、`shared_ptr`等，就会使情况更加复杂。

过去我总是习惯于将一些资源的释放放到destructor中，正如前面所述，destructor有时也是不可靠的，因此在一些情况下，我们要尽可能地选择更加准确的、可靠的控制方式，比如在`OnStop`中显式地将资源释放，这样的程序是更加健壮的。

这个例子，其实与"Process exit and destructor"有关，关于这个话题，参见 "Destructor" 章节。



## Example: in observer pattern

在object pattern中，是非常容易出现access outside of object lifetime的。

### Asynchronous event导致调用dangling pointer 

在TCP SDK（网络库，可以认为是一个[Event-driven architecture](https://en.wikipedia.org/wiki/Event-driven_architecture)），一般采 [Observer pattern](https://en.wikipedia.org/wiki/Observer_pattern) ；这种模式的一个特点就是: 将**Listener、observer对象**传入到TCP SDK（网络库），一般是通过pointer、reference的方式进行注册；TCP SDK会network的各种event通过回调**Listener对象**的成员方法通知到应用层，比如将**连接断开**的事件通过调用**Listener对象**的`OnDisconnected`通知到应用层。下面描述了在这种模式下非常容易出现的一个问题:

如果TCP SDK的网络断开是**异步**的，并且不对**Listener对象**的lifetime的进行控制；那么就存在这样的一种可能: **Listener对象**已经被释放了，而TCP SDK并不知晓，依然调用`OnDisconnected`，则这样就发生了本节标题中描述的dangling pointer错误，导致程序core dump。

#### 修正方法: asynchronous to synchronization

> NOTE: 其实这也是 "take control and make it computational and ordering"

对网络连接的断开、object的lifetime不进行**控制**而导致的问题，修正方法是：首先将网络连接关闭（使用阻塞的方法），待关闭完成后（`OnDisconnected`被调用了），再来析构`Listener`对象。

### 在constructor中register callback /listener/observer对象

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



## Example: Static Initialization Order Fiasco

参见`Static-Initialization-Order-Fiasco`章节。



## Example: iterator invalidation

1、iterator invalidate和object lifetime，Access outside  of lifetime of object 有关。

2、关于 Iterator invalidation，参见 `C++\Library\Standard-library\STL\Containers-library\Common-issue\Iterator-invalidation` 章节；

3、Pointer invalidation，参见:

a、stackoverflow [What exactly is invalidation of reference/pointer?](https://stackoverflow.com/questions/51748529/what-exactly-is-invalidation-of-reference-pointer)



## Example: realloc and move

dangling pointer and object lifetime and `vector<std::string>` and，realloc and move



## Example: Calling Virtual During Initialization

参见:

1、`Classes\Object-lifetime` 章节，其中对此进行了详细描述。

2、cppreference [virtual function specifier](https://en.cppreference.com/w/cpp/language/virtual) # During construction and destruction





## Example: access local object



源自: cppreference [Lifetime](https://en.cppreference.com/w/c/language/lifetime) 

```c


int* foo(void) {

   int a = 17; // a has automatic storage duration

​    return &a;

}  // lifetime of a ends

int main(void) {

​    int* p = foo(); // p points to an object past lifetime ("dangling pointer")

​    int n = *p; // undefined behavior

}

```





### APUE 11.4 Thread Creation

The typeless pointer passed to `pthread_create` and `pthread_exit` can be used to pass more than a single value. The pointer can be used to pass the address of a structure containing more complex information. Be careful that the memory used for the structure is still valid when the caller has completed. If the structure was allocated on the caller’s stack, for example, the memory contents might have changed by the time the structure is used. If a thread allocates a structure on its stack and passes a pointer to this structure to `pthread_exit`, then the stack might be destroyed and its memory reused for something else by the time the caller of `pthread_join` tries to use it.



## TODO



geeksforgeeks [Dangling, Void , Null and Wild Pointers](https://www.geeksforgeeks.org/dangling-void-null-wild-pointers/)


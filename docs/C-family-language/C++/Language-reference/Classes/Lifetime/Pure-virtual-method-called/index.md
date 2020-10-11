# 关于本章

本章讨论pure virtual method called异常，目前遇到了两种场景：

- 多线程race condition导致pure virtual method被调用了
- constructor、destructor中调用pure virtual method

## 多线程竞争导致pure virtual method called

在开发过程中偶现这个问题，大致场景如下：

使用一个网络库，这个网络库是基于callback模式的，该库需要developer创建继承自接口类`ICallback`的`Derived`类，并override其中的`OnConnect`、`OnReceive`、`OnClose`等方法；类`IConnection`表示网络连接，它的主要method：

- `Create(ICallback* Callback)` 创建连接对象
- `Send(const void* Data, int32_t Len)` 发送数据

对于每个network connection，需要调用其`Create`方法向其注册`ICallback`类对象。这样关于这个network connection对象的消息，底层网络库就通过回调`ICallback`对象的成员方法通知到用户；

`ICallback`是接口类，所以它将`OnConnect`、`OnReceive`、`OnClose`等方法都声明为pure virtual method；

这个网络库运行于一个独立的thread，用户线程调用`IConnection`的`Send`方法发送数据；网络库thread执行回调函数；

这种模式（多线程+不清晰的ownership of object），带来的一个问题是：对`Derived`类对象的lifetime的管理的麻烦；如果programmer删除了`Derived`类对象（显然，会调用destructor），而底层网络库又继续使用这个对象，显然这就会导致问题，由于存在数据竞争，所以这多种可能：

- `Derived`类对象被完全析构了，则导致dangling pointer
- `Derived`类对象被部分析构，则导致pure virtual method
- ......



解决方法是：需要等`OnClose`后（`OnClose`表示底层网络库不再使用该对象），object析构掉；

下面这篇文章中反映的情况大致和我遇到的场景类似；

### [GCC pure virtual method called](https://tombarta.wordpress.com/2008/07/10/gcc-pure-virtual-method-called/)

I spent about two hours today trying to debug a **race condition** in a multi-threaded `C++` app today… definitely not a fun thing to do. 



At one point, I saw this error pop up:

```
pure virtual method called
terminate called without an active exception
Aborted
```

What? I know I can’t instantiate a class that has any pure-virtual methods, so how did *this* error show up? To debug it, I decided to replace all of the potentially-erroneous pure virtuals with stub functions that printed warnings to `stderr`. Lo and behold, I confirmed that polymorphism *wasn’t working* in my application. I had a bunch of `Derived`s sitting in memory, and yet, the `Base` methods were being called.

Why was this happening? Because I was **deleting objects while they were still in use**. I don’t know if this is GCC-specific or not, but something very curious happens inside of destructors. Because the object hierarchy’s destructors get called from **most-derived** to **least-derived**, the object’s `vtable` switches up through parent classes. As a result, at some point in time (nondeterministic from a separate thread), my `Derived` objects were all really `Base`s. Calling a virtual member function on them in this mid-destruction state is what caused this situation.

> NOTE:  "**deleting objects while they were still in use**"，需要联系上下文来理解：作者描述的场景是在一个multiple thread中，一个thread，删除了另外一个thread正在使用的object；
>
> 上面这一段中的最后一句话揭示了根本原因所在：Calling a virtual member function on them in this mid-destruction state is what caused this situation.

Here’s about the simplest example I can think of that reproduces this situation:

```c++
#include <pthread.h>
#include <unistd.h>
struct base
{
    virtual ~base() { sleep(1); }
    virtual void func() = 0;
};
struct derived : public base
{
    virtual ~derived() { }
    virtual void func() { return; }
};
static void *thread_func(void* v)
{
    base *b = reinterpret_cast<base*>(v);
    while (true) b->func();
    return 0;
}
int main()
{
    pthread_t t;
    base *b = new derived();
    pthread_create(&t, 0, thread_func, b);
    delete b;
    return 0;
}
```



> NOTE: 编译: `g++ test.cpp -lpthread`，运行:
>
> ```c++
> pure virtual method called
> terminate called without an active exception
> 已放弃(吐核)
> ```
>
> 



So what’s the moral of the story? If you ever see the error message `pure virtual method called / terminate called without an active exception`, check your object lifetimes! You may be trying to call members on a destructing (and thus incomplete) object. Don’t waste as much time as I did.

### 如何修复？

- 使用virtual method来替换pure virtual method
- 避免race condition

## constructor、destructor中调用pure virtual method

### Examples

#### Constructor call pure virtual method

```c++
#include <iostream>

class BaseWithPureFunction
{
public:
	BaseWithPureFunction()
	{
		CallPureFunc();
		// 此处调用了纯虚拟函数，该虚拟函数由派生类
		//实现，但此处派生类还未构造成功，会导致r6025
		//错误
	}
	virtual void PureFunc() = 0;


	void CallPureFunc()

	{
		 PureFunc();

	}
};

class  Derived: public  BaseWithPureFunction
{
public:

	virtual void PureFunc()

	{
		 std::cout<<"Derived::PureFunc"<<std::endl;;
	}
};

int main()
{
	Derived d;
}

```

编译: `g++ test.cpp`，输出如下：

```
pure virtual method called
terminate called without an active exception
已放弃(吐核)
```



#### Destructor call pure virtual method

```c++
#include <iostream>

class BaseClassWithDestructorCallPureFun
{
public:
	virtual void PureFunc() = 0;

	void CallPureFunc()
	{
		PureFunc();
	}

	virtual ~BaseClassWithDestructorCallPureFun()
	{
		std::cout << "~BaseClassWithDestructorCallPureFun destructor call" << std::endl;

		CallPureFunc();
	}
};

class Derived: public BaseClassWithDestructorCallPureFun
{
public:
	virtual void PureFunc()
	{
		std::cout << "PureFunc Call From Derived" << std::endl;
	}

	virtual ~Derived()
	{
		std::cout << "~Derived destructor call" << std::endl;
	}
};

void Test()
{
	Derived d;
}
int main()
{
	Test();
}

```

编译: `g++ test.cpp`，输出如下：

```c++
~Derived destructor call
~BaseClassWithDestructorCallPureFun destructor call
pure virtual method called
terminate called without an active exception
已放弃(吐核)
```



### Analysis

关于此的分析，在[Pure-Virtual-Function-Called](./Pure-Virtual-Function-Called.md)中进行描述。



### 如何修复？
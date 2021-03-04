# Initialization and deinitialization order 

本章探讨constructor、destructor的调用次序问题(order)，原则如下:

1、constructor的调用次序: 沿着class hierarchy，自顶向下、自左至右进行初始化

2、destructor的调用是和constructor的相反的，这样是为了保证stack ordering

## cppreference [Constructors and member initializer lists](https://en.cppreference.com/w/cpp/language/initializer_list) # [Initialization order](https://en.cppreference.com/w/cpp/language/constructor#Initialization_order)

> NOTE: 
>
> 一、"actual order of initialization"是由
>
> 1、class hierarchy
>
> 2、"non-static data member are initialized in order of declaration in the class definition"
>
> 
>
> 在下面解释了这样做的原因:
>
> 1、这样的order是compiler能够control的
>
> 2、保证initialization 和 deinitialization是stack ordering

The order of **member initializers** in the list is irrelevant: the actual order of initialization is as follows:

1) If the constructor is for the **most-derived class**, **virtual bases** are initialized in the order in which they appear in depth-first left-to-right traversal of the base class declarations (left-to-right refers to the appearance in **base-specifier lists**)

> NOTE: 
>
> 1、"depth-first"的含义是什么？是否意味着 越"derived"，则越先被initialization

2) Then, **direct bases** are initialized in left-to-right order as they appear in this class's **base-specifier list**

3) Then, non-static data member are initialized in order of declaration in the class definition.

4) Finally, the body of the constructor is executed

(Note: if initialization order was controlled by the appearance in the member initializer lists of different constructors, then the [destructor](https://en.cppreference.com/w/cpp/language/destructor) wouldn't be able to ensure that the order of destruction is the reverse of the order of construction)



## cppreference [Destructors](https://en.cppreference.com/w/cpp/language/destructor) # [Destruction sequence](https://en.cppreference.com/w/cpp/language/destructor#Destruction_sequence)

For both user-defined or implicitly-defined destructors, after the body of the destructor is executed, the compiler calls the destructors for all non-static non-variant members of the class, in reverse order of declaration, then it calls the destructors of all direct non-virtual base classes in [reverse order of construction](https://en.cppreference.com/w/cpp/language/initializer_list#Initialization_order) (which in turn call the destructors of their members and their base classes, etc), and then, if this object is of most-derived class, it calls the destructors of all virtual bases.

Even when the destructor is called directly (e.g. `obj.~Foo();`), the return statement in `~Foo()` does not return control to the caller immediately: it calls all those member and base destructors first.







## stackoverflow [Constructor initialization-list evaluation order](https://stackoverflow.com/questions/1242830/constructor-initialization-list-evaluation-order)





## 我踩过的坑

今天在碰到了一个与initialization-list evaluation order相关的问题，并且它还导致了进程core掉了，现在想来，`c++`的这种设计太容易出现错误了，并且这种错误是非常严重但是隐晦的。

```c++
class Turn {
public:
	virtual ~Turn() {
	}
	Turn(const unsigned int interval, StockReader* stock_reader_p,
			const std::string& redis_address) :
			interval_(interval), stock_reader_p_(stock_reader_p), redis_address_(
					redis_address), redis_con_(redis_address_), AI_service_(
					redis_address_) {
		this_turn_left_time_ = 0;
		init_flag_ = false;
		time_number_ = 0;
		turn_number_ = 0;
	}
private:
	bool init_flag_;	//初始化标志
	unsigned int turn_number_;	//轮编号
	unsigned int time_number_;    //次编号
	unsigned int interval_;    //每次间隔，单位ms
	unsigned int this_turn_left_time_;    //本轮剩余时间，单位ms
	std::chrono::steady_clock::time_point start_time_; //main_loop的开始时间点
	CppCelery::Celery AI_service_; //
	StockReader* stock_reader_p_; //
	std::string redis_address_;
	RedisCon redis_con_; //redis连接
	RedisIO::AIServerQueue<> AI_server_queue_; //
};
```

```c++
template<typename TransportT>
class Producer {
public:
	/// @param broker_address
	Producer(const std::string& broker_address) :
			transport_(broker_address) {

	}
	/// 发布信息
	void publish(const std::string &message) {
		celery_queue_.write(transport_, message);
	}
private:
	std::string broker_address_;
	TransportT transport_;
	RedisIO::CeleryQueue<TransportT> celery_queue_;
};
```



上述代码能够编译通过，但是一旦运行起来，就会core掉，core的信息如下：

```
#0  0x00007fdbc35ad1cc in std::string::assign(std::string const&) () from /usr/anaconda3/lib/libstdc++.so.6
#1  0x00007fdbb733a235 in operator= (__str=..., this=0xc2a9348) at /usr/include/c++/4.8.2/bits/basic_string.h:547
#2  RedisCon::RedisCon (this=0xc2a9328, redis_address=...) at ../core/redis_method.cpp:22
#3  0x00007fdbb7303ff7 in Producer (broker_address=..., this=0xc2a9320) at ../core/celery/kombu/messaging.h:19
#4  Celery (broker_address=..., this=0xc2a9308) at ../core/celery/celery.h:27
#5  Turn (redis_address=..., stock_reader_p=0xc232400, interval=200, this=0xc2a92e0) at turn.h:34
#6  CQuoteOrderWorker::CQuoteOrderWorker (this=0xc2a8e90, quote_thread_num=<optimized out>, redis_address=..., stock_reader_p=<optimized out>, 
    quote_interval=<optimized out>) at quote_predict_worker_quote.cpp:67
#7  0x00007fdbb7316624 in CQuotePreImpl::init_thread_pool (this=this@entry=0xbf1ba50) at quote_predict_server.cpp:471
#8  0x00007fdbb7319514 in CQuotePreImpl::OnInit (this=0xbf1ba50) at quote_predict_server.cpp:557
#9  0x00007fdbb7319c0d in QuotePreInit (lpInstance=<optimized out>, pfOnSetTimeOut=<optimized out>) at quote_predict_server.cpp:60
#10 0x00007fdbc2c63680 in CF2CoreImpl::mf_InitPlugin (this=0x209ed70, lpInfo=0xbf1bc40, pfOnSetTimeOut=0x0) at f2core_impl.cpp:790
#11 0x00007fdbc2c61ad8 in CF2CoreImpl::Load (this=0x209ed70, iLoadLevel=2, pfOnSetTimeOut=0x0) at f2core_impl.cpp:376
#12 0x000000000040560f in CShell::init(int, char**) ()
#13 0x000000000040802d in main ()

```

在排查这个问题的时候，我感觉`redis_con_(redis_address_), AI_service_(redis_address_) `这种写法，类似于使用了未初始化的类成员变量，所以我尝试将上述`redis_address_`修改为由用户传入的`redis_address`，然后再次进行编译，运行，发现这种情况下，进程是不会core掉的；说明我的猜测是正确的，所以我就Google了相关的内容， 发现在类`Turn`中，`redis_address_`的确是声明在依赖它的两个成员变量之后的。在深入思考这个问题的时候，我想到了如下问题：

- 是先执行initializer list还是先执行 constructor body？
- 在执行initializer list的时候，`redis_address_`的值是什么？

- 为什么什么的代码会core掉？

这些问题既涉及到`c++`也设计到core dump，为此需要专门去了解相关内容。

## TODO



1、stackoverflow [Order of member constructor and destructor calls  ](https://stackoverflow.com/questions/2254263/order-of-member-constructor-and-destructor-calls  )


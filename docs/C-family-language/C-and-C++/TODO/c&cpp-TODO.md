[TOC]

# Reference in `c++`

https://en.cppreference.com/w/cpp/language/reference


# how to avoid infinit loop in the  loop of c++ container

QuotePredict.predict是采用的while来loop container，这种写法的一个弊端就是非常任意出现dead loop，所以这种情况是一定要谨慎的，必须要各种loop机制进行对比，找出最最合适的实现


# c++ constructor

## calling the superclass constructor

https://stackoverflow.com/questions/120876/what-are-the-rules-for-calling-the-superclass-constructor

## 调用类中的另外一个constructor

```
#include <iostream>
#include <string>

class SuperClass {
public:
       SuperClass(){}

	SuperClass(int foo) {
		foo_ = foo;
	}
	void out() {
		std::cout << foo_ << std::endl;
	}
private:
	int foo_;

};

class SubClass: public SuperClass {
public:
	SubClass(int foo) {

	}

	SubClass(int foo, int bar) {
		SubClass(foo);
	}

};

int main() {
	SubClass s = SubClass(3);
	SubClass ss = SubClass(3, 4);
	s.out();
        ss.out();
}

```

上述代码编译报错
```
test.cpp: 在构造函数‘SubClass::SubClass(int, int)’中:
test.cpp:26:15: 错误：‘SubClass foo’的声明隐藏了一个形参
   SubClass(foo);
               ^
test.cpp:26:15: 错误：对‘SubClass::SubClass()’的调用没有匹配的函数
```


###  [Can I call a constructor from another constructor (do constructor chaining) in C++?](https://stackoverflow.com/questions/308276/can-i-call-a-constructor-from-another-constructor-do-constructor-chaining-in-c)

修改为如下可用通过：
```
#include <iostream>
#include <string>

class SuperClass {
public:
       SuperClass(){}

	SuperClass(int foo) {
		foo_ = foo;
	}
	void out() {
		std::cout << foo_ << std::endl;
	}
private:
	int foo_;

};

class SubClass: public SuperClass {
public:
	SubClass(int foo) {

	}

	SubClass(int foo, int bar):SubClass(foo) {
	}

};

int main() {
	SubClass s = SubClass(3);
	SubClass ss = SubClass(3, 4);
	s.out();
        ss.out();
}
```


***SUMMARY*** : 对比下来，`c++`中调用parent class的constructor远没有Java和python中便利；并且`c++`中，调用本类的其它的constructor也是比较不便利的；

# Do I need to explicitly call the base virtual destructor?

https://stackoverflow.com/questions/677620/do-i-need-to-explicitly-call-the-base-virtual-destructor

# How to convert string to char array in C++?
https://stackoverflow.com/questions/13294067/how-to-convert-string-to-char-array-in-c


# Generate random numbers uniformly over an entire range

https://stackoverflow.com/questions/288739/generate-random-numbers-uniformly-over-an-entire-range

https://stackoverflow.com/questions/7560114/random-number-c-in-some-range

https://stackoverflow.com/questions/13445688/how-to-generate-a-random-number-in-c

# string to datetime and datetime diff

https://www.ibm.com/support/knowledgecenter/en/ssw_ibm_i_71/rtref/strpti.htm

https://en.cppreference.com/w/cpp/chrono/c/difftime

https://stackoverflow.com/questions/35749443/difference-between-two-datetime-in-c

https://en.cppreference.com/w/cpp/chrono/c/mktime


https://en.cppreference.com/w/cpp/chrono/c/tm


https://stackoverflow.com/questions/4781852/how-to-convert-a-string-to-datetime-in-c


# the order of declaration of the members in c++ and the order of destruction

[Order of member constructor and destructor calls  ](https://stackoverflow.com/questions/2254263/order-of-member-constructor-and-destructor-calls  )


# Movable but Non-Copyable Types

在阅读redis client [redis-plus-plus](https://github.com/sewenew/redis-plus-plus#connection) 的时候，其中介绍到：

> Redis class is movable but NOT copyable.

这让我想起来之前在阅读cppreference中[thread](https://en.cppreference.com/w/cpp/thread/thread)类的文档的时候，其中也有类似的描述

> No two `std::thread` objects may represent the same thread of execution; `std::thread` is not [*CopyConstructible*](https://en.cppreference.com/w/cpp/named_req/CopyConstructible) or [*CopyAssignable*](https://en.cppreference.com/w/cpp/named_req/CopyAssignable), although it is [*MoveConstructible*](https://en.cppreference.com/w/cpp/named_req/MoveConstructible) and [*MoveAssignable*](https://en.cppreference.com/w/cpp/named_req/MoveAssignable).



现在想来，其实movable 不填 non-copyable是很多class的一个特性，有必要总结一下：

- [Movable but Non-Copyable Types](http://cs.swan.ac.uk/~csoliver/ok-sat-library/internet_html/doc/doc/Boost/1_53_0/doc/html/move/movable_only_classes.html)
- google movable but NOT copyable

[std::mutex](https://en.cppreference.com/w/cpp/thread/mutex) is neither copyable nor movable.

# Template parameters

**Mutex** - the type of the mutex to lock. The type must meet the [*BasicLockable*](https://en.cppreference.com/w/cpp/named_req/BasicLockable) requirements

***THINKING*** : 如果不满足的话，compiler是否会报错呢？也就是它是否有static checking    


# Futures for C++11 at Facebook

https://code.fb.com/developer-tools/futures-for-c-11-at-facebook/


# when to use this in `c++`



# `const` 

```c++
std::priority_queue<TimedTask, std::vector<TimedTask>, TimedTaskLess> task_queue_;

task_queue_.top().begin();

bool begin() const{
}
```

如果`begin`不添加`const`，则会编译报错；

# when invoke constructor 

```c++
TimedTask task = std::move(task_queue_.top());

// not CopyAssignable
TimedTask& operator=(TimedTask const& e) = delete;
```

```
../../core/timer.cpp: 在成员函数‘void CppTimer::Timer::timer()’中:
../../core/timer.cpp:67:51: 错误：使用了被删除的函数‘CppTimer::TimedTask::TimedTask(const CppTimer::TimedTask&)’
       TimedTask task = std::move(task_queue_.top());
```

改成了这种写法还是会编译报错

```c++
../../core/timer.cpp:67:50: 错误：使用了被删除的函数‘CppTimer::TimedTask::TimedTask(const CppTimer::TimedTask&)’
       TimedTask task(std::move(task_queue_.top()));

```

改成这种写法也会报错：

```c++
../../core/timer.cpp: 在成员函数‘void CppTimer::Timer::timer()’中:
../../core/timer.cpp:68:41: 错误：将类型为‘CppTimer::TimedTask&’的引用初始化为类型为‘const value_type {aka const CppTimer::TimedTask}’的表达式无效
       TimedTask& task = task_queue_.top();
```



```c++
TimedTask task;
```

```
../../core/timer.cpp:95:13: 错误：对‘CppTimer::TimedTask::TimedTask()’的调用没有匹配的函数
   TimedTask task;

```



如果`TimedTask`没有copy constructor，则下面的代码也会编译报错：

```c++
void Timer::add_expire_task(TimedTask && task) {
	if (stop_expire_task_)
		throw std::runtime_error("task执行线程已经被停止了，不要再添加task");
	{
		ScopedLockT lock(expire_task_mutex_);
		expire_task_queue_.push(task);
	}
	expire_task_cv_.notify_one(); // notify callback_thread_ to task
}
```

```c++
../../core/timer.cpp:127:31:   required from here
/usr/include/c++/4.8.2/ext/new_allocator.h:120:4: 错误：使用了被删除的函数‘CppTimer::TimedTask::TimedTask(const CppTimer::TimedTask&)’
  { ::new((void *)__p) _Up(std::forward<_Args>(__args)...); }

```

最后修改为如下方式可以编译通过：

```c++
void Timer::add_expire_task(TimedTask && task) {
	if (stop_expire_task_)
		throw std::runtime_error("task执行线程已经被停止了，不要再添加task");
	{
		ScopedLockT lock(expire_task_mutex_);
		expire_task_queue_.push(std::move(task));
	}
	expire_task_cv_.notify_one(); // notify callback_thread_ to task
}
```

https://ariya.io/2015/01/c-class-and-preventing-object-copy

https://crascit.com/2016/08/15/avoiding-copies-and-moves-with-auto/





```c++
/// redis中使用list来保存行情数据，按照列来进行保存，参见"redis行情数据数据结构.md"
static const std::map<std::string, std::string> QuoteFieldToSimpleQuote = { {
		Price, LastPrice }, { AccVolume, Amount }, { AccTurover, Volum } };

for (const std::pair<std::string, std::string>& element : RedisQuoteKeys::QuoteFieldToSimpleQuote) {
			value = simple_quote_unpacker_p->GetDouble(element.second.c_str());
			latest_quote_[element.first] = value;
		}
```



```
In file included from fsc_quote_predict.h:16:0,
                 from fsc_quote_predict.cpp:8:
../core2/data_model/quote_predict_stock.h: 在成员函数‘void DataModel::QuotePredictStock::save_latest_quote(std::string, IF2Packer*)’中:
../core2/data_model/quote_predict_stock.h:94:71: 错误：将类型为‘std::pair<std::basic_string<char>, std::basic_string<char> >&’的引用初始化为类型为‘const std::pair<const std::basic_string<char>, std::basic_string<char> >’的表达式无效
   for (std::pair<std::string, std::string>& element : RedisQuoteKeys::QuoteFieldToBidOffer) {
```

最终 修改为如下方式可以通过：

```c++
for (const std::pair<std::string, std::string> element : RedisQuoteKeys::QuoteFieldToSimpleQuote) {
			value = simple_quote_unpacker_p->GetDouble(element.second.c_str());
			latest_quote_[element.first] = value;
		}
```

# move, copy and thire ownership

`c++`中，copy意味着share ownership，而move则不是share，而是transfer，被move的object是不能够再被使用的，比如说，move了一个function object，那么如何再次调用这个function object，则会抛出[`std::bad_function_call`](https://en.cppreference.com/w/cpp/utility/functional/bad_function_call)异常；显然，`move`对应这**unique ownership**

在我的timer application中，有两个thread：
- timer thread
- expire task thread

我第一次是使用的move来在将timer thread所的queue中所保存的task transfer到expire task thread的queue中，写法如下：
```
/// https://stackoverflow.com/a/20149745
TimedTask task = std::move(const_cast<TimedTask&>(task_queue_.top()));
task_queue_.pop();
add_expire_task(std::move(task));
if (task.is_periodic()) { // 如果是periodic，则需要再次加入到队列中
	task.once_more();
	task_queue_.push(std::move(task)); // 此时，time_thread_依然lock住了time_mutex_，所以此处是能够安全地添加task的
}
```
上面这段代码就是典型的在将一个task move出去后又再次使用的情况；timer thread在第一次执行上述代码的时候，显然其将task move到了expire task thread的队列中，此时添加到expire task thread中的task是一个正常的task；如果这个task是一个periodic，那么periodic将会将这个moved task再次添加到其队列中，显然这个moved task已经不是一个正常的task了，它的handler已经是`nullptr`，所以是不能够被调用的，当这个task expire后，就会被添加到expire task thread的queue中，然后expire task queue将执行这个task，由于这个task的handler是一个function object，但是这个function object的target已经被move out了，所以就抛出了[`bad_function_call`](https://en.cppreference.com/w/cpp/utility/functional/bad_function_call)异常

# function object and function pointer

我觉得function object应该包含一个指向对应function的pointer；

# `size_t`以及为什么我们要使用它

`size_t`与`int`的对比

# pandas c++ 

实现中的关键：
- 如何实现数据的高效存储与修改

在这一点上，redis是非常好的典范

- 如何实现数据的分析计算





## quantstack

https://quantstack.net/index.html   

https://github.com/QuantStack

# constructor in cpp


```
#include<iostream>
#include<map>

class A{
public:
A(int a){
a_=a;
std::cout<<a_<<std::endl;
}

A(){
a_=0;
}

A(const A& other){
a_=other.a_;
std::cout<<"copy constructor"<<std::endl;
}

A(A&& other){
a_=other.a_;
std::cout<<"move copy constructor"<<std::endl;
}

private:
int a_;
};

int main(){
std::map<int, A> m;
m.insert(std::make_pair<int, A>(1, A(1)));
return 0;
}
```
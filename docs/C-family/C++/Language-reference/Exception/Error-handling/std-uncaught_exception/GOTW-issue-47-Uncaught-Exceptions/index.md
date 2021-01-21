# [GotW #47 Uncaught Exceptions](http://www.gotw.ca/gotw/047.htm)

> NOTE: 这篇文章其实在说明: 在class `A`的destructor中，使用 `bool uncaught_exception() noexcept;` 来判断在object of `A`的lifetime中是否有exception被抛出是无法实现的，根本原因在于:  `bool uncaught_exception() noexcept;` 仅仅返回一个状态值，而这个状态可能在object of `A`之前就已经变更了；那么就可能存在这样的情况: 另外一个class `B`的destructor中，使用了object of `A`，而object of `B`由于exception stack unwinding，当它的destructor被执行的时候， `bool uncaught_exception() noexcept;`  返回为true，显然此时的exception状态在object of `A`之前就已经变更了。
>
> 总的来说: 使用  `bool uncaught_exception() noexcept;`  来查询exception状态是可以的，但是用它来判断exception状态变化(是否新抛出了exception)是不可靠的，尤其是在一个exception stack unwinding中使用的时候。在C++17中，提出的`int uncaught_exceptions() noexcept;`可以用于准确地、可靠地判断exception状态变化(是否新抛出了exception)。
>
> 本文中作者频繁的使用了"moral"这个词语，它表明的意思是: "道德的"，引申的含义是: 设计理念上的。"immoral"被翻译为不符合最佳设计理念的、不符合最佳设计规范的。
>
> 作者的行文思路是: 在"Problem"节中提出三个问题，然后在"Solution"章节中给出回答。

What is the standard function `uncaught_exception()`, and when should it be used? The answer given here isn't one that most people would expect.

## Problem

JG Question

### **1.** What does `std::uncaught_exception()` do?

Guru Questions

### **2.** Consider the following code:

```C++
T::~T() {
    if( !std::uncaught_exception() ) {
        // ... code that could throw ...
    } else {
        // ... code that won't throw ...
    }
}
```

> NOTE: 上诉code所做的是: 如果"there is no exception currently active"，那么就可以throw exception；否则不应该throw exception。在下面"Solution"章节中驳斥了这种用法。

Is this a good technique? Present arguments for and against.

### **3.** Is there any other good use for `uncaught_exception`? 

Discuss and draw conclusions.

## Solution

### **1.** What does `std::uncaught_exception()` do?

It provides a way of knowing whether there is an exception currently active. (Note that this is not the same thing as knowing whether it is safe to throw an exception.)

To quote directly from the standard (15.5.3/1):

> *The function `bool uncaught_exception()` returns true after completing evaluation of the object to be thrown until completing the initialization of the exception-declaration in the matching handler (_lib.uncaught_). This includes stack unwinding. If the exception is rethrown (_except.throw_), `uncaught_exception()` returns true from the point of rethrow until the rethrown exception is caught again.*

As it turns out, this specification is deceptively(欺诈地、看似) close to being useful.

> NOTE: 这段话的表明意思是: "事实证明，这个规范看似可能是有用的"；直白的意思是:  `bool uncaught_exception() noexcept;`是无用的

### **2.** Consider the following code:

```C++
T::~T() {
    if( !std::uncaught_exception() ) {
        // ... code that could throw ...
    } else {
        // ... code that won't throw ...
    }
}
```

Is this a good technique? Present arguments for and against.

In short: No, even though it attempts to solve a problem. There are technical grounds(理由) why it shouldn't be used (i.e., it doesn't always work), but I'm much more interested in arguing against this idiom on **moral grounds**.

> NOTE: 上面这一段是作者给出的最终结论，下面是作者的分析过程，通过这段分析过程我们可以看出作者得出这个结论的原因。

#### Background: The Problem

> NOTE: 问题是: 在destructor中抛出了一个exception，一旦在destructor中抛出了exception，"bad thing can happen"(在下面所述的):
>
> "If a destructor throws an exception while another exception is already active (i.e., during **stack unwinding**), the program is terminated. This is usually not a good thing."
>
> 

If a destructor throws an exception, Bad Things can happen. Specifically, consider code like the following:

```C++
//  The problem
//
class X
{
public:
	~X()
	{
		throw 1;
	}
};

void f()
{
	X x;
	throw 2;
} // calls X::~X (which throws), then calls terminate()

```

If a destructor throws an exception while another exception is already active (i.e., during **stack unwinding**), the program is terminated. This is usually not a good thing.

> NOTE: 完整程序如下:
>
> ```C++
> //  The problem
> //
> class X
> {
> public:
> 	~X()
> 	{
> 		throw 1;
> 	}
> };
> 
> void f()
> {
> 	X x;
> 	throw 2;
> } // calls X::~X (which throws), then calls terminate()
> 
> int main()
> {
> 	f();
> }
> // g++ test.cpp
> 
> ```
>
> 运行结果如下:
>
> ```C++
> terminate called after throwing an instance of 'int'
> Aborted (core dumped)
> ```
>
> 



#### The Wrong Solution

> NOTE: 本段阐述描述解决上述问题的方法

"Aha," many people -- including many experts -- have said, "let's use `uncaught_exception()` to figure out whether we can throw or not!" And that's where the code in Question 2 comes from... it's an attempt to solve the illustrated problem:

```C++
//  The wrong solution
//
T::~T()
{
	if (!std::uncaught_exception())
	{
		// ... code that could throw ...
	}
	else
	{
		// ... code that won't throw ...
	}
}


```

The idea is that "we'll use the path that could throw as long as it's safe to throw." This philosophy is wrong on two counts: 

1、first, this code doesn't do that; 

2、second (and more importantly), the philosophy itself is in error.

> NOTE:  上面这段话第一句是: 上述程序的核心思路(idea) 是: 如果"it's safe to throw"(即`!std::uncaught_exception()`为true，即当前没有active exception)，那么可以throw，否则不要throw。
>
> 作者认为这种idea是错误的，作者给出了两个原因:
>
> 1、上述这种机制是无法实现的，在"The Wrong Solution: Why the Code Is Unsound"中进行了说明
>
> 2、这种idea本身是错误的，在"The Wrong Solution: Why the Approach Is Immoral"中进行了说明

#### The Wrong Solution: Why the Code Is Unsound

> NOTE: 解释，为什么上面的写法是不完善的

One problem is that the above code won't actually work as expected in some situations. Consider:

```C++
//  Why the wrong solution is wrong
//
U::~U() {
    try {
        T t;
        // do work
    } catch( ... ) {
        // clean up
    }
}
```

If a `U` object is destroyed due to stack unwinding during to exception propagation, `T::~T` will fail to use the "code that could throw" path even though it safely could.

> NOTE: 由于`T t`被包围在try...catch...中，因此在它的destructor中可以throw的，但是当`U::~U()`是由于exception stack unwinding而执行的，因此在`T::~T`中`std::uncaught_exception()`为false，因此它会执行"code that won't throw"。完整测试程序如下:
>
> ```C++
> #include <exception>
> #include <iostream>
> class T
> {
> public:
> 	//  The wrong solution
> 	//
> 	~T()
> 	{
> 		if (!std::uncaught_exception())
> 		{
> 			// ... code that could throw ...
> 			std::cout << __PRETTY_FUNCTION__ << "code that could throw" << std::endl;
> 		}
> 		else
> 		{
> 			// ... code that won't throw ...
> 			std::cout << __PRETTY_FUNCTION__ << "code that won't throw" << std::endl;
> 		}
> 	}
> 
> };
> 
> class U
> {
> public:
> 	//  Why the wrong solution is wrong
> 	//
> 	~U()
> 	{
> 		std::cout << __PRETTY_FUNCTION__ << std::endl;
> 		try
> 		{
> 			T t;
> 			// do work
> 		} catch (...)
> 		{
> 			// clean up
> 
> 		}
> 	}
> 
> };
> void foo()
> {
> 	U u;
> 	throw 1;
> }
> int main()
> {
> 	try
> 	{
> 		foo();
> 	} catch (...)
> 	{
> 
> 	}
> }
> // g++ test.cpp
> 
> ```
>
> 输出如下:
>
> ```C++
> U::~U()
> T::~T()code that won't throw
> ```
>
> 

Note that none of this is materially different from the following:

> NOTE: 这段话的表面意思是: "请注意，这些都与以下内容没有本质区别:"



```C++
//  Variant: Another wrong solution
//
Transaction::~Transaction()
{
	if (uncaught_exception())
	{
		RollBack();
	}
}

```

> NOTE: 上述transaction是典型的仿照two-phase protocol来写的:
>
> 1、在constructor中，try-phase
>
> 2、在destructor中
>
> ```pseudocode
> if 有异常:
> 	RollBack();
> else:
> 	Commit();
> ```
>
> 但是，正如下面的这段所描述的，上述code是存在非常严重的问题的，如果exception不是由constructor中抛出的，即try-phase是成功的，而是由外层抛出的，在destructor中， 还是会执行`RollBack();`，显然这将导致非常严重的错误。

Again, note that this doesn't do the right thing if a **transaction** is attempted in a **destructor** that might be called during **stack unwinding**:

```C++
//  Variant: Why the wrong solution is still wrong
//
U::~U()
{
	try
	{
		Transaction t( /*...*/);
		// do work
	} catch (...)
	{
		// clean up
	}
}

```

#### The Wrong Solution: Why the Approach Is Immoral

> NOTE: 这一段，作者从更高的角度(设计理念上)对上述写法进行了驳斥: 它是不符合最佳设计理念的，在最后一段，作者给出了这样做的严重后果

In my view, however, the "it doesn't work" problem isn't even the main issue here. My major problem with this solution is not technical, but moral(设计理念上的): It is poor design to give `T::~T()` two different semantics, for the simple reason that it is always poor design to allow an operation to report the same error in two different ways. Not only does it complicate the interface and the semantics, but it makes the caller's life harder because the caller must be able to handle both flavours of error reporting -- and this when far too many programmers don't check errors well in the first place!	

#### The Right Solution

The right answer to the problem is much simpler:

```C++
//  The right solution
//
T::~T() /* throw() 
```

> NOTE: 没有看懂上述代码所要表达的含义，它想表达的是"在destructor尽可能地不要throw"?还是要么throw，要么不non-throw？
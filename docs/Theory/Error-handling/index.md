# 关于本章

本章讨论错误处理。





## wikipedia [Exception handling](https://en.wikipedia.org/wiki/Exception_handling)



Alternative approaches to exception handling in software are error checking, which maintains normal program flow with later explicit checks for contingencies reported using special return values or some auxiliary global variable such as C's [errno](https://en.wikipedia.org/wiki/Errno) or floating point status flags; or input validation to preemptively filter exceptional cases.

> NOTE: 思考：exception机制相比于error checking的优势是什么？在下面的文章中点出来了。



## Exception VS errono

exception是现代programming language提供的error handling机制，传统的做法是errono，那exception有什么优势呢？

第一个优势: 让代码结构更加清晰

在wikipedia [Exception handling syntax](https://en.wikipedia.org/wiki/Exception_handling_syntax)中有着解释:

> **Exception handling syntax** is the set of keywords and/or structures provided by a computer [programming language](https://en.wikipedia.org/wiki/Programming_language) to allow [exception handling](https://en.wikipedia.org/wiki/Exception_handling), which separates the handling of errors that arise during a program's operation from its **ordinary processes**.

第二优势: 更加得expressive

在函数声明中，specify它可能跑出的exception，这种方式和C++中concept feature一样是formal description，这样的grammar让programmer以统一的方式来specify它可能跑出的exception，这样的方式能够让programmer只需要根据声明就知道函数的exception情况，显然相比于传统的使用`error_no`的方式，这样的方式是更加expressive的。

Java就采用的这种方式，下面是Java中的一些例子:

stackify [How to Specify and Handle Exceptions in Java](https://stackify.com/specify-handle-exceptions-java) : 

```C++
public void doSomething(String input) throws MyBusinessException {
	// do something useful ...
	// if it fails
	throw new MyBusinessException("A message that describes the error.");
}
```

curator.apache [Getting Started](http://curator.apache.org/getting-started.html) :

```java
LeaderSelectorListener listener = new LeaderSelectorListenerAdapter()
{
    public void takeLeadership(CuratorFramework client) throws Exception
    {
        // this callback will get called when you are the leader
        // do whatever leader work you need to and only exit
        // this method when you want to relinquish leadership
    }
}

LeaderSelector selector = new LeaderSelector(client, path, listener);
selector.autoRequeue();  // not required, but this is behavior that you will probably expect
selector.start();
```

C++11、C++17中，去除了这种用法，参见`C++\Language-reference\Exception`章节。



## 原则

一、program应该及时地报告错误，甚至及时终止程序(比如抛出exception)，而不是在错误的基础上继续运行

将这个简称为: report error on time not run on error。



1、于此相关的一个关于"undefined behavior"的一个说法

2、基于error运行的出现，它的一切行为都是不可预期的、不可控的；从我的经历来看，一些小错误，没有及时处理，而导致的非常诡异的、非常难以排查的问题:

a、之前碰到过由于unsigned integer的overflow而导致程序非常异常的行为

二、turn undefined behavior as error or exception 

对于一些 undefined behavior ，最后将它们转换为exception及时地抛出，下面是一些典型的undefined behavior: 

1、array bound check

2、integer overflow


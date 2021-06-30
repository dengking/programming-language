# [About Node.js®](https://nodejs.org/en/about/)

As an asynchronous event driven **JavaScript runtime**, Node is designed to build scalable network applications. In the following "hello world" example, many connections can be handled concurrently. Upon each connection the callback is fired, but if there is no work to be done, **Node** will sleep.

> 总结：显然在Node.js中Node有着特殊的含义，现在想来它的含义应该就是“节点”。既然Node.js是一个**JavaScript runtime**，则它就可以运行JavaScript代码，显然它就相当于一个JavaScript的解释器，能够解释执行JavaScript代码，就好比python解释器一般。目前的这篇文章主要从concurrency model的角度来描述node.js，我的一个好奇是，像Vue.js这样的框架为何能够基于node.js而构建呢？

```javascript
const http = require('http');

const hostname = '127.0.0.1';
const port = 3000;

const server = http.createServer((req, res) => {
  res.statusCode = 200;
  res.setHeader('Content-Type', 'text/plain');
  res.end('Hello World\n');
});

server.listen(port, hostname, () => {
  console.log(`Server running at http://${hostname}:${port}/`);
});
```

This is in contrast to today's more common **concurrency model** where **OS threads** are employed. Thread-based networking is relatively inefficient and very difficult to use. Furthermore, users of Node are free from worries of **dead-locking** the process, since there are no locks. Almost no function in Node directly performs **I/O**, so the process never blocks. Because nothing blocks, scalable systems are very reasonable to develop in Node.

If some of this language is unfamiliar, there is a full article on [Blocking vs Non-Blocking](https://nodejs.org/en/docs/guides/blocking-vs-non-blocking/).

---

Node is similar in design to, and influenced by, systems like Ruby's [Event Machine](https://github.com/eventmachine/eventmachine) or Python's [Twisted](http://twistedmatrix.com/). Node takes the **event model** a bit further. It presents an [event loop](https://nodejs.org/en/docs/guides/event-loop-timers-and-nexttick/) as a runtime construct instead of as a library. 

In other systems there is always a **blocking call** to start the **event-loop**( 在其他系统中，始终存在阻塞调用以启动事件循环). Typically behavior is defined through callbacks at the beginning of a script and at the end starts a server（启动一个服务以响应事件） through a blocking call like `EventMachine::run()`. 

In Node there is no such **start-the-event-loop call**. Node simply enters the event loop after executing the input script. Node exits the event loop when there are no more callbacks to perform. This behavior is like browser JavaScript — the event loop is hidden from the user.

> NOTE: 
>
> It presents an [event loop](https://nodejs.org/en/docs/guides/event-loop-timers-and-nexttick/) as a runtime construct instead of as a library.这句好要如何来进行理解？其中的runtime construct是运行时构建的意思。
>
> 总结：这段话概括的描述了node.js的event model。



HTTP is a **first class citizen** in Node, designed with **streaming** and **low latency** in mind. This makes Node well suited for the foundation of a web library or framework.

Just because Node is designed without threads, doesn't mean you cannot take advantage of multiple cores in your environment. Child processes can be spawned by using our [`child_process.fork()`](https://nodejs.org/api/child_process.html#child_process_child_process_fork_modulepath_args_options) API, and are designed to be easy to communicate with. Built upon that same interface is the [`cluster`](https://nodejs.org/api/cluster.html) module, which allows you to share sockets between processes to enable load balancing over your cores.




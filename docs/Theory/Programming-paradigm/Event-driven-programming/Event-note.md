# JavaScript event loop
https://developer.mozilla.org/en-US/docs/Web/JavaScript/EventLoop

这篇文章非常值得一读。

这是一种典型的client event loop

JavaScript就是典型的asynchronous，发送了请求就不管了，然后“当”查询好了后，再通知。


# event-driven programming and event loop
event loop是实现event driven programming的一种方式，当然，还有其他的方式

# event loop模型
有非常多的基于event loop模型的software
- redis
- node.js
- JavaScript
- libevent
- libuv


Concurrency model 


# network event
https://www.cnblogs.com/alsodzy/p/10206914.html


https://en.wikipedia.org/wiki/Netty_(software)


# 断线重连

https://stackoverflow.com/questions/37029341/how-to-reconnect-socket-using-threads

https://stackoverflow.com/questions/57323865/how-to-reconnect-to-tcp-server-after-network-goes-down-and-comes-back-up-again

## redis reconnect
https://stackoverflow.com/questions/10879426/how-to-reconnect-redis-connection

https://github.com/NodeRedis/node-redis/issues/1007


https://developer.ibm.com/depmodels/cloud/articles/error-detection-and-handling-with-redis/


一种解决方案是使用一个thread，每隔一段时间就取检查，另外一种实现方式是当出现错误的时候，才去重连。


# on
event driven programming的一个典型的模式就是：当某个事件发生的时候，就执行某个函数。我觉得这种模式，使用JavaScript来进行描述是最最便利的。

https://www.w3schools.com/js/js_events.asp

https://storm.cis.fordham.edu/~mesterharm/2350/event.html



# event and concurrency

同时发生的事件发生可能多，如何快速地处理这些事件呢？显然这就涉及了concurrency的问题，即并发地执行handler。



# event dispatch


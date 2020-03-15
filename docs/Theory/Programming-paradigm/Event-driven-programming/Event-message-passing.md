# Message/event passing

要实现event passing，是有多种方式可选的。


## Inter-process communication

### Message queue

参见[Message queue](https://en.wikipedia.org/wiki/Message_queue)


AMQP参见[Advanced Message Queuing Protocol](https://en.wikipedia.org/wiki/Advanced_Message_Queuing_Protocol)



## Inter-thread communication

### condition variable

参见[Monitor (synchronization)](https://en.wikipedia.org/wiki/Monitor_(synchronization))

在youdao notebook的《`cppreference-std-condition_variable.md`》中，对condition variable和event passing之间的对应关系进行了比较深刻的分析；



## [Messaging pattern](https://en.wikipedia.org/wiki/Messaging_pattern)



## [Publish–subscribe pattern](https://en.wikipedia.org/wiki/Publish%E2%80%93subscribe_pattern)

一个典型的例子就是redis的pub/sub

在这种情况下，pub需要注册回调函数，用于指定当收到信息时，需要执行的动作。这非常类似于signal handler。

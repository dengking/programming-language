要实现event passing，是有多种方式可选的

# event loop

参见[Event loop](https://en.wikipedia.org/wiki/Event_loop)


# Observer pattern

[Observer pattern](http://en.wikipedia.org/wiki/Observer_pattern)


# inter-process communication

## Message queue

参见[Message queue](https://en.wikipedia.org/wiki/Message_queue)


AMQP参见[Advanced Message Queuing Protocol](https://en.wikipedia.org/wiki/Advanced_Message_Queuing_Protocol)


# inter-thread communication

## condition variable

参见[Monitor (synchronization)](https://en.wikipedia.org/wiki/Monitor_(synchronization))

在youdao notebook的《`cppreference-std-condition_variable.md`》中，对condition variable和event passing之间的对应关系进行了比较深刻的分析；

# Proactor pattern

参见[Proactor pattern](https://en.wikipedia.org/wiki/Proactor_pattern)

# Reactor pattern

参见[Reactor pattern](https://en.wikipedia.org/wiki/Reactor_pattern)

# Actor model

参见[Actor model](https://en.wikipedia.org/wiki/Actor_model)



# [Messaging pattern](https://en.wikipedia.org/wiki/Messaging_pattern)



# [Publish–subscribe pattern](https://en.wikipedia.org/wiki/Publish%E2%80%93subscribe_pattern)

一个典型的例子就是redis的pub/sub

在这种情况下，pub需要注册回调函数，用于指定当收到信息时，需要执行的动作。这非常类似于signal handler。

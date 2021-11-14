# Streams

本质上是stream of event:

dart.dev [Asynchronous programming: Streams](https://dart.dev/tutorials/language/streams)

> A stream is a sequence of asynchronous events

events包括: 

1、data event

2、error event



## dart.dev [Asynchronous programming: Streams](https://dart.dev/tutorials/language/streams)

Asynchronous programming in Dart is characterized by the [Future](https://api.dart.dev/stable/dart-async/Future-class.html) and [Stream](https://api.dart.dev/stable/dart-async/Stream-class.html) classes.

A stream is a sequence of **asynchronous events**. It is like an **asynchronous Iterable**—where, instead of getting the next event when you ask for it, the stream tells you that there is an event when it is ready.

> NOTE: 
>
> 当event已经ready的时候，

### Receiving stream events

Streams can be created in many ways, which is a topic for another article, but they can all be used in the same way: the *asynchronous for loop* (commonly just called **await for**) iterates over the events of a stream like the **for loop** iterates over an [Iterable](https://api.dart.dev/stable/dart-core/Iterable-class.html). 

This code simply receives each event of a stream of integer events, adds them up, and returns (a future of) the sum. When the loop body ends, the function is paused until the next event arrives or the stream is done.

### Two kinds of streams



#### Single subscription streams

> NOTE: 
>
> 它的含义是: "单个订阅流"，只有一个订阅者

#### Broadcast streams

You can start listening to such a stream at any time, and you get the events that are fired while you listen. More than one listener can listen at the same time, and you can listen again later after canceling a previous subscription.



### The `listen()` method

The final method on Stream is `listen()`. This is a “low-level” method—all other stream functions are defined in terms of `listen()`.

## dart.dev [Creating streams in Dart](https://dart.dev/articles/libraries/creating-streams)



## api.dart.dev [`Stream<T>` class](https://api.dart.dev/stable/2.14.4/dart-async/Stream-class.html)



## api.dart.dev [`StreamController<T>` class](https://api.dart.dev/stable/2.14.4/dart-async/StreamController-class.html)



## Stream 何时终止？

dart.dev [Asynchronous programming: Streams](https://dart.dev/tutorials/language/streams) # [Error events](https://dart.dev/tutorials/language/streams#error-events)

> Streams are done when there are no more events in them, and the code receiving the events is notified of this just as it is notified that a new event arrives. When reading events using an **await for** loop, the loops stops when the stream is done.


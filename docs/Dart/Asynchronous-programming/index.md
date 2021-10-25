# Dart asynchronous programming

大多数programming language的asynchronous programming都使用了如下两种idiom:

一、callback

二、futures: 

1、futures, async, await

2、futures, callback

在dart中，采用的是"futures, async, await"；原文给出了非常好的介绍。



## dart guides [A tour of the Dart language](https://dart.dev/guides/language/language-tour) # [Asynchrony support](https://dart.dev/guides/language/language-tour#asynchrony-support)

> NOTE: 原文这一段对dart的asynchronous programming进行了非常好的概述

### Handling Futures

### Handling Streams

## dart codelabs [Asynchronous programming: futures, async, await](https://dart.dev/codelabs/async-await)

To perform asynchronous operations in Dart, you can use the `Future` class and the `async` and `await` keywords.



###  Example: Incorrectly using an asynchronous function

> NOTE: 
>
> 原文给出的example非常好，非常好的战术了asynchronous function的用法
>
> "uncompleted future"是非常好的表达，在下面的 [What is a future?](https://dart.dev/codelabs/async-await) 段中有着介绍。

###  [Example: Introducing futures](https://dart.dev/codelabs/async-await)

> NOTE: 
>
> 非常好的example

### Working with futures: async and await

> NOTE: 
>
> `async` 关键字告诉dart，当前的function是一个asynchronous function
>
> "`await` keyword to wait for a future to complete"，显然它会造成阻塞、将asynchronous转化为synchrounous，原文也有这样的总结:
>
> > the `async` and `await` keywords result in asynchronous code that looks a lot like synchronous code





## dart libraries [Futures and error handling](https://dart.dev/guides/libraries/futures-error-handling)

> NOTE: 
>
> 老的方式，这种方式总结起来是:
>
> future + callback



```dart

Future<String> one() => Future.value('from one');
Future<String> two() => Future.error('error from two');
Future<String> three() => Future.value('from three');
Future<String> four() => Future.value('from four');

// Output of this program:
//   Got error: error from two
//   The value is 42
Future<void> main() async {
  await one() // Future completes with "from one".
      .then((_) => two()) // Future completes with two()'s error.
      .then((_) => three()) // Future completes with two()'s error.
      .then((_) => four()) // Future completes with two()'s error.
      .then((value) => value.length) // Future completes with two()'s error.
      .catchError((e) {
    print('Got error: $e'); // Finally, callback fires.
    return 42; // Future completes with 42.
  }).then((value) {
    print('The value is $value');
  });
}
```

> NOTE: 
>
> 上述程序是能够直接运行的



## medium [Dart asynchronous programming: Isolates and event loops](https://medium.com/dartlang/dart-asynchronous-programming-isolates-and-event-loops-bffc3e296a6a)



## Callback to the Future

如何将callback idiom转换为future idiom。

medium [Callback to the Future](https://medium.com/flutter-community/callback-to-the-future-f9ec0095be2c)

manning [Chapter 9. Asynchronous programming with callbacks and futures](https://livebook.manning.com/book/dart-in-action/chapter-9/)



## stackoverflow [Dartlang wait more than one future](https://stackoverflow.com/questions/42176092/dartlang-wait-more-than-one-future)


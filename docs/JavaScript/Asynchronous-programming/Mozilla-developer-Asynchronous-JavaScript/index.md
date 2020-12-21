# developer.mozilla [Asynchronous JavaScript](https://developer.mozilla.org/en-US/docs/Learn/JavaScript/Asynchronous)

## [Guides](https://developer.mozilla.org/en-US/docs/Learn/JavaScript/Asynchronous#Guides)

[General asynchronous programming concepts](https://developer.mozilla.org/en-US/docs/Learn/JavaScript/Asynchronous/Concepts)



[Introducing asynchronous JavaScript](https://developer.mozilla.org/en-US/docs/Learn/JavaScript/Asynchronous/Introducing)



[Cooperative asynchronous JavaScript: Timeouts and intervals](https://developer.mozilla.org/en-US/docs/Learn/JavaScript/Asynchronous/Loops_and_intervals)

Here we look at the traditional methods JavaScript has available for running code asychronously after a set time period has elapsed, or at a regular interval (e.g. a set number of times per second), talk about what they are useful for, and look at their inherent(固有的) issues.

[Handling async operations gracefully with Promises](https://developer.mozilla.org/en-US/docs/Learn/JavaScript/Asynchronous/Promises)

**Promises** are a comparatively new feature of the JavaScript language that allow you to defer further actions until after the previous action has completed, or respond to its failure. This is really useful for setting up a sequence of operations to work correctly. This article shows you how promises work, where you'll see them in use in WebAPIs, and how to write your own.

[Making asynchronous programming easier with async and await](https://developer.mozilla.org/en-US/docs/Learn/JavaScript/Asynchronous/Async_await)

Promises can be somewhat complex to set up and understand, and so modern browsers have implemented `async` functions and the `await` operator. The former(`async`) allows standard functions to implicitly behave asynchronously with promises, whereas the latter can be used inside `async` functions to wait for promises before the function continues. This makes chaining promises simpler and easier to read.

[Choosing the right approach](https://developer.mozilla.org/en-US/docs/Learn/JavaScript/Asynchronous/Choosing_the_right_approach)



## [General asynchronous programming concepts](https://developer.mozilla.org/en-US/docs/Learn/JavaScript/Asynchronous/Concepts)

### [Asynchronous?](https://developer.mozilla.org/en-US/docs/Learn/JavaScript/Asynchronous/Concepts#Asynchronous)

> NOTE: 以Apple spinning rainbow-colored cursor为例说明了**synchronous programming**。

### [Blocking code](https://developer.mozilla.org/en-US/docs/Learn/JavaScript/Asynchronous/Concepts#Blocking_code)

Asynchronous techniques are very useful, particularly in web programming. When a web app runs in a browser and it executes an intensive chunk of code without returning control to the browser, the browser can appear to be frozen. This is called **blocking**; the browser is blocked from continuing to handle user input and perform other tasks until the web app returns control of the processor.

> NOTE: 说明blocking的含义



### [Threads](https://developer.mozilla.org/en-US/docs/Learn/JavaScript/Asynchronous/Concepts#Threads)

#### [JavaScript is single-threaded](https://developer.mozilla.org/en-US/docs/Learn/JavaScript/Asynchronous/Concepts#JavaScript_is_single-threaded)

### [Asynchronous code](https://developer.mozilla.org/en-US/docs/Learn/JavaScript/Asynchronous/Concepts#Asynchronous_code)

Web workers are pretty useful, but they do have their limitations. A major one is they are not able to access the [DOM](https://developer.mozilla.org/en-US/docs/Glossary/DOM) — you can't get a worker to directly do anything to update the UI. We couldn't render our 1 million blue circles inside our worker; it can basically just do the number crunching.

> NOTE: task之间的dependency relation导致了需要有序地执行，使用promise来表达task之间的dependency relation。

The second problem is that although code run in a worker is not blocking, it is still basically **synchronous**. This becomes a problem when a function relies on the results of multiple previous processes to function. Consider the following thread diagrams:

```
Main thread: Task A --> Task B
```

In this case, let's say Task A is doing something like fetching an image from the server and Task B then does something to the image like applying a filter to it. If you start Task A running and then immediately try to run Task B, you'll get an error, because the image won't be available yet.

```
  Main thread: Task A --> Task B --> |Task D|
Worker thread: Task C -----------> |      |
```

In this case, let's say Task D makes use of the results of both Task B and Task C. If we can guarantee that these results will both be available at the same time, then we might be OK, but this is unlikely. If Task D tries to run when one of its inputs is not yet available, it will throw an error.

To fix such problems, browsers allow us to run certain operations asynchronously. Features like [Promises](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Promise) allow you to set an operation running (e.g. the fetching of an image from the server), and then wait until the result has returned before running another operation:

```
Main thread: Task A                   Task B
    Promise:      |__async operation__|
```

Since the operation is happening somewhere else, the main thread is not blocked while the async operation is being processed.

We'll start to look at how we can write asynchronous code in the next article. Exciting stuff, huh? Keep reading!
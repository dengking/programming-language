# Asynchronous programming

JavaScript中，function是first-class citizen，因此，在JavaScript中，进行asynchronous programming是非常便利的。

## 发展概述

从developer.mozilla [Asynchronous JavaScript](https://developer.mozilla.org/en-US/docs/Learn/JavaScript/Asynchronous) 中，可知: 

promise->async/await

在下面的benestudio [Async/await vs Coroutines vs Promises vs Callbacks](https://blog.benestudio.co/async-await-vs-coroutines-vs-promises-eaedee4e0829)中，会进行更加详细的分析。

## benestudio [Async/await vs Coroutines vs Promises vs Callbacks](https://blog.benestudio.co/async-await-vs-coroutines-vs-promises-eaedee4e0829)

How are `async/await`, `coroutines`, `promises` and `callbacks` related? Which one is better or worse? We are going to implement the same code with these 4 different approaches.

First of all, let’s define a simple `delay` function, that resolves with a given value after a given amount of time elapsed.

### Async Code With Async/await in JavaScript

> 

This is the most recent feature of all 4 approaches.

#### Pros

- Clean code, low syntax noise
- Native in current Chrome, Node.js

#### Cons

- Still needs to be transpiled(转换) with Babel for most projects

    > NOTE: Babel是一个transcompiler，参见 `JavaScript\Implementation\Babel` 章节

### Async Code With Promise Chaining

This is more oldschool than `async/await`.

#### Pros

- No extra libraries needed since Promises are native

#### Cons

- Noisy callbacks

### Async Code With Callbacks

Years ago this was the usual way to handle async continuations.

#### Pros

- Easy to understand
- No libs needed, backward compatible

#### Cons

- Pyramid of doom

    > NOTE: 表面意思是: ，引申义: 回调地狱，参见 `Theory\Pyramid-of-doom` 章节

- More difficult to maintain

### Async Code With Coroutines

This is the most exotic one, having similar syntax to `async/await`.

#### Pros

- No pyramid of doom
- Flow can be traced easily

#### Cons

- Some coroutine library is needed
- Generators must be used

If you are curious how a simple coroutine runner works, here we go. It’s just a simplified implementation to have an understanding of the concept.

### Conclusions & Opinions

In most cases `async/await` is the best choice, since it makes the code maintainable and it’s supported by Node.js and modern browsers. Older browsers or Node.js versions can be targeted by *Babel* transpiler.

Some folks like to write *callback hells* and *pyramid of dooms*. It’s time for them to move on and practise coding with`async/await`.

Of course, without a deep knowledge of *Promises*, `async/await` or *coroutines* are just magic, so it is very valuable to have a close friendship with *Promises*.

A typical JavaScript interview question could be to write equivalent code with the 4 above approaches.

Thanks for reading. Don’t forget to clap if you liked it!

### See Also

- [Exception Handling in ES6 — async-await with try-catch](https://blog.benestudio.co/exception-handling-in-es6-async-await-c9c7d8fb1dc5)
- [Optional Chaining Operator in JavaScript](https://blog.benestudio.co/optional-chaining-operator-in-javascript-342082de2db)
- [Currying in JavaScript ES6](https://blog.benestudio.co/currying-in-javascript-es6-540d2ad09400)

## TODO

logrocket [Understanding (and effectively using) asynchronous JavaScript](https://blog.logrocket.com/understanding-asynchronous-javascript/)
# [How does Node.js manage timers internally](https://asafdav2.github.io/2017/node-js-timers/)

Timers are an essential part in the Javascript developer tool set. The timers API has been with us a long time, dating back to the days when Javascript was limited to the browser. This API provides us with the `setTimeout`, `clearTimeout`, `setInterval` and `clearInterval` methods which allows us to schedule code for later execution, either once or repeatedly.

Thanks to Node.js [timer module](https://nodejs.org/api/timers.html), these methods (along with a few others, such as `setImmediate` and `clearImmediate`) are also available natively in node code. On top of user code and 3rd-party libraries using timers, timers are also used internally by the Node.js platform itself. For example, a dedicated timer is used with each TCP connection to detect a possible **connection timeout**. It’s very possible that Node.js will have to manage a large number of **timers**, so it’s important that the implementation will be highly efficient. In this post I will look at the way Node.js manages these timers under the hood.

***SUMMARY*** : 在node.js内部实现中，也是需要timer功能的；所以node.js内部是需要维护大量的timer的；以此类推，在OS中，也是如此；

Before tackling the timers implementation, it’s worthwhile to examine Node.js implementation of linked lists; they play a large role in the timers implementation, and have some interesting parts to them as well.



## Linked Lists

The [relevant code](https://github.com/nodejs/node/blob/master/lib/internal/linkedlist.js) is quite short, and contain all the methods you’d expect to find in a linked list implementation, such as `create`, `append`, `remove`, `peek`, `shift`, `isEmpty`.

## Timers	

Each Timer instance is initialized with `msec` argument which determines the delay (in millisecond) until timeout. It’s quite intuitive that if we initialized two timers with the same `msec` argument, then the second timer will timeout either with or after the first. Node.js uses this and organizes the Timers by indexing them according to their `msec`: all Timers with the same `msec` value will form a linked list, ordered by creation time (there are actually two indexes, one for `refed` timers and one for `unrefed` timers, but we’ll ignore this fact for now. See [here](https://nodejs.org/api/timers.html#timers_class_timeout) to read about the difference).

![Indexing timers by msec](https://asafdav2.github.io/public/img/node_js_timers/refed_timers.png)

In this example, we initialized 6 timers. `T1`,`T2`,`T3` were initialized with 50 msec, `T4` with 10 msec and `T5,T6` with 200 msec.

Each Timers list is backed up by a `TimerWrap`, which is a wrapper over a `uv_timer_t`, a [native libuv timer type](http://docs.libuv.org/en/v1.x/timer.html). Since we know the timers in each list are ordered by non-decreasing timeout, a single `TimerWrap` is enough, as we can reuse it between timeouts.


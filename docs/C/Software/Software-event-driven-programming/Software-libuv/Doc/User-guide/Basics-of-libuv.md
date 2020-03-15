# Basics of libuv[¶](http://docs.libuv.org/en/v1.x/guide/basics.html#basics-of-libuv)

`libuv` enforces an **asynchronous**, **event-driven** style of programming. Its core job is to provide an **event loop** and **callback based notifications of I/O** and other activities. `libuv` offers core utilities like timers, non-blocking networking support, asynchronous file system access, child processes and more.

## Event loops[¶](http://docs.libuv.org/en/v1.x/guide/basics.html#event-loops)

In event-driven programming, an application expresses interest in certain **events** and respond to them when they occur. The responsibility of gathering events from the operating system or monitoring other sources of events is handled by `libuv`, and the user can register callbacks to be invoked when an event occurs. The event-loop usually keeps running *forever*. In pseudocode:

```pseudocode
while there are still events to process:
    e = get the next event
    if there is a callback associated with e:
        call the callback
```

Some examples of events are:

- File is ready for writing
- A socket has data ready to be read
- A timer has timed out

This **event loop** is encapsulated by `uv_run()` – the end-all(终结) function when using `libuv`.

***SUMMARY***:这句话的意思就是说函数`uv_run()`代表的就是**event loop**。我需要理解libuv的programming paradigm，它为了便于用户使用，提供了非常高级的API，这些API期初看起来是非常抽象的，难以理解的，但是一旦掌握了libuv的programming paradigm就非常好理解了。

The most common activity of **systems programs** is to deal with input and output, rather than a lot of number-crunching. The problem with using conventional input/output functions (`read`, `fprintf`, etc.) is that they are **blocking**. The actual write to a hard disk or reading from a network, takes a disproportionately long time compared to the speed of the processor. The functions don’t return until the task is done, so that your program is doing nothing. For programs which require high performance this is a major roadblock as other activities and other I/O operations are kept waiting.

***SUMMARY***:[Asynchronous I/O](https://en.wikipedia.org/wiki/Asynchronous_I/O)  also compare I/O and the processing of data.

One of the standard solutions is to use threads. Each blocking I/O operation is started in a separate thread (or in a thread pool). When the blocking function gets invoked in the thread, the processor can schedule another thread to run, which actually needs the CPU.

***THINKING***:I vaguely remember where I saw a similar description, but now I can't remember it.

20190107：是在APUE的blocking IO章节中。

The approach followed by `libuv` uses another style, which is the **asynchronous, non-blocking** style. Most modern operating systems provide **event notification subsystems**. For example, a normal `read` call on a socket would block until the sender actually sent something. Instead, the application can request the operating system to **watch** the socket and put an **event notification** in the queue. The application can inspect the events at its convenience (perhaps doing some number crunching before to use the processor to the maximum) and grab the data. It is **asynchronous** because the application expressed interest at one point, then used the data at another point (in time and space). It is **non-blocking** because the application process was free to do other tasks. This fits in well with `libuv`’s event-loop approach, since the operating system events can be treated as just another `libuv` event. The non-blocking ensures that other events can continue to be handled as fast as they come in [[1\]](http://docs.libuv.org/en/v1.x/guide/basics.html#id2).

> Note
>
> How the I/O is run in the background is not of our concern, but due to the way our computer hardware works, with the thread as the basic unit of the processor（processor以thread作为basic unit）, `libuv` and OSes will usually run background/worker threads and/or polling to perform tasks in a non-blocking manner.



Bert Belder, one of the `libuv` core developers has a small video explaining the architecture of `libuv` and its background. If you have no prior experience with either `libuv` or `libev`, it is a quick, useful watch.

`libuv`’s event loop is explained in more detail in the [documentation](http://docs.libuv.org/en/v1.x/design.html#the-i-o-loop).

## Hello World[¶](http://docs.libuv.org/en/v1.x/guide/basics.html#hello-world)

With the basics out of the way, lets write our first libuv program. It does nothing, except start a loop which will exit immediately.

helloworld/main.c

```c
#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

int main() {
    uv_loop_t *loop = malloc(sizeof(uv_loop_t));
    uv_loop_init(loop);

    printf("Now quitting.\n");
    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);
    free(loop);
    return 0;
}
```



This program quits immediately because it has no **events** to process. A libuv **event loop** has to be told to watch out for events using the various API functions.

***SUMMARY***：在运行libuv的event loop的时候，需要告知event loop去watch out什么event；

Starting with libuv v1.0, users should allocate the memory for the loops before initializing it with `uv_loop_init(uv_loop_t *)`. This allows you to plug in **custom memory management**. Remember to de-initialize the loop using `uv_loop_close(uv_loop_t *)`and then delete the storage. The examples never close loops since the program quits after the loop ends and the system will reclaim memory. Production grade projects（生产级别的项目）, especially long running systems programs, should take care to release correctly.

### Default loop[¶](http://docs.libuv.org/en/v1.x/guide/basics.html#default-loop)

A **default loop** is provided by libuv and can be accessed using `uv_default_loop()`. You should use this loop if you only want a single loop.

Note:node.js uses the **default loop** as its **main loop**. If you are writing bindings you should be aware of this.

## Error handling[¶](http://docs.libuv.org/en/v1.x/guide/basics.html#error-handling)

**Initialization functions** or **synchronous functions** which may fail return a **negative number** on error. **Async functions** that may fail will pass a **status parameter** to their callbacks. The error messages are defined as `UV_E*` [constants](http://docs.libuv.org/en/v1.x/errors.html#error-constants).

You can use the `uv_strerror(int)` and `uv_err_name(int)` functions to get a `const char *` describing the error or the error name respectively.

I/O read callbacks (such as for files and sockets) are passed a parameter `nread`. If `nread` is less than 0, there was an error (UV_EOF is the end of file error, which you may want to handle differently).

## Handles and Requests[¶](http://docs.libuv.org/en/v1.x/guide/basics.html#handles-and-requests)

libuv works by the user expressing interest in particular **events**. This is usually done by creating a **handle** to an I/O device, timer or process. Handles are [opaque structs](https://en.wikipedia.org/wiki/Opaque_data_type) named as `uv_TYPE_t` where type signifies（表示） what the **handle** is used for.

***SUMMARY***:使用handle来设置对某个event的interest。**handle**的类型表示此handle的用途，其实也就对应了其所关注的event。这也就是libuv的**watcher**。

libuv **watchers**

```c
  UV_REQ_TYPE_PRIVATE
  UV_REQ_TYPE_MAX
} uv_req_type;


/* Handle types. */
typedef struct uv_loop_s uv_loop_t;
typedef struct uv_handle_s uv_handle_t;
typedef struct uv_stream_s uv_stream_t;
typedef struct uv_tcp_s uv_tcp_t;
typedef struct uv_udp_s uv_udp_t;
typedef struct uv_pipe_s uv_pipe_t;
typedef struct uv_tty_s uv_tty_t;
typedef struct uv_poll_s uv_poll_t;
typedef struct uv_timer_s uv_timer_t;
typedef struct uv_prepare_s uv_prepare_t;
typedef struct uv_check_s uv_check_t;
typedef struct uv_idle_s uv_idle_t;
typedef struct uv_async_s uv_async_t;
typedef struct uv_process_s uv_process_t;
typedef struct uv_fs_event_s uv_fs_event_t;
typedef struct uv_fs_poll_s uv_fs_poll_t;
typedef struct uv_signal_s uv_signal_t;

/* Request types. */
typedef struct uv_req_s uv_req_t;
typedef struct uv_getaddrinfo_s uv_getaddrinfo_t;
typedef struct uv_getnameinfo_s uv_getnameinfo_t;
typedef struct uv_shutdown_s uv_shutdown_t;
typedef struct uv_write_s uv_write_t;
typedef struct uv_connect_s uv_connect_t;
typedef struct uv_udp_send_s uv_udp_send_t;
typedef struct uv_fs_s uv_fs_t;
typedef struct uv_work_s uv_work_t;
```

**Handles** represent long-lived objects. **Async operations** on such **handles** are identified using **requests**. A **request** is short-lived (usually used across only one callback。译文： **request**的作用域只在一个**callback**之间) and usually indicates one I/O operation on a **handle**（**request**通常表示在一个**handle**上的一个I/0操作）. **Requests** are used to preserve（维护） **context** between the initiation（启动） and the callback of individual actions. For example, an **UDP socket** is represented by a `uv_udp_t`, while individual writes to the socket use a `uv_udp_send_t` structure that is passed to the callback after the write is done.

***SUMMARY***:request表示当handle所watch的event发生的时候，要执行的asynchronous operation，这些操作通常是I/O操作；

Handles are setup by a corresponding:

```c
uv_TYPE_init(uv_loop_t *, uv_TYPE_t *)
```

function.

**Callbacks** are functions which are called by libuv whenever an event the watcher is interested in has taken place. **Application specific logic** will usually be implemented in the **callback**. For example, an IO watcher’s callback will receive the data read from a file, **a timer callback will be triggered on timeout** and so on.

### Idling[¶](http://docs.libuv.org/en/v1.x/guide/basics.html#idling)

Here is an example of using an **idle handle**. The callback is called once on every turn of the **event loop**. A use case for **idle handles** is discussed in [Utilities](http://docs.libuv.org/en/v1.x/guide/utilities.html). Let us use an **idle watcher** to look at the watcher life cycle and see how `uv_run()` will now block because a watcher is present. The **idle watcher** is stopped when the count is reached and `uv_run()` exits since no event watchers are active.



### Storing context[¶](http://docs.libuv.org/en/v1.x/guide/basics.html#storing-context)

In callback based programming style you’ll often want to pass some ‘context’ – application specific information – between the **call site** and the **callback**. All handles and requests have a `void* data` member which you can set to the context and cast back in the callback. This is a common pattern used throughout the C library ecosystem. In addition `uv_loop_t` also has a similar data member.


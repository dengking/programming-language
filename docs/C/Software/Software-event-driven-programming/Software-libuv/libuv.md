# Welcome to the libuv documentation[¶](http://docs.libuv.org/en/v1.x/#welcome-to-the-libuv-documentation)

## Overview[¶](http://docs.libuv.org/en/v1.x/#overview)

`libuv` is a multi-platform support library with a focus on asynchronous I/O. It was primarily developed for use by [Node.js](http://nodejs.org/), but it’s also used by [Luvit](http://luvit.io/), [Julia](http://julialang.org/), [pyuv](https://github.com/saghul/pyuv), and [others](https://github.com/libuv/libuv/wiki/Projects-that-use-libuv).

## Features[¶](http://docs.libuv.org/en/v1.x/#features)

- Full-featured **event loop** backed by epoll, kqueue, IOCP, event ports.

  SUMMARY:I have seen the event loop in many places, including [libevent](https://libevent.org/), [celery](http://docs.celeryproject.org/en/latest/userguide/extending.html#worker)

- Asynchronous TCP and UDP sockets

- Asynchronous DNS resolution

- Asynchronous file and file system operations

- File system events

- ANSI escape code controlled TTY

- IPC with socket sharing, using Unix domain sockets or named pipes (Windows)

- Child processes

- Thread pool

- Signal handling

- High resolution clock

- Threading and synchronization primitives




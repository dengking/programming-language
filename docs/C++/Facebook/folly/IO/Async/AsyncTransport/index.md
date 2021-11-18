# [folly](https://github.com/facebook/folly)/[folly](https://github.com/facebook/folly/tree/main/folly)/[io](https://github.com/facebook/folly/tree/main/folly/io)/[async](https://github.com/facebook/folly/tree/main/folly/io/async)/**[AsyncTransport.h](https://github.com/facebook/folly/blob/main/folly/io/async/AsyncTransport.h)**



## `AsyncTransport`



> AsyncTransport defines an asynchronous API for bidirectional streaming I/O.

read、write；

### Operations

|              |                                         |      |
| ------------ | --------------------------------------- | ---- |
| `close`      | `close`、`closeNow`、`closeWithReset`、 |      |
| `read`       |                                         |      |
| `write`      |                                         |      |
| `connect`    |                                         |      |
| `event_base` | `attachEventBase`、`detachEventBase`    |      |



## 基于callback的

### `ReadCallback`



### `WriteCallback`



### `ReleaseIOBufCallback`



### `BufferCallback`



### `ReplaySafetyCallback`
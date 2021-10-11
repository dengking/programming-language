# [The Chromium Project](https://github.com/chromium)

## github [chromium](https://github.com/chromium)/**[chromium](https://github.com/chromium/chromium)**



## [chromium](https://github.com/chromium/chromium)/**[base](https://github.com/chromium/chromium/tree/master/base)**/



## closure、task、queue、concurrent

[chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/master/base)/**[task](https://github.com/chromium/chromium/tree/master/base/task)**/

This directory has the following layout:

- base/task/: public APIs for posting tasks and managing task queues.
- base/task/thread_pool/: implementation of the ThreadPool.
- base/task/sequence_manager/: implementation of the SequenceManager.

Apart from embedders explicitly managing a ThreadPoolInstance and/or SequenceManager instance(s) for their process/threads, the vast majority of users should only need APIs in base/task/.

Documentation:

- [Threading and tasks](https://github.com/chromium/chromium/blob/master/docs/threading_and_tasks.md)

  > NOTE: 
  >
  > 非常好，讲解得非常深入

- [Callbacks](https://github.com/chromium/chromium/blob/master/docs/callback.md)

  > NOTE: 
  >
  > 主要将closure、bind

- [Vision for future API changes](https://docs.google.com/document/d/1pySz2xeJ6kLlbzDnS2jqAC1F8T_6pLEV8pgaMfURXAw/edit)





https://www.chromium.org/developers/libraries-guide

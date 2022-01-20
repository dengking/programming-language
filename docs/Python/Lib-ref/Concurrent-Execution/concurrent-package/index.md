# [The `concurrent` package](https://docs.python.org/3/library/concurrent.html)

**Source code:** [Lib/concurrent/futures/thread.py](https://github.com/python/cpython/tree/3.10/Lib/concurrent/futures/thread.py) and [Lib/concurrent/futures/process.py](https://github.com/python/cpython/tree/3.10/Lib/concurrent/futures/process.py)



## [Executor Objects](https://docs.python.org/3/library/concurrent.futures.html#executor-objects)

An abstract class that provides methods to execute calls asynchronously. It should not be used directly, but through its concrete subclasses.

When using [`ProcessPoolExecutor`](https://docs.python.org/3/library/concurrent.futures.html#concurrent.futures.ProcessPoolExecutor), this method chops *iterables* into a number of chunks which it submits to the pool as separate tasks. The (approximate) size of these chunks can be specified by setting *chunksize* to a positive integer. For very long iterables, using a large value for *chunksize* can significantly improve performance compared to the default size of 1. With [`ThreadPoolExecutor`](https://docs.python.org/3/library/concurrent.futures.html#concurrent.futures.ThreadPoolExecutor), *chunksize* has no effect.

> NOTE: 
>
> 一、"当使用ProcessPoolExecutor时，该方法将可迭代对象分割成许多块，并将这些块作为单独的任务提交到池中"
>
> 当使用  [`ProcessPoolExecutor`](https://docs.python.org/3/library/concurrent.futures.html#concurrent.futures.ProcessPoolExecutor) 的时候，显然是会涉及IPC的，`chunksize`指的是IPC使用的接口？

```python
from concurrent.futures import *
with ThreadPoolExecutor(max_workers=1) as executor:
    future = executor.submit(pow, 323, 1235)
    print(future.result())

```



## [ThreadPoolExecutor](https://docs.python.org/3/library/concurrent.futures.html#threadpoolexecutor)

Deadlocks can occur when the callable associated with a [`Future`](https://docs.python.org/3/library/concurrent.futures.html#concurrent.futures.Future) waits on the results of another [`Future`](https://docs.python.org/3/library/concurrent.futures.html#concurrent.futures.Future). For example:

```python
import time
from concurrent.futures import *

def wait_on_b():
    time.sleep(5)
    print(b.result())  # b will never complete because it is waiting on a.
    return 5

def wait_on_a():
    time.sleep(5)
    print(a.result())  # a will never complete because it is waiting on b.
    return 6


executor = ThreadPoolExecutor(max_workers=2)
a = executor.submit(wait_on_b) # a 依赖于 b
b = executor.submit(wait_on_a) # b 依赖于 a
```

> NOTE: 
>
> 上述代码是充分运用了Python的dynamic的特性:
>
> ```python
> def wait_on_b():
>     time.sleep(5)
>     print(b.result())  # b will never complete because it is waiting on a.
>     return 5
> ```
>
> 在函数体中使用了variable `b`，但是`b`还没有给出definition，那`b` 是在什么地方定义的呢？在下面的地方给出的definition: 
>
> ```python
> a = executor.submit(wait_on_b)
> b = executor.submit(wait_on_a)
> ```
>
> 当执行的时候，则在当前environment中，会首先创造出`future a`、`future b`
>
> 上述deadlock是典型的相互依赖的场景

```Python
from concurrent.futures import *
def wait_on_future():
    f = executor.submit(pow, 5, 2)
    # This will never complete because there is only one worker thread and
    # it is executing this function.
    print(f.result())

executor = ThreadPoolExecutor(max_workers=1)
executor.submit(wait_on_future)
```

> NOTE: 
>
> 上述也是典型的相互依赖的场景: 
>
> 它也使用了Python dynamic 的特性

## [ProcessPoolExecutor](https://docs.python.org/3/library/concurrent.futures.html#processpoolexecutor)

The `__main__` module must be importable by worker subprocesses. This means that [`ProcessPoolExecutor`](https://docs.python.org/3/library/concurrent.futures.html#concurrent.futures.ProcessPoolExecutor) will not work in the interactive interpreter.

> NOTE: 
>
> 简而言之，必须要有`main`函数

Calling [`Executor`](https://docs.python.org/3/library/concurrent.futures.html#concurrent.futures.Executor) or [`Future`](https://docs.python.org/3/library/concurrent.futures.html#concurrent.futures.Future) methods from a callable submitted to a [`ProcessPoolExecutor`](https://docs.python.org/3/library/concurrent.futures.html#concurrent.futures.ProcessPoolExecutor) will result in deadlock.

> NOTE: 
>
> why？

## [Future Objects](https://docs.python.org/3/library/concurrent.futures.html#future-objects)

The [`Future`](https://docs.python.org/3/library/concurrent.futures.html#concurrent.futures.Future) class encapsulates the asynchronous execution of a callable. [`Future`](https://docs.python.org/3/library/concurrent.futures.html#concurrent.futures.Future) instances are created by [`Executor.submit()`](https://docs.python.org/3/library/concurrent.futures.html#concurrent.futures.Executor.submit).



## [Module Functions](https://docs.python.org/3/library/concurrent.futures.html#module-functions)

`concurrent.futures.as_completed(fs, timeout=None)`

> NOTE: 
>
> stackoverflow [How does concurrent.futures.as_completed work?](https://stackoverflow.com/questions/51239251/how-does-concurrent-futures-as-completed-work)
>
> programcreek [Python concurrent.futures.as_completed() Examples](https://www.programcreek.com/python/example/86022/concurrent.futures.as_completed)
>
> 
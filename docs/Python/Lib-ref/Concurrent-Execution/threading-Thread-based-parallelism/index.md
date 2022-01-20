# [`threading`](https://docs.python.org/3/library/threading.html#module-threading) — Thread-based parallelism

## **CPython implementation detail:**

In CPython, due to the [Global Interpreter Lock](https://docs.python.org/3/glossary.html#term-global-interpreter-lock), only one thread can execute Python code at once (even though certain performance-oriented libraries might overcome this limitation). 

However, threading is still an appropriate model if you want to run multiple I/O-bound tasks simultaneously.

> NOTE: 
>
> Python中的IO不需要GIL吗？


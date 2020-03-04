# Iterate

在python中，进行iterate（遍历）是非常遍历的，本文对python中与遍历有关的内容进行总结。

## Python语言提供的service

- [**iterator**](https://docs.python.org/3/glossary.html#term-iterator)
- [Iterator Types](https://docs.python.org/3/library/stdtypes.html#typeiter)

- [`itertools`](https://docs.python.org/3/library/itertools.html#module-itertools) — Functions creating iterators for efficient looping[¶](https://docs.python.org/3/library/itertools.html#module-itertools)

## Application



### [Iterating over every two elements in a list](https://stackoverflow.com/questions/5389507/iterating-over-every-two-elements-in-a-list)



### [Iterate a list as pair (current, next) in Python](https://stackoverflow.com/questions/5434891/iterate-a-list-as-pair-current-next-in-python)



[answer](https://stackoverflow.com/a/5434936)中给出了如下代码：

Here's a relevant example from the [itertools](http://docs.python.org/library/itertools.html#recipes) module docs:

```python
import itertools
def pairwise(iterable):
    "s -> (s0,s1), (s1,s2), (s2, s3), ..."
    a, b = itertools.tee(iterable)
    next(b, None)
    return zip(a, b)   
```

For Python 2, you need `itertools.izip` instead of `zip`:

```python
import itertools
def pairwise(iterable):
    "s -> (s0,s1), (s1,s2), (s2, s3), ..."
    a, b = itertools.tee(iterable)
    next(b, None)
    return itertools.izip(a, b)
```
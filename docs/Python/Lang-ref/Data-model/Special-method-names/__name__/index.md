# `__name__`

stackoverflow [Getting the class name of an instance?](https://stackoverflow.com/questions/510972/getting-the-class-name-of-an-instance)

```python
>>> import itertools
>>> x = itertools.count(0)
>>> type(x).__name__
'count'
```


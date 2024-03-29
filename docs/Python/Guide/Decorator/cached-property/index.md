# flask `locked_cached_property` （lazy property）



## GitHub flask2: [flask](https://github.com/pallets/flask)/[src](https://github.com/pallets/flask/tree/main/src)/[flask](https://github.com/pallets/flask/tree/main/src/flask)/**[helpers.py](https://github.com/pallets/flask/blob/main/src/flask/helpers.py)** 



## GitHub flask1.1: [flask](https://github.com/pallets/flask/tree/1.1.x)/[src](https://github.com/pallets/flask/tree/1.1.x/src)/[flask](https://github.com/pallets/flask/tree/1.1.x/src/flask)/**[helpers.py](https://github.com/pallets/flask/blob/1.1.x/src/flask/helpers.py)**

```python

class locked_cached_property(object):
    """A decorator that converts a function into a lazy property.  The
    function wrapped is called the first time to retrieve the result
    and then that calculated result is used the next time you access
    the value.  Works like the one in Werkzeug but has a lock for
    thread safety.
    """

    def __init__(self, func, name=None, doc=None):
        self.__name__ = name or func.__name__
        self.__module__ = func.__module__
        self.__doc__ = doc or func.__doc__
        self.func = func
        self.lock = RLock()

    def __get__(self, obj, type=None):
        if obj is None:
            return self
        with self.lock:
            value = obj.__dict__.get(self.__name__, _missing)
            if value is _missing:
                value = self.func(obj)
                obj.__dict__[self.__name__] = value
            return value
```



## `cached_property`



```Python

class cached_property(object):
    """A decorator that converts a function into a lazy property.  The
    function wrapped is called the first time to retrieve the result
    and then that calculated result is used the next time you access
    the value.  Works like the one in Werkzeug but has a lock for
    thread safety.
    """

    def __init__(self, func, name=None, doc=None):
        self.__name__ = name or func.__name__
        self.__module__ = func.__module__
        self.__doc__ = doc or func.__doc__
        self.func = func

    def __get__(self, obj, type=None):
        if obj is None:
            return self
        value = obj.__dict__.get(self.__name__, _missing)
        if value is _missing:
            value = self.func(obj)
            obj.__dict__[self.__name__] = value
        return value


```


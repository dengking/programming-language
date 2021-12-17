# Immutable



## Immutable member variable

其实就是override `__setattr__`

stackoverflow [Is it possible to overload Python assignment?](https://stackoverflow.com/questions/11024646/is-it-possible-to-overload-python-assignment)

```Python
class MyClass(object):
    def __init__(self, x):
        self.x = x
        self._locked = True
    def __setattr__(self, name, value):
        if self.__dict__.get("_locked", False) and name == "x":
            raise AttributeError("MyClass does not allow assignment to .x member")
        self.__dict__[name] = value

>>> m = MyClass(3)
>>> m.x
3
>>> m.x = 4
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  File "<stdin>", line 7, in __setattr__
AttributeError: MyClass does not allow assignment to .x member
```

## Immutable data structure

### [werkzeug](https://github.com/pallets/werkzeug)/[src](https://github.com/pallets/werkzeug/tree/main/src)/[werkzeug](https://github.com/pallets/werkzeug/tree/main/src/werkzeug)/**[datastructures.py](https://github.com/pallets/werkzeug/blob/main/src/werkzeug/datastructures.py)** /


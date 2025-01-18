# wikipedia [Type introspection](https://en.wikipedia.org/wiki/Type_introspection)



In [computing](https://en.wikipedia.org/wiki/Computing), *type introspection*(类型反省) is the ability of a program to **examine** the **type** or **properties** of an [object](https://en.wikipedia.org/wiki/Object_(computer_science)) at [runtime](https://en.wikipedia.org/wiki/Run_time_(program_lifecycle_phase)). Some [programming languages](https://en.wikipedia.org/wiki/Programming_language) possess this capability.

Introspection should not be confused with [reflection](https://en.wikipedia.org/wiki/Reflection_(computer_programming)), which goes a step further and is the ability for a program to **manipulate** the values, meta-data, properties and/or functions of an object at runtime. Some programming languages - e.g. Java, Python and Go - also possess that capability.



## Examples





### Python

The most common method of introspection in [Python](https://en.wikipedia.org/wiki/Python_(programming_language)) is using the `dir` function to detail the attributes of an object. For example:

```python
class Foo(object):
    def __init__(self, val):
        self.x = val
    def bar(self):
        return self.x

...

>>> dir(Foo(5))
['__class__', '__delattr__', '__dict__', '__doc__', '__getattribute__',
'__hash__', '__init__', '__module__', '__new__', '__reduce__', '__reduce_ex__',
'__repr__', '__setattr__', '__str__', '__weakref__', 'bar', 'x']
```

Also, the built-in functions `type` and `isinstance` can be used to determine what an object *is* while `hasattr` can determine what an object *does*. For example:

```python
>>> a = Foo(10)
>>> b = Bar(11)
>>> type(a)
<type 'Foo'>
>>> isinstance(a, Foo)
True
>>> isinstance(a, type(a))
True
>>> isinstance(a, type(b))
False
>>> hasattr(a, 'bar')
True
```

In Python 2 but not Python 3, declaring `class Foo` instead of `class Foo(object)` will result in `type` returning the generic `instance` type instead of the class.[[5\]](https://en.wikipedia.org/wiki/Type_introspection#cite_note-5)

> NOTE: 参见`youdao/programming-language-python/python2to3`，其中总结了python2和python3之间的差异


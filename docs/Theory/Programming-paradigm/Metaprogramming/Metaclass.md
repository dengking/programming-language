# Metaclass

## wikipedia [metaclass](https://en.wikipedia.org/wiki/Metaclass) 



因为python的[data model](https://docs.python.org/3.6/reference/datamodel.html)是：

>Objects are Python’s abstraction for data. All data in a Python program is represented by objects or by relations between objects. (In a sense, and in conformance to Von Neumann’s model of a “stored program computer,” code is also represented by objects.)

所以我们在python中定义了一个类之后，python解释器是会使用一个描述类的结构和性质的类对象来表示我们所定义的这个类的，这个类就是本节即将要介绍的**metaclass**。





In [object-oriented](https://en.wikipedia.org/wiki/Object-oriented_programming) [programming](https://en.wikipedia.org/wiki/Computer_programming), a **metaclass** is a [class](https://en.wikipedia.org/wiki/Class_(computer_science)) whose instances are classes. Just as an ordinary class defines the behavior of certain objects, a metaclass defines the behavior of certain classes and their instances. Not all object-oriented programming [languages](https://en.wikipedia.org/wiki/Programming_languages) support **metaclasses**. Among those that do, the extent to which metaclasses can override any given aspect of class behavior varies. Metaclasses can be implemented by having **classes** be [first-class citizen](https://en.wikipedia.org/wiki/First-class_citizen), in which case a metaclass is simply an object that constructs classes. Each language has its own [metaobject protocol](https://en.wikipedia.org/wiki/Metaobject_protocol), a set of rules that govern how objects, classes, and metaclasses interact.

***THINKING*** : **metaclass**的对象就是**metaobject**？？



### Python example H3

In [Python](https://en.wikipedia.org/wiki/Python_(programming_language)), the builtin class [`type`](https://docs.python.org/3.7/library/functions.html#type) is a **metaclass**.[[2\]](https://en.wikipedia.org/wiki/Metaclass#cite_note-2)[[3\]](https://en.wikipedia.org/wiki/Metaclass#cite_note-3)[[4\]](https://en.wikipedia.org/wiki/Metaclass#cite_note-4) Consider this simple Python class:

```python
class Car(object):
    def __init__(self, make, model, year, color):
        self.make = make
        self.model = model
        self.year = year
        self.color = color

    @property
    def description(self):
        """ Return a description of this car. """
        return "%s %s %s %s" % (self.color, self.year, self.make, self.model)
```

At run time, `Car` itself is an instance of `type`. The source code of the `Car` class, shown above, does not include such details as the size in bytes of `Car` objects, their binary layout in memory, how they are allocated, that the `__init__` method is automatically called each time a `Car` is created, and so on. These details come into play not only when a new `Car` object is created, but also each time any attribute of a `Car` is accessed. In languages without **metaclasses**, these details are defined by the **language specification** and **can't be overridden**. In Python, the **metaclass** - `type` - controls these details of `Car`'s behavior. They can be overridden by using a different metaclass instead of `type`.

上面这段话正是本文的核心所在。

The above example contains some redundant code to do with the four attributes `make`, `model`, `year`, and `color`. It is possible to eliminate some of this redundancy using a **metaclass**. In Python, a **metaclass** is most easily defined as a subclass of `type`.

```python
class AttributeInitType(type):
    def __call__(self, *args, **kwargs):
        """ Create a new instance. """

        # First, create the object in the normal default way.
        obj = type.__call__(self, *args)

        # Additionally, set attributes on the new object.
        for name, value in kwargs.items():
            setattr(obj, name, value)

        # Return the new object.
        return obj
```

This **metaclass** only overrides object creation. All other aspects of class and object behavior are still handled by `type`.

Now the class `Car` can be rewritten to use this **metaclass**. In Python 3 this done by providing a "keyword argument" `metaclass` to the class definition:

```python
class Car(object, metaclass=AttributeInitType):
    @property
    def description(self):
        """ Return a description of this car. """
        return " ".join(str(value) for value in self.__dict__.values())
```

The resulting object `Car` can be instantiated as usual, but can contain any number of keyword arguments:

```python
new_car = Car(make='Toyota', model='Prius', year=2005, color='Green', engine='Hybrid')
```

这个例子非常有启发性。





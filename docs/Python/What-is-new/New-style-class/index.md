# New-style Classes



## python doc [New-style Classes](https://www.python.org/doc/newstyle/)

---

> Warning
>
> New-style classes has been integrated into Python 2.7 and old-style classes has been removed in Python 3. Please refer to the [Python tutorial](https://docs.python.org/3/tutorial/classes.html) and the [Descriptor HowTo Guide](https://docs.python.org/3/howto/descriptor.html) for more up-to-date documentation about classes and descriptors respectively.

---



Unfortunately, new-style classes have not yet been integrated into Python's standard documentation. Fear not, however; many people have worked to provide useful information on creating and using **new-style classes**:

- [Unifying types and classes](https://www.python.org/download/releases/2.2.3/descrintro) (aka descrintro) is Guido's essay on new-style classes and should be your starting point.
- Raymond Hettinger's [How-To Guide for Descriptors](http://web.archive.org/web/20170221220235/http://users.rcn.com/python/download/Descriptor.htm) focuses on the single most useful aspect of new-style classes (which includes properties).
- [Python 2.3 Method Resolution Order](https://www.python.org/download/releases/2.3/mro) (MRO) covers multiple inheritance.
- Metaclasses will make your head explode. Here are several approaches to discussing them:
  - David Mertz and Michele Simionato's [DeveloperWorks article](http://gnosis.cx/publish/programming/metaclass_1.html) [[1\]](https://www.python.org/doc/newstyle/#id2)
  - [Mike Fletcher's slideshow](http://www.vrplumber.com/programming/metaclasses.pdf)
  - [Metaclasses in Five Minutes](http://www.voidspace.org.uk/python/articles/five-minutes.shtml)
- [Types and Objects](http://www.cafepy.com/articles/python_types_and_objects/index.html) is the start of a new-style class tutorial with lots of figures and examples, but it's rough and not complete.



[[1]](https://www.python.org/doc/newstyle/#id1) The original article cannot be found on DeveloperWorks anymore. We've replaced it with a mirror.

## stackoverflow [What is the difference between old style and new style classes in Python?](https://stackoverflow.com/questions/54867/what-is-the-difference-between-old-style-and-new-style-classes-in-python)

### [A1](https://stackoverflow.com/a/54873):

From <http://docs.python.org/2/reference/datamodel.html#new-style-and-classic-classes> :

**Up to Python 2.1, old-style classes were the only flavor available to the user.**(在Python 2.1中，旧式类是用户可用的唯一风格。)

The concept of (old-style) class is unrelated to the concept of type: if `x` is an instance of an old-style class, then `x.__class__` designates the class of `x`, but `type(x)` is always `<type   'instance'>`.

This reflects the fact that all old-style instances, independently of their class, are implemented with a single built-in type, called instance.

**New-style classes were introduced in Python 2.2 to unify the concepts of class and type**. 

统class和type

A new-style class is simply a user-defined type, no more, no less.If x is an instance of a new-style class, then `type(x)` is typically the same as `x.__class__`(although this is not guaranteed – a new-style class instance is permitted to override the value returned for `x.__class__`).

**The major motivation for introducing new-style classes is to provide a unified object model with a full meta-model**.

It also has a number of immediate benefits, like the ability to subclass most built-in types, or the introduction of "descriptors", which enable computed properties.

**For compatibility reasons, classes are still old-style by default**.

New-style classes are created by specifying another new-style class (i.e. a type) as a parent class, or the "top-level type" **object** if no other parent is needed.

The behavior of new-style classes differs from that of old-style classes in a number of important details in addition to what type returns.

Some of these changes are fundamental to the new object model, like the way special methods are invoked. Others are "fixes" that could not be implemented before for compatibility concerns, like the method resolution order in case of multiple inheritance.

**Python 3 only has new-style classes**.

No matter if you subclass from `object` or not, classes are new-style in Python 3.



### [A2](https://stackoverflow.com/a/1203997)

**Declaration-wise:**

New-style classes inherit from object, or from another new-style class.

```python
class NewStyleClass(object):
    pass

class AnotherNewStyleClass(NewStyleClass):
    pass
```

Old-style classes don't.

```python
class OldStyleClass():
    pass
```



### [A3](https://stackoverflow.com/a/19950198)

**Important behavior changes between old and new style classes**

- [super](https://stackoverflow.com/questions/576169/understanding-python-super-and-init-methods) added
- MRO changed (explained below)
- [descriptors](http://docs.python.org/2/howto/descriptor.html) added
- new style class objects cannot be raised unless derived from `Exception` (example below)
- [`__slots__`](http://docs.python.org/2/reference/datamodel.html#slots) added

#### MRO (Method Resolution Order) changed

It was mentioned in other answers, but here goes a concrete example of the difference between **classic MRO** and **C3 MRO** (used in new style classes).

The question is the order in which attributes (which include methods and member variables) are searched for in multiple inheritance.

**Classic classes** do a depth first search from left to right. Stop on first match. They do not have the `__mro__` attribute.

```py
class C: i = 0
class C1(C): pass
class C2(C): i = 2
class C12(C1, C2): pass
class C21(C2, C1): pass

assert C12().i == 0
assert C21().i == 2

try:
    C12.__mro__
except AttributeError:
    pass
else:
    assert False
```

**New-style classes** MRO is more complicated to synthesize in a single English sentence. It is explained in detail [here](http://www.python.org/download/releases/2.3/mro/). One of its properties is that a Base class is only searched for once all its Derived classes have been. They have the `__mro__` attribute which shows the search order.

```py
class C(object): i = 0
class C1(C): pass
class C2(C): i = 2
class C12(C1, C2): pass
class C21(C2, C1): pass

assert C12().i == 2
assert C21().i == 2

assert C12.__mro__ == (C12, C1, C2, C, object)
assert C21.__mro__ == (C21, C2, C1, C, object)
```

#### New style class objects cannot be raised unless derived from `Exception`

Around Python 2.5 many classes could be raised, around Python 2.6 this was removed. On Python 2.7.3:

```python
# OK, old:
class Old: pass
try:
    raise Old()
except Old:
    pass
else:
    assert False

# TypeError, new not derived from `Exception`.
class New(object): pass
try:
    raise New()
except TypeError:
    pass
else:
    assert False

# OK, derived from `Exception`.
class New(Exception): pass
try:
    raise New()
except New:
    pass
else:
    assert False

# `'str'` is a new style object, so you can't raise it:
try:
    raise 'str'
except TypeError:
    pass
else:
    assert False
```

## Unifying types and classes in Python 2.2 TODO
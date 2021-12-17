# Descriptor 

## docs [Descriptor HowTo Guide](https://docs.python.org/3/howto/descriptor.html#id1)[¶](https://docs.python.org/3/howto/descriptor.html#descriptor-howto-guide)

### [Definition and Introduction](https://docs.python.org/3/howto/descriptor.html#id3)[¶](https://docs.python.org/3/howto/descriptor.html#definition-and-introduction)

In general, a **descriptor** is an **object attribute** with “binding behavior”, one whose **attribute access** has been overridden by **methods** in the **descriptor protocol**. Those methods are [`__get__()`](https://docs.python.org/3/reference/datamodel.html#object.__get__), [`__set__()`](https://docs.python.org/3/reference/datamodel.html#object.__set__), and [`__delete__()`](https://docs.python.org/3/reference/datamodel.html#object.__delete__). If any of those methods are defined for an object, it is said to be a **descriptor**.

> NOTE: descriptor，中文意思就是描述符，那它描述了什么呢？现在想想，其实descriptor本身是一个instance，这个instance是专门用来描述属性访问的。显然descriptor需要作为一个类的instance的成员变量（也可以称为attribute）才能够对该instance的属性访问产生作用。

The default behavior for **attribute access** is to **get**, **set**, or **delete** the attribute from an object’s dictionary. For instance, `a.x` has a lookup chain starting with `a.__dict__['x']`, then `type(a).__dict__['x']`, and continuing through the base classes of `type(a)` excluding **metaclasses**（其实这段话就反映了base class和metaclass之间的差异）. If the **looked-up value**(所找到的值) is an object defining one of the **descriptor methods**, then Python may（可能，说明并不一定） override the default behavior and invoke the **descriptor method** instead. Where this occurs in the precedence chain depends on which **descriptor methods** were defined（在优先级链何处发生这种情况取决于定义了哪些描述符方法，从后面的描述可知，这个descriptor定义了哪些方法决定了它的类型，descriptor的类型就决定了它的precedence）.

Descriptors are a powerful, general purpose protocol. They are the mechanism behind **properties**, **methods**, **static methods**, **class methods**, and [`super()`](https://docs.python.org/3/library/functions.html#super). They are used throughout Python itself to implement the **new style classes** introduced in version 2.2. Descriptors simplify the underlying C-code and offer a flexible set of new tools for everyday Python programs.

### [Descriptor Protocol](https://docs.python.org/3/howto/descriptor.html#id4)[¶](https://docs.python.org/3/howto/descriptor.html#descriptor-protocol)

```python
descr.__get__(self, obj, type=None) -> value
descr.__set__(self, obj, value) -> None
descr.__delete__(self, obj) -> None
```

That is all there is to it. Define any of these methods and an object is considered a **descriptor** and can override default behavior upon being looked up as an attribute.

> NOTE: 总结：只要定义了上述方法之一，则一个object就被认为是一个descriptor，显然这是python的duck type。最后一段话说明

If an object defines both [`__get__()`](https://docs.python.org/3/reference/datamodel.html#object.__get__) and [`__set__()`](https://docs.python.org/3/reference/datamodel.html#object.__set__), it is considered a **data descriptor**. Descriptors that only define [`__get__()`](https://docs.python.org/3/reference/datamodel.html#object.__get__) are called **non-data descriptors** (they are typically used for methods but other uses are possible).

Data and non-data descriptors differ in how overrides are calculated with respect to entries in an instance’s dictionary. If an instance’s dictionary has an entry with the same name as a **data descriptor**, the **data descriptor** takes precedence（如果在instance’s dictionary有一个和data descriptor同名的entry，那么data descriptor优先）. If an instance’s dictionary has an entry with the same name as a **non-data descriptor**, the **dictionary entry** takes precedence.

思考：data descriptor takes precedence是什么含义？即优先执行data descriptor的函数而不用继续在dictionary中进行look up。**dictionary entry** takes precedence是什么含义？

To make a read-only data descriptor, define both [`__get__()`](https://docs.python.org/3/reference/datamodel.html#object.__get__) and [`__set__()`](https://docs.python.org/3/reference/datamodel.html#object.__set__) with the [`__set__()`](https://docs.python.org/3/reference/datamodel.html#object.__set__) raising an [`AttributeError`](https://docs.python.org/3/library/exceptions.html#AttributeError) when called. Defining the [`__set__()`](https://docs.python.org/3/reference/datamodel.html#object.__set__) method with an exception raising placeholder is enough to make it a data descriptor.

> NOTE: immutable 上面这段描述了如何来定义一个read-only descriptor。

### [Invoking Descriptors](https://docs.python.org/3/howto/descriptor.html#id5)[¶](https://docs.python.org/3/howto/descriptor.html#invoking-descriptors)

A descriptor can be called directly by its method name. For example, `d.__get__(obj)`.

> NOTE：上述`d`是指descriptor，`obj`是它的owner

Alternatively, it is more common for a descriptor to be invoked automatically upon attribute access. For example, `obj.d` looks up `d` in the dictionary of `obj`. If `d` defines the method [`__get__()`](https://docs.python.org/3/reference/datamodel.html#object.__get__), then `d.__get__(obj)` is invoked according to the precedence rules listed below.

思考：`obj.d` 和`d.__get__(obj)`两者的写法正好相反。

The details of invocation depend on whether `obj` is an object or a class.

调用的细节取决于`obj`是object还是class。

For objects, the machinery is in [`object.__getattribute__()`](https://docs.python.org/3/reference/datamodel.html#object.__getattribute__) which transforms `b.x` into `type(b).__dict__['x'].__get__(b, type(b))`. The implementation works through a **precedence chain** that gives **data descriptors** priority over **instance variables**, **instance variables** priority over **non-data descriptors**, and assigns lowest priority to [`__getattr__()`](https://docs.python.org/3/reference/datamodel.html#object.__getattr__) if provided. The full C implementation can be found in [`PyObject_GenericGetAttr()`](https://docs.python.org/3/c-api/object.html#c.PyObject_GenericGetAttr) in [Objects/object.c](https://github.com/python/cpython/tree/3.7/Objects/object.c).

总结：上面这段描述 了在进行attribute access的时候的precedence chain。

For classes, the machinery is in `type.__getattribute__()` which transforms `B.x` into `B.__dict__['x'].__get__(None, B)`. In pure Python, it looks like:

```python
def __getattribute__(self, key):
    "Emulate type_getattro() in Objects/typeobject.c"
    v = object.__getattribute__(self, key)
    if hasattr(v, '__get__'):
        return v.__get__(None, self)
    return v
```

The important points to remember are:

- **descriptors** are invoked by the [`__getattribute__()`](https://docs.python.org/3/reference/datamodel.html#object.__getattribute__) method

- overriding [`__getattribute__()`](https://docs.python.org/3/reference/datamodel.html#object.__getattribute__) prevents automatic **descriptor calls**

- [`object.__getattribute__()`](https://docs.python.org/3/reference/datamodel.html#object.__getattribute__) and `type.__getattribute__()` make different calls to [`__get__()`](https://docs.python.org/3/reference/datamodel.html#object.__get__).

  总结：两者的不同之处在于传给 [`__get__()`](https://docs.python.org/3/reference/datamodel.html#object.__get__)方法的入参是不同的。

- **data descriptors** always override **instance dictionaries**.

- **non-data descriptors** may be overridden by **instance dictionaries**.

The object returned by `super()` also has a custom [`__getattribute__()`](https://docs.python.org/3/reference/datamodel.html#object.__getattribute__) method for invoking **descriptors**（super也是支持descriptor的）. The call `super(B, obj).m()` searches `obj.__class__.__mro__` for the base class `A` immediately following `B` and then returns `A.__dict__['m'].__get__(obj, B)`. If not a descriptor, `m` is returned unchanged. If not in the dictionary, `m` reverts to（将恢复） a search using [`object.__getattribute__()`](https://docs.python.org/3/reference/datamodel.html#object.__getattribute__).

The implementation details are in `super_getattro()` in [Objects/typeobject.c](https://github.com/python/cpython/tree/3.7/Objects/typeobject.c). and a pure Python equivalent can be found in [Guido’s Tutorial](https://www.python.org/download/releases/2.2.3/descrintro/#cooperation).

The details above show that the mechanism for descriptors is embedded in the [`__getattribute__()`](https://docs.python.org/3/reference/datamodel.html#object.__getattribute__) methods for [`object`](https://docs.python.org/3/library/functions.html#object), [`type`](https://docs.python.org/3/library/functions.html#type), and [`super()`](https://docs.python.org/3/library/functions.html#super). Classes inherit this machinery when they derive from [`object`](https://docs.python.org/3/library/functions.html#object) or if they have a meta-class providing similar functionality. Likewise, classes can turn-off descriptor invocation by overriding[`__getattribute__()`](https://docs.python.org/3/reference/datamodel.html#object.__getattribute__).

总结：descriptor机制是嵌入在 [`__getattribute__()`](https://docs.python.org/3/reference/datamodel.html#object.__getattribute__) 中的。

### [Descriptor Example](https://docs.python.org/3/howto/descriptor.html#id6)[¶](https://docs.python.org/3/howto/descriptor.html#descriptor-example)

The following code creates a class whose objects are **data descriptors** which print a message for each get or set. Overriding [`__getattribute__()`](https://docs.python.org/3/reference/datamodel.html#object.__getattribute__) is alternate approach that could do this for every attribute. However, this descriptor is useful for monitoring just a few chosen attributes:

```python
class RevealAccess(object):
    """A data descriptor that sets and returns values
       normally and prints a message logging their access.
    """

    def __init__(self, initval=None, name='var'):
        self.val = initval
        self.name = name

    def __get__(self, obj, objtype):
        print('Retrieving', self.name)
        return self.val

    def __set__(self, obj, val):
        print('Updating', self.name)
        self.val = val

>>> class MyClass(object):
...     x = RevealAccess(10, 'var "x"')
...     y = 5
...
>>> m = MyClass()
>>> m.x
Retrieving var "x"
10
>>> m.x = 20
Updating var "x"
>>> m.x
Retrieving var "x"
20
>>> m.y
5
```

The protocol is simple and offers exciting possibilities. Several use cases are so common that they have been packaged into individual function calls. Properties, bound methods, static methods, and class methods are all based on the **descriptor protocol**.



## examples

### flask [`ConfigAttribute`](https://github.com/pallets/flask/blob/main/src/flask/config.py#L9)


## TODO

stackoverflow [Understanding __get__ and __set__ and Python descriptors](https://stackoverflow.com/questions/3798835/understanding-get-and-set-and-python-descriptors)
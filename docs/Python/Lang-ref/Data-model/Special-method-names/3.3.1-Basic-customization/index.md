# 3.3.1. Basic customization[¶](https://docs.python.org/3/reference/datamodel.html#basic-customization)

| 名称                                                         |      |
| ------------------------------------------------------------ | ---- |
| `object.__new__(cls[, ...])`[¶](https://docs.python.org/3/reference/datamodel.html#object.__new__) |      |
| `object.__init__(self[, ...])`[¶](https://docs.python.org/3/reference/datamodel.html#object.__init__) |      |
| `object.__del__(self)`[¶](https://docs.python.org/3/reference/datamodel.html#object.__del__) |      |
| `object.__repr__(self)`[¶](https://docs.python.org/3/reference/datamodel.html#object.__repr__) |      |
| `object.__str__(self)`[¶](https://docs.python.org/3/reference/datamodel.html#object.__str__) |      |
| `object.__bytes__(self)`[¶](https://docs.python.org/3/reference/datamodel.html#object.__bytes__) |      |
| `object.__format__(self, format_spec)`[¶](https://docs.python.org/3/reference/datamodel.html#object.__format__) |      |
| `object.__lt__(self, other)`[¶](https://docs.python.org/3/reference/datamodel.html#object.__lt__)<br>`object.__le__(self, other)`[¶](https://docs.python.org/3/reference/datamodel.html#object.__le__)<br>`object.__eq__(self, other)`[¶](https://docs.python.org/3/reference/datamodel.html#object.__eq__)<br>`object.__ne__(self, other)`[¶](https://docs.python.org/3/reference/datamodel.html#object.__ne__)<br>`object.__gt__(self, other)`[¶](https://docs.python.org/3/reference/datamodel.html#object.__gt__)<br>`object.__ge__(self, other)`[¶](https://docs.python.org/3/reference/datamodel.html#object.__ge__)<br> |      |
| `object.__hash__(self)`[¶](https://docs.python.org/3/reference/datamodel.html#object.__hash__) |      |
| `object.__bool__(self)`[¶](https://docs.python.org/3/reference/datamodel.html#object.__bool__) |      |



## `object.__del__(self)`[¶](https://docs.python.org/3/reference/datamodel.html#object.__del__)

Called when the instance is about to be destroyed. This is also called a finalizer or (improperly) a destructor. If a base class has a [`__del__()`](https://docs.python.org/3/reference/datamodel.html#object.__del__) method, the derived class’s [`__del__()`](https://docs.python.org/3/reference/datamodel.html#object.__del__) method, if any, must explicitly call it to ensure proper deletion of the base class part of the instance.

It is possible (though not recommended!) for the [`__del__()`](https://docs.python.org/3/reference/datamodel.html#object.__del__) method to postpone destruction of the instance by creating a new reference to it. This is called object *resurrection*（复活）. It is implementation-dependent whether [`__del__()`](https://docs.python.org/3/reference/datamodel.html#object.__del__) is called a second time when a resurrected object is about to be destroyed; the current [CPython](https://docs.python.org/3/glossary.html#term-cpython)implementation only calls it once.

总结：当一个a resurrected object要被destroy的时候， [`__del__()`](https://docs.python.org/3/reference/datamodel.html#object.__del__)是否还会被调用一次是implementation-dependent 。

It is not guaranteed that [`__del__()`](https://docs.python.org/3/reference/datamodel.html#object.__del__) methods are called for objects that still exist when the interpreter exits.

总结：看了上面的描述，感觉python的 [`__del__()`](https://docs.python.org/3/reference/datamodel.html#object.__del__)是挺不靠谱的。

---

> Note
>
> `del x` doesn’t directly call `x.__del__()` — the former decrements the reference count for `x` by one, and the latter is only called when `x`’s reference count reaches zero.

---

**CPython implementation detail:** It is possible for a **reference cycle** to prevent the reference count of an object from going to zero. In this case, the cycle will be later detected and deleted by the [cyclic garbage collector](https://docs.python.org/3/glossary.html#term-garbage-collection). A common cause of **reference cycles** is when an exception has been caught in a local variable. The frame’s locals then reference the exception, which references its own traceback, which references the locals of all frames caught in the traceback.

思考：上面这段话中所举的reference cycles的例子我没有搞清楚。

---

> See also
>
> Documentation for the [`gc`](https://docs.python.org/3/library/gc.html#module-gc) module.

---



---

> Warning
>
> Due to the precarious（危险） circumstances under which [`__del__()`](https://docs.python.org/3/reference/datamodel.html#object.__del__) methods are invoked, exceptions that occur during their execution are ignored, and a warning is printed to `sys.stderr` instead. In particular:
> - [`__del__()`](https://docs.python.org/3/reference/datamodel.html#object.__del__) can be invoked when arbitrary code is being executed, including from any arbitrary thread. If [`__del__()`](https://docs.python.org/3/reference/datamodel.html#object.__del__) needs to take a lock or invoke any other blocking resource, it may deadlock as the resource may already be taken by the code that gets interrupted to execute [`__del__()`](https://docs.python.org/3/reference/datamodel.html#object.__del__).
>
>   思考：
>
> - [`__del__()`](https://docs.python.org/3/reference/datamodel.html#object.__del__) can be executed during interpreter shutdown. As a consequence, the **global variables** it needs to access (including other modules) may already have been deleted or set to `None`. Python guarantees that globals whose name begins with a single underscore are deleted from their module before other globals are deleted; if no other references to such globals exist, this may help in assuring（保证） that imported modules are still available at the time when the [`__del__()`](https://docs.python.org/3/reference/datamodel.html#object.__del__) method is called.

---



## `object.__hash__(self)`[¶](https://docs.python.org/3/reference/datamodel.html#object.__hash__)

Called by built-in function [`hash()`](https://docs.python.org/3/library/functions.html#hash) and for operations on members of **hashed collections** including [`set`](https://docs.python.org/3/library/stdtypes.html#set), [`frozenset`](https://docs.python.org/3/library/stdtypes.html#frozenset), and [`dict`](https://docs.python.org/3/library/stdtypes.html#dict).  

[`__hash__()`](https://docs.python.org/3/reference/datamodel.html#object.__hash__) should return an integer. The only required property is that objects which compare equal have the same hash value; it is advised to mix together the hash values of the components of the object that also play a part in comparison of objects by packing them into a tuple and hashing the tuple. 

总结：上面这段话中提到的：objects which compare equal have the same hash value；compare equal意味着会调用对象的[`__eq__()`](https://docs.python.org/3/reference/datamodel.html#object.__eq__)，而获取hash值则需要调用对象的[`__hash__()`](https://docs.python.org/3/reference/datamodel.html#object.__hash__)方法；需要思考的是：为什么有这样的要求？

总结：上面这段话中提及了保证compare equal和hash value同步的最佳实践是：参与了object比较的成员也需要参与hash value的计算。

Example:

```python
def __hash__(self):
    return hash((self.name, self.nick, self.color))
```

---

> Note
>
> [`hash()`](https://docs.python.org/3/library/functions.html#hash) truncates the value returned from an object’s custom [`__hash__()`](https://docs.python.org/3/reference/datamodel.html#object.__hash__) method to the size of a`Py_ssize_t`. This is typically 8 bytes on 64-bit builds and 4 bytes on 32-bit builds. If an object’s [`__hash__()`](https://docs.python.org/3/reference/datamodel.html#object.__hash__)must interoperate(互操作) on builds of different bit sizes, be sure to check the width on all supported builds. An easy way to do this is with `python -c "import sys; print(sys.hash_info.width)"`.
>
> 思考：上面这段话是什么意思？

---

If a class does not define an [`__eq__()`](https://docs.python.org/3/reference/datamodel.html#object.__eq__) method it should not define a [`__hash__()`](https://docs.python.org/3/reference/datamodel.html#object.__hash__) operation either（未定义eq也不要定义hash）; if it defines [`__eq__()`](https://docs.python.org/3/reference/datamodel.html#object.__eq__) but not [`__hash__()`](https://docs.python.org/3/reference/datamodel.html#object.__hash__)（定义了eq但是没有定义hash）, its instances will not be usable as items in hashable collections.If a class defines **mutable** objects（什么是mutable objects） and implements an [`__eq__()`](https://docs.python.org/3/reference/datamodel.html#object.__eq__) method, it should not implement [`__hash__()`](https://docs.python.org/3/reference/datamodel.html#object.__hash__), since the implementation of **hashable collections** requires that a key’s hash value is immutable (if the object’s hash value changes, it will be in the wrong hash bucket).

思考：什么是mutable object？

User-defined classes have [`__eq__()`](https://docs.python.org/3/reference/datamodel.html#object.__eq__) and [`__hash__()`](https://docs.python.org/3/reference/datamodel.html#object.__hash__) methods by default; with them, all objects compare unequal (except with themselves) and `x.__hash__()` returns an appropriate value such that `x == y` implies both that `x is y` and `hash(x) == hash(y)`.

总结：显然eq和hash的默认实现是同步的，这种性质在离散数学中是有专门的术语来进行描述。

A class that overrides [`__eq__()`](https://docs.python.org/3/reference/datamodel.html#object.__eq__) and does not define [`__hash__()`](https://docs.python.org/3/reference/datamodel.html#object.__hash__) will have its [`__hash__()`](https://docs.python.org/3/reference/datamodel.html#object.__hash__) implicitly set to `None`. When the [`__hash__()`](https://docs.python.org/3/reference/datamodel.html#object.__hash__) method of a class is `None`, instances of the class will raise an appropriate [`TypeError`](https://docs.python.org/3/library/exceptions.html#TypeError)when a program attempts to retrieve their **hash value**, and will also be correctly identified as **unhashable** when checking `isinstance(obj, collections.abc.Hashable)`.

If a class that overrides [`__eq__()`](https://docs.python.org/3/reference/datamodel.html#object.__eq__) needs to retain(保留) the implementation of [`__hash__()`](https://docs.python.org/3/reference/datamodel.html#object.__hash__) from a parent class, the interpreter must be told this explicitly by setting `__hash__ = <ParentClass>.__hash__`.

If a class that does not override [`__eq__()`](https://docs.python.org/3/reference/datamodel.html#object.__eq__) wishes to suppress hash support, it should include `__hash__ =None` in the class definition. A class which defines its own [`__hash__()`](https://docs.python.org/3/reference/datamodel.html#object.__hash__) that explicitly raises a [`TypeError`](https://docs.python.org/3/library/exceptions.html#TypeError) would be incorrectly identified as hashable by an `isinstance(obj, collections.abc.Hashable)` call.

---

> Note
>
> By default, the [`__hash__()`](https://docs.python.org/3/reference/datamodel.html#object.__hash__) values of `str`, `bytes` and `datetime` objects are “salted” with an unpredictable random value. Although they remain constant within an individual Python process, they are not predictable between repeated invocations of Python.
>
> This is intended to provide protection against a denial-of-service caused by carefully-chosen inputs that exploit the worst case performance of a dict insertion, O(n^2) complexity. 
>
> See<http://www.ocert.org/advisories/ocert-2011-003.html> for details.
>
> Changing hash values affects the iteration order of sets. Python has never made guarantees about this ordering (and it typically varies between 32-bit and 64-bit builds).
>
> See also [`PYTHONHASHSEED`](https://docs.python.org/3/using/cmdline.html#envvar-PYTHONHASHSEED).

---
Changed in version 3.3: Hash randomization is enabled by default.
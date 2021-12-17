# 3.3. Special method names[¶](https://docs.python.org/3/reference/datamodel.html#special-method-names)

原文的内容比较冗长，检索起来不便，本文是对其的一个总结，可以作为shortcut。

## 3.3.1. Basic customization[¶](https://docs.python.org/3/reference/datamodel.html#basic-customization)

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



## 3.3.2. Customizing attribute access[¶](https://docs.python.org/3/reference/datamodel.html#customizing-attribute-access)



## 3.3.3. Customizing class creation[¶](https://docs.python.org/3/reference/datamodel.html#customizing-class-creation)



## 3.3.4. Customizing instance and subclass checks[¶](https://docs.python.org/3/reference/datamodel.html#customizing-instance-and-subclass-checks)



## 3.3.5. Emulating generic types[¶](https://docs.python.org/3/reference/datamodel.html#emulating-generic-types)



## 3.3.6. Emulating callable objects[¶](https://docs.python.org/3/reference/datamodel.html#emulating-callable-objects)



## 3.3.7. Emulating container types[¶](https://docs.python.org/3/reference/datamodel.html#emulating-container-types)

| 名称                                                         |                                                          | 网络资源                                                     | 例子 |
| ------------------------------------------------------------ | -------------------------------------------------------- | ------------------------------------------------------------ | ---- |
| `object.__contains__(self, item)`[¶](https://docs.python.org/3/reference/datamodel.html#object.__contains__) | Called to implement membership test operators（如`in`）. | - [Functionality of Python `in` vs. `__contains__`](https://stackoverflow.com/questions/38542543/functionality-of-python-in-vs-contains) <br>- https://www.oreilly.com/library/view/python-in-a/0596001886/re16.html |      |



## 3.3.8. Emulating numeric types[¶](https://docs.python.org/3/reference/datamodel.html#emulating-numeric-types)

这是最最简单的。



## 3.3.9. With Statement Context Managers[¶](https://docs.python.org/3/reference/datamodel.html#with-statement-context-managers)
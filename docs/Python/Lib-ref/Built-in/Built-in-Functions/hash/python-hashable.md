# [hashable](https://docs.python.org/3/glossary.html#term-hashable)

An object is *hashable* if it has a **hash value** which never changes during its lifetime (it needs a [`__hash__()`](https://docs.python.org/3/reference/datamodel.html#object.__hash__) method), and can be compared to other objects (it needs an [`__eq__()`](https://docs.python.org/3/reference/datamodel.html#object.__eq__) method). **Hashable objects** which compare equal must have the same **hash value**.

Hashability makes an object usable as a dictionary key and a set member, because these data structures use the hash value internally.

总结：显然Hashability 是一个object成为dictionary key和set member的前提条件，因为这些data structure在内部使用hash value。

All of Python’s immutable built-in objects are hashable（注意此处的build-in object是指由python interpreter所创建的）; mutable containers (such as lists or dictionaries) are not. Objects which are instances of user-defined classes are hashable by default. They all compare unequal (except with themselves), and their hash value is derived from their [`id()`](https://docs.python.org/3/library/functions.html#id).

总结：显然hashability和mutability一样，是object的一种性质。

# [How are Python's Built In Dictionaries Implemented](https://stackoverflow.com/questions/327311/how-are-pythons-built-in-dictionaries-implemented)





# [How are dictionaries implemented?](http://effbot.org/pyfaq/how-are-dictionaries-implemented.htm)



# [Python dictionary implementation](https://www.laurentluce.com/posts/python-dictionary-implementation/)





# [How is set() implemented?](https://stackoverflow.com/questions/3949310/how-is-set-implemented)



# [Internal working of Set in Python](https://www.geeksforgeeks.org/internal-working-of-set-in-python/)
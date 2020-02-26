[TOC]



# [Python Programming/Reflection](https://en.wikibooks.org/wiki/Python_Programming/Reflection)

A Python script can find out about the type, class, attributes and methods of an object. This is referred to as **reflection** or **introspection**. See also [Metaclasses](https://en.wikibooks.org/wiki/Python_Programming/Metaclasses).

Reflection-enabling functions include `type()`, `isinstance()`, `callable()`, `dir()` and `getattr()`.

## `type`

The type method enables to find out about the type of an object. The following tests return True:

```python
type(3) is int
type(3.0) is float
type(10**10) is long # Python 2
type(1 + 1j) is complex
type('Hello') is str
type([1, 2]) is list
type([1, [2, 'Hello']]) is list
type({'city': 'Paris'}) is dict
type((1,2)) is tuple
type(set()) is set
type(frozenset()) is frozenset
type(3).__name__ == "int"
type('Hello').__name__ == "str"
import types, re, Tkinter # For the following examples
type(re) is types.ModuleType
type(re.sub) is types.FunctionType
type(Tkinter.Frame) is types.ClassType
type(Tkinter.Frame).__name__ == "classobj"
type(Tkinter.Frame()).__name__ == "instance"
type(re.compile('myregex')).__name__ == "SRE_Pattern"
type(type(3)) is types.TypeType
```

The type function disregards(忽视) class inheritance: "`type(3) is object`" yields False while "`isinstance(3, object)`" yields True.

***SUMMARY*** : 关于`type` 和 `isinstance`之间的差异，参见`programming-language-python/python-docs/The Python Standard Library/Built-in Functions`

Links:

- [2. Built-in Functions # type](https://docs.python.org/2/library/functions.html#type), python.org
- [8.15. types — Names for built-in types](https://docs.python.org/2/library/types.html), python.org





## `isinstance`

Determines whether an **object** is an **instance** of a type or class.

The following tests return True:

```python
isinstance(3, int)
isinstance([1, 2], list)
isinstance(3, object)
isinstance([1, 2], object)
import tkinter; isinstance(tkinter.Frame(), tkinter.Frame)
import tkinter; tkinter.Frame().__class__.__name__ == "Frame"
```

Note that isinstance provides a weaker condition than a comparison using [#Type](https://en.wikibooks.org/wiki/Python_Programming/Reflection#Type).

Function `isinstance` and a user-defined class:

```python
class Plant: pass                        # Dummy class
class Tree(Plant): pass                  # Dummy class derived from Plant
tree = Tree()                            # A new instance of Tree class
print( isinstance(tree, Tree))             # True
print( isinstance(tree, Plant))            # True
print( isinstance(tree, object) )          # True
print( type(tree) is Tree    )             # False
print( type(tree).__name__ == "instance")  # True
print( tree.__class__.__name__ == "Tree" ) # True
```

Links:

- [Built-in Functions # isinstance](https://docs.python.org/2/library/functions.html#isinstance), python.org
- [isinstance() considered harmful](http://canonical.org/~kragen/isinstance/), canonical.org



## `issubclass`



Determines whether a class is a subclass of another class. Pertains to classes, not their instances.

```python
class Plant: pass                        # Dummy class
class Tree(Plant): pass                  # Dummy class derived from Plant
tree = Tree()                            # A new instance of Tree class
print( issubclass(Tree, Plant) )           # True
print( issubclass(Tree, object)   )        # False in Python 2
print( issubclass(int, object)   )         # True
print( issubclass(bool, int) )             # True
print( issubclass(int, int))               # True
print( issubclass(tree, Plant))            # Error - tree is not a class
```



## Duck typing

Duck typing provides an indirect means of **reflection**. It is a technique consisting in using an object as if it was of the requested type, while catching exceptions resulting from the object not supporting some of the features of the class or type.

Links:

- [Glossary # duck-typing](https://docs.python.org/3/glossary.html#term-duck-typing), python.org



## `callable`

For an object, determines whether it can be called. A class can be made callable by providing a `__call__()` method.

Examples:

- `callable(2)`
  - Returns False. Ditto for callable("Hello") and callable([1, 2]).
- `callable([1,2].pop)`
  - Returns True, as `pop` without "()" returns a function object.
- `callable([1,2].pop())`
  - Returns False, as `[1,2].pop()` returns 2 rather than a function object.

Links:

- [Built-in Functions # callable](https://docs.python.org/2/library/functions.html#callable), python.org





## `dir`



Returns the list of names of attributes of an object, which includes methods. Is somewhat heuristic and possibly incomplete, as per python.org.

Examples:

- `dir(3)`
- `dir("Hello")`
- `dir([1, 2])`
- `import re; dir(re)`
  - Lists names of functions and other objects available in the re module for regular expressions.

Links:

- [Built-in Functions # dir](https://docs.python.org/2/library/functions.html#dir), python.org



## `getattr`



Returns the value of an attribute of an object, given the attribute name passed as a string.

An example:

- `getattr(3, "imag")`

The list of attributes of an object can be obtained using [#Dir](https://en.wikibooks.org/wiki/Python_Programming/Reflection#Dir).

Links:

- [Built-in Functions # getattr](https://docs.python.org/2/library/functions.html#getattr), python.org



## Keywords

A list of Python keywords can be obtained from Python:

```python
import keyword
pykeywords = keyword.kwlist
print( keyword.iskeyword("if") )     # True
print( keyword.iskeyword("True"))    # False
```

Links:

- [32.6. keyword — Testing for Python keywords](https://docs.python.org/2/library/keyword.html), python.org



## Built-ins

A list of Python built-in objects and functions can be obtained from Python:

```python
print( dir(__builtins__) )          # Output the list
print( type(__builtins__.list))     # = <type 'type'>
print( type(__builtins__.open) )    # = <type 'builtin_function_or_method'>
print( list is __builtins__.list)   # True
print( open is __builtins__.open)   # True
```

Links:

- [28.3. `__builtin__` — Built-in objects](https://docs.python.org/2/library/__builtin__.html), python.org
- [Built-in Functions # dir](https://docs.python.org/2/library/functions.html#dir), python.org



## External links



- [2. Built-in Functions](http://docs.python.org/2/library/functions.html), docs.python.org
- [How to determine the variable type in Python?](http://stackoverflow.com/questions/402504/how-to-determine-the-variable-type-in-python), stackoverflow.com
- [Differences between isinstance() and type() in python](http://stackoverflow.com/questions/1549801/differences-between-isinstance-and-type-in-python), stackoverflow.com
- [W:Reflection (computer_programming)#Python](https://en.wikipedia.org/wiki/Reflection_(computer_programming)#Python), Wikipedia
- [W:Type introspection#Python](https://en.wikipedia.org/wiki/Type_introspection#Python), Wikipedia
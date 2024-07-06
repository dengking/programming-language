# Container datatypes

对python的container datatypes进行总结:

- [Built-in Types](https://docs.python.org/3/library/stdtypes.html#built-in-types) # Container datatypes
- [`collections`](https://docs.python.org/3/library/collections.html) — Container datatypes[¶](https://docs.python.org/3/library/collections.html#module-collections) 

## Dicts

### [Built-in Types](https://docs.python.org/3/library/stdtypes.html#built-in-types) # Mapping Types — [`dict`](https://docs.python.org/3/library/stdtypes.html#dict) 

> NOTE:
>
> 一. 相关内容:
>
> 1. [`defaultdict`](https://docs.python.org/3/library/collections.html#collections.defaultdict) objects[¶](https://docs.python.org/3/library/collections.html#defaultdict-objects) 

#### [**get**(*key*, *default=None*)](https://docs.python.org/3/library/stdtypes.html#dict.get) 

Return the value for *key* if *key* is in the dictionary, else *default*. If *default* is not given, it defaults to `None`, so that this method never raises a [`KeyError`](https://docs.python.org/3/library/exceptions.html#KeyError).

> NOTE:
>
> 一. 
>
> [stackoverflow-Why dict.get(key) instead of dict[key]?](https://stackoverflow.com/questions/11041405/why-dict-getkey-instead-of-dictkey)  
>
> [stackoverflow0Return a default value if a dictionary key is not available](https://stackoverflow.com/questions/6130768/return-a-default-value-if-a-dictionary-key-is-not-available) 



### [collections.defaultdict](https://docs.python.org/3/library/collections.html#collections.defaultdict) 

`default_factory`



### Python `dict` add key with defualt value if the key not exists

使用 [collections.defaultdict](https://docs.python.org/3/library/collections.html#collections.defaultdict)  



## Can python set be added new element while being iterated?

In Python, modifying a set (or any other collection) while iterating over it directly can lead to unexpected behavior or runtime errors. Specifically, adding elements to a set while iterating over it will raise a `RuntimeError` because the underlying data structure is being modified during iteration.

### Example of Direct Modification (Not Allowed)

```python
my_set = {1, 2, 3}

for item in my_set:
    if item == 2:
        my_set.add(4)  # This will raise a RuntimeError
```

### Workarounds

#### 1. Using a Separate Set to Collect New Elements

One common approach is to use a separate set to collect new elements and then update the original set after the iteration is complete.

```python
my_set = {1, 2, 3}
new_elements = set()

for item in my_set:
    if item == 2:
        new_elements.add(4)

my_set.update(new_elements)
print(my_set)  # Output: {1, 2, 3, 4}
```

#### 2. Using a List to Collect New Elements

You can also use a list to collect new elements and then update the set after the iteration.

```python
my_set = {1, 2, 3}
new_elements = []

for item in my_set:
    if item == 2:
        new_elements.append(4)

my_set.update(new_elements)
print(my_set)  # Output: {1, 2, 3, 4}
```

#### 3. Iterating Over a Copy of the Set

Another approach is to iterate over a copy of the set. This way, you can modify the original set without affecting the iteration.

```python
my_set = {1, 2, 3}

for item in my_set.copy():
    if item == 2:
        my_set.add(4)

print(my_set)  # Output: {1, 2, 3, 4}
```

### Summary

While you cannot directly add elements to a set during iteration, you can use workarounds such as collecting new elements in a separate set or list and updating the original set after the iteration, or iterating over a copy of the set. These methods allow you to achieve the desired behavior without encountering a `RuntimeError`.
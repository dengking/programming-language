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
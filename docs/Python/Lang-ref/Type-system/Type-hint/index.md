# Type hint

## Good resource

### mypy [Type hints cheat sheet (Python 3)](https://mypy.readthedocs.io/en/stable/cheat_sheet_py3.html)

```Python
x: Dict[str, float] = {'field': 2.0}
```

> NOTE: type hint for dictionary



```Python
# You don't need to initialize a variable to annotate it
a: int  # Ok (no value at runtime until assigned)

# The latter is useful in conditional branches
child: bool
if age < 18:
    child = True
else:
    child = False
```

> NOTE: 
>
> 可以不进行初始化了，后面在进行赋值也是可以的

## Type hint for variable

### stackoverflow [What are variable annotations?](https://stackoverflow.com/questions/39971929/what-are-variable-annotations)



```python
from typing import List
primes: List[int] = []
```



## [`typing`](https://docs.python.org/3/library/typing.html#module-typing) — Support for type hints[¶](https://docs.python.org/3/library/typing.html#module-typing)


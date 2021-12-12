# Functional programming



## docs Functional Programming HOWTO[¶](https://docs.python.org/3/howto/functional.html#functional-programming-howto)



## Functions are first class citizen

python中进行functional programming是非常容易的，因为python中functions are first class citizen。

### stackoverflow [How do I pass a method as a parameter in Python](https://stackoverflow.com/questions/706721/how-do-i-pass-a-method-as-a-parameter-in-python)

看了这些回答，这个具有启发意义：https://stackoverflow.com/a/706744 ；

functions (and methods) are first class objects in Python。



## FP apply

stackoverflow [Apply function to each element of a list](https://stackoverflow.com/questions/25082410/apply-function-to-each-element-of-a-list)



```python
>>> from string import upper
>>> mylis=['this is test', 'another test']
>>> map(upper, mylis)
['THIS IS TEST', 'ANOTHER TEST']
```

> NOTE: 
>
> 在Python3中，`map`函数所返回的是iterator，`map`函数并不会立即对列表的每个元素都执行`upper`函数



### 简单的封装

```Python
def apply(func, sequence):
    for i in sequence:
        func(i)
```

Python是duck typing的，因此封装是非常简单的。
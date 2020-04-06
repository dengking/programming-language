# Parameter and argument

## [What is the difference between arguments and parameters?](https://docs.python.org/3/faq/programming.html#id15)

[Parameters](https://docs.python.org/3/glossary.html#term-parameter) are defined by the names that appear in a function definition, whereas [arguments](https://docs.python.org/3/glossary.html#term-argument) are the values actually passed to a function when calling it. Parameters define what types of arguments a function can accept. For example, given the function definition:

```python
def func(foo, bar=None, **kwargs):
    pass
```

*foo*, *bar* and *kwargs* are **parameters** of `func`. However, when calling `func`, for example:

```python
func(42, bar=314, extra=somevar)
```

the values `42`, `314`, and `somevar` are arguments.

```c++
public void MyMethod(string myParam) { }

...

string myArg1 = "this is my argument";
myClass.MyMethod(myArg1);
```
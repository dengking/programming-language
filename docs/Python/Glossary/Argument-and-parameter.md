# [What is the difference between arguments and parameters?](https://docs.python.org/3/faq/programming.html#id15)

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



# [What's the difference between an argument and a parameter?](https://stackoverflow.com/questions/156767/whats-the-difference-between-an-argument-and-a-parameter)

[A](https://stackoverflow.com/a/156787)

A parameter is a variable in a method definition. When a method is called, the arguments are the data you pass into the method's parameters.

```c++
public void MyMethod(string myParam) { }

...

string myArg1 = "this is my argument";
myClass.MyMethod(myArg1);
```
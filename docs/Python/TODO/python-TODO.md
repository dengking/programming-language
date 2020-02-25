[TOC]

# python 多继承

多继承的时候，子类`__init__`函数如何来调用基类的`__init__`函数

# python lazy-loading module

在GitHub python-in-action中已经创建了源代码，在本地的`programming-language-python/pattern/lazy_import/`中，还有一些理论知识需要学习；




# python `dir()`

https://docs.python.org/3/library/functions.html#dir


# What is `__future__` in Python used for and how/when to use it, and how it works

https://stackoverflow.com/questions/7075082/what-is-future-in-python-used-for-and-how-when-to-use-it-and-how-it-works


https://www.liaoxuefeng.com/wiki/001374738125095c955c1e6d8bb493182103fac9270762a000/001386820023084e5263fe54fde4e4e8616597058cc4ba1000



# python `with`

- python `with` and scope

以下面的代码为例：
```python
x = tf.constant([[37.0, -23.0], [1.0, 4.0]])
w = tf.Variable(tf.random_uniform([2, 2]))
y = tf.matmul(x, w)
output = tf.nn.softmax(y)
init_op = w.initializer

with tf.Session() as sess:
  # Run the initializer on `w`.
  sess.run(init_op)

  # Evaluate `output`. `sess.run(output)` will return a NumPy array containing
  # the result of the computation.
  print(sess.run(output))

  # Evaluate `y` and `output`. Note that `y` will only be computed once, and its
  # result used both to return `y_val` and as an input to the `tf.nn.softmax()`
  # op. Both `y_val` and `output_val` will be NumPy arrays.
  y_val, output_val = sess.run([y, output])
```
其实在`with`的外层是可以访问在`with`中定义的变量的，但是在`c++`中，比如是`for`，则在`for`的外层是无法访问到在`for`内部定义的变量的；


# 增强系统的可扩展性

[tf.register_tensor_conversion_function](https://www.tensorflow.org/api_docs/python/tf/register_tensor_conversion_function)

记得在`py-mysql`中也有类似的函数。

# `c++` named requirement and python protocol


# 对比python的language reference和`c++`的language reference

- python expression和`c++` expression
- python statement and `c++` expression

# iterator generator

# iterable, awaitable


# python class attribute and instance attribute and attribute reference

```
In [11]: class Test:
    ...:     t=1
    ...:     def __init__(self,t=None):
    ...:         print(self.t)
    ...:         print(id(self.t))
    ...:         self.t=t if t else self.t
    ...:         print(self.t)
    ...:         print(id(self.t))
    ...:         
    ...:         

In [12]: t=Test(4)
1
139713319475648
4
139713319475744

In [13]: t=Test(4)
1
139713319475648
4
139713319475744

In [9]: t.t
Out[9]: 4

In [10]: Test.t
Out[10]: 1
```

上述例子体现了python中的attribute reference 和 class attribute 和 instance attribute之间的关系；






# python exception

# python 中`yield`的准确含义，到底是**产生**，还是**让渡**

# python dict
## python combine two dict
https://stackoverflow.com/questions/38987/how-to-merge-two-dictionaries-in-a-single-expression

## python assign the same value to a dict
https://stackoverflow.com/questions/2974022/is-it-possible-to-assign-the-same-value-to-multiple-keys-in-a-dict-object-at-onc

# python use member variable as default argument of member function
https://nikos7am.com/posts/mutable-default-arguments/



# python cache


# python 构造list的方法对比

`list(1)`会抛出异常`TypeError: 'int' object is not iterable`，但是`[1]`却能够正常；如果没有记错的话，第二种方法叫做display;

# python 中的创建文件的时候的权限字段为什么是`0o`即八进制呢？

# getpass.getuser()文档中的一些内容

https://docs.python.org/3/library/getpass.html

# os.makedirs

https://docs.python.org/3/library/os.html#os.makedirs

https://stackoverflow.com/questions/5210778/elegant-way-to-make-all-dirs-in-a-path

# automated testing

python如何实现automated testing

# python-determine-the-size-of-object
https://stackoverflow.com/questions/449560/how-do-i-determine-the-size-of-an-object-in-python


# 
keyword-only arguments, chained exceptions, yield from, and enums.


# six and 2to3


# DEFAULT ARGUMENTS

SMARTER DEFAULT ARGUMENTS (PYTHON RECIPE)

http://code.activestate.com/recipes/577786-smarter-default-arguments/


# LEGB

# Understanding UnboundLocalError in Python

https://eli.thegreenplace.net/2011/05/15/understanding-unboundlocalerror-in-python



# [What are metaclasses in Python?](https://stackoverflow.com/questions/100003/what-are-metaclasses-in-python)

# python `object` 和 `type`

```
class T:
    pass
```

```
T.__bases__
Out[22]: (object,)
```

```
T.__class__
Out[14]: type
```

```
i=T()

i.__class__
Out[16]: __main__.T
```


# debug in python

https://stackoverflow.com/questions/16867347/step-by-step-debugging-with-ipython

本地已经收录了IPython-debug

<https://github.com/cool-RR/PySnooper>

# python reflection



https://www.bnmetrics.com/blog/introspection-reflection-in-python

https://www.geeksforgeeks.org/reflection-in-python/

https://docs.python.org/3.7/c-api/reflection.html



# python `__builtin__`



这涉及到了：

- python 2中的`__builtin__`模块
- `dir()`返回的成员中包含有`__builtins__`

在这篇文章中给出了解答

[Where is the `__builtin__` module in Python3? Why was it renamed?](https://stackoverflow.com/questions/9047745/where-is-the-builtin-module-in-python3-why-was-it-renamed)

# class的scope

```python
class PredictionField:
    PREDICT_TIME = 'predict_time'
    PREDICT_VALUE = 'predict_value'
    PREDICT_PROB_RISE = 'predict_prob_rise'
    PREDICT_PROB_FALL = 'predict_prob_fall'
    PREDICT_PROB_FLAT = 'predict_prob_flat'

    ALL_FIELDS = [ getattr(PredictionField,x) for x in dir(PredictionField) if not x.startswith("__")]
```

报错如下：
```python
<ipython-input-1-edae3ab18252> in PredictionField()
      1 class PredictionField:
      2     PREDICT_TIME = 'predict_time'
----> 3     ALL_FIELDS = tuple(getattr(PredictionField, x) for x in dir(PredictionField) if x.startswith("PREDICT"))
      4 

NameError: name 'PredictionField' is not defined
```

# Python Programming/Metaclasses

<https://en.wikibooks.org/wiki/Python_Programming/Metaclasses>

# python `tuple`

```python
tuple(1,2,3)
---------------------------------------------------------------------------
TypeError                                 Traceback (most recent call last)
<ipython-input-24-3e233cfc2777> in <module>()
----> 1 tuple(1,2,3)

TypeError: tuple() takes at most 1 argument (3 given)
```



## [Why should you use namedtuple instead of a tuple?](https://pythontips.com/2015/06/06/why-should-you-use-namedtuple-instead-of-a-tuple/)




# python execution model and variable and scope

```
def outer():
    a = 9
    def inner():
        print(a)
    inner()
    print(a)
```

```
def outer():
    a = 9
    def inner():

        a = 10
        print(a)
    inner()
    print(a)
```

```
def outer():
    a = 9
    def inner():
        print(a)
        a = 10
    inner()
    print(a)
```

```
def outer():
    def inner():
        a = 10
    print(a)
```

思考一个问题：python解释器在编译函数的时候，是否会将该函数的所有local variable都保存到该函数的namespace中？


# python setInterval

python setinterval equivalent

celery中的Periodic Tasks[¶](http://docs.celeryproject.org/en/latest/userguide/periodic-tasks.html#periodic-tasks) 也是一种setinterval

# python ajax equivalent

ajax本质来说是一种异步，是一种promise；python中应该有


# ajax and promise


# python classmethod property


# 静态成员变量的容器

```python
class PredictionField:
    PREDICT_TIME = 'predict_time'
    PREDICT_VALUE = 'predict_value'
    PREDICT_PROB_RISE = 'predict_prob_rise'
    PREDICT_PROB_FALL = 'predict_prob_fall'
    PREDICT_PROB_FLAT = 'predict_prob_flat'

    ALL_FIELDS = tuple(getattr(PredictionField, x) for x in dir(PredictionField) if x.startswith("PREDICT"))

```
上述代码是存在错误的，但是我想要实现类似上述的效果；

回忆起之前使用SQLalchemy的经验，SQLalchemy中在定义表格的时候，肯定会有类似于上面的需求，即获取所有的类成员变量；


# json.dumps vs flask.jsonify

https://stackoverflow.com/questions/7907596/json-dumps-vs-flask-jsonify

http://flask.pocoo.org/docs/1.0/api/#module-flask.json


# python interpreter是如何了来compile function的？

函数的local variable是否会保存到function的namespace中？

# python的dynamic 

需要总结python的dynamic特性

google **python dynamic**

https://stackoverflow.com/a/2829588

https://pythonconquerstheuniverse.wordpress.com/2009/10/03/static-vs-dynamic-typing-of-programming-languages/

https://wiki.python.org/moin/Why%20is%20Python%20a%20dynamic%20language%20and%20also%20a%20strongly%20typed%20language

## 一个小例子

```python
def test():
    def p():
        print(a)
    a = 1
    p()
```

# rebind argument in function body
```python
    def extract_data(quote_df, data_window):
        if quote_df.shape[0] > data_window:
            quote_df = quote_df.tail(data_window)
        return quote_df
```

# [`functools`](https://docs.python.org/3/library/functools.html#module-functools) — Higher-order functions and operations on callable objects[¶](https://docs.python.org/3/library/functools.html#module-functools)



# [Redis in python, how do you close the connection?](https://stackoverflow.com/questions/24875806/redis-in-python-how-do-you-close-the-connection)





# [How to flatten nested python dictionaries?](https://stackoverflow.com/questions/39135433/how-to-flatten-nested-python-dictionaries)

[Aggregate Python lists stored as values in a nested dictionary into one list for arbitrary levels [duplicate\]](https://stackoverflow.com/questions/51426716/aggregate-python-lists-stored-as-values-in-a-nested-dictionary-into-one-list-for)





# python type system

[**duck-typing**](https://docs.python.org/3.7/glossary.html#term-duck-typing)



既然python是duck-type，那么为什么python还要提供[ABC](https://docs.python.org/3.7/glossary.html#term-abstract-base-class)呢？



在 [**abstract base class**](https://docs.python.org/3.7/glossary.html#term-abstract-base-class)已经将这个问题说清了



<https://docs.python.org/3.7/library/collections.abc.html>



## 20190429

python的动态特性所带来的问题：比如在编写一个函数的时候，由于python lack declaration，所以我们压根就不知道入参的类型是什么，只有到运行的时候，我们才能够知道入参的准确类型，才能够对其进行准确的运用，而在programming阶段，对其完全都是按照心中的预期进行programming的，所以python程序必须要进行完整的测试；

并且python的这种动态特性，带来了大量的类似下面这样的代码：

```python
    @classmethod
    def read(cls, redis_conn, ml_model_sub_name, stock_market, reverse=True, fields=None, data_range=None, window=None,
             predict_time_as_index=True):
        """

        :param redis_conn:
        :param ml_model_sub_name:
        :param stock_market:
        :param reverse 参见`class RedisStructureIOBase` doc
        :param fields 参见`class PredictionField`
        :param data_range 参见`class RedisStructureIOBase` doc
        :param window 参见`class RedisStructureIOBase` doc
        :param predict_time_as_index:将predict_time字段作为索引
        :return:
        """
        data_range = cls.convert_data_range(reverse=reverse, data_range=data_range, window=window)
        if fields is None:
            fields = [PredictionField.PREDICT_VALUE]
        if PredictionField.PREDICT_TIME in fields:
            fields = list(fields)
            fields.remove(PredictionField.PREDICT_TIME)
```

重点看最后两行，我的目的是如果`fields`中包含`PredictionField.PREDICT_TIME`，则就需要将其remove掉，但是如果用户传入的是`tuple`，则必须要先将其转换为`list`，然后才能够调用remove，因为tuple 是unmutable的，故其没有`remove`方法；

显然，如果在静态语言的话，则当我们在函数声明中，指定了入参的类型，则就不会出现这样的事情了；

由此可见，动态也有动态的弊端。



# python debugger

[`pdb`](https://docs.python.org/3/library/pdb.html#module-pdb) — The Python Debugger[¶](https://docs.python.org/3/library/pdb.html#module-pdb)

[How to execute multi-line statements within Python's own debugger (PDB)](https://stackoverflow.com/questions/5967241/how-to-execute-multi-line-statements-within-pythons-own-debugger-pdb)

# python build_in `format` `input`
https://docs.python.org/3/library/functions.html#input





# operation with transaction

在向database中加入批量数据的时候，有时候是需要实现transaction的，即如果在添加过程中出现了错误，在需要rollback；只有确保无误后才能够commit；

# **build_in** `print`, `repr` and special method `__str__` and `__repr__`

[How to print objects of class using print()?](https://stackoverflow.com/questions/1535327/how-to-print-objects-of-class-using-print)


# `list().append(stock_market)`

为什么上述代码没有返回一个`list`？



# python `iter` and `getitem`

今天在使用`to_list`功能的时候碰到了这样的一个问题：

```
In [14]: to_list(QuoteTable.Price)
---------------------------------------------------------------------------
NotImplementedError                       Traceback (most recent call last)
<ipython-input-14-b4f3918eec14> in <module>()
----> 1 to_list(QuoteTable.Price)

/home/quotepredict/workspace/AIServer/data_util/type_conversion.py in to_list(original)
     23             l.append(original)
     24         else:
---> 25             l.extend(item for item in original)
     26     except TypeError as e:
     27         l.append(original)

/home/quotepredict/workspace/AIServer/data_util/type_conversion.py in <genexpr>(.0)
     23             l.append(original)
     24         else:
---> 25             l.extend(item for item in original)
     26     except TypeError as e:
     27         l.append(original)

/usr/anaconda3/lib/python3.6/site-packages/sqlalchemy/sql/operators.py in __getitem__(self, index)
    409 
    410         """
--> 411         return self.operate(getitem, index)
    412 
    413     def __lshift__(self, other):

/usr/anaconda3/lib/python3.6/site-packages/sqlalchemy/orm/attributes.py in operate(self, op, *other, **kwargs)
    178 
    179     def operate(self, op, *other, **kwargs):
--> 180         return op(self.comparator, *other, **kwargs)
    181 
    182     def reverse_operate(self, op, other, **kwargs):

/usr/anaconda3/lib/python3.6/site-packages/sqlalchemy/sql/operators.py in __getitem__(self, index)
    409 
    410         """
--> 411         return self.operate(getitem, index)
    412 
    413     def __lshift__(self, other):

/usr/anaconda3/lib/python3.6/site-packages/sqlalchemy/orm/properties.py in operate(self, op, *other, **kwargs)
    268 
    269         def operate(self, op, *other, **kwargs):
--> 270             return op(self.__clause_element__(), *other, **kwargs)
    271 
    272         def reverse_operate(self, op, other, **kwargs):

/usr/anaconda3/lib/python3.6/site-packages/sqlalchemy/sql/operators.py in __getitem__(self, index)
    409 
    410         """
--> 411         return self.operate(getitem, index)
    412 
    413     def __lshift__(self, other):

/usr/anaconda3/lib/python3.6/site-packages/sqlalchemy/sql/elements.py in operate(self, op, *other, **kwargs)
    690 
    691     def operate(self, op, *other, **kwargs):
--> 692         return op(self.comparator, *other, **kwargs)
    693 
    694     def reverse_operate(self, op, other, **kwargs):

/usr/anaconda3/lib/python3.6/site-packages/sqlalchemy/sql/operators.py in __getitem__(self, index)
    409 
    410         """
--> 411         return self.operate(getitem, index)
    412 
    413     def __lshift__(self, other):

<string> in <lambda>(self, op, *other, **kwargs)

/usr/anaconda3/lib/python3.6/site-packages/sqlalchemy/sql/type_api.py in operate(self, default_comparator, op, *other, **kwargs)
     61         def operate(self, default_comparator, op, *other, **kwargs):
     62             o = default_comparator.operator_lookup[op.__name__]
---> 63             return o[0](self.expr, op, *(other + o[1:]), **kwargs)
     64 
     65         @util.dependencies('sqlalchemy.sql.default_comparator')

/usr/anaconda3/lib/python3.6/site-packages/sqlalchemy/sql/default_comparator.py in _getitem_impl(expr, op, other, **kw)
    190         return _binary_operate(expr, op, other, **kw)
    191     else:
--> 192         _unsupported_impl(expr, op, other, **kw)
    193 
    194 

/usr/anaconda3/lib/python3.6/site-packages/sqlalchemy/sql/default_comparator.py in _unsupported_impl(expr, op, *arg, **kw)
    195 def _unsupported_impl(expr, op, *arg, **kw):
    196     raise NotImplementedError("Operator '%s' is not supported on "
--> 197                               "this expression" % op.__name__)
    198 
    199 

NotImplementedError: Operator 'getitem' is not supported on this expression
```

`QuoteTable.Price`是可以`iter` 的，但是当将其置于`for in `中时，它就跑出来上述异常；


# python中的并发方式

**gunicorn**的文档中的并发方式的总结是非常好的，它同样适合于celery

http://docs.gunicorn.org/en/stable/design.html#


# How to make a flat list out of list of lists?

https://stackoverflow.com/questions/952914/how-to-make-a-flat-list-out-of-list-of-lists

# value type of a dict is list raise error

# Python: Find in list

https://stackoverflow.com/questions/9542738/python-find-in-list

# for语句的执行

```
In [9]: for index, word in enumerate([]):
   ...:     print(index)
   ...:     

In [10]: index
---------------------------------------------------------------------------
NameError                                 Traceback (most recent call last)
<ipython-input-10-caf7e7bf96a7> in <module>()
----> 1 index

NameError: name 'index' is not defined
```


# python str to float

https://stackoverflow.com/questions/40097590/detect-whether-a-python-string-is-a-number-or-a-letter

https://stackoverflow.com/questions/354038/how-do-i-check-if-a-string-is-a-number-float

https://stackoverflow.com/questions/354038/how-do-i-check-if-a-string-is-a-number-float


# for and 三元表达式

```
fen_zi_word_list='abc'

similar_words={'a':'1', 'b':'2', 'c':'3'}

''.join(similar_words[word] if word in similar_words else word for word in fen_zi_word_list)
Out[9]: '123'
```

# python iterable and iteration

python中的支持算法的build-in function，都是基于iterable的，所以搞清楚它是非常主要的；

https://stackoverflow.com/questions/19151/build-a-basic-python-iterator

https://www.programiz.com/python-programming/iterator

https://docs.python.org/3/reference/datamodel.html#object.__iter__

https://diveintopython3.net/iterators.html

# iteration and deletion
https://thispointer.com/python-different-ways-to-iterate-over-a-list-in-reverse-order/

其实这种算法应该大多数是不好的，一遍遍历，还一边进行删除，这会导致一定的问题，应该尽可能地将循环的判断条件转换一下

# python min and max of integer

https://stackoverflow.com/questions/7604966/maximum-and-minimum-values-for-ints

https://stackoverflow.com/questions/7604966/maximum-and-minimum-values-for-ints
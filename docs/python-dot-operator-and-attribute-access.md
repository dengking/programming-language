```python
class TestBase:
    Name='TestBase'
    def __init__(self):
        self.name='testbase'

class T1(TestBase):
    pass


class T2(TestBase):
    pass
```



```python
t1=T1()
t2=T2()

id(t1.Name)==id(t2.Name)
```



```python
class TestBase:
    Name='TestBase'
    @classmethod
    def ChangeName(cls, name):
        cls.Name = name
    def __init__(self):
        self.name='testbase'
     
class T1(TestBase):
    pass

class T2(TestBase):
    pass
```



```
t1.ChangeName('T1')
id(t1.Name)==id(t2.Name)
```

python `=`operator

python `.` operator

如:

```python
t1.test='a'
```

这段代码是能够正确运行的，它在`t1`中添加了`tests`属性。

```python
t1.Name
```

则是纯粹的属性access。
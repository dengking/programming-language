[TOC]

# 前言

想要把python玩得高级，需要能够对python进行高度的customization，主要包括如下方面

# customize class creation

python中类定义语句是一个可执行语句，所以当导入一个包含了有类定义的语句的module的时候，就会触发python解释器来执行这个类的定义，所以此时就会触发一些与类创建有关的hook方法的执行，关于这些方法，在下面的文章中有非常好的解释：

- [Customizing class creation in Python](https://snarky.ca/customizing-class-creation-in-python/)

在python doc的[3.3.3. Customizing class creation](https://docs.python.org/3/reference/datamodel.html#customizing-class-creation)中描述了自定义类的创建；其中提及：

> By default, classes are constructed using [type()](https://docs.python.org/3/library/functions.html#type). The class body is executed in a new namespace and the class name is bound locally to the result of type(name, bases, namespace).

这就触发了我的一个想法是，我是否自己来控制何时使用`type`来创建类，并且传入一个参数值；
# customize module load

这是我在阅读celery的源代码的时候发现的，celery的`find_app`函数中就有一个分支是自己来加载定义有`app`的python文件 ；
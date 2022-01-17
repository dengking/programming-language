# swig pointer

存在着两种可能性:

1、入参类型是pointer

比如在Python中，将一个list传入到其中

2、出参类型是pointer

函数返回一个list，这list是指定长度的



## swig document中包含pointer描述的章节

5.3 Pointers and complex objects

5.4.9 Pointers to functions and callbacks

6.10 References and pointers

6.13 A brief discussion of multiple inheritance, pointers, and type checking

6.23 Pointers to Members

6.24 Smart pointers and operator->()

11.2 C arrays and pointers

## swig typemap void pointer

## stackoverflow [How to handle void pointers in SWIG director methods in Python](https://stackoverflow.com/questions/48132910/how-to-handle-void-pointers-in-swig-director-methods-in-python)





## py4u [Swig, returning an array of doubles](https://www.py4u.net/discuss/196905)

> NOTE: 
>
> 内容也是不错的

## py4u [How to use Swig to treat a void * function parameter as Python string](https://www.py4u.net/discuss/217653)



## stackoverflow [Accessing void pointers in Python (using SWIG or something else)](https://stackoverflow.com/questions/2345354/accessing-void-pointers-in-python-using-swig-or-something-else)

### [A](https://stackoverflow.com/a/10634684/10173843)

> NOTE: 
>
> 用 'cdata.i'

## stackoverflow [return double * from swig as python list](https://stackoverflow.com/questions/14033387/return-double-from-swig-as-python-list)

### [A](https://stackoverflow.com/a/14080586/10173843)

> NOTE: 
>
> 非常好的例子，典型的描述 typemap



## stackoverflow [Wrapping a void * argument in Python SWIG](https://stackoverflow.com/questions/35124093/wrapping-a-void-argument-in-python-swig)



## stackoverflow [How to handle void pointers in SWIG director methods in Python](https://stackoverflow.com/questions/48132910/how-to-handle-void-pointers-in-swig-director-methods-in-python)
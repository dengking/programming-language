# debug Python

## 通过gdb来调试

### 通过attach 来实现

在ipython中以交互式的方式来运行。



### 直接调试

```C++
gdb --args python signal_test.py
```

TODO

1、[Debugging: stepping through Python script using gdb?](https://stackoverflow.com/questions/7412708/debugging-stepping-through-python-script-using-gdb) # [A](https://stackoverflow.com/a/7920256/10173843)

奇技淫巧

2、python [DebuggingWithGdb](https://wiki.python.org/moin/DebuggingWithGdb)

### Debugging Python C extensions with GDB

其实可以按照调试shared library的方式来实现

redhat [Debugging Python C extensions with GDB](https://developers.redhat.com/articles/2021/09/08/debugging-python-c-extensions-gdb#)



## switowski [5 Ways of Debugging with IPython](https://switowski.com/blog/ipython-debugging)

> NOTE: 
>
> 这篇文章

### Embedding IPython session in the code

> NOTE: 
>
> 将ipython进行嵌套的实现

### Putting a breakpoint in your code

> NOTE: 
>
> 介绍了ipdb、

### %run -d filename.py

> NOTE: 
>
> 


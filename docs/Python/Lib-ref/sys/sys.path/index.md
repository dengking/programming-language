# `sys.path`



## stackoverflow [Permanently adding a file path to sys.path in Python](https://stackoverflow.com/questions/12257747/permanently-adding-a-file-path-to-sys-path-in-python)



```Python
import sys
sys.path
sys.path.append('/path/to/the/example_file.py')
print(sys.path)
```



### [A](https://stackoverflow.com/a/12257807/10173843)

There are a few ways:

1、One of the simplest is to create a `my-paths.pth` file (as described [here](http://docs.python.org/library/site.html)). This is just a file with the extension `.pth` that you put into your system `site-packages` directory. On each line of the file you put one directory name, so you can put a line in there with `/path/to/the/` and it will add that directory to the path.

> NOTE:
>
> 一、这种方式在 stackoverflow [Adding folder to Python's path permanently](https://stackoverflow.com/questions/3722248/adding-folder-to-pythons-path-permanently) # [A](https://stackoverflow.com/a/3722397/10173843) 中也进行了讨论

2、You could also use the PYTHONPATH environment variable, which is like the system PATH variable but contains directories that will be added to `sys.path`. See [the documentation](http://docs.python.org/tutorial/modules.html#the-module-search-path).

3、Note that no matter what you do, `sys.path` contains *directories* not files. You can't "add a file to `sys.path`". You always add its directory and then you can import the file.

> NOTE:
>
> ```Python
> import sys
> sys.path
> sys.path.append('/path/to/the/dictionary')
> ```
>
> 






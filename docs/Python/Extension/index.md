# Python extension



## import dynamic library

docs.python [How do I make an executable from a Python script?](https://docs.python.org/3/faq/windows.html#id5)[¶](https://docs.python.org/3/faq/windows.html#how-do-i-make-an-executable-from-a-python-script)



### Linux 

可以直接import `*.so`

### Windows

windows下可以直接import `*.pyd`，无法直接import `*.dll`，关于此，在"PYD VS DLL"中进行了介绍。





stackoverflow [Using SWIG wrapped functions from Windows .dll](https://stackoverflow.com/questions/8979869/using-swig-wrapped-functions-from-windows-dll) # [A](https://stackoverflow.com/a/38258050/10173843)

> So the final short answer appears to be that the C++ build should produce `_my_dll.pyd` (not `_my_dll.dll`) in order to be able to import `my_dll` in Python on Windows. This is in contrast to the Linux build, where one would build `_my_dll.so`
>
> Simply renaming .dll file to .pyd works, but it is better to build it, as pointed out in comments above.

#### PYD VS DLL



stackoverflow [Python C extension: Use extension PYD or DLL?](https://stackoverflow.com/questions/8262884/python-c-extension-use-extension-pyd-or-dll)



docs.python [Is a `*.pyd` file the same as a DLL?](https://docs.python.org/3/faq/windows.html#id6)

#### Load dll

stackoverflow [How can I use a DLL file from Python?](https://stackoverflow.com/questions/252417/how-can-i-use-a-dll-file-from-python) 
# ctypes

## [`ctypes`](https://docs.python.org/3/library/ctypes.html#module-ctypes) — A foreign function library for Python

> NOTE: 
>
> 原文主要分为两部分:
>
> 1、ctypes tutorial[¶](https://docs.python.org/3/library/ctypes.html#ctypes-tutorial)
>
> 2、ctypes reference[¶](https://docs.python.org/3/library/ctypes.html#ctypes-reference)



### ctypes tutorial[¶](https://docs.python.org/3/library/ctypes.html#ctypes-tutorial)



`None`, integers, bytes objects and (unicode) strings are the only native Python objects that can directly be used as parameters in these function calls. `None` is passed as a C `NULL` pointer, bytes objects and strings are passed as pointer to the memory block that contains their data (`char*` or `wchar_t*`). Python integers are passed as the platforms default C `int` type, their value is masked to fit into the C type.

> NOTE: 
>
> 一、Python C type map
>
> 这段话非常重要

#### Fundamental data types[¶](https://docs.python.org/3/library/ctypes.html#fundamental-data-types)

> NOTE: 
>
> 一、Python C type map

Assigning a new value to instances of the pointer types [`c_char_p`](https://docs.python.org/3/library/ctypes.html#ctypes.c_char_p), [`c_wchar_p`](https://docs.python.org/3/library/ctypes.html#ctypes.c_wchar_p), and [`c_void_p`](https://docs.python.org/3/library/ctypes.html#ctypes.c_void_p) changes the *memory location* they point to, *not the contents* of the memory block (of course not, because **Python bytes objects** are **immutable**):



### ctypes reference[¶](https://docs.python.org/3/library/ctypes.html#ctypes-reference)











## pass byte array to C++ function that accept a void pointer



### stackoverflow [SWIG: Pass Python String to argument of type void pointer](https://stackoverflow.com/questions/41131554/swig-pass-python-string-to-argument-of-type-void-pointer)



## Python将指针转换为地址值

stackoverflow [How to get the pointer address of a ctypes.c_char_p instance](https://stackoverflow.com/questions/32234970/how-to-get-the-pointer-address-of-a-ctypes-c-char-p-instance)

[A](https://stackoverflow.com/a/32235051/10173843)

You could just cast it to `c_void_p` and get the value:

```python
>>> ctypes.cast(s, ctypes.c_void_p).value
4333430692
```

comment:

Alternatively, use the buffer protocol: `ctypes.c_void_p.from_buffer(s).value`. Using `from_buffer` is about 3 times faster since `cast` is implemented using an FFI call. But for the difference to matter you'd have to be doing this millions of times in a tight loop that does nothing else. 

– [Eryk Sun](https://stackoverflow.com/users/205580/eryk-sun)

## Python将地址值转化为pointer

stackoverflow [ctypes: construct pointer from arbitrary integer](https://stackoverflow.com/questions/5783761/ctypes-construct-pointer-from-arbitrary-integer)

[A](https://stackoverflow.com/a/5784284/10173843)

You can use `ctypes.cast(addr, type)`. I'll extend your example to acquire an address via a known object, to demonstrate:

```python
INTP = ctypes.POINTER(ctypes.c_int)
num = ctypes.c_int(42)
addr = ctypes.addressof(num)
print 'address:', addr, type(addr)
ptr = ctypes.cast(addr, INTP)
print 'pointer:', ptr
print 'value:', ptr[0]
```

Output:

```python
address: 4301122528 <type 'int'>
pointer: <__main__.LP_c_int object at 0x1005decb0>
value: 42
```



## 从指定地址读取数据



### stackoverflow [How to read from pointer address in Python?](https://stackoverflow.com/questions/48808997/how-to-read-from-pointer-address-in-python)



[A](https://stackoverflow.com/a/48809155/10173843)

```Python
import ctypes
g = (ctypes.c_char*40000).from_address(0x561124456)
```



[A](https://stackoverflow.com/a/51200615/10173843)



```C++
import ctypes
import binascii
import numpy as np

myCfunslib.getData.restype = ctypes.c_void_p
#myCfunslib.getData.restype=ctypes.POINTER(ctypes.c_ubyte)#no need to cast
dataSize = 1092 * 1208
#call the c function to get the data memory pointer
cMemoryPointer = myCfunslib.getData();
newpnt = ctypes.cast(cMemoryPointer, ctypes.POINTER(ctypes.c_ubyte))
# and construct an array using this data
DataBytes = np.ctypeslib.as_array(newpnt, (dataSize,)) #no internal copy
print "the mid byte of the data in python side is ", DataBytes[dataSize/2]
```

[A](https://stackoverflow.com/a/51107407/10173843)



### examplefiles [Ctypes: fast way to convert a return pointer to an array or Python list](https://www.examplefiles.net/cs/554597)



## Examples



### stackoverflow [dereferencing the whole Data of c_void_p not only the first byte](https://stackoverflow.com/questions/51981858/dereferencing-the-whole-data-of-c-void-p-not-only-the-first-byte)



### pointer and array

#### stackoverflow [Pointers and arrays in Python ctypes](https://stackoverflow.com/questions/1363163/pointers-and-arrays-in-python-ctypes)
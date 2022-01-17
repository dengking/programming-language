# ctypes



## Examples

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



### stackoverflow [dereferencing the whole Data of c_void_p not only the first byte](https://stackoverflow.com/questions/51981858/dereferencing-the-whole-data-of-c-void-p-not-only-the-first-byte)



## pass byte array to C++ function that accept a void pointer



### stackoverflow [SWIG: Pass Python String to argument of type void pointer](https://stackoverflow.com/questions/41131554/swig-pass-python-string-to-argument-of-type-void-pointer)



### stackoverflow [SWIG: Pass Python String to argument of type void pointer](https://stackoverflow.com/questions/41131554/swig-pass-python-string-to-argument-of-type-void-pointer)




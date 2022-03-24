# CMake AddressSanitizer

## MSVC



### microsoft [Using the AddressSanitizer from Visual Studio: CMake](https://docs.microsoft.com/en-us/cpp/sanitizers/asan?view=msvc-170#ide-cmake)



## XCode

### stackoverflow [What's the proper way to enable AddressSanitizer in CMake that works in Xcode](https://stackoverflow.com/questions/44320465/whats-the-proper-way-to-enable-addresssanitizer-in-cmake-that-works-in-xcode)



```cmake
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
```

> NOTE: 
>
> 上面这种方式经过验证是可行的

## Linux

### CSDN [AddressSanitizer+cmake在Linux平台下的使用](https://blog.csdn.net/I_ren/article/details/106348994)

```shell
-fsanitize=address
-fno-omit-frame-pointer     #打印函数调用路径
-fsanitize-recover=address  #AddressSanitizer遇到错误时能够继续
```



## Library



### github [arsenm](https://github.com/arsenm)/**[sanitizers-cmake](https://github.com/arsenm/sanitizers-cmake)**

> NOTE: 这是在 stackoverflow [What's the proper way to enable AddressSanitizer in CMake that works in Xcode](https://stackoverflow.com/questions/44320465/whats-the-proper-way-to-enable-addresssanitizer-in-cmake-that-works-in-xcode) 中提及的。


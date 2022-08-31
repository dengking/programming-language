# 修改SWIG library

SWIG library控制代码的生成，在一些特殊的情况下，可以通过修改SWIG library文件来生成预期的代码。

## IM elite

由于IM elite的header file不使用STL，而是使用的自研的 `ne_std` ， `ne_std` 中一些容器没有完全提供和STL中容器相同的接口，而swig生成的wrapper层代码是按照STL接口生成的， 这就导致了在将swig生成的wrapper层代码中的STL替换回`ne_std` 后，由于`ne_std` 缺少一些接口将导致生成的代码无法编译通过。可以按照这种方式进行修改: 修改swig中对应容器的库文件(用于控制生成的wrapper层代码)，将其中使用了 `ne_std` 没有提供的接口的函数替换为等价的接口。通过观察发现，目前这些函数在C#中并没有被使用，因此一种简便的方式是直接将这些函数体注释，通过抛出exception来提示它被调用了，下面是一个简单的example:

```C++
      void Reverse() {
        // std::reverse($self->begin(), $self->end());
         throw std::runtime_error("unsupported in ne_std");
      }
```

上述被注释的是原文件中的内容，下面的throw语句是新增的内容。

目前仅仅 `std::vector` 存在上述问题，因此只需要修改 `std_vector.i` 即可，由于本次只需要考虑C#语言，因此仅修改swig安装目录中的如下文件即可:

```
Lib\csharp\std_vector.i
```

修改后的文件位于  `unity\swig\swig-4.0.2\Lib\csharp\std_vector.i` ，读者可以直接用该文件替换自己电脑上的对应文件。
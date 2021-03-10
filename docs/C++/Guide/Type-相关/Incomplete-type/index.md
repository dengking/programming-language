# Incomplete type

本章对incomplete type进行总结，在C++ language中，incomplete type包括:

1、void pointer，参见 `Pointer-to-void` 章节

2、opaque pointer，参见 `Opaque-pointer` 章节

本章对incomplete type的一些问题进行探讨，包括:

1、如何检测incomplete type，参见 `Check-incomplete-type` 章节

2、delete incomplete type，参见 `Delete-incomplete-type` 章节

## 发展概述: C++和C中，可能无法兼容的地方

C是支持opaque pointer的，opaque pointer是典型的incomplete type，由于它是pointer，因此它所指向的object是dynamic allocation的，对于C而言，对于dynamic allocated的object，通过它的pointer是可以deallocation的，因此，使用opaque pointer是不存在问题的；

C++的目标之一是兼容C，因此C++也是允许使用opaque pointer，但是C++中有destructor的，如果delete incomplete type，可能会导致undefined behavior，这就是C++和C可能无法兼容的地方。

## cppreference [Type](https://en.cppreference.com/w/cpp/language/type) # [Incomplete type](https://en.cppreference.com/w/cpp/language/type#Incomplete_type)
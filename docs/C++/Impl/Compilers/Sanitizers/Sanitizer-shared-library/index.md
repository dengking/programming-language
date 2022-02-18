# Asan shared library

当使用Asan发现了shared library中的code出现问题的时候，需要依赖OS提供的shared library的工具来进行协助定位，在Linux OS，一般常用的是addr2line，对于shared library，在使用addr2line的时候需要特别的注意，在addr2line章节中进行了专门的说明。



## 素材

csdn [使用AddressSanitizer搭配addr2line查找C/C++内存泄漏问题](https://blog.csdn.net/albertsh/article/details/116868211)


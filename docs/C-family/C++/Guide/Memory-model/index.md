# 关于本章

本章讨论C++ memory model。



## 为什么C++11引入memory model？

参见 `Why-need-memory-model` 章节。

## C++11 memory model feature

抽象地描述/model了当前主流hardware:

1、multi-threaded by design(multicore)

2、has a well-defined *memory model*(memory reordering)

能够容纳大多数hardware的feature/差异，让programmer以portable、standard方式，充分发挥hardware power。

参考: 

1、stackoverflow [C++11 introduced a standardized memory model. What does it mean? And how is it going to affect C++ programming?](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g)



## Memory model and consistency model and memory ordering

需要注意的是: consistency model其实所讨论的就是memory的问题，因此一个programming language的memory model是需要定义它的consistency model的，它需要明确定义它的consistency。



## TODO



### wikipedia [C++11#Multithreading memory model](https://en.wikipedia.org/wiki/C%2B%2B11#Multithreading_memory_model)

C++11 standardizes support for [multithreaded programming](https://en.wikipedia.org/wiki/Thread_(computer_science))，所以相应的memory model也需要进行调整。



### github [cpplinks](https://github.com/MattPD/cpplinks)

[atomics.lockfree.memory_model.md](https://github.com/MattPD/cpplinks/blob/master/atomics.lockfree.memory_model.md)


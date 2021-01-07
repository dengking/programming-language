# 关于本章

本章讨论C++ memory model。

## 为什么C++11引入memory model？

1、时代背景: parallel computing

2、portable

3、充分发挥hardware power

参考: 

1、stackoverflow [C++11 introduced a standardized memory model. What does it mean? And how is it going to affect C++ programming?](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g)



## C++11 memory model feature

抽象地描述/model了当前主流hardware:

1、multi-threaded by design(multicore)

2、has a well-defined *memory model*(memory reordering)

能够容纳大多数hardware的feature/差异，让programmer以portable、standard方式，充分发挥hardware power。

参考: 

1、stackoverflow [C++11 introduced a standardized memory model. What does it mean? And how is it going to affect C++ programming?](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g)



## TODO



### github [cpplinks](https://github.com/MattPD/cpplinks)

[atomics.lockfree.memory_model.md](https://github.com/MattPD/cpplinks/blob/master/atomics.lockfree.memory_model.md)


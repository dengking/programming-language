# 关于本章

本章讨论C++ memory model。

## 为什么C++11引入memory model？

1、时代背景: parallel computing

对于single thread，其实memory model并没有什么意义；但是对于multi-thread，它有着非常重要的意义，参考:

1、[C++11 introduced a standardized memory model. What does it mean? And how is it going to affect C++ programming?](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g) # [A](https://stackoverflow.com/a/6319356) 的如下comment:

> @Nawaz: Yes! Memory accesses can get reordered by the compiler or CPU. Think about (e.g.) caches and speculative loads. The order in which system memory gets hit can be nothing like what you coded. The compiler and CPU will ensure such reorderings do not break *single-threaded* code. For multi-threaded code, the "memory model" characterizes the possible re-orderings, and what happens if two threads read/write the same location at the same time, and how you excert control over both. **For single-threaded code, the memory model is irrelevant**. – [Nemo](https://stackoverflow.com/users/768469/nemo) [Jun 12 '11 at 17:08](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g#comment7393384_6319356)

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



## Memory model and consistency model and memory ordering

需要注意的是: consistency model其实所讨论的就是memory的问题，因此一个programming language的memory model是需要定义它的consistency model的，它需要明确定义它的consistency。



## TODO



### github [cpplinks](https://github.com/MattPD/cpplinks)

[atomics.lockfree.memory_model.md](https://github.com/MattPD/cpplinks/blob/master/atomics.lockfree.memory_model.md)


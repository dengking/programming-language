# Memory model

## 维基百科[C++11#Multithreading memory model](https://en.wikipedia.org/wiki/C%2B%2B11#Multithreading_memory_model)

C++11 standardizes support for [multithreaded programming](https://en.wikipedia.org/wiki/Thread_(computer_science))，所以相应的memory model也需要进行调整。

## cppreference [Memory model](https://en.cppreference.com/w/cpp/language/memory_model)



## modernescpp [C++ Memory Model](http://www.modernescpp.com/index.php/c-memory-model)



## TODO

添加关于`std::vector<bool>`的描述，主要想表达的是memory model的分辨率是byte，而不是bit，所以它无法返回一个reference，原文reference是pointer，是地址。


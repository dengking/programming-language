# GC

在阅读cppreference [C++11](https://en.cppreference.com/w/cpp/11)时，无意中发现:

> Core language features
>
> - [GC interface](https://en.cppreference.com/w/cpp/memory#Garbage_collector_support)

原来C++ 11提供了"Garbage collector support"。在Wikipedia [C++11#Allow garbage collected implementations](https://infogalactic.com/info/C%2B%2B11#Allow_garbage_collected_implementations) 中有相关介绍:

> Prior C++ standards provided for **programmer-driven garbage collection** via `set_new_handler`, but gave no definition of **object reachability** for the purpose of **automatic garbage collection**. C++11 defines conditions under which pointer values are "safely derived" from other values. An implementation may specify that it operates under *strict pointer safety*, in which case pointers that are not derived according to these rules can become invalid.
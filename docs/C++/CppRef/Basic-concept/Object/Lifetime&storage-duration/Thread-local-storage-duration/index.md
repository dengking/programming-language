# Thread local storage duration

一、这是C++11引入的，源于"C++11 introduced a standardized memory model"，关于此，参见`C++\Language-reference\Basic-concept\Abstract-machine\Memory-model`。

对于对于object with thread storage duration相比于其它类型的object，它的initialization、deinitialization是比较复杂的，原文中有专门说明: 

> See [Non-local variables](https://en.cppreference.com/w/cpp/language/initialization#Non-local_variables) and [Static local variables](https://en.cppreference.com/w/cpp/language/storage_duration#Static_local_variables) for details on initialization of objects with this storage duration.

在前面的注解中，已经描述了我的整理思路。

二、Thread local storage duration基本上和static local storage duration的规则类似，cppreference中也将两者放到了一起进行描述。


# Static storage duration

1、对于object with static storage duration，相比于其它类型的object，它的initialization、deinitialization是比较复杂的，本章对object with static storage duration一些问题进行专门总结和介绍。

2、cppreference中，对于object with static storage duration的相关内容是比较分散的，在cppreference [Storage class specifiers](https://en.cppreference.com/w/cpp/language/storage_duration) 中的总结如下:

> See [Non-local variables](https://en.cppreference.com/w/cpp/language/initialization#Non-local_variables) and [Static local variables](https://en.cppreference.com/w/cpp/language/storage_duration#Static_local_variables) for details on initialization of objects with this storage duration.

本文将相关内容进行了整理。

3、cppreference [Initialization#Non-local variables](https://en.cppreference.com/w/cpp/language/initialization#Non-local_variables) 章节的内容是难以阅读的，所以本文会首先使用一篇非常容易理解的文章作为开头，从而对object with static storage duration有一个完整的认知。

## Lifetime of object with static storage duration

对于object with static storage duration，C++语言有着特殊的设计，根据object的scope，我们进一步将它们分为:

| classification                                | 描述章节              |
| --------------------------------------------- | --------------------- |
| Non-local object with static storage duration | `Non-local-object`    |
| Local object with static storage duration     | `Static-local-object` |


# 关于本章

在Python标准文档的[Data model](https://docs.python.org/3/reference/datamodel.html)中描述了python 语言的data model，受到python标准文档的启发，我觉得有必要为C++语言也创建data model；C++的data model是基于[Object](https://en.cppreference.com/w/cpp/language/object)的，它将是本章描述的内容。

阅读[cppreference.com](https://en.cppreference.com/)中关于C、C++的描述，可以发现object的是这两门语言中非常核心的概念，是完整地认知C、C++的基础。本文讨论C++中object的概念以及与object相关的各种内容，本章讨论的内容如下：

| 主题                                 | 概述                                                         | 章节                                                         |
| ------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| object                               | 主要讨论C++中object的概念                                    | `cppreference-object`                                        |
| object lifetime and storage duration | object lifetime与object storage是两个相互依存的概念，<br>在[Object](https://en.cppreference.com/w/cpp/language/object)中这样描述两者的关系：<br>[lifetime](https://en.cppreference.com/w/cpp/language/lifetime) (bounded by storage duration or temporary); <br/>object storage描述的是C++ object位于内存在的位置：stack、heap； | `C++\Language-reference\Basic-concept\Object\Lifetime-and-storage-duration` |
| object layout                        | 前面讨论了C++ object的storage，object layout则讨论的是object在内存中的布局 | `Object-layout`                                              |



